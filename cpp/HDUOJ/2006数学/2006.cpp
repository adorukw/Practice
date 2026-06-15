#include <iostream>

using namespace std;

int main(){
    int n;
    while(cin>>n){
        int product=1;
        for(int i=0;i<n;i++){
            int x;
            cin>>x;
            if(x%2==1) product*=x;
        }
        cout<<product<<'\n';
    }

    return 0;
}