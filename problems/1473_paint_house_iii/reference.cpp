/*
 * LeetCode 1473: 粉刷房子 III (Paint House III)
 * 
 * 【题目本质】
 * 给 m 个房子涂色（部分已涂色），使恰好形成 target 个街区，求最小花费。
 * 本质是三维 DP：位置 × 颜色 × 街区数。
 *
 * 【解法总览】
 * 解法1: 记忆化搜索(自顶向下)   — O(m·n²·target) / O(m·n·target) + 递归栈
 * 解法2: 三维递推 DP(自底向上)   — O(m·n²·target) / O(m·n·target) — 面试首选
 * 解法3: 空间优化 DP(滚动数组)   — O(m·n²·target) / O(n·target) — 加分项
 *
 * DP 填表示意 (houses=[0,0,0], n=2, target=2):
 *
 *          k=1       k=2
 * i=0 j=1  cost[0][0]  ∞
 *     j=2  cost[0][1]  ∞
 *
 * i=1 j=1  dp[0][1][1]+c10          min(dp[0][2][1])+c10
 *                ↑ 同色不加街区             ↑ 异色加街区
 *     j=2  dp[0][2][1]+c11          min(dp[0][1][1])+c11
 *
 * 填表方向: i 从 0 到 m-1，每层遍历 j(颜色) 和 k(街区数)
 * 依赖: dp[i][j][k] 依赖 dp[i-1][j][k] (同色) 和 dp[i-1][j'!=j][k-1] (异色)
 */

// ============================================================
// 解法1: 记忆化搜索 — 自顶向下递归
// 时间: O(m·n²·target)  空间: O(m·n·target) + 递归栈 O(m)
//
// 【思路】
// 最自然的递归思维：从第0个房子开始，逐个做决定。
// 递归函数定义：从第i个房子开始，前一个房子颜色为prevColor，
// 还需要形成remain个街区，求最小花费。
// 发现大量重叠子问题（相同的 i, prevColor, remain 被多次计算），
// 加 memo 数组缓存即可。
// ============================================================
class Solution1 {
public:
    int minCost(vector<int>& houses, vector<vector<int>>& cost, int m, int n, int target) {
        // memo[i][prevColor][remain]，prevColor范围0~n，remain范围0~target
        vector<vector<vector<int>>> memo(m, vector<vector<int>>(n + 1, vector<int>(target + 1, -1)));
        
        // 从第0个房子开始，前一个"颜色"为0（不存在），需要凑target个街区
        int ans = dfs(houses, cost, memo, m, n, 0, 0, target);
        return ans >= 1e8 ? -1 : ans;
    }
    
private:
    int dfs(vector<int>& houses, vector<vector<int>>& cost,
            vector<vector<vector<int>>>& memo,
            int m, int n, int i, int prevColor, int remain) {
        // 街区数已经不够了（变负了）
        if (remain < 0) return 1e8;
        // 所有房子处理完，检查是否恰好用完target个街区
        if (i == m) return remain == 0 ? 0 : (int)1e8;
        // 剩余房子数 < 剩余需要的街区数 → 不可能
        if (remain > m - i) return 1e8;
        
        if (memo[i][prevColor][remain] != -1) return memo[i][prevColor][remain];
        
        int res = (int)1e8;
        
        if (houses[i] != 0) {
            // 已涂色：颜色固定，花费0
            int color = houses[i];
            // 是否和前一个同色 → 决定是否消耗一个街区
            int newRemain = (color == prevColor) ? remain : remain - 1;
            res = dfs(houses, cost, memo, m, n, i + 1, color, newRemain);
        } else {
            // 未涂色：尝试每种颜色
            for (int j = 1; j <= n; j++) {
                int newRemain = (j == prevColor) ? remain : remain - 1;
                int sub = dfs(houses, cost, memo, m, n, i + 1, j, newRemain);
                if (sub < 1e8) {
                    res = min(res, sub + cost[i][j - 1]); // j-1: cost是0-indexed
                }
            }
        }
        
        memo[i][prevColor][remain] = res;
        return res;
    }
};

