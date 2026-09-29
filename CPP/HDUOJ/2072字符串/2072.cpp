#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        if(line=="#") return 0;
        stringstream ss(line);
        string word;
        unordered_set<string> words;
        while(ss>>word){
            words.insert(word);
        }

        cout<<words.size()<<endl;
    }
    return 0;
}