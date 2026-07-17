#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double x, y;
    int width, height;
} TransformComponent;

typedef struct {
    double vx, vy;
} PhysicsComponent;

typedef struct {
    Uint8 r, g, b, a;
} RenderComponent;

typedef struct {
    int maxHp;
    int currentHp;
    SDL_bool invincible;
} HealthComponent;

typedef struct {
    double patrolMinX, patrolMaxX;
    double speed;
    int direction;
} AIComponent;

const int MAX_COMPONENTS = 8;

typedef enum {
    COMP_TRANSFORM = 0,
    COMP_PHYSICS,
    COMP_RENDER,
    COMP_HEALTH,
    COMP_AI,
    COMP_COUNT
} ComponentType;

typedef struct {
    int id;
    char name[32];
    SDL_bool active;

    void *components[COMP_COUNT];
    SDL_bool hasComponent[COMP_COUNT];
} GameObject;

GameObject *GameObjectCreate(const char *name) {
    static int nextId = 1;
    GameObject *obj = (GameObject *)calloc(1, sizeof(GameObject));
    obj->id = nextId++;
    strncpy(obj->name, name, 31);
    obj->active = SDL_TRUE;
    return obj;
}

void GameObjectAddComponent(GameObject *obj, ComponentType type, void *comp) {
    obj->components[type] = comp;
    obj->hasComponent[type] = SDL_TRUE;
}

void *GameObjectGetComponent(GameObject *obj, ComponentType type) {
    if (!obj->hasComponent[type]) {
        return NULL;
    }
    return obj->components[type];
}

void GameObjectDestroy(GameObject *obj) {
    for (int i = 0; i < COMP_COUNT; i++) {
        if (obj->hasComponent[i] && obj->components[i]) {
            free(obj->components[i]);
        }
    }
    free(obj);
}

void InputSystemUpdate(GameObject *obj, double delta) {
    if (!obj->active)
        return;
    PhysicsComponent *phys =
        (PhysicsComponent *)GameObjectGetComponent(obj, COMP_PHYSICS);
    if (!phys)
        return;

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    double speed = 200;
    phys->vx = 0;
    phys->vy = 0;
    if (keys[SDL_SCANCODE_LEFT]) {
        phys->vx = -speed;
    }

    if (keys[SDL_SCANCODE_RIGHT]) {
        phys->vx = speed;
    }

    if (keys[SDL_SCANCODE_UP]) {
        phys->vy = -speed;
    }

    if (keys[SDL_SCANCODE_DOWN]) {
        phys->vy = speed;
    }
}

void AISystemUpdate(GameObject *obj, double delta) {
    if (!obj->active)
        return;
    AIComponent *ai = (AIComponent *)GameObjectGetComponent(obj, COMP_AI);
    TransformComponent *trans =
        (TransformComponent *)GameObjectGetComponent(obj, COMP_TRANSFORM);
    PhysicsComponent *phys =
        (PhysicsComponent *)GameObjectGetComponent(obj, COMP_PHYSICS);
    if (!ai || !trans || !phys) {
        return;
    }

    phys->vx = ai->speed * ai->direction;
    phys->vy = 0;
    if (trans->x <= ai->patrolMinX)
        ai->direction = 1;
    if (trans->x >= ai->patrolMaxX)
        ai->direction = -1;
}

void PhysicsSystemUpdate(GameObject *obj, double delta) {
    if (!obj->active) {
        return;
    }
    TransformComponent *trans =
        (TransformComponent *)GameObjectGetComponent(obj, COMP_TRANSFORM);
    PhysicsComponent *phys =
        (PhysicsComponent *)GameObjectGetComponent(obj, COMP_PHYSICS);
    if (!trans || !phys) {
        return;
    }

    trans->x += phys->vx * delta;
    trans->y += phys->vy * delta;
}

void RenderSystemRender(GameObject *obj, SDL_Renderer *renderer) {
    if (!obj->active) {
        return;
    }
    TransformComponent *trans =
        (TransformComponent *)GameObjectGetComponent(obj, COMP_TRANSFORM);
    RenderComponent *render =
        (RenderComponent *)GameObjectGetComponent(obj, COMP_RENDER);
    if (!trans || !render) {
        return;
    }

    SDL_SetRenderDrawColor(
        renderer, render->r, render->g, render->b, render->a);
    SDL_Rect rect = { (int)trans->x, (int)trans->y, trans->width,
                      trans->height };
    SDL_RenderFillRect(renderer, &rect);
}

GameObject *CreatePlayer(double x, double y) {
    GameObject *obj = GameObjectCreate("Player");

    TransformComponent *trans = calloc(1, sizeof(TransformComponent));
    trans->x = x;
    trans->y = y;
    trans->width = 30;
    trans->height = 30;
    GameObjectAddComponent(obj, COMP_TRANSFORM, trans);

    PhysicsComponent *phys = calloc(1, sizeof(PhysicsComponent));
    GameObjectAddComponent(obj, COMP_PHYSICS, phys);

    RenderComponent *render = calloc(1, sizeof(RenderComponent));
    render->r = 100;
    render->g = 200;
    render->b = 255;
    render->a = 255;
    GameObjectAddComponent(obj, COMP_RENDER, render);

    HealthComponent *health = calloc(1, sizeof(HealthComponent));
    health->maxHp = 100;
    health->currentHp = 100;
    GameObjectAddComponent(obj, COMP_HEALTH, health);

    return obj;
}

GameObject *CreatePatrolEnemy(double x, double y, double min_x, double max_x) {
    GameObject *obj = GameObjectCreate("PatrolEnemy");

    TransformComponent *trans = calloc(1, sizeof(TransformComponent));
    trans->x = x;
    trans->y = y;
    trans->width = 25;
    trans->height = 25;
    GameObjectAddComponent(obj, COMP_TRANSFORM, trans);

    PhysicsComponent *phys = calloc(1, sizeof(PhysicsComponent));
    GameObjectAddComponent(obj, COMP_PHYSICS, phys);

    RenderComponent *render = calloc(1, sizeof(RenderComponent));
    render->r = 255;
    render->g = 100;
    render->b = 100;
    render->a = 255;
    GameObjectAddComponent(obj, COMP_RENDER, render);

    AIComponent *ai = calloc(1, sizeof(AIComponent));
    ai->patrolMinX = min_x;
    ai->patrolMaxX = max_x;
    ai->speed = 100.0f;
    ai->direction = 1;
    GameObjectAddComponent(obj, COMP_AI, ai);

    return obj;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "组件模式示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
        600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    GameObject *player = CreatePlayer(400, 300);
    GameObject *enemy1 = CreatePatrolEnemy(100, 200, 50, 350);
    GameObject *enemy2 = CreatePatrolEnemy(500, 400, 450, 750);

    GameObject *objects[] = { player, enemy1, enemy2 };
    int objectCount = 3;

    SDL_bool running = SDL_TRUE;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        double delta = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;
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

        InputSystemUpdate(player, delta);
        AISystemUpdate(enemy1, delta);
        AISystemUpdate(enemy2, delta);

        for (int i = 0; i < objectCount; i++) {
            PhysicsSystemUpdate(objects[i], delta);
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < objectCount; i++) {
            RenderSystemRender(objects[i], renderer);
        }
        SDL_RenderPresent(renderer);
    }
    for (int i = 0; i < objectCount; i++) {
        GameObjectDestroy(objects[i]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
