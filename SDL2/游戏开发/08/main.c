#include <SDL2/SDL.h>
#include <math.h>

#define MAX_POLY_VERTS 8

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    Vec2 vertices[MAX_POLY_VERTS];
    int count;
    Vec2 center; /* 中心点（用于旋转） */
    float angle; /* 旋转角度（弧度） */
} Polygon;

/* 向量运算 */
Vec2 Vec2_Sub(Vec2 a, Vec2 b) { return (Vec2){ a.x - b.x, a.y - b.y }; }
float Vec2_Dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
float Vec2_Length(Vec2 v) { return sqrtf(v.x * v.x + v.y * v.y); }
Vec2 Vec2_Normalize(Vec2 v) {
    float len = Vec2_Length(v);
    if (len < 0.0001f)
        return (Vec2){ 0, 0 };
    return (Vec2){ v.x / len, v.y / len };
}

/* 获取多边形边的法线 */
Vec2 GetEdgeNormal(Vec2 v1, Vec2 v2) {
    Vec2 edge = Vec2_Sub(v2, v1);
    /* 法线：旋转 90 度 */
    return (Vec2){ -edge.y, edge.x };
}

/* 计算多边形在轴上的投影 */
void ProjectPolygon(Polygon *poly, Vec2 axis, float *min, float *max) {
    *min = *max = Vec2_Dot(poly->vertices[0], axis);
    for (int i = 1; i < poly->count; i++) {
        float proj = Vec2_Dot(poly->vertices[i], axis);
        if (proj < *min)
            *min = proj;
        if (proj > *max)
            *max = proj;
    }
}

/* SAT 碰撞检测 */
typedef struct {
    SDL_bool collide;
    Vec2 mtv;      /* 最小分离向量 */
    float overlap; /* 重叠量 */
} CollisionResult;

CollisionResult SAT_Check(Polygon *a, Polygon *b) {
    CollisionResult result = { SDL_TRUE, { 0, 0 }, 1e30f };

    /* 检查两个多边形的所有边的法线 */
    for (int poly = 0; poly < 2; poly++) {
        Polygon *p = (poly == 0) ? a : b;
        for (int i = 0; i < p->count; i++) {
            int j = (i + 1) % p->count;
            Vec2 normal = GetEdgeNormal(p->vertices[i], p->vertices[j]);
            normal = Vec2_Normalize(normal);

            /* 投影 */
            float min_a, max_a, min_b, max_b;
            ProjectPolygon(a, normal, &min_a, &max_a);
            ProjectPolygon(b, normal, &min_b, &max_b);

            /* 检查重叠 */
            if (max_a < min_b || max_b < min_a) {
                /* 找到分离轴，不碰撞 */
                result.collide = SDL_FALSE;
                return result;
            }

            /* 计算重叠量 */
            float overlap = (max_a < max_b ? max_a : max_b) -
                            (min_a > min_b ? min_a : min_b);
            if (overlap < result.overlap) {
                result.overlap = overlap;
                result.mtv = normal;
                /* MTV 方向应从 A 指向 B */
                Vec2 d = Vec2_Sub(b->center, a->center);
                if (Vec2_Dot(d, normal) < 0) {
                    result.mtv.x = -normal.x;
                    result.mtv.y = -normal.y;
                }
            }
        }
    }
    return result;
}

/* 创建矩形多边形（带旋转） */
Polygon MakeRect(float cx, float cy, float w, float h, float angle) {
    Polygon p;
    p.count = 4;
    p.center = (Vec2){ cx, cy };
    p.angle = angle;
    /* 局部坐标顶点 */
    Vec2 local[4] = { { -w / 2, -h / 2 },
                      { w / 2, -h / 2 },
                      { w / 2, h / 2 },
                      { -w / 2, h / 2 } };
    /* 旋转并平移 */
    float cos_a = cosf(angle), sin_a = sinf(angle);
    for (int i = 0; i < 4; i++) {
        p.vertices[i].x = cx + local[i].x * cos_a - local[i].y * sin_a;
        p.vertices[i].y = cy + local[i].x * sin_a + local[i].y * cos_a;
    }
    return p;
}

/* 创建三角形 */
Polygon MakeTriangle(float cx, float cy, float size, float angle) {
    Polygon p;
    p.count = 3;
    p.center = (Vec2){ cx, cy };
    p.angle = angle;
    Vec2 local[3] = { { 0, -size },
                      { size * 0.866f, size * 0.5f },
                      { -size * 0.866f, size * 0.5f } };
    float cos_a = cosf(angle), sin_a = sinf(angle);
    for (int i = 0; i < 3; i++) {
        p.vertices[i].x = cx + local[i].x * cos_a - local[i].y * sin_a;
        p.vertices[i].y = cy + local[i].x * sin_a + local[i].y * cos_a;
    }
    return p;
}

/* 绘制多边形 */
void DrawPolygon(SDL_Renderer *r, Polygon *p, Uint8 cr, Uint8 cg, Uint8 cb) {
    SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
    for (int i = 0; i < p->count; i++) {
        int j = (i + 1) % p->count;
        SDL_RenderDrawLine(
            r, (int)p->vertices[i].x, (int)p->vertices[i].y,
            (int)p->vertices[j].x, (int)p->vertices[j].y);
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "SAT 碰撞示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    float angle1 = 0, angle2 = 0;
    float pos1_x = 300, pos1_y = 300;
    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
        }

        /* 控制 */
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float speed = 200;
        if (keys[SDL_SCANCODE_LEFT])
            pos1_x -= speed * delta;
        if (keys[SDL_SCANCODE_RIGHT])
            pos1_x += speed * delta;
        if (keys[SDL_SCANCODE_UP])
            pos1_y -= speed * delta;
        if (keys[SDL_SCANCODE_DOWN])
            pos1_y += speed * delta;
        if (keys[SDL_SCANCODE_Q])
            angle1 -= 2.0f * delta;
        if (keys[SDL_SCANCODE_E])
            angle1 += 2.0f * delta;

        /* 创建多边形 */
        Polygon poly1 = MakeRect(pos1_x, pos1_y, 80, 60, angle1);
        Polygon poly2 = MakeTriangle(550, 300, 50, angle2);
        angle2 += delta;

        /* SAT 碰撞检测 */
        CollisionResult cr = SAT_Check(&poly1, &poly2);

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        /* 碰撞时变红，否则绿色 */
        Uint8 r = cr.collide ? 255 : 100;
        Uint8 g = cr.collide ? 100 : 200;
        Uint8 b = cr.collide ? 100 : 100;
        DrawPolygon(renderer, &poly1, r, g, b);
        DrawPolygon(renderer, &poly2, r, g, b);

        /* 绘制 MTV（如果碰撞） */
        if (cr.collide) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
            int cx = (int)poly1.center.x, cy = (int)poly1.center.y;
            int ex = cx + (int)(cr.mtv.x * cr.overlap * 5);
            int ey = cy + (int)(cr.mtv.y * cr.overlap * 5);
            SDL_RenderDrawLine(renderer, cx, cy, ex, ey);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
