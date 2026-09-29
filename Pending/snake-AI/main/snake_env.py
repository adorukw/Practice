import pygame
import random
import numpy as np


class SnakeEnv:
    def __init__(self, width=10, height=10, state_mode='cnn',
                 reward_food=10.0, reward_death=-10.0, reward_step=-0.1, max_steps=500):
        self.width = width
        self.height = height
        self.state_mode = state_mode  # 'cnn' or 'mlp'
        self.reward_food = reward_food
        self.reward_death = reward_death
        self.reward_step = reward_step
        self.max_steps = max_steps

        self.block_size = 40
        self.display = None
        self.clock = None
        self.reset()

    def reset(self):
        self.head = [self.width // 2, self.height // 2]
        self.snake = [self.head.copy()]
        self.direction = 1  # 0:Up, 1:Right, 2:Down, 3:Left
        self.score = 0
        self.steps = 0
        self._place_food()
        self.game_over = False
        return self._get_state()

    def _place_food(self):
        while True:
            self.food = [random.randint(
                0, self.width - 1), random.randint(0, self.height - 1)]
            if self.food not in self.snake:
                break

    def step(self, action):
        self.steps += 1
        # 防止直接掉头
        if abs(action - self.direction) != 2:
            self.direction = action

        # 移动
        if self.direction == 0:
            self.head[1] -= 1
        elif self.direction == 1:
            self.head[0] += 1
        elif self.direction == 2:
            self.head[1] += 1
        elif self.direction == 3:
            self.head[0] -= 1

        reward = self.reward_step
        if self._is_collision() or self.steps >= self.max_steps:
            self.game_over = True
            return self._get_state(), self.reward_death, True

        if self.head == self.food:
            self.score += 1
            reward = self.reward_food
            self._place_food()
        else:
            self.snake.pop()

        self.snake.insert(0, self.head.copy())
        return self._get_state(), reward, False

    def _is_collision(self):
        if self.head[0] < 0 or self.head[0] >= self.width or \
           self.head[1] < 0 or self.head[1] >= self.height:
            return True
        if self.head in self.snake[1:]:
            return True
        return False

    def _get_state(self):
        if self.state_mode == 'cnn':
            # 3通道：食物，头，身体
            state = np.zeros((3, self.height, self.width), dtype=np.float32)
            state[0, self.food[1], self.food[0]] = 1.0
            state[1, self.head[1], self.head[0]] = 1.0
            for pt in self.snake[1:]:
                state[2, pt[1], pt[0]] = 1.0
            return state
        else:
            # MLP特征向量：头位置(2), 食物位置(2), 身体相对于头的方向(4), 距离墙距离(4)等
            # 这里简化为10维向量
            head_x, head_y = self.head[0]/self.width, self.head[1]/self.height
            food_x, food_y = self.food[0]/self.width, self.food[1]/self.height
            state = [head_x, head_y, food_x, food_y,
                     float(self.direction == 0), float(self.direction == 1),
                     float(self.direction == 2), float(self.direction == 3),
                     float(self.head[0]), float(self.head[1])]
            return np.array(state, dtype=np.float32)

    def render(self, fps=10):
        if not self.display:
            pygame.init()
            self.display = pygame.display.set_mode(
                (self.width*self.block_size, self.height*self.block_size))
            self.clock = pygame.time.Clock()
        self.display.fill((0, 0, 0))
        pygame.draw.rect(self.display, (200, 0, 0), (
            self.food[0]*self.block_size, self.food[1]*self.block_size, self.block_size, self.block_size))
        for i, pt in enumerate(self.snake):
            color = (0, 0, 255) if i == 0 else (0, 255, 0)
            pygame.draw.rect(self.display, color, (
                pt[0]*self.block_size, pt[1]*self.block_size, self.block_size, self.block_size))
        pygame.display.flip()
        self.clock.tick(fps)
