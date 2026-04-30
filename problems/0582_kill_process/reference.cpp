// ============================================================
// 582. Kill Process
// ============================================================
// 难度: Medium | 标签: Tree, DFS, BFS, Array, Hash Table
//
// 题意: 给定进程ID数组pid和父进程ID数组ppid，
//       杀掉指定进程kill及其所有子孙进程，返回被杀进程列表
// ============================================================

// ============================================================
// 解法一：BFS（推荐）
// ============================================================
// 思路：
//   1. 建立 parent -> children 的邻接表
//   2. 从 kill 节点开始 BFS，收集所有可达节点
// 时间: O(n) 建图 + O(n) 遍历
// 空间: O(n) 邻接表 + O(n) 队列
class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        // Step 1: 建立邻接表 (parent -> children)
        unordered_map<int, vector<int>> children;
        for (int i = 0; i < (int)pid.size(); ++i) {
            children[ppid[i]].push_back(pid[i]);
        }

        // Step 2: BFS 从 kill 开始遍历
        vector<int> result;
        queue<int> q;
        q.push(kill);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            result.push_back(curr);
            // 遍历所有子进程
            if (children.count(curr)) {
                for (int child : children[curr]) {
                    q.push(child);
                }
            }
        }
        return result;
    }
};

// ============================================================
// 解法二：DFS 递归
// ============================================================
// 思路：
//   1. 建邻接表同上
//   2. 递归遍历 kill 节点的整棵子树
// 时间: O(n) | 空间: O(n) 递归栈 + 邻接表
// 注意: 树很深时可能栈溢出（n <= 5*10^4 安全）
class Solution2 {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        unordered_map<int, vector<int>> children;
        for (int i = 0; i < (int)pid.size(); ++i) {
            children[ppid[i]].push_back(pid[i]);
        }

        vector<int> result;
        dfs(children, kill, result);
        return result;
    }

private:
    void dfs(unordered_map<int, vector<int>>& children,
             int node, vector<int>& result) {
        result.push_back(node);
        if (children.count(node)) {
            for (int child : children[node]) {
                dfs(children, child, result);
            }
        }
    }
};

// ============================================================
// 解法三：DFS 迭代（栈模拟）
// ============================================================
// 思路：
//   与 BFS 几乎一样，只是用 stack 替换 queue
//   结果顺序为 DFS 序（先深后广）
// 时间: O(n) | 空间: O(n)
class Solution3 {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        unordered_map<int, vector<int>> children;
        for (int i = 0; i < (int)pid.size(); ++i) {
            children[ppid[i]].push_back(pid[i]);
        }

        vector<int> result;
        stack<int> stk;
        stk.push(kill);

        while (!stk.empty()) {
            int curr = stk.top();
            stk.pop();
            result.push_back(curr);
            if (children.count(curr)) {
                for (int child : children[curr]) {
                    stk.push(child);
                }
            }
        }
        return result;
    }
};
