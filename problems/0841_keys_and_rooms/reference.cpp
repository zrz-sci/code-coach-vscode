/*
 * LeetCode 841: Keys and Rooms (钥匙和房间)
 * 难度: Medium | 标签: Depth-First Search, Breadth-First Search, Graph
 *
 * 题目:
 *   n 个房间 (0 到 n-1), 除 0 号外都上锁。
 *   rooms[i] = 进入房间 i 后获得的钥匙列表。
 *   判断从 0 号出发能否访问所有房间。
 *
 * 本质: 有向图单源可达性 -- 从节点 0 出发能否到达所有节点。
 *
 * DFS 过程 (rooms = [[1],[2],[3],[]]):
 *
 *   dfs(0) -> visited[0]=T, 钥匙[1]
 *     dfs(1) -> visited[1]=T, 钥匙[2]
 *       dfs(2) -> visited[2]=T, 钥匙[3]
 *         dfs(3) -> visited[3]=T, 钥匙[] -> 返回
 *       返回
 *     返回
 *   返回
 *   visited = [T,T,T,T] -> true
 *
 * DFS 过程 (rooms = [[1,3],[3,0,1],[2],[0]]):
 *
 *   dfs(0) -> visited[0]=T, 钥匙[1,3]
 *     dfs(1) -> visited[1]=T, 钥匙[3,0,1]
 *       dfs(3) -> visited[3]=T, 钥匙[0]
 *         0 已访问, 跳过 -> 返回
 *       0 已访问, 跳过
 *       1 已访问, 跳过 -> 返回
 *     dfs(3): 3 已访问, 跳过 -> 返回
 *   visited = [T,T,F,T] -> false (房间2不可达!)
 */


// ================================================================
// 解法一: DFS 递归 -- O(n + E) 时间, O(n) 空间 [最推荐]
// ================================================================
// 从房间 0 开始递归 DFS:
//   1. 标记当前房间已访问
//   2. 遍历当前房间的每把钥匙 (即每条出边)
//   3. 如果目标房间未访问, 递归进入
//   4. 最后检查是否所有房间都已访问
class Solution {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();
        vector<bool> visited(n, false);

        dfs(rooms, 0, visited);

        // 检查是否所有房间都被访问
        for (bool v : visited) {
            if (!v) return false;
        }
        return true;
    }

private:
    void dfs(vector<vector<int>>& rooms, int room, vector<bool>& visited) {
        visited[room] = true;
        for (int key : rooms[room]) {
            if (!visited[key]) {
                dfs(rooms, key, visited);
            }
        }
    }
};


// ================================================================
// 解法二: DFS 迭代 (显式栈) -- O(n + E) 时间, O(n) 空间
// ================================================================
// 用显式栈替代递归, 避免深度过大时的栈溢出
// 逻辑与递归 DFS 完全相同
class Solution2 {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();
        vector<bool> visited(n, false);
        stack<int> stk;

        // 从房间 0 开始
        stk.push(0);
        visited[0] = true;
        int count = 1;  // 已访问房间计数

        while (!stk.empty()) {
            int room = stk.top();
            stk.pop();

            // 遍历当前房间的所有钥匙
            for (int key : rooms[room]) {
                if (!visited[key]) {
                    visited[key] = true;
                    count++;
                    stk.push(key);
                }
            }
        }

        return count == n;
    }
};


// ================================================================
// 解法三: BFS (队列) -- O(n + E) 时间, O(n) 空间
// ================================================================
// 广度优先: 逐层扩展可达房间
// 与 DFS 等价, 只是遍历顺序不同
class Solution3 {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();
        vector<bool> visited(n, false);
        queue<int> q;

        // 从房间 0 开始
        q.push(0);
        visited[0] = true;
        int count = 1;

        while (!q.empty()) {
            int room = q.front();
            q.pop();

            for (int key : rooms[room]) {
                if (!visited[key]) {
                    visited[key] = true;
                    count++;
                    q.push(key);
                }
            }
        }

        return count == n;

        // BFS 过程 (rooms = [[1],[2],[3],[]]):
        //   队列: [0]          visited: {0}
        //   取出 0, 钥匙[1]    -> 队列: [1]   visited: {0,1}
        //   取出 1, 钥匙[2]    -> 队列: [2]   visited: {0,1,2}
        //   取出 2, 钥匙[3]    -> 队列: [3]   visited: {0,1,2,3}
        //   取出 3, 钥匙[]     -> 队列: []    visited: {0,1,2,3}
        //   count=4 == n=4 -> true
    }
};


// ================================================================
// 解法四: DFS 递归 + 计数器 (另一种检查方式) -- O(n + E)
// ================================================================
// 不用最后遍历 visited, 在 DFS 过程中维护计数器
// 如果计数器达到 n, 可以提前终止 (但最坏仍 O(n+E))
class Solution4 {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();
        vector<bool> visited(n, false);
        int count = 0;

