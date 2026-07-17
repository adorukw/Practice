#include <SDL2/SDL.h>
#include <stdio.h>

/* 游戏配置 */
typedef struct {
    int window_width;
    int window_height;
    int target_fps;
    SDL_bool vsync;
} GameConfig;

/* 游戏主结构 */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    GameConfig config;
    SDL_bool running;
    Uint32 last_frame_time;
} Game;

/* 初始化游戏 */
int Game_Init(Game *game, GameConfig config) {
    game->config = config;
    game->running = SDL_TRUE;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init 失败: %s\n", SDL_GetError());
        return -1;
    }

    game->window = SDL_CreateWindow(
        "SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config.window_width, config.window_height, SDL_WINDOW_SHOWN);
    if (!game->window)
        return -1;

    Uint32 flags = SDL_RENDERER_ACCELERATED;
    if (config.vsync)
        flags |= SDL_RENDERER_PRESENTVSYNC;
    game->renderer = SDL_CreateRenderer(game->window, -1, flags);
    if (!game->renderer)
        return -1;

    game->last_frame_time = SDL_GetTicks();
    return 0;
}

/* 处理输入 */
void Game_HandleInput(Game *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            game->running = SDL_FALSE;
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            game->running = SDL_FALSE;
    }
}

/* 更新逻辑 */
void Game_Update(Game *game, float delta) { /* 游戏逻辑更新，后续章节扩展 */ }

/* 渲染 */
void Game_Render(Game *game) {
    SDL_SetRenderDrawColor(game->renderer, 30, 30, 40, 255);
    SDL_RenderClear(game->renderer);
    /* 渲染游戏对象，后续章节扩展 */
    SDL_RenderPresent(game->renderer);
}

/* 清理 */
void Game_Cleanup(Game *game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    Game game;
    GameConfig config = { .window_width = 800,
                          .window_height = 600,
                          .target_fps = 60,
                          .vsync = SDL_TRUE };

    if (Game_Init(&game, config) != 0) {
        printf("游戏初始化失败\n");
        return 1;
    }

    /* 主循环 */
    while (game.running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - game.last_frame_time) / 1000.0f;
        game.last_frame_time = current_time;

        /* 防止 delta 过大（如程序暂停后恢复） */
        if (delta > 0.1f)
            delta = 0.1f;

        Game_HandleInput(&game);
        Game_Update(&game, delta);
        Game_Render(&game);
    }

    Game_Cleanup(&game);
    return 0;
}
