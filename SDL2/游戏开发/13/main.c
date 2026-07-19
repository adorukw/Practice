#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define GRID_W 40
#define GRID_H 30
#define CELL_SIZE 20

/* 网格类型 */
typedef enum {
    CELL_EMPTY = 0,
    CELL_WALL,
    CELL_START,
    CELL_END,
    CELL_PATH,
    CELL_VISITED
} CellType;

CellType grid[GRID_W][GRID_H];

/* A* 节点 */
typedef struct {
    int x, y;
    float g, h, f;
    int parent_x, parent_y;
    SDL_bool in_open;
    SDL_bool in_closed;
} AStarNode;

AStarNode nodes[GRID_W][GRID_H];

/* 优先队列（简化实现：线性查找最小值） */
typedef struct {
    AStarNode *data[GRID_W * GRID_H];
    int count;
} PriorityQueue;

void PQ_Init(PriorityQueue *pq) { pq->count = 0; }

void PQ_Push(PriorityQueue *pq, AStarNode *node) {
    pq->data[pq->count++] = node;
}

AStarNode *PQ_PopMin(PriorityQueue *pq) {
    if (pq->count == 0)
        return NULL;
    int min_idx = 0;
    for (int i = 1; i < pq->count; i++) {
        if (pq->data[i]->f < pq->data[min_idx]->f) {
            min_idx = i;
        }
    }
    AStarNode *min_node = pq->data[min_idx];
    pq->data[min_idx] = pq->data[pq->count - 1];
    pq->count--;
    return min_node;
}

/* 启发函数：曼哈顿距离 */
float Heuristic(int x1, int y1, int x2, int y2) {
    return (float)(abs(x1 - x2) + abs(y1 - y2));
}

/* 8方向移动 */
int directions[8][2] = {
    { 0, -1 },  { 0, 1 },  { -1, 0 }, { 1, 0 }, /* 上下左右 */
    { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }  /* 对角线 */
};
float dir_costs[8] = { 1, 1, 1, 1, 1.414f, 1.414f, 1.414f, 1.414f };

