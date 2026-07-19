#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_W 800
#define WINDOW_H 600

/* 节点状态 */
typedef enum { BT_SUCCESS = 0, BT_FAILURE, BT_RUNNING } BTStatus;

/* 黑板：共享数据 */
typedef struct {
    float self_x, self_y;
    float target_x, target_y;
    int self_hp;
    int self_max_hp;
    float view_range;
    float attack_range;
    float move_speed;
    float patrol_target_x, patrol_target_y;
} Blackboard;

/* 行为树节点基类 */
typedef struct BTNode BTNode;
struct BTNode {
    BTStatus (*tick)(BTNode *self, Blackboard *bb, float delta);
    void (*destroy)(BTNode *self);
};

/* ============ 条件节点 ============ */

/* 条件：生命值低 */
typedef struct {
    BTNode base;
    float threshold; /* 生命值百分比阈值 */
} ConditionLowHP;

BTStatus ConditionLowHP_Tick(BTNode *self, Blackboard *bb, float delta) {
    ConditionLowHP *node = (ConditionLowHP *)self;
    float ratio = (float)bb->self_hp / bb->self_max_hp;
    return (ratio < node->threshold) ? BT_SUCCESS : BT_FAILURE;
}

BTNode *CreateConditionLowHP(float threshold) {
    ConditionLowHP *node = malloc(sizeof(ConditionLowHP));
    node->base.tick = ConditionLowHP_Tick;
    node->base.destroy = NULL;
    node->threshold = threshold;
    return (BTNode *)node;
}

/* 条件：看到目标 */
typedef struct {
    BTNode base;
} ConditionSeeTarget;

BTStatus ConditionSeeTarget_Tick(BTNode *self, Blackboard *bb, float delta) {
    float dx = bb->target_x - bb->self_x;
    float dy = bb->target_y - bb->self_y;
    float dist = sqrtf(dx * dx + dy * dy);
    return (dist < bb->view_range) ? BT_SUCCESS : BT_FAILURE;
}

BTNode *CreateConditionSeeTarget() {
    ConditionSeeTarget *node = malloc(sizeof(ConditionSeeTarget));
    node->base.tick = ConditionSeeTarget_Tick;
    node->base.destroy = NULL;
    return (BTNode *)node;
}

/* 条件：在攻击范围 */
BTStatus ConditionInRange_Tick(BTNode *self, Blackboard *bb, float delta) {
    float dx = bb->target_x - bb->self_x;
    float dy = bb->target_y - bb->self_y;
    float dist = sqrtf(dx * dx + dy * dy);
    return (dist < bb->attack_range) ? BT_SUCCESS : BT_FAILURE;
}

BTNode *CreateConditionInRange() {
    ConditionSeeTarget *node = malloc(sizeof(ConditionSeeTarget));
    node->base.tick = ConditionInRange_Tick;
    node->base.destroy = NULL;
    return (BTNode *)node;
}

/* ============ 动作节点 ============ */

/* 动作：逃跑 */
BTStatus ActionFlee_Tick(BTNode *self, Blackboard *bb, float delta) {
    float dx = bb->self_x - bb->target_x;
    float dy = bb->self_y - bb->target_y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 0.1f)
        return BT_RUNNING;

    bb->self_x += dx / dist * bb->move_speed * 2.0f * delta;
    bb->self_y += dy / dist * bb->move_speed * 2.0f * delta;

    /* 边界 */
    if (bb->self_x < 0)
        bb->self_x = 0;
    if (bb->self_x > WINDOW_W)
        bb->self_x = WINDOW_W;
    if (bb->self_y < 0)
        bb->self_y = 0;
    if (bb->self_y > WINDOW_H)
        bb->self_y = WINDOW_H;

    return BT_RUNNING;
}

BTNode *CreateActionFlee() {
    ConditionSeeTarget *node = malloc(sizeof(ConditionSeeTarget));
    node->base.tick = ActionFlee_Tick;
    node->base.destroy = NULL;
    return (BTNode *)node;
}

/* 动作：追击 */
BTStatus ActionChase_Tick(BTNode *self, Blackboard *bb, float delta) {
    float dx = bb->target_x - bb->self_x;
    float dy = bb->target_y - bb->self_y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 0.1f)
        return BT_SUCCESS;

    bb->self_x += dx / dist * bb->move_speed * 1.5f * delta;
    bb->self_y += dy / dist * bb->move_speed * 1.5f * delta;
    return BT_RUNNING;
}

