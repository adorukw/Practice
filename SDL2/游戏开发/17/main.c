#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============ 游戏状态数据 ============ */

#define MAX_INVENTORY 20
#define MAX_QUESTS 50

typedef struct {
    char name[32];
    int quantity;
} InventoryItem;

typedef struct {
    int id;
    SDL_bool completed;
    SDL_bool active;
} QuestState;

typedef struct {
    /* 版本号 */
    int version;

    /* 玩家数据 */
    float player_x, player_y;
    int player_hp;
    int player_max_hp;
    int player_level;
    int player_exp;

    /* 物品栏 */
    InventoryItem inventory[MAX_INVENTORY];
    int inventory_count;

    /* 任务状态 */
    QuestState quests[MAX_QUESTS];
    int quest_count;

    /* 世界状态 */
    int current_level;
    SDL_bool doors_opened[32];
    SDL_bool enemies_defeated[64];

    /* 游戏时间 */
    Uint32 play_time;
} SaveData;

/* ============ 二进制序列化 ============ */

#define SAVE_VERSION 2

SDL_bool SaveGame_Binary(const char *filename, SaveData *data) {
    /* 写入临时文件，成功后重命名 */
    char tmp_filename[256];
    snprintf(tmp_filename, sizeof(tmp_filename), "%s.tmp", filename);

    SDL_RWops *file = SDL_RWFromFile(tmp_filename, "wb");
    if (!file) {
        printf("无法创建存档文件: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    /* 写入魔数（标识文件类型） */
    const char magic[] = "SAV2";
    SDL_RWwrite(file, magic, 4, 1);

    /* 写入版本号 */
    int version = SAVE_VERSION;
    SDL_RWwrite(file, &version, sizeof(int), 1);

    /* 写入校验和（简单异或） */
    Uint32 checksum = 0;
    Uint8 *bytes = (Uint8 *)data;
    for (size_t i = 0; i < sizeof(SaveData); i++) {
        checksum ^= bytes[i];
    }
    SDL_RWwrite(file, &checksum, sizeof(Uint32), 1);

    /* 写入数据 */
    SDL_RWwrite(file, data, sizeof(SaveData), 1);

    SDL_RWclose(file);

    /* 重命名临时文件为正式文件 */
    if (rename(tmp_filename, filename) != 0) {
        printf("重命名存档文件失败\n");
        return SDL_FALSE;
    }

    printf("存档保存成功: %s\n", filename);
    return SDL_TRUE;
}

SDL_bool LoadGame_Binary(const char *filename, SaveData *data) {
    SDL_RWops *file = SDL_RWFromFile(filename, "rb");
    if (!file) {
        printf("无法打开存档文件: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    /* 读取并验证魔数 */
    char magic[4];
    SDL_RWread(file, magic, 4, 1);
    if (strncmp(magic, "SAV2", 4) != 0) {
        printf("无效的存档文件格式\n");
        SDL_RWclose(file);
        return SDL_FALSE;
    }

    /* 读取版本号 */
    int version;
    SDL_RWread(file, &version, sizeof(int), 1);
    if (version > SAVE_VERSION) {
        printf("不支持的存档版本: %d\n", version);
        SDL_RWclose(file);
        return SDL_FALSE;
    }

    /* 读取校验和 */
    Uint32 saved_checksum;
    SDL_RWread(file, &saved_checksum, sizeof(Uint32), 1);

    /* 读取数据 */
    SDL_RWread(file, data, sizeof(SaveData), 1);
    SDL_RWclose(file);

    /* 验证校验和 */
    Uint32 computed_checksum = 0;
    Uint8 *bytes = (Uint8 *)data;
    for (size_t i = 0; i < sizeof(SaveData); i++) {
        computed_checksum ^= bytes[i];
    }
    if (computed_checksum != saved_checksum) {
        printf("存档文件已损坏\n");
        return SDL_FALSE;
    }

    /* 版本兼容处理 */
    if (version < SAVE_VERSION) {
        printf("升级存档从版本 %d 到 %d\n", version, SAVE_VERSION);
        /* 补充新字段的默认值 */
        if (version < 2) {
            /* 版本1没有 play_time 字段，设置默认值 */
            data->play_time = 0;
        }
    }

    printf("存档加载成功: %s (版本 %d)\n", filename, version);
    return SDL_TRUE;
}

/* ============ JSON 风格文本序列化（简化） ============ */

SDL_bool SaveGame_Text(const char *filename, SaveData *data) {
    FILE *file = fopen(filename, "w");
    if (!file)
        return SDL_FALSE;

    fprintf(file, "{\n");
    fprintf(file, "  \"version\": %d,\n", SAVE_VERSION);
    fprintf(file, "  \"player\": {\n");
    fprintf(file, "    \"x\": %.2f,\n", data->player_x);
    fprintf(file, "    \"y\": %.2f,\n", data->player_y);
    fprintf(file, "    \"hp\": %d,\n", data->player_hp);
    fprintf(file, "    \"max_hp\": %d,\n", data->player_max_hp);
    fprintf(file, "    \"level\": %d,\n", data->player_level);
    fprintf(file, "    \"exp\": %d\n", data->player_exp);
    fprintf(file, "  },\n");
    fprintf(file, "  \"inventory\": [\n");
    for (int i = 0; i < data->inventory_count; i++) {
        fprintf(
            file, "    {\"name\": \"%s\", \"quantity\": %d}%s\n",
            data->inventory[i].name, data->inventory[i].quantity,
            (i < data->inventory_count - 1) ? "," : "");
    }
    fprintf(file, "  ],\n");
    fprintf(file, "  \"current_level\": %d,\n", data->current_level);
    fprintf(file, "  \"play_time\": %u\n", data->play_time);
    fprintf(file, "}\n");

    fclose(file);
    printf("文本存档保存成功: %s\n", filename);
    return SDL_TRUE;
}

/* ============ 演示 ============ */

void InitSaveData(SaveData *data) {
    memset(data, 0, sizeof(SaveData));
    data->version = SAVE_VERSION;
    data->player_x = 400.0f;
    data->player_y = 300.0f;
    data->player_hp = 85;
    data->player_max_hp = 100;
    data->player_level = 7;
    data->player_exp = 3400;

    data->inventory_count = 3;
    strcpy(data->inventory[0].name, "Potion");
    data->inventory[0].quantity = 5;
    strcpy(data->inventory[1].name, "Sword");
    data->inventory[1].quantity = 1;
    strcpy(data->inventory[2].name, "Key");
    data->inventory[2].quantity = 3;

    data->quest_count = 2;
    data->quests[0].id = 1;
    data->quests[0].completed = SDL_TRUE;
    data->quests[1].id = 2;
    data->quests[1].active = SDL_TRUE;

    data->current_level = 3;
    data->doors_opened[0] = SDL_TRUE;
    data->doors_opened[5] = SDL_TRUE;
    data->enemies_defeated[10] = SDL_TRUE;
    data->enemies_defeated[11] = SDL_TRUE;

    data->play_time = 7200; /* 2小时 */
}

void PrintSaveData(SaveData *data) {
    printf("=== 存档数据 ===\n");
    printf("版本: %d\n", data->version);
    printf(
        "玩家: 位置(%.0f, %.0f) HP %d/%d 等级 %d 经验 %d\n", data->player_x,
        data->player_y, data->player_hp, data->player_max_hp,
        data->player_level, data->player_exp);
    printf("物品栏 (%d):\n", data->inventory_count);
    for (int i = 0; i < data->inventory_count; i++) {
        printf(
            "  %s x%d\n", data->inventory[i].name, data->inventory[i].quantity);
    }
    printf("当前关卡: %d\n", data->current_level);
    printf(
        "游戏时间: %u 秒 (%.1f 小时)\n", data->play_time,
        data->play_time / 3600.0f);
    printf("================\n");
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    /* 获取存档路径 */
    // char *pref_path = SDL_GetPrefPath("MyGame", "SaveDemo");
    // char save_path[256];
    // snprintf(
    //     save_path, sizeof(save_path), "%ssave.dat",
    //     pref_path ? pref_path : "./");
    // char save_text_path[256];
    // snprintf(
    //     save_text_path, sizeof(save_text_path), "%ssave.json",
    //     pref_path ? pref_path : "./");
    const char *save_path = "./save.dat";
    const char *save_text_path = "./save.json";

    printf("存档路径: %s\n", save_path);

    SaveData save_data;
    InitSaveData(&save_data);

    printf("\n初始存档数据:\n");
    PrintSaveData(&save_data);

    /* 保存二进制存档 */
    printf("\n保存二进制存档...\n");
    SaveGame_Binary(save_path, &save_data);

    /* 保存文本存档 */
    printf("\n保存文本存档...\n");
    SaveGame_Text(save_text_path, &save_data);

    /* 清空数据，模拟重新启动 */
    printf("\n清空内存数据...\n");
    memset(&save_data, 0, sizeof(SaveData));

    /* 加载二进制存档 */
    printf("\n加载二进制存档...\n");
    if (LoadGame_Binary(save_path, &save_data)) {
        PrintSaveData(&save_data);
    }

    // if (pref_path)
    //     SDL_free(pref_path);
    SDL_Quit();
    return 0;
}
