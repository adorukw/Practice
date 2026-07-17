#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX_FINGERS 10

typedef struct {
    int active;
    float x, y; /* 归一化坐标 0-1 */
    int pixel_x, pixel_y;
} Finger;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "鼠标与触摸示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    /* 检查触摸设备 */
    int num_touch = SDL_GetNumTouchDevices();
    printf("检测到 %d 个触摸设备\n", num_touch);

    Finger fingers[MAX_FINGERS] = { 0 };

    /* 拖拽对象 */
    SDL_Rect drag_rect = { 350, 250, 100, 100 };
    SDL_bool dragging = SDL_FALSE;
    int drag_offset_x, drag_offset_y;

    SDL_bool running = SDL_TRUE;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mx = event.button.x, my = event.button.y;
                    /* 检查是否点击在矩形内 */
                    if (mx >= drag_rect.x && mx < drag_rect.x + drag_rect.w &&
                        my >= drag_rect.y && my < drag_rect.y + drag_rect.h) {
                        dragging = SDL_TRUE;
                        drag_offset_x = mx - drag_rect.x;
                        drag_offset_y = my - drag_rect.y;
                        printf("开始拖拽\n");
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT && dragging) {
                    dragging = SDL_FALSE;
                    printf("结束拖拽\n");
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging) {
                    drag_rect.x = event.motion.x - drag_offset_x;
                    drag_rect.y = event.motion.y - drag_offset_y;
                }
                break;

            case SDL_FINGERDOWN:
                if (event.tfinger.fingerId < MAX_FINGERS) {
                    int idx = (int)event.tfinger.fingerId;
                    fingers[idx].active = 1;
                    fingers[idx].x = event.tfinger.x;
                    fingers[idx].y = event.tfinger.y;
                    fingers[idx].pixel_x = (int)(event.tfinger.x * 800);
                    fingers[idx].pixel_y = (int)(event.tfinger.y * 600);
                    printf(
                        "手指 %d 按下: (%.2f, %.2f)\n", idx, event.tfinger.x,
                        event.tfinger.y);
                }
                break;

            case SDL_FINGERUP:
                if (event.tfinger.fingerId < MAX_FINGERS) {
                    fingers[(int)event.tfinger.fingerId].active = 0;
                }
                break;

            case SDL_FINGERMOTION:
                if (event.tfinger.fingerId < MAX_FINGERS) {
                    int idx = (int)event.tfinger.fingerId;
                    fingers[idx].x = event.tfinger.x;
                    fingers[idx].y = event.tfinger.y;
                    fingers[idx].pixel_x = (int)(event.tfinger.x * 800);
                    fingers[idx].pixel_y = (int)(event.tfinger.y * 600);
                }
                break;
            }
        }

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 绘制可拖拽矩形 */
        SDL_SetRenderDrawColor(renderer, dragging ? 255 : 100, 180, 255, 255);
        SDL_RenderFillRect(renderer, &drag_rect);

        /* 绘制鼠标位置（十字光标） */
        int mx, my;
        Uint32 mouse_state = SDL_GetMouseState(&mx, &my);
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
        SDL_RenderDrawLine(renderer, mx - 10, my, mx + 10, my);
        SDL_RenderDrawLine(renderer, mx, my - 10, mx, my + 10);

        /* 显示鼠标按钮状态 */
        if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
            SDL_Rect lbtn = { mx - 5, my - 5, 10, 10 };
            SDL_RenderFillRect(renderer, &lbtn);
        }

        /* 绘制触摸点 */
        for (int i = 0; i < MAX_FINGERS; i++) {
            if (fingers[i].active) {
                SDL_SetRenderDrawColor(renderer, 255, 100, 255, 200);
                SDL_Rect f = { fingers[i].pixel_x - 20, fingers[i].pixel_y - 20,
                               40, 40 };
                SDL_RenderFillRect(renderer, &f);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
