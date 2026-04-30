/*
 * LeetCode 97: Interleaving String (交错字符串)
 * 
 * 【题目本质】
 * 给定 s1, s2, s3，判断 s3 是否由 s1 和 s2 交错组成。
 * 等价于：在 (m+1)×(n+1) 的网格上，从 (0,0) 走到 (m,n)，
 * 每步向右(消耗s2字符)或向下(消耗s1字符)，路径字符拼接恰好是 s3。
 *
 * 【解法总览】
 * 解法1: 暴力递归         — O(2^(m+n)) / O(m+n) — 最直觉，理解问题
 * 解法2: 记忆化搜索       — O(m·n) / O(m·n)     — 发现重叠子问题后优化
 * 解法3: 二维 DP (递推)   — O(m·n) / O(m·n)     — ⭐ 面试首选
 * 解法4: 一维 DP (空间优化) — O(m·n) / O(n)     — Follow-up: O(s2.length) 空间
 */

// ============================================================
// 解法1: 暴力递归 — 穷举每步取 s1 还是 s2
// 时间: O(2^(m+n))  空间: O(m+n) 递归栈
//
// 【思路】
// 最朴素的想法：s3 的第 k 个字符要么来自 s1[i]，要么来自 s2[j]。
// 如果匹配就尝试递归，两路中有一路成功即可。
// 瓶颈：当 s1[i] == s2[j] == s3[k] 时两个分支都要走，指数爆炸。
//
// 决策树示例 (s1="ab", s2="ac", s3="aabc"):
//              (0,0)  s3[0]='a'
//            /        \
//     取s1[0]='a'   取s2[0]='a'
//       (1,0)          (0,1)
//      s3[1]='a'     s3[1]='a'
//       / \            / \
//   (2,0) (1,1)   (1,1) (0,2)    ← 注意 (1,1) 被重复计算!
//   ...   ...     ...   ...
// ============================================================
class Solution1 {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        return dfs(s1, s2, s3, 0, 0);
    }

private:
    bool dfs(string& s1, string& s2, string& s3, int i, int j) {
        int k = i + j;
        if (k == (int)s3.size()) return true;

        // 尝试从 s1 取第 i 个字符
        if (i < (int)s1.size() && s1[i] == s3[k] && dfs(s1, s2, s3, i + 1, j))
            return true;
        // 尝试从 s2 取第 j 个字符
        if (j < (int)s2.size() && s2[j] == s3[k] && dfs(s1, s2, s3, i, j + 1))
            return true;

        return false;
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 递归 + memo 消除重叠子问题
// 时间: O(m·n)  空间: O(m·n)
//
// 【思路】
// 解法1中 (i,j) 状态会被重复访问（如上面决策树中的 (1,1)）。
// 状态总数只有 O(m·n) 个，用 memo[i][j] 缓存结果。
// 关键：k = i+j 由 (i,j) 唯一确定，所以状态只需两个维度。
// memo 要用三值标记：0=未访问, 1=true, -1=false。
// 不能用 bool，否则无法区分"没算过"和"算过结果为false"。
// ============================================================
class Solution2 {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;

        // 三值 memo: 0=未访问, 1=true, -1=false
        vector<vector<int>> memo(m + 1, vector<int>(n + 1, 0));
        return dfs(s1, s2, s3, 0, 0, memo);
    }

private:
    bool dfs(string& s1, string& s2, string& s3, int i, int j,
             vector<vector<int>>& memo) {
        int k = i + j;
        if (k == (int)s3.size()) return true;
        if (memo[i][j] != 0) return memo[i][j] == 1; // 已经算过

        bool res = false;
        if (i < (int)s1.size() && s1[i] == s3[k])
            res = dfs(s1, s2, s3, i + 1, j, memo);
        if (!res && j < (int)s2.size() && s2[j] == s3[k])
            res = dfs(s1, s2, s3, i, j + 1, memo);

        memo[i][j] = res ? 1 : -1;
        return res;
    }
};

