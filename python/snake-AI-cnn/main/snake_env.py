import pygame
import random
import numpy as np

# 可修改参数
GRID_WIDTH = 10         # 网格宽度（列数）
GRID_HEIGHT = 10        # 网格高度（行数）
BLOCK_SIZE = 100         # 渲染时的方块大小(像素)
REWARD_FOOD = 10.0      # 吃到食物的奖励
REWARD_DEATH = -10.0    # 死亡的惩罚（撞墙或撞自己）
REWARD_STEP = -0.1      # 每走一步的惩罚（鼓励快速吃食物）
MAX_STEPS = 500         # 每回合最大步数（防止死循环）

# 颜色定义 (用于渲染)
WHITE = (255, 255, 255)
RED = (200, 0, 0)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)
BLACK = (0, 0, 0)


class SnakeEnv:
    def __init__(self):
        self.width = GRID_WIDTH
        self.height = GRID_HEIGHT
        self.display = None
        self.clock = None
        self.reset()

    def reset(self):
        # 初始化蛇的位置 (在中间)
        self.head = [self.width // 2, self.height // 2]
        self.snake = [self.head.copy()]
        # 初始方向：0-上, 1-右, 2-下, 3-左
        self.direction = 1
        self.score = 0
        self.steps = 0
        self._place_food()
        self.game_over = False
        return self._get_state()

    def _place_food(self):
        while True:
            self.food = [
                random.randint(0, self.width - 1),
                random.randint(0, self.height - 1)
            ]
            if self.food not in self.snake:
                break

    def step(self, action):
        self.steps += 1

        # 防止蛇直接掉头
        if (action == 0 and self.direction != 2) or \
           (action == 1 and self.direction != 3) or \
           (action == 2 and self.direction != 0) or \
           (action == 3 and self.direction != 1):
            self.direction = action

        # 移动蛇头
        if self.direction == 0:
            self.head[1] -= 1  # 上
        elif self.direction == 1:
            self.head[0] += 1  # 右
        elif self.direction == 2:
            self.head[1] += 1  # 下
        elif self.direction == 3:
            self.head[0] -= 1  # 左

        # 检查是否死亡
        if self._is_collision():
            self.game_over = True
            return self._get_state(), REWARD_DEATH, self.game_over

        # 检查是否超时
        if self.steps > MAX_STEPS:
            self.game_over = True
            return self._get_state(), REWARD_DEATH, self.game_over

        self.snake.insert(0, self.head.copy())

        # 检查是否吃到食物
        reward = REWARD_STEP
        if self.head == self.food:
            self.score += 1
            reward = REWARD_FOOD
            self._place_food()
        else:
            self.snake.pop()  # 没吃到食物，尾巴缩进

        return self._get_state(), reward, self.game_over

    def _is_collision(self):
        # 撞墙
        if self.head[0] < 0 or self.head[0] >= self.width or \
           self.head[1] < 0 or self.head[1] >= self.height:
            return True
        # 撞自己 (除去头部的身体)
        if self.head in self.snake[1:]:
            return True
        return False

    def _get_state(self):
        # 生成适合 CNN 处理的 3 通道图像状态: (Channels, Height, Width)
        # Channel 0: 食物
        # Channel 1: 蛇头
        # Channel 2: 蛇身
        state = np.zeros((3, self.height, self.width), dtype=np.float32)

        # 食物通道
        state[0, self.food[1], self.food[0]] = 1.0

        # 蛇头通道
        # 注意边界检查，因为 _is_collision 之后可能会越界
        if 0 <= self.head[1] < self.height and 0 <= self.head[0] < self.width:
            state[1, self.head[1], self.head[0]] = 1.0

        # 蛇身通道
        for pt in self.snake[1:]:
            if 0 <= pt[1] < self.height and 0 <= pt[0] < self.width:
                state[2, pt[1], pt[0]] = 1.0

        return state

    def render(self, fps=10):
        if self.display is None:
            pygame.init()
            self.display = pygame.display.set_mode(
                (self.width * BLOCK_SIZE, self.height * BLOCK_SIZE))
            pygame.display.set_caption('Snake DQN CNN')
            self.clock = pygame.time.Clock()

        self.display.fill(BLACK)

        # 画食物
        pygame.draw.rect(self.display, RED, pygame.Rect(
            self.food[0]*BLOCK_SIZE, self.food[1]*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))

        # 画蛇
        for i, pt in enumerate(self.snake):
            color = BLUE if i == 0 else GREEN  # 头和身体区分颜色
            pygame.draw.rect(self.display, color, pygame.Rect(
                pt[0]*BLOCK_SIZE, pt[1]*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
            # 画个内框让蛇看起来更有层次
            pygame.draw.rect(self.display, BLACK, pygame.Rect(
                pt[0]*BLOCK_SIZE+4, pt[1]*BLOCK_SIZE+4, BLOCK_SIZE-8, BLOCK_SIZE-8))

        # 显示分数
        font = pygame.font.Font(None, 36)
        text = font.render('Score: %d' % self.score, True, WHITE)
        self.display.blit(text, (10, 10))

        pygame.display.flip()
        self.clock.tick(fps)
