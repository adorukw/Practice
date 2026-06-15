import numpy as np
import time
import os
import matplotlib.pyplot as plt
from snake_env import SnakeGame
from agent_dqn_mlp import DQNAgent

WIDTH = 20
HEIGHT = 20
STATE_SIZE = 10           # 对应 snake_env.py 中 get_state() 返回的数组长度
ACTION_SIZE = 3           # 动作数量: 0=直行, 1=右转, 2=左转
HIDDEN_SIZE = 128         # DQN网络隐藏层大小
BUFFER_SIZE = 10000       # 经验回放缓冲区大小
BATCH_SIZE = 128           # 训练批大小
GAMMA = 0.9              # 折扣因子
LR = 5e-5                 # 学习率
EPS_START = 1.0           # 初始探索率
EPS_END = 0.05            # 最小探索率
EPS_DECAY = 0.9995         # 探索率衰减因子
TARGET_UPDATE_FREQ = 100   # 目标网络更新频率 (每多少训练步更新一次)
EPISODES = 10000          # 总训练轮数
SHOW_EVERY = 50          # 每多少轮显示一次进度
SAVE_EVERY = 500          # 每多少轮保存一次模型
MODEL_DIR = '../models/'    # 模型保存目录
MODEL_NAME = 'dqn_mlp_agent'  # 模型名称前缀


def train(episodes=EPISODES, show_progress_every=SHOW_EVERY, save_every=SAVE_EVERY):
    """
    训练DQN智能体。
    参数:
        episodes: 训练轮数
        show_progress_every: 每多少轮显示一次训练进度
        save_every: 每多少轮保存一次模型
    """
    os.makedirs(MODEL_DIR, exist_ok=True)

    env = SnakeGame(width=WIDTH, height=HEIGHT, gui=False)

    # 初始化DQN智能体
    agent = DQNAgent(
        state_size=STATE_SIZE,
        action_size=ACTION_SIZE,
        hidden_size=HIDDEN_SIZE,
        buffer_size=BUFFER_SIZE,
        batch_size=BATCH_SIZE,
        gamma=GAMMA,
        lr=LR,
        epsilon=EPS_START,
        epsilon_min=EPS_END,
        epsilon_decay=EPS_DECAY,
        target_update_freq=TARGET_UPDATE_FREQ,
        # device=torch.device("cpu")
    )

    # 可选：加载已有模型继续训练
    # model_path = os.path.join(MODEL_DIR, f'{MODEL_NAME}_final.pkl')
    # if os.path.exists(model_path):
    #     agent.load(model_path)

    print('开始训练DQN贪吃蛇智能体...')
    print(f'训练轮数: {episodes}')
    print(f'状态空间: {STATE_SIZE}')
    print(f'动作空间: {ACTION_SIZE}')
    print(f'网络结构: {STATE_SIZE}->{HIDDEN_SIZE}->{HIDDEN_SIZE}->{ACTION_SIZE}')
    print(f'经验回放缓冲区: {BUFFER_SIZE}')
    print(f'批大小: {BATCH_SIZE}')
    print(f'学习率: {LR}, 折扣因子: {GAMMA}')
    print(f'探索率: {EPS_START} -> {EPS_END} (衰减: {EPS_DECAY})')
    print('-' * 60)

    start_time = time.time()

    for episode in range(episodes):
        state = env.reset()
        total_reward = 0
        steps = 0
        done = False

        while not done:
            # 1. 选择动作
            action = agent.get_action(state, training=True)

            # 2. 执行动作，与环境交互
            next_state, reward, done, info = env.step(action)

            # 3. 存储经验
            agent.push_memory(state, action, reward, next_state, done)

            # 4. 学习
            loss = agent.learn()  # 内部会检查经验是否足够

            # 更新状态
            state = next_state
            total_reward += reward
            steps += 1

        score = info['score']
        agent.record_episode(total_reward, score, steps)

        # 定期显示训练进度
        if (episode + 1) % show_progress_every == 0:
            stats = agent.get_stats(window=show_progress_every)
            elapsed_time = time.time() - start_time
            time_per_episode = elapsed_time / (episode + 1)

            print(f"轮数: {episode + 1:5d}/{episodes} | ", end='')
            print(f"平均分数: {stats['avg_score']:6.2f} | ", end='')
            print(f"最高分数: {stats['max_score']:3d} | ", end='')
            print(f"平均奖励: {stats['avg_reward']:7.2f} | ", end='')
            print(f"平均步数: {stats['avg_length']:5.1f} | ", end='')
            print(f"平均损失: {stats['avg_loss']:7.4f} | ", end='')
            print(f"探索率: {agent.epsilon:5.3f} | ", end='')
            print(f"经验池: {len(agent.memory):5d} | ", end='')
            print(f"用时: {elapsed_time:6.1f}s")

        # 定期保存模型
        if (episode + 1) % save_every == 0:
            model_path = os.path.join(
                MODEL_DIR, f'{MODEL_NAME}.pkl')
            agent.save(model_path)

    # 训练结束，保存最终模型
    total_time = time.time() - start_time
    final_model_path = os.path.join(MODEL_DIR, f'{MODEL_NAME}_final.pkl')
    agent.save(final_model_path)

    print('\n' + '=' * 60)
    print('训练完成！')
    print(f'总训练轮数: {episodes}')
    print(f'总用时: {total_time:.1f} 秒')
    print(f'平均每轮: {total_time/episodes:.2f} 秒')
    print(f'最终模型已保存至: {final_model_path}')
    print('=' * 60)

    # 绘制训练曲线
    plot_training_progress(agent, window_size=100)

    return agent


