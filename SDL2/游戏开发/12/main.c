#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define WINDOW_W 800
#define WINDOW_H 600

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "屏幕后处理示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    /* 场景目标纹理 */
    SDL_Texture *scene = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W,
        WINDOW_H);

    /* Vignette 纹理（预渲染） */
    SDL_Texture *vignette = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W,
        WINDOW_H);
    SDL_SetRenderTarget(renderer, vignette);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    /* 绘制径向渐变（边缘暗） */
    for (int r = 400; r > 0; r -= 2) {
        Uint8 alpha = (Uint8)(255 * (1.0f - r / 400.0f) * 0.7f);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
        /* 画填充圆（用矩形近似） */
        int cx = WINDOW_W / 2, cy = WINDOW_H / 2;
        for (int y = -r; y <= r; y++) {
            int half_w = (int)sqrtf(r * r - y * y);
            SDL_RenderDrawLine(
                renderer, cx - half_w, cy + y, cx + half_w, cy + y);
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetTextureBlendMode(vignette, SDL_BLENDMODE_BLEND);

    /* 扫描线纹理 */
    SDL_Texture *scanlines = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W,
        WINDOW_H);
    SDL_SetRenderTarget(renderer, scanlines);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    for (int y = 0; y < WINDOW_H; y += 3) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_W, y);
    }
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetTextureBlendMode(scanlines, SDL_BLENDMODE_BLEND);

    /* 后处理模式 */
    typedef enum {
        POST_NONE = 0,
        POST_VIGNETTE,
        POST_SCANLINES,
        POST_COLOR_MOD,
        POST_BLUR,
        POST_ALL
    } PostMode;
    PostMode mode = POST_NONE;
    const char *mode_names[] = { "无后处理", "暗角", "扫描线",
                                 "色彩调整", "模糊", "全部效果" };

    printf("操作: 1-6切换后处理模式, ESC退出\n");

    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();
    float time = 0;

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        time += delta;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = SDL_FALSE;
                if (event.key.keysym.scancode == SDL_SCANCODE_1)
                    mode = POST_NONE;
                if (event.key.keysym.scancode == SDL_SCANCODE_2)
                    mode = POST_VIGNETTE;
                if (event.key.keysym.scancode == SDL_SCANCODE_3)
                    mode = POST_SCANLINES;
                if (event.key.keysym.scancode == SDL_SCANCODE_4)
                    mode = POST_COLOR_MOD;
                if (event.key.keysym.scancode == SDL_SCANCODE_5)
                    mode = POST_BLUR;
                if (event.key.keysym.scancode == SDL_SCANCODE_6)
                    mode = POST_ALL;
                printf("模式: %s\n", mode_names[mode]);
            }
        }

        /* === 第1步：渲染场景到 scene 纹理 === */
        SDL_SetRenderTarget(renderer, scene);
        SDL_SetRenderDrawColor(renderer, 30, 40, 60, 255);
        SDL_RenderClear(renderer);

        /* 绘制一些动态内容 */
        for (int i = 0; i < 10; i++) {
            float angle = time * 0.5f + i * 0.628f;
            int x = (int)(WINDOW_W / 2.0 + cosf(angle) * 200);
            int y = (int)(WINDOW_H / 2.0 + sinf(angle * 1.3f) * 150);
            SDL_SetRenderDrawColor(
                renderer, 100 + i * 15, 200 - i * 10, 255 - i * 20, 255);
            SDL_Rect r = { x - 20, y - 20, 40, 40 };
            SDL_RenderFillRect(renderer, &r);
        }

        /* 中心圆 */
        SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
        int cx = WINDOW_W / 2, cy = WINDOW_H / 2;
        int radius = (int)(50 + sinf(time * 2) * 10);
        for (int y = -radius; y <= radius; y++) {
            int hw = (int)sqrtf(radius * radius - y * y);
            SDL_RenderDrawLine(renderer, cx - hw, cy + y, cx + hw, cy + y);
        }

        /* === 第2步：应用后处理并渲染到屏幕 === */
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* 色彩调整 */
        if (mode == POST_COLOR_MOD || mode == POST_ALL) {
            /* 动态色彩调整 */
            Uint8 r = (Uint8)(180 + sinf(time) * 50);
            Uint8 g = (Uint8)(180 + sinf(time + 2) * 50);
            Uint8 b = (Uint8)(180 + sinf(time + 4) * 50);
            SDL_SetTextureColorMod(scene, r, g, b);
        } else {
            SDL_SetTextureColorMod(scene, 255, 255, 255);
        }

        /* 模糊（多次偏移绘制） */
        if (mode == POST_BLUR || mode == POST_ALL) {
            SDL_SetTextureAlphaMod(scene, 128);
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    if (dx == 0 && dy == 0)
                        continue;
                    SDL_Rect dest = { dx * 2, dy * 2, WINDOW_W, WINDOW_H };
                    SDL_RenderCopy(renderer, scene, NULL, &dest);
                }
            }
            SDL_SetTextureAlphaMod(scene, 255);
        }

        /* 绘制场景 */
        SDL_RenderCopy(renderer, scene, NULL, NULL);

        /* Vignette */
        if (mode == POST_VIGNETTE || mode == POST_ALL) {
            SDL_RenderCopy(renderer, vignette, NULL, NULL);
        }

        /* 扫描线 */
        if (mode == POST_SCANLINES || mode == POST_ALL) {
            SDL_RenderCopy(renderer, scanlines, NULL, NULL);
        }

        /* 显示当前模式（用颜色块） */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect indicator = { 10, 10, 20, 20 };
        SDL_RenderFillRect(renderer, &indicator);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(scene);
    SDL_DestroyTexture(vignette);
    SDL_DestroyTexture(scanlines);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
