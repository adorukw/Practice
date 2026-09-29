import numpy as np
import pickle
from collections import defaultdict


class QLearningAgent:
    def __init__(self, action_size=4, alpha=0.1, gamma=0.9, epsilon=0.1):
        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon
        self.action_size = action_size
        self.q_table = defaultdict(lambda: np.zeros(action_size))

    def get_action(self, state, evaluate=False):
        state_key = tuple(state.flatten())
        if not evaluate and np.random.rand() < self.epsilon:
            return np.random.randint(self.action_size)
        return np.argmax(self.q_table[state_key])

    def learn(self, s, a, r, s_, d):
        s_key = tuple(s.flatten())
        s_next_key = tuple(s_.flatten())

        old_val = self.q_table[s_key][a]
        next_max = np.max(self.q_table[s_next_key])

        # Q-Learning update
        self.q_table[s_key][a] = old_val + self.alpha * \
            (r + self.gamma * next_max * (1-d) - old_val)

    def save(self, path, env_params):
        data = {
            'type': 'q_learning',
            'q_table': dict(self.q_table),
            'env_params': env_params,
            'agent_params': {'alpha': self.alpha, 'gamma': self.gamma, 'epsilon': self.epsilon}
        }
        with open(path, 'wb') as f:
            pickle.dump(data, f)

    def load(self, path):
        with open(path, 'rb') as f:
            data = pickle.load(f)
        self.q_table.update(data['q_table'])
