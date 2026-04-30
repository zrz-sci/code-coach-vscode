/*
 * LeetCode 1376: 通知所有员工所需的时间
 *
 * 【题目本质】
 * 在一棵以 headID 为根的多叉树中，求从根到所有叶子节点的最长加权路径。
 * 边权 = 父节点的 informTime（父节点通知下属所需时间）。
 *
 * 【解法总览】
 * 解法1: DFS 自顶向下  — O(n) / O(n) — 递归，最简洁 ⭐面试首选
 * 解法2: BFS 层序遍历  — O(n) / O(n) — 迭代，无栈溢出风险
 * 解法3: 自底向上+记忆化 — O(n) / O(n) — 不需要建树，思维独特
 */

/*
 * 树结构示意（以更复杂的例子说明）:
 *
 *   n=7, headID=0
 *   manager    = [-1, 0, 0, 1, 1, 2, 2]
 *   informTime = [ 4, 3, 2, 0, 0, 0, 0]
 *
 *   建出的多叉树:
 *            0 (time=4)
 *           / \
 *          1   2 (time=2)
 *    (time=3)  / \
 *        / \  5   6    (叶子, time=0)
 *       3   4
 *   (叶子)  (叶子)
 *
 *   路径计算（并行传播，取max）:
 *   0→1→3: 4 + 3 + 0 = 7
 *   0→1→4: 4 + 3 + 0 = 7
 *   0→2→5: 4 + 2 + 0 = 6
 *   0→2→6: 4 + 2 + 0 = 6
 *
 *   DFS过程:
 *   dfs(0) = informTime[0] + max(dfs(1), dfs(2))
 *          = 4 + max(3 + max(0,0), 2 + max(0,0))
 *          = 4 + max(3, 2)
 *          = 4 + 3 = 7
 *
 *   答案 = 7
 */

// ============================================================
// 解法1: DFS 自顶向下 — 递归求根到叶子的最长加权路径
// 时间: O(n)  空间: O(n) (邻接表 + 递归栈)
//
// 【思路】
// 消息从根往下传，每个节点的传播时间 = 自己通知下属的时间 + 下属中最慢那条链的时间。
// 这就是经典的"多叉树上求最大深度"的变体，区别是每层的"深度"不是1而是informTime。
//
// 递归三要素:
//   返回值: 以 node 为根的子树完成传播所需的总时间
//   终止条件: 叶子节点（无子节点），返回 0
//   单层逻辑: informTime[node] + max(所有子节点的递归结果)
// ============================================================
class Solution1 {
public:
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        // 建邻接表: 父亲 → 孩子列表
        // 因为 manager 数组是"孩子→父亲"，我们需要反转
        vector<vector<int>> children(n);
        for (int i = 0; i < n; i++) {
            if (manager[i] != -1) {  // 跳过根节点，其 manager=-1
                children[manager[i]].push_back(i);
            }
        }
        return dfs(headID, children, informTime);
    }

private:
    int dfs(int node, vector<vector<int>>& children, vector<int>& informTime) {
        // 找出所有子节点中传播最慢的那条链
        int maxChild = 0;
        for (int child : children[node]) {
            maxChild = max(maxChild, dfs(child, children, informTime));
        }
        // 当前节点通知下属的时间 + 最慢子链的时间
        // 叶子节点: children为空, maxChild=0, informTime[leaf]=0, 返回0 ✓
        return informTime[node] + maxChild;
    }
};

// ============================================================
// 解法2: BFS 层序遍历 — 迭代，模拟消息传播过程
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 用 BFS 模拟消息从根逐层传播的过程。
// 队列中存 {节点ID, 消息到达该节点的累计时间}。
// 每个节点出队时，将"累计时间 + 自己的informTime"传给子节点入队。
// 答案 = 所有节点的累计到达时间的最大值。
//
// 为什么不能只看最后出队的元素？
// 因为 BFS 按层遍历，同一层的节点可能有不同的累计时间
// （来自不同路径），最后出队的不一定是最大的。
//
// BFS 传播过程 (上面的例子):
//   Queue: {(0, 0)}
//   出队(0, 0): 入队(1, 0+4=4), (2, 0+4=4)
//   Queue: {(1,4), (2,4)}
//   出队(1, 4): 入队(3, 4+3=7), (4, 4+3=7)
//   出队(2, 4): 入队(5, 4+2=6), (6, 4+2=6)
//   Queue: {(3,7), (4,7), (5,6), (6,6)}
//   全部出队，都是叶子节点
//   ans = max(0, 4, 4, 7, 7, 6, 6) = 7 ✓
// ============================================================
class Solution2 {
public:
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        // 建邻接表
        vector<vector<int>> children(n);
        for (int i = 0; i < n; i++) {
            if (manager[i] != -1) {
                children[manager[i]].push_back(i);
            }
        }

        // BFS: {节点ID, 累计到达时间}
        queue<pair<int, int>> q;
        q.push({headID, 0});
        int ans = 0;

        while (!q.empty()) {
            auto [node, time] = q.front();
            q.pop();

            // 每个节点出队时更新答案
            // 叶子节点的 time 就是消息到达它的总时间
            ans = max(ans, time);

            for (int child : children[node]) {
                // 子节点的到达时间 = 当前累计时间 + 当前节点通知下属的时间
                q.push({child, time + informTime[node]});
            }
        }
        return ans;
    }
};

