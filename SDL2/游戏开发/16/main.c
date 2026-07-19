#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

/* ============ 控件基类 ============ */

typedef struct Widget Widget;

/* 事件类型 */
typedef enum { UI_EVENT_CLICK, UI_EVENT_HOVER, UI_EVENT_KEY } UIEventType;

typedef struct {
    UIEventType type;
    int x, y; /* 鼠标位置 */
    int key;  /* 按键 */
} UIEvent;

/* 控件基类结构 */
struct Widget {
    char name[32];
    int x, y; /* 相对父控件位置 */
    int width, height;
    SDL_bool visible;
    SDL_bool enabled;
    SDL_bool hovered;
    SDL_bool pressed;

    Widget *parent;
    Widget *children[16];
    int child_count;

    /* 虚函数 */
    void (*render)(Widget *self, SDL_Renderer *r, int abs_x, int abs_y);
    SDL_bool (*handle_event)(
        Widget *self, UIEvent *event, int abs_x, int abs_y);

    /* 点击回调 */
    void (*on_click)(Widget *self, void *user_data);
    void *user_data;
};

void Widget_Init(
    Widget *w, const char *name, int x, int y, int width, int height) {
    strncpy(w->name, name, 31);
    w->name[31] = '\0';
    w->x = x;
    w->y = y;
    w->width = width;
    w->height = height;
    w->visible = SDL_TRUE;
    w->enabled = SDL_TRUE;
    w->hovered = SDL_FALSE;
    w->pressed = SDL_FALSE;
    w->parent = NULL;
    w->child_count = 0;
    w->render = NULL;
    w->handle_event = NULL;
    w->on_click = NULL;
    w->user_data = NULL;
}

void Widget_AddChild(Widget *parent, Widget *child) {
    if (parent->child_count < 16) {
        parent->children[parent->child_count++] = child;
        child->parent = parent;
    }
}

/* 检查点是否在控件内 */
SDL_bool Widget_Contains(Widget *w, int abs_x, int abs_y, int px, int py) {
    int wx = px + w->x, wy = py + w->y;
    return (
        abs_x >= wx && abs_x < wx + w->width && abs_y >= wy &&
        abs_y < wy + w->height);
}

/* 事件分发 */
SDL_bool Widget_DispatchEvent(Widget *w, UIEvent *event, int px, int py) {
    if (!w->visible || !w->enabled)
        return SDL_FALSE;

    int abs_x = px + w->x, abs_y = py + w->y;

    /* 先分发给子控件（从后向前，后绘制的在上层） */
    for (int i = w->child_count - 1; i >= 0; i--) {
        if (Widget_DispatchEvent(w->children[i], event, abs_x, abs_y)) {
            return SDL_TRUE;
        }
    }

    /* 检查自身 */
    if (event->type == UI_EVENT_CLICK) {
        if (Widget_Contains(w, event->x, event->y, px, py)) {
            w->pressed = SDL_TRUE;
            return SDL_TRUE;
        }
    }
    if (event->type == UI_EVENT_HOVER) {
        SDL_bool was_hovered = w->hovered;
        w->hovered = Widget_Contains(w, event->x, event->y, px, py);
        if (w->hovered && !was_hovered) {
            /* 鼠标进入 */
        }
    }

    /* 自身处理 */
    if (w->handle_event) {
        return w->handle_event(w, event, abs_x, abs_y);
    }
    return SDL_FALSE;
}

/* 渲染分发 */
void Widget_Render(Widget *w, SDL_Renderer *r, int px, int py) {
    if (!w->visible)
        return;

    int abs_x = px + w->x, abs_y = py + w->y;

    /* 渲染自身 */
    if (w->render) {
        w->render(w, r, abs_x, abs_y);
    }

    /* 渲染子控件 */
    for (int i = 0; i < w->child_count; i++) {
        Widget_Render(w->children[i], r, abs_x, abs_y);
    }
}

/* ============ 按钮控件 ============ */

typedef struct {
    Widget base;
    char text[32];
    Uint8 bg_r, bg_g, bg_b;
    Uint8 hover_r, hover_g, hover_b;
    Uint8 press_r, press_g, press_b;
} Button;

