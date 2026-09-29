#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int CharToNum(char c) { return c - 'A' + 1; }

int main() {
    int target;
    string letters;
    while (cin >> target >> letters) {
        if (target == 0 && letters == "END") break;
        sort(letters.begin(), letters.end(), greater<char>());
        bool found = false;
        int n = letters.size();
        for (int v = 0; v < n && !found; v++) {
            for (int w = 0; w < n && !found; w++) {
                if (v == w) continue;
                for (int x = 0; x < n && !found; x++) {
                    if (x == v || x == w) continue;
                    for (int y = 0; y < n && !found; y++) {
                        if (y == v || y == w || y == x) continue;
                        for (int z = 0; z < n && !found; z++) {
                            if (z == v || z == w || z == x || z == y) continue;
                            long long vNum = CharToNum(letters[v]);
                            long long wNum = CharToNum(letters[w]);
                            long long xNum = CharToNum(letters[x]);
                            long long yNum = CharToNum(letters[y]);
                            long long zNum = CharToNum(letters[z]);

                            long long res = vNum - wNum * wNum +
                                            xNum * xNum * xNum -
                                            yNum * yNum * yNum * yNum +
                                            zNum * zNum * zNum * zNum * zNum;

                            if (res == target) {
                                cout << letters[v] << letters[w] << letters[x]
                                     << letters[y] << letters[z] << endl;
                                found = true;
                            }
                        }
                    }
                }
            }
        }
        if (!found) cout << "no solution" << endl;
    }
    return 0;
}