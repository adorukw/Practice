import random
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from collections import deque, namedtuple
import os

from model_dqn_cnn import DQN

Experience = namedtuple('Experience', field_names=[
                        'state', 'action', 'reward', 'next_state', 'done'])

class ReplayBuffer:
    """
    经验回放缓冲区。
    用于存储和随机抽样历史经验，以打破数据间的相关性，稳定训练。
    """

    def __init__(self, capacity):
        self.buffer = deque(maxlen=capacity)

    def __len__(self):
        return len(self.buffer)

    def push(self, state, action, reward, next_state, done):
        self.buffer.append(Experience(state, action, reward, next_state, done))

    def sample(self, batch_size):
        if batch_size > len(self.buffer):
            raise ValueError(
                f'Batch size {batch_size} 超出缓冲区大小 {len(self.buffer)}')

        experiences = random.sample(self.buffer, k=batch_size)
        # 注意：对于CNN，状态是 (C, H, W) 的numpy数组，需要增加一个批次维度
        states = torch.from_numpy(
            np.stack([e.state for e in experiences])).float()
        actions = torch.from_numpy(
            np.vstack([e.action for e in experiences])).long()
        rewards = torch.from_numpy(
            np.vstack([e.reward for e in experiences])).float()
        next_states = torch.from_numpy(
            np.stack([e.next_state for e in experiences])).float()
        dones = torch.from_numpy(
            np.vstack([e.done for e in experiences]).astype(np.uint8)).float()
        return states, actions, rewards, next_states, dones