// ============================================================
// 解法3: 二维 DP (递推) — ⭐ 面试首选
// 时间: O(m·n)  空间: O(m·n)
//
// 【思路】
// 把记忆化搜索翻译成自底向上的填表。
// 状态定义: dp[i][j] = s1 的前 i 个字符和 s2 的前 j 个字符
//           能否交错组成 s3 的前 (i+j) 个字符。
//
// 转移方程:
//   dp[i][j] = (dp[i-1][j] && s1[i-1] == s3[i+j-1])   // 最后一个字符来自 s1
//           || (dp[i][j-1] && s2[j-1] == s3[i+j-1])    // 最后一个字符来自 s2
//
// 填表方向和依赖关系:
//
//        s2:   ""   d    b    b    c    a
//    s1:  j:    0    1    2    3    4    5
//    ""  0:  [ T    F    F    F    F    F ]
//     a  1:  [ T    F    F    F    F    F ]
//     a  2:  [ T    T    T    T    T    F ]
//     b  3:  [ F    T    T    F    T    F ]
//     c  4:  [ F    F    T    T    T    T ]
//     c  5:  [ F    F    F    T    F    T ] ← dp[5][5] = true ✓
//
//  依赖: dp[i][j] ← dp[i-1][j] (上方)
//                  ← dp[i][j-1] (左方)
//  填表: 从上到下, 每行从左到右
// ============================================================
class Solution3 {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;

        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;

        // 第一列: 只用 s1 拼 s3
        // 链式: 一旦断开(不匹配), 后面全是 false
        for (int i = 1; i <= m; i++)
            dp[i][0] = dp[i - 1][0] && (s1[i - 1] == s3[i - 1]);

        // 第一行: 只用 s2 拼 s3
        for (int j = 1; j <= n; j++)
            dp[0][j] = dp[0][j - 1] && (s2[j - 1] == s3[j - 1]);

        // 填表: 从上到下, 从左到右
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                // s3 当前位置 k = i+j-1 (0-indexed)
                // 要么这个字符来自 s1[i-1]（从上方转移）
                // 要么这个字符来自 s2[j-1]（从左方转移）
                dp[i][j] = (dp[i - 1][j] && s1[i - 1] == s3[i + j - 1])
                         || (dp[i][j - 1] && s2[j - 1] == s3[i + j - 1]);
            }
        }

        return dp[m][n];
    }
};

// ============================================================
// 解法4: 一维 DP (空间优化) — Follow-up: O(s2.length) 空间
// 时间: O(m·n)  空间: O(n)
//
// 【思路】
// 解法3中 dp[i][j] 只依赖:
//   - dp[i-1][j]: 上一行同列 → 在一维数组中, dp[j] 更新前就是上一行的值
//   - dp[i][j-1]: 本行前一列 → 在一维数组中, dp[j-1] 已经是本行更新后的值
//
// 所以从左到右遍历 j, dp[j] 天然对应正确的依赖关系。
//
// 空间优化示意:
//   二维: dp[i][j] = dp[i-1][j]     ||  dp[i][j-1]
//                     ↑ 上方              ↑ 左方
//   一维: dp[j]新    = dp[j]旧(上方)  ||  dp[j-1]新(左方)
//
// 注意: 每轮 i 循环开始时要更新 dp[0] = dp[i][0]
//       dp[0] 代表"只用 s1 前 i 个字符", 也有链式依赖
// ============================================================
class Solution4 {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;

        // 让较短的作为 s2 可以进一步节省空间到 O(min(m,n))
        // 这里为了简洁不做交换, 但面试中可以提及

        vector<bool> dp(n + 1, false);
        dp[0] = true;

        // 初始化第一行 (i=0): 只用 s2
        for (int j = 1; j <= n; j++)
            dp[j] = dp[j - 1] && (s2[j - 1] == s3[j - 1]);

