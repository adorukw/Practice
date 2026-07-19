#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600

/* AI 状态 */
typedef enum {
    STATE_PATROL = 0,
    STATE_CHASE,
    STATE_ATTACK,
    STATE_FLEE,
    STATE_DEAD,
    STATE_COUNT
} AIState;

/* 敌人结构 */
typedef struct {
    float x, y;
    float vx, vy;
    int hp;
    int max_hp;
    AIState state;

    /* 巡逻路径 */
    float patrol_points[4][2];
    int patrol_current;
    float patrol_speed;

    /* 视野 */
    float view_range;
    float attack_range;

    /* 攻击 */
    float attack_cooldown;
    float attack_timer;
    int attack_damage;
} Enemy;

/* 玩家结构 */
typedef struct {
    float x, y;
    int hp;
} Player;

/* 状态名称（调试用） */
const char *state_names[] = { "巡逻", "追击", "攻击", "逃跑", "死亡" };

/* 距离计算 */
float Distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1, dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

/* ============ 状态逻辑 ============ */

void State_Patrol(Enemy *e, Player *p, float delta) {
    /* 沿巡逻路径移动 */
    float *target = e->patrol_points[e->patrol_current];
    float dx = target[0] - e->x;
    float dy = target[1] - e->y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 5) {
        /* 到达当前巡逻点，切换下一个 */
        e->patrol_current = (e->patrol_current + 1) % 4;
    } else {
        e->vx = dx / dist * e->patrol_speed;
        e->vy = dy / dist * e->patrol_speed;
        e->x += e->vx * delta;
        e->y += e->vy * delta;
    }

    /* 转换条件：看到玩家 */
    if (Distance(e->x, e->y, p->x, p->y) < e->view_range) {
        e->state = STATE_CHASE;
        printf("敌人: 巡逻 -> 追击\n");
    }
}

void State_Chase(Enemy *e, Player *p, float delta) {
    /* 追击玩家 */
    float dx = p->x - e->x;
    float dy = p->y - e->y;
    float dist = sqrtf(dx * dx + dy * dy);

    float chase_speed = e->patrol_speed * 1.5f;
    e->vx = dx / dist * chase_speed;
    e->vy = dy / dist * chase_speed;
    e->x += e->vx * delta;
    e->y += e->vy * delta;

    /* 转换条件 */
    if (dist < e->attack_range) {
        e->state = STATE_ATTACK;
        printf("敌人: 追击 -> 攻击\n");
    } else if (dist > e->view_range * 1.5f) {
        /* 玩家脱离视野 */
        e->state = STATE_PATROL;
        printf("敌人: 追击 -> 巡逻\n");
    }

    /* 生命值低转换 */
    if (e->hp < e->max_hp * 0.3f) {
        e->state = STATE_FLEE;
        printf("敌人: 追击 -> 逃跑\n");
    }
}

void State_Attack(Enemy *e, Player *p, float delta) {
    /* 攻击冷却 */
    e->attack_timer -= delta;
    if (e->attack_timer <= 0) {
        /* 执行攻击 */
        p->hp -= e->attack_damage;
        e->attack_timer = e->attack_cooldown;
        printf("敌人攻击! 玩家HP: %d\n", p->hp);
    }

    /* 转换条件 */
    float dist = Distance(e->x, e->y, p->x, p->y);
    if (dist > e->attack_range) {
        e->state = STATE_CHASE;
        printf("敌人: 攻击 -> 追击\n");
    }

    if (e->hp < e->max_hp * 0.3f) {
        e->state = STATE_FLEE;
        printf("敌人: 攻击 -> 逃跑\n");
    }
}

void State_Flee(Enemy *e, Player *p, float delta) {
    /* 远离玩家 */
    float dx = e->x - p->x;
    float dy = e->y - p->y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > 0.1f) {
        float flee_speed = e->patrol_speed * 2.0f;
        e->vx = dx / dist * flee_speed;
        e->vy = dy / dist * flee_speed;
        e->x += e->vx * delta;
        e->y += e->vy * delta;
    }

    /* 边界 */
    if (e->x < 0)
        e->x = 0;
    if (e->x > WINDOW_W)
        e->x = WINDOW_W;
    if (e->y < 0)
        e->y = 0;
    if (e->y > WINDOW_H)
        e->y = WINDOW_H;

    /* 转换条件：远离玩家足够远，恢复巡逻 */
    if (dist > e->view_range * 2) {
        e->state = STATE_PATROL;
        printf("敌人: 逃跑 -> 巡逻\n");
    }
}

void State_Dead(Enemy *e, Player *p, float delta) {
    /* 死亡状态，不执行任何动作 */
    e->vx = 0;
    e->vy = 0;
}

