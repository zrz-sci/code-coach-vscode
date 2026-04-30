/*
 * LeetCode 474: 一和零 (Ones and Zeroes)
 *
 * 【题目本质】
 * 二维费用的 0/1 背包问题：每个字符串是一件物品，消耗 (zeros, ones) 两种资源，
 * 价值为 1，背包容量为 (m, n)，求最大价值。
 *
 * 【解法总览】
 * 解法1: 暴力递归 (DFS)         — O(2^len)     / O(len)       — 理解结构
 * 解法2: 记忆化搜索             — O(len×m×n)   / O(len×m×n)   — 发现重叠子问题
 * 解法3: 三维 DP (递推)         — O(len×m×n)   / O(len×m×n)   — 递归→递推
 * 解法4: 二维 DP (空间优化)     — O(len×m×n)   / O(m×n)       — 面试首选 ⭐
 *
 * 【与经典背包的对应】
 * 普通 0/1 背包:  dp[w] = max(dp[w], dp[w - weight[i]] + value[i])
 * 本题 (二维费用): dp[i][j] = max(dp[i][j], dp[i-zeros][j-ones] + 1)
 * 物品=字符串, 重量=(zeros,ones), 价值=1, 容量=(m,n)
 */

// ============================================================
// 解法1: 暴力递归 — 枚举每个字符串"选/不选"
// 时间: O(2^len × L)  空间: O(len) 递归栈
//
// 【思路】
// 最朴素的思考方式: 对每个字符串, 要么选要么不选.
// 如果选, 消耗对应的 0 和 1 预算, 子集大小 +1.
// 递归枚举所有可能, 取最大值.
// 瓶颈: 指数级时间复杂度, len=600 时完全不可行.
// 价值: 展示了"选/不选"的递归结构, 这是后续 DP 的基础.
// ============================================================

// 决策树 (假设 strs = ["10", "0", "1"], m=1, n=1):
//
//                        (idx=0, zeros_left=1, ones_left=1)
//                       /                              \
//              选 "10"                               不选 "10"
//     (idx=1, zl=0, ol=0)                    (idx=1, zl=1, ol=1)
//        |                                     /                \
//    不选"0"(预算不够也只能不选)          选"0"                  不选"0"
//   (idx=2, zl=0, ol=0)           (idx=2, zl=0, ol=1)    (idx=2, zl=1, ol=1)
//        |                           |          \            |          \
//    不选"1"                     不选"1"        选"1"?     不选"1"       选"1"
//   return 1                    return 1    (zl=0>=0?     return 0    return 1
//                                           ol=1>=1? YES)
//                                           return 2             最终答案 = 2

