import matplotlib.pyplot as plt
import numpy as np
import time
from snake_env import SnakeEnv, GRID_HEIGHT, GRID_WIDTH
from dqn_agent import DQNAgent

# ================= 可修改参数 =================
NUM_EPISODES = 500000          # 训练总回合数
MODEL_SAVE_PATH = "../models/snake_cnn_model.pth"  # 模型保存路径
PLOT_SAVE_PATH = "training_curve.png"   # 曲线图保存路径
PRINT_EVERY = 10               # 每多少回合打印一次日志
SAVE_EVERY = 1000              # 每多少回合保存一次模型（新增）
# ==============================================


def train():
    env = SnakeEnv()
    agent = DQNAgent(GRID_HEIGHT, GRID_WIDTH)

    scores = []
    mean_scores = []
    total_score = 0

    # 添加计时变量
    start_time = time.time()
    episode_times = []

    print("Starting Training...")
    print(f"Target: {NUM_EPISODES} episodes")
    print("-" * 50)

    for episode in range(1, NUM_EPISODES + 1):
        episode_start = time.time()

        state = env.reset()
        done = False

        while not done:
            action = agent.get_action(state)
            next_state, reward, done = env.step(action)

            agent.store_transition(state, action, reward, next_state, done)
            agent.train_step()

            state = next_state

        agent.update_epsilon()

        # 记录数据
        scores.append(env.score)
        total_score += env.score
        mean_score = total_score / episode

        # 计算本回合耗时
        episode_time = time.time() - episode_start
        episode_times.append(episode_time)

        # 计算统计信息
        if len(episode_times) > 100:  # 使用最近100回合计算平均时间
            avg_episode_time = np.mean(episode_times[-100:])
        else:
            avg_episode_time = np.mean(episode_times) if episode_times else 0

        # 计算预计剩余时间
        episodes_remaining = NUM_EPISODES - episode
        estimated_time_remaining = avg_episode_time * episodes_remaining

        # 格式化时间显示
        def format_time(seconds):
            if seconds < 60:
                return f"{seconds:.1f}秒"
            elif seconds < 3600:
                minutes = seconds / 60
                return f"{minutes:.1f}分钟"
            else:
                hours = seconds / 3600
                return f"{hours:.1f}小时"

        if episode % PRINT_EVERY == 0:
            # 计算总训练时间
            elapsed_time = time.time() - start_time

            print(f"Episode: {episode}/{NUM_EPISODES} "
                  f"| Score: {env.score:2d} "
                  f"| Mean Score: {mean_score:.2f} "
                  f"| Epsilon: {agent.epsilon:.3f} "
                  f"| Time: {format_time(elapsed_time)}"
                  f"| ETA: {format_time(estimated_time_remaining)}"
                  f"| Avg: {avg_episode_time:.3f}s/ep")

        # 定期保存模型（防止训练中断）
        if episode % SAVE_EVERY == 0:
            temp_path = MODEL_SAVE_PATH.removesuffix(
                ".pth") + f"_ep{episode}.pth"
            agent.save_model(temp_path)
            mean_scores.append(mean_score)

    # 最终保存模型
    agent.save_model(MODEL_SAVE_PATH)
    print(f"\n{'='*50}")
    print(f"Training completed!")
    print(f"Total training time: {format_time(time.time() - start_time)}")
    print(f"Final model saved to {MODEL_SAVE_PATH}")

    # 绘制数据曲线
    plot_training_results(scores, mean_scores)


def plot_training_results(scores, mean_scores):
    plt.figure(figsize=(12, 6))

    # 绘制原始分数（使用滑动平均平滑）
    window_size = min(100, len(scores) // 10)
    if window_size > 1:
        smoothed_scores = np.convolve(scores, np.ones(
            window_size)/window_size, mode='valid')
        plt.plot(range(window_size-1, len(scores)), smoothed_scores,
                 label=f"Score (Smoothed, window={window_size})", alpha=0.7, linewidth=1)

    plt.plot(scores, label="Score per Episode", alpha=0.3, linewidth=0.5)

    # 绘制平均分数
    if mean_scores:
        plt.plot(mean_scores, label="Mean Score", linewidth=2, color='red')

    plt.title(f"Training Curve: DQN + CNN Snake ({len(scores)} Episodes)")
    plt.xlabel("Episodes")
    plt.ylabel("Score")
    plt.legend()
    plt.grid(alpha=0.3)

    # 添加统计信息文本框
    stats_text = f"Max Score: {max(scores) if scores else 0}\n"
    stats_text += f"Avg Last 100: {np.mean(scores[-100:]) if len(scores) >= 100 else np.mean(scores):.2f}"
    plt.text(0.02, 0.98, stats_text, transform=plt.gca().transAxes,
             verticalalignment='top', bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

    plt.tight_layout()
    plt.savefig(PLOT_SAVE_PATH, dpi=150)
    plt.show()
    print(f"Training plot saved to {PLOT_SAVE_PATH}")


if __name__ == "__main__":
    train()
