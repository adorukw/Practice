#include <iostream>
#include <vector>

using namespace std;

int main(){
    int n;
    int dp[56];
    dp[1]=1,dp[2]=2,dp[3]=3;
    for(int i=4;i<55;i++){
        dp[i]=dp[i-1]+dp[i-3];
    }
    while(cin>>n){
        if(n==0) break;
        cout<<dp[n]<<'\n';
    }

    return 0;
}