#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int A, B, n;
    while (std::cin >> A >> B >> n && (A || B || n)) {
        if (n == 1 || n == 2) {
            std::cout << 1 << '\n';
            continue;
        }

        std::vector<int> f(50, 0);
        f[1] = f[2] = 1;

        int i;
        for (i = 3; i <= 49; i++) {
            f[i] = (A * f[i - 1] + B * f[i - 2]) % 7;

            if (i > 3 && f[i] == 1 && f[i - 1] == 1) break;
        }

        int index;
        if (i > n) {
            index = n;
        }
        else {
            int period = i - 2;
            index = n % period;
            if (index == 0) index = period;
        }

        std::cout << f[index] << '\n';
    }

    return 0;
}