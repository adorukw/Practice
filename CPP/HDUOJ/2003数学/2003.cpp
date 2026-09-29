#include <iostream>
#include <iomanip>

using namespace std;

int main(){
    double n;
    while(cin>>n){
        if(n<0) n=-n;
        cout<<fixed<<setprecision(2)<<n<<'\n';
    }

    return 0;
}