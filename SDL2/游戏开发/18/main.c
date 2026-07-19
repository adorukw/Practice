#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* ============ 音频管理器 ============ */

#define MAX_SOUNDS 32
#define MAX_MUSIC 8
#define AUDIO_CHANNELS 32

/* 音频分组 */
typedef enum {
    AUDIO_GROUP_BGM = 0,
    AUDIO_GROUP_SFX,
    AUDIO_GROUP_VOICE,
    AUDIO_GROUP_COUNT
} AudioGroup;

/* 音效资源 */
typedef struct {
    char name[32];
    Mix_Chunk *chunk;
    int ref_count;
} SoundResource;

/* 音乐资源 */
typedef struct {
    char name[32];
    Mix_Music *music;
} MusicResource;

/* 淡变状态 */
typedef struct {
    SDL_bool active;
    float current;
    float target;
    float rate; /* 每秒变化量 */
} FadeState;

/* 音频管理器 */
typedef struct {
    SoundResource sounds[MAX_SOUNDS];
    int sound_count;
    MusicResource musics[MAX_MUSIC];
    int music_count;

    int group_volume[AUDIO_GROUP_COUNT]; /* 0-128 */
    int master_volume;

    /* 当前播放的音乐 */
    int current_music;
    FadeState music_fade;

    /* 通道分组（哪些通道属于哪个组） */
    int channel_group[AUDIO_CHANNELS];
} AudioManager;

void Audio_Init(AudioManager *am) {
    memset(am, 0, sizeof(*am));
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(AUDIO_CHANNELS);

    /* 默认音量 */
    am->master_volume = 128;
    for (int i = 0; i < AUDIO_GROUP_COUNT; i++) {
        am->group_volume[i] = 100;
    }
    am->current_music = -1;

    /* 默认通道分组：0-3 BGM, 4-15 SFX, 16-31 Voice */
    for (int i = 0; i < 4; i++)
        am->channel_group[i] = AUDIO_GROUP_BGM;
    for (int i = 4; i < 16; i++)
        am->channel_group[i] = AUDIO_GROUP_SFX;
    for (int i = 16; i < 32; i++)
        am->channel_group[i] = AUDIO_GROUP_VOICE;
}

void Audio_Cleanup(AudioManager *am) {
    for (int i = 0; i < am->sound_count; i++) {
        if (am->sounds[i].chunk)
            Mix_FreeChunk(am->sounds[i].chunk);
    }
    for (int i = 0; i < am->music_count; i++) {
        if (am->musics[i].music)
            Mix_FreeMusic(am->musics[i].music);
    }
    Mix_CloseAudio();
}

/* 计算实际音量 = 主音量 * 组音量 / 128 */
int Audio_GetEffectiveVolume(AudioManager *am, AudioGroup group) {
    return am->master_volume * am->group_volume[group] / 128;
}

/* 加载音效 */
int Audio_LoadSound(AudioManager *am, const char *name, const char *path) {
    if (am->sound_count >= MAX_SOUNDS)
        return -1;
    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (!chunk) {
        printf("加载音效失败 %s: %s\n", path, Mix_GetError());
        return -1;
    }
    strncpy(am->sounds[am->sound_count].name, name, 31);
    am->sounds[am->sound_count].chunk = chunk;
    am->sounds[am->sound_count].ref_count = 0;
    return am->sound_count++;
}

/* 查找音效 */
int Audio_FindSound(AudioManager *am, const char *name) {
    for (int i = 0; i < am->sound_count; i++) {
        if (strcmp(am->sounds[i].name, name) == 0)
            return i;
    }
    return -1;
}

/* 播放音效 */
int Audio_PlaySound(AudioManager *am, const char *name, AudioGroup group) {
    int idx = Audio_FindSound(am, name);
    if (idx < 0)
        return -1;

    /* 找到该组的空闲通道 */
    int channel = -1;
    for (int i = 0; i < AUDIO_CHANNELS; i++) {
        if (am->channel_group[i] == group) {
            if (!Mix_Playing(i)) {
                channel = i;
                break;
            }
        }
    }
    if (channel < 0) {
        /* 没有空闲通道，强制使用第一个 */
        for (int i = 0; i < AUDIO_CHANNELS; i++) {
            if (am->channel_group[i] == group) {
                channel = i;
                break;
            }
        }
    }
    if (channel < 0)
        return -1;

    /* 设置音量 */
    Mix_Volume(channel, Audio_GetEffectiveVolume(am, group));

    /* 播放 */
    Mix_PlayChannel(channel, am->sounds[idx].chunk, 0);
    return channel;
}

/* 3D 音效播放 */
void Audio_PlaySound3D(
    AudioManager *am, const char *name, float listener_x, float listener_y,
    float source_x, float source_y, float max_distance) {
    int idx = Audio_FindSound(am, name);
    if (idx < 0)
        return;

    float dx = source_x - listener_x;
    float dy = source_y - listener_y;
    float distance = sqrtf(dx * dx + dy * dy);

    /* 距离衰减 */
    float volume_ratio = 1.0f - (distance / max_distance);
    if (volume_ratio < 0)
        volume_ratio = 0;

    /* 立体声平衡 */
    float pan = dx / max_distance;
    if (pan > 1)
        pan = 1;
    if (pan < -1)
        pan = -1;

    /* 找通道并播放 */
    int channel = Audio_PlaySound(am, name, AUDIO_GROUP_SFX);
    if (channel >= 0) {
        /* 设置音量 */
        int vol =
            (int)(Audio_GetEffectiveVolume(am, AUDIO_GROUP_SFX) * volume_ratio);
        Mix_Volume(channel, vol);
        /* 设置立体声平衡 */
        Uint8 left = (Uint8)(255 * (1.0f - pan) / 2);
        Uint8 right = (Uint8)(255 * (1.0f + pan) / 2);
        Mix_SetPanning(channel, left, right);
    }
}

