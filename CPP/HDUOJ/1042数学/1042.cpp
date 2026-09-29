#include <iostream>
#include <vector>

using namespace std;

vector<int> Mul(const vector<int>& a, const vector<int>& b) {
    int lenA = a.size(), lenB = b.size();
    vector<int> res(lenA + lenB, 0);
    
    for(int i = 0; i < lenA; i++) {
        for(int j = 0; j < lenB; j++) {
            res[i + j] += a[i] * b[j];
        }
    }

    int carry = 0;
    for(int i = 0; i < res.size(); i++) {
        int sum = res[i] + carry;  
        res[i] = sum % 10;         
        carry = sum / 10;
    }

    while(res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }

    return res;
}

vector<int> IntToBigInt(int n) {
    if (n == 0) return {0};
    
    vector<int> result;
    while (n > 0) {
        result.push_back(n % 10);
        n /= 10;
    }
    return result;
}

int main() {
    int n;
    
    while(cin >> n) {
        if(n < 0) continue;
        vector<int> result = {1};
        
        for(int i = 2; i <= n; i++) {
            vector<int> current = IntToBigInt(i);
            result = Mul(result, current);
        }
        
        for(int i = result.size() - 1; i >= 0; i--) {
            cout << result[i];
        }
        cout << endl;
    }
    
    return 0;
}