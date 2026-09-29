#include <iostream>     // 输入输出流库
#include <vector>       // 动态数组库，用于邻接表
#include <cstring>      // C风格字符串操作库，包含memset函数
using namespace std;    // 使用标准命名空间

const int MAX_AMOUNT = 505;   // 定义最大节点数，题目说明M,N≤500

// 邻接表存储图
// G[i]表示女生i愿意配对的男生列表
// 例如：G[1] = {1,2,3} 表示女生1愿意和男生1、2、3配对
vector<int> G[MAX_AMOUNT];

// match数组：记录匹配关系
// match[j]表示男生j当前匹配的女生编号，0表示未匹配
// 例如：match[1]=2 表示男生1当前与女生2配对
int match[MAX_AMOUNT];

// used数组：DFS过程中标记男生是否被访问过
// 防止重复访问同一个男生
bool used[MAX_AMOUNT];

// DFS函数：为女生u寻找增广路
// 返回true：找到了增广路，成功匹配
// 返回false：没有找到增广路
bool dfs(int u) {           // u是当前尝试匹配的女生编号
    // 遍历女生u所有愿意配对的男生
    for (int v : G[u]) {    // 范围for循环，v是男生编号
        if (!used[v]) {     // 如果男生v在本次DFS中还没有被考虑过
            used[v] = true; // 标记男生v已被本次DFS访问
            
            // 核心逻辑：尝试为男生v匹配
            // 情况1：男生v没有匹配，直接匹配
            // 情况2：男生v已有匹配(match[v]!=0)，尝试为原配女生找新匹配
            if (match[v] == 0 || dfs(match[v])) {
                // 如果进入这里，说明：
                // 1. match[v]==0，男生v单身，直接匹配
                // 2. dfs(match[v])==true，成功为原配女生找到了新对象
                match[v] = u;  // 更新匹配关系：男生v匹配女生u
                return true;   // 匹配成功
            }
        }
    }
    return false;  // 所有可能都尝试过了，无法匹配
}

int main() {
    int K, M, N;  // K:可能的组合数, M:女生人数, N:男生人数
    
    // 多组测试数据，输入0结束
    while (cin >> K && K != 0) {  // 先读取K，如果K不等于0继续
        cin >> M >> N;  // 读取女生人数M和男生人数N
        
        // --- 初始化部分 ---
        // 清空所有女生的邻接表
        for (int i = 1; i <= M; i++) {
            G[i].clear();  // 清空vector，准备存储新的边
        }
        // 重置所有男生的匹配状态为0（未匹配）
        memset(match, 0, sizeof(match));
        
        // --- 读入边（配对意愿）---
        for (int i = 0; i < K; i++) {  // 循环K次，读取K条边
            int girl, boy;  // 女生编号，男生编号
            cin >> girl >> boy;  // 读取一条配对意愿
            G[girl].push_back(boy);  // 在女生girl的列表中添加入男生boy
        }
        
        int result = 0;  // 最大匹配数，初始为0
        
        // --- 匈牙利算法主循环 ---
        // 为每个女生尝试寻找匹配
        for (int i = 1; i <= M; i++) {  // 遍历所有女生
            // 每次为新的女生寻找匹配时，清空used数组
            // 因为used记录的是本次DFS中是否考虑过某个男生
            memset(used, 0, sizeof(used));
            
            // 为女生i尝试匹配
            if (dfs(i)) {  // 如果dfs返回true，匹配成功
                result++;  // 匹配数加1
            }
        }
        
        // 输出结果
        cout << result << endl;
    }
    
    return 0;  // 程序正常结束
}