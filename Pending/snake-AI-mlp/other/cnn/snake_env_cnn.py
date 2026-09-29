import numpy as np
import pygame
import random
from enum import Enum
from collections import namedtuple

# 奖励参数
DEATH_PENALTY = -10        # 撞墙或自撞的惩罚
BASE_SCORE_REWARD = 10     # 吃到食物的基础奖励
LENGTH_BONUS = 0.5         # 根据长度增加的额外奖励
LIVE_REWARD = 0           # 每存活一步的奖励
CLOSE_FOOD = 0            # 靠近食物的奖励
LEAVE_FOOD = 0            # 远离食物的惩罚
MOVEMENT_LIMIT = 150       # 单局游戏最大步数限制（防止无限循环）


class Direction(Enum):
    """方向枚举"""
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


class SnakeGameCNN:
    """
    贪吃蛇游戏环境（CNN图像状态版本）。
    状态表示：一个 (1, height, width) 的二维网格图像。
    通道1的值定义：
        0.0: 空白
        0.3: 蛇身
        0.7: 蛇头
        1.0: 食物
    """

    def __init__(self, width=6, height=6, gui=False):
        self.width = width
        self.height = height
        self.gui = gui
        if gui:
            pygame.init()
            self.display = pygame.display.set_mode(
                (width*BLOCK_SIZE, height*BLOCK_SIZE))
            pygame.display.set_caption('贪吃蛇 (CNN)')
            self.clock = pygame.time.Clock()
            self.font = pygame.font.Font(None, 25)
        self.reset()

    def reset(self):
        """重置游戏状态，返回初始状态图像"""
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
        return self._get_image_state()

    def _place_food(self):
        """随机放置食物"""
        while True:
            self.food = Point(
                random.randint(0, self.width-1),
                random.randint(0, self.height-1)
            )
            if self.food not in self.snake:
                break

    def _get_image_state(self):
        """
        核心改动：返回图像状态 (CNN 输入)。
        形状: (1, height, width)，值在 [0.0, 1.0] 区间。
        """
        # 初始化全零网格
        state_grid = np.zeros((self.height, self.width), dtype=np.float32)

        # 标记蛇身 (值为0.3)
        for pt in self.snake[1:]:  # 从索引1开始，排除蛇头
            state_grid[pt.y, pt.x] = 0.3

        # 标记蛇头 (值为0.7)
        if self.snake:  # 确保蛇不为空
            head = self.snake[0]
            state_grid[head.y, head.x] = 0.7

        # 标记食物 (值为1.0)
        state_grid[self.food.y, self.food.x] = 1.0

        # 增加通道维度，适配CNN输入: (C, H, W) -> (1, H, W)
        return state_grid.reshape(1, self.height, self.width)

    def get_state(self):
        """获取当前状态图像，兼容旧接口"""
        return self._get_image_state()

    def get_shape(self):
        """
        返回图像状态表示的形状，用于CNN模型初始化。
        返回格式: (通道数, 高度, 宽度)
        """
        return (1, self.height, self.width)

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
        """判断指定点是否危险（撞墙或自撞）"""
        return (
            point.x < 0 or point.x >= self.width or
            point.y < 0 or point.y >= self.height or
            point in self.snake[1:]  # 排除蛇头自身
        )

    def _get_next_position(self, direction):
        """从当前蛇头向指定方向移动一步"""
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
        """向右转"""
        directions = [Direction.RIGHT, Direction.DOWN,
                      Direction.LEFT, Direction.UP]
        idx = directions.index(direction)
        return directions[(idx + 1) % 4]

    def _turn_left(self, direction):
        """向左转"""
        directions = [Direction.RIGHT, Direction.DOWN,
                      Direction.LEFT, Direction.UP]
        idx = directions.index(direction)
        return directions[(idx - 1) % 4]

    def step(self, action):
        """
        执行一个动作，返回新的状态、奖励、终止标志和信息。
        参数:
            action: 0=直行, 1=右转, 2=左转
        返回:
            state: 图像状态 (1, H, W)
            reward: 奖励值
            done: 是否结束
            info: 额外信息字典，包含'score'
        """
        self.frame_iteration += 1

        if self.gui:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    quit()

        # 根据动作改变方向
        if action == 1:
            self.direction = self._turn_right(self.direction)
        elif action == 2:
            self.direction = self._turn_left(self.direction)

        # 计算新位置
        new_head = self._get_next_position(self.direction)

        # 计算移动前与食物的距离
        old_distance = abs(self.head.x - self.food.x) + \
            abs(self.head.y - self.food.y)

        reward = 0
        done = False

        # 在移动之前检查是否危险
        if self._is_danger(new_head) or self.frame_iteration > MOVEMENT_LIMIT * len(self.snake):
            reward += DEATH_PENALTY
            done = True
            return self._get_image_state(), reward, done, {'score': self.score}

        # 安全移动蛇头
        self.snake.insert(0, new_head)
        self.head = new_head

        # 计算移动后与食物的距离
        new_distance = abs(self.head.x - self.food.x) + \
            abs(self.head.y - self.food.y)

        # 检查是否吃到食物
        if new_head == self.food:
            self.score += 1
            dynamic_reward = BASE_SCORE_REWARD + LENGTH_BONUS * len(self.snake)
            reward += dynamic_reward
            self._place_food()
        else:
            if old_distance < new_distance:
                reward += CLOSE_FOOD
            else:
                reward += LEAVE_FOOD
            self.snake.pop()  # 移动蛇尾
            reward += LIVE_REWARD

        if self.gui:
            self._update_ui()

        return self._get_image_state(), reward, done, {'score': self.score}

    def _update_ui(self):
        """更新游戏界面（仅在GUI模式有效）"""
        self.display.fill(BLACK)

        # 绘制蛇身
        for idx, pt in enumerate(self.snake):
            color = PINK if idx == 0 else BLUE
            pygame.draw.rect(
                self.display,
                color,
                pygame.Rect(pt.x*BLOCK_SIZE, pt.y*BLOCK_SIZE,
                            BLOCK_SIZE, BLOCK_SIZE)
            )
            pygame.draw.rect(
                self.display,
                WHITE,
                pygame.Rect(pt.x*BLOCK_SIZE+4, pt.y*BLOCK_SIZE+4, 12, 12)
            )

        # 绘制食物
        pygame.draw.rect(
            self.display,
            RED,
            pygame.Rect(self.food.x*BLOCK_SIZE, self.food.y *
                        BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE)
        )

        # 显示分数
        text = self.font.render(f'分数:{self.score}', True, WHITE)
        self.display.blit(text, [0, 0])

        pygame.display.flip()
        self.clock.tick(SPEED)

    def close(self):
        """关闭游戏界面"""
        if self.gui:
            pygame.quit()
