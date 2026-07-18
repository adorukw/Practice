#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_W 800
#define WINDOW_H 600
#define CELL_SIZE 80
#define GRID_COLS (WINDOW_W / CELL_SIZE + 1)
#define GRID_ROWS (WINDOW_H / CELL_SIZE + 1)
#define MAX_OBJECTS_PER_CELL 64
#define MAX_OBJECTS 500

/* 游戏对象 */
typedef struct {
    int id;
    float x, y;
    float vx, vy;
    int radius;
    SDL_bool active;
} GameObject;

/* 网格单元 */
typedef struct {
    int object_ids[MAX_OBJECTS_PER_CELL];
    int count;
} GridCell;

/* 网格 */
typedef struct {
    GridCell cells[GRID_COLS][GRID_ROWS];
} Grid;

void Grid_Clear(Grid *grid) { memset(grid, 0, sizeof(Grid)); }

/* 计算对象所在的网格坐标 */
void Grid_GetCell(float x, float y, int *cx, int *cy) {
    *cx = (int)(x / CELL_SIZE);
    *cy = (int)(y / CELL_SIZE);
    if (*cx < 0)
        *cx = 0;
    if (*cx >= GRID_COLS)
        *cx = GRID_COLS - 1;
    if (*cy < 0)
        *cy = 0;
    if (*cy >= GRID_ROWS)
        *cy = GRID_ROWS - 1;
}

/* 将对象插入网格 */
void Grid_Insert(Grid *grid, GameObject *obj) {
    int cx, cy;
    Grid_GetCell(obj->x, obj->y, &cx, &cy);
    GridCell *cell = &grid->cells[cx][cy];
    if (cell->count < MAX_OBJECTS_PER_CELL) {
        cell->object_ids[cell->count++] = obj->id;
    }
}

/* 查询附近对象（当前单元 + 相邻 8 单元） */
void Grid_Query(
    Grid *grid, float x, float y, float radius, int *result_ids,
    int *result_count, int max_results) {
    int cx, cy;
    Grid_GetCell(x, y, &cx, &cy);
    *result_count = 0;

    /* 计算需要查询的单元范围（考虑半径） */
    int min_cx = cx, max_cx = cx, min_cy = cy, max_cy = cy;
    int r_cells = (int)(radius / CELL_SIZE) + 1;
    min_cx -= r_cells;
    max_cx += r_cells;
    min_cy -= r_cells;
    max_cy += r_cells;
    if (min_cx < 0)
        min_cx = 0;
    if (max_cx >= GRID_COLS)
        max_cx = GRID_COLS - 1;
    if (min_cy < 0)
        min_cy = 0;
    if (max_cy >= GRID_ROWS)
        max_cy = GRID_ROWS - 1;

    for (int i = min_cx; i <= max_cx; i++) {
        for (int j = min_cy; j <= max_cy; j++) {
            GridCell *cell = &grid->cells[i][j];
            for (int k = 0; k < cell->count; k++) {
                int id = cell->object_ids[k];
                /* 去重（简化：实际应用哈希集合） */
                SDL_bool found = SDL_FALSE;
                for (int r = 0; r < *result_count; r++) {
                    if (result_ids[r] == id) {
                        found = SDL_TRUE;
                        break;
                    }
                }
                if (!found && *result_count < max_results) {
                    result_ids[(*result_count)++] = id;
                }
            }
        }
    }
}

