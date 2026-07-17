#include <SDL2/SDL.h>
#include <stdio.h>

/* 前向声明 */
typedef struct GameState GameState;
typedef struct Game Game;

/* 游戏状态接口 */
struct GameState {
    const char *name;
    int (*OnEnter)(Game *game);
    void (*OnExit)(Game *game);
    void (*HandleInput)(Game *game);
    void (*Update)(Game *game, float delta);
    void (*Render)(Game *game);
};

/* 游戏主结构 */
struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_bool running;
    Uint32 last_frame_time;

    /* 状态栈 */
    GameState *state_stack[16];
    int state_count;

    /* 全局游戏数据 */
    int score;
    int player_health;
};

/* 状态栈操作 */
void Game_PushState(Game *game, GameState *state) {
    if (game->state_count >= 16)
        return;
    if (game->state_count > 0) {
        /* 当前栈顶状态暂停（无需显式调用，Update 不执行即可） */
    }
    game->state_stack[game->state_count] = state;
    game->state_count++;
    if (state->OnEnter)
        state->OnEnter(game);
    printf("进入状态: %s\n", state->name);
}

void Game_PopState(Game *game) {
    if (game->state_count <= 0)
        return;
    GameState *top = game->state_stack[game->state_count - 1];
    if (top->OnExit)
        top->OnExit(game);
    printf("退出状态: %s\n", top->name);
    game->state_count--;
}

void Game_SwitchState(Game *game, GameState *state) {
    Game_PopState(game);
    Game_PushState(game, state);
}

GameState *Game_GetTopState(Game *game) {
    if (game->state_count <= 0)
        return NULL;
    return game->state_stack[game->state_count - 1];
}

/* ============ 主菜单状态 ============ */
int Menu_OnEnter(Game *game) {
    printf("  加载菜单资源\n");
    return 0;
}
void Menu_OnExit(Game *game) { printf("  卸载菜单资源\n"); }
void Menu_HandleInput(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            game->running = SDL_FALSE;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                game->running = SDL_FALSE;
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                /* 切换到游戏状态 */
                extern GameState PlayingState;
                Game_SwitchState(game, &PlayingState);
            }
        }
    }
}
void Menu_Update(Game *game, float delta) { /* 菜单动画等 */ }
void Menu_Render(Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 40, 20, 60, 255);
    SDL_RenderClear(game->renderer);

    /* 绘制菜单选项（用矩形代替文字） */
    SDL_SetRenderDrawColor(game->renderer, 200, 180, 100, 255);
    SDL_Rect start_btn = { 300, 250, 200, 50 };
    SDL_RenderFillRect(game->renderer, &start_btn);

    SDL_SetRenderDrawColor(game->renderer, 150, 150, 150, 255);
    SDL_Rect quit_btn = { 300, 320, 200, 50 };
    SDL_RenderFillRect(game->renderer, &quit_btn);

    SDL_RenderPresent(game->renderer);
}

GameState MenuState = { .name = "Menu",
                        .OnEnter = Menu_OnEnter,
                        .OnExit = Menu_OnExit,
                        .HandleInput = Menu_HandleInput,
                        .Update = Menu_Update,
                        .Render = Menu_Render };

/* ============ 游戏中状态 ============ */
typedef struct {
    float x, y;
} PlayingData;

static PlayingData playing_data;

int Playing_OnEnter(Game *game) {
    printf("  加载游戏资源\n");
    playing_data.x = 400;
    playing_data.y = 300;
    game->score = 0;
    game->player_health = 100;
    return 0;
}
void Playing_OnExit(Game *game) { printf("  卸载游戏资源\n"); }
void Playing_HandleInput(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            game->running = SDL_FALSE;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                /* ESC 暂停 */
                extern GameState PauseState;
                Game_PushState(game, &PauseState);
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_P) {
                /* P 键游戏结束 */
                extern GameState GameOverState;
                Game_SwitchState(game, &GameOverState);
            }
        }
    }
}
void Playing_Update(Game *game, float delta) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    float speed = 300.0f;
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        playing_data.y -= speed * delta;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        playing_data.y += speed * delta;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        playing_data.x -= speed * delta;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        playing_data.x += speed * delta;

    /* 边界 */
    if (playing_data.x < 0)
        playing_data.x = 0;
    if (playing_data.x > 780)
        playing_data.x = 780;
    if (playing_data.y < 0)
        playing_data.y = 0;
    if (playing_data.y > 580)
        playing_data.y = 580;

    game->score += (int)(delta * 10);
}
void Playing_Render(Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 20, 40, 30, 255);
    SDL_RenderClear(game->renderer);

    /* 绘制玩家 */
    SDL_SetRenderDrawColor(game->renderer, 100, 200, 255, 255);
    SDL_Rect player = { (int)playing_data.x, (int)playing_data.y, 20, 20 };
    SDL_RenderFillRect(game->renderer, &player);

    /* 绘制分数（用矩形长度表示） */
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 100, 255);
    SDL_Rect score_bar = { 10, 10, game->score / 10, 20 };
    SDL_RenderFillRect(game->renderer, &score_bar);

    SDL_RenderPresent(game->renderer);
}

