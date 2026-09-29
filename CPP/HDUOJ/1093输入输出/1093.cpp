#include <iostream>

using namespace std;

int main(){
    int N;
    while(cin>>N){
        for(int i=0;i<N;i++){
            int M;
            cin>>M;
            int res=0,num;
            for(int j=0;j<M;j++){
                cin>>num;
                res+=num;
            }
            cout<<res<<endl;
        }
    }
    return 0;
}