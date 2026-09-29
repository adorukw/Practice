#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};
const int MAX_AMOUNT = 105;
const int INF = 0x3f3f3f3f;

struct PreNode {
    int x, y;
    PreNode() : x(-1), y(-1) {}
    PreNode(int x_, int y_) : x(x_), y(y_) {}
};

typedef pair<int, pair<int, int>> PII;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    while (cin >> N >> M) {
        vector<string> grid(N);
        for (int i = 0; i < N; i++) {
            cin >> grid[i];
        }

        int dist[MAX_AMOUNT][MAX_AMOUNT];
        PreNode pre[MAX_AMOUNT][MAX_AMOUNT];
        memset(dist, 0x3f, sizeof(dist));
        dist[0][0] = 0;
        pre[0][0] = PreNode(-1, -1);
        priority_queue<PII, vector<PII>, greater<PII>> pq;
        pq.push({0, {0, 0}});
        while (!pq.empty()) {
            auto curr = pq.top();
            pq.pop();
            int currTime = curr.first;
            int x = curr.second.first;
            int y = curr.second.second;

            if (currTime > dist[x][y]) continue;

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
                if (grid[nx][ny] == 'X') continue;

                int cost = 0;
                if (grid[nx][ny] == '.')
                    cost = 1;
                else
                    cost = 1 + (grid[nx][ny] - '0');

                int newTime = currTime + cost;
                if (newTime < dist[nx][ny]) {
                    dist[nx][ny] = newTime;
                    pre[nx][ny] = PreNode(x, y);
                    pq.push({newTime, {nx, ny}});
                }
            }
        }

        int endX = N - 1, endY = M - 1;
        if (dist[endX][endY] == INF) {
            cout << "God please help our poor hero." << endl;
            cout << "FINISH" << endl;
            continue;
        }

        vector<pair<int, int>> path;
        int x = endX, y = endY;
        while (x != -1 && y != -1) {
            path.emplace_back(x, y);
            auto p = pre[x][y];
            x = p.x;
            y = p.y;
        }
        reverse(path.begin(), path.end());

        vector<string> timeLog;
        int currSecond = 1;
        int len = path.size();
        for (int i = 0; i < len - 1; i++) {
            int x1 = path[i].first, y1 = path[i].second;
            int x2 = path[i + 1].first, y2 = path[i + 1].second;

            timeLog.push_back(to_string(currSecond) + "s:(" + to_string(x1) +
                              "," + to_string(y1) + ")->(" + to_string(x2) +
                              "," + to_string(y2) + ")");
            currSecond++;
            if (grid[x2][y2] >= '1' && grid[x2][y2] <= '9') {
                int n = grid[x2][y2] - '0';
                for (int k = 0; k < n; k++) {
                    timeLog.push_back(to_string(currSecond) + "s:FIGHT AT (" +
                                      to_string(x2) + "," + to_string(y2) +
                                      ")");
                    currSecond++;
                }
            }
        }
        cout
            << "It takes " << dist[endX][endY]
            << " seconds to reach the target position, let me show you the way."
            << endl;
        for (auto& log : timeLog) {
            cout << log << '\n';
        }
        cout << "FINISH" << '\n';
    }

    return 0;
}