/* A* 寻路 */
SDL_bool AStar_FindPath(int sx, int sy, int ex, int ey) {
    /* 初始化节点 */
    for (int x = 0; x < GRID_W; x++) {
        for (int y = 0; y < GRID_H; y++) {
            nodes[x][y].x = x;
            nodes[x][y].y = y;
            nodes[x][y].g = 0;
            nodes[x][y].h = 0;
            nodes[x][y].f = 0;
            nodes[x][y].parent_x = -1;
            nodes[x][y].parent_y = -1;
            nodes[x][y].in_open = SDL_FALSE;
            nodes[x][y].in_closed = SDL_FALSE;
        }
    }

    PriorityQueue open;
    PQ_Init(&open);

    /* 起点 */
    AStarNode *start = &nodes[sx][sy];
    start->g = 0;
    start->h = Heuristic(sx, sy, ex, ey);
    start->f = start->g + start->h;
    start->in_open = SDL_TRUE;
    PQ_Push(&open, start);

    while (open.count > 0) {
        AStarNode *current = PQ_PopMin(&open);
        current->in_open = SDL_FALSE;
        current->in_closed = SDL_TRUE;

        /* 标记已访问（可视化） */
        if (grid[current->x][current->y] == CELL_EMPTY) {
            grid[current->x][current->y] = CELL_VISITED;
        }

        /* 到达终点 */
        if (current->x == ex && current->y == ey) {
            /* 重建路径 */
            int cx = ex, cy = ey;
            while (cx != sx || cy != sy) {
                if (grid[cx][cy] == CELL_EMPTY ||
                    grid[cx][cy] == CELL_VISITED) {
                    grid[cx][cy] = CELL_PATH;
                }
                int px = nodes[cx][cy].parent_x;
                int py = nodes[cx][cy].parent_y;
                cx = px;
                cy = py;
            }
            return SDL_TRUE;
        }

        /* 遍历邻居 */
        for (int i = 0; i < 8; i++) {
            int nx = current->x + directions[i][0];
            int ny = current->y + directions[i][1];

            /* 边界检查 */
            if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H)
                continue;
            /* 障碍检查 */
            if (grid[nx][ny] == CELL_WALL)
                continue;
            /* 对角线移动时检查角落（防止穿墙角） */
            if (i >= 4) {
                int dx = directions[i][0];
                int dy = directions[i][1];
                if (grid[current->x + dx][current->y] == CELL_WALL ||
                    grid[current->x][current->y + dy] == CELL_WALL) {
                    continue;
                }
            }

            AStarNode *neighbor = &nodes[nx][ny];
            if (neighbor->in_closed)
                continue;

            float tentative_g = current->g + dir_costs[i];

            if (!neighbor->in_open) {
                neighbor->g = tentative_g;
                neighbor->h = Heuristic(nx, ny, ex, ey);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent_x = current->x;
                neighbor->parent_y = current->y;
                neighbor->in_open = SDL_TRUE;
                PQ_Push(&open, neighbor);
            } else if (tentative_g < neighbor->g) {
                neighbor->g = tentative_g;
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent_x = current->x;
                neighbor->parent_y = current->y;
            }
        }
    }
    return SDL_FALSE; /* 无路径 */
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "A* 寻路示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GRID_W * CELL_SIZE, GRID_H * CELL_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 初始化地图 */
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++)
            grid[x][y] = CELL_EMPTY;

    /* 添加一些墙 */
    for (int i = 5; i < 25; i++)
        grid[i][10] = CELL_WALL;
    for (int i = 5; i < 20; i++)
        grid[15][i] = CELL_WALL;
    for (int i = 10; i < 25; i++)
        grid[25][i] = CELL_WALL;

    int start_x = 2, start_y = 2;
    int end_x = 37, end_y = 27;
    grid[start_x][start_y] = CELL_START;
    grid[end_x][end_y] = CELL_END;

    printf(
        "操作: 左键设置起点, 右键设置终点, 中键添加/删除墙, 空格寻路, C清空, "
        "ESC退出\n");

    SDL_bool running = SDL_TRUE;
    SDL_bool path_found = SDL_FALSE;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = SDL_FALSE;
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    /* 清除上次路径 */
                    for (int x = 0; x < GRID_W; x++)
                        for (int y = 0; y < GRID_H; y++)
                            if (grid[x][y] == CELL_PATH ||
                                grid[x][y] == CELL_VISITED)
                                grid[x][y] = CELL_EMPTY;
                    path_found = AStar_FindPath(start_x, start_y, end_x, end_y);
                    printf(path_found ? "找到路径!\n" : "无路径!\n");
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_C) {
                    for (int x = 0; x < GRID_W; x++)
                        for (int y = 0; y < GRID_H; y++)
                            if (grid[x][y] == CELL_PATH ||
                                grid[x][y] == CELL_VISITED)
                                grid[x][y] = CELL_EMPTY;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x / CELL_SIZE;
                int my = event.button.y / CELL_SIZE;
                if (mx >= 0 && mx < GRID_W && my >= 0 && my < GRID_H) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        grid[start_x][start_y] = CELL_EMPTY;
                        start_x = mx;
                        start_y = my;
                        grid[start_x][start_y] = CELL_START;
                    } else if (event.button.button == SDL_BUTTON_RIGHT) {
                        grid[end_x][end_y] = CELL_EMPTY;
                        end_x = mx;
                        end_y = my;
                        grid[end_x][end_y] = CELL_END;
                    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                        if (grid[mx][my] == CELL_WALL)
                            grid[mx][my] = CELL_EMPTY;
                        else if (grid[mx][my] == CELL_EMPTY)
                            grid[mx][my] = CELL_WALL;
                    }
                }
            }
        }

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        for (int x = 0; x < GRID_W; x++) {
            for (int y = 0; y < GRID_H; y++) {
                SDL_Rect cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE,
                                  CELL_SIZE };
                switch (grid[x][y]) {
                case CELL_EMPTY:
                    SDL_SetRenderDrawColor(renderer, 50, 50, 60, 255);
                    break;
                case CELL_WALL:
                    SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
                    break;
                case CELL_START:
                    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
                    break;
                case CELL_END:
                    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
                    break;
                case CELL_PATH:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
                    break;
                case CELL_VISITED:
                    SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);
                    break;
                }
                SDL_RenderFillRect(renderer, &cell);
                /* 网格线 */
                SDL_SetRenderDrawColor(renderer, 40, 40, 50, 255);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
