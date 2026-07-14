#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window =
        SDL_CreateWindow("事件系统示例", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_bool running = SDL_TRUE;
    int eventCount = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            eventCount++;

            switch (event.type) {
            case SDL_QUIT:
                printf("[事件 %d] 用户请求退出\n", eventCount);
                running = SDL_FALSE;
                break;

            case SDL_KEYDOWN:
                printf("[事件 %d] 键盘按下：%s (扫描码 %d)\n", eventCount,
                       SDL_GetKeyName(event.key.keysym.sym),
                       event.key.keysym.scancode);
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = SDL_FALSE;
                }
                break;

            case SDL_KEYUP:
                printf("[事件 %d] 键盘释放: %s\n", eventCount,
                       SDL_GetKeyName(event.key.keysym.sym));
                break;

            case SDL_MOUSEMOTION:
                printf("[事件 %d] 鼠标移动：(%d, %d)\n", eventCount,
                       event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                printf("[事件 %d] 鼠标按下：按钮 %d 位置 (%d, %d)\n",
                       eventCount, event.button.button, event.button.x,
                       event.button.y);
                break;

            case SDL_MOUSEWHEEL:
                printf("[事件 %d] 鼠标滚轮: x=%d y=%d\n", eventCount,
                       event.wheel.x, event.wheel.y);
                break;

            case SDL_WINDOWEVENT:
                printf("[事件 %d] 窗口事件: %d\n", eventCount,
                       event.window.event);
                break;

            default:
                printf("[事件 %d] 其他事件类型: %u\n", eventCount, event.type);
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    printf("总共处理了 %d 个事件\n", eventCount);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}