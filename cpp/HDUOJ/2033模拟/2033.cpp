#include <iostream>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        for (int i = 0; i < n; i++) {
            int AH, AM, AS, BH, BM, BS;
            cin >> AH >> AM >> AS >> BH >> BM >> BS;
            int a = AH * 3600 + AM * 60 + AS;
            int b = BH * 3600 + BM * 60 + BS;
            int t = a + b;
            int h = t / 3600;
            int m = (t % 3600) / 60;
            int s = t % 60;
            cout << h << ' ' << m << ' ' << s << endl;
        }
    }

    return 0;
}