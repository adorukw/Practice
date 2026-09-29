import torch
import torch.nn as nn
import torch.nn.functional as F

# 可修改参数
INPUT_SIZE = 11  # 状态特征维度，对应snake_env.py中get_state()的返回值长度。
HIDDEN_SIZE = 128  # 隐藏层神经元数量。
OUTPUT_SIZE = 4  # 输出维度，即可选动作的数量。


class DQN(nn.Module):
    """
    用于近似Q值的深度Q网络（DQN）模型。
    输入：状态 (state_size 维)
    输出：每个可选动作的Q值 (action_size 维)
    """

    def __init__(self, input_size=INPUT_SIZE, hidden_size=HIDDEN_SIZE, output_size=OUTPUT_SIZE):
        """
        初始化网络层。
        参数:
            input_size: 状态特征维度，对应snake_env.py中get_state()的返回值长度。
            hidden_size: 隐藏层神经元数量。
            output_size: 输出维度，即可选动作的数量。
        """
        super(DQN, self).__init__()
        # 定义三个全连接层
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        """
        定义网络的前向传播过程。
        参数:
            x: 输入的状态张量。
        返回:
            每个动作对应的Q值。
        """
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        return self.fc3(x)