void Button_Render(Widget *self, SDL_Renderer *r, int abs_x, int abs_y) {
    Button *btn = (Button *)self;
    Uint8 cr, cg, cb;
    if (self->pressed) {
        cr = btn->press_r;
        cg = btn->press_g;
        cb = btn->press_b;
    } else if (self->hovered) {
        cr = btn->hover_r;
        cg = btn->hover_g;
        cb = btn->hover_b;
    } else {
        cr = btn->bg_r;
        cg = btn->bg_g;
        cb = btn->bg_b;
    }
    SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
    SDL_Rect rect = { abs_x, abs_y, self->width, self->height };
    SDL_RenderFillRect(r, &rect);

    /* 边框 */
    SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
    SDL_RenderDrawRect(r, &rect);

    /* 文字（用矩形简化） */
    int text_w = strlen(btn->text) * 8;
    int text_x = abs_x + (self->width - text_w) / 2;
    int text_y = abs_y + (self->height - 12) / 2;
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    /* 简化：画一个小矩形表示文字 */
    SDL_Rect text_rect = { text_x, text_y, text_w, 12 };
    SDL_RenderFillRect(r, &text_rect);
}

SDL_bool
Button_HandleEvent(Widget *self, UIEvent *event, int abs_x, int abs_y) {
    if (event->type == UI_EVENT_CLICK) {
        if (Widget_Contains(
                self, event->x, event->y, self->parent ? (self->parent->x) : 0,
                self->parent ? (self->parent->y) : 0)) {
            /* 这里简化处理，实际应在鼠标释放时触发 */
        }
    }
    return SDL_FALSE;
}

Button *
Button_Create(const char *name, const char *text, int x, int y, int w, int h) {
    Button *btn = calloc(1, sizeof(Button));
    Widget_Init(&btn->base, name, x, y, w, h);
    strncpy(btn->text, text, 31);
    btn->text[31] = '\0';
    btn->base.render = Button_Render;
    btn->base.handle_event = Button_HandleEvent;
    btn->bg_r = 80;
    btn->bg_g = 120;
    btn->bg_b = 200;
    btn->hover_r = 100;
    btn->hover_g = 140;
    btn->hover_b = 220;
    btn->press_r = 60;
    btn->press_g = 100;
    btn->press_b = 180;
    return btn;
}

/* ============ 面板控件 ============ */

typedef struct {
    Widget base;
    Uint8 bg_r, bg_g, bg_b, bg_a;
} Panel;

void Panel_Render(Widget *self, SDL_Renderer *r, int abs_x, int abs_y) {
    Panel *p = (Panel *)self;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, p->bg_r, p->bg_g, p->bg_b, p->bg_a);
    SDL_Rect rect = { abs_x, abs_y, self->width, self->height };
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 150, 150, 150, 255);
    SDL_RenderDrawRect(r, &rect);
}

Panel *Panel_Create(const char *name, int x, int y, int w, int h) {
    Panel *p = calloc(1, sizeof(Panel));
    Widget_Init(&p->base, name, x, y, w, h);
    p->base.render = Panel_Render;
    p->bg_r = 40;
    p->bg_g = 40;
    p->bg_b = 50;
    p->bg_a = 220;
    return p;
}

/* ============ HUD 元素：血条 ============ */

typedef struct {
    Widget base;
    int current;
    int max_value;
} HealthBar;

void HealthBar_Render(Widget *self, SDL_Renderer *r, int abs_x, int abs_y) {
    HealthBar *hb = (HealthBar *)self;
    /* 背景 */
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);
    SDL_Rect bg = { abs_x, abs_y, self->width, self->height };
    SDL_RenderFillRect(r, &bg);
    /* 填充 */
    float ratio = (float)hb->current / hb->max_value;
    SDL_SetRenderDrawColor(
        r, (Uint8)(255 * (1 - ratio)), (Uint8)(200 * ratio), 50, 255);
    SDL_Rect fill = { abs_x, abs_y, (int)(self->width * ratio), self->height };
    SDL_RenderFillRect(r, &fill);
    /* 边框 */
    SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
    SDL_RenderDrawRect(r, &bg);
}

