// ============================================================
// LeetCode 797: All Paths From Source to Target (所有可能的路径)
//
// 【解法1】DFS/回溯 — O(2^n * n) / O(n)
//
// DAG 中从 0 到 n-1 找所有路径
// 回溯三步: push → dfs → pop
// DAG 无环, 不需要 visited
// ============================================================

/*
 * DFS 路径枚举示意 (graph = [[1,2],[3],[3],[]])：
 *
 * 图结构:
 *   0 ──→ 1 ──→ 3
 *   |           ↑
 *   └──→ 2 ────┘
 *
 * DFS 过程 (path 变化):
 *
 * dfs(0)  path=[0]
 *  ├─ dfs(1)  path=[0,1]
 *  │   └─ dfs(3)  path=[0,1,3] → node==n-1, 收集！
 *  │      回溯 → path=[0,1]
 *  │   回溯 → path=[0]
 *  └─ dfs(2)  path=[0,2]
 *      └─ dfs(3)  path=[0,2,3] → node==n-1, 收集！
 *         回溯 → path=[0,2]
 *      回溯 → path=[0]
 *
 * 结果: [[0,1,3], [0,2,3]]
 */

class Solution {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> result;
        vector<int> path = {0};  // 起点 0 先放入路径
        dfs(graph, 0, path, result);
        return result;
    }

private:
    void dfs(vector<vector<int>>& graph, int node, vector<int>& path,
             vector<vector<int>>& result) {
        // 终止条件: 到达终点 n-1
        if (node == (int)graph.size() - 1) {
            result.push_back(path);  // 收集路径 (拷贝)
            return;
        }

        // 遍历当前节点的所有邻居
        for (int next : graph[node]) {
            path.push_back(next);           // 选择: 加入路径
            dfs(graph, next, path, result); // 递归: 继续探索
            path.pop_back();                // 撤销: 回溯
        }
    }
};


// ============================================================
// 【解法2】BFS — O(2^n * n) / O(2^n * n)
//
// 队列中存储完整路径, 逐层扩展
// 不推荐: 每次入队都要拷贝路径, 空间浪费大
// 仅作对比理解
// ============================================================

/*
 * BFS 过程示意 (graph = [[1,2],[3],[3],[]])：
 *
 * 队列状态:
 * 初始:   [[0]]
 * 扩展0:  [[0,1], [0,2]]
 * 扩展1:  [[0,2], [0,1,3]]     ← [0,1,3] 到达终点, 收集!
 * 扩展2:  [[0,1,3], [0,2,3]]   ← [0,2,3] 到达终点, 收集!
 */

class Solution_BFS {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> result;
        int n = graph.size();
        queue<vector<int>> q;
        q.push({0});  // 初始路径只有节点 0

        while (!q.empty()) {
            vector<int> path = q.front();
            q.pop();
            int node = path.back();  // 路径末尾节点

            if (node == n - 1) {
                result.push_back(path);  // 到达终点, 收集
                continue;
            }

            // 对每个邻居, 创建新路径并入队
            for (int next : graph[node]) {
                vector<int> newPath = path;  // 拷贝路径 (开销大!)
                newPath.push_back(next);
                q.push(newPath);
            }
        }
        return result;
    }
};


// ============================================================
// 【解法3】记忆化 DFS (适用于只求路径本身的变体)
//
// 如果只需要路径数量, 可用 dp[i] = 从 i 到 n-1 的路径数
// dp[n-1] = 1, dp[i] = sum(dp[j]) for j in graph[i]
// 但本题需要具体路径, 记忆化可以缓存子路径列表
// ============================================================

class Solution_Memo {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        int n = graph.size();
        // memo[i] = 从节点 i 到 n-1 的所有路径
        vector<vector<vector<int>>> memo(n);
        vector<bool> computed(n, false);
        return dfs(graph, 0, memo, computed);
    }

