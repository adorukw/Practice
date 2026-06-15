#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

// 计算最小夹角
double minAngle(double deg) {
    // 确保角度在 [0, 360) 范围内
    deg = fmod(deg, 360.0);
    if (deg < 0) deg += 360.0;
    // 返回最小夹角（小于等于180度）
    return min(deg, 360.0 - deg);
}

int main() {
    double D;
    // 12小时的秒数，也是时针分针的一个相对运动周期
    const double half_day = 12.0 * 60.0 * 60.0; // 43200 seconds
    
    while (cin >> D) {
        if (D < 0) break;
        
        vector<double> events;
        events.push_back(0.0);    // 起始时间：0秒
        events.push_back(half_day); // 结束时间：43200秒（12小时）
        
        // 三对指针的相对角速度（度/秒）
        // 时针速度: 360 / (12 * 60 * 60) = 1/120 deg/s
        // 分针速度: 360 / (60 * 60) = 1/10 deg/s
        // 秒针速度: 360 / 60 = 6 deg/s
        
        double speeds[3]; // 存储相对角速度
        speeds[0] = 1.0/10.0 - 1.0/120.0;   // 分针相对时针： (1/10 - 1/120) = 11/120 deg/s
        speeds[1] = 6.0 - 1.0/120.0;         // 秒针相对时针： (6 - 1/120) = 719/120 deg/s
        speeds[2] = 6.0 - 1.0/10.0;          // 秒针相对分针： (6 - 1/10) = 59/10 deg/s
        
        // 为每一对指针生成关键事件点
        for (int i = 0; i < 3; i++) {
            double v = speeds[i];
            // 相对运动一圈是360度，周期是 T = 360 / |v|
            double T = 360.0 / fabs(v);
            
            // 在12小时内，会发生多次相对运动
            int cycles = int(half_day / T) + 2; // 多算几个周期以防万一
            
            for (int j = 0; j <= cycles; j++) {
                double base_time = j * T; // 每个周期的开始时间
                
                // 关键事件1：角度差变为D度（可能进入或离开幸福状态）
                double t1 = base_time + D / fabs(v);
                // 关键事件2：角度差变为(360-D)度
                double t2 = base_time + (360.0 - D) / fabs(v);
                
                // 只添加在 [0, half_day] 范围内的时间点
                if (t1 < half_day) events.push_back(t1);
                if (t2 < half_day) events.push_back(t2);
            }
        }
        
        // 排序并去重
        sort(events.begin(), events.end());
        vector<double> unique_events;
        for (size_t i = 0; i < events.size(); i++) {
            if (i == 0 || events[i] - events[i-1] > 1e-10) { // 避免浮点数重复
                unique_events.push_back(events[i]);
            }
        }
        events = unique_events;
        
        // 计算总幸福时间
        double happy_time = 0.0;
        
        for (size_t i = 0; i < events.size() - 1; i++) {
            double start = events[i];
            double end = events[i+1];
            double mid = (start + end) / 2.0; // 取区间中点进行判断
            
            // 计算中点时刻三个指针的角度（度）
            double h_angle = fmod(mid / 120.0, 360.0);        // 时针： mid/120
            double m_angle = fmod(mid / 10.0, 360.0);          // 分针： mid/10
            double s_angle = fmod(6.0 * mid, 360.0);          // 秒针： 6*mid
            
            // 计算三对指针的最小夹角
            double diff_hm = minAngle(h_angle - m_angle);
            double diff_hs = minAngle(h_angle - s_angle);
            double diff_ms = minAngle(m_angle - s_angle);
            
            // 如果三对指针的最小夹角都 >= D，则整个区间都是幸福时间
            if (diff_hm >= D - 1e-10 && diff_hs >= D - 1e-10 && diff_ms >= D - 1e-10) {
                happy_time += (end - start);
            }
        }
        
        // 因为12小时的模式在一天内重复两次
        happy_time *= 2.0;
        
        // 计算幸福时间占一天总时间（86400秒）的百分比
        double percentage = (happy_time / 86400.0) * 100.0;
        
        // 输出结果，保留三位小数
        cout << fixed << setprecision(3) << percentage << endl;
    }
    
    return 0;
}