#include <SDL2/SDL.h>
#include <stdio.h>
#include <limits.h>

const int TARGET_FPS = 60;
const double FRAME_TIME = 1000.0 / TARGET_FPS;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* window =
        SDL_CreateWindow("渲染循环示例", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_bool running = SDL_TRUE;
    double x = 0;
    double speed = 200;

    Uint32 lastTime = SDL_GetTicks();
    int frameCount = 0;
    Uint32 fpsTimer = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = SDL_FALSE;
        }

        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        x += speed * deltaTime;
        if (x > 800) x = -50;

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_Rect rect = {(int)x, 280, 50, 50};
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - frameTime);
        }

        frameCount++;
        if (SDL_GetTicks() - fpsTimer >= 1000) {
            printf("FPS:%d\n", frameCount);
            frameCount = 0;
            fpsTimer = SDL_GetTicks();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}