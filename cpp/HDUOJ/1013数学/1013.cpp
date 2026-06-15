#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    while (cin >> s) {
        if (s == "0") break;
        
        int sum = 0;
        for (char c : s) {
            sum += (c - '0');
        }
        
        // 使用数字根公式
        int result = (sum % 9 == 0) ? 9 : (sum % 9);
        cout << result << '\n';
    }
    return 0;
}