/*
 * LeetCode 851: Loud and Rich (喧闹和富有)
 *
 * 【题目本质】
 * richer 关系构成 DAG，对每个节点找其所有祖先（含自身）中 quiet 值最小的人。
 * 核心问题：在 DAG 上传播"最安静人"信息。
 *
 * 【解法总览】
 * 解法1: DFS + 记忆化  — O(V+E) / O(V+E) — 面试推荐，代码简洁
 * 解法2: 拓扑排序 BFS   — O(V+E) / O(V+E) — 面试推荐，逻辑清晰
 */

// ============================================================
// 解法1: DFS + 记忆化 — 自顶向下 ⭐ 面试推荐
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 建反向图: richer[i]=[a,b] (a比b富) → 建边 b→a (穷→富)
// 对每个人 x，DFS 沿反向边找到所有比 x 富的人，取 quiet 最小的。
// 记忆化：已算过的节点直接返回，避免重复遍历。
//
// 为什么建反向图？
//   我们要找"所有比 x 富的人"。richer 告诉我们 a 比 b 富(a→b)。
//   如果建正向图 a→b，从 x 出发遍历到的是比 x 穷的人，方向反了。
//   建反向图 b→a，从 x(穷) 出发，沿边到达的都是比 x 富的人。
//
// 记忆化正确性：
//   DAG 无环，所以 DFS 不会死循环。
//   节点 u 的答案只依赖于比 u 富的人的答案（更"上游"的节点）。
//   一旦 u 的所有上游都处理完，u 的答案就确定了，可以缓存。
//
// 示例: richer=[[1,0],[2,1],[3,1],[3,7],[4,3],[5,3],[6,3]]
//       quiet=[3,2,5,4,6,1,7,0]
//
// 反向图 (穷→富):
//   0→1, 1→2, 1→3, 7→3, 3→4, 3→5, 3→6
//
// DFS(0): → 1 → 2 (叶子,ans=2) → 3 → 4(ans=4) → 5(ans=5) → 6(ans=6)
//         ans[3]=5 (quiet[5]=1最小)
//         ans[1]=5 (quiet[5]=1 < quiet[2]=5 < quiet[1]=2)
//         ans[0]=5 (quiet[5]=1 < quiet[0]=3)
// ============================================================
class Solution1 {
public:
    vector<vector<int>> graph; // 反向图: 穷 → 富
    vector<int> answer;

    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        graph.resize(n);
        answer.assign(n, -1); // -1 表示未计算

        // 建反向图: a 比 b 富 → 建边 b → a
        for (auto& r : richer) {
            graph[r[1]].push_back(r[0]);
        }

        // 对每个人执行 DFS (有记忆化，总共 O(V+E))
        for (int i = 0; i < n; i++) {
            dfs(i, quiet);
        }
        return answer;
    }

    int dfs(int u, vector<int>& quiet) {
        // 记忆化: 已经计算过则直接返回
        if (answer[u] != -1) return answer[u];

        // 初始: 自己是"比自己富的人中最安静的" (包含自身)
        answer[u] = u;

        // 遍历所有比 u 富的人
        for (int v : graph[u]) {
            int candidate = dfs(v, quiet);
            // 如果找到了更安静的人，更新答案
            if (quiet[candidate] < quiet[answer[u]]) {
                answer[u] = candidate;
            }
        }

        return answer[u];
    }
};

