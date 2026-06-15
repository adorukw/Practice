#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool IsIncluded(char c, vector<char> v) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == c) return true;
    }

    return false;
}

int main() {
    vector<char> upper(26, '0');
    vector<char> lower(26, '0');
    vector<char> num = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    vector<char> other = {'~', '!', '@', '#', '$', '%', '^'};
    for (int i = 0; i < 26; i++) {
        upper[i] = 'A' + i;
        lower[i] = 'a' + i;
    }

    int M;
    while (cin >> M) {
        for (int i = 0; i < M; i++) {
            string s;
            cin >> s;
            bool isSafe = true;
            if (s.length() < 8 || s.length() > 16) {
                isSafe = false;
                cout << "NO" << endl;
                continue;
            }
            int count = 0;
            bool hasUpper = false, hasLower = false, hasNum = false,
                 hasOther = false;
            for (int j = 0; j < s.length(); j++) {
                if (IsIncluded(s[j], upper) && !hasUpper) {
                    hasUpper = true;
                    count++;
                    continue;
                }
                if (IsIncluded(s[j], lower) && !hasLower) {
                    hasLower = true;
                    count++;
                    continue;
                }
                if (IsIncluded(s[j], num) && !hasNum) {
                    hasNum = true;
                    count++;
                    continue;
                }
                if (IsIncluded(s[j], other) && !hasOther) {
                    hasOther = true;
                    count++;
                    continue;
                }
            }
            if (count < 3) {
                isSafe = false;
                cout << "NO" << endl;
                continue;
            }
            cout << "YES" << endl;
        }
    }

    return 0;
}