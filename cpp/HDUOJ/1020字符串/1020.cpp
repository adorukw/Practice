#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int caseNum;
    cin >> caseNum;
    for (int i = 0; i < caseNum; i++) {
        string str;
        cin >> str;
        int len = str.length();
        int count = 1;
        for (int j = 0; j < len; j++) {
            if (j != len - 1 && str[j] == str[j + 1])
                count++;
            else {
                if (count > 1)
                    cout << count << str[j];
                else
                    cout << str[j];
                count = 1;
            }
        }
        cout << '\n';
    }

    return 0;
}