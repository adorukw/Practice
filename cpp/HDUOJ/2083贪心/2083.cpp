#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int M;
    cin >> M;
    while (M--) {
        int N;
        cin >> N;
        vector<int> points;
        for (int i = 0; i < N; i++) {
            int point;
            cin >> point;
            points.push_back(point);
        }
        sort(points.begin(), points.end());

        int center = points[N / 2];
        int sum = 0;
        for (int i = 0; i < N; i++) {
            sum += abs(points[i] - center);
        }

        cout << sum << endl;
    }
    return 0;
}