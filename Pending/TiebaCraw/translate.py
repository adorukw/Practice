import pygame
import time

def clip_sprite_sheet(spritesheet, x, y, width, height, colorkey=None, scale=None):
    """
    从精灵图中截取指定位置的子图像，并可选择缩放
    
    参数:
    spritesheet (pygame.Surface): 原始精灵图表面
    x (int): 截取区域的左上角 x 坐标
    y (int): 截取区域的左上角 y 坐标
    width (int): 截取区域的宽度
    height (int): 截取区域的高度
    colorkey (tuple or None): 设置透明色，如 (0, 0, 0) 表示黑色透明
    scale (tuple/float or None): 缩放比例 (scale_x, scale_y) 或统一缩放比例
    
    返回:
    pygame.Surface: 新创建的独立子图像表面
    """
    # 创建透明表面作为目标图像
    sprite = pygame.Surface((width, height), pygame.SRCALPHA)
    
    # 从精灵图上截取指定区域
    sprite.blit(spritesheet, (0, 0), (x, y, width, height))
    
    # 设置透明色（可选）
    if colorkey is not None:
        sprite.set_colorkey(colorkey)
    
    # 缩放图像（如果指定了缩放比例）
    if scale is not None:
        if isinstance(scale, (int, float)):
            # 统一缩放
            new_size = (int(width * scale), int(height * scale))
        elif isinstance(scale, tuple) and len(scale) == 2:
            # 分别指定宽高缩放
            new_size = (int(width * scale[0]), int(height * scale[1]))
        else:
            raise ValueError("scale 参数必须是数字或 (scale_x, scale_y) 元组")
        
        # 执行缩放（平滑缩放）
        sprite = pygame.transform.smoothscale(sprite, new_size)
    
    return sprite

def play_animation(screen, frames, position, frame_delay=0.1, loop=True):
    """
    播放帧序列动画
    
    参数:
    screen (pygame.Surface): 游戏窗口表面
    frames (list): 帧序列列表
    position (tuple): 动画在屏幕上的位置 (x, y)
    frame_delay (float): 每帧之间的延迟（秒）
    loop (bool): 是否循环播放
    
    返回:
    bool: 如果动画结束返回True，否则False
    """
    current_time = time.time()
    if not hasattr(play_animation, 'last_time'):
        play_animation.last_time = current_time
        play_animation.current_frame = 0
    
    # 计算是否应该切换到下一帧
    if current_time - play_animation.last_time >= frame_delay:
        play_animation.current_frame += 1
        play_animation.last_time = current_time
        
        # 检查是否到达帧序列末尾
        if play_animation.current_frame >= len(frames):
            if loop:
                play_animation.current_frame = 0  # 循环播放
            else:
                return True  # 动画结束
    
    # 绘制当前帧
    screen.blit(frames[play_animation.current_frame], position)
    
    return False  # 动画仍在播放中

# 使用示例
if __name__ == "__main__":
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    pygame.display.set_caption("精灵动画演示")
    clock = pygame.time.Clock()
    
    # 加载精灵图（确保图像路径正确）
    try:
        spritesheet = pygame.image.load("mario_bros.png").convert_alpha()
    except:
        # 如果加载失败，创建一个替代精灵图
        print("警告：无法加载精灵图，使用默认测试图")
        spritesheet = pygame.Surface((256, 64), pygame.SRCALPHA)
        spritesheet.fill((0, 100, 200))
        for i in range(4):
            pygame.draw.circle(spritesheet, (255, 255, 0), (i * 64 + 32, 32), 30)
            pygame.draw.circle(spritesheet, (0, 0, 0), (i * 64 + 32, 32), 30, 2)
    
    # 创建帧序列（带缩放）
    frames = []
    for i in range(19):  # 假设有4帧
        # 第0帧不缩放，其他帧按位置逐渐放大
        scale_factor = 1.0 + i * 0.2
        # 提取并缩放每一帧
        frame = clip_sprite_sheet(
            spritesheet, 
            i * 80,  # 每个精灵宽度64像素
            0, 
            16, 32, 
            scale=(scale_factor, scale_factor)
        )
        frames.append(frame)
    
    # 另一组动画帧（带颜色键）
    colored_frames = []
    for i in range(19):
        # 提取帧并缩放为50%
        frame = clip_sprite_sheet(
            spritesheet,
            i * 80,
            0,
            16, 32,
            colorkey=(0, 100, 200),  # 设置透明色
            scale=1
        )
        colored_frames.append(frame)
    
    # 主循环
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
        
        screen.fill((50, 100, 200))  # 蓝色背景
        
        # 播放底部动画（位置：屏幕底部，缩小50%，不循环）
        end1 = play_animation(
            screen, 
            colored_frames, 
            (400 - 16, 480),  # 位置
            frame_delay=10,
        )
        
        # 播放顶部动画（位置：屏幕顶部）
        end2 = play_animation(
            screen, 
            frames, 
            (400 - 32 * 1.2, 100),  # 位置
            frame_delay=0.5
        )
        
        # 中间播放反转帧序列的动画
        reversed_frames = list(reversed(frames))
        end3 = play_animation(
            screen, 
            reversed_frames, 
            (400 - 32 * 1.2, 300),  # 位置
            frame_delay=0.1
        )
        
        # 显示说明文字
        font = pygame.font.SysFont(None, 24)
        text = font.render("按ESC退出 | 顶部: 放大动画 | 中间: 反向动画 | 底部: 缩小透明动画", True, (255, 255, 255))
        screen.blit(text, (10, 10))
        
        pygame.display.flip()
        clock.tick(600)
    
    pygame.quit()