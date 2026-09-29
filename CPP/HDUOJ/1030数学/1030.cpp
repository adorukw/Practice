#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

void GetCoordinate(int n, int& level, int& left, int& right) {
    level = (int)sqrt(n - 1) + 1;
    left = (n - (level - 1) * (level - 1) - 1) / 2 + 1;
    right = (level * level - n) / 2 + 1;
}

int ShortestPath(int m, int n) {
    int leveM, leftM, rightM;
    int leveN, leftN, rightN;
    GetCoordinate(m, leveM, leftM, rightM);
    GetCoordinate(n, leveN, leftN, rightN);
    return abs(leveM - leveN) + abs(leftM - leftN) + abs(rightM - rightN);
}

int main() {
    int M, N;
    while (cin >> M >> N) {
        if (M > N) swap(M, N);
        int res = ShortestPath(M, N);
        cout << res << '\n';
    }

    return 0;
}