from snake_env import SnakeEnv
from agent_dqn_cnn import AgentDQNCNN

# ================= 参数配置 =================
ENV_CONFIG = {
    'width': 10, 'height': 10, 'state_mode': 'cnn',
    'reward_food': 10.0, 'reward_death': -10.0, 'max_steps': 500
}

MODEL_CONFIG = {
    'grid_h': 10, 'grid_w': 10, 'in_channels': 3, 'conv_filters': [16, 32], 'fc_hidden': 256
}

AGENT_CONFIG = {
    'lr': 1e-4, 'gamma': 0.99, 'batch_size': 64, 'memory_size': 50000,
    'eps_start': 1.0, 'eps_end': 0.05, 'eps_decay': 0.998, 'target_update': 200
}

TRAIN_EPISODES = 2000
SAVE_PATH = "snake_dqn_cnn.pth"
# ============================================


def train():
    env = SnakeEnv(**ENV_CONFIG)
    agent = AgentDQNCNN(MODEL_CONFIG, AGENT_CONFIG)

    for ep in range(1, TRAIN_EPISODES + 1):
        state = env.reset()
        total_reward = 0
        done = False

        while not done:
            action = agent.get_action(state)
            next_state, reward, done = env.step(action)
            agent.store(state, action, reward, next_state, done)
            agent.learn()
            state = next_state
            total_reward += reward

        if ep % 50 == 0:
            print(f"Ep: {ep}, Score: {env.score}, Epsilon: {agent.epsilon:.2f}")
            agent.save(SAVE_PATH, ENV_CONFIG)


if __name__ == "__main__":
    train()
