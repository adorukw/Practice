#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        string binary = "";
        while (n) {
            int reminder = n % 2;
            binary += to_string(reminder);
            n /= 2;
        }
        reverse(binary.begin(), binary.end());
        cout << binary << endl;
    }

    return 0;
}