import torch
from snake_env import SnakeEnv
from agent_dqn_cnn import AgentDQNCNN
from agent_dqn_mlp import AgentDQNMLP


def play(model_path):
    # 1. 加载模型存档
    checkpoint = torch.load(model_path, map_location='cpu')

    # 2. 自动从存档恢复环境和配置
    env_params = checkpoint['env_params']
    model_params = checkpoint['model_params']
    agent_params = checkpoint['agent_params']

    # 3. 实例化对应的 Agent
    if checkpoint['type'] == 'dqn_cnn':
        agent = AgentDQNCNN(model_params, agent_params)
    elif checkpoint['type'] == 'dqn_mlp':
        agent = AgentDQNMLP(model_params, agent_params)
    else:
        raise ValueError("Unknown model type")

    agent.load(model_path)
    env = SnakeEnv(**env_params)

    # 4. 运行游戏
    while True:
        state = env.reset()
        done = False
        while not done:
            env.render(fps=15)
            action = agent.get_action(state, evaluate=True)
            state, reward, done = env.step(action)
        print(f"Game Over! Score: {env.score}")


if __name__ == "__main__":
    # 修改这里来播放不同的模型
    play("../models/snake_dqn_mlp.pth")
