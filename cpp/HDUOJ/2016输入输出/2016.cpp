#include <climits>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        if (n == 0) break;
        vector<int> nums(n);
        int min = INT_MAX;
        for (int i = 0; i < n; i++) {
            cin >> nums[i];
            if (nums[i] < min) min = nums[i];
        }
        int temp=nums[0];
        nums[0]=min;
        for(int i=1;i<n;i++){
            if(nums[i]==min){
                nums[i]=temp;
                break;
            }
        }
        for (int i = 0; i < n; i++) {
            cout << nums[i] << " ";
        }
        cout<<'\n';
    }

    return 0;
}