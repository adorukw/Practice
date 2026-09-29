#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<int> Mul(const vector<int>& a, int b) {
    vector<int> c;
    int t = 0;
    for (int i = 0; i < a.size(); i++) {
        t += a[i] * b;
        c.push_back(t % 10);
        t /= 10;
    }
    while (t) {
        c.push_back(t % 10);
        t /= 10;
    }

    return c;
}

vector<int> Div(const vector<int>& a, int b) {
    vector<int> c;
    int r = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        r = r * 10 + a[i];
        c.push_back(r / b);
        r %= b;
    }
    reverse(c.begin(), c.end());
    while (c.size() > 1 && c.back() == 0) c.pop_back();

    return c;
}

int main() {
    vector<int> cat[101];
    cat[0] = {1};
    for (int i = 1; i <= 100; i++) {
        vector<int> temp = Mul(cat[i - 1], 4 * i - 2);
        cat[i] = Div(temp, i + 1);
    }

    int n;
    while (cin >> n) {
        vector<int> res = cat[n];
        for (int i = res.size() - 1; i >= 0; i--) {
            cout << res[i];
        }
        cout << '\n';
    }

    return 0;
}