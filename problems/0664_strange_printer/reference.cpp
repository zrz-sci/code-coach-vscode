/*
 * LeetCode 664: 奇怪的打印机 (Strange Printer)
 * 难度: Hard
 * 标签: String, Dynamic Programming
 *
 * 核心思路: 区间 DP
 * dp[i][j] = 打印 s[i..j] 这个子串所需的最少打印次数
 *
 * 解法一: Bottom-up 区间 DP (以首字符为基准) — O(n^3) / O(n^2)
 * 解法二: Bottom-up 区间 DP (以尾字符为基准) — O(n^3) / O(n^2)
 * 解法三: Top-down 记忆化搜索 — O(n^3) / O(n^2)
 */


/*
 * ============================================================
 * 解法一: Bottom-up 区间 DP (以首字符 s[i] 为基准)
 * ============================================================
 *
 * 思路:
 *   对于区间 s[i..j]:
 *   1. 默认: s[i] 单独打一次, dp[i][j] = dp[i+1][j] + 1
 *   2. 优化: 如果存在 m (i < m <= j) 使得 s[i] == s[m],
 *            则打印 s[i] 时可以延伸到 s[m], 合并打印
 *            dp[i][j] = min(dp[i][j], dp[i+1][m] + dp[m+1][j])
 *
 * 画 DP 表填充过程 (s = "aba"):
 *
 *   初始化 (长度=1): dp[0][0]=1, dp[1][1]=1, dp[2][2]=1
 *
 *   长度=2:
 *     dp[0][1]="ab": 默认 dp[1][1]+1=2, s[0]='a'!=s[1]='b' -> 2
 *     dp[1][2]="ba": 默认 dp[2][2]+1=2, s[1]='b'!=s[2]='a' -> 2
 *
 *   长度=3:
 *     dp[0][2]="aba": 默认 dp[1][2]+1=3
 *       m=2: s[0]='a'==s[2]='a' -> dp[1][2]+dp[3][2] = 2+0 = 2
 *     dp[0][2] = min(3, 2) = 2
 *
 *   最终 DP 表:
 *        j=0  j=1  j=2
 *   i=0 [ 1    2    2 ]
 *   i=1 [ -    1    2 ]
 *   i=2 [ -    -    1 ]
 *   答案 = dp[0][2] = 2
 *
 * 时间: O(n^3) — 三层循环
 * 空间: O(n^2) — DP 表
 */
class Solution_BottomUp_Front {
public:
    int strangePrinter(string s) {
        if (s.empty()) return 0;

        // 预处理: 去除连续重复字符
        // "aaabbb" -> "ab", 不影响答案但缩减状态数
        string t;
        for (char c : s) {
            if (t.empty() || t.back() != c) t += c;
        }
        s = t;

        int n = s.size();
        // dp[i][j] = 打印 s[i..j] 的最少次数
        vector<vector<int>> dp(n, vector<int>(n, 0));

        // Base case: 单个字符需要1次打印
        for (int i = 0; i < n; i++) {
            dp[i][i] = 1;
        }

        // 按区间长度从小到大填充
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;

                // 默认: s[i] 单独打印一次
                dp[i][j] = dp[i + 1][j] + 1;

                // 优化: 如果 s[i] == s[m], 可以合并打印
                for (int m = i + 1; m <= j; m++) {
                    if (s[i] == s[m]) {
                        int val = dp[i + 1][m]; // s[i] 延伸到 s[m], s[m] 免费
                        if (m + 1 <= j) {
                            val += dp[m + 1][j]; // 处理 s[m+1..j]
                        }
                        dp[i][j] = min(dp[i][j], val);
                    }
                }
            }
        }

        return dp[0][n - 1];
    }
};


