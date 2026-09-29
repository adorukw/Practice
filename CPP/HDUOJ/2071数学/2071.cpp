#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        double maxNum;
        cin >> maxNum;
        for (int i = 1; i < n; i++) {
            double num;
            cin >> num;
            if (maxNum < num) maxNum = num;
        }
        cout << fixed << setprecision(2) << maxNum << endl;
    }
    return 0;
}