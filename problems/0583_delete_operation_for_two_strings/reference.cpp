/*
 * LeetCode 583: 两个字符串的删除操作 (Delete Operation for Two Strings)
 * 
 * 【题目本质】
 * 两个字符串各删除若干字符使之相同，求最小删除步数。
 * 等价于：保留最长公共子序列(LCS)，删除其余部分。
 * 答案 = len(word1) + len(word2) - 2 * LCS长度。
 *
 * 【解法总览】
 * 解法1: 暴力递归           — O(2^(m+n)) / O(m+n)    — 理解递归结构
 * 解法2: 记忆化搜索         — O(mn) / O(mn)          — 消除重叠子问题
 * 解法3: 直接DP(最小删除数) — O(mn) / O(mn)          — 面试首选
 * 解法4: LCS转化DP          — O(mn) / O(mn)          — 另一视角
 * 解法5: 空间优化DP          — O(mn) / O(min(m,n))   — 加分项
 *
 * 【系列题关系】
 * - 1143 LCS: 本题的核心子问题，解法4直接复用
 * - 72 编辑距离: 本题只有"删除"，72多了"替换"，转移多一个分支
 * - 712 最小ASCII删除和: +1 改为 +ASCII值
 */

// ============================================================
// 解法1: 暴力递归 — 直接递归尝试删与不删
// 时间: O(2^(m+n))  空间: O(m+n) 递归栈
//
// 【思路】
// 比较 word1[i] 和 word2[j]:
// - 相同：同时保留，前进到 (i+1, j+1)
// - 不同：要么删 word1[i] (花费1步，前进到 i+1, j)
//         要么删 word2[j] (花费1步，前进到 i, j+1)
// base case: 一方用完，另一方剩余字符全部要删
//
// 为什么会超时？大量重叠子问题:
//   dfs(1,2) 可以从 dfs(0,2) 和 dfs(1,1) 同时递归到
// ============================================================
class Solution1 {
public:
    int minDistance(string word1, string word2) {
        return dfs(word1, word2, 0, 0);
    }

private:
    int dfs(string& w1, string& w2, int i, int j) {
        // base case: word1用完 → 删掉word2剩余所有字符
        if (i == (int)w1.size()) return (int)w2.size() - j;
        // base case: word2用完 → 删掉word1剩余所有字符
        if (j == (int)w2.size()) return (int)w1.size() - i;

        if (w1[i] == w2[j]) {
            // 字符相同，都保留，不花费删除步数
            return dfs(w1, w2, i + 1, j + 1);
        } else {
            // 不同，尝试两种删除，取最小
            return 1 + min(dfs(w1, w2, i + 1, j),    // 删 word1[i]
                           dfs(w1, w2, i, j + 1));    // 删 word2[j]
        }
    }
};

// ============================================================
// 解法2: 记忆化搜索(自顶向下DP) — 给解法1加缓存
// 时间: O(mn)  空间: O(mn)
//
// 【思路】
// 状态 (i,j) 总共 m×n 种，每种只算一次。
// 加一个 memo[i][j] 缓存结果，从指数级降到多项式。
// ============================================================
class Solution2 {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> memo(m, vector<int>(n, -1));
        return dfs(word1, word2, 0, 0, memo);
    }

private:
    int dfs(string& w1, string& w2, int i, int j, vector<vector<int>>& memo) {
        if (i == (int)w1.size()) return (int)w2.size() - j;
        if (j == (int)w2.size()) return (int)w1.size() - i;

        if (memo[i][j] != -1) return memo[i][j];

        if (w1[i] == w2[j]) {
            memo[i][j] = dfs(w1, w2, i + 1, j + 1, memo);
        } else {
            memo[i][j] = 1 + min(dfs(w1, w2, i + 1, j, memo),
                                  dfs(w1, w2, i, j + 1, memo));
        }
        return memo[i][j];
    }
};

