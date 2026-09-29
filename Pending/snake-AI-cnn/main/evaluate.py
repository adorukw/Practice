import numpy as np
from datetime import datetime
from dqn_agent import DQNAgent
from snake_env import SnakeEnv, GRID_HEIGHT, GRID_WIDTH, MAX_STEPS

# 可修改参数
DEFAULT_MODEL_LOAD_PATH = "../models/snake_cnn_model_ep70000.pth"
DEFAULT_TEST_EPISODES = 10000   # 评估的总回合数
DEFAULT_RENDER_EVERY = 0    # 每N回合渲染一次，0表示不渲染
DEFAULT_RENDER_FPS = 1000   # 渲染时的帧率
BATCH_EVALUATE = True  # 是否批量评估多个模型


def evaluate_model(model_load_path=None, test_episodes=None,
                   render_every=None, render_fps=None,
                   output_file="evaluated.txt", verbose=True):
    """
    加载模型并进行批量测试，输出性能统计报告。

    参数:
    - model_load_path: 要加载的模型文件路径
    - test_episodes: 评估的总回合数
    - render_every: 每N回合渲染一次，0表示不渲染
    - render_fps: 渲染时的帧率
    - output_file: 结果输出文件名
    - verbose: 是否在控制台打印详细信息
    """
    # 使用默认值或传入的参数
    model_load_path = model_load_path or DEFAULT_MODEL_LOAD_PATH
    test_episodes = test_episodes or DEFAULT_TEST_EPISODES
    render_every = render_every or DEFAULT_RENDER_EVERY
    render_fps = render_fps or DEFAULT_RENDER_FPS

    # 1. 初始化环境和智能体
    env = SnakeEnv()
    agent = DQNAgent(GRID_HEIGHT, GRID_WIDTH)

    # 2. 加载训练好的模型
    try:
        agent.load_model(model_load_path)
        if verbose:
            print(f"[信息] 成功加载模型: {model_load_path}")
    except Exception as e:
        print(f"[错误] 加载模型失败: {e}")
        return

    # 3. 初始化统计指标
    all_scores = []
    all_steps = []
    death_by_wall = 0
    death_by_self = 0
    death_by_timeout = 0

    if verbose:
        print(f"[信息] 开始性能评估，总回合数: {test_episodes}")
        print("-" * 50)

    # 4. 主测试循环
    for episode in range(1, test_episodes + 1):
        state = env.reset()
        done = False
        episode_steps = 0

        # 决定本回合是否渲染
        render_this_episode = (
            render_every > 0 and episode % render_every == 0)
        if render_this_episode:
            env.render(fps=render_fps)

        while not done:
            # 如果开启渲染，处理退出事件
            if render_this_episode:
                import pygame
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        pygame.quit()
                        if verbose:
                            print("\n[信息] 用户中断评估。")
                        _print_and_save_stats(
                            all_scores, all_steps, death_by_wall, death_by_self,
                            death_by_timeout, episode, model_load_path, output_file, verbose)
                        return

            # 使用纯贪心策略选择动作 (evaluate=True)
            action = agent.get_action(state, evaluate=True)
            next_state, reward, done = env.step(action)

            # 记录死亡原因（仅在游戏结束时判断）
            if done and episode_steps < env.steps:  # env.steps 是当前步数，在step()中已更新
                # 判断死亡原因需要检查蛇头位置
                head_x, head_y = env.head
                # 撞墙判断
                if head_x < 0 or head_x >= env.width or head_y < 0 or head_y >= env.height:
                    death_by_wall += 1
                # 步数超时判断
                elif env.steps > MAX_STEPS:
                    death_by_timeout += 1
                else:
                    # 默认为撞到自己身体
                    death_by_self += 1

            state = next_state
            episode_steps += 1

            # 渲染（如果开启）
            if render_this_episode:
                env.render(fps=render_fps)

        # 5. 记录本回合数据
        all_scores.append(env.score)
        all_steps.append(episode_steps)

        # 6. 定期打印进度
        if verbose and (episode % max(1, test_episodes // 10) == 0 or
                        episode == 1 or episode == test_episodes):
            avg_score_sofar = np.mean(all_scores) if all_scores else 0
            print(f"回合 [{episode:4d}/{test_episodes}] | "
                  f"本局分数: {env.score:3d} | "
                  f"本局步数: {episode_steps:4d} | "
                  f"平均分数: {avg_score_sofar:6.2f}")

    # 7. 关闭渲染窗口（如果曾打开）
    if render_every > 0:
        import pygame
        pygame.quit()

    # 8. 打印最终统计报告
    _print_and_save_stats(
        all_scores, all_steps, death_by_wall, death_by_self,
        death_by_timeout, test_episodes, model_load_path, output_file, verbose)


def _print_and_save_stats(scores, steps, wall, self, timeout,
                          total_episodes, model_path, output_file, verbose=True):
    """
    计算、打印并保存最终的统计指标到文件。
    """
    if not scores:
        msg = "没有收集到有效数据。\n"
        if verbose:
            print(msg)
        with open(output_file, 'a', encoding='utf-8') as f:
            f.write(msg)
        return

    scores_arr = np.array(scores)
    steps_arr = np.array(steps)

    # 构建报告内容
    report_lines = []
    report_lines.append("="*60)
    report_lines.append("模型性能评估报告")
    report_lines.append("="*60)
    report_lines.append(
        f"评估时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    report_lines.append(f"测试总回合数: {total_episodes}")
    report_lines.append(f"加载的模型: {model_path}")
    report_lines.append("-"*60)

    # 分数统计
    report_lines.append("[1] 得分 (Score) 统计:")
    report_lines.append(
        f"    平均值: {np.mean(scores_arr):.2f} ± {np.std(scores_arr):.2f}")
    report_lines.append(f"    中位数: {np.median(scores_arr):.2f}")
    report_lines.append(f"    最大值: {np.max(scores_arr)}")
    report_lines.append(f"    最小值: {np.min(scores_arr)}")
    report_lines.append(f"    第25百分位数: {np.percentile(scores_arr, 25):.2f}")
    report_lines.append(f"    第75百分位数: {np.percentile(scores_arr, 75):.2f}")

    # 步数/效率统计
    report_lines.append("\n[2] 步数/效率统计:")
    if np.sum(scores_arr) > 0:
        steps_per_food = np.sum(steps_arr) / np.sum(scores_arr)
        report_lines.append(f"    平均每获得1分所需步数: {steps_per_food:.2f}")
    report_lines.append(
        f"    平均每局步数: {np.mean(steps_arr):.2f} ± {np.std(steps_arr):.2f}")
    if np.sum(steps_arr) > 0:
        report_lines.append(
            f"    平均每步得分: {np.sum(scores_arr) / np.sum(steps_arr):.4f}")
    else:
        report_lines.append("    平均每步得分: 0.0000")

    # 生存/死亡统计
    report_lines.append("\n[3] 死亡原因分析:")
    total_deaths = wall + self + timeout
    if total_deaths > 0:
        report_lines.append(f"    撞墙次数: {wall} ({wall/total_deaths*100:.1f}%)")
        report_lines.append(
            f"    撞自身次数: {self} ({self/total_deaths*100:.1f}%)")
        report_lines.append(
            f"    步数超时次数: {timeout} ({timeout/total_deaths*100:.1f}%)")
    else:
        report_lines.append("    (无死亡记录)")

    # 性能评级
    avg_score = np.mean(scores_arr)
    report_lines.append("\n[4] 性能评级:")
    if avg_score >= 30:
        report_lines.append(f"    ★★★★★ 卓越 (平均分: {avg_score:.1f})")
    elif avg_score >= 20:
        report_lines.append(f"    ★★★★☆ 优秀 (平均分: {avg_score:.1f})")
    elif avg_score >= 10:
        report_lines.append(f"    ★★★☆☆ 良好 (平均分: {avg_score:.1f})")
    elif avg_score >= 5:
        report_lines.append(f"    ★★☆☆☆ 一般 (平均分: {avg_score:.1f})")
    else:
        report_lines.append(f"    ★☆☆☆☆ 有待提高 (平均分: {avg_score:.1f})")

    report_lines.append("="*60)
    report_lines.append("")  # 空行分隔

    # 在控制台打印
    if verbose:
        for line in report_lines:
            print(line)

    # 写入文件
    with open(output_file, 'a', encoding='utf-8') as f:
        f.write("\n" + "="*60 + "\n")
        f.write("模型性能评估报告\n")
        f.write("="*60 + "\n")
        f.write(f"评估时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        f.write(f"测试总回合数: {total_episodes}\n")
        f.write(f"加载的模型: {model_path}\n")
        f.write("-"*60 + "\n")

        # 将报告内容写入文件
        for line in report_lines[6:]:  # 跳过开头的标题行（已写入）
            f.write(line + "\n")


if __name__ == "__main__":
    # evaluate_model(
    #     model_load_path=DEFAULT_MODEL_LOAD_PATH,  # 指定模型路径
    #     test_episodes=DEFAULT_TEST_EPISODES,                            # 测试1000局
    #     render_every=DEFAULT_RENDER_EVERY,                                # 不渲染
    #     render_fps=DEFAULT_RENDER_FPS,                               # 渲染帧率
    #     output_file="evaluation_results.txt",         # 输出到指定文件
    #     verbose=True                                   # 在控制台显示进度
    # )

    if BATCH_EVALUATE:
        model_paths = []
        for start_idx in range(73000, 209000, 2000):
            model_paths.append(f"./models/snake_cnn_model_ep{start_idx}.pth")
        print(len(model_paths))

        for model_path in model_paths:
            print(f"\n评估模型: {model_path}")
            evaluate_model(
                model_load_path=model_path,
                test_episodes=DEFAULT_TEST_EPISODES,
                output_file="evaluation_results.txt",
                render_every=DEFAULT_RENDER_EVERY,
                render_fps=DEFAULT_RENDER_FPS,
                verbose=True
            )
