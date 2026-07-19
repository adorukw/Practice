#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTICLES 2000

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;   /* 加速度 */
    float life;     /* 剩余生命（秒） */
    float max_life; /* 初始生命 */
    float size;
    Uint8 r, g, b;
    Uint8 start_alpha;
    SDL_bool active;
} Particle;

typedef enum {
    EMIT_POINT,  /* 点发射 */
    EMIT_CIRCLE, /* 圆形区域发射 */
    EMIT_LINE,   /* 线段发射 */
    EMIT_CONE    /* 锥形发射 */
} EmitShape;

typedef struct {
    float x, y;
    EmitShape shape;
    float radius; /* 圆形半径或线段长度 */
    float angle;  /* 锥形方向 */
    float spread; /* 锥形扩散角度 */

    float emit_rate; /* 每秒发射数 */
    float emit_accumulator;

    float min_life, max_life;
    float min_speed, max_speed;
    float min_size, max_size;
    float gravity;
    float drag; /* 阻力系数 */

    Uint8 color_r, color_g, color_b;
    SDL_BlendMode blend_mode;
} ParticleEmitter;

typedef struct {
    Particle particles[MAX_PARTICLES];
    int next_free;
    int active_count;
} ParticleSystem;

void PS_Init(ParticleSystem *ps) { memset(ps, 0, sizeof(*ps)); }

Particle *PS_Spawn(ParticleSystem *ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        int idx = (ps->next_free + i) % MAX_PARTICLES;
        if (!ps->particles[idx].active) {
            ps->next_free = (idx + 1) % MAX_PARTICLES;
            ps->particles[idx].active = SDL_TRUE;
            ps->active_count++;
            return &ps->particles[idx];
        }
    }
    return NULL; /* 池满 */
}

void PS_Update(ParticleSystem *ps, float delta) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle *p = &ps->particles[i];
        if (!p->active)
            continue;

        /* 更新速度（加速度 + 阻力） */
        p->vx += p->ax * delta;
        p->vy += p->ay * delta;
        p->vx *= (1.0f - 0.5f * delta); /* 简单阻力 */
        p->vy *= (1.0f - 0.5f * delta);

        /* 更新位置 */
        p->x += p->vx * delta;
        p->y += p->vy * delta;

        /* 更新生命 */
        p->life -= delta;
        if (p->life <= 0) {
            p->active = SDL_FALSE;
            ps->active_count--;
        }
    }
}

void PS_Render(ParticleSystem *ps, SDL_Renderer *r, SDL_BlendMode blend) {
    SDL_SetRenderDrawBlendMode(r, blend);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle *p = &ps->particles[i];
        if (!p->active)
            continue;

        /* 根据生命比例计算 alpha 和大小 */
        float life_ratio = p->life / p->max_life;
        Uint8 alpha = (Uint8)(p->start_alpha * life_ratio);
        int size = (int)(p->size * life_ratio);
        if (size < 1)
            size = 1;

        SDL_SetRenderDrawColor(r, p->r, p->g, p->b, alpha);
        SDL_Rect rect = { (int)p->x - size / 2, (int)p->y - size / 2, size,
                          size };
        SDL_RenderFillRect(r, &rect);
    }
}

