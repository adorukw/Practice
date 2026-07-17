#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 游戏配置 */
#define GRID_SIZE 20
#define GRID_COLS 30
#define GRID_ROWS 20
#define WINDOW_WIDTH (GRID_COLS * GRID_SIZE)
#define WINDOW_HEIGHT (GRID_ROWS * GRID_SIZE)
#define MOVE_INTERVAL 150  /* 蛇移动间隔（毫秒） */

/* 方向枚举 */
typedef enum {
    DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} Direction;

/* 蛇身节点 */
typedef struct {
    int x, y;
} SnakeSegment;

/* 游戏状态 */
typedef struct {
    SnakeSegment snake[GRID_COLS * GRID_ROWS];
    int snake_length;
    Direction direction;
    Direction next_direction;
    SnakeSegment food;
    int score;
    int game_over;
    Uint32 move_timer;
} GameState;

/* 初始化游戏 */
void Game_Init(GameState* game) {
    /* 蛇初始位置在中间，长度 3，向右移动 */
    game->snake_length = 3;
    game->snake[0].x = 10; game->snake[0].y = 10;  /* 头 */
    game->snake[1].x = 9;  game->snake[1].y = 10;
    game->snake[2].x = 8;  game->snake[2].y = 10;
    game->direction = DIR_RIGHT;
    game->next_direction = DIR_RIGHT;
    game->score = 0;
    game->game_over = 0;
    game->move_timer = 0;

    /* 随机生成食物 */
    srand((unsigned)time(NULL));
    game->food.x = rand() % GRID_COLS;
    game->food.y = rand() % GRID_ROWS;
}

/* 生成新食物（不与蛇身重叠） */
void Game_SpawnFood(GameState* game) {
    int valid = 0;
    while (!valid) {
        game->food.x = rand() % GRID_COLS;
        game->food.y = rand() % GRID_ROWS;
        valid = 1;
        for (int i = 0; i < game->snake_length; i++) {
            if (game->snake[i].x == game->food.x &&
                game->snake[i].y == game->food.y) {
                valid = 0;
                break;
            }
        }
    }
}

/* 更新游戏逻辑 */
void Game_Update(GameState* game, Uint32 delta) {
    if (game->game_over) return;

    game->move_timer += delta;
    if (game->move_timer < MOVE_INTERVAL) return;
    game->move_timer = 0;

    /* 应用下一个方向 */
    game->direction = game->next_direction;

    /* 计算新蛇头位置 */
    SnakeSegment new_head = game->snake[0];
    switch (game->direction) {
    case DIR_UP:    new_head.y--; break;
    case DIR_DOWN:  new_head.y++; break;
    case DIR_LEFT:  new_head.x--; break;
    case DIR_RIGHT: new_head.x++; break;
    }

    /* 碰撞检测：边界 */
    if (new_head.x < 0 || new_head.x >= GRID_COLS ||
        new_head.y < 0 || new_head.y >= GRID_ROWS) {
        game->game_over = 1;
        return;
    }

    /* 碰撞检测：自身（不包括尾部，因为尾部会移动） */
    for (int i = 0; i < game->snake_length - 1; i++) {
        if (game->snake[i].x == new_head.x &&
            game->snake[i].y == new_head.y) {
            game->game_over = 1;
            return;
        }
    }

    /* 检查是否吃到食物 */
    int ate_food = (new_head.x == game->food.x &&
                    new_head.y == game->food.y);

    /* 移动蛇：将所有节点后移一位 */
    for (int i = game->snake_length - 1; i > 0; i--) {
        game->snake[i] = game->snake[i - 1];
    }
    game->snake[0] = new_head;

    /* 如果吃到食物，增加长度并生成新食物 */
    if (ate_food) {
        game->snake_length++;
        game->score += 10;
        Game_SpawnFood(game);
    }
}

/* 处理输入 */
void Game_HandleInput(GameState* game, SDL_Scancode scancode) {
    Direction new_dir = game->direction;
    switch (scancode) {
    case SDL_SCANCODE_UP:    new_dir = DIR_UP; break;
    case SDL_SCANCODE_DOWN:  new_dir = DIR_DOWN; break;
    case SDL_SCANCODE_LEFT:  new_dir = DIR_LEFT; break;
    case SDL_SCANCODE_RIGHT: new_dir = DIR_RIGHT; break;
    default: return;
    }

    /* 不能直接反向 */
    if ((game->direction == DIR_UP && new_dir == DIR_DOWN) ||
        (game->direction == DIR_DOWN && new_dir == DIR_UP) ||
        (game->direction == DIR_LEFT && new_dir == DIR_RIGHT) ||
        (game->direction == DIR_RIGHT && new_dir == DIR_LEFT)) {
        return;
    }
    game->next_direction = new_dir;
}

/* 渲染游戏 */
void Game_Render(GameState* game, SDL_Renderer* renderer,
                 TTF_Font* font) {
    /* 清屏 */
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    /* 绘制网格线（可选） */
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
    for (int x = 0; x <= GRID_COLS; x++) {
        SDL_RenderDrawLine(renderer, x * GRID_SIZE, 0,
                          x * GRID_SIZE, WINDOW_HEIGHT);
    }
    for (int y = 0; y <= GRID_ROWS; y++) {
        SDL_RenderDrawLine(renderer, 0, y * GRID_SIZE,
                          WINDOW_WIDTH, y * GRID_SIZE);
    }

    /* 绘制食物（红色） */
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
    SDL_Rect food_rect = {
        game->food.x * GRID_SIZE + 2,
        game->food.y * GRID_SIZE + 2,
        GRID_SIZE - 4, GRID_SIZE - 4
    };
    SDL_RenderFillRect(renderer, &food_rect);

    /* 绘制蛇（头部亮色，身体暗色） */
    for (int i = 0; i < game->snake_length; i++) {
        if (i == 0) {
            SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);  /* 头部 */
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 180, 60, 255);   /* 身体 */
        }
        SDL_Rect seg = {
            game->snake[i].x * GRID_SIZE + 1,
            game->snake[i].y * GRID_SIZE + 1,
            GRID_SIZE - 2, GRID_SIZE - 2
        };
        SDL_RenderFillRect(renderer, &seg);
    }

    /* 绘制分数 */
    if (font) {
        char score_str[32];
        sprintf(score_str, "Score: %d", game->score);
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, score_str, white);
        if (surface) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {10, 5, surface->w, surface->h};
            SDL_RenderCopy(renderer, tex, NULL, &dest);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surface);
        }
    }

    /* 游戏结束提示 */
    if (game->game_over && font) {
        const char* msg = "Game Over! Press R to Restart";
        SDL_Color yellow = {255, 255, 100, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, msg, yellow);
        if (surface) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dest = {
                WINDOW_WIDTH / 2 - surface->w / 2,
                WINDOW_HEIGHT / 2 - surface->h / 2,
                surface->w, surface->h
            };
            SDL_RenderCopy(renderer, tex, NULL, &dest);
            SDL_DestroyTexture(tex);
            SDL_FreeSurface(surface);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "贪吃蛇", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 20);
    if (!font) {
        font = TTF_OpenFont("arial.ttf", 20);
    }

    GameState game;
    Game_Init(&game);

    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        Uint32 delta = current_time - last_time;
        last_time = current_time;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = SDL_FALSE;
                if (event.key.keysym.scancode == SDL_SCANCODE_R)
                    Game_Init(&game);  /* 重新开始 */
                if (!game.game_over) {
                    Game_HandleInput(&game, event.key.keysym.scancode);
                }
            }
        }

        Game_Update(&game, delta);
        Game_Render(&game, renderer, font);
    }

    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
