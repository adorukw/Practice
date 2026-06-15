import torch
import torch.nn as nn
import torch.nn.functional as F

class DQN(nn.Module):
    """
    用于近似Q值的卷积神经网络（CNN）模型。
    输入：状态图像 (形状: [batch_size, channels, height, width])
    输出：每个可选动作的Q值 (action_size 维)
    """

    def __init__(self, input_shape, action_size):
        """
        初始化网络层。
        参数:
            input_shape: 状态图像的形状 (元组: channels, height, width)。
                        例如，对于单通道6x6网格，输入为 (1, 6, 6)。
            action_size: 输出维度，即可选动作的数量。
        """
        super(DQN, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=input_shape[0], out_channels=16, kernel_size=3, stride=1, padding=1)
        self.conv2 = nn.Conv2d(in_channels=16, out_channels=32, kernel_size=3, stride=1, padding=1)
        
        # 动态计算全连接层的输入维度
        def conv2d_size_out(size, kernel_size=3, stride=1, padding=1):
            return (size + 2 * padding - kernel_size) // stride + 1
        
        convw = conv2d_size_out(input_shape[2])  # 宽度经过两次卷积后的尺寸
        convw = conv2d_size_out(convw)
        convh = conv2d_size_out(input_shape[1])  # 高度经过两次卷积后的尺寸
        convh = conv2d_size_out(convh)
        linear_input_size = convw * convh * 32  # 32是conv2的输出通道数
        
        self.fc1 = nn.Linear(linear_input_size, 128)
        self.fc2 = nn.Linear(128, action_size)

    def forward(self, x):
        """
        定义网络的前向传播过程。
        参数:
            x: 输入的状态张量，形状为 (batch_size, channels, height, width)。
        返回:
            每个动作对应的Q值，形状为 (batch_size, action_size)。
        """
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = x.view(x.size(0), -1)  # 展平
        x = F.relu(self.fc1(x))
        return self.fc2(x)