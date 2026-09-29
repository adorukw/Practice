import torch
import torch.nn as nn
import torch.optim as optim
import random
import numpy as np
from collections import deque
from model_dqn_cnn import DQNCNN


class AgentDQNCNN:
    def __init__(self, model_params, agent_params):
        self.device = torch.device(
            "cuda" if torch.cuda.is_available() else "cpu")
        self.model_params = model_params
        self.agent_params = agent_params

        self.policy_net = DQNCNN(**model_params).to(self.device)
        self.target_net = DQNCNN(**model_params).to(self.device)
        self.target_net.load_state_dict(self.policy_net.state_dict())

        self.optimizer = optim.Adam(
            self.policy_net.parameters(), lr=agent_params['lr'])
        self.memory = deque(maxlen=agent_params['memory_size'])
        self.gamma = agent_params['gamma']
        self.epsilon = agent_params['eps_start']
        self.eps_end = agent_params['eps_end']
        self.eps_decay = agent_params['eps_decay']
        self.batch_size = agent_params['batch_size']
        self.target_update = agent_params['target_update']
        self.steps = 0

    def get_action(self, state, evaluate=False):
        if not evaluate and random.random() < self.epsilon:
            return random.randint(0, 3)

        state_t = torch.FloatTensor(state).to(self.device)
        with torch.no_grad():
            return self.policy_net(state_t).argmax().item()

    def store(self, s, a, r, s_, d):
        self.memory.append((s, a, r, s_, d))

    def learn(self):
        if len(self.memory) < self.batch_size:
            return

        batch = random.sample(self.memory, self.batch_size)
        s, a, r, s_, d = zip(*batch)

        s = torch.FloatTensor(np.array(s)).to(self.device)
        a = torch.LongTensor(a).unsqueeze(1).to(self.device)
        r = torch.FloatTensor(r).unsqueeze(1).to(self.device)
        s_ = torch.FloatTensor(np.array(s_)).to(self.device)
        d = torch.FloatTensor(d).unsqueeze(1).to(self.device)

        curr_q = self.policy_net(s).gather(1, a)
        next_q = self.target_net(s_).max(1)[0].unsqueeze(1)
        target_q = r + (1 - d) * self.gamma * next_q

        loss = nn.MSELoss()(curr_q, target_q)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

        self.steps += 1
        if self.steps % self.target_update == 0:
            self.target_net.load_state_dict(self.policy_net.state_dict())

        if self.epsilon > self.eps_end:
            self.epsilon *= self.eps_decay

    def save(self, path, env_params):
        checkpoint = {
            'type': 'dqn_cnn',
            'model_state_dict': self.policy_net.state_dict(),
            'model_params': self.model_params,
            'agent_params': self.agent_params,
            'env_params': env_params,
            'epsilon': self.epsilon
        }
        torch.save(checkpoint, path)

    def load(self, path):
        checkpoint = torch.load(path, map_location=self.device)
        self.policy_net.load_state_dict(checkpoint['model_state_dict'])
        self.target_net.load_state_dict(checkpoint['model_state_dict'])
        self.epsilon = checkpoint.get('epsilon', self.epsilon)
