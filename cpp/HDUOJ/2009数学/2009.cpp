#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main(){
    int n,m;
    while(cin>>n>>m){
        double sum=0,x=n;
        for(int i=0;i<m;i++){
            sum+=x;
            x=sqrt(x);
        }
        cout<<fixed<<setprecision(2)<<sum<<'\n';
    }

    return 0;
}