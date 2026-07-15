#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef struct {
    SDL_Texture *texture;
    int frameWidth;
    int frameHeight;

    int rowIndex;     // 当前动作在第几行（0开始）
    int framesPerRow; // 该行有多少帧

    int currentFrame;
    double frameTime;
    double accumulator;
    SDL_bool loop;
} Animation;

void AnimationUpdate(Animation *anim, double delta) {
    anim->accumulator += delta;
    while (anim->accumulator >= anim->frameTime) {
        anim->accumulator -= anim->frameTime;
        anim->currentFrame++;
        if (anim->currentFrame >= anim->framesPerRow) {
            if (anim->loop) {
                anim->currentFrame = 0;
            } else {
                anim->currentFrame = anim->framesPerRow - 1;
            }
        }
    }
}

void AnimationDraw(Animation *anim, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect src = { anim->currentFrame * anim->frameWidth,
                     anim->rowIndex * anim->frameHeight, anim->frameWidth,
                     anim->frameHeight };
    SDL_Rect dest = { x, y, anim->frameWidth, anim->frameHeight };
    SDL_RenderCopy(renderer, anim->texture, &src, &dest);
}

void AnimationDrawScaled(
    Animation *anim, SDL_Renderer *renderer, int x, int y, int destW,
    int destH) {
    SDL_Rect src = { anim->currentFrame * anim->frameWidth,
                     anim->rowIndex * anim->frameHeight, anim->frameWidth,
                     anim->frameHeight };
    SDL_Rect dest = { x, y, destW, destH };
    SDL_RenderCopy(renderer, anim->texture, &src, &dest);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow(
        "精灵动画示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920,
        1080, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *sheet = IMG_LoadTexture(renderer, "SpriteSheet1.png");

    // 假设我们想播放第0行的行走动作（4帧）
    // 创建四个动画，分别对应四行
    Animation playerAnim0 = { .texture = sheet,
                              .frameWidth = 460,
                              .frameHeight = 600,
                              .rowIndex = 0,
                              .framesPerRow = 4,
                              .currentFrame = 0,
                              .frameTime = 0.5,
                              .accumulator = 0,
                              .loop = SDL_TRUE };

    Animation playerAnim1 = { .texture = sheet,
                              .frameWidth = 460,
                              .frameHeight = 600,
                              .rowIndex = 1,
                              .framesPerRow = 4,
                              .currentFrame = 0,
                              .frameTime = 0.3, // 可以不同速度
                              .accumulator = 0,
                              .loop = SDL_TRUE };

    Animation playerAnim2 = { .texture = sheet,
                              .frameWidth = 460,
                              .frameHeight = 600,
                              .rowIndex = 2,
                              .framesPerRow = 4,
                              .currentFrame = 0,
                              .frameTime = 0.4,
                              .accumulator = 0,
                              .loop = SDL_TRUE };

    Animation playerAnim3 = { .texture = sheet,
                              .frameWidth = 460,
                              .frameHeight = 600,
                              .rowIndex = 3,
                              .framesPerRow = 4,
                              .currentFrame = 0,
                              .frameTime = 0.2,
                              .accumulator = 0,
                              .loop = SDL_TRUE };

    SDL_bool running = SDL_TRUE;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double delta = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = SDL_FALSE;
            }
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                running = SDL_FALSE;
            }
        }

        // 更新所有动画
        AnimationUpdate(&playerAnim0, delta);
        AnimationUpdate(&playerAnim1, delta);
        AnimationUpdate(&playerAnim2, delta);
        AnimationUpdate(&playerAnim3, delta);

        SDL_SetRenderDrawColor(renderer, 40, 40, 50, 255);
        SDL_RenderClear(renderer);

        // 计算缩放尺寸
        int scaledW = 180;
        double scale = (double)scaledW / playerAnim0.frameWidth;
        int scaledH = (int)(playerAnim0.frameHeight * scale);

        int startX = 120;
        int spacing = 70;
        int baseY = 450;

        // 绘制四个动画
        Animation *anims[] = { &playerAnim0, &playerAnim1, &playerAnim2,
                               &playerAnim3 };
        for (int i = 0; i < 4; i++) {
            AnimationDrawScaled(
                anims[i], renderer, startX + i * (scaledW + spacing), baseY,
                scaledW, scaledH);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(sheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
