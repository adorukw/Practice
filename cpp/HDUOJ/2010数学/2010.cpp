#include <iostream>

using namespace std;

int main() {
    int m, n;
    while (cin >> m >> n) {
        bool first = true;
        bool isNarcissistic = false;
        
        for (int i = m; i <= n; i++) { 
            int a = i / 100;  
            int b = i / 10 % 10; 
            int c = i % 10;     
            
            int sum = a*a*a + b*b*b + c*c*c;
            
            if (sum == i) {
                isNarcissistic = true;
                
                if (!first) {
                    cout << " ";  
                }
                cout << i;
                first = false;
            }
        }
        
        if (!isNarcissistic) {
            cout << "no";
        }
        cout << endl;
    }
    
    return 0;
}