/*
 * ============================================================
 * 解法二: Bottom-up 区间 DP (以尾字符 s[j] 为基准)
 * ============================================================
 *
 * 思路 (等价写法, 很多题解使用这种):
 *   对于区间 s[i..j]:
 *   1. 默认: s[j] 单独打一次, dp[i][j] = dp[i][j-1] + 1
 *   2. 优化: 如果存在 k (i <= k < j) 使得 s[k] == s[j],
 *            则打印 s[k] 时可以延伸到 s[j], 合并打印
 *            dp[i][j] = min(dp[i][j], dp[i][k] + dp[k+1][j-1])
 *
 *   特殊情况: 当 s[i] == s[j] 时,
 *     k=i: dp[i][i] + dp[i+1][j-1] = 1 + dp[i+1][j-1]
 *     但更直接: dp[i][j] = dp[i][j-1]
 *     (打印 s[i] 时延伸到 s[j], s[j] 免费, 等于不需要处理 s[j])
 *
 * 画 DP 表填充过程 (s = "abab"):
 *
 *   初始化: dp[0][0]=dp[1][1]=dp[2][2]=dp[3][3]=1
 *
 *   长度=2:
 *     dp[0][1]="ab": dp[0][0]+1=2, s[0]='a'!=s[1]='b' -> 2
 *     dp[1][2]="ba": dp[1][1]+1=2, s[1]='b'!=s[2]='a' -> 2
 *     dp[2][3]="ab": dp[2][2]+1=2, s[2]='a'!=s[3]='b' -> 2
 *
 *   长度=3:
 *     dp[0][2]="aba": dp[0][1]+1=3
 *       k=0: s[0]='a'==s[2]='a' -> dp[0][0]+dp[1][1] = 1+1 = 2
 *       dp[0][2] = 2
 *     dp[1][3]="bab": dp[1][2]+1=3
 *       k=1: s[1]='b'==s[3]='b' -> dp[1][1]+dp[2][2] = 1+1 = 2
 *       dp[1][3] = 2
 *
 *   长度=4:
 *     dp[0][3]="abab": dp[0][2]+1=3
 *       k=0: s[0]='a'==s[3]='b'? NO
 *       k=1: s[1]='b'==s[3]='b' -> dp[0][1]+dp[2][2] = 2+1 = 3
 *       k=2: s[2]='a'==s[3]='b'? NO
 *       dp[0][3] = min(3, 3) = 3
 *
 *   答案 = dp[0][3] = 3
 *
 * 时间: O(n^3), 空间: O(n^2)
 */
class Solution_BottomUp_Back {
public:
    int strangePrinter(string s) {
        if (s.empty()) return 0;

        // 去重预处理
        string t;
        for (char c : s) {
            if (t.empty() || t.back() != c) t += c;
        }
        s = t;

        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));

        for (int i = 0; i < n; i++) {
            dp[i][i] = 1;
        }

        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;

                // 默认: s[j] 单独打印一次
                dp[i][j] = dp[i][j - 1] + 1;

                // 优化: 如果 s[k] == s[j], 打印 s[k] 时延伸覆盖 s[j]
                for (int k = i; k < j; k++) {
                    if (s[k] == s[j]) {
                        // dp[i][k]: 打印 s[i..k]
                        // dp[k+1][j-1]: 打印 s[k+1..j-1] (s[j] 被 s[k] 的延伸覆盖)
                        int val = dp[i][k];
                        if (k + 1 <= j - 1) {
                            val += dp[k + 1][j - 1];
                        }
                        dp[i][j] = min(dp[i][j], val);
                    }
                }
            }
        }

        return dp[0][n - 1];
    }
};


/*
 * ============================================================
 * 解法三: Top-down 记忆化搜索
 * ============================================================
 *
 * 思路完全相同，只是用递归+记忆化代替 bottom-up 循环。
 * 优点: 代码更直觉，不需要考虑遍历顺序。
 * 缺点: 递归栈开销，但 n<=100 不是问题。
 *
 * 时间: O(n^3), 空间: O(n^2)
 */
