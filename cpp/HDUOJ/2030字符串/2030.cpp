#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        getchar();
        for (int i = 0; i < n; i++) {
            string s;
            getline(cin, s);
            int count = 0;
            for (int i = 0; i < s.length(); i++) {
                if (s[i] < 0) count++;
            }
            cout << count / 2 << endl;
        }
    }

    return 0;
}