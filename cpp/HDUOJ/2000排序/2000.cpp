#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main(){
    string s;
    while(cin>>s){
        vector<char> v(s.begin(),s.end());
        sort(v.begin(),v.end());
        cout<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';
    }

    return 0;
}