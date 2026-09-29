#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class MazeSolver {
private:
    int N, M, T;
    vector<vector<char>> maze;
    vector<vector<bool>> visited;
    int sx, sy, dx, dy;
    bool found;
    
    // 奇偶性剪枝
    bool parityCut() {
        int minSteps = abs(sx - dx) + abs(sy - dy);
        if (minSteps > T) return false;
        if ((T - minSteps) % 2 != 0) return false;
        return true;
    }
    
    void DFS(int x, int y, int steps) {
        if (found || steps > T) return;
        
        if (x == dx && y == dy && steps == T) {
            found = true;
            return;
        }
        
        int minRemain = abs(x - dx) + abs(y - dy);
        if (steps + minRemain > T) return;
        
        int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dirs[i][0];
            int ny = y + dirs[i][1];
            
            if (nx >= 0 && nx < N && ny >= 0 && ny < M && 
                !visited[nx][ny] && maze[nx][ny] != 'X') {
                visited[nx][ny] = true;
                DFS(nx, ny, steps + 1);
                visited[nx][ny] = false;
                if (found) return;
            }
        }
    }
    
public:
    MazeSolver(int n, int m, int t) : N(n), M(m), T(t) {
        maze.assign(N, vector<char>(M));
        visited.assign(N, vector<bool>(M, false));
        found = false;
    }
    
    void readMaze() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cin >> maze[i][j];
                if (maze[i][j] == 'S') {
                    sx = i; sy = j;
                } else if (maze[i][j] == 'D') {
                    dx = i; dy = j;
                }
            }
        }
    }
    
    bool canSurvive() {
        if (!parityCut()) return false;
        visited[sx][sy] = true;
        DFS(sx, sy, 0);
        return found;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, T;
    while (cin >> N >> M >> T) {
        if (N == 0 && M == 0 && T == 0) break;
        
        MazeSolver solver(N, M, T);
        solver.readMaze();
        
        if (solver.canSurvive()) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}


#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int N, M, T;
vector<vector<char>> maze;
vector<vector<bool>> visited;
int sx, sy, dx, dy;  // 起点和终点坐标
bool found;  // 是否找到解

// 奇偶性剪枝：从(sx,sy)到(dx,dy)的最短步数是曼哈顿距离
// 如果(T - 最短距离)是奇数，则不可能在T步到达
bool parityCut() {
    int minSteps = abs(sx - dx) + abs(sy - dy);
    if (minSteps > T) return false;  // 最短距离都超过T
    if ((T - minSteps) % 2 != 0) return false;  // 奇偶性不匹配
    return true;
}

void DFS(int x, int y, int steps) {
    // 剪枝：如果已经找到解，或者步数超过T，或者当前位置无效
    if (found || steps > T) return;
    
    // 如果当前位置是门且在恰好第T步
    if (x == dx && y == dy && steps == T) {
        found = true;
        return;
    }
    
    // 剪枝：从当前位置到终点的最短距离
    int minRemain = abs(x - dx) + abs(y - dy);
    if (steps + minRemain > T) return;  // 即使走最短路径也会超过T
    
    // 四个方向
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dirs[i][0];
        int ny = y + dirs[i][1];
        
        if (nx >= 0 && nx < N && ny >= 0 && ny < M && 
            !visited[nx][ny] && maze[nx][ny] != 'X') {
            visited[nx][ny] = true;
            DFS(nx, ny, steps + 1);
            visited[nx][ny] = false;  // 回溯
            if (found) return;  // 如果找到解，提前结束
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    while (cin >> N >> M >> T) {
        if (N == 0 && M == 0 && T == 0) break;
        
        maze.assign(N, vector<char>(M));
        visited.assign(N, vector<bool>(M, false));
        found = false;
        
        // 读取迷宫并记录起点和终点
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cin >> maze[i][j];
                if (maze[i][j] == 'S') {
                    sx = i; sy = j;
                } else if (maze[i][j] == 'D') {
                    dx = i; dy = j;
                }
            }
        }
        
        // 先进行奇偶性剪枝
        if (!parityCut()) {
            cout << "NO\n";
            continue;
        }
        
        visited[sx][sy] = true;
        DFS(sx, sy, 0);
        
        if (found) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}