import numpy as np
import pygame
import random
from enum import Enum
from collections import namedtuple

DEATH_PENALTY = -10        # 撞墙或自撞的惩罚，从-10改为-5
BASE_SCORE_REWARD = 20          # 吃到食物的奖励，从10改为5
LENGTH_BONUS = 0.5          # 长蛇奖励，从10改为5
LIVE_REWARD = 0        # 每步存活奖励（保持负值，鼓励快速找到食物）
CLOSE_FOOD = 0          # 靠近食物的奖励
LEAVE_FOOD = 0         # 远离食物的惩罚
MOVEMENT_LIMIT = 150      # 最大步数限制（防止无限循环）
SEED = 42                  # 随机种子
USE_SEED = False           # 是否使用固定随机种子


class Direction(Enum):
    RIGHT = 0
    DOWN = 1
    LEFT = 2
    UP = 3


Point = namedtuple('Point', 'x,y')

# 颜色定义
WHITE = (255, 255, 255)
RED = (200, 0, 0)
PINK = (255, 0, 255)
BLUE = (0, 100, 255)
BLACK = (0, 0, 0)
GREEN = (0, 255, 0)

BLOCK_SIZE = 20
SPEED = 100


class SnakeGame:
    def __init__(self, width=10, height=10, gui=False):
        self.width = width
        self.height = height
        self.gui = gui
        if gui:
            pygame.init()
            self.display = pygame.display.set_mode(
                (width*BLOCK_SIZE, height*BLOCK_SIZE))
            pygame.display.set_caption('贪吃蛇')
            self.clock = pygame.time.Clock()
            self.font = pygame.font.Font(None, 25)
        self.reset()

    def reset(self):
        if USE_SEED:
            random.seed(SEED)
        self.direction = Direction.RIGHT
        self.head = Point(self.width//2, self.height//2)
        self.snake = [
            self.head,
            Point(self.head.x-1, self.head.y),
            Point(self.head.x-2, self.head.y)
        ]
        self.score = 0
        self._place_food()
        self.frame_iteration = 0
        return self.get_state()

    def _place_food(self):
        while True:
            self.food = Point(
                random.randint(0, self.width-1),
                random.randint(0, self.height-1)
            )
            if self.food not in self.snake:
                break

    def get_state(self):
        """返回优化后的状态表示（去除冗余后为9个特征）"""
        head = self.snake[0]

        # 1. 危险特征 (3个布尔值) - 保持不变
        danger_straight = self._is_danger(
            self._get_next_position(self.direction))
        danger_right = self._is_danger(
            self._get_next_position(self._turn_right(self.direction)))
        danger_left = self._is_danger(
            self._get_next_position(self._turn_left(self.direction)))

        # 2. 食物相对位置 (优化为2个整数，替换原来的4个布尔值)
        if self.food.x < head.x:
            food_dir_x = -1  # 食物在左
        elif self.food.x > head.x:
            food_dir_x = 1   # 食物在右
        else:
            food_dir_x = 0   # 食物在同一列

        if self.food.y < head.y:
            food_dir_y = -1  # 食物在上
        elif self.food.y > head.y:
            food_dir_y = 1   # 食物在下
        else:
            food_dir_y = 0   # 食物在同一行

        # 3. 当前方向 (优化为1个整数，替换原来的4个布尔值)
        if self.direction == Direction.RIGHT:
            dir_encoded = 0
        elif self.direction == Direction.DOWN:
            dir_encoded = 1
        elif self.direction == Direction.LEFT:
            dir_encoded = 2
        else:  # Direction.UP
            dir_encoded = 3

        # 4. 您新增的“三步视野”特征 (2个布尔值) - 保持不变
        forward_one = self._get_next_position_at(head, self.direction)
        forward_two = self._get_next_position_at(forward_one, self.direction)
        danger_2steps_ahead = self._is_danger(forward_two)

        forward_three = self._get_next_position_at(forward_two, self.direction)
        danger_3steps_ahead = self._is_danger(forward_three)

        # --- 新增：离散化的连续特征 ---
        # 特征1：蛇身长度（离散化为3档）
        snake_length = len(self.snake)
        if snake_length < 5:
            length_bin = 0  # 短
        elif snake_length < 10:
            length_bin = 1  # 中
        else:
            length_bin = 2  # 长

        # 特征2：与食物的曼哈顿距离（离散化为4档）
        manhattan_distance = abs(head.x - self.food.x) + \
            abs(head.y - self.food.y)
        max_possible_distance = self.width + self.height - 2
        # 按距离比例分档
        if manhattan_distance < max_possible_distance * 0.25:
            distance_bin = 0  # 非常近
        elif manhattan_distance < max_possible_distance * 0.5:
            distance_bin = 1  # 比较近
        elif manhattan_distance < max_possible_distance * 0.75:
            distance_bin = 2  # 比较远
        else:
            distance_bin = 3  # 非常远

        # 返回状态数组：注意现在是混合类型（布尔值和整数）
        return np.array([
            # 3个原始危险特征
            danger_straight, danger_right, danger_left,
            # 2个优化后的食物方向特征 (整数)
            food_dir_x, food_dir_y,
            # 1个优化后的方向特征 (整数)
            dir_encoded,
            # 2个新增视野特征
            danger_2steps_ahead, danger_3steps_ahead,
            length_bin, distance_bin
        ], dtype=int)  # 统一为float类型

    def get_shape(self):
        """
        返回图像状态表示的形状，用于CNN模型。
        返回格式: (通道数, 高度, 宽度)
        """
        return (1, self.height, self.width)  # 单通道灰度图像

    def _get_next_position_at(self, start_point, direction):
        """从指定点向指定方向移动一步"""
        if direction == Direction.RIGHT:
            return Point(start_point.x + 1, start_point.y)
        elif direction == Direction.LEFT:
            return Point(start_point.x - 1, start_point.y)
        elif direction == Direction.UP:
            return Point(start_point.x, start_point.y - 1)
        elif direction == Direction.DOWN:
            return Point(start_point.x, start_point.y + 1)
        return start_point

    def _is_danger(self, point):
        return (
            point.x < 0 or point.x >= self.width or
            point.y < 0 or point.y >= self.height or
            point in self.snake[1:]
        )

    def _get_next_position(self, direction):
        head = self.snake[0]
        if direction == Direction.RIGHT:
            return Point(head.x + 1, head.y)
        elif direction == Direction.LEFT:
            return Point(head.x - 1, head.y)
        elif direction == Direction.UP:
            return Point(head.x, head.y - 1)
        elif direction == Direction.DOWN:
            return Point(head.x, head.y + 1)

    def _turn_right(self, direction):
        directions = [Direction.RIGHT, Direction.DOWN,
                      Direction.LEFT, Direction.UP]
        idx = directions.index(direction)
        return directions[(idx + 1) % 4]

    def _turn_left(self, direction):
        directions = [Direction.RIGHT, Direction.DOWN,
                      Direction.LEFT, Direction.UP]
        idx = directions.index(direction)
        return directions[(idx - 1) % 4]

    def step(self, action):
        self.frame_iteration += 1

        if self.gui:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    quit()

        if action == 1:
            self.direction = self._turn_right(self.direction)
        elif action == 2:
            self.direction = self._turn_left(self.direction)

        old_distance = abs(self.head.x - self.food.x) + \
            abs(self.head.y - self.food.y)
        new_head = self._get_next_position(self.direction)
        self.snake.insert(0, new_head)
        self.head = new_head
        new_distance = abs(self.head.x - self.food.x) + \
            abs(self.head.y - self.food.y)

        reward = 0
        done = False

        if self._is_danger(new_head) or self.frame_iteration > MOVEMENT_LIMIT * len(self.snake):
            reward += DEATH_PENALTY
            done = True
            return self.get_state(), reward, done, {'score': self.score}
        elif new_head == self.food:
            self.score += 1
            dynamic_reward = BASE_SCORE_REWARD+LENGTH_BONUS*len(self.snake)
            reward += dynamic_reward
            self._place_food()
        else:
            if old_distance < new_distance:
                reward += CLOSE_FOOD
            else:
                reward += LEAVE_FOOD
            self.snake.pop()
            reward += LIVE_REWARD

        if self.gui:
            self._update_ui()

        return self.get_state(), reward, done, {'score': self.score}

    def _update_ui(self):
        self.display.fill(BLACK)  # 改为黑色背景，白色文字可见

        for idx, pt in enumerate(self.snake):
            color = BLUE if idx == 0 else GREEN
            pygame.draw.rect(
                self.display,
                color,
                pygame.Rect(pt.x*BLOCK_SIZE, pt.y*BLOCK_SIZE,
                            BLOCK_SIZE, BLOCK_SIZE)
            )
            pygame.draw.rect(
                self.display,
                WHITE,
                pygame.Rect(pt.x*BLOCK_SIZE+4, pt.y*BLOCK_SIZE +
                            4, BLOCK_SIZE-8, BLOCK_SIZE-8)
            )

        pygame.draw.rect(
            self.display,
            RED,
            pygame.Rect(self.food.x*BLOCK_SIZE, self.food.y *
                        BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE)
        )

        font = pygame.font.Font(None, 36)
        text = font.render('Score: %d' % self.score, True, WHITE)
        self.display.blit(text, (10, 10))

        pygame.display.flip()
        self.clock.tick(SPEED)

    def close(self):
        if self.gui:
            pygame.quit()
