#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

void GameLoop_ThreeInOne(SDL_Window *window, SDL_Renderer *renderer) {
    // 三个方块的状态
    float x_var = 0.0f;           // 可变步长（蓝色）
    float x_fix_no_interp = 0.0f; // 固定步长无插值（绿色）
    float x_fix_interp = 0.0f;    // 固定步长有插值（橙色）
    float prev_x_fix = 0.0f;      // 用于插值的前一帧位置

    const float SPEED = 350.0f;          // 速度（像素/秒）
    const float FIXED_DT = 1.0f / 60.0f; // 固定步长 1/60 秒
    float accumulator = 0.0f;

    Uint32 last_time = SDL_GetTicks();
    SDL_bool running = SDL_TRUE;
    int frame_count = 0;

    srand(time(NULL));

    while (running) {
        Uint32 now = SDL_GetTicks();
        float frame_time = (now - last_time) / 1000.0f;
        last_time = now;

        // 人为制造帧率抖动：每 30 帧左右插入一次 15ms 延迟
        if (frame_count % 33 == 0) {
            SDL_Delay(15);
            // 重新计算实际帧时间
            now = SDL_GetTicks();
            frame_time = (now - last_time) / 1000.0f;
            last_time = now;
        }
        frame_count++;

        // 钳制最大帧时间
        if (frame_time > 0.08f)
            frame_time = 0.08f;

        // 输入处理
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = SDL_FALSE;
        }

        // ========== 更新三种逻辑 ==========

        // 1. 可变步长（蓝色）
        x_var += SPEED * frame_time;
        if (x_var > 770)
            x_var = 0;

        // 2. 固定步长无插值（绿色）
        accumulator += frame_time;
        while (accumulator >= FIXED_DT) {
            x_fix_no_interp += SPEED * FIXED_DT;
            if (x_fix_no_interp > 730)
                x_fix_no_interp = 0;
            accumulator -= FIXED_DT;
        }
        // 无插值：直接显示当前物理位置（但注意物理位置只在固定步长更新时变化，
        // 这里为了公平，我们在每次物理更新时同步更新
        // x_fix_no_interp，渲染时直接用）

        // 3. 固定步长 + 插值（橙色）
        // 物理更新与无插值共用同一套逻辑，但单独维护插值状态
        // 为了清晰，我们单独写一套
        static float x_fix_interp_phys = 0.0f;
        static float prev_x_interp = 0.0f;
        static float acc_interp = 0.0f;
        acc_interp += frame_time;
        while (acc_interp >= FIXED_DT) {
            prev_x_interp = x_fix_interp_phys;
            x_fix_interp_phys += SPEED * FIXED_DT;
            if (x_fix_interp_phys > 790)
                x_fix_interp_phys = 0;
            acc_interp -= FIXED_DT;
        }
        float alpha = acc_interp / FIXED_DT;
        float render_x_interp =
            prev_x_interp + (x_fix_interp_phys - prev_x_interp) * alpha;

        // ========== 渲染三个方块 ==========
        SDL_SetRenderDrawColor(renderer, 18, 22, 28, 255);
        SDL_RenderClear(renderer);

        // 蓝色 - 可变步长
        SDL_SetRenderDrawColor(renderer, 56, 148, 224, 255);
        SDL_Rect ra = { (int)x_var, 50, 48, 52 };
        SDL_RenderFillRect(renderer, &ra);

        // 绿色 - 固定步长无插值
        SDL_SetRenderDrawColor(renderer, 72, 196, 68, 255);
        SDL_Rect rb = { (int)x_fix_no_interp, 220, 48, 52 };
        SDL_RenderFillRect(renderer, &rb);

        // 橙色 - 固定步长 + 插值
        SDL_SetRenderDrawColor(renderer, 228, 152, 44, 255);
        SDL_Rect rc = { (int)render_x_interp, 390, 48, 52 };
        SDL_RenderFillRect(renderer, &rc);

        SDL_RenderPresent(renderer);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow(
        "步长策略对比（含帧率抖动）", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 840, 500, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    GameLoop_ThreeInOne(win, ren);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
