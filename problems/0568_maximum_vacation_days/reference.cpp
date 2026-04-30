// 【题目本质】
// n 个城市 k 周，城市间有航班约束，每周可以飞或留，求最大休假天数。
// 经典分层图 DP：每一周是一层，枚举上一层可达的城市进行转移。

// 【解法总览】
// Solution1: DP 滚动数组优化  O(n^2 * k) / O(n)
// Solution2: DFS + 记忆化      O(n^2 * k) / O(n*k)

// ===================== Solution 1 =====================
// DP 滚动数组
// prev[c] = 上一周结束在城市 c 的最大休假天数
class Solution1 {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size(), k = days[0].size();
        vector<int> prev(n, -1);

        // 第0周：从城市0出发，可留在0或飞到相邻城市
        for (int c = 0; c < n; ++c) {
            if (c == 0 || flights[0][c]) {
                prev[c] = days[c][0];
            }
        }

        // 第1周到第k-1周
        for (int w = 1; w < k; ++w) {
            vector<int> curr(n, -1);
            for (int c = 0; c < n; ++c) {           // 枚举本周要待的城市
                for (int p = 0; p < n; ++p) {        // 枚举上周在哪个城市
                    if (prev[p] == -1) continue;     // 上周不可达
                    if (p == c || flights[p][c]) {   // 可以留在原地或飞过来
                        curr[c] = max(curr[c], prev[p] + days[c][w]);
                    }
                }
            }
            prev = curr;
        }
        return *max_element(prev.begin(), prev.end());
    }
};

// ===================== Solution 2 =====================
// DFS + 记忆化（自顶向下）
// memo[city][week] = 从第 week 周在 city 开始，到最后能获得的最大休假
class Solution2 {
    int n, k;
    vector<vector<int>> memo;

    int dfs(int city, int week, vector<vector<int>>& flights, vector<vector<int>>& days) {
        if (week == k) return 0;
        if (memo[city][week] != -1) return memo[city][week];

        int best = 0;
        for (int c = 0; c < n; ++c) {
            if (c == city || flights[city][c]) {
                best = max(best, days[c][week] + dfs(c, week + 1, flights, days));
            }
        }
        return memo[city][week] = best;
    }
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        n = flights.size();
        k = days[0].size();
        memo.assign(n, vector<int>(k, -1));
        return dfs(0, 0, flights, days);
    }
};

// 【解法对比】
// Solution1: 自底向上DP + 滚动数组，空间最优 O(n)，面试推荐
// Solution2: 自顶向下记忆化，思路直观，空间 O(n*k)

// 【易错点】
// 1. flights[i][i]==0 但可以留在城市 i（不需要航班）
// 2. 不可达状态用 -1 标记，不能用 0（0天休假也合法）
// 3. 第0周就可以从城市0飞到其他城市（周一早上飞）
// 4. 答案是所有城市中最后一周的最大值，不一定在城市0结束

// 【面试追问】
// Q1: 状态转移方程？dp[w][c] = max(dp[w-1][p] + days[c][w])，p可达c
// Q2: 能否用Dijkstra？这是最长路径问题，DAG上DP更合适
// Q3: n很大时优化？用邻接表减少无效枚举，或预处理反向邻接表
