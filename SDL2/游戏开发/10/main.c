#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* ============ 动画系统 ============ */

#define MAX_FRAMES 16
#define MAX_ANIMS 8

typedef struct {
    int frames[MAX_FRAMES]; /* 帧索引 */
    int frame_count;
    float frame_duration; /* 每帧持续时间（秒） */
    SDL_bool loop;
    int priority;
} Animation;

typedef enum {
    ANIM_IDLE = 0,
    ANIM_WALK,
    ANIM_JUMP,
    ANIM_ATTACK,
    ANIM_COUNT
} AnimType;

typedef struct {
    SDL_Texture *spritesheet;
    int frame_width, frame_height;
    int frames_per_row;

    Animation anims[ANIM_COUNT];
    AnimType current_anim;
    int current_frame;
    float frame_timer;

    /* 动画事件回调 */
    void (*on_event)(int event_type);
} Animator;

void Animator_Init(Animator *a, SDL_Texture *sheet, int fw, int fh, int fpr) {
    a->spritesheet = sheet;
    a->frame_width = fw;
    a->frame_height = fh;
    a->frames_per_row = fpr;
    a->current_anim = ANIM_IDLE;
    a->current_frame = 0;
    a->frame_timer = 0;
    a->on_event = NULL;
}

void Animator_SetAnimation(Animator *a, AnimType type) {
    if (type == a->current_anim)
        return;
    /* 优先级检查：新动画优先级必须 >= 当前动画 */
    if (a->anims[type].priority < a->anims[a->current_anim].priority) {
        /* 但如果当前动画不循环且已结束，允许切换 */
        if (a->anims[a->current_anim].loop)
            return;
    }
    a->current_anim = type;
    a->current_frame = 0;
    a->frame_timer = 0;
}

void Animator_Update(Animator *a, float delta) {
    Animation *anim = &a->anims[a->current_anim];
    a->frame_timer += delta;

    while (a->frame_timer >= anim->frame_duration) {
        a->frame_timer -= anim->frame_duration;
        a->current_frame++;

        /* 触发帧事件（示例：每帧触发） */
        if (a->on_event) {
            a->on_event(a->current_frame);
        }

        if (a->current_frame >= anim->frame_count) {
            if (anim->loop) {
                a->current_frame = 0;
            } else {
                /* 非循环动画播放完毕，回到 idle */
                a->current_frame = anim->frame_count - 1;
                Animator_SetAnimation(a, ANIM_IDLE);
                return;
            }
        }
    }
}

void Animator_Render(
    Animator *a, SDL_Renderer *r, int x, int y, SDL_bool flip) {
    Animation *anim = &a->anims[a->current_anim];
    int frame_idx = anim->frames[a->current_frame];
    SDL_Rect src = { (frame_idx % a->frames_per_row) * a->frame_width,
                     (frame_idx / a->frames_per_row) * a->frame_height,
                     a->frame_width, a->frame_height };
    SDL_Rect dest = { x, y, a->frame_width, a->frame_height };
    SDL_RenderCopyEx(
        r, a->spritesheet, &src, &dest, 0, NULL,
        flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

/* ============ 角色控制器 ============ */

typedef struct {
    Animator animator;
    float x, y;
    float vx, vy;
    SDL_bool on_ground;
    SDL_bool facing_right;
    SDL_bool attacking;
    float attack_timer;
} Character;

void Character_Update(Character *c, float delta) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    /* 水平移动 */
    float move = 0;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        move -= 1;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        move += 1;

    c->vx = move * 200;
    if (move != 0)
        c->facing_right = (move > 0);

    /* 跳跃 */
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_W] ||
         keys[SDL_SCANCODE_UP]) &&
        c->on_ground) {
        c->vy = -500;
        c->on_ground = SDL_FALSE;
    }

    /* 攻击 */
    if (keys[SDL_SCANCODE_J] && !c->attacking) {
        c->attacking = SDL_TRUE;
        c->attack_timer = 0.5f; /* 攻击持续 0.5 秒 */
        Animator_SetAnimation(&c->animator, ANIM_ATTACK);
    }
    if (c->attacking) {
        c->attack_timer -= delta;
        if (c->attack_timer <= 0)
            c->attacking = SDL_FALSE;
    }

    /* 重力 */
    c->vy += 1200 * delta;
    c->x += c->vx * delta;
    c->y += c->vy * delta;

    /* 地面碰撞 */
    if (c->y > 400) {
        c->y = 400;
        c->vy = 0;
        c->on_ground = SDL_TRUE;
    }

    /* 边界 */
    if (c->x < 0)
        c->x = 0;
    if (c->x > 800 - 64)
        c->x = 800 - 64;

    /* 动画状态机 */
    if (!c->attacking) {
        if (!c->on_ground) {
            Animator_SetAnimation(&c->animator, ANIM_JUMP);
        } else if (fabsf(c->vx) > 10) {
            Animator_SetAnimation(&c->animator, ANIM_WALK);
        } else {
            Animator_SetAnimation(&c->animator, ANIM_IDLE);
        }
    }

    Animator_Update(&c->animator, delta);
}

/* ============ 主程序 ============ */

