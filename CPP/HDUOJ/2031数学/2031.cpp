#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N, R;
    while (cin >> N >> R) {
        if (N == 0) {
            cout << 0 << endl;
            continue;
        }

        bool isNegative = false;
        if (N < 0) {
            N = -N;
            isNegative = true;
        }

        vector<char> digits;

        while (N > 0) {
            int reminder = N % R;

            if (reminder < 10)
                digits.push_back(reminder + '0');
            else
                digits.push_back(reminder - 10 + 'A');
            N /= R;
        }
        if (isNegative) cout << '-';
        for (auto i = digits.rbegin(); i != digits.rend(); i++) {
            cout << *i;
        }
        cout << endl;
    }

    return 0;
}