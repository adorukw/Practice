#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window =
        SDL_CreateWindow("渲染器示例", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    printf("渲染器名称：%s\n", info.name);

    SDL_bool running = SDL_TRUE;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = SDL_FALSE;
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); /* 深蓝灰 */
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* 白色 */
        for (int i = 0; i < 50; i++) {
            SDL_RenderDrawPoint(renderer, 100 + i * 12, 100);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* 红色 */
        SDL_RenderDrawLine(renderer, 50, 200, 750, 200);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /* 绿色 */
        SDL_Rect rect_outline = {100, 300, 200, 100};
        SDL_RenderDrawRect(renderer, &rect_outline);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); /* 蓝色 */
        SDL_Rect rect_fill = {400, 300, 200, 100};
        SDL_RenderFillRect(renderer, &rect_fill);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); /* 黄色 */
        SDL_Rect rects[] = {
            {100, 450, 50, 50}, {200, 450, 50, 50}, {300, 450, 50, 50}};
        SDL_RenderFillRects(renderer, rects, 3);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}