import random
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from collections import deque, namedtuple
import os

from model_dqn_mlp import DQN

Experience = namedtuple('Experience', field_names=[
                        'state', 'action', 'reward', 'next_state', 'done'])


class ReplayBuffer:
    """
    经验回放缓冲区。
    用于存储和随机抽样历史经验，以打破数据间的相关性，稳定训练。
    """

    def __init__(self, capacity):
        """
       参数:
           capacity: 缓冲区的最大容量。
       """
        self.buffer = deque(maxlen=capacity)

    def __len__(self):
        return len(self.buffer)

    def push(self, state, action, reward, next_state, done):
        self.buffer.append(Experience(state, action, reward, next_state, done))

    def sample(self, batch_size):
        """
        从缓冲区中随机抽取一批经验。
        返回:
            包含(state, action, reward, next_state, done)的元组，均为PyTorch张量。
        """
        if batch_size > len(self.buffer):
            raise ValueError(
                f'Batch size {batch_size} 超出缓冲区大小 {len(self.buffer)}')

        experiences = random.sample(self.buffer, k=batch_size)

        states = torch.from_numpy(
            np.vstack([e.state for e in experiences])).float()
        actions = torch.from_numpy(
            np.vstack([e.action for e in experiences])).long()
        rewards = torch.from_numpy(
            np.vstack([e.reward for e in experiences])).float()
        next_states = torch.from_numpy(
            np.vstack([e.next_state for e in experiences])).float()
        dones = torch.from_numpy(
            np.vstack([e.done for e in experiences]).astype(np.uint8)).float()
        return states, actions, rewards, next_states, dones