        dfs(rooms, 0, visited, count);

        return count == n;
    }

private:
    void dfs(vector<vector<int>>& rooms, int room,
             vector<bool>& visited, int& count) {
        visited[room] = true;
        count++;

        for (int key : rooms[room]) {
            if (!visited[key]) {
                dfs(rooms, key, visited, count);
            }
        }
    }
};


/*
 * 复杂度分析:
 *
 * | 解法          | 时间       | 空间    |
 * |--------------|----------|--------|
 * | DFS 递归      | O(n + E) | O(n)   |
 * | DFS 迭代栈    | O(n + E) | O(n)   |
 * | BFS 队列      | O(n + E) | O(n)   |
 * | DFS + 计数器  | O(n + E) | O(n)   |
 *
 * 其中:
 *   n = rooms.length (房间数, 2 <= n <= 1000)
 *   E = sum(rooms[i].length) (总钥匙数/总边数, <= 3000)
 *
 * 四种解法本质相同, 都是图遍历, 只是实现细节不同:
 *   - 递归 DFS: 代码最短, 面试首选
 *   - 迭代 DFS: 防栈溢出 (本题 n<=1000 不会溢出, 但是好习惯)
 *   - BFS: 层次清晰, 适合需要最短路径的变体
 *   - 计数器: 避免最后的遍历检查
 *
 * 易错点:
 * 1. 起点: 从房间 0 开始, 不要忘记标记 visited[0] = true
 * 2. 图的方向: 这是有向图, rooms[i] 的钥匙是从 i 出发的边
 * 3. 判定: 是 count == n, 不是 count == n-1
 * 4. 不需要建图: rooms[i] 本身就是邻接表
 * 5. 自环安全: 如果 rooms[i] 含 i, visited 检查会跳过, 不会死循环
 */

// ============================================================
// 【解法对比】
//
// | 维度         | DFS递归 ⭐      | DFS迭代栈       | BFS队列          |
// |-------------|---------------|----------------|-----------------|
// | 时间         | O(n + E)      | O(n + E)       | O(n + E)        |
// | 空间         | O(n) + 递归栈  | O(n)           | O(n)            |
// | 代码量       | 最短           | 中等           | 中等             |
// | 栈溢出风险   | 有 (n大时)     | 无             | 无               |
// | 面试推荐     | ⭐ 首选        | 追问栈溢出时    | 追问BFS变体时     |
//
// 四种解法本质相同, 均为有向图单源可达性, 核心区别:
//   递归DFS — 代码最简, 面试默认首选
//   迭代DFS — 防栈溢出, 工程实践更稳健
//   BFS     — 若变体需要"最少步数", BFS天然适合
//   计数器  — 微优化, 避免最后一遍遍历
//
// ============================================================
// 【易错点】
//
// 1. 忘记标记房间0为已访问:
//    ✗ visited[0] 未设 true → 最终 count 少 1
//    ✓ DFS从0进入时第一步就标记 visited[0] = true
//
// 2. 把无向图思维套上来:
//    rooms 是有向图邻接表 (房间i的钥匙 = i的出边)
//    不要反向加边, 也不需要双向遍历
//
// 3. 判定条件写错:
//    ✗ return count == n - 1  → 漏掉了房间0
//    ✓ return count == n      → 所有 n 个房间都要计入
//
// 4. 迭代 DFS 中入栈时机:
//    ✗ 出栈时才标记 visited → 同一房间可能多次入栈
//    ✓ 入栈前就标记 visited → 避免重复入栈
//
// 5. 混淆"钥匙"和"房间编号":
//    rooms[i] 中的值是房间编号 (0 到 n-1), 不是钥匙的"类型"
//    直接当作下一个可访问的节点即可
//
// ============================================================
// 【面试追问】
//
// Q1: 如果问的是"最少开门次数到达所有房间"怎么做?
//     BFS 从房间0开始, 层数 = 最远房间距离。
//     但本题所有边权为1, DFS 和 BFS 都只需 n+E 步。
//     若边权不同 (如开门有代价), 需要 Dijkstra。
//
// Q2: 如果允许多次进入同一房间, 有区别吗?
//     没有区别。visited 数组保证每个房间只处理一次。
//     多次进入不会产生新钥匙, 所以不影响可达性。
//
// Q3: 如何判断哪些房间不可达并输出它们?
//     DFS/BFS 结束后, 遍历 visited 数组,
//     收集所有 visited[i] == false 的房间编号。
//     时间不变, 仍为 O(n + E)。
//
// ============================================================
// 【相关题型】
//
// - 547. Number of Provinces: 无向图连通分量 (Union-Find/DFS)
// - 200. Number of Islands: 网格 DFS/BFS
// - 332. Reconstruct Itinerary: 有向图欧拉路径
// - 1971. Find if Path Exists in Graph: 图可达性判定
// ============================================================
