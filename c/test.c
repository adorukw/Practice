#include <stdio.h>

#define ROWS 20
#define COLS 20
#define CELL_WIDTH 4  // 每个单元格宽度

int main() {
    int grid[ROWS][COLS];

    // 填充示例数据
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = i * COLS + j + 1;
        }
    }

    // 打印列序号（顶部）
    printf("%*s", CELL_WIDTH, "");  // 行号占位
    for (int j = 0; j < COLS; j++) {
        printf("%*d", CELL_WIDTH, j + 1);
    }
    printf("\n");

    // // 可选分隔线
    // printf("%*s", CELL_WIDTH, "");
    // for (int j = 0; j < COLS; j++) {
    //     printf("%-*s", CELL_WIDTH, "----");
    // }
    // printf("\n");

    // 打印每行：行号 + 数据
    for (int i = 0; i < ROWS; i++) {
        printf("%*d", CELL_WIDTH, i + 1);  // 行号
        for (int j = 0; j < COLS; j++) {
            printf("%*d", CELL_WIDTH, grid[i][j]);
        }
        printf("\n");
    }

    return 0;
}