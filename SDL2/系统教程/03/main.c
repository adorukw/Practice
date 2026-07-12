#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (!SDL_VERSION_ATLEAST(2, 0, 10)) {
        SDL_Log("需要SDL2 2.0.10或更高版本");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        SDL_Log("SDL初始化失败：%s", SDL_GetError());
        return 1;
    }
    SDL_Log("视频、音频、定时器子系统初始化成功");

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) == 0) {
        SDL_Log("事件子系统初始化成功");
    }
    else {
        SDL_Log("事件子系统加载失败：%s", SDL_GetError());
    }

    Uint32 subsystems = SDL_WasInit(SDL_INIT_EVERYTHING);
    printf("当前已初始化的子系统：\n");
    if (subsystems & SDL_INIT_VIDEO) printf("  - VIDEO\n");
    if (subsystems & SDL_INIT_AUDIO) printf("  - AUDIO\n");
    if (subsystems & SDL_INIT_TIMER) printf("  - TIMER\n");
    if (subsystems & SDL_INIT_EVENTS) printf("  - EVENTS\n");
    if (subsystems & SDL_INIT_JOYSTICK) printf("  - JOYSTICK\n");

    SDL_Window* window = SDL_CreateWindow("test", 0, 0, 100, 100, 0);

    if (window == NULL) {
        SDL_Log("预期错误演示：%s", SDL_GetError());
    }
    else {
        SDL_DestroyWindow(window);
    }

    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    SDL_Log("事件子系统已卸载");

    SDL_Quit();
    SDL_Log("SDL已完全退出");
    return 0;
}