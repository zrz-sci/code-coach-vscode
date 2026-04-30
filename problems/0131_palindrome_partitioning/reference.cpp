/*
 * LeetCode 131: Palindrome Partitioning (分割回文串)
 *
 * 【题目本质】
 * 枚举字符串的所有分割方式，保留每段都是回文的方案。
 * 核心 = 回溯枚举分割 + 回文判断。
 *
 * 【解法总览】
 * 解法1: 回溯 + 朴素回文判断    — O(n·2ⁿ) / O(n)   — 最直觉
 * 解法2: 回溯 + DP 预处理回文表  — O(n·2ⁿ) / O(n²)  — 面试首选
 * 解法3: 回溯 + 记忆化回文判断   — O(n·2ⁿ) / O(n²)  — 替代方案
 */

// ============================================================
// 解法1: 回溯 + 朴素回文判断
// 时间: O(n · 2ⁿ)  空间: O(n) 递归栈深度
//
// 【思路】
// 要找"所有"合法分割方案 → 穷举 → 回溯。
// 在每个位置 start，尝试截取 s[start..end] 作为一段。
// 如果是回文就递归处理剩余部分，否则跳过（剪枝）。
//
// 决策树 (s = "aab"):
//                      start=0
//              /           |            \
//          "a"(✓)       "aa"(✓)       "aab"(✗)
//         start=1        start=2
//        /      \          |
//     "a"(✓)  "ab"(✗)    "b"(✓)
//    start=2             start=3 → 收集 ["aa","b"]
//      |
//    "b"(✓)
//   start=3 → 收集 ["a","a","b"]
//
// 结果: [["a","a","b"], ["aa","b"]]
// ============================================================
class Solution1 {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, path, result);
        return result;
    }

private:
    void backtrack(const string& s, int start,
                   vector<string>& path, vector<vector<string>>& result) {
        // 终止条件：所有字符都被分割完毕
        if (start == (int)s.size()) {
            result.push_back(path);
            return;
        }
        // 枚举第一段的结束位置 end
        for (int end = start; end < (int)s.size(); end++) {
            // 剪枝：只有回文才继续
            if (isPalindrome(s, start, end)) {
                path.push_back(s.substr(start, end - start + 1)); // 做选择
                backtrack(s, end + 1, path, result);              // 递归剩余
                path.pop_back();                                   // 撤销选择
            }
        }
    }

    // 朴素双指针判断回文，O(n)
    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};

// ============================================================
// 解法2: 回溯 + DP 预处理回文表 — 面试首选 ⭐
// 时间: O(n · 2ⁿ)  空间: O(n²) 回文表
//
// 【思路】
// 解法1中，每次判断 s[start..end] 是否回文需要 O(n)。
// 而回溯过程中同一子串可能被多次判断（不同路径经过同一子串）。
//
// 优化：用 DP 一次性预处理所有子串的回文性。
// isPalin[i][j] = true 表示 s[i..j] 是回文。
// 转移方程: isPalin[i][j] = (s[i] == s[j]) && (j-i <= 1 || isPalin[i+1][j-1])
//
// DP 填表依赖关系:
//        j →
//    i   (i,j) 依赖 (i+1, j-1)
//    ↓        ↙
//         (i+1, j-1)
//
// 因此 i 必须从大到小遍历（从下往上填），j 从 i 到 n-1（从左往右填）。
//
// 以 s = "aab" 为例，回文表:
//       j=0  j=1  j=2
// i=0:  T    T    F      "a"=T, "aa"=T, "aab"=F
// i=1:  -    T    F      "a"=T, "ab"=F
// i=2:  -    -    T      "b"=T
// ============================================================
class Solution2 {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();

        // 第一步：DP 预处理回文表
        vector<vector<bool>> isPalin(n, vector<bool>(n, false));
        for (int i = n - 1; i >= 0; i--) {        // i 从下往上
            for (int j = i; j < n; j++) {          // j 从 i 往右
                if (s[i] == s[j]) {
                    // 长度 1 ("a") 或长度 2 ("aa"): 首尾相同即回文
                    // 长度 > 2: 还需要内层 [i+1..j-1] 也是回文
                    isPalin[i][j] = (j - i <= 1) || isPalin[i + 1][j - 1];
                }
                // s[i] != s[j] 时 isPalin[i][j] 保持 false
            }
        }

        // 第二步：回溯枚举分割
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, isPalin, path, result);
        return result;
    }

private:
    void backtrack(const string& s, int start,
                   const vector<vector<bool>>& isPalin,
                   vector<string>& path, vector<vector<string>>& result) {
        if (start == (int)s.size()) {
            result.push_back(path);
            return;
        }
        for (int end = start; end < (int)s.size(); end++) {
            // O(1) 查表替代 O(n) 判断 —— 这就是优化的核心
            if (isPalin[start][end]) {
                path.push_back(s.substr(start, end - start + 1));
                backtrack(s, end + 1, isPalin, path, result);
                path.pop_back();
            }
        }
    }
};

