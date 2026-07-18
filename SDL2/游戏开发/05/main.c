#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTITIES 1024
#define MAX_COMPONENT_TYPES 8

/* 组件类型枚举 */
typedef enum {
    COMP_TRANSFORM = 0,
    COMP_VELOCITY,
    COMP_SPRITE,
    COMP_HEALTH,
    COMP_AI_PATROL,
    COMP_PLAYER_INPUT,
    COMP_COUNT
} ComponentType;

/* 组件位掩码 */
typedef Uint32 ComponentMask;
#define MASK(c) (1u << (c))

/* ============ 组件数据结构 ============ */
typedef struct {
    float x, y;
    int w, h;
} C_Transform;
typedef struct {
    float vx, vy;
} C_Velocity;
typedef struct {
    Uint8 r, g, b, a;
} C_Sprite;
typedef struct {
    int hp, max_hp;
} C_Health;
typedef struct {
    float min_x, max_x, speed;
    int dir;
} C_AIPatrol;
typedef struct {
    float speed;
} C_PlayerInput;

/* ============ ECS 世界 ============ */
typedef struct {
    /* 实体管理 */
    ComponentMask entity_masks[MAX_ENTITIES];
    SDL_bool entity_active[MAX_ENTITIES];
    int entity_count;

    /* 组件存储（每个类型一个数组） */
    C_Transform transforms[MAX_ENTITIES];
    C_Velocity velocities[MAX_ENTITIES];
    C_Sprite sprites[MAX_ENTITIES];
    C_Health healths[MAX_ENTITIES];
    C_AIPatrol ai_patrols[MAX_ENTITIES];
    C_PlayerInput player_inputs[MAX_ENTITIES];
} World;

void World_Init(World *world) {
    memset(world, 0, sizeof(World));
    world->entity_count = 0;
}

/* 创建实体，返回 ID */
int World_CreateEntity(World *world) {
    if (world->entity_count >= MAX_ENTITIES)
        return -1;
    int id = world->entity_count++;
    world->entity_masks[id] = 0;
    world->entity_active[id] = SDL_TRUE;
    return id;
}

/* 添加组件 */
void World_AddTransform(World *w, int id, C_Transform c) {
    w->transforms[id] = c;
    w->entity_masks[id] |= MASK(COMP_TRANSFORM);
}
void World_AddVelocity(World *w, int id, C_Velocity c) {
    w->velocities[id] = c;
    w->entity_masks[id] |= MASK(COMP_VELOCITY);
}
void World_AddSprite(World *w, int id, C_Sprite c) {
    w->sprites[id] = c;
    w->entity_masks[id] |= MASK(COMP_SPRITE);
}
void World_AddHealth(World *w, int id, C_Health c) {
    w->healths[id] = c;
    w->entity_masks[id] |= MASK(COMP_HEALTH);
}
void World_AddAIPatrol(World *w, int id, C_AIPatrol c) {
    w->ai_patrols[id] = c;
    w->entity_masks[id] |= MASK(COMP_AI_PATROL);
}
void World_AddPlayerInput(World *w, int id, C_PlayerInput c) {
    w->player_inputs[id] = c;
    w->entity_masks[id] |= MASK(COMP_PLAYER_INPUT);
}

/* 销毁实体 */
void World_DestroyEntity(World *w, int id) {
    w->entity_active[id] = SDL_FALSE;
    w->entity_masks[id] = 0;
}

/* ============ 系统 ============ */

/* 玩家输入系统：处理有 PlayerInput + Velocity 的实体 */
void System_PlayerInput(World *w, float delta) {
    ComponentMask required = MASK(COMP_PLAYER_INPUT) | MASK(COMP_VELOCITY);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < w->entity_count; i++) {
        if (!w->entity_active[i])
            continue;
        if ((w->entity_masks[i] & required) != required)
            continue;

        C_PlayerInput *input = &w->player_inputs[i];
        C_Velocity *vel = &w->velocities[i];
        vel->vx = 0;
        vel->vy = 0;
        if (keys[SDL_SCANCODE_LEFT])
            vel->vx = -input->speed;
        if (keys[SDL_SCANCODE_RIGHT])
            vel->vx = input->speed;
        if (keys[SDL_SCANCODE_UP])
            vel->vy = -input->speed;
        if (keys[SDL_SCANCODE_DOWN])
            vel->vy = input->speed;
    }
}