// ============================================================
// 解法3: 直接DP(递推，自底向上) — 面试首选
// 时间: O(mn)  空间: O(mn)
//
// 【思路】
// dp[i][j] = 使 word1 前i个字符 和 word2 前j个字符 变相同的最小删除步数
//
// 状态转移:
//   word1[i-1] == word2[j-1]: dp[i][j] = dp[i-1][j-1]
//       → 匹配! 这两个字符都保留，不增加删除步数
//   word1[i-1] != word2[j-1]: dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + 1
//       → 不匹配，删 word1[i-1] 或删 word2[j-1]
//
// 初始化:
//   dp[0][j] = j  (word1为空，word2需要删掉j个字符)
//   dp[i][0] = i  (word2为空，word1需要删掉i个字符)
//
// dp 填表过程 (word1="sea", word2="eat"):
//
//        ""  e  a  t
//   ""  [ 0  1  2  3 ]   ← dp[0][j] = j
//    s  [ 1  2  3  4 ]   's'≠'e','a','t' → 都是 min(上,左)+1
//    e  [ 2  1  2  3 ]   'e'=='e' → dp[2][1]=dp[1][0]=1
//    a  [ 3  2  1  2 ]   'a'=='a' → dp[3][2]=dp[2][1]=1
//                   ↑ 答案=2
//
// dp[i][j] 的依赖关系:
//   dp[i-1][j-1]  dp[i-1][j]
//        ↘            ↓
//   dp[i][j-1]  → dp[i][j]
// ============================================================
class Solution3 {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        // 初始化: 一方为空时，另一方全部删除
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        for (int i = 0; i <= m; i++) dp[i][0] = i;

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    // 匹配：不需要删，继承左上角的结果
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    // 不匹配：删 word1[i-1] 或 word2[j-1]，取最小+1
                    dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1;
                }
            }
        }
        return dp[m][n];
    }
};

// ============================================================
// 解法4: LCS转化 — 先求最长公共子序列长度，再算删除步数
// 时间: O(mn)  空间: O(mn)
//
// 【思路】
// 核心观察: 删除最少 ↔ 保留最多的公共部分
// 保留的公共部分 = 最长公共子序列 (LCS)
// 答案 = m + n - 2 * LCS长度
//
// LCS 的 DP:
//   lcs[i][j] = word1前i个 与 word2前j个 的LCS长度
//   word1[i-1]==word2[j-1]: lcs[i][j] = lcs[i-1][j-1] + 1  (匹配,LCS+1)
//   否则:                   lcs[i][j] = max(lcs[i-1][j], lcs[i][j-1])
//
// LCS 填表 (word1="sea", word2="eat"):
//
//        ""  e  a  t
//   ""  [ 0  0  0  0 ]
//    s  [ 0  0  0  0 ]
//    e  [ 0  1  1  1 ]   'e'=='e' → lcs=0+1=1
//    a  [ 0  1  2  2 ]   'a'=='a' → lcs=1+1=2
//                   ↑ LCS=2, 答案=3+3-2*2=2
//
// 注意和解法3转移方向的区别:
//   解法3: 匹配→不+1, 不匹配→min+1
//   解法4: 匹配→+1,   不匹配→max不+1
// ============================================================
class Solution4 {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> lcs(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    // 匹配: LCS 长度 +1
                    lcs[i][j] = lcs[i - 1][j - 1] + 1;
                } else {
                    // 不匹配: 取跳过 word1[i-1] 或跳过 word2[j-1] 的较大值
                    lcs[i][j] = max(lcs[i - 1][j], lcs[i][j - 1]);
                }
            }
        }
        // 需要删除的 = 两串总长 - 保留的公共部分 × 2
        return m + n - 2 * lcs[m][n];
    }
};

// ============================================================
// 解法5: 空间优化DP(滚动数组) — 将解法3压缩到一维
// 时间: O(mn)  空间: O(min(m,n))
//
// 【思路】
// dp[i][j] 只依赖 dp[i-1][j-1](左上), dp[i-1][j](正上), dp[i][j-1](左)
// 所以只需一行数组 + 一个 prev 变量保存"左上角"
//
// 滚动过程示意 (处理第 i 行):
//   更新前: dp[j] 存的是 dp[i-1][j] (上一行的值)
//   prev 保存 dp[i-1][j-1] (更新 dp[j] 之前先暂存)
//   更新后: dp[j] 变成 dp[i][j] (当前行的值)
//
// 让较短的串做列，进一步优化空间到 O(min(m,n))
// ============================================================
class Solution5 {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        // 让较短的串作为内层循环，节省空间
        if (m < n) return minDistance(word2, word1);

        vector<int> dp(n + 1);
        // 初始化第0行: dp[j] = j
        for (int j = 0; j <= n; j++) dp[j] = j;

