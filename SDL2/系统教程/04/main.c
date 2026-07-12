#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    int numDisplays = SDL_GetNumVideoDisplays();
    printf("检测到%d个显示器\n", numDisplays);
    for (int i = 0; i < numDisplays; i++) {
        SDL_Rect bounds;
        SDL_GetDisplayBounds(i, &bounds);
        printf("显示器%d：位置(%d,%d),尺寸 %dx%d\n", i, bounds.x, bounds.y,
               bounds.w, bounds.h);
    }

    SDL_Window* window = SDL_CreateWindow(
        "窗口管理示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_Delay(1000);

    SDL_SetWindowPosition(window, 100, 100);
    printf("窗口已移动到 (100, 100)\n");
    SDL_Delay(1000);

    SDL_SetWindowSize(window, 400, 300);
    printf("窗口大小已调整为 400x300\n");
    SDL_Delay(1000);

    SDL_MaximizeWindow(window);
    printf("窗口已最大化\n");
    SDL_Delay(1000);

    SDL_RestoreWindow(window);
    printf("窗口已恢复\n");
    SDL_Delay(1000);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    printf("已切换到桌面全屏模式\n");
    SDL_Delay(2000);

    SDL_SetWindowFullscreen(window, 0);
    printf("已退出全屏\n");
    SDL_Delay(1000);

    int x, y, w, h;
    SDL_GetWindowPosition(window, &x, &y);
    SDL_GetWindowSize(window, &w, &h);
    Uint32 flags = SDL_GetWindowFlags(window);
    printf("当前窗口: 位置(%d,%d) 尺寸 %dx%d 标志 0x%x\n", x, y, w, h, flags);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}