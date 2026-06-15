#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        if (n == 0) continue;
        vector<pair<int, int>> times;
        for (int i = 0; i < n; i++) {
            int Ti_s, Ti_e;
            cin >> Ti_s >> Ti_e;
            times.push_back(make_pair(Ti_s, Ti_e));
        }
        sort(times.begin(), times.end(),
             [](pair<int, int> a, pair<int, int> b) {
                 return a.second < b.second;
             });
        int res = 0;
        int last = 0;
        for (int i = 0; i < n; i++) {
            if (times[i].first >= last) {
                res++;
                last = times[i].second;
            }
        }
        cout << res << endl;
    }

    return 0;
}