        for (int i = 1; i <= m; i++) {
            int prev = dp[0]; // prev = dp[i-1][0]，即左上角初始值
            dp[0] = i;        // dp[i][0] = i (word2为空，word1删i个)

            for (int j = 1; j <= n; j++) {
                int temp = dp[j]; // 暂存 dp[i-1][j]，下一轮当左上角

                if (word1[i - 1] == word2[j - 1]) {
                    dp[j] = prev;  // dp[i-1][j-1]，匹配不加删除
                } else {
                    // dp[j] 此时还是 dp[i-1][j] (上)
                    // dp[j-1] 已更新为 dp[i][j-1] (左)
                    dp[j] = min(dp[j], dp[j - 1]) + 1;
                }
                prev = temp; // 更新 prev 为刚才保存的 dp[i-1][j]
            }
        }
        return dp[n];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间   | 空间        | 特点                          |
// |------|--------|-------------|-------------------------------|
// | 1    | 指数级 | O(m+n)      | 理解递归结构，面试能讲即可      |
// | 2    | O(mn)  | O(mn)       | 加 memo，递归转DP的过渡        |
// | 3    | O(mn)  | O(mn)       | ⭐ 面试首选，直接定义删除步数   |
// | 4    | O(mn)  | O(mn)       | ⭐ LCS视角，思路优雅           |
// | 5    | O(mn)  | O(min(m,n)) | 空间最优，展示DP优化功底        |
//
// 解法3 vs 解法4:
//   - 解法3 直接求"最小删除数"，转移: 匹配不+1, 不匹配min+1
//   - 解法4 先求"LCS长度"，转移: 匹配+1, 不匹配max → 再算答案
//   - 本质相同，只是 DP 状态定义不同。面试选自己更顺手的。
//
// ============================================================
// 【易错点】
//
// 1. 下标偏移:
//    ✗ word1[i] == word2[j]  (dp 下标从1开始，但字符串从0开始)
//    ✓ word1[i-1] == word2[j-1]
//
// 2. 初始化:
//    ✗ dp[0][j] = 0  (忘了空串匹配需要删除)
//    ✓ dp[0][j] = j, dp[i][0] = i
//
// 3. 解法3和解法4的转移搞混:
//    ✗ 解法3(直接DP)里写 dp[i][j] = dp[i-1][j-1] + 1  (匹配时不应+1)
//    ✓ 解法3匹配时 dp[i][j] = dp[i-1][j-1] (保留不删，不+1)
//    ✓ 解法4匹配时 lcs[i][j] = lcs[i-1][j-1] + 1 (LCS长度+1)
//
// 4. 空间优化忘记保存左上角:
//    ✗ dp[j] = prev; 但 prev 在循环中没有正确更新
//    ✓ 每次先 temp=dp[j], 更新dp[j]后, prev=temp
//
// ============================================================
// 【面试追问】
//
// Q1(基础): "暴力递归为什么是指数级？重叠子问题在哪？"
// → 递归树中 dfs(i,j) 会被多条路径到达。比如 dfs(2,3) 可以从
//   dfs(1,3) 删 word1 和 dfs(2,2) 删 word2 同时递归到。
//   总状态数 O(mn)，但不加缓存会重复计算指数次。
//
// Q2(优化): "这题和 LCS(1143) 是什么关系？"
// → 答案 = m+n-2*LCS。删除最少 ↔ 保留最多的公共子序列。
//   这是同一问题的对偶视角。
//
// Q3(空间): "能否 O(n) 空间？"
// → 可以。dp[i][j] 只依赖上一行同列(上)、上一行前一列(左上)、
//   当前行前一列(左)。用一维数组+prev变量模拟即可。
//
// Q4(变体): "如果改成编辑距离(72题)，代码改哪里？"
// → 不匹配时多一个选择: 替换 = dp[i-1][j-1]+1。
//   即 dp[i][j] = min(dp[i-1][j]+1, dp[i][j-1]+1, dp[i-1][j-1]+1)
//   本题只有前两项（只能删除不能替换）。
//
// Q5(变体): "如果不是最小步数，而是最小ASCII删除和(712题)？"
// → 把 +1 改为 +word1[i-1] 或 +word2[j-1] 的 ASCII 值。
//   初始化也要累加 ASCII 值而不是简单的 +1。
// ============================================================
