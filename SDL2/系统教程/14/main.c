#include <SDL2/SDL.h>
#include <stdio.h>

/* 逻辑动作定义 */
typedef enum {
    ACTION_MOVE_LEFT = 0,
    ACTION_MOVE_RIGHT,
    ACTION_MOVE_UP,
    ACTION_MOVE_DOWN,
    ACTION_JUMP,
    ACTION_FIRE,
    ACTION_COUNT
} Action;

/* 按键映射配置 */
typedef struct {
    SDL_Scancode scancodes[ACTION_COUNT];
    int pressed[ACTION_COUNT];       /* 当前是否按下 */
    int just_pressed[ACTION_COUNT];  /* 本帧刚按下 */
    int just_released[ACTION_COUNT]; /* 本帧刚释放 */
} InputState;

void InputState_Init(InputState *state) {
    /* 默认按键映射 */
    state->scancodes[ACTION_MOVE_LEFT] = SDL_SCANCODE_A;
    state->scancodes[ACTION_MOVE_RIGHT] = SDL_SCANCODE_D;
    state->scancodes[ACTION_MOVE_UP] = SDL_SCANCODE_W;
    state->scancodes[ACTION_MOVE_DOWN] = SDL_SCANCODE_S;
    state->scancodes[ACTION_JUMP] = SDL_SCANCODE_SPACE;
    state->scancodes[ACTION_FIRE] = SDL_SCANCODE_J;

    for (int i = 0; i < ACTION_COUNT; i++) {
        state->pressed[i] = 0;
        state->just_pressed[i] = 0;
        state->just_released[i] = 0;
    }
}

/* 每帧开始时调用，重置 just_pressed/just_released */
void InputState_PreUpdate(InputState *state) {
    for (int i = 0; i < ACTION_COUNT; i++) {
        state->just_pressed[i] = 0;
        state->just_released[i] = 0;
    }
}

/* 处理键盘事件，更新 just_pressed/just_released */
void InputState_HandleEvent(InputState *state, const SDL_Event *event) {
    if (event->type != SDL_KEYDOWN && event->type != SDL_KEYUP)
        return;
    if (event->key.repeat)
        return; /* 忽略自动重复 */

    SDL_Scancode sc = event->key.keysym.scancode;
    for (int i = 0; i < ACTION_COUNT; i++) {
        if (state->scancodes[i] == sc) {
            if (event->type == SDL_KEYDOWN && !state->pressed[i]) {
                state->pressed[i] = 1;
                state->just_pressed[i] = 1;
            } else if (event->type == SDL_KEYUP) {
                state->pressed[i] = 0;
                state->just_released[i] = 1;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "输入管理示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    InputState input;
    InputState_Init(&input);

    float px = 400, py = 300;
    float speed = 300.0f;
    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        /* 1. 重置本帧状态 */
        InputState_PreUpdate(&input);

        /* 2. 处理事件 */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
            InputState_HandleEvent(&input, &event);
        }

        /* 3. 使用输入状态更新逻辑 */
        /* 持续移动（状态轮询） */
        if (input.pressed[ACTION_MOVE_LEFT])
            px -= speed * delta;
        if (input.pressed[ACTION_MOVE_RIGHT])
            px += speed * delta;
        if (input.pressed[ACTION_MOVE_UP])
            py -= speed * delta;
        if (input.pressed[ACTION_MOVE_DOWN])
            py += speed * delta;

        /* 单次动作（事件触发） */
        if (input.just_pressed[ACTION_JUMP]) {
            printf("跳跃！\n");
        }
        if (input.just_pressed[ACTION_FIRE]) {
            printf("开火！\n");
        }
        if (input.just_released[ACTION_FIRE]) {
            printf("停止开火\n");
        }

        /* 边界限制 */
        if (px < 0)
            px = 0;
        if (px > 780)
            px = 780;
        if (py < 0)
            py = 0;
        if (py > 580)
            py = 580;

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 玩家颜色根据开火状态变化 */
        SDL_SetRenderDrawColor(
            renderer, input.pressed[ACTION_FIRE] ? 255 : 100, 180, 255, 255);
        SDL_Rect player = { (int)px, (int)py, 20, 20 };
        SDL_RenderFillRect(renderer, &player);

        /* 显示当前按下的键 */
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        int indicator_y = 550;
        const char *names[] = { "L", "R", "U", "D", "J", "F" };
        for (int i = 0; i < ACTION_COUNT; i++) {
            if (input.pressed[i]) {
                SDL_Rect ind = { 10 + i * 30, indicator_y, 25, 25 };
                SDL_RenderFillRect(renderer, &ind);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
