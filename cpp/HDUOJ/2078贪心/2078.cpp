#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        sort(a.rbegin(), a.rend());
        int maxDiff = 0;
        for (int start = 0; start < n; start++) {
            int curr = (100 - a[start]) * (100 - a[start]);
            int count = 1;
            for (int i = start + 1; i < n && count <= m; i++) {
                curr += (a[i - 1] - a[i]) * (a[i - 1] - a[i]);
                count++;
            }
            if (curr > maxDiff) {
                maxDiff = curr;
            }
        }
        cout << maxDiff << endl;
    }
    return 0;
}