// ============================================================
// 解法2: 拓扑排序 BFS — 自底向上 ⭐
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 建正向图: a 比 b 富 → 建边 a → b (富→穷)
// 入度为 0 的节点是最富的人（没有人比他们更富）。
// BFS 从最富的人开始，逐层向穷人传播"当前已知最安静的人"。
//
// 传播逻辑:
//   处理节点 u 时，u 的 answer 已经是最终答案（所有比 u 富的人都已处理）。
//   对 u 的每个邻居 v (u 比 v 富):
//     如果 quiet[answer[u]] < quiet[answer[v]]，
//     则更新 answer[v] = answer[u]（v 发现了一个更安静的有钱人）。
//
// 为什么拓扑序保证正确？
//   拓扑排序确保处理 v 之前，所有比 v 富的人都已经处理完毕。
//   所以 v 出队时，所有可能更新 v 的节点都已经贡献过了。
//
// 示例 (续):
//   正向图: 1→0, 2→1, 3→1, 3→7, 4→3, 5→3, 6→3
//   入度: 0:1, 1:2, 2:0, 3:3, 4:0, 5:0, 6:0, 7:1
//   初始队列: [2, 4, 5, 6]
//
//   处理2: 2→1, answer[1]=1 vs answer[2]=2, quiet[2]=5>quiet[1]=2, 不更新
//   处理4: 4→3, answer[3]=3 vs answer[4]=4, quiet[4]=6>quiet[3]=4, 不更新
//   处理5: 5→3, answer[3]=3 vs answer[5]=5, quiet[5]=1<quiet[3]=4, answer[3]=5
//   处理6: 6→3, answer[3]=5 vs answer[6]=6, quiet[6]=7>quiet[5]=1, 不更新
//   处理3: 3→1, answer[1]=1 vs answer[3]=5, quiet[5]=1<quiet[1]=2, answer[1]=5
//          3→7, answer[7]=7 vs answer[3]=5, quiet[5]=1>quiet[7]=0, 不更新
//   处理1: 1→0, answer[0]=0 vs answer[1]=5, quiet[5]=1<quiet[0]=3, answer[0]=5
//   处理7: 无出边
//   处理0: 无出边
//
//   answer = [5,5,2,5,4,5,6,7] ✓
// ============================================================
class Solution2 {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<vector<int>> graph(n); // 正向图: 富 → 穷
        vector<int> indegree(n, 0);

        // 建图: a 比 b 富 → 边 a → b
        for (auto& r : richer) {
            graph[r[0]].push_back(r[1]);
            indegree[r[1]]++;
        }

        // 初始化: 每个人的答案是自己
        vector<int> answer(n);
        iota(answer.begin(), answer.end(), 0); // answer[i] = i

        // BFS 拓扑排序
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) {
                q.push(i); // 最富的人（无人比他们更富）
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // 向所有比 u 穷的人传播最安静人信息
            for (int v : graph[u]) {
                // 如果 u 知道的"最安静有钱人"比 v 当前知道的更安静
                if (quiet[answer[u]] < quiet[answer[v]]) {
                    answer[v] = answer[u];
                }

                // 入度减1，如果变为0则入队
                if (--indegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        return answer;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间    | 空间    | 建图方向        | 优势            |
// |------------|--------|--------|---------------|----------------|
// | DFS+记忆化  | O(V+E) | O(V+E) | 反向(穷→富)    | 代码最短         |
// | 拓扑排序BFS | O(V+E) | O(V+E) | 正向(富→穷)    | 逻辑最清晰       |
//
// 两种解法复杂度完全相同，面试选哪个都行。
// ============================================================
// 【易错点】
//
// 1. 建图方向搞反:
//    ✗ DFS 用正向图 → 搜到比自己穷的人
//    ✓ DFS 用反向图(穷→富)，拓扑排序用正向图(富→穷)
//
// 2. 比较 quiet 时用人编号而非 quiet 值:
//    ✗ if (candidate < answer[u])
//    ✓ if (quiet[candidate] < quiet[answer[u]])
//
// 3. 忘记初始化 answer[i] = i:
//    "比自己富或等富"包含自己。忘了初始化 → answer 可能未定义。
//
// 4. 记忆化标记用 0:
//    ✗ answer[u] = 0 表示未计算 → 人0被误判为已算
//    ✓ answer[u] = -1 表示未计算
//
// ============================================================
// 【相关题型】
//
// - 210. Course Schedule II:
//   拓扑排序经典题。同样建图+BFS。
//
// - 802. Find Eventual Safe States:
//   反向图+拓扑排序找安全节点。
//
// - 207. Course Schedule:
//   判断 DAG 是否有环。拓扑排序基础。
// ============================================================
