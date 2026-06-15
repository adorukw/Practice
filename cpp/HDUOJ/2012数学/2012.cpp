#include <iostream>
using namespace std;

bool IsPrime(int n){
    if(n<=1) return false;
    if(n==2) return true;
    if(n%2==0) return false;
    for(int i=3;i*i<n;i++){
        if(n%i==0) return false;
    }
    return true;
}

int main(){
    int x,y;
    while(cin>>x>>y){
        if(x==0&&y==0) break;
        bool flag=true;

        for(int i=x;i<=y;i++){
            int n=i*i+i+41;
            if(!IsPrime(n)){
                flag=false;
                break;
            }
        }
        if(flag) cout<<"OK"<<endl;
        else cout<<"Sorry"<<endl;
    }

    return 0;
}