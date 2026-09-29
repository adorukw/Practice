import pygame
import time
from snake_env import SnakeGame
from agent_qlearning import QLearningAgent
from agent_dqn_mlp import DQNAgent as DQN_MLPAgent

# ==================== 可修改的配置参数 (START) ====================
# 模型路径配置
QLEARNING_MODEL_PATH = '../models/qlearning_agent.pkl'  # Q-learning 智能体模型文件的保存/加载路径
DQN_MLP_MODEL_PATH = '../models/dqn_mlp_agent.pkl'      # DQN MLP 智能体模型文件的保存/加载路径

# 游戏环境配置
WIDTH = 20                         # 游戏网格的宽度（单位：格子数）
HEIGHT = 20                        # 游戏网格的高度（单位：格子数）

# 人类玩家模式配置
HUMAN_FPS = 10                     # 人类玩家模式下的游戏帧率（每秒帧数），控制游戏速度
HUMAN_QUIT_KEY = pygame.K_ESCAPE  # 人类玩家模式下，退出游戏的按键（pygame 键盘常量）

# AI玩家模式通用配置
AI_TEST_EPISODES = 10               # 每次运行AI演示时，连续进行的游戏对局数
AI_ACTION_DELAY_MS = 25            # AI每执行一个动作后的图形界面延迟（毫秒），便于观察
AI_EPISODE_INTERVAL = 3            # AI每局游戏结束后的暂停时间（秒）

# AI玩家模式 - Q-learning 特定配置
# 传递给 QLearningAgent 的状态向量维度，必须与 `snake_env.py` 中 `get_state()` 的返回值长度一致
QL_STATE_SIZE = 11
QL_ACTION_SIZE = 3                 # QLearningAgent 的动作空间大小（0: 无操作/直行, 1: 右转, 2: 左转）

# 训练模式配置
TRAIN_EPISODES = 1000              # 通过本文件菜单选择训练时，默认的训练轮次
# ==================== 可修改的配置参数 (END) ====================


def play_human():
    print('人类玩家模式')
    print('控制：方向键控制移动')
    print('-'*30)

    env = SnakeGame(width=WIDTH, height=HEIGHT, gui=True)
    state = env.reset()

    running = True
    clock = pygame.time.Clock()

    while running:
        action = 0
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    action = 2
                elif event.key == pygame.K_DOWN:
                    action = 1
                elif event.key == pygame.K_LEFT:
                    action = 2
                elif event.key == pygame.K_RIGHT:
                    action = 1
                elif event.key == HUMAN_QUIT_KEY:  # 使用配置的退出键
                    running = False
        state, reward, done, info = env.step(action)

        if done:
            print(f"游戏结束！分数: {info['score']}")
            time.sleep(AI_EPISODE_INTERVAL)  # 使用配置的局间间隔
            break

        clock.tick(HUMAN_FPS)  # 使用配置的帧率

    env.close()


def play_ai_qlearning(model_path=QLEARNING_MODEL_PATH):
    print("🤖 AI玩家模式 (Q-learning)")
    print("目标: 观看AI如何玩游戏")
    print("-" * 30)

    # 使用配置的状态和动作空间大小初始化智能体
    agent = QLearningAgent(state_size=QL_STATE_SIZE,
                           action_size=QL_ACTION_SIZE)
    if not agent.load(model_path):
        print("❌ 找不到模型文件，请先训练模型")
        return

    env = SnakeGame(width=WIDTH, height=HEIGHT, gui=True)

    episodes = AI_TEST_EPISODES  # 使用配置的测试局数
    episode_scores = []

    for episode in range(episodes):
        print(f"第 {episode + 1}/{episodes} 局")

        state = env.reset()
        total_reward = 0
        done = False

        while not done:
            action = agent.get_action(state)

            state, reward, done, info = env.step(action)
            total_reward += reward

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    env.close()
                    return

            pygame.time.delay(AI_ACTION_DELAY_MS)  # 使用配置的动作延迟

        score = info['score']
        episode_scores.append(score)
        print(f'分数：{score}，总奖励：{total_reward:.1f}')

        time.sleep(AI_EPISODE_INTERVAL)  # 使用配置的局间间隔

    env.close()

    print(f"\n📊 统计:")
    print(f"  平均分数: {sum(episode_scores)/len(episode_scores):.1f}")
    print(f"  最高分数: {max(episode_scores)}")
    print(f"  最低分数: {min(episode_scores)}")


def play_ai_dqn_mlp(model_path=DQN_MLP_MODEL_PATH):
    print("🤖 AI玩家模式 (DQN MLP)")
    print("目标: 观看AI如何玩游戏")
    print("-" * 30)

    from agent_dqn_mlp import create_agent_from_checkpoint

    agent = create_agent_from_checkpoint(model_path)
    if not agent.load(model_path):
        print("❌ 找不到模型文件，请先训练模型")
        return

    env = SnakeGame(width=WIDTH, height=HEIGHT, gui=True)

    episodes = AI_TEST_EPISODES  # 使用配置的测试局数
    episode_scores = []

    for episode in range(episodes):
        print(f"第 {episode + 1}/{episodes} 局")

        state = env.reset()
        total_reward = 0
        done = False

        while not done:
            action = agent.get_action(state, training=False)

            state, reward, done, info = env.step(action)
            total_reward += reward

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    env.close()
                    return

            pygame.time.delay(AI_ACTION_DELAY_MS)  # 使用配置的动作延迟

        score = info['score']
        episode_scores.append(score)
        print(f'分数：{score}，总奖励：{total_reward:.1f}')

        time.sleep(AI_EPISODE_INTERVAL)  # 使用配置的局间间隔

    env.close()

    print(f"\n📊 统计:")
    print(f"  平均分数: {sum(episode_scores)/len(episode_scores):.1f}")
    print(f"  最高分数: {max(episode_scores)}")
    print(f"  最低分数: {min(episode_scores)}")


if __name__ == "__main__":
    import sys

    print("贪吃蛇游戏演示")
    print("=" * 40)
    print("1. 👤 人类玩家")
    print("2. 🤖 QLearning AI玩家")
    print("3. 🤖 DQN MLP AI玩家")
    print("4. 🔍 分析AI")
    print("5. 🎬 训练AI")
    print("=" * 40)

    choice = input("请选择 (1-5): ").strip()

    if choice == '1':
        play_human()
    elif choice == '2':
        play_ai_qlearning(QLEARNING_MODEL_PATH)
    elif choice == '3':
        play_ai_dqn_mlp(DQN_MLP_MODEL_PATH)
    elif choice == '4':
        play_ai_qlearning(QLEARNING_MODEL_PATH)
    elif choice == '5':
        from train_qlearning import train
        train(episodes=TRAIN_EPISODES)  # 使用配置的训练轮次
    else:
        print("无效选择")
