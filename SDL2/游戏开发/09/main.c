#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define GRAVITY 1500.0f    /* 重力加速度 */
#define JUMP_FORCE 550.0f  /* 跳跃初速度 */
#define MOVE_SPEED 300.0f  /* 水平移动速度 */
#define FRICTION 0.85f     /* 地面摩擦 */
#define AIR_FRICTION 0.98f /* 空气阻力 */
#define COYOTE_TIME 0.1f   /* 土狼时间（秒） */
#define JUMP_BUFFER 0.1f   /* 跳跃缓冲（秒） */

typedef struct {
    float x, y;
    float vx, vy;
    int w, h;
    SDL_bool on_ground;
    float coyote_timer;      /* 土狼时间计时器 */
    float jump_buffer_timer; /* 跳跃缓冲计时器 */
} PhysicsBody;

typedef struct {
    float x, y;
    int w, h;
} Platform;

/* AABB 碰撞检测 */
SDL_bool AABB_Collide(
    float ax, float ay, int aw, int ah, float bx, float by, int bw, int bh) {
    return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "物理模拟示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    PhysicsBody player = { 100, 100, 0, 0, 30, 40, SDL_FALSE, 0, 0 };

    /* 平台 */
    Platform platforms[] = { { 0, 550, 800, 50 }, /* 地面 */
                             { 200, 450, 150, 20 },
                             { 450, 350, 150, 20 },
                             { 650, 250, 150, 20 },
                             { 100, 200, 100, 20 } };
    int platform_count = 5;

    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();
    SDL_bool jump_held = SDL_FALSE;

    printf("操作: A/D 或左右键移动, 空格或W跳跃, ESC退出\n");

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
            if (event.type == SDL_KEYDOWN &&
                (event.key.keysym.scancode == SDL_SCANCODE_SPACE ||
                 event.key.keysym.scancode == SDL_SCANCODE_W)) {
                /* 跳跃缓冲：记录跳跃意图 */
                player.jump_buffer_timer = JUMP_BUFFER;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        /* === 物理更新 === */

        /* 水平输入 */
        float input_x = 0;
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
            input_x -= 1;
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
            input_x += 1;

        if (input_x != 0) {
            player.vx = input_x * MOVE_SPEED;
        } else {
            /* 摩擦力 */
            if (player.on_ground) {
                player.vx *= FRICTION;
                if (fabsf(player.vx) < 1)
                    player.vx = 0;
            } else {
                player.vx *= AIR_FRICTION;
            }
        }

        /* 重力 */
        player.vy += GRAVITY * delta;

        /* 土狼时间：在地面时重置，离开后倒计时 */
        if (player.on_ground) {
            player.coyote_timer = COYOTE_TIME;
        } else {
            player.coyote_timer -= delta;
        }

        /* 跳跃缓冲倒计时 */
        if (player.jump_buffer_timer > 0) {
            player.jump_buffer_timer -= delta;
        }

        /* 执行跳跃（土狼时间 + 跳跃缓冲） */
        if (player.jump_buffer_timer > 0 && player.coyote_timer > 0) {
            player.vy = -JUMP_FORCE;
            player.on_ground = SDL_FALSE;
            player.coyote_timer = 0;
            player.jump_buffer_timer = 0;
        }

        /* 可变跳跃高度：松开跳跃键时减小向上速度 */
        jump_held = keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_W];
        if (!jump_held && player.vy < -200) {
            player.vy = -200;
        }

        /* === 位置更新与碰撞 === */

        /* 分轴移动：先 X 后 Y，便于处理碰撞方向 */
        player.on_ground = SDL_FALSE;

        /* X 轴移动 */
        player.x += player.vx * delta;
        for (int i = 0; i < platform_count; i++) {
            Platform *p = &platforms[i];
            if (AABB_Collide(
                    player.x, player.y, player.w, player.h, p->x, p->y, p->w,
                    p->h)) {
                if (player.vx > 0) {
                    player.x = p->x - player.w;
                } else if (player.vx < 0) {
                    player.x = p->x + p->w;
                }
                player.vx = 0;
            }
        }

        /* Y 轴移动 */
        player.y += player.vy * delta;
        for (int i = 0; i < platform_count; i++) {
            Platform *p = &platforms[i];
            if (AABB_Collide(
                    player.x, player.y, player.w, player.h, p->x, p->y, p->w,
                    p->h)) {
                if (player.vy > 0) {
                    /* 落到平台上 */
                    player.y = p->y - player.h;
                    player.on_ground = SDL_TRUE;
                } else if (player.vy < 0) {
                    /* 头撞到平台底部 */
                    player.y = p->y + p->h;
                }
                player.vy = 0;
            }
        }

        /* 边界 */
        if (player.x < 0) {
            player.x = 0;
            player.vx = 0;
        }
        if (player.x > WINDOW_W - player.w) {
            player.x = WINDOW_W - player.w;
            player.vx = 0;
        }
        if (player.y > WINDOW_H) {
            /* 掉出屏幕，重置 */
            player.x = 100;
            player.y = 100;
            player.vx = 0;
            player.vy = 0;
        }

        /* === 渲染 === */
        SDL_SetRenderDrawColor(renderer, 30, 40, 60, 255);
        SDL_RenderClear(renderer);

        /* 绘制平台 */
        SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
        for (int i = 0; i < platform_count; i++) {
            SDL_Rect r = { (int)platforms[i].x, (int)platforms[i].y,
                           platforms[i].w, platforms[i].h };
            SDL_RenderFillRect(renderer, &r);
        }

        /* 绘制玩家 */
        SDL_SetRenderDrawColor(
            renderer, player.on_ground ? 100 : 255, 200, 255, 255);
        SDL_Rect pr = { (int)player.x, (int)player.y, player.w, player.h };
        SDL_RenderFillRect(renderer, &pr);

        /* 绘制速度向量（调试用） */
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
        int cx = (int)(player.x + player.w / 2.0);
        int cy = (int)(player.y + player.h / 2.0);
        SDL_RenderDrawLine(
            renderer, cx, cy, cx + (int)(player.vx * 0.1f),
            cy + (int)(player.vy * 0.1f));

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
