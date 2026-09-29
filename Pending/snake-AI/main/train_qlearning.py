from snake_env import SnakeEnv
from agent_qlearning import QLearningAgent

# Q-learning 状态空间很大，建议用非常小的网格(如 6x6)
ENV_CONFIG = {
    'width': 6, 'height': 6, 'state_mode': 'mlp',
    'reward_food': 10.0, 'reward_death': -10.0
}

AGENT_CONFIG = {
    'alpha': 0.1, 'gamma': 0.9, 'epsilon': 0.1
}

TRAIN_EPISODES = 5000
SAVE_PATH = "snake_qtable.pkl"


def train():
    env = SnakeEnv(**ENV_CONFIG)
    agent = QLearningAgent(**AGENT_CONFIG)

    for ep in range(1, TRAIN_EPISODES + 1):
        state = env.reset()
        done = False
        while not done:
            action = agent.get_action(state)
            next_state, reward, done = env.step(action)
            agent.learn(state, action, reward, next_state, done)
            state = next_state

        if ep % 500 == 0:
            print(f"Ep: {ep}, Score: {env.score}")
            agent.save(SAVE_PATH, ENV_CONFIG)


if __name__ == "__main__":
    train()
