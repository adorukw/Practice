#include <iostream>

using namespace std;

int main(){
    double n;
    while(cin>>n&&n!=0){
        int positve=0,negative=0,zero=0;
        for(int i=0;i<n;i++){
            double num;
            cin>>num;
            if(num>0) positve++;
            else if(num<0) negative++;
            else zero++;    
        }
        cout<<negative<<" "<<zero<<" "<<positve<<'\n';
    }

    return 0;
}