/* 状态更新分发 */
void Enemy_Update(Enemy *e, Player *p, float delta) {
    switch (e->state) {
    case STATE_PATROL:
        State_Patrol(e, p, delta);
        break;
    case STATE_CHASE:
        State_Chase(e, p, delta);
        break;
    case STATE_ATTACK:
        State_Attack(e, p, delta);
        break;
    case STATE_FLEE:
        State_Flee(e, p, delta);
        break;
    case STATE_DEAD:
        State_Dead(e, p, delta);
        break;
    default:
        break;
    }

    /* 全局转换：死亡 */
    if (e->hp <= 0 && e->state != STATE_DEAD) {
        e->state = STATE_DEAD;
        printf("敌人: -> 死亡\n");
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "FSM AI 示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W,
        WINDOW_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player player = { 400, 300, 100 };

    Enemy enemy = { 0 };
    enemy.x = 100;
    enemy.y = 100;
    enemy.hp = 100;
    enemy.max_hp = 100;
    enemy.state = STATE_PATROL;
    enemy.patrol_points[0][0] = 100;
    enemy.patrol_points[0][1] = 100;
    enemy.patrol_points[1][0] = 700;
    enemy.patrol_points[1][1] = 100;
    enemy.patrol_points[2][0] = 700;
    enemy.patrol_points[2][1] = 500;
    enemy.patrol_points[3][0] = 100;
    enemy.patrol_points[3][1] = 500;
    enemy.patrol_current = 0;
    enemy.patrol_speed = 100;
    enemy.view_range = 200;
    enemy.attack_range = 50;
    enemy.attack_cooldown = 1.0f;
    enemy.attack_timer = 0;
    enemy.attack_damage = 10;

    printf("操作: WASD移动玩家, J攻击敌人, ESC退出\n");
    printf("敌人状态: %s\n", state_names[enemy.state]);

    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();
    Uint32 last_state_print = 0;

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

        /* 玩家移动 */
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float speed = 250;
        if (keys[SDL_SCANCODE_W])
            player.y -= speed * delta;
        if (keys[SDL_SCANCODE_S])
            player.y += speed * delta;
        if (keys[SDL_SCANCODE_A])
            player.x -= speed * delta;
        if (keys[SDL_SCANCODE_D])
            player.x += speed * delta;
        /* 边界 */
        if (player.x < 0)
            player.x = 0;
        if (player.x > WINDOW_W - 20)
            player.x = WINDOW_W - 20;
        if (player.y < 0)
            player.y = 0;
        if (player.y > WINDOW_H - 20)
            player.y = WINDOW_H - 20;

        /* 玩家攻击 */
        if (keys[SDL_SCANCODE_J]) {
            if (Distance(player.x, player.y, enemy.x, enemy.y) < 60) {
                enemy.hp -= 1;
            }
        }

        Enemy_Update(&enemy, &player, delta);

        /* 每秒打印状态 */
        if (current_time - last_state_print >= 1000) {
            printf(
                "敌人状态: %s, HP: %d/%d, 玩家HP: %d\n",
                state_names[enemy.state], enemy.hp, enemy.max_hp, player.hp);
            last_state_print = current_time;
        }

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 绘制巡逻路径 */
        SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
        for (int i = 0; i < 4; i++) {
            int j = (i + 1) % 4;
            SDL_RenderDrawLine(
                renderer, (int)enemy.patrol_points[i][0],
                (int)enemy.patrol_points[i][1], (int)enemy.patrol_points[j][0],
                (int)enemy.patrol_points[j][1]);
        }

        /* 绘制敌人视野范围 */
        SDL_SetRenderDrawColor(renderer, 100, 100, 50, 100);
        /* 用圆近似 */
        for (int a = 0; a < 360; a += 5) {
            float rad = a * 3.14159f / 180.0f;
            SDL_RenderDrawPoint(
                renderer, (int)(enemy.x + cosf(rad) * enemy.view_range),
                (int)(enemy.y + sinf(rad) * enemy.view_range));
        }

        /* 绘制玩家（蓝色） */
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect player_rect = { (int)player.x, (int)player.y, 20, 20 };
        SDL_RenderFillRect(renderer, &player_rect);

        /* 绘制敌人（根据状态变色） */
        Uint8 enemy_colors[][3] = {
            { 100, 255, 100 }, /* 巡逻：绿 */
            { 255, 200, 100 }, /* 追击：橙 */
            { 255, 100, 100 }, /* 攻击：红 */
            { 255, 255, 100 }, /* 逃跑：黄 */
            { 100, 100, 100 }  /* 死亡：灰 */
        };
        Uint8 *c = enemy_colors[enemy.state];
        SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], 255);
        SDL_Rect enemy_rect = { (int)enemy.x - 15, (int)enemy.y - 15, 30, 30 };
        SDL_RenderFillRect(renderer, &enemy_rect);

        /* 绘制血条 */
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect hp_bg = { (int)enemy.x - 15, (int)enemy.y - 25, 30, 5 };
        SDL_RenderFillRect(renderer, &hp_bg);
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        SDL_Rect hp_fill = { (int)enemy.x - 15, (int)enemy.y - 25,
                             30 * enemy.hp / enemy.max_hp, 5 };
        SDL_RenderFillRect(renderer, &hp_fill);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
