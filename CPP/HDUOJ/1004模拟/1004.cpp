#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N;
    while (std::cin >> N) {
        if (N == 0) break;
        std::string color;
        std::unordered_map<std::string, int> colorCount;
        for (int i = 0; i < N; i++) {
            std::cin >> color;
            colorCount[color]++;
        }
        int maxCount = 0;
        std::string maxColor;
        for (auto it = colorCount.begin(); it != colorCount.end(); it++) {
            if (it->second > maxCount) {
                maxCount = it->second;
                maxColor = it->first;
            }
        }
        std::cout << maxColor + '\n';
    }
}