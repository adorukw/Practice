#include <iostream>
#include <vector>

using namespace std;

int main() {
    int carHeight = 168;
    int height;
    while (cin >> height) {
        if (height <= carHeight) {
            cout << "CRASH " << height << '\n';
            return 0;
        }
    }
    cout << "NO CRASH" << '\n';
}