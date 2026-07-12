#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argb[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL初始化失败：%s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window =
        SDL_CreateWindow("SDL2窗口", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("窗口创建失败：%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    printf("窗口已显示，3秒后自动关闭...\n");
    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}