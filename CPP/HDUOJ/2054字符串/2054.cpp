#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

// 规范化数字字符串
string normalize(const string& s) {
    if (s.empty()) return s;

    string result = s;
    int sign = 1;  // 1表示正数，-1表示负数
    int i = 0;

    // 处理符号
    if (result[0] == '+') {
        i = 1;
    }
    else if (result[0] == '-') {
        sign = -1;
        i = 1;
    }

    // 找到小数点的位置
    size_t dot_pos = result.find('.');

    if (dot_pos == string::npos) {
        // 没有小数点，是整数
        // 去除前导零
        while (i < result.length() && result[i] == '0') {
            i++;
        }

        // 如果全是0
        if (i == result.length() || (sign == -1 && i == result.length())) {
            return "0";
        }

        result = result.substr(i);
    }
    else {
        // 有小数点
        // 处理整数部分
        string int_part = result.substr(i, dot_pos - i);
        string dec_part = result.substr(dot_pos + 1);

        // 去除整数部分的前导零
        int j = 0;
        while (j < int_part.length() && int_part[j] == '0') {
            j++;
        }
        int_part = int_part.substr(j);

        // 如果整数部分为空
        if (int_part.empty()) {
            int_part = "0";
        }

        // 去除小数部分的尾部零
        while (!dec_part.empty() && dec_part.back() == '0') {
            dec_part.pop_back();
        }

        // 构建结果
        if (dec_part.empty()) {
            result = int_part;
        }
        else {
            result = int_part + "." + dec_part;
        }
    }

    // 添加符号
    if (sign == -1 && result != "0") {
        result = "-" + result;
    }

    return result;
}

int main() {
    string A, B;

    // 注意：题目没有说明有多组测试数据，但通常有
    while (cin >> A >> B) {
        string normA = normalize(A);
        string normB = normalize(B);

        if (normA == normB) {
            cout << "YES" << endl;
        }
        else {
            cout << "NO" << endl;
        }
    }

    return 0;
}