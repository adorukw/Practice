#include <SDL2/SDL.h>
#include <stdio.h>

const int NUM_WINDOWS = 3;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Color colors[NUM_WINDOWS];
    SDL_Window* windows[NUM_WINDOWS];
    SDL_Renderer* renderers[NUM_WINDOWS];
    colors[0] = (SDL_Color){200, 80, 80, 255};
    colors[1] = (SDL_Color){80, 200, 80, 255};
    colors[2] = (SDL_Color){80, 80, 200, 255};

    for (int i = 0; i < NUM_WINDOWS; i++) {
        char title[32];
        sprintf(title, "窗口 %d", i + 1);
        windows[i] =
            SDL_CreateWindow(title, 100 + i * 250, 100 + i * 50, 400, 300,
                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        renderers[i] =
            SDL_CreateRenderer(windows[i], -1, SDL_RENDERER_ACCELERATED);
        printf("窗口 %d ID: %u\n", i + 1, SDL_GetWindowID(windows[i]));
    }

    SDL_bool running = SDL_TRUE;
    int active_window = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            case SDL_WINDOWEVENT:
                for (int i = 0; i < NUM_WINDOWS; i++) {
                    if (SDL_GetWindowID(windows[i]) == event.window.windowID) {
                        switch (event.window.event) {
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            active_window = i;
                            printf("窗口 %d 获取焦点\n", i + 1);
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                            printf("窗口 %d 被调整为 %dx%d\n", i + 1,
                                   event.window.data1, event.window.data2);
                            break;
                        case SDL_WINDOWEVENT_CLOSE:
                            printf("窗口 %d 请求关闭\n", i + 1);
                            /* 隐藏而非销毁，演示 */
                            SDL_HideWindow(windows[i]);
                            break;
                        case SDL_WINDOWEVENT_EXPOSED:
                            SDL_SetRenderDrawColor(renderers[i], colors[i].r,
                                                   colors[i].g, colors[i].b,
                                                   255);
                            SDL_RenderClear(renderers[i]);
                            SDL_RenderPresent(renderers[i]);
                            break;
                        }
                        break;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    running = SDL_FALSE;
                }
                break;
            }
        }
        for (int i = 0; i < NUM_WINDOWS; i++) {
            SDL_SetRenderDrawColor(renderers[i], colors[i].r, colors[i].g,
                                   colors[i].b, 255);
            SDL_RenderClear(renderers[i]);

            /* 在聚焦的窗口中绘制标记 */
            if (i == active_window) {
                SDL_SetRenderDrawColor(renderers[i], 255, 255, 255, 255);
                SDL_Rect mark = {10, 10, 20, 20};
                SDL_RenderFillRect(renderers[i], &mark);
            }

            SDL_RenderPresent(renderers[i]);
        }

        SDL_Delay(16);
    }

    for (int i = 0; i < NUM_WINDOWS; i++) {
        SDL_DestroyRenderer(renderers[i]);
        SDL_DestroyWindow(windows[i]);
    }
    SDL_Quit();
    return 0;
}