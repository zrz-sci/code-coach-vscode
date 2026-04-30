/*
 * LeetCode 256: 粉刷房子 (Paint House)
 * 
 * 【题目本质】
 * n 栋房子排成一排，每栋可涂红/蓝/绿三色，相邻不同色，求最小总成本。
 * 经典的线性 DP：带约束的多阶段决策最优化。
 *
 * 【解法总览】
 * 解法1: 暴力递归 — O(2^n) / O(n)     — 枚举所有合法方案（展示本质）
 * 解法2: 记忆化搜索 — O(n) / O(n)     — 自顶向下 DP
 * 解法3: 二维 DP — O(n) / O(n)         — 自底向上递推（面试首选）
 * 解法4: 滚动变量 DP — O(n) / O(1)    — 空间优化（加分项）
 *
 * 【DP 填表过程示意】
 * 示例: costs = [[17,2,17],[16,16,5],[14,3,19]]
 *
 *         R    B    G
 * i=0:   17    2   17     ← base case = costs[0]
 * i=1:   18   33    7     ← dp[1][0]=min(2,17)+16, dp[1][1]=min(17,17)+16, dp[1][2]=min(17,2)+5
 * i=2:   21   10   37     ← dp[2][0]=min(33,7)+14, dp[2][1]=min(18,7)+3,  dp[2][2]=min(18,33)+19
 *
 * 答案 = min(21, 10, 37) = 10
 *
 * 依赖关系:
 * dp[i][j] 依赖 dp[i-1][k] 其中 k != j（上一行的另外两列）
 * → 每行只依赖上一行 → 可以用滚动变量优化空间
 */

// ============================================================
// 解法1: 暴力递归 — 枚举所有合法涂色方案
// 时间: O(2^n)  空间: O(n) 递归栈深度
//
// 【思路】
// 对每栋房子尝试 3 种颜色，约束相邻不同色后递归到下一栋。
// 本质是 DFS 遍历决策树。这是理解问题的起点。
//
// 决策树（前2层）:
//                      start
//              /         |         \
//          h0=R(17)   h0=B(2)    h0=G(17)
//          /   \       /   \       /   \
//      h1=B  h1=G  h1=R  h1=G  h1=R  h1=B
//      (16)  (5)   (16)  (5)   (16)  (16)
//       ...   ...   ...   ...   ...   ...
//
// 问题: 大量重复计算。dfs(2, R) 可能从多条路径被调用。
// ============================================================
class Solution1 {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        int ans = INT_MAX;
        for (int c = 0; c < 3; c++) {
            ans = min(ans, dfs(costs, 0, c));
        }
        return ans;
    }

private:
    // 从第 i 栋开始（第 i 栋涂颜色 c）到最后一栋的最小成本
    int dfs(vector<vector<int>>& costs, int i, int c) {
        if (i == (int)costs.size() - 1) return costs[i][c];

        int best = INT_MAX;
        for (int next = 0; next < 3; next++) {
            if (next != c) { // 相邻不同色约束
                best = min(best, dfs(costs, i + 1, next));
            }
        }
        return costs[i][c] + best;
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 自顶向下 DP
// 时间: O(n·3) = O(n)  空间: O(n·3) = O(n)
//
// 【思路】
// 解法1的瓶颈: dfs(i, c) 被重复调用（重叠子问题）。
// 参数空间只有 n×3 种组合，加 memo 数组缓存后每种只算一次。
// 这是从暴力递归到 DP 的桥梁——逻辑完全不变，只加了缓存。
// ============================================================
class Solution2 {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        vector<vector<int>> memo(n, vector<int>(3, -1));
        int ans = INT_MAX;
        for (int c = 0; c < 3; c++) {
            ans = min(ans, dfs(costs, memo, 0, c));
        }
        return ans;
    }

private:
    int dfs(vector<vector<int>>& costs, vector<vector<int>>& memo, int i, int c) {
        if (i == (int)costs.size() - 1) return costs[i][c];
        if (memo[i][c] != -1) return memo[i][c]; // 命中缓存，直接返回

        int best = INT_MAX;
        for (int next = 0; next < 3; next++) {
            if (next != c) {
                best = min(best, dfs(costs, memo, i + 1, next));
            }
        }
        return memo[i][c] = costs[i][c] + best; // 计算并缓存
    }
};

