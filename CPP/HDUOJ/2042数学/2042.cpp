#include <iostream>

using namespace std;

int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; i++) {
        int a;
        cin >> a;
        int count = 3;
        for (int j = 0; j < a; j++) {
            count = (count - 1) * 2;
        }
        cout << count << endl;
    }

    return 0;
}