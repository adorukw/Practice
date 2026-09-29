import torch.nn as nn
import torch.nn.functional as F


class DQNCNN(nn.Module):
    def __init__(self, grid_h, grid_w, in_channels=3, conv_filters=[16, 32], fc_hidden=256):
        super(DQNCNN, self).__init__()
        self.conv1 = nn.Conv2d(
            in_channels, conv_filters[0], kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(
            conv_filters[0], conv_filters[1], kernel_size=3, padding=1)
        self.flat_size = conv_filters[1] * grid_h * grid_w
        self.fc1 = nn.Linear(self.flat_size, fc_hidden)
        self.fc2 = nn.Linear(fc_hidden, 4)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = x.view(-1, self.flat_size)
        x = F.relu(self.fc1(x))
        return self.fc2(x)
