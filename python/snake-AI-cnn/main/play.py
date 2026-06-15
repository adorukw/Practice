import pygame
from snake_env import SnakeEnv, GRID_HEIGHT, GRID_WIDTH
from dqn_agent import DQNAgent

# 可修改参数
MODEL_LOAD_PATH = "../models/snake_cnn_model_ep137000.pth"  # 要加载的模型文件
FPS = 10                                # 游戏运行帧率（数字越大越快）
TEST_EPISODES = 100000                       # 测试的回合数


def play():
    env = SnakeEnv()
    agent = DQNAgent(GRID_HEIGHT, GRID_WIDTH)

    try:
        agent.load_model(MODEL_LOAD_PATH)
        print(f"Successfully loaded model {MODEL_LOAD_PATH}")
    except Exception as e:
        print(f"Failed to load model: {e}")
        return

    for episode in range(1, TEST_EPISODES + 1):
        state = env.reset()
        done = False
        print(f"Starting Test Episode {episode}...")
        env.render(fps=FPS)

        while not done:
            # pygame 事件处理，允许随时关闭窗口
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    return

            # evaluate=True 意味着使用纯贪心策略，不引入随机探索
            action = agent.get_action(state, evaluate=True)
            state, reward, done = env.step(action)

            env.render(fps=FPS)

        print(f"Episode {episode} finished with Score: {env.score}")

    pygame.quit()


if __name__ == "__main__":
    play()
