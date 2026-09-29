#include <iostream>

using namespace std;

int main(){
    int N;
    while(cin>>N){
        for(int i=0;i<N;i++){
            int a,b;
            cin>>a>>b;
            cout<<a+b<<'\n';
        }
    }

    return 0;
}