/* 播放音乐 */
void Audio_PlayMusic(AudioManager *am, const char *name, SDL_bool loop) {
    for (int i = 0; i < am->music_count; i++) {
        if (strcmp(am->musics[i].name, name) == 0) {
            am->current_music = i;
            Mix_VolumeMusic(Audio_GetEffectiveVolume(am, AUDIO_GROUP_BGM));
            Mix_PlayMusic(am->musics[i].music, loop ? -1 : 1);
            return;
        }
    }
}

/* 音乐淡入 */
void Audio_FadeMusic(AudioManager *am, float target, float duration) {
    am->music_fade.active = SDL_TRUE;
    am->music_fade.current = am->group_volume[AUDIO_GROUP_BGM] / 128.0f;
    am->music_fade.target = target;
    am->music_fade.rate = fabsf(target - am->music_fade.current) / duration;
}

/* 更新音频管理器 */
void Audio_Update(AudioManager *am, float delta) {
    /* 更新音乐淡变 */
    if (am->music_fade.active) {
        float diff = am->music_fade.target - am->music_fade.current;
        float step = am->music_fade.rate * delta;
        if (fabsf(diff) <= step) {
            am->music_fade.current = am->music_fade.target;
            am->music_fade.active = SDL_FALSE;
        } else {
            am->music_fade.current += (diff > 0 ? step : -step);
        }
        /* 应用到音乐音量 */
        int vol = (int)(am->music_fade.current * 128 * am->master_volume / 128);
        Mix_VolumeMusic(vol);
    }
}

/* 设置组音量 */
void Audio_SetGroupVolume(AudioManager *am, AudioGroup group, int volume) {
    am->group_volume[group] = volume;
    if (group == AUDIO_GROUP_BGM) {
        Mix_VolumeMusic(Audio_GetEffectiveVolume(am, group));
    }
}

/* ============ 主程序 ============ */

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *window = SDL_CreateWindow(
        "音频管理示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    AudioManager audio;
    Audio_Init(&audio);

    printf("音频管理器初始化成功\n");
    printf("操作:\n");
    printf("  1 - 播放音效（需加载文件）\n");
    printf("  2 - 播放 3D 音效（鼠标位置）\n");
    printf("  3 - 音乐淡入\n");
    printf("  4 - 音乐淡出\n");
    printf("  +/- - 调整主音量\n");
    printf("  ESC - 退出\n");

    /* 模拟听者位置 */
    float listener_x = 400, listener_y = 300;

    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;

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
                    /* 播放音效（需实际文件） */
                    printf("播放音效（需加载音频文件）\n");
                    break;
                case SDL_SCANCODE_2: {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    printf(
                        "3D 音效: 声源(%d, %d) 距离 %.0f\n", mx, my,
                        sqrtf(
                            (mx - listener_x) * (mx - listener_x) +
                            (my - listener_y) * (my - listener_y)));
                    break;
                default:
                    break;
                }
                case SDL_SCANCODE_3:
                    Audio_FadeMusic(&audio, 1.0f, 2.0f);
                    printf("音乐淡入\n");
                    break;
                case SDL_SCANCODE_4:
                    Audio_FadeMusic(&audio, 0.0f, 2.0f);
                    printf("音乐淡出\n");
                    break;
                case SDL_SCANCODE_EQUALS:
                    audio.master_volume = (audio.master_volume + 16 > 128)
                                              ? 128
                                              : audio.master_volume + 16;
                    printf("主音量: %d\n", audio.master_volume);
                    break;
                case SDL_SCANCODE_MINUS:
                    audio.master_volume = (audio.master_volume - 16 < 0)
                                              ? 0
                                              : audio.master_volume - 16;
                    printf("主音量: %d\n", audio.master_volume);
                    break;
                }
            }
        }

        Audio_Update(&audio, delta);

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 绘制听者（中心） */
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        SDL_Rect listener = { (int)listener_x - 10, (int)listener_y - 10, 20,
                              20 };
        SDL_RenderFillRect(renderer, &listener);

        /* 绘制鼠标位置 */
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_Rect mouse = { mx - 5, my - 5, 10, 10 };
        SDL_RenderFillRect(renderer, &mouse);

        /* 绘制音量条 */
        for (int i = 0; i < AUDIO_GROUP_COUNT; i++) {
            int x = 10 + i * 200;
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_Rect bg = { x, 10, 180, 20 };
            SDL_RenderFillRect(renderer, &bg);
            SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
            SDL_Rect fill = { x, 10, 180 * audio.group_volume[i] / 128, 20 };
            SDL_RenderFillRect(renderer, &fill);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    Audio_Cleanup(&audio);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
