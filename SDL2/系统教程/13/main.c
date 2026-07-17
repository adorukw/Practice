#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow(
        "混合模式示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *circle = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 200, 200);
    SDL_SetRenderTarget(renderer, circle);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = -100; y < 100; y++) {
        for (int x = -100; x < 100; x++) {
            if (x * x + y * y < 100 * 100) {
                SDL_RenderDrawPoint(renderer, x + 100, y + 100);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);

    SDL_bool running = SDL_TRUE;
    Uint32 startTime = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
        }

        Uint32 elapsed = SDL_GetTicks() - startTime;
        double t = elapsed / 100.f;

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        /* 1. 普通混合（白色圆） */
        SDL_SetTextureColorMod(circle, 255, 255, 255);
        SDL_SetTextureAlphaMod(circle, 255);
        SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
        SDL_Rect r1 = { 50, 200, 150, 150 };
        SDL_RenderCopy(renderer, circle, NULL, &r1);

        /* 2. 颜色调制（红色圆） */
        SDL_SetTextureColorMod(circle, 255, 50, 50);
        SDL_Rect r2 = { 250, 200, 150, 150 };
        SDL_RenderCopy(renderer, circle, NULL, &r2);

        /* 3. 透明度动画（淡入淡出） */
        float phase = t * 0.4f;                      // 周期约15.7秒
        float factor = 0.5f + 0.5f * sin(phase);     // 0~1
        factor = factor * factor * (3 - 2 * factor); // smoothstep 进一步柔化

        Uint8 alpha = (Uint8)(255 * factor);
        SDL_SetTextureColorMod(circle, 100, 255, 100);
        SDL_SetTextureAlphaMod(circle, alpha);
        SDL_Rect r3 = { 450, 200, 150, 150 };
        SDL_RenderCopy(renderer, circle, NULL, &r3);

        /* 4. 加法混合（光效） */
        SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_ADD);
        SDL_SetTextureColorMod(circle, 255, 200, 100);
        SDL_SetTextureAlphaMod(circle, 200);
        SDL_Rect r4 = { 650, 200, 150, 150 };
        SDL_RenderCopy(renderer, circle, NULL, &r4);

        /* 5. 乘法混合（阴影） */
        SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_MOD);
        SDL_SetTextureColorMod(circle, 100, 100, 100);
        SDL_SetTextureAlphaMod(circle, 255);
        SDL_Rect r5 = { 150, 400, 150, 150 };
        SDL_RenderCopy(renderer, circle, NULL, &r5);

        /* 6. 旋转的彩色圆 */
        SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
        Uint8 cr = (Uint8)(127 + 127 * sin(t));
        Uint8 cg = (Uint8)(127 + 127 * sin(t + 2.0f));
        Uint8 cb = (Uint8)(127 + 127 * sin(t + 4.0f));
        SDL_SetTextureColorMod(circle, cr, cg, cb);
        SDL_Rect r6 = { 450, 400, 150, 150 };
        SDL_Point center = { 75, 75 };
        SDL_RenderCopyEx(
            renderer, circle, NULL, &r6, t * 90.0f, &center, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        SDL_Delay(500);
    }

    SDL_DestroyTexture(circle);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
