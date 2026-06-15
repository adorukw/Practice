import os
import sys
import pythoncom
from win32com.client import Dispatch
from win32com.shell import shell
from win32com.shell import shellcon

def create_shortcut_with_hotkey():
    # 检查是否在Windows系统上运行
    if sys.platform != 'win32':
        print("此程序仅支持Windows系统")
        return

    try:
        # 屏幕保护程序路径
        scr_path = r"C:\Windows\System32\scrnsave.scr"
        if not os.path.exists(scr_path):
            raise FileNotFoundError("未找到屏幕保护程序文件")

        # 获取桌面路径
        desktop = shell.SHGetFolderPath(0, shellcon.CSIDL_DESKTOP, None, 0)
        shortcut_path = os.path.join(desktop, "屏幕保护.lnk")

        # 创建快捷方式
        wsh_shell = Dispatch('WScript.Shell')
        shortcut = wsh_shell.CreateShortCut(shortcut_path)
        shortcut.TargetPath = scr_path
        shortcut.WorkingDirectory = r"C:\Windows\System32"
        shortcut.save()

        # 设置快捷键 (Ctrl+Alt+S)
        with open(shortcut_path, 'rb') as f:
            data = bytearray(f.read())
        
        # 快捷键位置偏移量
        # Ctrl(0x02) + Alt(0x04) = 0x06, S键的虚拟键码为0x53
        data[0x15] = 0x06  # 修饰键
        data[0x16] = 0x53  # 虚拟键码 (S键)

        with open(shortcut_path, 'wb') as f:
            f.write(data)

        print(f"已创建快捷方式到桌面：{shortcut_path}")
        print("快捷键设置为 Ctrl+Alt+S")

    except Exception as e:
        print(f"操作失败：{str(e)}")

if __name__ == '__main__':
    # 初始化COM库
    pythoncom.CoInitialize()
    create_shortcut_with_hotkey()
    pythoncom.CoUninitialize()