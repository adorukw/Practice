#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main(){
    double x1,y1,x2,y2;
    while(cin>>x1>>y1>>x2>>y2){
        double dist=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
        cout<<fixed<<setprecision(2)<<dist<<'\n';
    }

    return 0;
}