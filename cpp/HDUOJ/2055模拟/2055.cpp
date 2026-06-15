#include <iostream>

using namespace std;

int main() {
    int T;
    cin >> T;
    while (T--) {
        char x;
        int y;
        cin >> x >> y;
        int z = 0;
        if (x < 'a')
            z = x - 'A' + 1;
        else
            z = -(x - 'a' + 1);
        cout << y + z << endl;
    }

    return 0;
}