HealthBar *HealthBar_Create(const char *name, int x, int y, int w, int h) {
    HealthBar *hb = calloc(1, sizeof(HealthBar));
    Widget_Init(&hb->base, name, x, y, w, h);
    hb->base.render = HealthBar_Render;
    hb->current = 100;
    hb->max_value = 100;
    return hb;
}

/* ============ 主程序 ============ */

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "UI 系统示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
        SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* 根控件（桌面） */
    Widget desktop;
    Widget_Init(&desktop, "Desktop", 0, 0, 800, 600);

    /* HUD 面板 */
    Panel *hud = Panel_Create("HUD", 10, 10, 200, 80);
    hud->bg_a = 150;
    Widget_AddChild(&desktop, &hud->base);

    HealthBar *hp_bar = HealthBar_Create("HP", 10, 10, 180, 20);
    Widget_AddChild(&hud->base, &hp_bar->base);

    /* 主菜单面板 */
    Panel *menu = Panel_Create("Menu", 250, 200, 300, 200);
    Widget_AddChild(&desktop, &menu->base);

    Button *btn_start = Button_Create("Start", "开始游戏", 50, 30, 200, 40);
    Widget_AddChild(&menu->base, &btn_start->base);

    Button *btn_quit = Button_Create("Quit", "退出游戏", 50, 100, 200, 40);
    Widget_AddChild(&menu->base, &btn_quit->base);

    int player_hp = 100;
    SDL_bool menu_visible = SDL_TRUE;
    SDL_bool running = SDL_TRUE;

    printf("操作: 鼠标点击按钮, H切换菜单, +/-调整血量, ESC退出\n");

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    running = SDL_FALSE;
                if (event.key.keysym.scancode == SDL_SCANCODE_H)
                    menu_visible = !menu_visible;
                if (event.key.keysym.scancode == SDL_SCANCODE_EQUALS)
                    player_hp = (player_hp + 10 > 100) ? 100 : player_hp + 10;
                if (event.key.keysym.scancode == SDL_SCANCODE_MINUS)
                    player_hp = (player_hp - 10 < 0) ? 0 : player_hp - 10;
            }
            if (event.type == SDL_MOUSEMOTION) {
                UIEvent ui_event = { .type = UI_EVENT_HOVER,
                                     .x = event.motion.x,
                                     .y = event.motion.y };
                Widget_DispatchEvent(&desktop, &ui_event, 0, 0);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == 1) {
                UIEvent ui_event = { .type = UI_EVENT_CLICK,
                                     .x = event.button.x,
                                     .y = event.button.y };
                Widget_DispatchEvent(&desktop, &ui_event, 0, 0);

                /* 检查按钮点击 */
                if (menu_visible) {
                    int mx = event.button.x, my = event.button.y;
                    /* 检查开始按钮 */
                    if (mx >= 300 && mx < 500 && my >= 230 && my < 270) {
                        printf("点击: 开始游戏\n");
                    }
                    /* 检查退出按钮 */
                    if (mx >= 300 && mx < 500 && my >= 300 && my < 340) {
                        printf("点击: 退出游戏\n");
                        running = SDL_FALSE;
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                /* 重置所有控件的 pressed 状态 */
                btn_start->base.pressed = SDL_FALSE;
                btn_quit->base.pressed = SDL_FALSE;
            }
        }

        /* 更新血条 */
        hp_bar->current = player_hp;

        /* 设置菜单可见性 */
        menu->base.visible = menu_visible;

        /* 渲染 */
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        /* 模拟游戏场景（背景） */
        SDL_SetRenderDrawColor(renderer, 50, 60, 80, 255);
        SDL_Rect game_bg = { 0, 0, 800, 600 };
        SDL_RenderFillRect(renderer, &game_bg);

        /* 渲染 UI */
        Widget_Render(&desktop, renderer, 0, 0);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    free(hud);
    free(hp_bar);
    free(menu);
    free(btn_start);
    free(btn_quit);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