/* 圆形碰撞检测 */
SDL_bool CheckCircleCollision(GameObject *a, GameObject *b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dist_sq = dx * dx + dy * dy;
    float radius_sum = a->radius + b->radius;
    return dist_sq < radius_sum * radius_sum;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "空间分区示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 创建大量对象 */
    GameObject objects[MAX_OBJECTS];
    int object_count = 300;
    for (int i = 0; i < object_count; i++) {
        objects[i].id = i;
        objects[i].x = (float)(rand() % WINDOW_W);
        objects[i].y = (float)(rand() % WINDOW_H);
        objects[i].radius = 5 + rand() % 10;
        objects[i].active = SDL_TRUE;
        /* 随机速度 */
        objects[i].vx = (float)(rand() % 100 - 50);
        objects[i].vy = (float)(rand() % 100 - 50);
    }

    Grid grid;
    SDL_bool running = SDL_TRUE;
    Uint32 last_time = SDL_GetTicks();
    int collision_count = 0;
    int check_count = 0;

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        if (delta > 0.1f)
            delta = 0.1f;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = SDL_FALSE;
        }

        /* 更新对象位置 */
        for (int i = 0; i < object_count; i++) {
            if (!objects[i].active)
                continue;
            objects[i].x += objects[i].vx * delta;
            objects[i].y += objects[i].vy * delta;
            /* 边界反弹 */
            if (objects[i].x < 0 || objects[i].x > WINDOW_W)
                objects[i].vx *= -1;
            if (objects[i].y < 0 || objects[i].y > WINDOW_H)
                objects[i].vy *= -1;
        }

        /* 清空并重建网格 */
        Grid_Clear(&grid);
        for (int i = 0; i < object_count; i++) {
            if (objects[i].active)
                Grid_Insert(&grid, &objects[i]);
        }

        /* 使用空间分区进行碰撞检测 */
        collision_count = 0;
        check_count = 0;
        for (int i = 0; i < object_count; i++) {
            if (!objects[i].active)
                continue;
            /* 查询附近对象 */
            int nearby[64];
            int nearby_count;
            Grid_Query(
                &grid, objects[i].x, objects[i].y, objects[i].radius * 2,
                nearby, &nearby_count, 64);

            for (int j = 0; j < nearby_count; j++) {
                int id = nearby[j];
                if (id <= i)
                    continue; /* 避免重复检测 */
                if (!objects[id].active)
                    continue;
                check_count++;
                if (CheckCircleCollision(&objects[i], &objects[id])) {
                    collision_count++;
                    /* 碰撞处理：简单分离 */
                    float dx = objects[id].x - objects[i].x;
                    float dy = objects[id].y - objects[i].y;
                    float dist = sqrtf(dx * dx + dy * dy);
                    if (dist > 0) {
                        float overlap =
                            (objects[i].radius + objects[id].radius - dist) / 2;
                        objects[i].x -= dx / dist * overlap;
                        objects[i].y -= dy / dist * overlap;
                        objects[id].x += dx / dist * overlap;
                        objects[id].y += dy / dist * overlap;
                    }
                }
            }
        }

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        /* 绘制网格（可视化） */
        SDL_SetRenderDrawColor(renderer, 40, 40, 50, 255);
        for (int i = 0; i <= GRID_COLS; i++) {
            SDL_RenderDrawLine(
                renderer, i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_H);
        }
        for (int j = 0; j <= GRID_ROWS; j++) {
            SDL_RenderDrawLine(
                renderer, 0, j * CELL_SIZE, WINDOW_W, j * CELL_SIZE);
        }

        /* 绘制对象 */
        for (int i = 0; i < object_count; i++) {
            if (!objects[i].active)
                continue;
            SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
            /* 用矩形近似圆形 */
            SDL_Rect r = { (int)objects[i].x - objects[i].radius,
                           (int)objects[i].y - objects[i].radius,
                           objects[i].radius * 2, objects[i].radius * 2 };
            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);

        /* 每秒打印统计 */
        static Uint32 stat_timer = 0;
        static int frame_count = 0;
        frame_count++;
        if (current_time - stat_timer >= 1000) {
            printf(
                "对象: %d, 检测次数: %d, 碰撞: %d (暴力检测需要 %d 次)\n",
                object_count, check_count, collision_count,
                object_count * (object_count - 1) / 2);
            stat_timer = current_time;
            frame_count = 0;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
