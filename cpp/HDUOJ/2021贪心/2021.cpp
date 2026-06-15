#include <iostream>

using namespace std;

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    const int money[] = {100, 50, 10, 5, 2, 1};

    int n;
    while (cin >> n) {
        if (n == 0) break;

        int sum = 0;
        for (int i = 0; i < n; i++) {
            int x;
            cin >> x;
            while (x > 0) {
                if (x >= 100)
                    x -= 100;
                else if (x >= 50)
                    x -= 50;
                else if (x >= 10)
                    x -= 10;
                else if (x >= 5)
                    x -= 5;
                else if (x >= 2)
                    x -= 2;
                else
                    x -= 1;
                sum += 1;
            }
        }
        cout << sum << '\n';
    }
}