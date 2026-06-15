## 环境配置
```bash
conda create -n SnakeAI python=3.9 -y
conda activate SnakeAI
conda install numpy matplotlib pygame -c conda-forge -y
pip install torch torchvision torchaudio
python -c "import numpy, matplotlib, pygame, torch; print('所有库导入成功！')"
```