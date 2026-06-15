#include <cctype>  // 使用标准字符判断函数
#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        // 关键：忽略读取n后的换行符
        cin.ignore();

        for (int i = 0; i < n; i++) {
            string s;
            getline(cin, s);  // 读取整行，包括空格

            bool isValid = true;

            // 检查字符串是否为空
            if (s.empty()) {
                cout << "no\n";
                continue;
            }

            // 检查第一个字符：必须是字母或下划线
            char firstChar = s[0];
            if (!((firstChar >= 'A' && firstChar <= 'Z') ||
                  (firstChar >= 'a' && firstChar <= 'z') || firstChar == '_')) {
                cout << "no\n";
                continue;
            }

            // 检查后续字符：必须是字母、数字或下划线
            for (size_t j = 1; j < s.length(); j++) {
                char c = s[j];
                if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                      (c >= '0' && c <= '9') || c == '_')) {
                    isValid = false;
                    break;
                }
            }

            cout << (isValid ? "yes\n" : "no\n");
        }
    }

    return 0;
}