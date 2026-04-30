// 788. Rotated Digits
// 难度: Medium
// 标签: Math, Dynamic Programming

// ============================================================
// 解法一：暴力枚举 (推荐面试首选)
// 时间 O(N * logN), 空间 O(1)
// ============================================================
class Solution {
public:
    int rotatedDigits(int n) {
        int count = 0;
        for (int i = 1; i <= n; ++i) {
            if (isGood(i)) ++count;
        }
        return count;
    }

private:
    bool isGood(int x) {
        // 合法数字: 0,1,2,5,6,8,9
        // 变化数字: 2,5,6,9 (旋转后不同)
        // 非法数字: 3,4,7
        bool hasChanged = false;
        while (x > 0) {
            int d = x % 10;
            if (d == 3 || d == 4 || d == 7) return false;
            if (d == 2 || d == 5 || d == 6 || d == 9) hasChanged = true;
            x /= 10;
        }
        return hasChanged;
    }
};

// ============================================================
// 解法二：查表优化暴力
// 时间 O(N * logN), 空间 O(1), 常数更优
// ============================================================
class Solution2 {
public:
    int rotatedDigits(int n) {
        // 0 = 不变(合法), 1 = 变化(合法), -1 = 非法
        int table[10] = {0, 0, 1, -1, -1, 1, 1, -1, 0, 1};
        int count = 0;
        for (int i = 1; i <= n; ++i) {
            bool valid = true;
            bool changed = false;
            int x = i;
            while (x > 0) {
                int d = x % 10;
                if (table[d] == -1) { valid = false; break; }
                if (table[d] == 1) changed = true;
                x /= 10;
            }
            if (valid && changed) ++count;
        }
        return count;
    }
};

// ============================================================
// 解法三：数位 DP (适用于 N 极大的场景)
// 时间 O(logN), 空间 O(logN)
// ============================================================
class Solution3 {
public:
    int rotatedDigits(int n) {
        string s = to_string(n);
        int len = s.size();
        // memo[pos][tight][hasChanged]
        // -1 表示未计算
        vector<vector<vector<int>>> memo(len, vector<vector<int>>(2, vector<int>(2, -1)));
        return dp(s, 0, true, false, memo);
    }

private:
    // 合法数字集合 (排序便于 tight 剪枝)
    vector<int> validDigits = {0, 1, 2, 5, 6, 8, 9};

    bool isChanged(int d) {
        return d == 2 || d == 5 || d == 6 || d == 9;
    }

    int dp(const string& s, int pos, bool tight, bool hasChanged,
           vector<vector<vector<int>>>& memo) {
        if (pos == (int)s.size()) {
            return hasChanged ? 1 : 0;
        }

        if (memo[pos][tight][hasChanged] != -1) {
            return memo[pos][tight][hasChanged];
        }

        int limit = tight ? (s[pos] - '0') : 9;
        int result = 0;

        for (int d : validDigits) {
            if (d > limit) break;
            bool newTight = tight && (d == limit);
            bool newChanged = hasChanged || isChanged(d);
            result += dp(s, pos + 1, newTight, newChanged, memo);
        }

        memo[pos][tight][hasChanged] = result;
        return result;
    }
};

// ============================================================
// 解法四：DP 迭代版 (Bottom-up)
// 时间 O(N), 空间 O(N)
// 用 dp 数组记录每个数字的状态
// ============================================================
class Solution4 {
public:
    int rotatedDigits(int n) {
        // dp[i]:
        //   0 = 非法 (含有 3,4,7)
        //   1 = 合法但不变 (只含 0,1,8)
        //   2 = 好数 (合法且至少有 2,5,6,9)
        vector<int> dp(n + 1, 0);
        int count = 0;

        for (int i = 0; i <= n; ++i) {
            if (i < 10) {
                if (i == 0 || i == 1 || i == 8) {
                    dp[i] = 1;
                } else if (i == 2 || i == 5 || i == 6 || i == 9) {
                    dp[i] = 2;
                    ++count;
                }
                // 3, 4, 7 保持 dp[i] = 0
            } else {
                int a = dp[i / 10];  // 高位部分的状态
                int b = dp[i % 10];  // 最低位的状态
                if (a == 0 || b == 0) {
                    dp[i] = 0;  // 非法
                } else if (a == 2 || b == 2) {
                    dp[i] = 2;  // 好数
                    ++count;
                } else {
                    dp[i] = 1;  // 合法但不变
                }
            }
        }
        return count;
    }
};
