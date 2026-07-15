#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Texture* LoadBMPTexture(SDL_Renderer* renderer, const char* file) {
    SDL_Surface* surface = SDL_LoadBMP(file);
    if (!surface) {
        printf("无法加载 %s：%s\n", file, SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("无法创建纹理：%s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("纹理示例", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = LoadBMPTexture(renderer, "test.bmp");
    int tex_w = 0, tex_h = 0;
    if (texture) {
        SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
        printf("纹理尺寸：%dx%d\n", tex_w, tex_h);
    }

    SDL_Texture* gradient =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STREAMING, 256, 256);
    Uint32 pixels[256 * 256];
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            Uint8 r = (Uint8)x;
            Uint8 g = (Uint8)y;
            Uint8 b = (Uint8)(255 - (x + y) / 2);
            pixels[y * 256 + x] = (r << 24) | (g << 16) | (b << 8) | 0xFF;
        }
    }
    SDL_UpdateTexture(gradient, NULL, pixels, 256 * sizeof(Uint32));

    SDL_bool running = SDL_TRUE;
    double angle = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = SDL_FALSE;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (texture) {
            SDL_Rect dest = {50, 50, tex_w, tex_h};
            SDL_RenderCopy(renderer, texture, NULL, &dest);
        }

        SDL_Rect dest_grad = {500, 50, 200, 200};
        SDL_RenderCopy(renderer, gradient, NULL, &dest_grad);

        SDL_Rect dest_rot = {300, 350, 150, 150};
        SDL_Point center = {75, 75};
        SDL_RenderCopyEx(renderer, texture, NULL, &dest_rot, angle, &center,
                         SDL_FLIP_NONE);
        angle += 0.5;
        if (angle >= 360) angle = 0;

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(gradient);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
