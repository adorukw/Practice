import numpy as np
import time
from snake_env import SnakeGame
from agent_qlearning import QLearningAgent
import matplotlib.pyplot as plt

# 游戏环境参数
WIDTH = 20                    # 游戏网格的宽度（单位：格子数）
HEIGHT = 20                   # 游戏网格的高度（单位：格子数）

# Q-learning算法核心参数
# 学习率 (Learning Rate)。控制新旧Q值的更新权重，范围通常为(0, 1]。值越大，对新样本的学习越快，但可能不稳定。
ALPHA = 0.0001
# 折扣因子 (Discount Factor)。衡量未来奖励的重要性，范围[0, 1]。越接近1，智能体越考虑长期回报。
GAMMA = 0.95
# 初始探索率 (ε)。在ε-greedy策略中，以EPSILON的概率随机选择动作（探索），以(1-EPSILON)的概率选择当前最优动作（利用）。
EPSILON = 1
DECAY_RATE = 0.9995           # 探索率衰减系数。每轮训练后，EPSILON会乘以该系数，使其逐渐减小，让智能体从探索为主慢慢过渡到利用为主。
MIN_EPSILON = 0.05            # 最小探索率。确保训练后期仍保持一个最低限度的随机探索，避免策略过早陷入局部最优。

# 训练流程参数
EPISODES = 100000             # 计划训练的总轮数（即游戏对局数）。
STATE_SIZE = 10               # 状态向量的维度。这必须与`snake_env.py`中`get_state()`方法返回的数组长度（10）严格一致。
SAVE_EVERY = 2000             # 模型保存频率。每训练SAVE_EVERY轮，就将当前的Q表（智能体）保存到文件一次，用于中断后恢复或评估中间结果。


def train(episodes=1000, show_progress_every=100, save_every=500):
    """
    训练Q-learning智能体

    参数:
        episodes: 训练轮数
        show_progress_every: 每多少轮显示进度
        save_every: 每多少轮保存模型
    """
    env = SnakeGame(width=WIDTH, height=HEIGHT, gui=False)

    state_size = STATE_SIZE
    action_size = 3

    agent = QLearningAgent(
        state_size=state_size,
        action_size=action_size,
        alpha=ALPHA,
        gamma=GAMMA,
        epsilon=EPSILON
    )

    print('开始训练贪吃蛇智能体...')
    print(f'训练轮数：{episodes}')
    print(f'状态空间：{state_size}')
    print(f'动作空间：{action_size}')
    print('-'*50)

    agent.load('./models/qlearning_agent.pkl')

    start_time = time.time()

    for episode in range(episodes):
        state = env.reset()
        total_reward = 0
        steps = 0

        done = False

        while not done:
            action = agent.get_action(state)

            next_state, reward, done, info = env.step(action)

            agent.update(state, action, reward, next_state, done)

            state = next_state
            total_reward += reward
            steps += 1

        score = info['score']
        agent.record_episode(total_reward, score, steps)

        agent.decay_epsilon(decay_rate=DECAY_RATE, min_epsilon=MIN_EPSILON)

        if (episode+1) % show_progress_every == 0:
            stats = agent.get_stats()
            elapsed_time = time.time()-start_time
            time_per_episode = elapsed_time/(episode+1)

            print(f"轮数: {episode + 1}/{episodes}")
            print(f"  平均奖励: {stats['avg_reward']:.2f}")
            print(f"  平均分数: {stats['avg_score']:.2f}")
            print(f"  最高分数: {stats['max_score']}")
            print(f"  探索率: {agent.epsilon:.3f}")
            print(f"  用时: {elapsed_time:.1f}s, 每轮: {time_per_episode:.2f}s")
            print("-" * 30)

        if (episode+1) % save_every == 0:
            agent.save('../models/qlearning_agent.pkl')

    total_time = time.time()-start_time
    print(f'训练完成！')
    print(f'总用时：{total_time:.1f}s')
    print(f'平均每轮：{total_time/episodes:.2f}s')

    agent.save('../models/qlearning_agent_final.pkl')

    agent.plot_training_progress(window_size=50)

    return agent


if __name__ == '__main__':
    agent = train(episodes=EPISODES, show_progress_every=100,
                  save_every=SAVE_EVERY)