// ============================================================
// 解法2: 三维递推 DP — 自底向上填表 (面试首选)
// 时间: O(m·n²·target)  空间: O(m·n·target)
//
// 【思路】
// 将记忆化搜索翻译成递推。
// 状态定义: dp[i][j][k] = 前(i+1)个房子，第i个颜色为j，恰好k个街区的最小花费
// 
// 转移:
//   同色: dp[i][j][k] = dp[i-1][j][k] + paintCost
//   异色: dp[i][j][k] = min(dp[i-1][j'][k-1], j'!=j) + paintCost
//   取两者最小值
//
// 为什么这样做是正确的？
// 街区数k的变化只取决于当前颜色和上一个颜色是否相同：
//   - 同色：不产生新街区，街区数不变
//   - 异色：产生新街区，街区数+1（所以从k-1转移）
// ============================================================
class Solution2 {
public:
    int minCost(vector<int>& houses, vector<vector<int>>& cost, int m, int n, int target) {
        // 用1e8而非INT_MAX，避免加法溢出（最大可能花费 100*10000 = 1e6 远小于1e8）
        const int INF = 1e8;
        
        // dp[i][j][k]: j用1~n编号，k范围1~target
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n + 1, vector<int>(target + 1, INF)));
        
        // ===== Base case: 第一个房子 =====
        // 第一个房子一定形成1个街区（k=1）
        if (houses[0] != 0) {
            dp[0][houses[0]][1] = 0; // 已涂色，花费0
        } else {
            for (int j = 1; j <= n; j++) {
                dp[0][j][1] = cost[0][j - 1]; // 涂色j，花费cost[0][j-1]
            }
        }
        
        // ===== 逐个房子转移 =====
        for (int i = 1; i < m; i++) {
            // 确定第i个房子可选的颜色范围
            int lo, hi;
            if (houses[i] != 0) {
                lo = hi = houses[i]; // 已涂色，只能用这个颜色
            } else {
                lo = 1; hi = n; // 未涂色，所有颜色可选
            }
            
            for (int j = lo; j <= hi; j++) {
                // 涂色花费：已涂色为0，未涂色为cost[i][j-1]
                int paintCost = (houses[i] != 0) ? 0 : cost[i][j - 1];
                
                // k最多为i+1（前i+1个房子最多i+1个街区），且不超过target
                for (int k = 1; k <= min(i + 1, target); k++) {
                    int best = INF;
                    
                    // 情况1: 同色 → 街区数不变，从dp[i-1][j][k]转移
                    best = min(best, dp[i - 1][j][k]);
                    
                    // 情况2: 异色 → 新增一个街区，从dp[i-1][j'][k-1]转移
                    if (k >= 2) {
                        for (int j2 = 1; j2 <= n; j2++) {
                            if (j2 != j) {
                                best = min(best, dp[i - 1][j2][k - 1]);
                            }
                        }
                    }
                    
                    if (best < INF) {
                        dp[i][j][k] = best + paintCost;
                    }
                }
            }
        }
        
        // ===== 答案: 最后一个房子，所有颜色，恰好target个街区 =====
        int ans = INF;
        for (int j = 1; j <= n; j++) {
            ans = min(ans, dp[m - 1][j][target]);
        }
        return ans >= INF ? -1 : ans;
    }
};

