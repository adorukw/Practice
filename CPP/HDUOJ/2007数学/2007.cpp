#include <iostream>

using namespace std;

int main(){
    int m,n;
    while(cin>>m>>n){
        int x=0,y=0;
        if(m>n) swap(m,n);
        for(int i=m;i<=n;i++){
            if(i%2==0) x+=i*i;
            else y+=i*i*i;
        }
        cout<<x<<" "<<y<<'\n';
    }

    return 0;
}