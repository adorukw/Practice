#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool IsIncluded(int a, vector<int> B) {
    for (int i = 0; i < B.size(); i++) {
        if (a == B[i]) return true;
    }

    return false;
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        if (n == 0 && m == 0) break;
        vector<int> A(n), B(m);
        for (int i = 0; i < n; i++) cin >> A[i];
        for (int i = 0; i < m; i++) cin >> B[i];
        vector<int> C;
        for (int i = 0; i < n; i++) {
            if (!IsIncluded(A[i], B)) C.push_back(A[i]);
        }

        if (C.size() == 0) {
            cout << "NULL" << endl;
            continue;
        }

        sort(C.begin(), C.end());
        for (int i = 0; i < C.size(); i++) {
            cout << C[i] << " ";
        }
    }

    return 0;
}