GameState PlayingState = { .name = "Playing",
                           .OnEnter = Playing_OnEnter,
                           .OnExit = Playing_OnExit,
                           .HandleInput = Playing_HandleInput,
                           .Update = Playing_Update,
                           .Render = Playing_Render };

/* ============ 暂停状态 ============ */
int Pause_OnEnter(Game *game) {
    printf("  进入暂停\n");
    return 0;
}
void Pause_OnExit(Game *game) { printf("  退出暂停\n"); }
void Pause_HandleInput(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            game->running = SDL_FALSE;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||
                event.key.keysym.scancode == SDL_SCANCODE_P) {
                Game_PopState(game); /* 返回游戏 */
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                /* Q 退出到主菜单 */
                Game_PopState(game); /* 弹出暂停 */
                extern GameState MenuState;
                Game_SwitchState(game, &MenuState); /* 切换菜单 */
            }
        }
    }
}
void Pause_Update(Game *game, float delta) { /* 暂停状态不更新游戏逻辑 */ }
void Pause_Render(Game *game) {
    /* 先渲染下层状态（游戏画面） */
    if (game->state_count >= 2) {
        GameState *below = game->state_stack[game->state_count - 2];
        if (below->Render)
            below->Render(game);
    }

    /* 再渲染半透明遮罩 */
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 150);
    SDL_Rect overlay = { 0, 0, 800, 600 };
    SDL_RenderFillRect(game->renderer, &overlay);

    /* 绘制暂停文字（用矩形代替） */
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_Rect text_bg = { 300, 280, 200, 40 };
    SDL_RenderFillRect(game->renderer, &text_bg);

    SDL_RenderPresent(game->renderer);
}

GameState PauseState = { .name = "Pause",
                         .OnEnter = Pause_OnEnter,
                         .OnExit = Pause_OnExit,
                         .HandleInput = Pause_HandleInput,
                         .Update = Pause_Update,
                         .Render = Pause_Render };

/* ============ 游戏结束状态 ============ */
int GameOver_OnEnter(Game *game) {
    printf("  游戏结束，得分: %d\n", game->score);
    return 0;
}
void GameOver_OnExit(Game *game) {}
void GameOver_HandleInput(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            game->running = SDL_FALSE;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                extern GameState MenuState;
                Game_SwitchState(game, &MenuState);
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                game->running = SDL_FALSE;
            }
        }
    }
}
void GameOver_Update(Game *game, float delta) {}
void GameOver_Render(Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 60, 20, 20, 255);
    SDL_RenderClear(game->renderer);

    /* 用矩形表示游戏结束文字 */
    SDL_SetRenderDrawColor(game->renderer, 255, 100, 100, 255);
    SDL_Rect text = { 250, 250, 300, 60 };
    SDL_RenderFillRect(game->renderer, &text);

    /* 分数条 */
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 100, 255);
    SDL_Rect score_bar = { 200, 350, game->score / 2, 20 };
    SDL_RenderFillRect(game->renderer, &score_bar);

    SDL_RenderPresent(game->renderer);
}

GameState GameOverState = { .name = "GameOver",
                            .OnEnter = GameOver_OnEnter,
                            .OnExit = GameOver_OnExit,
                            .HandleInput = GameOver_HandleInput,
                            .Update = GameOver_Update,
                            .Render = GameOver_Render };

/* ============ 主函数 ============ */
int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    Game game = { 0 };
    game.window = SDL_CreateWindow(
        "状态管理示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    game.renderer =
        SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    game.running = SDL_TRUE;
    game.last_frame_time = SDL_GetTicks();

    /* 初始状态：主菜单 */
    Game_PushState(&game, &MenuState);

    printf("操作说明:\n");
    printf("  菜单: 回车开始游戏\n");
    printf("  游戏: WASD移动, ESC暂停, P结束游戏\n");
    printf("  暂停: ESC/P继续, Q退出到菜单\n");
    printf("  结束: 回车返回菜单\n\n");

    /* 主循环 */
    while (game.running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - game.last_frame_time) / 1000.0f;
        game.last_frame_time = current_time;
        if (delta > 0.1f)
            delta = 0.1f;

        GameState *top = Game_GetTopState(&game);
        if (top) {
            top->HandleInput(&game);
            top->Update(&game, delta);
            top->Render(&game);
        } else {
            game.running = SDL_FALSE;
        }
    }

    /* 清理所有状态 */
    while (game.state_count > 0) {
        Game_PopState(&game);
    }

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    SDL_Quit();
    return 0;
}
