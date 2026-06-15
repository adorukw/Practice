#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        getchar();
        for (int i = 0; i < n; i++) {
            int vowel[5];
            for (int j = 0; j < 5; j++) {
                vowel[j] = 0;
            }

            string s;
            getline(cin, s);
            for (const char& c : s) {
                if (c == 'a' || c == 'A')
                    vowel[0]++;
                else if (c == 'e' || c == 'E')
                    vowel[1]++;
                else if (c == 'i' || c == 'I')
                    vowel[2]++;
                else if (c == 'o' || c == 'O')
                    vowel[3]++;
                else if (c == 'u' || c == 'U')
                    vowel[4]++;
            }
            cout << "a:" << vowel[0] << '\n';
            cout << "e:" << vowel[1] << '\n';
            cout << "i:" << vowel[2] << '\n';
            cout << "o:" << vowel[3] << '\n';
            cout << "u:" << vowel[4] << '\n';
            if (i != n - 1) cout << '\n';
        }
    }

    return 0;
}