#include <iostream>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;
    while (N--) {
        string phoneNum;
        cin >> phoneNum;
        string shortNum = "6";
        for (int i = 0; i < 5; i++) {
            shortNum += phoneNum[6 + i];
        }
        cout << shortNum << endl;
    }
    return 0;
}