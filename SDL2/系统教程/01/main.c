#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    /* 初始化 SDL，只启用 VIDEO 子系统 */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 初始化失败: %s\n", SDL_GetError());
        return 1;
    }

    /* 打印编译时版本和运行时版本 */
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    printf("编译时版本: %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
    printf("运行时版本: %d.%d.%d\n", linked.major, linked.minor, linked.patch);

    /* 退出 SDL */
    SDL_Quit();
    return 0;
}