import numpy as np
import pickle
import os
from collections import defaultdict
import matplotlib.pyplot as plt

class QLearningAgent:
    def __init__(self, state_size, action_size, alpha=0.1, gamma=0.9, epsilon=0.1):
        """
        Q-learning智能体

        参数:
            state_size: 状态空间大小
            action_size: 动作空间大小
            alpha: 学习率 (0-1)
            gamma: 折扣因子 (0-1)
            epsilon: 探索率 (0-1)
        """
        self.state_size = state_size
        self.action_size = action_size
        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon

        self.q_table = defaultdict(lambda: np.zeros(action_size))

        self.episode_rewards = []
        self.episode_scores = []
        self.episode_lengths = []

    def get_action(self, state):
        """
        根据ε-greedy策略选择动作

        参数:
            state: 当前状态

        返回:
            action: 动作索引
        """
        if np.random.rand() < self.epsilon:
            return np.random.randint(self.action_size)

        state_key = tuple(state)
        q_values = self.q_table[state_key]

        max_q = np.max(q_values)
        max_indices = np.where(q_values == max_q)[0]
        return np.random.choice(max_indices)

    def update(self, state, action, reward, next_state, done):
        """
        更新Q表

        参数:
            state: 当前状态
            action: 执行的动作
            reward: 获得的奖励
            next_state: 下一个状态
            done: 是否结束
        """
        state_key = tuple(state)
        next_state_key = tuple(next_state)

        current_q = self.q_table[state_key][action]

        if done:
            next_max_q = 0
        else:
            next_max_q = np.max(self.q_table[next_state_key])

        new_q = current_q+self.alpha*(reward+self.gamma*next_max_q-current_q)
        self.q_table[state_key][action] = new_q

        return new_q

    def decay_epsilon(self, decay_rate=0.995, min_epsilon=0.01):
        self.epsilon = max(min_epsilon, self.epsilon*decay_rate)

    def record_episode(self, total_reward, score, episode_length):
        self.episode_rewards.append(total_reward)
        self.episode_scores.append(score)
        self.episode_lengths.append(episode_length)

    def plot_training_progress(self, window_size=100):
        plt.rcParams['font.sans-serif'] = ['WenQuanYi Zen Hei']
        plt.rcParams['axes.unicode_minus'] = False
        fig, axes = plt.subplots(3, 1, figsize=(10, 12))

        def moving_average(data, window_size):
            return np.convolve(data, np.ones(window_size)/window_size, mode='valid')

        axes[0].plot(self.episode_rewards, alpha=0.3, label='单轮奖励')
        if len(self.episode_rewards) >= window_size:
            ma_rewards = moving_average(self.episode_rewards, window_size)
            axes[0].plot(range(window_size-1, len(self.episode_rewards)),
                         ma_rewards, 'r-', linewidth=2, label=f'{window_size}轮移动平均')
        axes[0].set_xlabel('轮数')
        axes[0].set_ylabel('奖励')
        axes[0].set_title('训练奖励曲线')
        axes[0].legend()
        axes[0].grid(True, alpha=0.3)

        axes[1].plot(self.episode_scores, alpha=0.3, label='单轮分数')
        if len(self.episode_scores) >= window_size:
            ma_scores = moving_average(self.episode_scores, window_size)
            axes[1].plot(range(window_size-1, len(self.episode_scores)),
                         ma_scores, 'r-', linewidth=2, label=f'{window_size}轮移动平均')
        axes[1].set_xlabel('轮数')
        axes[1].set_ylabel('分数')
        axes[1].set_title('训练分数曲线')
        axes[1].legend()
        axes[1].grid(True, alpha=0.3)

        axes[2].plot(self.episode_lengths, alpha=0.3, label='单轮长度')
        if len(self.episode_lengths) >= window_size:
            ma_lengths = moving_average(self.episode_lengths, window_size)
            axes[2].plot(range(window_size-1, len(self.episode_lengths)),
                         ma_lengths, 'r-', linewidth=2, label=f'{window_size}轮移动平均')
        axes[2].set_xlabel('轮数')
        axes[2].set_ylabel('步数')
        axes[2].set_title('训练步数曲线')
        axes[2].legend()
        axes[2].grid(True, alpha=0.3)

        plt.tight_layout()
        # plt.savefig('training_progress.png', dpi=100, bbox_inches='tight')
        plt.show()

    def save(self, filename='qlearning_agent.pkl'):
        with open(filename, 'wb') as f:
            pickle.dump({
                'q_table': dict(self.q_table),
                'alpha': self.alpha,
                'gamma': self.gamma,
                'epsilon': self.epsilon,
                'state_size': self.state_size,
                'action_size': self.action_size,
                'episode_rewards': self.episode_rewards,
                'episode_scores': self.episode_scores,
                'episode_lengths': self.episode_lengths
            }, f)
        print(f'模型已保存到{filename}')

    def load(self, filename='qlearning_agent.pkl'):
        if not os.path.exists(filename):
            print(f'文件{filename}不存在')
            return False

        with open(filename, 'rb') as f:
            data = pickle.load(f)

        self.q_table = defaultdict(lambda: np.zeros(self.action_size))
        self.q_table.update(data['q_table'])
        self.alpha = data['alpha']
        self.gamma = data['gamma']
        self.epsilon = data['epsilon']
        self.state_size = data['state_size']
        self.action_size = data['action_size']
        self.episode_rewards = data.get('episode_rewards', [])
        self.episode_scores = data.get('episode_scores', [])
        self.episode_lengths = data.get('episode_lengths', [])

        print(f'模型已从{filename}加载')
        return True

    def get_stats(self):
        if len(self.episode_rewards) == 0:
            return {}

        return {
            'total_episodes': len(self.episode_rewards),
            'avg_reward': np.mean(self.episode_rewards[-100:]),
            'avg_score': np.mean(self.episode_scores[-100:]),
            'avg_length': np.mean(self.episode_lengths[-100:]),
            'max_score': np.max(self.episode_scores),
            'min_score': np.min(self.episode_scores),
            'current_epsilon': self.epsilon
        }