// ============================================================
// 解法3: 回溯 + 记忆化回文判断（懒计算版）
// 时间: O(n · 2ⁿ)  空间: O(n²)
//
// 【思路】
// 解法2是"全量预处理"——先把 n² 个子串全部算一遍。
// 但实际回溯中不一定会用到所有子串（剪枝后很多分支被跳过）。
//
// 本解法用"按需计算 + 记忆化"：
// - memo[i][j] = 0 表示未计算，1 表示回文，-1 表示非回文
// - 首次查询时递归计算并缓存，后续直接返回
//
// 适用于：大量分支被剪枝、实际用到的子串远少于 n² 的情况。
// ============================================================
class Solution3 {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        vector<vector<int>> memo(n, vector<int>(n, 0)); // 0=未算
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, memo, path, result);
        return result;
    }

private:
    void backtrack(const string& s, int start,
                   vector<vector<int>>& memo,
                   vector<string>& path, vector<vector<string>>& result) {
        if (start == (int)s.size()) {
            result.push_back(path);
            return;
        }
        for (int end = start; end < (int)s.size(); end++) {
            if (checkPalin(s, start, end, memo)) {
                path.push_back(s.substr(start, end - start + 1));
                backtrack(s, end + 1, memo, path, result);
                path.pop_back();
            }
        }
    }

    // 记忆化递归判断回文
    bool checkPalin(const string& s, int i, int j, vector<vector<int>>& memo) {
        if (i >= j) return true;                    // base case: 空串或单字符
        if (memo[i][j] != 0) return memo[i][j] == 1; // 已缓存，直接返回
        bool res = (s[i] == s[j]) && checkPalin(s, i + 1, j - 1, memo);
        memo[i][j] = res ? 1 : -1;                 // 缓存结果
        return res;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 回文判断 | 预处理 | 额外空间 | 特点 |
// |------|----------|--------|----------|------|
// | 解法1 | 每次O(n) | 无 | O(n) | 最简单，面试先写 |
// | 解法2 | O(1)查表 | O(n²) | O(n²) | 面试追问后优化⭐ |
// | 解法3 | O(1)查表 | 按需 | O(n²) | 懒计算替代方案 |
//
// 面试策略: 先写解法1展示回溯理解 → 面试官追问后加 DP 预处理变成解法2
//
// ============================================================
// 【易错点】
//
// 1. DP 填表方向: i 必须从 n-1 到 0（从下往上），因为 isPalin[i][j]
//    依赖 isPalin[i+1][j-1]。如果 i 从 0 到 n-1，i+1 还未计算。
//    ✗ for(int i=0; i<n; i++)  → 依赖的 isPalin[i+1][j-1] 尚未计算
//    ✓ for(int i=n-1; i>=0; i--)
//
// 2. 长度<=2 的特判: isPalin[i][j] = (s[i]==s[j]) && isPalin[i+1][j-1]
//    当 j == i+1 时，i+1 > j-1 = i，即 isPalin[i+1][i]，这个位置
//    初始是 false 但语义上应该是 true（空串是回文）。
//    ✗ isPalin[i][j] = (s[i]==s[j]) && isPalin[i+1][j-1]  // j-i==1时出错
//    ✓ isPalin[i][j] = (s[i]==s[j]) && (j-i <= 1 || isPalin[i+1][j-1])
//
// 3. substr 参数: s.substr(pos, length)，第二个参数是长度不是结束索引。
//    ✗ s.substr(start, end)
//    ✓ s.substr(start, end - start + 1)
//
// 4. 回溯忘记 pop_back:
//    ✗ path.push_back(...); backtrack(...); // 没有撤销
//    ✓ path.push_back(...); backtrack(...); path.pop_back();
//
// ============================================================
// 【面试追问】
//
// Q1(基础理解): 为什么用回溯而不是 DP？
//    → 题目要求返回"所有"方案，回溯天然枚举所有可能。
//      纯 DP 适合求"最优值"（如最少分割次数），不适合枚举所有方案。
//
// Q2(优化): 回文判断能否加速？
//    → 用 DP 预处理 isPalin[i][j] 表，O(n²) 预处理后 O(1) 查询。
//      或者用记忆化递归按需计算。
//
// Q3(变体): 如果改为"返回最少分割次数"（LeetCode 132）？
//    → 不再需要枚举所有方案。定义 dp[i] = s[0..i] 的最少切割次数。
//      转移: 对所有 j <= i，若 s[j..i] 回文，dp[i] = min(dp[i], dp[j-1]+1)。
//      仍然需要回文预处理表。时间 O(n²)，空间 O(n²)。
//
// Q4(进阶): 如果字符串很长但回文很少，有没有更好的方法？
//    → 可以用 Manacher 算法在 O(n) 内找出所有回文子串，
//      然后只对实际存在的回文子串建图做回溯/DP。
// ============================================================
