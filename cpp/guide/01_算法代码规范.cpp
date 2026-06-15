#include <bits/stdc++.h>
using namespace std;

// 类型别名
typedef long long ll;
typedef pair<int, int> pii;

// 常量定义
const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3f;
const int MOD = 1e9 + 7;
const int MAXN = 1e5 + 5;

// 全局变量
int n, m;
int arr[MAXN];
vector<int> graph[MAXN];
int dist[MAXN];
bool visited[MAXN];

// 函数声明
void bfs(int start);
void dfs(int u);

int main() {
    // 输入输出优化
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m;

    // 初始化
    memset(dist, INF, sizeof(dist));
    memset(visited, false, sizeof(visited));

    // 读入数据
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // 算法逻辑...

    return 0;
}

void bfs(int start) {
    // 局部变量
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}