// ============================================================
// 解法3: 二维 DP（递推）— 面试首选 ⭐
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 把记忆化搜索翻译成自底向上的 for 循环填表。
//
// 状态定义:
//   dp[i][j] = 粉刷第 0~i 栋房子，且第 i 栋涂颜色 j(0=R,1=B,2=G) 的最小总成本
//
// 状态转移（核心思想: 第 i 栋涂 j，前一栋只能涂非 j 的颜色）:
//   dp[i][0] = costs[i][0] + min(dp[i-1][1], dp[i-1][2])
//   dp[i][1] = costs[i][1] + min(dp[i-1][0], dp[i-1][2])
//   dp[i][2] = costs[i][2] + min(dp[i-1][0], dp[i-1][1])
//
// Base case: dp[0][j] = costs[0][j]
// 答案: min(dp[n-1][0], dp[n-1][1], dp[n-1][2])
// ============================================================
class Solution3 {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        vector<vector<int>> dp(n, vector<int>(3, 0));

        // base case: 第 0 栋房子，直接就是自身成本
        dp[0][0] = costs[0][0];
        dp[0][1] = costs[0][1];
        dp[0][2] = costs[0][2];

        for (int i = 1; i < n; i++) {
            // 第 i 栋涂红: 前一栋只能蓝或绿，取更便宜的
            dp[i][0] = costs[i][0] + min(dp[i - 1][1], dp[i - 1][2]);
            // 第 i 栋涂蓝: 前一栋只能红或绿
            dp[i][1] = costs[i][1] + min(dp[i - 1][0], dp[i - 1][2]);
            // 第 i 栋涂绿: 前一栋只能红或蓝
            dp[i][2] = costs[i][2] + min(dp[i - 1][0], dp[i - 1][1]);
        }

        return min({dp[n - 1][0], dp[n - 1][1], dp[n - 1][2]});
    }
};

// ============================================================
// 解法4: 滚动变量 DP — 空间优化
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法3中 dp[i] 只依赖 dp[i-1]，所以不需要二维数组。
// 用 3 个变量记录上一栋房子涂 R/B/G 的最优成本即可。
//
// 关键: 更新时必须用临时变量暂存新值，否则互相覆盖导致错误！
//
// 空间从 O(n) 降到 O(1)，这是线性 DP 空间优化的标准套路。
// ============================================================
class Solution4 {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();

        // 初始化为第 0 栋房子的成本
        int prevR = costs[0][0];
        int prevB = costs[0][1];
        int prevG = costs[0][2];

        for (int i = 1; i < n; i++) {
            // 必须先算出所有新值，再一起更新
            // 否则 prevR 被覆盖后，算 curB 时用的就是错误的 prevR
            int curR = costs[i][0] + min(prevB, prevG);
            int curB = costs[i][1] + min(prevR, prevG);
            int curG = costs[i][2] + min(prevR, prevB);

            prevR = curR;
            prevB = curB;
            prevG = curG;
        }

        return min({prevR, prevB, prevG});
    }
};

