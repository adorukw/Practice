#include <iostream>
#include <iomanip>  // 用于控制输出格式
using namespace std;

const double PI = 3.1415927;
const int INCHES_PER_FOOT = 12;
const int FEET_PER_MILE = 5280;
const int SECONDS_PER_HOUR = 3600;  // 60 * 60

int main() {
    double diameter, time;
    int revolutions;
    int tripCount = 1;  // 记录行程编号
    
    // 循环读取数据，直到转数为0
    while (cin >> diameter >> revolutions >> time && revolutions != 0) {
        // 计算周长（英寸）
        double circumference = PI * diameter;
        
        // 计算总距离（英寸）
        double totalInches = circumference * revolutions;
        
        // 转换为英里
        // 1英里 = 5280英尺，1英尺 = 12英寸
        double miles = totalInches / (INCHES_PER_FOOT * FEET_PER_MILE);
        
        // 计算平均速度（英里/小时）
        // 时间time是秒，转换为小时：time/3600
        double mph = miles / (time / SECONDS_PER_HOUR);
        
        // 输出结果，保留两位小数
        cout << fixed << setprecision(2);
        cout << "Trip #" << tripCount << ": " 
             << miles << " " << mph << endl;
        
        tripCount++;
    }
    
    return 0;
}