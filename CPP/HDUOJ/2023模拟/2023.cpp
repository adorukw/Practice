#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, m;
    while (cin >> n >> m) {
        vector<vector<int>> totalScores(n, vector<int>(m));
        vector<double> meanScorePerson(n, 0);
        vector<double> meanScoreCourse(m, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                int score;
                cin >> score;
                totalScores[i][j] = score;
                meanScorePerson[i] += score;
                meanScoreCourse[j] += score;
            }
            meanScorePerson[i] /= m;
            if (i != 0) cout << ' ';
            cout << fixed << setprecision(2) << meanScorePerson[i];
        }
        cout << '\n';
        for (int i = 0; i < m; i++) {
            meanScoreCourse[i] /= n;
            if (i != 0) cout << ' ';
            cout << fixed << setprecision(2) << meanScoreCourse[i];
        }
        cout << '\n';

        int count = 0;
        for (int i = 0; i < n; i++) {
            bool isGreater = true;
            for (int j = 0; j < m; j++) {
                if (totalScores[i][j] < meanScoreCourse[j]) isGreater = false;
            }
            if (isGreater) count++;
        }
        cout << count << '\n' << '\n';
    }

    return 0;
}