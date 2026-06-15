#include <algorithm>
#include <cctype>
#include <climits>
#include <iostream>
#include <string>

using namespace std;

// 将十六进制字符串转换为long long
long long HexToDec(const string& hex) {
    if (hex.empty()) return 0;

    long long dec = 0;
    for (char c : hex) {
        dec <<= 4;  // 等价于 dec *= 16
        if (isdigit(c)) {
            dec += c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            dec += c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            dec += c - 'a' + 10;
        }
    }
    return dec;
}

// 将long long转换为十六进制字符串
string DecToHex(long long dec) {
    if (dec == 0) return "0";

    bool negative = false;
    if (dec < 0) {
        negative = true;
        dec = -dec;  // 注意：对于LONG_LONG_MIN，这里会溢出
    }

    string hex = "";
    while (dec > 0) {
        int remainder = dec % 16;
        char digit;
        if (remainder < 10) {
            digit = '0' + remainder;
        }
        else {
            digit = 'A' + remainder - 10;
        }
        hex = digit + hex;  // 效率较低，但对于长度<15可接受
        dec /= 16;
    }

    if (negative) {
        hex = "-" + hex;
    }

    return hex;
}

// 处理LONG_LONG_MIN的特殊情况
string DecToHexSafe(long long dec) {
    if (dec == 0) return "0";

    // 处理 LONG_LONG_MIN
    if (dec == LLONG_MIN) {
        // LONG_LONG_MIN的十六进制表示
        return "-8000000000000000";
    }

    return DecToHex(dec);
}

int main() {
    string A_str, B_str;

    while (cin >> A_str >> B_str) {
        // 解析A
        int signA = 1;
        if (!A_str.empty()) {
            if (A_str[0] == '-') {
                signA = -1;
                A_str = A_str.substr(1);
            }
            else if (A_str[0] == '+') {
                A_str = A_str.substr(1);
            }
        }

        // 解析B
        int signB = 1;
        if (!B_str.empty()) {
            if (B_str[0] == '-') {
                signB = -1;
                B_str = B_str.substr(1);
            }
            else if (B_str[0] == '+') {
                B_str = B_str.substr(1);
            }
        }

        // 转换为十进制
        long long A_val = HexToDec(A_str) * signA;
        long long B_val = HexToDec(B_str) * signB;

        // 计算和
        long long sum = A_val + B_val;

        // 转换为十六进制输出
        cout << DecToHexSafe(sum) << endl;
    }

    return 0;
}