class Solution1 {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        return dfs(strs, 0, m, n);
    }
    
    int dfs(vector<string>& strs, int idx, int zeroLeft, int oneLeft) {
        // 终止条件: 所有字符串都考虑完了
        if (idx == (int)strs.size()) return 0;
        
        int zeros = count(strs[idx].begin(), strs[idx].end(), '0');
        int ones = (int)strs[idx].size() - zeros;
        
        // 选择1: 不选当前字符串
        int skip = dfs(strs, idx + 1, zeroLeft, oneLeft);
        
        // 选择2: 选当前字符串 (前提: 两种预算都够)
        int pick = 0;
        if (zeroLeft >= zeros && oneLeft >= ones) {
            pick = 1 + dfs(strs, idx + 1, zeroLeft - zeros, oneLeft - ones);
        }
        
        return max(skip, pick);
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 在暴力递归上加 memo
// 时间: O(len × m × n)  空间: O(len × m × n)
//
// 【思路】
// 暴力递归的瓶颈: 大量重复计算. 比如 "选第1个不选第2个" 和 
// "不选第1个选第2个" 到第3个字符串时, 如果剩余预算相同, 
// 后面的递归完全重复.
// 状态由 (idx, zeroLeft, oneLeft) 唯一确定 → 用 memo 缓存.
// 状态总数: len × (m+1) × (n+1) = 600 × 101 × 101 ≈ 6M, 可接受.
// ============================================================
class Solution2 {
public:
    // memo[idx][zeroLeft][oneLeft], -1 表示未计算
    int memo[601][101][101];
    
    int findMaxForm(vector<string>& strs, int m, int n) {
        memset(memo, -1, sizeof(memo));
        return dfs(strs, 0, m, n);
    }
    
    int dfs(vector<string>& strs, int idx, int zeroLeft, int oneLeft) {
        if (idx == (int)strs.size()) return 0;
        
        // 已经计算过, 直接返回缓存值
        if (memo[idx][zeroLeft][oneLeft] != -1)
            return memo[idx][zeroLeft][oneLeft];
        
        int zeros = count(strs[idx].begin(), strs[idx].end(), '0');
        int ones = (int)strs[idx].size() - zeros;
        
        int skip = dfs(strs, idx + 1, zeroLeft, oneLeft);
        int pick = 0;
        if (zeroLeft >= zeros && oneLeft >= ones) {
            pick = 1 + dfs(strs, idx + 1, zeroLeft - zeros, oneLeft - ones);
        }
        
        // 缓存并返回
        return memo[idx][zeroLeft][oneLeft] = max(skip, pick);
    }
};

// ============================================================
// 解法3: 三维 DP (递推) — 将记忆化搜索翻译成填表
// 时间: O(len × m × n)  空间: O(len × m × n)
//
// 【思路】
// 状态定义: dp[k][i][j] = 从前 k 个字符串中选, 
//           最多 i 个 0 和 j 个 1 时, 最大子集大小.
// 转移:
//   dp[k][i][j] = dp[k-1][i][j]                       // 不选第k个
//   dp[k][i][j] = max(上面, dp[k-1][i-z][j-o] + 1)    // 选第k个 (如果预算够)
// 初始化: dp[0][i][j] = 0 (不选任何字符串, 子集为空)
// 答案: dp[len][m][n]
//
// 填表方向和依赖关系:
//   dp[k][i][j] 依赖 dp[k-1][i][j] 和 dp[k-1][i-z][j-o]
//   → dp[k] 只依赖 dp[k-1] → 可以压缩掉第一维!
//
//   k维: 0 → len (正序, 因为依赖 k-1)
//   i维: 0 → m   (任意序, 因为依赖的 i-z ≤ i, 但这里不压缩所以无所谓)
//   j维: 0 → n   (同上)
// ============================================================
class Solution3 {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        vector<vector<vector<int>>> dp(len + 1, 
            vector<vector<int>>(m + 1, vector<int>(n + 1, 0)));
        
        for (int k = 1; k <= len; k++) {
            int zeros = count(strs[k-1].begin(), strs[k-1].end(), '0');
            int ones = (int)strs[k-1].size() - zeros;
            
            for (int i = 0; i <= m; i++) {
                for (int j = 0; j <= n; j++) {
                    // 不选第k个字符串: 继承上一轮的值
                    dp[k][i][j] = dp[k-1][i][j];
                    // 选第k个字符串: 需要预算够
                    if (i >= zeros && j >= ones) {
                        dp[k][i][j] = max(dp[k][i][j], 
                                          dp[k-1][i - zeros][j - ones] + 1);
                    }
                }
            }
        }
        
        return dp[len][m][n];
    }
};

// ============================================================
// 解法4: 二维 DP (空间优化) — 面试首选 ⭐
// 时间: O(len × m × n)  空间: O(m × n)
//
// 【思路】
// 从解法3我们看到 dp[k] 只依赖 dp[k-1], 所以压缩掉物品维度,
// 只保留一个 (m+1) × (n+1) 的二维数组.
//
// 关键: 为什么要倒序遍历 i 和 j?
// 这是 0/1 背包的核心技巧:
//   - 正序: dp[i-z][j-o] 可能已经在"本轮"被更新过
//           → 等于用了"选了当前物品后"的值
//           → 同一件物品可能被选多次 → 变成了完全背包!
//   - 倒序: dp[i-z][j-o] 一定是"上一轮"的旧值
//           → 保证每件物品最多选一次 → 正确的 0/1 背包!
//
// 填表方向 (对每个物品):
//   i: m → zeros (倒序)
//   j: n → ones  (倒序)
//
// 示例 dp 变化过程 (strs=["10","0","1"], m=1, n=1):
//
// 初始 dp:          处理"10"(z=1,o=1):   处理"0"(z=1,o=0):   处理"1"(z=0,o=1):
//   j→ 0  1           j→ 0  1              j→ 0  1              j→ 0  1
// i=0  0  0         i=0  0  0            i=0  0  0            i=0  0  1  ← 选"1"
// i=1  0  0         i=1  0  1  ← 选"10" i=1  1  1  ← 选"0"  i=1  1  2  ← 最终答案!
//                                        (dp[1][0]=max(0,dp[0][0]+1)=1)
// ============================================================
class Solution4 {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        // dp[i][j]: 最多 i 个 0 和 j 个 1 时, 能选的最大字符串数量
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (const string& s : strs) {
            // 预处理当前字符串的 0/1 数量 (这就是"物品的两维重量")
            int zeros = count(s.begin(), s.end(), '0');
            int ones = (int)s.size() - zeros;
            
            // 0/1 背包: 倒序遍历两个容量维度
            // 为什么从 m 到 zeros 而不是到 0? 
            // 因为 i < zeros 时无法选当前物品, dp[i][j] 不会变, 跳过即可
            for (int i = m; i >= zeros; i--) {
                for (int j = n; j >= ones; j--) {
                    // max(不选当前字符串, 选当前字符串)
                    dp[i][j] = max(dp[i][j], dp[i - zeros][j - ones] + 1);
                }
            }
        }
        
