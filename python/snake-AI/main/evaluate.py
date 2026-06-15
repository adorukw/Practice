import torch
import numpy as np
from snake_env import SnakeEnv
from agent_dqn_cnn import AgentDQNCNN
from agent_dqn_mlp import AgentDQNMLP


def evaluate(model_path, episodes=100):
    checkpoint = torch.load(model_path, map_location='cpu')
    env = SnakeEnv(**checkpoint['env_params'])

    if checkpoint['type'] == 'dqn_cnn':
        agent = AgentDQNCNN(
            checkpoint['model_params'], checkpoint['agent_params'])
    else:
        agent = AgentDQNMLP(
            checkpoint['model_params'], checkpoint['agent_params'])

    agent.load(model_path)

    scores = []
    for i in range(episodes):
        state = env.reset()
        done = False
        while not done:
            action = agent.get_action(state, evaluate=True)
            state, reward, done = env.step(action)
        scores.append(env.score)

    print(f"--- Evaluation for {model_path} ---")
    print(f"Episodes: {episodes}")
    print(f"Mean Score: {np.mean(scores):.2f}")
    print(f"Max Score: {np.max(scores)}")


if __name__ == "__main__":
    evaluate("snake_dqn_cnn.pth")