def plot_training_progress(agent, window_size=100):
    """
    绘制训练进度曲线。
    参数:
        agent: 训练好的DQNAgent实例
        window_size: 移动平均窗口大小
    """
    if len(agent.episode_scores) == 0:
        print("没有训练数据可供绘图。")
        return

    plt.rcParams['font.sans-serif'] = ['WenQuanYi Zen Hei']
    plt.rcParams['axes.unicode_minus'] = False

    fig, axes = plt.subplots(2, 2, figsize=(14, 10))

    episodes = list(range(1, len(agent.episode_scores) + 1))

    # 计算移动平均
    def moving_average(data, window):
        return np.convolve(data, np.ones(window)/window, mode='valid')

    # 1. 分数曲线
    axes[0, 0].plot(episodes, agent.episode_scores,
                    alpha=0.3, label='单轮分数', color='blue')
    if len(agent.episode_scores) >= window_size:
        ma_scores = moving_average(agent.episode_scores, window_size)
        axes[0, 0].plot(range(window_size, len(agent.episode_scores)+1),
                        ma_scores, 'r-', linewidth=2,
                        label=f'{window_size}轮移动平均')
    axes[0, 0].set_xlabel('训练轮数')
    axes[0, 0].set_ylabel('分数')
    axes[0, 0].set_title('训练分数曲线')
    axes[0, 0].legend()
    axes[0, 0].grid(True, alpha=0.3)

    # 2. 奖励曲线
    axes[0, 1].plot(episodes, agent.episode_rewards,
                    alpha=0.3, label='单轮奖励', color='green')
    if len(agent.episode_rewards) >= window_size:
        ma_rewards = moving_average(agent.episode_rewards, window_size)
        axes[0, 1].plot(range(window_size, len(agent.episode_rewards)+1),
                        ma_rewards, 'r-', linewidth=2,
                        label=f'{window_size}轮移动平均')
    axes[0, 1].set_xlabel('训练轮数')
    axes[0, 1].set_ylabel('奖励')
    axes[0, 1].set_title('训练奖励曲线')
    axes[0, 1].legend()
    axes[0, 1].grid(True, alpha=0.3)

    # 3. 步数曲线
    axes[1, 0].plot(episodes, agent.episode_lengths,
                    alpha=0.3, label='单轮步数', color='orange')
    if len(agent.episode_lengths) >= window_size:
        ma_lengths = moving_average(agent.episode_lengths, window_size)
        axes[1, 0].plot(range(window_size, len(agent.episode_lengths)+1),
                        ma_lengths, 'r-', linewidth=2,
                        label=f'{window_size}轮移动平均')
    axes[1, 0].set_xlabel('训练轮数')
    axes[1, 0].set_ylabel('步数')
    axes[1, 0].set_title('训练步数曲线')
    axes[1, 0].legend()
    axes[1, 0].grid(True, alpha=0.3)

    # 4. 探索率和损失曲线
    ax4 = axes[1, 1]
    # 探索率（左轴）
    color = 'tab:blue'
    ax4.set_xlabel('训练轮数')
    ax4.set_ylabel('探索率 (ε)', color=color)
    eps_x = list(range(1, len(agent.episode_scores)+1,
                 max(1, len(agent.episode_scores)//100)))
    eps_y = [agent.epsilon * (agent.epsilon_decay ** i)
             for i in range(len(agent.episode_scores))]
    ax4.plot(episodes, eps_y[:len(episodes)],
             color=color, linewidth=2, label='探索率')
    ax4.tick_params(axis='y', labelcolor=color)

    # 损失（右轴）
    if agent.training_losses:
        ax5 = ax4.twinx()
        color = 'tab:red'
        ax5.set_ylabel('训练损失', color=color)
        # 对损失进行下采样以便显示
        loss_every = max(1, len(agent.training_losses) // 500)
        loss_x = list(range(0, len(agent.training_losses), loss_every))
        loss_y = [agent.training_losses[i] for i in loss_x]
        ax5.plot(loss_x, loss_y, alpha=0.5, color=color, label='训练损失')
        ax5.tick_params(axis='y', labelcolor=color)

    axes[1, 1].set_title('探索率和训练损失')
    fig.tight_layout()

    # 保存图像
    # plt.savefig('dqn_training_progress.png', dpi=100, bbox_inches='tight')
    plt.show()


if __name__ == '__main__':
    # 开始训练
    agent = train(episodes=EPISODES,
                  show_progress_every=SHOW_EVERY,
                  save_every=SAVE_EVERY)
