#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

typedef struct {
    SDL_Texture *texture;
    int width;
    int height;
} TextTexture;

TextTexture RenderText(
    SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color) {
    TextTexture res = { NULL, 0, 0 };
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (surface) {
        res.texture = SDL_CreateTextureFromSurface(renderer, surface);
        res.width = surface->w;
        res.height = surface->h;
        SDL_FreeSurface(surface);
    }
    return res;
}

void FreeTextTexture(TextTexture *tt) {
    if (tt->texture) {
        SDL_DestroyTexture(tt->texture);
        tt->texture = NULL;
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init() != 0) {
        printf("TTF_Init 失败: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "文字渲染示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 加载字体（需要准备字体文件，如 Arial.ttf） */
    TTF_Font *fontLarge = TTF_OpenFontIndex(
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Bold.ttc", 48, 4);
    TTF_Font *fontMedium = TTF_OpenFontIndex(
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Bold.ttc", 24, 4);
    TTF_Font *fontSmall = TTF_OpenFontIndex(
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Bold.ttc", 16, 4);

    TextTexture title = { NULL, 0, 0 };
    TextTexture hint = { NULL, 0, 0 };
    if (fontLarge) {
        title = RenderText(
            renderer, fontLarge, "SDL2 文字渲染",
            (SDL_Color){ 255, 255, 255, 255 });
    }
    if (fontSmall) {
        hint = RenderText(
            renderer, fontSmall, "按 ESC 退出，空格切换颜色",
            (SDL_Color){ 180, 180, 180, 255 });
    }

    TextTexture fpsText = { NULL, 0, 0 };
    char fpsStr[32];
    int frameCount = 0;
    Uint32 fpsTimer = SDL_GetTicks();
    int currentFps = 0;

    SDL_bool running = SDL_TRUE;
    SDL_Color dynamicColor = { 100, 200, 255, 255 };
    SDL_bool colorToggle = SDL_FALSE;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = SDL_FALSE;
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    colorToggle = !colorToggle;
                    dynamicColor = colorToggle
                                       ? (SDL_Color){ 255, 200, 100, 255 }
                                       : (SDL_Color){ 100, 200, 255, 255 };
                }
            }
        }

        frameCount++;
        Uint32 now = SDL_GetTicks();
        if (now - fpsTimer >= 500) {
            currentFps = frameCount * 1000 / (now - fpsTimer);
            frameCount = 0;
            fpsTimer = now;
            FreeTextTexture(&fpsText);
            if (fontMedium) {
                sprintf(fpsStr, "FPS:%d", currentFps);
                fpsText = RenderText(
                    renderer, fontMedium, fpsStr,
                    (SDL_Color){ 255, 255, 100, 255 });
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        if (title.texture) {
            SDL_Rect dest = { 400 - title.width / 2, 50, title.width,
                              title.height };
            SDL_RenderCopy(renderer, title.texture, NULL, &dest);
        }

        if (fpsText.texture) {
            SDL_Rect dest = { 10, 10, fpsText.width, fpsText.height };
            SDL_RenderCopy(renderer, fpsText.texture, NULL, &dest);
        }

        /* 绘制动态颜色文字 */
        if (fontMedium) {
            TextTexture dyn = RenderText(
                renderer, fontMedium, "动态文字（每帧渲染，演示用）",
                dynamicColor);
            if (dyn.texture) {
                SDL_Rect dest = { 400 - dyn.width / 2, 300, dyn.width,
                                  dyn.height };
                SDL_RenderCopy(renderer, dyn.texture, NULL, &dest);
                FreeTextTexture(&dyn);
            }
        }

        /* 绘制提示 */
        if (hint.texture) {
            SDL_Rect dest = { 400 - hint.width / 2, 550, hint.width,
                              hint.height };
            SDL_RenderCopy(renderer, hint.texture, NULL, &dest);
        }

        SDL_RenderPresent(renderer);
    }

    FreeTextTexture(&title);
    FreeTextTexture(&hint);
    FreeTextTexture(&fpsText);
    if (fontLarge)
        TTF_CloseFont(fontLarge);
    if (fontMedium)
        TTF_CloseFont(fontMedium);
    if (fontSmall)
        TTF_CloseFont(fontSmall);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
