#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int MAX_AMOUNT = 1005;
const int INF = INT_MAX / 2;

struct Edge {
    int to;
    int time;
    Edge(int t, int w) : to(t), time(w) {}
};

vector<Edge> graph[MAX_AMOUNT];
int dist[MAX_AMOUNT];
bool visited[MAX_AMOUNT];

void Dijkstra(vector<int>& starts) {
    fill(dist, dist + MAX_AMOUNT, INF);
    memset(visited, false, sizeof(visited));

    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    for (int s : starts) {
        dist[s] = 0;
        pq.push({0, s});
    }

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (const Edge& e : graph[u]) {
            int v = e.to;
            int w = e.time;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    int T, S, D;
    while (cin >> T >> S >> D) {
        for (int i = 0; i < MAX_AMOUNT; i++) {
            graph[i].clear();
        }

        for (int i = 0; i < T; i++) {
            int a, b, time;
            cin >> a >> b >> time;
            graph[a].push_back(Edge(b, time));
            graph[b].push_back(Edge(a, time));
        }

        vector<int> starts(S);
        for (int i = 0; i < S; i++) {
            cin >> starts[i];
        }

        vector<int> dests(D);
        for (int i = 0; i < D; i++) {
            cin >> dests[i];
        }
        Dijkstra(starts);

        int res = INF;
        for (int d : dests) {
            res = min(res, dist[d]);
        }

        cout << res << endl;
    }
    return 0;
}