/* 动画事件回调 */
void OnAnimEvent(int frame) {
    /* 示例：攻击动画第 2 帧产生伤害 */
    /* 实际应用中这里触发攻击判定 */
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow(
        "动画系统示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 创建程序生成的精灵图（4x4 网格，每帧 64x64） */
    SDL_Texture *sheet = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 256, 256);
    SDL_SetRenderTarget(renderer, sheet);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    /* 第 0-3 帧：idle（轻微呼吸） */
    for (int i = 0; i < 4; i++) {
        int size = 40 + (i % 2) * 2;
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect body = { i * 64 + 12, 64 - size + 12, 40, size };
        SDL_RenderFillRect(renderer, &body);
        /* 眼睛 */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect eye = { i * 64 + 20, 20, 8, 8 };
        SDL_RenderFillRect(renderer, &eye);
    }

    /* 第 4-7 帧：walk（腿部移动） */
    for (int i = 0; i < 4; i++) {
        int leg_offset = (i % 2) * 5;
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect body = { i * 64 + 12 + 64, 12, 40, 40 };
        SDL_RenderFillRect(renderer, &body);
        SDL_SetRenderDrawColor(renderer, 80, 160, 200, 255);
        SDL_Rect leg1 = { i * 64 + 64 + 15, 52, 8, 12 + leg_offset };
        SDL_Rect leg2 = { i * 64 + 64 + 41, 52, 8, 12 - leg_offset + 10 };
        SDL_RenderFillRect(renderer, &leg1);
        SDL_RenderFillRect(renderer, &leg2);
    }

    /* 第 8-11 帧：jump（向上） */
    for (int i = 0; i < 4; i++) {
        int y_offset = -i * 3;
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect body = { i * 64 + 12, 12 + y_offset + 128, 40, 40 };
        SDL_RenderFillRect(renderer, &body);
    }

    /* 第 12-15 帧：attack（挥拳） */
    for (int i = 0; i < 4; i++) {
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect body = { i * 64 + 12, 12 + 192, 40, 40 };
        SDL_RenderFillRect(renderer, &body);
        /* 手臂 */
        SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
        SDL_Rect arm = { i * 64 + 52, 20 + i * 5 + 192, 12, 8 };
        SDL_RenderFillRect(renderer, &arm);
    }

    SDL_SetRenderTarget(renderer, NULL);

    /* 设置角色 */
    Character player;
    Animator_Init(&player.animator, sheet, 64, 64, 4);
    player.animator.on_event = OnAnimEvent;

    /* 配置动画 */
    /* idle: 帧 0-3，循环，0.2秒/帧，优先级 0 */
    player.animator.anims[ANIM_IDLE] = (Animation){ .frames = { 0, 1, 2, 3 },
                                                    .frame_count = 4,
                                                    .frame_duration = 0.2f,
                                                    .loop = SDL_TRUE,
                                                    .priority = 0 };
    /* walk: 帧 4-7，循环，0.1秒/帧，优先级 1 */
    player.animator.anims[ANIM_WALK] = (Animation){ .frames = { 4, 5, 6, 7 },
                                                    .frame_count = 4,
                                                    .frame_duration = 0.1f,
                                                    .loop = SDL_TRUE,
                                                    .priority = 1 };
    /* jump: 帧 8-11，不循环，0.15秒/帧，优先级 2 */
    player.animator.anims[ANIM_JUMP] = (Animation){ .frames = { 8, 9, 10, 11 },
                                                    .frame_count = 4,
                                                    .frame_duration = 0.15f,
                                                    .loop = SDL_FALSE,
                                                    .priority = 2 };
    /* attack: 帧 12-15，不循环，0.1秒/帧，优先级 3（最高） */
    player.animator.anims[ANIM_ATTACK] =
        (Animation){ .frames = { 12, 13, 14, 15 },
                     .frame_count = 4,
                     .frame_duration = 0.1f,
                     .loop = SDL_FALSE,
                     .priority = 3 };

    player.x = 400;
    player.y = 400;
    player.vx = 0;
    player.vy = 0;
    player.on_ground = SDL_TRUE;
    player.facing_right = SDL_TRUE;
    player.attacking = SDL_FALSE;

    printf("操作: A/D移动, 空格/W跳跃, J攻击, ESC退出\n");

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

        Character_Update(&player, delta);

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 40, 50, 70, 255);
        SDL_RenderClear(renderer);

        /* 地面 */
        SDL_SetRenderDrawColor(renderer, 80, 100, 80, 255);
        SDL_Rect ground = { 0, 464, 800, 136 };
        SDL_RenderFillRect(renderer, &ground);

        /* 角色 */
        Animator_Render(
            &player.animator, renderer, (int)player.x, (int)player.y,
            !player.facing_right);

        /* 显示当前动画状态 */
        const char *anim_names[] = { "IDLE", "WALK", "JUMP", "ATTACK" };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect status_bg = { 10, 10, 100, 20 };
        SDL_RenderFillRect(renderer, &status_bg);
        /* 用不同颜色表示不同动画 */
        Uint8 colors[][3] = { { 100, 200, 255 },
                              { 100, 255, 100 },
                              { 255, 200, 100 },
                              { 255, 100, 100 } };
        SDL_SetRenderDrawColor(
            renderer, colors[player.animator.current_anim][0],
            colors[player.animator.current_anim][1],
            colors[player.animator.current_anim][2], 255);
        SDL_Rect status = { 12, 12, 96, 16 };
        SDL_RenderFillRect(renderer, &status);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(sheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
