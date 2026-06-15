#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main(){
    int n;
    while(cin>>n){
        vector<int> scores(n);
        for(int i=0;i<n;i++){
            cin>>scores[i];
        }
        sort(scores.begin(),scores.end());
        scores.erase(scores.begin());
        scores.pop_back();
        int sum=0;
        for(int i=0;i<scores.size();i++){
            sum+=scores[i];
        }
        double avg=sum*1.0/scores.size();
        cout<<fixed<<setprecision(2)<<avg<<'\n';
    }

    return 0;
}