        return dp[m][n];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间          | 空间          | 适用场景            |
// |----------|---------------|---------------|---------------------|
// | 暴力递归 | O(2^len)      | O(len)        | 理解递归结构        |
// | 记忆化   | O(len×m×n)    | O(len×m×n)    | 自顶向下思维更自然时|
// | 三维DP   | O(len×m×n)    | O(len×m×n)    | 需要回溯选了哪些时  |
// | 二维DP   | O(len×m×n)    | O(m×n)        | 面试首选 ⭐         |
//
// 解法1→2: 发现重叠子问题, 加 memo 缓存
// 解法2→3: 递归改递推, 逻辑完全对应
// 解法3→4: dp[k] 只依赖 dp[k-1], 压缩物品维度 + 倒序遍历
//
// ============================================================
// 【易错点】
//
// 1. 正序遍历导致物品重复选:
//    ✗ for (int i = zeros; i <= m; i++)  → 完全背包, 物品可选多次
//    ✓ for (int i = m; i >= zeros; i--)  → 0/1 背包, 每件物品最多选一次
//
// 2. dp 初始化为非零值:
//    ✗ dp[i][j] = -1 或 INT_MIN
//    ✓ dp[i][j] = 0  → 不选任何字符串时子集大小为 0
//
// 3. 循环下界写成 0 导致数组越界:
//    ✗ for (int i = m; i >= 0; i--)  → 当 i < zeros 时 dp[i-zeros] 下标为负
//    ✓ for (int i = m; i >= zeros; i--)  → 安全且跳过无意义计算
//
// 4. 混淆 zeros 和 ones:
//    ✗ int zeros = count(s.begin(), s.end(), '1');  → 统计的是 1 的个数!
//    ✓ int zeros = count(s.begin(), s.end(), '0');
//       int ones = s.size() - zeros;
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1(基础): "这道题和经典 0/1 背包有什么关系?"
//    → 本质相同. 经典背包是一维容量(重量≤W), 本题是二维容量(0≤m, 1≤n).
//      dp 从一维扩展到二维, "选/不选"逻辑和倒序遍历技巧完全一致.
//
// Q2(变体): "如果每个字符串可以选无限次, 怎么改?"
//    → 变成二维完全背包. 只需把倒序改正序:
//       for (int i = zeros; i <= m; i++)
//         for (int j = ones; j <= n; j++)
//      正序使得 dp[i-z][j-o] 是"本轮已更新"的值, 允许重复选.
//
// Q3(追踪): "能否输出具体选了哪些字符串?"
//    → 需要三维 DP (不压缩空间). 从 dp[len][m][n] 回溯:
//      若 dp[k][i][j] != dp[k-1][i][j], 说明第 k 个被选了,
//      跳到 dp[k-1][i-zeros_k][j-ones_k] 继续. 否则跳到 dp[k-1][i][j].
//
// Q4(扩展): "如果 m 和 n 非常大 (比如 10^6), 但 strs 很短, 怎么优化?"
//    → dp 数组太大放不下. 考虑用 map 或稀疏表示, 
//      只存有实际值变化的 (i,j) 对. 或者换回记忆化搜索, 
//      只计算实际被访问到的状态.
//
// 【相关题型】
// - 416. 分割等和子集 — 0/1 背包(一维容量, dp值为bool), 本题的简化版
// - 494. 目标和 — 0/1 背包变体(求方案数), dp 转移用 += 而不是 max
// - 322. 零钱兑换 — 完全背包(正序遍历), 对比本题的倒序
// - 518. 零钱兑换II — 完全背包求方案数, 和本题"0/1 vs 完全"对比学习
// - 879. 盈利计划 — 同为二维费用背包, 难度更高, 直接复用本题模板
// ============================================================
