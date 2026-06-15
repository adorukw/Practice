#include <climits>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int L, N, C, T, VR, VT1, VT2;
    while (cin >> L) {
        int N, C, T;
        cin >> N >> C >> T;
        int VR, VT1, VT2;
        cin >> VR >> VT1 >> VT2;

        vector<int> pos(N + 2);
        for (int i = 1; i <= N; i++) {
            cin >> pos[i];
        }
        pos[0] = 0, pos[N + 1] = L;

        int total = pos.size();
        vector<double> dp(total, INT_MAX);
        dp[0] = 0;

        for (int i = 1; i < total; i++) {
            for (int j = 0; j < i; j++) {
                int d = pos[i] - pos[j];
                double cost;

                if (d <= C)
                    cost = d / (double)VT1;
                else
                    cost = C / (double)VT1 + (d - C) / (double)VT2;

                double add = (j == 0 ? 0 : T);
                if (dp[j] + cost + add < dp[i]) dp[i] = dp[j] + cost + add;
            }
        }

        double timeR = L / (double)VR;
        double timeT = dp.back();

        if (timeR < timeT)
            cout << "Good job,rabbit!" << endl;
        else
            cout << "What a pity rabbit!" << endl;
    }

    return 0;
}