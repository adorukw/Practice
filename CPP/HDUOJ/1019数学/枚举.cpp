#include <climits>
#include <iostream>
#include <vector>

using namespace std;

bool IsLcm(long long lcm, vector<int>& nums) {
    for (int i = 0; i < nums.size(); i++) {
        if (lcm % nums[i] != 0) return false;
    }

    return true;
}

int main() {
    int caseNum;
    cin >> caseNum;
    for (int i = 0; i < caseNum; i++) {
        int n;
        cin >> n;
        vector<int> nums(n);
        int maxNum = INT_MIN;
        for (int j = 0; j < n; j++) {
            cin >> nums[j];
            if (nums[j] > maxNum) maxNum = nums[j];
        }
        long long lcm = maxNum;
        while (!IsLcm(lcm, nums)) lcm += maxNum;
        cout << lcm << '\n';
    }

    return 0;
}