// ============================================================
// 解法3: 空间优化 DP — 滚动数组
// 时间: O(m·n²·target)  空间: O(n·target)
//
// 【思路】
// dp[i] 只依赖 dp[i-1]，经典滚动数组优化场景。
// 只保留 prev 和 curr 两层，每处理完一个房子后 swap。
// 空间从 O(m·n·target) 降到 O(n·target)。
// ============================================================
class Solution3 {
public:
    int minCost(vector<int>& houses, vector<vector<int>>& cost, int m, int n, int target) {
        const int INF = 1e8;
        
        vector<vector<int>> prev(n + 1, vector<int>(target + 1, INF));
        vector<vector<int>> curr(n + 1, vector<int>(target + 1, INF));
        
        // Base case
        if (houses[0] != 0) {
            prev[houses[0]][1] = 0;
        } else {
            for (int j = 1; j <= n; j++) {
                prev[j][1] = cost[0][j - 1];
            }
        }
        
        for (int i = 1; i < m; i++) {
            // 每轮重置 curr 为 INF
            for (int j = 0; j <= n; j++)
                fill(curr[j].begin(), curr[j].end(), INF);
            
            int lo, hi;
            if (houses[i] != 0) {
                lo = hi = houses[i];
            } else {
                lo = 1; hi = n;
            }
            
            for (int j = lo; j <= hi; j++) {
                int paintCost = (houses[i] != 0) ? 0 : cost[i][j - 1];
                
                for (int k = 1; k <= min(i + 1, target); k++) {
                    int best = prev[j][k]; // 同色
                    
                    if (k >= 2) {
                        for (int j2 = 1; j2 <= n; j2++) {
                            if (j2 != j) {
                                best = min(best, prev[j2][k - 1]); // 异色
                            }
                        }
                    }
                    
                    if (best < INF) {
                        curr[j][k] = best + paintCost;
                    }
                }
            }
            
            swap(prev, curr); // O(1) 交换指针，不是O(n*target)拷贝
        }
        
        int ans = INF;
        for (int j = 1; j <= n; j++) {
            ans = min(ans, prev[j][target]);
        }
        return ans >= INF ? -1 : ans;
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 (记忆化搜索) vs 解法2 (递推 DP):
//   本质等价，记忆化搜索更容易写（直接翻译递归思维），
//   递推 DP 没有递归栈开销，面试推荐。
//
// 解法2 vs 解法3:
//   仅空间优化（m·n·target → n·target），代码改动很小：
//   把三维数组替换成两个二维数组 + swap。
//
// 进一步优化（面试追问）:
//   对每个 k，预处理 prev[*][k] 的最小值和次小值，
//   可将"遍历所有异色"从 O(n) 降到 O(1)，
//   总时间从 O(m·n²·target) 降到 O(m·n·target)。
//
// 【易错点】
// 1. 用 INT_MAX 作为 INF → best + paintCost 整数溢出
//    ✗ const int INF = INT_MAX;
//    ✓ const int INF = 1e8;  // 100 * 10^4 = 10^6 << 10^8
//
// 2. 颜色编号搞混:
//    ✗ dp[0][j][1] = cost[0][j]; // j从1开始但cost是0-indexed
//    ✓ dp[0][j][1] = cost[0][j - 1];
//
// 3. 已涂色房子仍然加了 cost:
//    ✗ dp[i][j][k] = best + cost[i][j-1]; // 不管是否已涂色
//    ✓ int paintCost = (houses[i] != 0) ? 0 : cost[i][j-1];
//
// 4. 初始化 dp[0][j][0] = 0 而非 dp[0][j][1]:
//    ✗ dp[0][j][0] = cost[0][j-1]; // 0个街区不对，第一个房子形成1个街区
//    ✓ dp[0][j][1] = cost[0][j-1];
//
// 5. 空间优化时忘记每轮重置 curr:
//    ✗ 不重置，导致上一轮的值残留
//    ✓ 每轮开头 fill(curr, INF)
//
// 【面试追问】
// Q1: 为什么需要三维而不是二维？
//     → 需要知道第 i 个房子的颜色，才能判断第 i+1 个房子是否开新街区。
//
// Q2: 内层 O(n) 遍历异色能否优化？
//     → 对每个 k 维护 prev[*][k] 的最小值和次小值（及颜色），
//        当 j 和最小值颜色相同时取次小值，否则取最小值。O(1)。
//
// Q3: 如果 n 很大（如 10^5），当前 O(m·n²·target) 超时怎么办？
//     → 必须使用 Q2 的优化，降到 O(m·n·target)。
//        同时空间用滚动数组 O(n·target)。
// ============================================================
