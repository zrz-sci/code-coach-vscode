/*
 * LeetCode 210: 课程表 II (Course Schedule II)
 * 
 * 【题目本质】
 * 给定有向图（课程依赖关系），求一个拓扑排序。
 * 如果图有环（循环依赖），返回空数组。
 *
 * 【与 207 题的关系】
 * 207 只需判断有无环 → 返回 bool
 * 210 需要输出拓扑排序序列 → 返回 vector<int>
 * 代码差异：多一个 result 数组收集出队/完成的节点
 *
 * 【解法总览】
 * 解法1: BFS (Kahn算法)  — O(V+E) / O(V+E) — 面试首选，最直觉
 * 解法2: DFS (后序反转)   — O(V+E) / O(V+E) — 展示DFS功底
 */

// ============================================================
// 解法1: BFS — Kahn 算法（入度表 + 队列）
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 核心思想："逐层剥洋葱"
// - 入度为 0 的节点 = 没有先修课 = 可以直接学
// - 学完一门课后，把依赖它的课程的入度减 1
// - 新产生的入度为 0 的课程又可以学了
// - 重复直到没有入度为 0 的节点
// - 如果此时还有课程没学（入度 > 0），说明存在环
//
// 有向图示意 (示例2):
//
//   prerequisites = [[1,0],[2,0],[3,1],[3,2]]
//
//     0 ──→ 1 ──→ 3       入度:
//     |            ↑       0: 0  ← 起点!
//     └───→ 2 ────┘       1: 1
//                          2: 1
//                          3: 2
//
//   BFS 执行过程:
//   Step1: queue=[0]         result=[]
//   Step2: 弹0, 1入度→0, 2入度→0  queue=[1,2]  result=[0]
//   Step3: 弹1, 3入度→1     queue=[2]            result=[0,1]
//   Step4: 弹2, 3入度→0     queue=[3]            result=[0,1,2]
//   Step5: 弹3              queue=[]             result=[0,1,2,3] ✓
//
// ============================================================
class Solution1 {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // 建邻接表 + 统计入度
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);
        
        for (auto& p : prerequisites) {
            // p = [a, b] 表示 b→a（先学b再学a）
            // 所以 graph[b] 包含 a，a 的入度 +1
            graph[p[1]].push_back(p[0]);
            indegree[p[0]]++;
        }
        
        // 入度为 0 的节点入队 —— 它们没有先修课，可以直接学
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }
        
        // BFS：每次取出一门可学的课
        vector<int> result;
        while (!q.empty()) {
            int course = q.front();
            q.pop();
            result.push_back(course);
            
            // "学完 course" → 去掉从 course 出发的所有边
            for (int next : graph[course]) {
                indegree[next]--;
                // 如果 next 的所有先修课都学完了（入度变为0），入队
                if (indegree[next] == 0) {
                    q.push(next);
                }
            }
        }
        
        // 关键判断：如果有节点始终无法入队（入度 > 0），说明有环
        if ((int)result.size() != numCourses) return {};
        return result;
    }
};

// ============================================================
// 解法2: DFS — 后序遍历 + 反转
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 拓扑排序的 DFS 做法：
// - 对每个节点做 DFS，递归处理所有后继节点
// - 当一个节点的所有后继都处理完毕，把它加入结果（后序位置）
// - 这样得到的是"反向拓扑序"，最后反转即可
//
// 为什么后序的反转是拓扑序？
// - 后序保证：节点 u 在其所有后继节点之后被加入 result
// - 反转后：节点 u 在其所有后继节点之前 → 拓扑序的定义
//
// 判环：三色标记法
//   白色(0) = 未访问
//   灰色(1) = 正在访问中（在当前 DFS 路径上）
//   黑色(2) = 已完成（所有后继都处理完了）
//
//   遇到灰色节点 = 当前路径上的祖先 = 存在环!
//
// DFS 执行过程 (示例2):
//
//     0 ──→ 1 ──→ 3
//     |            ↑
//     └───→ 2 ────┘
//
//   从节点0开始 DFS:
//   dfs(0) → 灰色0
//     dfs(1) → 灰色1
//       dfs(3) → 灰色3 → 无后继 → 黑色3, result=[3]
//     黑色1, result=[3,1]
//     dfs(2) → 灰色2
//       dfs(3) → 已黑色, 跳过
//     黑色2, result=[3,1,2]
//   黑色0, result=[3,1,2,0]
//
//   反转: [0,2,1,3] ← 合法拓扑序 ✓
//
// ============================================================
class Solution2 {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // 建图
        vector<vector<int>> graph(numCourses);
        for (auto& p : prerequisites) {
            graph[p[1]].push_back(p[0]);
        }
        