class DQNAgent_CNN:
    """
    DQN智能体（CNN版本），使用卷积神经网络、经验回放和目标网络。
    """

    def __init__(self, state_shape, action_size, hidden_size=128, buffer_size=10000,
                 batch_size=64, gamma=0.99, lr=1e-3, epsilon=1.0, epsilon_min=0.1,
                 epsilon_decay=0.995, target_update_freq=10, device=None):
        """
        初始化DQN智能体（CNN版本）。
        参数:
            state_shape: 状态图像的形状 (元组: channels, height, width)。
            action_size: 动作维度 (3: 直行, 右转, 左转)。
            hidden_size: 保留参数，用于兼容性，但CNN模型内部有自己的全连接层维度。
            buffer_size: 经验回放缓冲区容量。
            batch_size: 训练批大小。
            gamma: 折扣因子。
            lr: 学习率。
            epsilon: 初始探索率 (epsilon-greedy)。
            epsilon_min: 最小探索率。
            epsilon_decay: 探索率衰减因子。
            target_update_freq: 更新目标网络的频率。
            device: 计算设备 ('cuda' 或 'cpu')，为None则自动选择。
        """
        self.state_shape = state_shape
        self.action_size = action_size
        self.batch_size = batch_size
        self.gamma = gamma
        self.epsilon = epsilon
        self.epsilon_min = epsilon_min
        self.epsilon_decay = epsilon_decay
        self.target_update_freq = target_update_freq
        self.train_step = 0

        if device is None:
            self.device = torch.device(
                "cuda" if torch.cuda.is_available() else "cpu")
        else:
            self.device = device

        # 初始化策略网络和目标网络 (使用CNN)
        self.policy_net = DQN(state_shape, action_size).to(self.device)
        self.target_net = DQN(state_shape, action_size).to(self.device)
        self.target_net.load_state_dict(self.policy_net.state_dict())
        self.target_net.eval()

        self.optimizer = optim.Adam(self.policy_net.parameters(), lr=lr)
        self.memory = ReplayBuffer(buffer_size)

        # 用于记录训练过程的列表
        self.episode_rewards = []
        self.episode_scores = []
        self.episode_lengths = []
        self.training_losses = []

    def get_action(self, state, training=True):
        if training and np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size)
        else:
            with torch.no_grad():
                # 状态已经是 (C, H, W) 格式，需要增加批次维度
                state_tensor = torch.FloatTensor(state).unsqueeze(0).to(self.device)
                q_values = self.policy_net(state_tensor)
                return q_values.argmax().item()

    def push_memory(self, state, action, reward, next_state, done):
        self.memory.push(state, action, reward, next_state, done)

    def learn(self):
        if len(self.memory) < self.batch_size:
            return None
        states, actions, rewards, next_states, dones = self.memory.sample(
            self.batch_size)
        states = states.to(self.device)
        actions = actions.to(self.device)
        rewards = rewards.to(self.device)
        next_states = next_states.to(self.device)
        dones = dones.to(self.device)

        q_current = self.policy_net(states).gather(1, actions)
        with torch.no_grad():
            q_next_max = self.target_net(next_states).max(1)[0].unsqueeze(1)
            q_target = rewards + (self.gamma * q_next_max * (1 - dones))

        loss_fn = nn.MSELoss()
        loss = loss_fn(q_current, q_target)
        self.optimizer.zero_grad()
        loss.backward()
        torch.nn.utils.clip_grad_value_(self.policy_net.parameters(), 1)
        self.optimizer.step()

        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

        self.train_step += 1
        if self.train_step % self.target_update_freq == 0:
            self.target_net.load_state_dict(self.policy_net.state_dict())

        loss_value = loss.item()
        self.training_losses.append(loss_value)
        return loss_value

    def record_episode(self, total_reward, score, episode_length):
        self.episode_rewards.append(total_reward)
        self.episode_scores.append(score)
        self.episode_lengths.append(episode_length)

    def get_stats(self, window=100):
        if len(self.episode_rewards) == 0:
            return {}
        n = min(window, len(self.episode_rewards))
        avg_reward = np.mean(self.episode_rewards[-n:]) if n > 0 else 0
        avg_score = np.mean(self.episode_scores[-n:]) if n > 0 else 0
        avg_length = np.mean(self.episode_lengths[-n:]) if n > 0 else 0
        avg_loss = np.mean(
            self.training_losses[-n*10:]) if len(self.training_losses) > 0 else 0
        return {
            'total_episodes': len(self.episode_rewards),
            'avg_reward': avg_reward,
            'avg_score': avg_score,
            'avg_length': avg_length,
            'avg_loss': avg_loss,
            'max_score': np.max(self.episode_scores) if self.episode_scores else 0,
            'current_epsilon': self.epsilon,
            'buffer_size': len(self.memory)
        }

    def save(self, filename='dqn_cnn_agent.pkl'):
        os.makedirs(os.path.dirname(filename) if os.path.dirname(
            filename) else '.', exist_ok=True)
        torch.save({
            'policy_net_state_dict': self.policy_net.state_dict(),
            'target_net_state_dict': self.target_net.state_dict(),
            'optimizer_state_dict': self.optimizer.state_dict(),
            'epsilon': self.epsilon,
            'episode_rewards': self.episode_rewards,
            'episode_scores': self.episode_scores,
            'episode_lengths': self.episode_lengths,
            'training_losses': self.training_losses,
            'train_step': self.train_step,
            'state_shape': self.state_shape,  # 保存状态形状
            'action_size': self.action_size,
        }, filename)
        print(f'CNN模型已经保存到：{filename}')

    def load(self, filename='dqn_cnn_agent.pkl'):
        if not os.path.exists(filename):
            print(f'CNN模型文件{filename}不存在！')
            return False
        checkpoint = torch.load(filename, map_location=self.device)
        self.policy_net.load_state_dict(checkpoint['policy_net_state_dict'])
        self.target_net.load_state_dict(checkpoint['policy_net_state_dict'])
        self.optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        self.epsilon = checkpoint.get('epsilon', self.epsilon_min)
        self.episode_rewards = checkpoint.get('episode_rewards', [])
        self.episode_scores = checkpoint.get('episode_scores', [])
        self.episode_lengths = checkpoint.get('episode_lengths', [])
        self.training_losses = checkpoint.get('training_losses', [])
        self.train_step = checkpoint.get('train_step', 0)
        print(f'CNN模型已从 {filename} 加载')
        return True

def create_cnn_agent_from_checkpoint(filename, device=None):
    """从检查点文件创建正确配置的CNN智能体"""
    if not os.path.exists(filename):
        print(f'文件 {filename} 不存在')
        return None
    checkpoint = torch.load(filename, map_location=device)
    state_shape = checkpoint.get('state_shape', (1, 6, 6))  # 默认形状
    action_size = checkpoint.get('action_size', 3)
    agent = DQNAgent_CNN(
        state_shape=state_shape,
        action_size=action_size,
        device=device
    )
    agent.policy_net.load_state_dict(checkpoint['policy_net_state_dict'])
    agent.target_net.load_state_dict(checkpoint['policy_net_state_dict'])
    agent.optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
    agent.epsilon = checkpoint.get('epsilon', agent.epsilon_min)
    print(f'从 {filename} 创建并加载CNN模型')
    return agent