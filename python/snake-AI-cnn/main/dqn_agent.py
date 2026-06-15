import torch
import torch.optim as optim
import torch.nn as nn
import numpy as np
import random
from collections import deque
from dqn_model import DQN

# 可修改参数
LR = 1e-4                   # 学习率
GAMMA = 0.99                # 折扣因子
BATCH_SIZE = 64             # 每次训练的批次大小
MEMORY_SIZE = 50000         # 经验回放池最大容量
EPSILON_START = 1.0         # 初始探索率
EPSILON_END = 0.05          # 最小探索率
EPSILON_DECAY = 0.995       # 探索率衰减系数
TARGET_UPDATE_FREQ = 100    # 目标网络更新频率(步数)


class DQNAgent:
    def __init__(self, grid_h, grid_w):
        self.device = torch.device(
            "cuda" if torch.cuda.is_available() else "cpu")
        print(f"Using device: {self.device}")

        # 策略网络和目标网络
        self.policy_net = DQN(grid_h, grid_w).to(self.device)
        self.target_net = DQN(grid_h, grid_w).to(self.device)
        self.target_net.load_state_dict(self.policy_net.state_dict())
        self.target_net.eval()

        self.optimizer = optim.Adam(self.policy_net.parameters(), lr=LR)
        self.criterion = nn.MSELoss()

        self.memory = deque(maxlen=MEMORY_SIZE)
        self.epsilon = EPSILON_START
        self.step_counter = 0

    def get_action(self, state, evaluate=False):
        # Evaluate 模式下总是选择最优动作
        if not evaluate and random.random() < self.epsilon:
            return random.randint(0, 3)  # 随机动作

        state_tensor = torch.FloatTensor(state).unsqueeze(
            0).to(self.device)  # 添加 Batch 维度
        with torch.no_grad():
            q_values = self.policy_net(state_tensor)
        return torch.argmax(q_values).item()

    def store_transition(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def train_step(self):
        if len(self.memory) < BATCH_SIZE:
            return

        # 采样数据
        batch = random.sample(self.memory, BATCH_SIZE)
        states, actions, rewards, next_states, dones = zip(*batch)

        # 转换为 Tensor
        states = torch.FloatTensor(np.array(states)).to(self.device)
        actions = torch.LongTensor(actions).unsqueeze(1).to(self.device)
        rewards = torch.FloatTensor(rewards).unsqueeze(1).to(self.device)
        next_states = torch.FloatTensor(np.array(next_states)).to(self.device)
        dones = torch.FloatTensor(dones).unsqueeze(1).to(self.device)

        # 计算当前 Q 值
        curr_q = self.policy_net(states).gather(1, actions)

        # 计算目标 Q 值
        with torch.no_grad():
            max_next_q = self.target_net(next_states).max(1)[0].unsqueeze(1)
            target_q = rewards + (1 - dones) * GAMMA * max_next_q

        # 更新策略网络
        loss = self.criterion(curr_q, target_q)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        # 更新探索率和目标网络
        self.step_counter += 1
        if self.step_counter % TARGET_UPDATE_FREQ == 0:
            self.target_net.load_state_dict(self.policy_net.state_dict())

    def update_epsilon(self):
        if self.epsilon > EPSILON_END:
            self.epsilon *= EPSILON_DECAY

    def save_model(self, path):
        torch.save(self.policy_net.state_dict(), path)

    def load_model(self, path):
        self.policy_net.load_state_dict(
            torch.load(path, map_location=self.device))
        self.target_net.load_state_dict(self.policy_net.state_dict())