/* 发射器更新 */
void Emitter_Update(ParticleEmitter *e, ParticleSystem *ps, float delta) {
    e->emit_accumulator += e->emit_rate * delta;
    while (e->emit_accumulator >= 1.0f) {
        e->emit_accumulator -= 1.0f;
        Particle *p = PS_Spawn(ps);
        if (!p)
            break;

        /* 计算初始位置 */
        p->x = e->x;
        p->y = e->y;
        if (e->shape == EMIT_CIRCLE) {
            float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
            float r = (float)(rand() % 100) / 100.0f * e->radius;
            p->x += cosf(angle) * r;
            p->y += sinf(angle) * r;
        } else if (e->shape == EMIT_LINE) {
            p->x += (float)(rand() % 100) / 100.0f * e->radius - e->radius / 2;
        } else if (e->shape == EMIT_CONE) {
            float spread =
                (float)(rand() % 100) / 100.0f * e->spread - e->spread / 2;
            float angle = e->angle + spread;
            float speed = e->min_speed + (float)(rand() % 100) / 100.0f *
                                             (e->max_speed - e->min_speed);
            p->vx = cosf(angle) * speed;
            p->vy = sinf(angle) * speed;
        }

        /* 计算初始速度（非锥形） */
        if (e->shape != EMIT_CONE) {
            float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
            float speed = e->min_speed + (float)(rand() % 100) / 100.0f *
                                             (e->max_speed - e->min_speed);
            p->vx = cosf(angle) * speed;
            p->vy = sinf(angle) * speed;
        }

        /* 其他属性 */
        p->ax = 0;
        p->ay = e->gravity;
        p->life = e->min_life +
                  (float)(rand() % 100) / 100.0f * (e->max_life - e->min_life);
        p->max_life = p->life;
        p->size = e->min_size +
                  (float)(rand() % 100) / 100.0f * (e->max_size - e->min_size);
        p->r = e->color_r;
        p->g = e->color_g;
        p->b = e->color_b;
        p->start_alpha = 255;
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "粒子系统示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ParticleSystem ps;
    PS_Init(&ps);

    /* 火焰发射器 */
    ParticleEmitter fire = { .x = 400,
                             .y = 500,
                             .shape = EMIT_CIRCLE,
                             .radius = 20,
                             .emit_rate = 200,
                             .min_life = 0.5f,
                             .max_life = 1.0f,
                             .min_speed = 50,
                             .max_speed = 150,
                             .min_size = 8,
                             .max_size = 15,
                             .gravity = -200, /* 向上（负重力） */
                             .color_r = 255,
                             .color_g = 150,
                             .color_b = 50,
                             .blend_mode = SDL_BLENDMODE_ADD };

    /* 爆炸参数模板（不再作为发射器使用，仅保存配置） */
    struct {
        float x, y;
        float radius;
        float min_life, max_life;
        float min_speed, max_speed;
        float min_size, max_size;
        float gravity;
        Uint8 r, g, b;
    } explosion_cfg = {
        0, 0,       /* 坐标动态设置 */
        5,          /* 半径 */
        0.3f, 0.8f, /* 生命范围 */
        100, 300,   /* 速度范围 */
        5, 12,      /* 大小范围 */
        100,        /* 重力 */
        255, 255, 100 /* 颜色 */
    };

    printf("操作: 鼠标点击产生爆炸, ESC退出\n");

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
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                /* ===== 修复爆炸：直接生成粒子 ===== */
                float ex = event.button.x;
                float ey = event.button.y;

                for (int i = 0; i < 100; i++) {
                    Particle *p = PS_Spawn(&ps);
                    if (!p) break;

                    /* 初始位置（圆形区域内随机） */
                    float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
                    float r = (float)(rand() % 100) / 100.0f * explosion_cfg.radius;
                    p->x = ex + cosf(angle) * r;
                    p->y = ey + sinf(angle) * r;

                    /* 随机速度（360度） */
                    float v_angle = (float)(rand() % 360) * 3.14159f / 180.0f;
                    float speed = explosion_cfg.min_speed +
                                  (float)(rand() % 100) / 100.0f *
                                  (explosion_cfg.max_speed - explosion_cfg.min_speed);
                    p->vx = cosf(v_angle) * speed;
                    p->vy = sinf(v_angle) * speed;

                    p->ax = 0;
                    p->ay = explosion_cfg.gravity;
                    p->life = explosion_cfg.min_life +
                              (float)(rand() % 100) / 100.0f *
                              (explosion_cfg.max_life - explosion_cfg.min_life);
                    p->max_life = p->life;
                    p->size = explosion_cfg.min_size +
                              (float)(rand() % 100) / 100.0f *
                              (explosion_cfg.max_size - explosion_cfg.min_size);
                    p->r = explosion_cfg.r;
                    p->g = explosion_cfg.g;
                    p->b = explosion_cfg.b;
                    p->start_alpha = 255;
                }
            }
        }

        /* 更新火焰发射器 */
        Emitter_Update(&fire, &ps, delta);
        PS_Update(&ps, delta);

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        /* 渲染粒子（加法混合） */
        PS_Render(&ps, renderer, SDL_BLENDMODE_ADD);

        /* 显示粒子数量 */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect count_bg = { 10, 10, 120, 20 };
        SDL_RenderFillRect(renderer, &count_bg);
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        SDL_Rect count_bar = { 12, 12, ps.active_count * 116 / MAX_PARTICLES, 16 };
        SDL_RenderFillRect(renderer, &count_bar);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
