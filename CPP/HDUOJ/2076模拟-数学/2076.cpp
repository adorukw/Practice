#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

int main() {
    int T;
    cin >> T;
    while (T--) {
        int hour, minute, second;
        cin >> hour >> minute >> second;
        hour = hour % 12;
        double minuteDegree = 360 / 60 * minute + 6.0 / 60 * second;
        double hourDegree =
            360 / 12 * hour + 30.0 / 60 * minute + 0.5 / 60 * second;
        double hourAndMinuteDgree = min(abs(hourDegree - minuteDegree),
                                        360 - abs(hourDegree - minuteDegree));
        cout << (int)hourAndMinuteDgree << endl;
    }
    return 0;
}