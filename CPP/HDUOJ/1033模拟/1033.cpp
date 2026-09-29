#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    
    // 读取每个测试用例
    while (getline(cin, s)) {
        // 跳过空行
        if (s.empty()) continue;
        
        // 初始位置和方向
        int x = 300, y = 420;
        int dir = 0;  // 0:右, 1:上, 2:左, 3:下
        
        // 输出起始指令
        cout << "300 420 moveto" << endl;
        cout << "310 420 lineto" << endl;
        
        // 更新当前位置
        x = 310;
        y = 420;
        
        // 处理每个折叠指令
        for (char c : s) {
            if (c == 'A') {
                // 顺时针转
                dir = (dir + 3) % 4;
            } else if (c == 'V') {
                // 逆时针转
                dir = (dir + 1) % 4;
            }
            
            // 根据新方向移动
            switch (dir) {
                case 0: x += 10; break;  // 右
                case 1: y += 10; break;  // 上
                case 2: x -= 10; break;  // 左
                case 3: y -= 10; break;  // 下
            }
            
            // 输出绘图指令
            cout << x << " " << y << " lineto" << endl;
        }
        
        // 结束指令
        cout << "stroke" << endl;
        cout << "showpage" << endl;
    }
    
    return 0;
}