        for (int i = 1; i <= m; i++) {
            // 更新 dp[0]: 对应 dp[i][0], 只用 s1 前 i 个字符
            // 易错: 忘记更新这一行, dp[0] 永远是 true
            dp[0] = dp[0] && (s1[i - 1] == s3[i - 1]);

            for (int j = 1; j <= n; j++) {
                // dp[j] 更新前 = dp[i-1][j] (上方)
                // dp[j-1] 更新后 = dp[i][j-1] (左方)
                dp[j] = (dp[j] && s1[i - 1] == s3[i + j - 1])
                      || (dp[j - 1] && s2[j - 1] == s3[i + j - 1]);
            }
        }

        return dp[n];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间        | 空间      | 适用场景          |
// |------------|-------------|-----------|-------------------|
// | 暴力递归   | O(2^(m+n))  | O(m+n)    | 理解问题, TLE     |
// | 记忆化搜索 | O(m·n)      | O(m·n)    | 递归思路清晰      |
// | 二维 DP    | O(m·n)      | O(m·n)    | ⭐ 面试首选        |
// | 一维 DP    | O(m·n)      | O(n)      | Follow-up 加分    |
//
// 解法1 → 解法2: 加 memo 消除重叠子问题, 从指数降到多项式
// 解法2 → 解法3: 递归翻译成循环, 消除栈开销, 逻辑等价
// 解法3 → 解法4: 利用依赖关系压缩空间, 从 O(m·n) 到 O(n)
//
// 【易错点】
// 1. 忘记 s1.size() + s2.size() != s3.size() 的提前判断
//    → 不判断会导致数组越界或错误结果
//
// 2. s3 索引写成 s3[i+j] 而不是 s3[i+j-1]
//    → dp 的 (i,j) 是 1-indexed("前i个"), s3 是 0-indexed
//    → 第 i+j 个字符的 0-indexed 下标是 i+j-1
//
// 3. 一维 DP 漏更新 dp[0]
//    → dp[0] 对应 dp[i][0]（只用 s1 的情况）
//    → 必须每轮 i 循环时 dp[0] = dp[0] && (s1[i-1] == s3[i-1])
//
// 4. 记忆化用 vector<vector<bool>> 导致无法区分"未访问"和"false"
//    → 应用 int 三值: 0=未访问, 1=true, -1=false
//
// 5. 初始化第一行/第一列时忘记链式依赖
//    → ✗ dp[i][0] = (s1[i-1] == s3[i-1])
//    → ✓ dp[i][0] = dp[i-1][0] && (s1[i-1] == s3[i-1])
//    → 前面不匹配, 后面即使匹配也不行
//
// 【面试追问 — 递进链】
//
// Q1(基础): 暴力为什么是指数级? 重叠子问题在哪里?
//    → 当 s1[i]==s2[j]==s3[k] 时两个分支都走, 但 (i+1,j) 和 (i,j+1) 后续
//      可能都经过同一个 (i+1,j+1), 这就是重叠。状态总数 O(m·n), 远小于 2^(m+n)。
//
// Q2(优化): 为什么空间可以压缩到一维?
//    → dp[i][j] 只依赖 dp[i-1][j] (正上方) 和 dp[i][j-1] (正左方)。
//      一维滚动数组中, dp[j] 未更新前就是上一行的值, dp[j-1] 已更新就是本行左边。
//
// Q3(进阶): 能否做到 O(min(m,n)) 空间?
//    → 可以。交错的定义对 s1, s2 是对称的 (交换 s1 s2 结果不变)。
//      让较短的做内层循环(对应 dp 的列), 空间就是 O(min(m,n))。
//
// Q4(变体): 如果要输出所有交错方式, 怎么做?
//    → 回溯 + memo 剪枝。DFS 过程记录路径(每步选 s1 还是 s2), 到终点收集。
//      memo[i][j]=false 的状态直接跳过, 避免无效搜索。
// ============================================================
