#include <iostream>

using namespace std;

bool IsVowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

bool ContainsVowel(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (IsVowel(s[i])) return true;
    }

    return false;
}

bool ContainsContinuous(string s) {
    for (int i = 0; i < s.length() - 1; i++) {
        if (s[i] == s[i + 1] && s[i] != 'e' && s[i] != 'o') return true;
    }

    return false;
}

bool ContainsContinuousVowel(string s) {
    if (s.length() < 3) return false;
    for (int i = 0; i < s.length() - 2; i++) {
        if (IsVowel(s[i]) && IsVowel(s[i + 1]) && IsVowel(s[i + 2]))
            return true;
    }

    return false;
}

bool ContainsContinuousConsonant(string s) {
    if (s.length() < 3) return false;
    for (int i = 0; i < s.length() - 2; i++) {
        if (!IsVowel(s[i]) && !IsVowel(s[i + 1]) && !IsVowel(s[i + 2]))
            return true;
    }

    return false;
}

int main() {
    string passWord;

    while (cin >> passWord && passWord != "end") {
        if (ContainsVowel(passWord) && !ContainsContinuous(passWord) &&
            !ContainsContinuousVowel(passWord) &&
            !ContainsContinuousConsonant(passWord)) {
            cout << '<' << passWord << '>' << " is acceptable." << '\n';
        }
        else
            cout << '<' << passWord << "> is not acceptable." << '\n';
    }

    return 0;
}