// ============================================================
// 解法4b: 原地修改 costs 数组 — 如果允许修改输入
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 直接在 costs 数组上累加，连临时变量都不需要。
// 注意: 面试中修改输入前应先询问面试官是否允许。
// 这里能直接修改的原因是 costs[i-1] 在处理完第 i 行后就不再需要原始值了。
// ============================================================
class Solution4b {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        for (int i = 1; i < n; i++) {
            // costs[i-1] 已经是累计最优成本，直接加到 costs[i] 上
            costs[i][0] += min(costs[i - 1][1], costs[i - 1][2]);
            costs[i][1] += min(costs[i - 1][0], costs[i - 1][2]);
            costs[i][2] += min(costs[i - 1][0], costs[i - 1][1]);
        }
        return min({costs[n - 1][0], costs[n - 1][1], costs[n - 1][2]});
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间    | 空间   | 适用场景          |
// |----------|---------|--------|-------------------|
// | 暴力递归 | O(2^n)  | O(n)   | 理解问题本质      |
// | 记忆化   | O(n)    | O(n)   | 递归思维更自然时   |
// | 二维DP   | O(n)    | O(n)   | ⭐面试首选        |
// | 滚动变量 | O(n)    | O(1)   | 追问空间优化时    |
// | 原地修改 | O(n)    | O(1)   | 允许修改输入时    |
//
// 暴力→记忆化: 加缓存消除重叠子问题
// 记忆化→递推: 递归改循环，消除栈溢出风险
// 递推→滚动变量: dp[i]只依赖dp[i-1]，压缩到常数空间
//
// ============================================================
// 【易错点】
//
// 1. 滚动变量互相覆盖:
//    ✗ prevR = costs[i][0] + min(prevB, prevG);
//      prevB = costs[i][1] + min(prevR, prevG);  // prevR 已被覆盖!
//    ✓ 先算 curR/curB/curG 三个临时值，再一起赋值
//
// 2. dp 初始化错误:
//    ✗ dp[0][j] = 0  — 第0栋房子也要花钱!
//    ✓ dp[0][j] = costs[0][j]
//
// 3. 最终答案只取了一种颜色:
//    ✗ return dp[n-1][0];
//    ✓ return min({dp[n-1][0], dp[n-1][1], dp[n-1][2]});
//    最后一栋涂哪种颜色是不确定的，必须三者取最小
//
// 4. 原地修改 costs 时行内覆盖:
//    costs[i][0] += min(costs[i-1][1], costs[i-1][2]);
//    costs[i][1] += min(costs[i-1][0], costs[i-1][2]);
//    这里 costs[i-1][0] 没被改过(我们只改 costs[i])，所以是安全的。
//    但如果误写成修改 costs[i-1] 就会出错。
//
// ============================================================
// 【面试追问】
//
// Q1(基础): 贪心行不行？每步选当前最便宜的颜色？
//   → 不行。贪心只看局部最优，可能导致后面被迫选昂贵颜色。
//     例如 [[1,100,100],[100,1,2],[1,100,100]]，贪心选 R(1)→B(1)→R(1)=3，
//     但最优可能不同。需要 DP 考虑全局。
//
// Q2(优化): 能否 O(1) 空间？
//   → 可以。dp[i] 只依赖 dp[i-1]，用3个滚动变量。见解法4。
//
// Q3(扩展): 颜色从 3 种变成 k 种？(LeetCode 265)
//   → 朴素做法 O(nk²)。优化: 维护上一行的最小值(min1)和次小值(min2)
//     及最小值对应的颜色索引(minIdx)。转移时:
//     - 如果 j == minIdx，用 min2
//     - 否则用 min1
//     总体 O(nk)。关键改动: 把 min(dp[i-1][k!=j]) 从遍历变为 O(1) 查询。
//
// Q4(变体): 房子排成环形（首尾相邻）？
//   → 枚举第0栋的颜色(3种情况)，每种情况 DP 到最后一栋时排除该颜色，
//     取三种情况的全局最小值。类似 House Robber II (213) 的处理思路。
//
// ============================================================
// 【相关题型】
//
// 265. Paint House II — 颜色扩展到 k 种，复用本题 DP 框架，
//     转移方程从 min(两个) 变为 min(k-1个)，用 min1/min2 优化到 O(1)。
// 198. House Robber — 同样是线性 DP + 相邻约束 (不能都选)，
//     框架相同，区别: 本题是"相邻不同色"，Robber 是"相邻不能都选"。
// 1473. Paint House III — 多一维"分组"状态，但核心决策框架类似。
// ============================================================
