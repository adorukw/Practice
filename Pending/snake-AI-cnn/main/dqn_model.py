import torch.nn as nn
import torch.nn.functional as F

# 可修改参数
IN_CHANNELS = 3         # 输入通道数(食物, 蛇头, 蛇身)
CONV1_FILTERS = 16      # 第一层卷积核数量
CONV2_FILTERS = 32      # 第二层卷积核数量
FC_HIDDEN_SIZE = 256    # 全连接层隐藏节点数
NUM_ACTIONS = 4         # 动作数量 (上下左右)


class DQN(nn.Module):
    def __init__(self, grid_h, grid_w):
        super(DQN, self).__init__()

        # 卷积层，使用 padding=1 保持空间维度一致
        self.conv1 = nn.Conv2d(IN_CHANNELS, CONV1_FILTERS,
                               kernel_size=3, stride=1, padding=1)
        self.conv2 = nn.Conv2d(CONV1_FILTERS, CONV2_FILTERS,
                               kernel_size=3, stride=1, padding=1)

        # 展平后的维度
        flat_size = CONV2_FILTERS * grid_h * grid_w

        # 全连接层
        self.fc1 = nn.Linear(flat_size, FC_HIDDEN_SIZE)
        self.fc2 = nn.Linear(FC_HIDDEN_SIZE, NUM_ACTIONS)

    def forward(self, x):
        # 输入 x shape: (Batch, Channels, Height, Width)
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))

        # 展平特征图
        x = x.view(x.size(0), -1)

        x = F.relu(self.fc1(x))
        out = self.fc2(x)
        return out