BTNode *CreateActionChase() {
    ConditionSeeTarget *node = malloc(sizeof(ConditionSeeTarget));
    node->base.tick = ActionChase_Tick;
    node->base.destroy = NULL;
    return (BTNode *)node;
}

/* 动作：攻击 */
typedef struct {
    BTNode base;
    float cooldown;
    float timer;
} ActionAttack;

BTStatus ActionAttack_Tick(BTNode *self, Blackboard *bb, float delta) {
    ActionAttack *node = (ActionAttack *)self;
    node->timer -= delta;
    if (node->timer <= 0) {
        node->timer = node->cooldown;
        printf("攻击! (模拟)\n");
    }
    return BT_RUNNING;
}

BTNode *CreateActionAttack(float cooldown) {
    ActionAttack *node = malloc(sizeof(ActionAttack));
    node->base.tick = ActionAttack_Tick;
    node->base.destroy = NULL;
    node->cooldown = cooldown;
    node->timer = 0;
    return (BTNode *)node;
}

/* 动作：巡逻 */
BTStatus ActionPatrol_Tick(BTNode *self, Blackboard *bb, float delta) {
    float dx = bb->patrol_target_x - bb->self_x;
    float dy = bb->patrol_target_y - bb->self_y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 5) {
        /* 到达，选择新巡逻点 */
        bb->patrol_target_x = (float)(100 + rand() % 600);
        bb->patrol_target_y = (float)(100 + rand() % 400);
    } else {
        bb->self_x += dx / dist * bb->move_speed * delta;
        bb->self_y += dy / dist * bb->move_speed * delta;
    }
    return BT_RUNNING;
}

BTNode *CreateActionPatrol() {
    ConditionSeeTarget *node = malloc(sizeof(ConditionSeeTarget));
    node->base.tick = ActionPatrol_Tick;
    node->base.destroy = NULL;
    return (BTNode *)node;
}

/* ============ 组合节点 ============ */

#define MAX_CHILDREN 8

/* 选择节点 */
typedef struct {
    BTNode base;
    BTNode *children[MAX_CHILDREN];
    int child_count;
    int current_child;
} SelectorNode;

BTStatus Selector_Tick(BTNode *self, Blackboard *bb, float delta) {
    SelectorNode *node = (SelectorNode *)self;
    for (int i = 0; i < node->child_count; i++) {
        BTStatus status = node->children[i]->tick(node->children[i], bb, delta);
        if (status != BT_FAILURE) {
            return status; /* 成功或运行中 */
        }
    }
    return BT_FAILURE;
}

BTNode *CreateSelector() {
    SelectorNode *node = calloc(1, sizeof(SelectorNode));
    node->base.tick = Selector_Tick;
    return (BTNode *)node;
}

void Selector_AddChild(BTNode *self, BTNode *child) {
    SelectorNode *node = (SelectorNode *)self;
    if (node->child_count < MAX_CHILDREN) {
        node->children[node->child_count++] = child;
    }
}

/* 序列节点 */
typedef struct {
    BTNode base;
    BTNode *children[MAX_CHILDREN];
    int child_count;
} SequenceNode;

BTStatus Sequence_Tick(BTNode *self, Blackboard *bb, float delta) {
    SequenceNode *node = (SequenceNode *)self;
    for (int i = 0; i < node->child_count; i++) {
        BTStatus status = node->children[i]->tick(node->children[i], bb, delta);
        if (status != BT_SUCCESS) {
            return status; /* 失败或运行中 */
        }
    }
    return BT_SUCCESS;
}

BTNode *CreateSequence() {
    SequenceNode *node = calloc(1, sizeof(SequenceNode));
    node->base.tick = Sequence_Tick;
    return (BTNode *)node;
}

void Sequence_AddChild(BTNode *self, BTNode *child) {
    SequenceNode *node = (SequenceNode *)self;
    if (node->child_count < MAX_CHILDREN) {
        node->children[node->child_count++] = child;
    }
}

