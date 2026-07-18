#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

/* ============ 碰撞体定义 ============ */

typedef struct {
    float x, y; /* 左上角 */
    int w, h;
} AABB;

typedef struct {
    float x, y; /* 圆心 */
    float vx, vy;
    float r;
} Circle;

/* ============ 碰撞检测函数 ============ */

/* AABB vs AABB */
SDL_bool AABB_vs_AABB(AABB a, AABB b) {
    return (
        a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

/* Circle vs Circle */
SDL_bool Circle_vs_Circle(Circle a, Circle b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dist_sq = dx * dx + dy * dy;
    float r_sum = a.r + b.r;
    return dist_sq < r_sum * r_sum;
}

/* AABB vs Circle */
SDL_bool AABB_vs_Circle(AABB box, Circle c) {
    float closest_x = c.x;
    float closest_y = c.y;
    if (closest_x < box.x)
        closest_x = box.x;
    if (closest_x > box.x + box.w)
        closest_x = box.x + box.w;
    if (closest_y < box.y)
        closest_y = box.y;
    if (closest_y > box.y + box.h)
        closest_y = box.y + box.h;

    float dx = c.x - closest_x;
    float dy = c.y - closest_y;
    return (dx * dx + dy * dy) < (c.r * c.r);
}

/* ============ 碰撞响应 ============ */

/* 圆与 AABB 的精确碰撞响应：分离位置并反射速度 */
void Circle_vs_AABB_Response(Circle *ball, AABB box) {
    /* 1. 找到 AABB 上离圆心最近的点 */
    float closest_x = ball->x;
    float closest_y = ball->y;
    if (closest_x < box.x)
        closest_x = box.x;
    if (closest_x > box.x + box.w)
        closest_x = box.x + box.w;
    if (closest_y < box.y)
        closest_y = box.y;
    if (closest_y > box.y + box.h)
        closest_y = box.y + box.h;

    /* 2. 计算法线（从最近点指向圆心）和穿透深度 */
    float dx = ball->x - closest_x;
    float dy = ball->y - closest_y;
    float dist = sqrtf(dx * dx + dy * dy);

    /* 完全重合时给一个默认法线 */
    if (dist < 0.0001f) {
        dx = 1.0f;
        dy = 0.0f;
        dist = 1.0f;
    }

    float nx = dx / dist;
    float ny = dy / dist;
    float penetration = ball->r - dist;

    /* 3. 将圆球推出碰撞 */
    ball->x += nx * penetration;
    ball->y += ny * penetration;

    /* 4. 反射速度（仅当速度方向指向墙壁内部时） */
    float vel_dot = ball->vx * nx + ball->vy * ny;
    if (vel_dot < 0) {
        ball->vx -= 2.0f * vel_dot * nx;
        ball->vy -= 2.0f * vel_dot * ny;
    }
}

/* 圆形间弹性碰撞（质量相等） */
void Circle_vs_Circle_Response(Circle *a, Circle *b) {
    /* 1. 分离位置 */
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 0.0001f) {
        dx = 1.0f;
        dy = 0.0f;
        dist = 1.0f;
    }
    float nx = dx / dist;
    float ny = dy / dist;
    float overlap = (a->r + b->r - dist) / 2.0f;
    a->x -= nx * overlap;
    a->y -= ny * overlap;
    b->x += nx * overlap;
    b->y += ny * overlap;

    /* 2. 弹性碰撞响应（质量相等） */
    float dvx = a->vx - b->vx;
    float dvy = a->vy - b->vy;
    float dv_dot = dvx * nx + dvy * ny;

    /* 只有相互靠近时才处理速度 */
    if (dv_dot > 0) {
        a->vx -= dv_dot * nx;
        a->vy -= dv_dot * ny;
        b->vx += dv_dot * nx;
        b->vy += dv_dot * ny;
    }
}

/* ============ 演示程序 ============ */

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "碰撞检测示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 玩家（AABB） */
    AABB player = { 375, 275, 50, 50 };
    float player_speed = 300.0f;

    /* 静态方块（AABB） */
    AABB walls[] = { { 100, 100, 100, 100 },
                     { 600, 100, 100, 100 },
                     { 100, 400, 100, 100 },
                     { 600, 400, 100, 100 },
                     { 350, 250, 100, 100 } };
    int wall_count = 5;

    /* 圆形对象 */
    Circle balls[] = { { 200, 300, 20, 150, 100 }, { 600, 300, 25, -120, 80 } };
    int ball_count = 2;

    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        if (delta > 0.1f)
            delta = 0.1f;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
        }

        /* ===== 玩家移动（分轴处理，彻底解决角落抖动） ===== */
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float dx = 0, dy = 0;
        if (keys[SDL_SCANCODE_LEFT])
            dx -= player_speed * delta;
        if (keys[SDL_SCANCODE_RIGHT])
            dx += player_speed * delta;
        if (keys[SDL_SCANCODE_UP])
            dy -= player_speed * delta;
        if (keys[SDL_SCANCODE_DOWN])
            dy += player_speed * delta;

        /* X 轴移动并分离 */
        player.x += dx;
        for (int i = 0; i < wall_count; i++) {
            if (AABB_vs_AABB(player, walls[i])) {
                /* 根据重叠方向推出 */
                if (dx > 0)
                    player.x = walls[i].x - player.w; // 从左边撞
                else if (dx < 0)
                    player.x = walls[i].x + walls[i].w; // 从右边撞
            }
        }

        /* Y 轴移动并分离 */
        player.y += dy;
        for (int i = 0; i < wall_count; i++) {
            if (AABB_vs_AABB(player, walls[i])) {
                if (dy > 0)
                    player.y = walls[i].y - player.h;
                else if (dy < 0)
                    player.y = walls[i].y + walls[i].h;
            }
        }

        /* 玩家边界限制 */
        if (player.x < 0)
            player.x = 0;
        if (player.x > 800 - player.w)
            player.x = 800 - player.w;
        if (player.y < 0)
            player.y = 0;
        if (player.y > 600 - player.h)
            player.y = 600 - player.h;

        /* ===== 圆球运动与边界反弹 ===== */
        for (int i = 0; i < ball_count; i++) {
            balls[i].x += balls[i].vx * delta;
            balls[i].y += balls[i].vy * delta;

            if (balls[i].x - balls[i].r < 0) {
                balls[i].x = balls[i].r;
                balls[i].vx *= -1;
            }
            if (balls[i].x + balls[i].r > 800) {
                balls[i].x = 800 - balls[i].r;
                balls[i].vx *= -1;
            }
            if (balls[i].y - balls[i].r < 0) {
                balls[i].y = balls[i].r;
                balls[i].vy *= -1;
            }
            if (balls[i].y + balls[i].r > 600) {
                balls[i].y = 600 - balls[i].r;
                balls[i].vy *= -1;
            }
        }

        /* ===== 圆球之间碰撞 ===== */
        for (int i = 0; i < ball_count; i++) {
            for (int j = i + 1; j < ball_count; j++) {
                if (Circle_vs_Circle(balls[i], balls[j])) {
                    Circle_vs_Circle_Response(&balls[i], &balls[j]);
                }
            }
        }

        /* ===== 圆球与墙壁碰撞 ===== */
        for (int i = 0; i < ball_count; i++) {
            for (int j = 0; j < wall_count; j++) {
                if (AABB_vs_Circle(walls[j], balls[i])) {
                    Circle_vs_AABB_Response(&balls[i], walls[j]);
                }
            }
        }

        /* 玩家与圆球碰撞检测（仅显示，不响应） */
        SDL_bool player_hit = SDL_FALSE;
        for (int i = 0; i < ball_count; i++) {
            if (AABB_vs_Circle(player, balls[i])) {
                player_hit = SDL_TRUE;
            }
        }

        /* ===== 渲染 ===== */
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        /* 墙壁 */
        SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
        for (int i = 0; i < wall_count; i++) {
            SDL_Rect r = { (int)walls[i].x, (int)walls[i].y, walls[i].w,
                           walls[i].h };
            SDL_RenderFillRect(renderer, &r);
        }

        /* 玩家（碰撞时变红） */
        SDL_SetRenderDrawColor(
            renderer, player_hit ? 255 : 100, player_hit ? 100 : 200, 255, 255);
        SDL_Rect pr = { (int)player.x, (int)player.y, player.w, player.h };
        SDL_RenderFillRect(renderer, &pr);

        /* 圆球（扫描线绘制） */
        for (int i = 0; i < ball_count; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
            int r = (int)balls[i].r;
            int cx = (int)balls[i].x;
            int cy = (int)balls[i].y;
            for (int dy = -r; dy <= r; dy++) {
                int half_w = (int)sqrtf(r * r - dy * dy);
                int x1 = cx - half_w;
                int x2 = cx + half_w;
                int y = cy + dy;
                SDL_RenderDrawLine(renderer, x1, y, x2, y);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