class Solution_TopDown {
public:
    int strangePrinter(string s) {
        if (s.empty()) return 0;

        // 去重预处理
        string t;
        for (char c : s) {
            if (t.empty() || t.back() != c) t += c;
        }
        s = t;

        int n = s.size();
        vector<vector<int>> memo(n, vector<int>(n, -1));
        return dfs(s, 0, n - 1, memo);
    }

private:
    int dfs(const string& s, int i, int j, vector<vector<int>>& memo) {
        // Base case: 空区间
        if (i > j) return 0;

        // Base case: 单个字符
        if (i == j) return 1;

        // 已计算过
        if (memo[i][j] != -1) return memo[i][j];

        // 默认: s[i] 单独打印一次
        int result = dfs(s, i + 1, j, memo) + 1;

        // 优化: 找到 s[m] == s[i] 的位置，合并打印
        for (int m = i + 1; m <= j; m++) {
            if (s[i] == s[m]) {
                // s[i] 的打印延伸到 s[m]，s[m] 免费
                result = min(result, dfs(s, i + 1, m, memo) + dfs(s, m + 1, j, memo));
            }
        }

        memo[i][j] = result;
        return result;
    }
};


/*
 * ============================================================
 * 测试用例
 * ============================================================
 */
int main() {
    Solution_BottomUp_Front sol1;
    Solution_BottomUp_Back  sol2;
    Solution_TopDown        sol3;

    // --- 基本测试 ---
    cout << "=== Test 1: s = \"aaabbb\" ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("aaabbb") << endl;   // 2
    cout << "Back:    " << sol2.strangePrinter("aaabbb") << endl;   // 2
    cout << "TopDown: " << sol3.strangePrinter("aaabbb") << endl;   // 2

    cout << "\n=== Test 2: s = \"aba\" ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("aba") << endl;      // 2
    cout << "Back:    " << sol2.strangePrinter("aba") << endl;      // 2
    cout << "TopDown: " << sol3.strangePrinter("aba") << endl;      // 2

    // --- 更多测试 ---
    cout << "\n=== Test 3: s = \"abab\" ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("abab") << endl;     // 3
    cout << "Back:    " << sol2.strangePrinter("abab") << endl;     // 3
    cout << "TopDown: " << sol3.strangePrinter("abab") << endl;     // 3

    cout << "\n=== Test 4: s = \"abcba\" ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("abcba") << endl;    // 3
    cout << "Back:    " << sol2.strangePrinter("abcba") << endl;    // 3
    cout << "TopDown: " << sol3.strangePrinter("abcba") << endl;    // 3

    // --- 边界测试 ---
    cout << "\n=== Test 5: s = \"a\" (单字符) ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("a") << endl;        // 1

    cout << "\n=== Test 6: s = \"aaaa\" (全同) ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("aaaa") << endl;     // 1

    cout << "\n=== Test 7: s = \"abcdef\" (全不同) ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("abcdef") << endl;   // 6

    cout << "\n=== Test 8: s = \"abcabc\" ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("abcabc") << endl;   // 5
    cout << "Back:    " << sol2.strangePrinter("abcabc") << endl;   // 5
    cout << "TopDown: " << sol3.strangePrinter("abcabc") << endl;   // 5

    // --- 回文结构 ---
    cout << "\n=== Test 9: s = \"abacaba\" ===" << endl;
    cout << "Front:   " << sol1.strangePrinter("abacaba") << endl;  // 4
    cout << "Back:    " << sol2.strangePrinter("abacaba") << endl;  // 4
    cout << "TopDown: " << sol3.strangePrinter("abacaba") << endl;  // 4

    cout << "\nAll tests completed!" << endl;
    return 0;
}