/* ============ 主程序 ============ */

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "行为树 AI 示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 构建行为树：
     * Selector (根)
     * ├── Sequence (逃跑)
     * │   ├── ConditionLowHP(0.3)
     * │   └── ActionFlee
     * ├── Sequence (战斗)
     * │   ├── ConditionSeeTarget
     * │   └── Selector
     * │       ├── Sequence (攻击)
     * │       │   ├── ConditionInRange
     * │       │   └── ActionAttack
     * │       └── ActionChase
     * └── ActionPatrol
     */
    BTNode *root = CreateSelector();

    BTNode *flee_seq = CreateSequence();
    Sequence_AddChild(flee_seq, CreateConditionLowHP(0.3f));
    Sequence_AddChild(flee_seq, CreateActionFlee());

    BTNode *combat_seq = CreateSequence();
    Sequence_AddChild(combat_seq, CreateConditionSeeTarget());

    BTNode *combat_sel = CreateSelector();
    BTNode *attack_seq = CreateSequence();
    Sequence_AddChild(attack_seq, CreateConditionInRange());
    Sequence_AddChild(attack_seq, CreateActionAttack(1.0f));
    Selector_AddChild(combat_sel, attack_seq);
    Selector_AddChild(combat_sel, CreateActionChase());

    Sequence_AddChild(combat_seq, combat_sel);

    Selector_AddChild(root, flee_seq);
    Selector_AddChild(root, combat_seq);
    Selector_AddChild(root, CreateActionPatrol());

    /* 黑板 */
    Blackboard bb = { .self_x = 400,
                      .self_y = 300,
                      .target_x = 100,
                      .target_y = 100,
                      .self_hp = 100,
                      .self_max_hp = 100,
                      .view_range = 250,
                      .attack_range = 50,
                      .move_speed = 100,
                      .patrol_target_x = 200,
                      .patrol_target_y = 200 };

    printf("操作: WASD移动玩家(目标), J攻击敌人, ESC退出\n");

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

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        float speed = 250;
        if (keys[SDL_SCANCODE_W])
            bb.target_y -= speed * delta;
        if (keys[SDL_SCANCODE_S])
            bb.target_y += speed * delta;
        if (keys[SDL_SCANCODE_A])
            bb.target_x -= speed * delta;
        if (keys[SDL_SCANCODE_D])
            bb.target_x += speed * delta;
        if (keys[SDL_SCANCODE_J])
            bb.self_hp -= 1;
        if (bb.self_hp < 0)
            bb.self_hp = 0;
        /* 自动回血 */
        if (!keys[SDL_SCANCODE_J] && bb.self_hp < bb.self_max_hp) {
            bb.self_hp += (int)(10 * delta);
            if (bb.self_hp > bb.self_max_hp)
                bb.self_hp = bb.self_max_hp;
        }

        /* 边界 */
        if (bb.target_x < 0)
            bb.target_x = 0;
        if (bb.target_x > WINDOW_W - 20)
            bb.target_x = WINDOW_W - 20;
        if (bb.target_y < 0)
            bb.target_y = 0;
        if (bb.target_y > WINDOW_H - 20)
            bb.target_y = WINDOW_H - 20;

        /* 执行行为树 */
        root->tick(root, &bb, delta);

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 视野范围 */
        SDL_SetRenderDrawColor(renderer, 80, 80, 50, 255);
        for (int a = 0; a < 360; a += 5) {
            float rad = a * 3.14159f / 180.0f;
            SDL_RenderDrawPoint(
                renderer, (int)(bb.self_x + cosf(rad) * bb.view_range),
                (int)(bb.self_y + sinf(rad) * bb.view_range));
        }

        /* 攻击范围 */
        SDL_SetRenderDrawColor(renderer, 120, 50, 50, 255);
        for (int a = 0; a < 360; a += 5) {
            float rad = a * 3.14159f / 180.0f;
            SDL_RenderDrawPoint(
                renderer, (int)(bb.self_x + cosf(rad) * bb.attack_range),
                (int)(bb.self_y + sinf(rad) * bb.attack_range));
        }

        /* 目标（玩家，蓝色） */
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect target_rect = { (int)bb.target_x, (int)bb.target_y, 20, 20 };
        SDL_RenderFillRect(renderer, &target_rect);

        /* 敌人（红色） */
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_Rect enemy_rect = { (int)bb.self_x - 15, (int)bb.self_y - 15, 30,
                                30 };
        SDL_RenderFillRect(renderer, &enemy_rect);

        /* 血条 */
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect hp_bg = { (int)bb.self_x - 15, (int)bb.self_y - 25, 30, 5 };
        SDL_RenderFillRect(renderer, &hp_bg);
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        SDL_Rect hp_fill = { (int)bb.self_x - 15, (int)bb.self_y - 25,
                             30 * bb.self_hp / bb.self_max_hp, 5 };
        SDL_RenderFillRect(renderer, &hp_fill);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    /* 注意：实际项目应递归释放所有节点，此处省略 */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