/* AI 巡逻系统：处理有 AIPatrol + Velocity + Transform 的实体 */
void System_AIPatrol(World *w, float delta) {
    ComponentMask required =
        MASK(COMP_AI_PATROL) | MASK(COMP_VELOCITY) | MASK(COMP_TRANSFORM);

    for (int i = 0; i < w->entity_count; i++) {
        if (!w->entity_active[i])
            continue;
        if ((w->entity_masks[i] & required) != required)
            continue;

        C_AIPatrol *ai = &w->ai_patrols[i];
        C_Velocity *vel = &w->velocities[i];
        C_Transform *trans = &w->transforms[i];

        vel->vx = ai->speed * ai->dir;
        vel->vy = 0;
        if (trans->x <= ai->min_x)
            ai->dir = 1;
        if (trans->x >= ai->max_x)
            ai->dir = -1;
    }
}

/* 移动系统：处理有 Transform + Velocity 的实体 */
void System_Movement(World *w, float delta) {
    ComponentMask required = MASK(COMP_TRANSFORM) | MASK(COMP_VELOCITY);

    for (int i = 0; i < w->entity_count; i++) {
        if (!w->entity_active[i])
            continue;
        if ((w->entity_masks[i] & required) != required)
            continue;

        C_Transform *t = &w->transforms[i];
        C_Velocity *v = &w->velocities[i];
        t->x += v->vx * delta;
        t->y += v->vy * delta;

        /* 边界 */
        if (t->x < 0)
            t->x = 0;
        if (t->x > 800 - t->w)
            t->x = 800 - t->w;
        if (t->y < 0)
            t->y = 0;
        if (t->y > 600 - t->h)
            t->y = 600 - t->h;
    }
}

/* 渲染系统：处理有 Transform + Sprite 的实体 */
void System_Render(World *w, SDL_Renderer *renderer) {
    ComponentMask required = MASK(COMP_TRANSFORM) | MASK(COMP_SPRITE);

    for (int i = 0; i < w->entity_count; i++) {
        if (!w->entity_active[i])
            continue;
        if ((w->entity_masks[i] & required) != required)
            continue;

        C_Transform *t = &w->transforms[i];
        C_Sprite *s = &w->sprites[i];
        SDL_SetRenderDrawColor(renderer, s->r, s->g, s->b, s->a);
        SDL_Rect rect = { (int)t->x, (int)t->y, t->w, t->h };
        SDL_RenderFillRect(renderer, &rect);
    }
}

/* ============ 主程序 ============ */
int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "ECS 示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
        SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    World world;
    World_Init(&world);

    /* 创建玩家 */
    int player = World_CreateEntity(&world);
    World_AddTransform(&world, player, (C_Transform){ 400, 300, 30, 30 });
    World_AddVelocity(&world, player, (C_Velocity){ 0, 0 });
    World_AddSprite(&world, player, (C_Sprite){ 100, 200, 255, 255 });
    World_AddHealth(&world, player, (C_Health){ 100, 100 });
    World_AddPlayerInput(&world, player, (C_PlayerInput){ 250 });

    /* 创建巡逻敌人 */
    int enemy1 = World_CreateEntity(&world);
    World_AddTransform(&world, enemy1, (C_Transform){ 100, 200, 25, 25 });
    World_AddVelocity(&world, enemy1, (C_Velocity){ 0, 0 });
    World_AddSprite(&world, enemy1, (C_Sprite){ 255, 100, 100, 255 });
    World_AddAIPatrol(&world, enemy1, (C_AIPatrol){ 50, 350, 120, 1 });

    int enemy2 = World_CreateEntity(&world);
    World_AddTransform(&world, enemy2, (C_Transform){ 500, 400, 25, 25 });
    World_AddVelocity(&world, enemy2, (C_Velocity){ 0, 0 });
    World_AddSprite(&world, enemy2, (C_Sprite){ 255, 150, 50, 255 });
    World_AddAIPatrol(&world, enemy2, (C_AIPatrol){ 450, 750, 80, -1 });

    /* 创建静态装饰物（只有 Transform + Sprite） */
    int decor = World_CreateEntity(&world);
    World_AddTransform(&world, decor, (C_Transform){ 350, 100, 100, 20 });
    World_AddSprite(&world, decor, (C_Sprite){ 100, 100, 100, 255 });

    printf("创建了 %d 个实体\n", world.entity_count);
    printf("操作：方向键移动玩家，ESC 退出\n");

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

        /* 系统按顺序执行 */
        System_PlayerInput(&world, delta);
        System_AIPatrol(&world, delta);
        System_Movement(&world, delta);

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);
        System_Render(&world, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