// ============================================================
// 解法3: 自底向上 + 记忆化 — 不需要建树
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 换个角度: 不从根往下看，而是从每个节点往上看。
// 对于节点 i，"消息到达 i 的时间" = "消息到达 i 的父亲的时间" + "父亲的 informTime"。
// 沿着 manager 数组一路向上直到根节点，累加路径上每个节点的 informTime。
//
// 不建邻接表，直接用 manager 数组"回溯"到根。
// 用 memo 记忆化避免重复计算（多个节点可能共享部分向上的路径）。
//
// 记忆化过程 (同一个例子):
//   getTime(3) → getTime(1) → getTime(0) → 0 (根节点)
//     memo[0] = 0
//     memo[1] = memo[0] + informTime[0] = 0 + 4 = 4
//     memo[3] = memo[1] + informTime[1] = 4 + 3 = 7
//   getTime(4): manager[4]=1, memo[1]已有=4
//     memo[4] = 4 + 3 = 7
//   getTime(5): manager[5]=2, getTime(2) → memo[0]已有=0
//     memo[2] = 0 + 4 = 4
//     memo[5] = 4 + 2 = 6
//   getTime(6): manager[6]=2, memo[2]已有=4
//     memo[6] = 4 + 2 = 6
//   ans = max(0, 4, 4, 7, 7, 6, 6) = 7 ✓
// ============================================================
class Solution3 {
public:
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        // memo[i] = 从根传播到节点 i 的累计时间
        vector<int> memo(n, -1);
        memo[headID] = 0;  // 消息起点，累计时间为0

        int ans = 0;
        for (int i = 0; i < n; i++) {
            ans = max(ans, getTime(i, manager, informTime, memo));
        }
        return ans;
    }

private:
    int getTime(int node, vector<int>& manager, vector<int>& informTime, vector<int>& memo) {
        if (memo[node] != -1) return memo[node];

        // 到达 node 的时间 = 到达父亲的时间 + 父亲通知下属的时间
        // 注意: 加的是 manager[node] 的 informTime，不是 node 自己的！
        // 因为是"父亲花时间通知我"，不是"我花时间通知下属"
        int parent = manager[node];
        memo[node] = getTime(parent, manager, informTime, memo) + informTime[parent];
        return memo[node];
    }
};

// ============================================================
// 【解法对比】
//
// | 特性         | DFS自顶向下  | BFS       | 自底向上+记忆化 |
// |-------------|-------------|-----------|---------------|
// | 是否建树     | 是          | 是        | 否            |
// | 遍历方向     | 根→叶       | 根→叶     | 叶→根         |
// | 实现方式     | 递归        | 迭代      | 递归          |
// | 栈溢出风险   | 有(链状树)  | 无        | 有(链状树)    |
// | 代码简洁度   | ⭐⭐⭐      | ⭐⭐      | ⭐⭐          |
// | 面试推荐     | ⭐首选      | 追问迭代  | 展示思维广度  |
//
// 三种解法时间空间复杂度相同，都是 O(n)。
// 面试中先写 DFS，被追问迭代时给 BFS，被追问不建树时给解法3。
//
// ============================================================
// 【易错点】
//
// 1. 并行 vs 串行:
//    ✗ total += dfs(child)     // 串行：以为下属一个一个通知
//    ✓ maxChild = max(maxChild, dfs(child))  // 并行：下属同时开始传播
//
// 2. BFS 中 ans 的取值:
//    ✗ 只在 q.empty() 后取最后出队的 time
//      // 最后出队的不一定是最深节点，因为不同子树深度不同
//    ✓ 每次出队都 ans = max(ans, time)
//
// 3. 自底向上解法中加谁的 informTime:
//    ✗ memo[node] = getTime(parent) + informTime[node]
//      // node 的 informTime 是 node 通知自己下属的时间，不是到达 node 的时间
//    ✓ memo[node] = getTime(parent) + informTime[parent]
//      // parent 花 informTime[parent] 时间通知 node
//
// 4. 建邻接表时忘记判断 manager[i] != -1:
//    ✗ children[manager[i]].push_back(i)  // 当 i=headID 时 manager[i]=-1，越界
//    ✓ if (manager[i] != -1) children[manager[i]].push_back(i)
//
// ============================================================
// 【面试追问】(递进链)
//
// Q1: "这道题的本质是什么？为什么用 max 而不是 sum？"
// A1: 本质是多叉树上根到叶子的最长加权路径。用 max 是因为一个管理者
//     通知完所有下属后，下属们同时开始传播（并行），瓶颈在最慢的那条链。
//
// Q2: "如果树很深（10^5 个节点的链状树），递归解法会怎样？怎么解决？"
// A2: 递归深度达到 10^5，可能栈溢出。解决方案：
//     - 用 BFS（解法2），完全迭代
//     - 用显式栈模拟 DFS
//
// Q3: "如果不仅要求总时间，还要输出最后收到消息的员工 ID 呢？"
// A3: DFS 中让递归返回 pair<int,int>{time, nodeID}；
//     BFS 中在更新 ans 时同步记录 lastNode。
//     注意：可能有多个员工同时最后收到消息（取任意一个或全部）。
//
// Q4: "如果组织架构不是树而是 DAG（允许一个员工有多个上级）？"
// A4: 树变成 DAG 后，用拓扑排序 + DP。每个节点的到达时间 =
//     max(所有入边的 "父节点到达时间 + 父节点informTime")。
//     按拓扑序从根往下处理即可。
// ============================================================