private:
    vector<vector<int>>& dfs(vector<vector<int>>& graph, int node,
                              vector<vector<vector<int>>>& memo,
                              vector<bool>& computed) {
        if (computed[node]) return memo[node];
        computed[node] = true;

        int n = graph.size();

        // 基础情况: 到达终点
        if (node == n - 1) {
            memo[node] = {{n - 1}};
            return memo[node];
        }

        // 递归: 拼接当前节点到子路径前面
        for (int next : graph[node]) {
            auto& subPaths = dfs(graph, next, memo, computed);
            for (auto& subPath : subPaths) {
                vector<int> fullPath = {node};
                fullPath.insert(fullPath.end(), subPath.begin(), subPath.end());
                memo[node].push_back(fullPath);
            }
        }
        return memo[node];
    }
};


// ============================================================
// 复杂度分析
// ============================================================
//
// 解法1 DFS/回溯:
//   时间: O(2^n * n) — 最多 2^(n-2) 条路径, 每条长度最多 n
//   空间: O(n) — 递归栈深度 + path 数组 (不算输出)
//
// 解法2 BFS:
//   时间: O(2^n * n) — 同上
//   空间: O(2^n * n) — 队列中同时存储多条路径 (远大于 DFS)
//
// 解法3 记忆化:
//   时间: O(2^n * n) — 同上 (路径总数不变)
//   空间: O(2^n * n) — 缓存所有子路径
//
// n <= 15, 任何解法都能通过

// ============================================================
// 【解法对比】
//
// | 解法            | 时间        | 空间        | 优势                        |
// |----------------|------------|------------|---------------------------|
// | DFS/回溯 (解法1) | O(2^n * n) | O(n)       | 面试首选, 空间最优             |
// | BFS (解法2)     | O(2^n * n) | O(2^n * n) | 直观但空间浪费, 不推荐         |
// | 记忆化 (解法3)   | O(2^n * n) | O(2^n * n) | 适合"路径计数"变体             |
//
// 面试策略: 默认写 DFS/回溯。BFS 存完整路径太浪费, 除非面试官明确要求。
// 如果追问"只求路径数", 用 dp[i] = sum(dp[j]) 即可, 不需要存路径。
//
// ============================================================
// 【易错点】
//
// 1. 误加 visited 数组:
//    ✗ 对 DAG 加 visited → 某些路径被跳过, 漏解
//    ✓ DAG 无环, 不需要 visited (不会死循环)
//
// 2. 忘记初始化 path = {0}:
//    ✗ path 为空, 直接 dfs(0) → 结果路径中缺少起点 0
//    ✓ path 初始化时放入起点 0
//
// 3. 回溯时忘记 pop_back:
//    ✗ 只 push 不 pop → 路径越来越长, 结果全错
//    ✓ push → dfs → pop 三步回溯模板
//
// 4. BFS 中修改原 path 而非拷贝:
//    ✗ path.push_back(next); q.push(path);  → 共享引用导致混乱
//    ✓ vector<int> newPath = path; newPath.push_back(next); q.push(newPath);
//
// ============================================================
// 【面试追问】
//
// Q1: 如果图中有环 (不是 DAG), 代码需要怎么改?
// → 必须加 visited 集合防止死循环。由于要找所有路径,
//   不能用全局 visited (会阻断其他路径), 而是用"路径级 visited":
//   进入节点时标记, 回溯时取消标记 (和回溯模板一致)。
//   时间复杂度可能变为指数级, 取决于环的结构。
//
// Q2: 如果只需要返回路径的数量而非具体路径, 怎么优化?
// → 用 DP: dp[i] = 从节点 i 到 n-1 的路径数。
//   dp[n-1] = 1; 对其余节点逆拓扑序计算 dp[i] = sum(dp[j]) for j in graph[i]。
//   由于 DAG 天然有拓扑序, 直接记忆化递归也行。
//   时间 O(V+E), 空间 O(V), 远优于枚举所有路径。
//
// Q3: 如果要找最短路径 (边权均为 1) 而非所有路径?
// → BFS 层序遍历即可。从节点 0 出发 BFS, 第一次到达 n-1 时
//   的层数就是最短路径长度。如果需要记录路径, 用 parent 数组
//   回溯重建。时间 O(V+E), 比枚举所有路径高效得多。
// ============================================================
