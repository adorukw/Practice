from snake_env import SnakeEnv
from agent_dqn_mlp import AgentDQNMLP

# ================= 参数配置 =================
ENV_CONFIG = {
    'width': 10, 'height': 10, 'state_mode': 'mlp',
    'reward_food': 10.0, 'reward_death': -10.0, 'max_steps': 500
}

MODEL_CONFIG = {
    'input_size': 10, 'hidden_size': 128
}

AGENT_CONFIG = {
    'lr': 1e-4, 'gamma': 0.99, 'batch_size': 64, 'memory_size': 20000,
    'eps_start': 1.0, 'eps_end': 0.05, 'eps_decay': 0.995, 'target_update': 100
}

TRAIN_EPISODES = 2000
SAVE_PATH = "../models/snake_dqn_mlp.pth"
# ============================================


def train():
    env = SnakeEnv(**ENV_CONFIG)
    agent = AgentDQNMLP(MODEL_CONFIG, AGENT_CONFIG)

    for ep in range(1, TRAIN_EPISODES + 1):
        state = env.reset()
        done = False
        while not done:
            action = agent.get_action(state)
            next_state, reward, done = env.step(action)
            agent.store(state, action, reward, next_state, done)
            agent.learn()
            state = next_state

        if ep % 50 == 0:
            print(f"Ep: {ep}, Score: {env.score}, Eps: {agent.epsilon:.2f}")
            agent.save(SAVE_PATH, ENV_CONFIG)


if __name__ == "__main__":
    train()
