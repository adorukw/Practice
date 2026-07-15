#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

SDL_Texture *LoadTexture(SDL_Renderer *renderer, const char *path) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, path);
  if (!texture) {
    printf("加载 %s 失败：%s\n", path, IMG_GetError());
  }
  return texture;
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if ((IMG_Init(img_flags) & img_flags) != img_flags) {
    printf("SDL_image初始化失败：%s\n", IMG_GetError());
    SDL_Quit();
    return -1;
  }
  printf("SDL_image 初始化成功\n");

  SDL_Window *window =
      SDL_CreateWindow("SDL_image 示例", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  /* 加载 PNG 纹理（需要准备一个带透明通道的 PNG） */
  SDL_Texture *png_tex = LoadTexture(renderer, "sprite.png");
  /* 加载 JPG 纹理（背景图） */
  SDL_Texture *jpg_tex = LoadTexture(renderer, "background.jpg");

  int png_w = 0, png_h = 0;
  if (png_tex) {
    SDL_QueryTexture(png_tex, NULL, NULL, &png_w, &png_h);
    printf("PNG 纹理尺寸: %dx%d\n", png_w, png_h);
  }

  SDL_bool running = SDL_TRUE;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = SDL_FALSE;
      if (event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        running = SDL_FALSE;
    }

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    if (jpg_tex) {
      SDL_Rect bg_dest = {0, 0, 800, 600};
      SDL_RenderCopy(renderer, jpg_tex, NULL, &bg_dest);
    }

    if (png_tex) {
      SDL_Rect sprite_dest = {350, 250, png_w, png_h};
      SDL_RenderCopy(renderer, png_tex, NULL, &sprite_dest);
    }

    SDL_RenderPresent(renderer);
  }

  if (png_tex)
    SDL_DestroyTexture(png_tex);
  if (jpg_tex)
    SDL_DestroyTexture(jpg_tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
