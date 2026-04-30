// ========================================
// 656. Coin Path
// ========================================
// 解法一：反向 DP（推荐）
// 时间 O(n * maxJump)  空间 O(n)
// ========================================
// 从终点反向计算最小代价，天然保证字典序最小
class Solution {
public:
    vector<int> cheapestJump(vector<int>& coins, int maxJump) {
        int n = coins.size();

        // 终点不可达，直接返回空
        if (coins[n - 1] == -1) return {};

        const long long INF = 1e18;
        vector<long long> dp(n, INF);  // dp[i] = 从位置 i 到终点的最小代价
        vector<int> next(n, -1);       // next[i] = 从位置 i 的最优下一跳

        // 初始化终点
        dp[n - 1] = coins[n - 1];

        // 从右向左遍历
        for (int i = n - 2; i >= 0; i--) {
            // 位置 i 不可达
            if (coins[i] == -1) continue;

            // 枚举从 i 可以跳到的所有位置
            for (int j = i + 1; j <= min(i + maxJump, n - 1); j++) {
                if (dp[j] == INF) continue;

                long long cost = (long long)coins[i] + dp[j];

                // 严格小于时更新：保证相同代价取最小 index（字典序最小）
                if (cost < dp[i]) {
                    dp[i] = cost;
                    next[i] = j;
                }
            }
        }

        // 起点不可达终点
        if (dp[0] == INF) return {};

        // 沿 next 指针构建路径（转为 1-indexed）
        vector<int> path;
        int cur = 0;
        while (cur != -1) {
            path.push_back(cur + 1);  // 转为 1-indexed
            cur = next[cur];
        }

        return path;
    }
};

// ========================================
// 解法二：正向 DP
// 时间 O(n * maxJump)  空间 O(n)
// ========================================
// dp[i] = 从位置 0 到位置 i 的最小代价
// 需要额外处理字典序：用路径长度辅助判断
class Solution2 {
public:
    vector<int> cheapestJump(vector<int>& coins, int maxJump) {
        int n = coins.size();
        if (coins[n - 1] == -1) return {};

        const long long INF = 1e18;
        vector<long long> dp(n, INF);
        vector<int> prev(n, -1);    // 记录前驱
        vector<int> len(n, 0);      // 记录路径长度（用于字典序比较）

        dp[0] = coins[0];
        len[0] = 1;

        for (int i = 1; i < n; i++) {
            if (coins[i] == -1) continue;

            // 检查所有能跳到位置 i 的位置
            for (int j = max(0, i - maxJump); j < i; j++) {
                if (dp[j] == INF) continue;

                long long cost = dp[j] + coins[i];

                // 代价更小，或者代价相同但路径更长（字典序更小的路径步数多、每步跳得少）
                if (cost < dp[i] || (cost == dp[i] && len[j] + 1 > len[i])) {
                    dp[i] = cost;
                    prev[i] = j;
                    len[i] = len[j] + 1;
                }
            }
        }

        if (dp[n - 1] == INF) return {};

        // 从终点回溯构建路径
        vector<int> path;
        int cur = n - 1;
        while (cur != -1) {
            path.push_back(cur + 1);  // 转为 1-indexed
            cur = prev[cur];
        }
        reverse(path.begin(), path.end());

        return path;
    }
};

// ========================================
// 解法三：反向 DP + 单调队列优化
// 时间 O(n)  空间 O(n)
// ========================================
// 用单调队列（滑动窗口最小值）优化内层循环
// 适用于 maxJump 很大的情况
class Solution3 {
public:
    vector<int> cheapestJump(vector<int>& coins, int maxJump) {
        int n = coins.size();
        if (coins[n - 1] == -1) return {};

        const long long INF = 1e18;
        vector<long long> dp(n, INF);
        vector<int> next(n, -1);

        dp[n - 1] = coins[n - 1];

        // 单调队列存储 dp 值最小的位置（从右到左处理）
        // 队列中的元素按 dp 值递增，保证队首是窗口最小值
        deque<int> dq;
        dq.push_back(n - 1);

        for (int i = n - 2; i >= 0; i--) {
            // 移除窗口外的元素（超出 maxJump 范围的）
            while (!dq.empty() && dq.front() > i + maxJump) {
                dq.pop_front();
            }

            if (coins[i] == -1) continue;

            if (!dq.empty() && dp[dq.front()] != INF) {
                dp[i] = (long long)coins[i] + dp[dq.front()];
                next[i] = dq.front();
            }

            // 维护单调队列：移除队尾所有 dp 值 >= dp[i] 的元素
            while (!dq.empty() && dp[dq.back()] >= dp[i]) {
                dq.pop_back();
            }
            dq.push_back(i);
        }

        if (dp[0] == INF) return {};

        vector<int> path;
        int cur = 0;
        while (cur != -1) {
            path.push_back(cur + 1);
            cur = next[cur];
        }

        return path;
    }
};
