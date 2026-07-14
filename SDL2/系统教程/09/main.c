#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window =
        SDL_CreateWindow("键盘鼠标示例", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    double px = 400, py = 300;
    double speed = 300;

    char text[256] = "";
    SDL_bool textInputActive = SDL_FALSE;

    Uint32 lastTime = SDL_GetTicks();
    SDL_bool running = SDL_TRUE;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;

            case SDL_KEYDOWN:
                /* F1 切换文本输入模式 */
                if (event.key.keysym.scancode == SDL_SCANCODE_F1 &&
                    !event.key.repeat) {
                    if (textInputActive) {
                        SDL_StopTextInput();
                        textInputActive = SDL_FALSE;
                        printf("文本输入关闭\n");
                    }
                    else {
                        SDL_StartTextInput();
                        textInputActive = SDL_TRUE;
                        text[0] = '\0';
                        printf("文本输入开启(输入字符，回车结束)\n");
                    }
                }
                /* ESC 退出 */
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    running = SDL_FALSE;
                }
                /* 回车确认文本输入 */
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN &&
                    textInputActive) {
                    printf("输入的文本是：%s\n", text);
                }

                /* 退格删除字符 */
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE &&
                    textInputActive) {
                    int len = strlen(text);
                    if (len > 0) text[len - 1] - '\0';
                }
                break;

            case SDL_TEXTINPUT:
                if (strlen(text) + strlen(event.text.text) < 255) {
                    strcat(text, event.text.text);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                printf("鼠标点击: (%d, %d) 按钮 %d\n", event.button.x,
                       event.button.y, event.button.button);
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    SDL_bool relative = SDL_GetRelativeMouseMode();
                    SDL_SetRelativeMouseMode(!relative);
                    printf("相对鼠标模式: %s\n", !relative ? "开启" : "关闭");
                }
                break;

            case SDL_MOUSEWHEEL:
                printf("滚轮: y=%d\n", event.wheel.y);
                break;
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
            py -= speed * deltaTime;
        if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
            py += speed * deltaTime;
        if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT])
            px -= speed * deltaTime;
        if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])
            px += speed * deltaTime;

        if (px < 0) px = 0;
        if (px > 780) px = 780;
        if (py < 0) py = 0;
        if (py > 580) py = 580;

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        /* 绘制玩家（蓝色方块） */
        SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255);
        SDL_Rect player = {(int)px, (int)py, 20, 20};
        SDL_RenderFillRect(renderer, &player);

        /* 绘制鼠标位置（红色方块） */
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_Rect cursor = {mx - 5, my - 5, 10, 10};
        SDL_RenderFillRect(renderer, &cursor);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}