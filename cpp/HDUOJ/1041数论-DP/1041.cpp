#include <iostream>

using namespace std;

const int MAX_DIGITS=305;

int main(){
    int n;
    while(cin>>n){
        int prev1[MAX_DIGITS]={0};
        int prev2[MAX_DIGITS]={0};

        prev1[0]=1;
        prev2[0]=0;

        if(n==1||n==2){
            cout<<n-1<<'\n';
            continue;
        }

        for(int i=3;i<=n;i++){
            for(int j=0;j<MAX_DIGITS;j++){
                prev2[j]*=2;
                int temp=prev2[j]+prev1[j];
                prev2[j]=prev1[j];
                prev1[j]=temp;
            }

            for(int j=0;j<MAX_DIGITS;j++){
                if(prev1[j]>9){
                    prev1[j+1]+=prev1[j]/10;
                    prev1[j]%=10;
                }
            }
        }

        for(int i=MAX_DIGITS-1;i>=0;i--){
            if(prev1[i]!=0){
                for(int j=i;j>=0;j--){
                    cout<<prev1[j];
                }
                cout<<'\n';
                break;
            }
        }
    }

    return 0;
}