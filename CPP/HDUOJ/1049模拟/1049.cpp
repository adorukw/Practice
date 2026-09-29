#include <iostream>

using namespace std;

int main(){
    int n,u,d;
    while(cin>>n>>u>>d){
        if(n==0&&u==0&&d==0) break;

        int time=0,height=0;
        while(true){
            height+=u;
            time++;
            if(height>=n) break;
            height-=d;
            time++;
        }

        cout<<time<<'\n';
    }
}