/*
 * ============================================================
 * 复杂度总结
 * ============================================================
 *
 * 所有解法:
 *   时间: O(n^3) — 三层循环/递归展开
 *   空间: O(n^2) — DP 表 / 记忆化数组
 *
 * 去重优化: 如果原始字符串有大量连续重复，n 可缩减为 m，
 *           实际复杂度变为 O(m^3)，m 远小于 n。
 *
 * ============================================================
 * 区间 DP 模板总结
 * ============================================================
 *
 * 1. 定义状态: dp[i][j] = 区间 [i, j] 上的最优值
 *
 * 2. Base case: dp[i][i] = 单元素的值
 *
 * 3. 转移: 枚举分割点 k，合并 dp[i][k] 和 dp[k+1][j]
 *    或枚举匹配点，利用相同元素合并
 *
 * 4. 遍历: 按区间长度 len 从小到大
 *    for len = 2..n:
 *      for i = 0..(n-len):
 *        j = i + len - 1
 *        转移...
 *
 * 5. 答案: dp[0][n-1]
 *
 * 经典区间 DP 题目:
 *   - 312 Burst Balloons
 *   - 516 Longest Palindromic Subsequence
 *   - 664 Strange Printer (本题)
 *   - 1039 Minimum Score Triangulation of Polygon
 *   - 1246 Palindrome Removal
 */

// ============================================================
// 【解法对比】
//
// | 解法                    | 时间    | 空间    | 优势                        |
// |------------------------|--------|--------|---------------------------|
// | Bottom-up 首字符 (解法一) | O(n^3) | O(n^2) | 正向思维, 先写 s[i] 再扩展     |
// | Bottom-up 尾字符 (解法二) | O(n^3) | O(n^2) | 多数题解采用, 便于对照学习      |
// | Top-down 记忆化 (解法三)  | O(n^3) | O(n^2) | 代码最直觉, 不用管遍历顺序     |
//
// 三种解法本质相同, 只是状态转移的"基准字符"和实现方式不同。
// 面试策略: 推荐写 Top-down 记忆化 — 思路最自然, 不易写错边界。
//
// ============================================================
// 【易错点】
//
// 1. 忘记去重预处理:
//    ✗ 直接在原串上 DP → "aaabbb" 有 6 个状态
//    ✓ 先去连续重复 → "ab" 只有 2 个状态, 大幅减少计算量
//
// 2. 区间 DP 遍历顺序错误:
//    ✗ 按 i 从小到大、j 从小到大 → dp[i+1][m] 可能还没算
//    ✓ 按区间长度 len 从小到大, 保证子区间先于父区间被计算
//
// 3. 边界越界 (m+1 > j 或 k+1 > j-1):
//    ✗ 直接写 dp[m+1][j] → 当 m==j 时 m+1 > j, 访问无效
//    ✓ 加条件 if (m+1 <= j) val += dp[m+1][j]; 或定义 dp 为 i>j 时返回 0
//
// 4. 混淆 "首字符基准" 与 "尾字符基准" 的转移:
//    ✗ 基准是 s[i] 却写成 dp[i][k] + dp[k+1][j-1] 的形式
//    ✓ 首字符基准: dp[i+1][m] + dp[m+1][j]
//       尾字符基准: dp[i][k] + dp[k+1][j-1]
//
// ============================================================
// 【面试追问】
//
// Q1: 去重预处理为什么不影响答案?
// → 连续相同字符 (如 "aaa") 一定在同一次打印中完成,
//   不会跨越不同字符。所以 "aaabbb" 和 "ab" 的最少打印次数
//   完全相同。去重后 n 变小, 可以将 O(n^3) 中的 n 降为去重后
//   的长度 m, 实际加速显著。
//
// Q2: 这道题和 LC 312 Burst Balloons 有什么关联?
// → 都是经典区间 DP, 核心模式相同:
//   (1) 定义 dp[i][j] 为区间 [i,j] 上的最优值
//   (2) 枚举分割点/匹配点进行状态转移
//   (3) 按区间长度从小到大遍历
//   区别在于: 312 枚举"最后戳破的气球", 664 枚举"相同字符合并点"。
//
// Q3: 如果打印机可以覆盖任意子区间 (不要求从某端开始), 答案会变吗?
// → 不变。因为每次打印本质就是"选一段连续区间填同一字符",
//   没有方向限制。DP 的转移已经考虑了所有可能的覆盖方式,
//   和打印方向无关。
// ============================================================
