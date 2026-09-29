import pygame
import random
import numpy as np
from enum import Enum

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


class Direction(Enum):
    RIGHT = 0
    DOWN = 1
    LEFT = 2
    UP = 3


class SnakeEnv:
    def __init__(self):
        self.width = GRID_WIDTH
        self.height = GRID_HEIGHT
        self.display = None
        self.clock = None
        self.reset()

    def reset(self):
        self.head = [self.width // 2, self.height // 2]
        self.snake = [self.head.copy()]
        self.direction = Direction.RIGHT
        self.score = 0
        self.steps = 0
        self._place_food()
        self.game_over = False
        return self._get_state()

    def _place_food(self):
        while True:
            self.food = [
                random.randint(0, self.width-1),
                random.randint(0, self.height-1)
            ]
            if self.food not in self.snake:
                break

    def step(self, action):
        self.steps += 1

        if (action == Direction.RIGHT.value and self.direction != Direction.LEFT) or \
            (action == Direction.LEFT.value and self.direction != Direction.RIGHT) or \
            (action == Direction.UP.value and self.direction != Direction.DOWN) or \
                (action == Direction.DOWN.value and self.direction != Direction.UP):
            self.direction = Direction(action)

        if self.direction == Direction.RIGHT:
            self.head[0] += 1
        elif self.direction == Direction.LEFT:
            self.head[0] -= 1
        elif self.direction == Direction.UP:
            self.head[1] -= 1
        elif self.direction == Direction.DOWN:
            self.head[1] += 1

        if self._is_collision():
            self.game_over = True
            return self._get_state(), REWARD_DEATH, True

        if self.steps > MAX_STEPS:
            self.game_over = True
            return self._get_state(), REWARD_DEATH, True

        self.snake.insert(0, self.head.copy())

        reward = REWARD_STEP
        if self.head == self.food:
            self.score += 1
            reward = REWARD_FOOD
            self._place_food()
        else:
            self.snake.pop()

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

    def _get_state(self, fps=10):
        """
        返回11维状态向量：
        [危险直前, 危险左方, 危险右方,
         方向右, 方向下, 方向左, 方向上,
         食物Δx, 食物Δy,
         食物左?, 食物上?]
        """
        head = self.head
        dir_idx = self.direction.value

        # 方向向量：右、下、左、上
        dir_vector = [(1, 0), (0, 1), (-1, 0), (0, -1)]
        # 左转向量
        left_vector = [(0, -1), (1, 0), (0, 1), (-1, 0)]
        # 右转向量
        right_vector = [(0, 1), (-1, 0), (0, -1), (1, 0)]

        straight = [head[0] + dir_vector[dir_idx]
                    [0], head[1] + dir_vector[dir_idx][1]]
        left = [head[0] + left_vector[dir_idx][0],
                head[1] + left_vector[dir_idx][1]]
        right = [head[0] + right_vector[dir_idx]
                 [0], head[1] + right_vector[dir_idx][1]]

        danger_straight = float(self._is_collision(straight))
        danger_left = float(self._is_collision(left))
        danger_right = float(self._is_collision(right))

        dir_onehot = [0.0, 0.0, 0.0, 0.0]
        dir_onehot[dir_idx] = 1.0

        # 食物相对位置（归一化到[-1, 1]）
        food_dx = (self.food[0] - head[0]) / self.width
        food_dy = (self.food[1] - head[1]) / self.height

        food_left = float(self.food[0] < head[0])
        food_up = float(self.food[1] < head[1])

        state = [
            danger_straight, danger_left, danger_right,
            *dir_onehot,
            food_dx, food_dy,
            food_left, food_up
        ]
        return np.array(state, dtype=np.float32)

    def render(self):
        if self.display is None:
            pygame.init()
            self.display = pygame.display.set_mode(
                (self.width*BLOCK_SIZE, self.height*BLOCK_SIZE)
            )
            pygame.display.set_cation("Snake DQN")
            self.clock = pygame.time.Clock()

        self.display.fill(BLACK)

        pygame.draw.rect(self.display, RED, pygame.Rect(
            self.food[0]*BLOCK_SIZE, self.food[1] *
            BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE
        ))

        for i, pt in enumerate(self.snake):
            color = BLUE if i == 0 else GREEN
            pygame.draw.rect(self.display, color, pygame.Rect(
                pt[0]*BLOCK_SIZE, pt[1]*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
            pygame.draw.rect(self.display, BLACK, pygame.Rect(
                pt[0]*BLOCK_SIZE+4, pt[1]*BLOCK_SIZE+4, BLOCK_SIZE-8, BLOCK_SIZE-8))

        font = pygame.font.Font(None, 36)
        text = font.render('Score: %d' % self.score, True, WHITE)
        self.display.blit(text, (10, 10))

        pygame.display.flip()
        self.clock.tick(fps)
