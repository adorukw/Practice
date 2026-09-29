#include <iostream>
#include <vector>

using namespace std;

int main(){
    int n;
    while(cin>>n){
        if(n==0) break;
        vector<int> list(n);
        for(int i=0;i<n;i++){
            cin>>list[i];
        }

        for(int i=0;i<n-1;i++){
            for(int j=0;j<n-i-1;j++){
                if(abs(list[j])<abs(list[j+1])){
                    int temp=list[j];
                    list[j]=list[j+1];
                    list[j+1]=temp;
                }
            }
        }

        for(int i=0;i<n;i++){
            cout<<list[i]<<" ";
        }
        cout<<'\n';
    }

    return 0;
}