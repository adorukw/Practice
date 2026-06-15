#include <iostream>

using namespace std;

int main(){
    int y,m,d;
    char c;
    int mday[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    while(cin>>y>>c>>m>>c>>d){
        int sum=0;
        for(int i=1;i<m;i++){
            sum+=mday[i];
        }
        sum+=d;
        if((y%4==0&&y%100!=0)||(y%400==0)) if(m>2) sum++;
        cout<<sum<<'\n';
    }

    return 0;
}