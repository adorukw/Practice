#include <iostream>

using namespace std;

int main(){
    int n;
    while(cin>>n){
        int count=1;
        for(int i=n-1;i>0;i--){
            count+=1;
            count*=2;
        }
        cout<<count<<'\n';
    }

    return 0;
}