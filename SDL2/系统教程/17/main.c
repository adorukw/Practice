#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *window = SDL_CreateWindow(
        "音频示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
        SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    /* 初始化 SDL_mixer */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio 失败: %s\n", Mix_GetError());
        return 1;
    }
    printf("SDL_mixer 初始化成功\n");

    /* 查询音频格式 */
    int freq, channels;
    Uint16 format;
    Mix_QuerySpec(&freq, &format, &channels);
    printf("音频格式: %dHz, %d-bit, %d 声道\n", freq, format & 0xFF, channels);

    /* 分配 16 个音效通道 */
    Mix_AllocateChannels(16);

    /* 加载音效（需要准备 sound.wav） */
    Mix_Chunk *sound = Mix_LoadWAV("sound.mp3");
    if (!sound) {
        printf("音效加载失败（正常，无文件）: %s\n", Mix_GetError());
    } else {
        printf("音效加载成功\n");
    }

    /* 加载音乐（需要准备 music.ogg） */
    Mix_Music *music = Mix_LoadMUS("music.mp3");
    if (!music) {
        printf("音乐加载失败（正常，无文件）: %s\n", Mix_GetError());
    } else {
        printf("音乐加载成功\n");
    }

    /* 设置音量（0-128） */
    if (sound)
        Mix_VolumeChunk(sound, 64); /* 音效音量 */
    Mix_VolumeMusic(64);            /* 音乐音量 */

    SDL_bool running = SDL_TRUE;
    SDL_bool music_playing = SDL_FALSE;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    running = SDL_FALSE;
                    break;

                case SDL_SCANCODE_1:
                    /* 播放音效 */
                    if (sound) {
                        int channel = Mix_PlayChannel(-1, sound, 0);
                        printf("音效在通道 %d 播放\n", channel);
                    }
                    break;

                case SDL_SCANCODE_2:
                    /* 播放音效 3 次 */
                    if (sound) {
                        Mix_PlayChannel(
                            -1, sound, 2); /* loops=2 表示播放 3 次 */
                    }
                    break;

                case SDL_SCANCODE_3:
                    /* 播放音乐 */
                    if (music && !music_playing) {
                        Mix_PlayMusic(music, -1); /* -1 表示循环 */
                        music_playing = SDL_TRUE;
                        printf("音乐开始播放\n");
                    }
                    break;

                case SDL_SCANCODE_4:
                    /* 暂停/恢复音乐 */
                    if (music_playing) {
                        if (Mix_PausedMusic()) {
                            Mix_ResumeMusic();
                            printf("音乐恢复\n");
                        } else {
                            Mix_PauseMusic();
                            printf("音乐暂停\n");
                        }
                    }
                    break;

                case SDL_SCANCODE_5:
                    /* 停止音乐 */
                    if (music_playing) {
                        Mix_HaltMusic();
                        music_playing = SDL_FALSE;
                        printf("音乐停止\n");
                    }
                    break;

                case SDL_SCANCODE_UP:
                    /* 音量增加 */
                    {
                        int vol = Mix_VolumeMusic(-1);
                        Mix_VolumeMusic(vol + 16 > 128 ? 128 : vol + 16);
                        printf("音乐音量: %d\n", Mix_VolumeMusic(-1));
                    }
                    break;

                case SDL_SCANCODE_DOWN:
                    /* 音量减少 */
                    {
                        int vol = Mix_VolumeMusic(-1);
                        Mix_VolumeMusic(vol - 16 < 0 ? 0 : vol - 16);
                        printf("音乐音量: %d\n", Mix_VolumeMusic(-1));
                    }
                    break;
                default:
                    break;
                }
            }
        }

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 显示操作提示（用矩形代替文字，简化） */
        SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255);
        SDL_Rect r1 = { 50, 100, 200, 40 };
        SDL_RenderFillRect(renderer, &r1); /* 1: 播放音效 */

        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        SDL_Rect r2 = { 50, 160, 200, 40 };
        SDL_RenderFillRect(renderer, &r2); /* 2: 播放音效3次 */

        SDL_SetRenderDrawColor(renderer, 255, 180, 100, 255);
        SDL_Rect r3 = { 50, 220, 200, 40 };
        SDL_RenderFillRect(renderer, &r3); /* 3: 播放音乐 */

        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_Rect r4 = { 50, 280, 200, 40 };
        SDL_RenderFillRect(renderer, &r4); /* 4: 暂停/恢复 */

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    /* 清理 */
    if (sound)
        Mix_FreeChunk(sound);
    if (music)
        Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
