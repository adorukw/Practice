#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    string O1, O2;
    while (cin >> n >> O1 >> O2) {
        vector<string> seq;
        string st;
        int inPtr = 0, outPtr = 0;
        while (inPtr < n) {
            st.push_back(O1[inPtr]);
            seq.push_back("in");
            inPtr++;
            while (!st.empty() && st.back() == O2[outPtr]) {
                st.pop_back();
                seq.push_back("out");
                outPtr++;
            }
        }
        while (!st.empty() && st.back() == O2[outPtr]) {
            st.pop_back();
            seq.push_back("out");
            outPtr++;
        }
        if (outPtr != n)
            cout << "No.\n";
        else {
            cout << "Yes.\n";
            for (int i = 0; i < seq.size(); i++) {
                cout << seq[i] << '\n';
            }
        }
        cout << "FINISH";
    }

    return 0;
}