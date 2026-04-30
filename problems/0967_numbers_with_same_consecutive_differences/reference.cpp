// LeetCode 967: Numbers With Same Consecutive Differences
// 解法一：BFS 逐层扩展 —— 推荐解法
// 时间 O(2^n)，空间 O(2^n)

class Solution {
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        // 初始化：所有一位数字 1-9 作为种子
        vector<int> cur;
        for (int d = 1; d <= 9; d++) {
            cur.push_back(d);
        }

        // 逐层扩展，每次追加一位数字
        for (int step = 1; step < n; step++) {
            vector<int> next;
            for (int num : cur) {
                int lastDigit = num % 10;

                // 尝试追加 lastDigit + k
                if (lastDigit + k <= 9) {
                    next.push_back(num * 10 + lastDigit + k);
                }

                // 尝试追加 lastDigit - k（k != 0 时才不重复）
                if (k != 0 && lastDigit - k >= 0) {
                    next.push_back(num * 10 + lastDigit - k);
                }
            }
            cur = next;
        }

        return cur;
    }
};


// ================================================================
// 解法二：DFS/回溯
// 时间 O(2^n)，空间 O(n) 递归栈
// ================================================================

class Solution2 {
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        vector<int> result;

        // 从每个首位数字 1-9 开始 DFS
        for (int firstDigit = 1; firstDigit <= 9; firstDigit++) {
            dfs(firstDigit, n - 1, k, result);
        }

        return result;
    }

private:
    // num: 当前构建的数字
    // remaining: 还需要追加的位数
    // k: 相邻位差值
    void dfs(int num, int remaining, int k, vector<int>& result) {
        if (remaining == 0) {
            result.push_back(num);
            return;
        }

        int lastDigit = num % 10;

        // 分支 1：追加 lastDigit + k
        if (lastDigit + k <= 9) {
            dfs(num * 10 + lastDigit + k, remaining - 1, k, result);
        }

        // 分支 2：追加 lastDigit - k（避免 k=0 时重复）
        if (k != 0 && lastDigit - k >= 0) {
            dfs(num * 10 + lastDigit - k, remaining - 1, k, result);
        }
    }
};


// ================================================================
// 解法三：迭代 DFS（显式栈模拟）
// 时间 O(2^n)，空间 O(2^n)
// ================================================================

class Solution3 {
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        vector<int> result;

        // 栈中存储 (当前数字, 已有位数)
        stack<pair<int, int>> stk;

        // 初始化所有首位
        for (int d = 1; d <= 9; d++) {
            stk.push({d, 1});
        }

        while (!stk.empty()) {
            auto [num, len] = stk.top();
            stk.pop();

            if (len == n) {
                result.push_back(num);
                continue;
            }

            int lastDigit = num % 10;

            // 注意：栈是后进先出，如果需要特定顺序可以调整压栈顺序
            if (k != 0 && lastDigit - k >= 0) {
                stk.push({num * 10 + lastDigit - k, len + 1});
            }
            if (lastDigit + k <= 9) {
                stk.push({num * 10 + lastDigit + k, len + 1});
            }
        }

        return result;
    }
};


// ================================================================
// 解法四：DP 计数思路（变体：如果只需要计数而非列举）
// 这里仍然列举所有数字，但用 DP 思维组织代码
// 时间 O(n * 10)，空间 O(10) —— 实际只适用于计数
// ================================================================

class Solution4 {
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        // dp[d] 存储当前层末位为 d 的所有数字
        vector<vector<int>> dp(10);

        // 初始化第一层
        for (int d = 1; d <= 9; d++) {
            dp[d].push_back(d);
        }

        // 逐层扩展
        for (int step = 1; step < n; step++) {
            vector<vector<int>> newDp(10);

            for (int d = 0; d <= 9; d++) {
                if (dp[d].empty()) continue;

                // 下一位可以是 d + k 或 d - k
                for (int num : dp[d]) {
                    if (d + k <= 9) {
                        newDp[d + k].push_back(num * 10 + d + k);
                    }
                    if (k != 0 && d - k >= 0) {
                        newDp[d - k].push_back(num * 10 + d - k);
                    }
                }
            }

            dp = newDp;
        }

        // 收集所有结果
        vector<int> result;
        for (int d = 0; d <= 9; d++) {
            for (int num : dp[d]) {
                result.push_back(num);
            }
        }

        return result;
    }
};


// ================================================================
// 解法五：字符串构建（展示另一种实现方式）
// 时间 O(2^n * n)，空间 O(2^n * n) —— 字符串操作有额外开销
// ================================================================

class Solution5 {
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        vector<int> result;
        string current;

        for (char firstChar = '1'; firstChar <= '9'; firstChar++) {
            current.push_back(firstChar);
            buildNumber(current, n, k, result);
            current.pop_back();
        }

        return result;
    }

private:
    void buildNumber(string& current, int n, int k, vector<int>& result) {
        if ((int)current.size() == n) {
            result.push_back(stoi(current));
            return;
        }

        int lastDigit = current.back() - '0';

        if (lastDigit + k <= 9) {
            current.push_back('0' + lastDigit + k);
            buildNumber(current, n, k, result);
            current.pop_back();
        }

        if (k != 0 && lastDigit - k >= 0) {
            current.push_back('0' + lastDigit - k);
            buildNumber(current, n, k, result);
            current.pop_back();
        }
    }
};