class DQNAgent:
    """
    DQN智能体，使用深度Q网络、经验回放和目标网络。
    """

    def __init__(self, state_size, action_size, hidden_size=64, buffer_size=10000,
                 batch_size=64, gamma=0.99, lr=1e-3, epsilon=1.0, epsilon_min=0.1,
                 epsilon_decay=0.995, target_update_freq=10, device=None):
        """
        初始化DQN智能体。
        参数:
            state_size: 状态维度 (在您的snake_env.py中是10)。
            action_size: 动作维度 (3: 直行, 右转, 左转)。
            hidden_size: 神经网络隐藏层大小。
            buffer_size: 经验回放缓冲区容量。
            batch_size: 训练批大小。
            gamma: 折扣因子。
            lr: 学习率。
            epsilon: 初始探索率 (epsilon-greedy)。
            epsilon_min: 最小探索率。
            epsilon_decay: 探索率衰减因子。
            target_update_freq: 更新目标网络的频率（每隔多少训练步更新一次）。
            device: 计算设备 ('cuda' 或 'cpu')，为None则自动选择。
        """
        self.state_size = state_size
        self.action_size = action_size
        self.batch_size = batch_size
        self.gamma = gamma
        self.epsilon = epsilon
        self.epsilon_min = epsilon_min
        self.epsilon_decay = epsilon_decay
        self.target_update_freq = target_update_freq
        self.train_step = 0  # 记录训练步数，用于控制目标网络更新

        if device is None:
            # 确保这里创建的是torch.device对象
            if torch.cuda.is_available():
                self.device = torch.device("cuda")
                print("使用CUDA (GPU)")
            else:
                self.device = torch.device("cpu")
                print("使用CPU")
        else:
            # 确保传入的device已经是torch.device对象
            self.device = device

        # 初始化策略网络和目标网络
        self.policy_net = DQN(state_size, hidden_size,
                              action_size).to(self.device)
        self.target_net = DQN(state_size, hidden_size,
                              action_size).to(self.device)
        # 初始时，目标网络与策略网络权重相同
        self.target_net.load_state_dict(self.policy_net.state_dict())
        self.target_net.eval()  # 目标网络设置为评估模式，不计算梯度

        # 初始化优化器 (Adam) 和经验回放缓冲区
        self.optimizer = optim.Adam(self.policy_net.parameters(), lr=lr)
        self.memory = ReplayBuffer(buffer_size)

        # 用于记录训练过程的列表
        self.episode_rewards = []
        self.episode_scores = []
        self.episode_lengths = []
        self.training_losses = []

    def get_action(self, state, training=True):
        """
        根据当前状态选择动作（epsilon-greedy策略）。
        参数:
            state: 当前状态 (numpy数组)。
            training: 是否为训练模式。训练时启用探索，评估时关闭。
        返回:
            action: 动作索引 (0, 1, 2)。
        """
        if training and np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size)
        else:
            with torch.no_grad():   # 不计算梯度，节省内存
                # 将状态转换为张量并添加到批次维度
                state_tensor = torch.FloatTensor(
                    state).unsqueeze(0).to(self.device)
                q_values = self.policy_net(state_tensor)
                return q_values.argmax().item()

    def push_memory(self, state, action, reward, next_state, done):
        self.memory.push(state, action, reward, next_state, done)

    def learn(self):
        """
        执行一次学习步骤。
        从经验回放缓冲区采样，计算损失，更新策略网络，并定期更新目标网络。
        返回:
            loss: 本次训练的损失值。如果经验不足未学习，则返回None。
        """
        if len(self.memory) < self.batch_size:
            return None
        # 1. 从缓冲区采样一批经验
        states, actions, rewards, next_states, dones = self.memory.sample(
            self.batch_size)
        # 将数据转移到设备上（GPU/CPU）
        states = states.to(self.device)
        actions = actions.to(self.device)
        rewards = rewards.to(self.device)
        next_states = next_states.to(self.device)
        dones = dones.to(self.device)

        # 2. 计算当前Q值 (Q_expected)
        # 使用策略网络计算当前状态下执行动作actions的Q值
        # gather(1, actions) 用于从每个样本的输出中选择对应动作的Q值
        q_current = self.policy_net(states).gather(1, actions)

        # 3. 计算目标Q值 (Q_target)
        with torch.no_grad():
            # 用目标网络计算下一状态的最大Q值
            q_next_max = self.target_net(next_states).max(1)[0].unsqueeze(1)
            # DQN目标公式: R + γ * max_{a'} Q_target(s', a') * (1 - done)
            # 如果游戏结束(done=1)，则下一状态的Q值为0
            q_target = rewards + (self.gamma * q_next_max * (1 - dones))

        # 4. 计算均方误差损失
        loss_fn = nn.MSELoss()
        loss = loss_fn(q_current, q_target)

        # 5. 反向传播和优化
        self.optimizer.zero_grad()  # 清空过往梯度
        loss.backward()  # 反向传播
        # 可选：梯度裁剪，防止梯度爆炸
        torch.nn.utils.clip_grad_value_(self.policy_net.parameters(), 1)
        self.optimizer.step()  # 更新参数

        # 6. 衰减探索率epsilon
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

        # 7. 定期更新目标网络（硬更新）
        self.train_step += 1
        if self.train_step % self.target_update_freq == 0:
            self.target_net.load_state_dict(self.policy_net.state_dict())

        # 记录损失
        loss_value = loss.item()
        self.training_losses.append(loss_value)
        return loss_value

    def record_episode(self, total_reward, score, episode_length):
        self.episode_rewards.append(total_reward)
        self.episode_scores.append(score)
        self.episode_lengths.append(episode_length)

    def decay_epsilon(self, decay_rate=0.9995, min_epsilon=0.1):
        self.epsilon = max(min_epsilon, self.epsilon * decay_rate)

    def get_stats(self, window=100):
        """
        获取最近的训练统计信息。
        参数:
            window: 计算平均值的窗口大小。
        返回:
            包含统计信息的字典。
        """
        if len(self.episode_rewards) == 0:
            return {}

        # 计算最近window轮的平均值，如果数据不足则计算全部
        n = min(window, len(self.episode_rewards))
        avg_reward = np.mean(self.episode_rewards[-n:]) if n > 0 else 0
        avg_score = np.mean(self.episode_scores[-n:]) if n > 0 else 0
        avg_length = np.mean(self.episode_lengths[-n:]) if n > 0 else 0
        avg_loss = np.mean(
            self.training_losses[-n*10:]) if len(self.training_losses) > 0 else 0  # 损失步数更多

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

    def save(self, filename='dqn_agent.pkl'):
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
            'state_size': self.state_size,  # 保存状态大小
            'action_size': self.action_size,  # 保存动作大小
            'hidden_size': self.policy_net.fc1.out_features,  # 关键：保存隐藏层大小
        }, filename)
        print(f'模型已经保存到：{filename}')

    def load(self, filename='dqn_agent.pkl'):
        if not os.path.exists(filename):
            print(f'模型文件{filename}不存在！')
            return

        checkpoint = torch.load(filename, map_location=self.device)
        self.policy_net.load_state_dict(checkpoint['policy_net_state_dict'])
        self.target_net.load_state_dict(
            checkpoint['policy_net_state_dict'])  # 加载到目标网络
        self.optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        self.epsilon = checkpoint.get('epsilon', self.epsilon_min)
        self.episode_rewards = checkpoint.get('episode_rewards', [])
        self.episode_scores = checkpoint.get('episode_scores', [])
        self.episode_lengths = checkpoint.get('episode_lengths', [])
        self.training_losses = checkpoint.get('training_losses', [])
        self.train_step = checkpoint.get('train_step', 0)

        print(f'模型已从 {filename} 加载')
        return True


def create_agent_from_checkpoint(filename, device=None):
    """从检查点文件创建正确配置的智能体"""
    if not os.path.exists(filename):
        print(f'文件 {filename} 不存在')
        return None

    checkpoint = torch.load(filename, map_location=device)

    # 从检查点获取配置参数
    state_size = checkpoint.get('state_size', 10)
    action_size = checkpoint.get('action_size', 3)
    hidden_size = checkpoint.get('hidden_size', 128)

    # 创建智能体
    agent = DQNAgent(
        state_size=state_size,
        action_size=action_size,
        hidden_size=hidden_size,
        device=device
    )

    # 加载权重
    agent.policy_net.load_state_dict(checkpoint['policy_net_state_dict'])
    agent.target_net.load_state_dict(checkpoint['policy_net_state_dict'])
    agent.optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
    agent.epsilon = checkpoint.get('epsilon', agent.epsilon_min)

    print(f'从 {filename} 创建并加载模型')
    return agent