        // 三色标记：0=白, 1=灰(访问中), 2=黑(已完成)
        vector<int> color(numCourses, 0);
        vector<int> result;
        bool hasCycle = false;
        
        // 对每个未访问的节点发起 DFS
        for (int i = 0; i < numCourses; i++) {
            if (color[i] == 0) {
                dfs(graph, i, color, result, hasCycle);
                if (hasCycle) return {};  // 提前终止
            }
        }
        
        // 后序遍历的结果是反向拓扑序，需要反转
        reverse(result.begin(), result.end());
        return result;
    }

private:
    void dfs(vector<vector<int>>& graph, int node,
             vector<int>& color, vector<int>& result, bool& hasCycle) {
        if (hasCycle) return;  // 剪枝：已发现环，不必继续
        
        color[node] = 1;  // 标记为灰色（正在访问）
        
        for (int next : graph[node]) {
            if (color[next] == 1) {
                // 遇到灰色 = 当前递归栈中的节点 = 有环!
                hasCycle = true;
                return;
            }
            if (color[next] == 0) {
                // 白色：未访问，递归探索
                dfs(graph, next, color, result, hasCycle);
                if (hasCycle) return;
            }
            // 黑色(2)：已完成，不需要再处理
        }
        
        // 后序位置：所有后继都处理完了，加入 result
        color[node] = 2;  // 标记为黑色（已完成）
        result.push_back(node);
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | BFS (Kahn)              | DFS (后序反转)          |
// |-----------|-------------------------|------------------------|
// | 直觉性     | ⭐高 "先学没前提的课"     | 中，需理解后序+反转      |
// | 判环       | result.size()!=n        | 三色标记遇灰色          |
// | 代码风格   | 迭代，简洁               | 递归，需小心栈溢出       |
// | 扩展       | 容易改成按层/并行调度     | 容易改成求最长路径       |
// | 面试推荐   | ⭐首选                  | 追问时展示DFS功底       |
//
// 【易错点】
//
// 1. 建图方向搞反:
//    ✗ graph[p[0]].push_back(p[1])  ← a→b，但题意是 b→a
//    ✓ graph[p[1]].push_back(p[0])  ← b→a，b是a的先修课
//    原因: prerequisites[i]=[a,b] 表示学a之前要先学b
//
// 2. DFS 忘记反转:
//    ✗ 直接返回 result → 得到的是反向拓扑序 [3,1,2,0]
//    ✓ reverse(result.begin(), result.end()) → [0,2,1,3]
//
// 3. DFS 用 bool visited 代替三色:
//    ✗ if (visited[next]) hasCycle = true;
//       → "已完成"的节点也会被误判为环
//    ✓ 必须区分"访问中(灰)"和"已完成(黑)"
//       只有遇到灰色才是环，黑色是正常的交叉边
//
// 4. BFS 忘记判断环:
//    ✗ 直接 return result; → 有环时返回不完整的结果
//    ✓ if (result.size() != numCourses) return {};
//
// 5. 孤立节点遗漏:
//    没有出现在 prerequisites 中的课程，入度为 0，
//    BFS 会正常入队，DFS 外层 for 循环也会覆盖到。
//    ✗ 只遍历 prerequisites 中出现过的节点
//    ✓ 遍历 0 到 numCourses-1 的所有节点
//
// 【面试追问】
//
// Q1: 和 207 题代码差异在哪？
// → 207 返回 bool，只需计数器 count++，最后 return count == numCourses
//   210 返回 vector<int>，需要 result.push_back(course)
//   核心拓扑排序逻辑完全相同
//
// Q2: 如果要输出所有合法拓扑序怎么办？
// → 回溯法：每次从所有入度为0的节点中"选一个"，选完更新入度，
//   递归到下一个位置。回溯时恢复入度。复杂度取决于拓扑序总数。
//
// Q3: 如果每学期最多学 k 门课，最少几个学期？
// → BFS 分层处理，每层最多取 k 个入度为0的节点。
//   层数 = 学期数。参考 LeetCode 1136 Parallel Courses。
//
// Q4: 怎么检测具体哪些课程形成了环？
// → DFS 中，当发现灰色节点时，从当前递归栈回溯到那个灰色节点，
//   路径上的所有节点就构成了环。可以用一个 path 向量记录当前路径。
// ============================================================
