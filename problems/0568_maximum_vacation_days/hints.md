# 568. Maximum Vacation Days - 最大休假天数

## 核心思路
DP：dp[w][c] = 前 w 周结束时在城市 c 能获得的最大休假天数。每周一可以飞到相邻城市或留在原地，转移时枚举上一周所在城市。

## 思维链
1. 每周一做决策：留在当前城市 or 飞到某个城市
2. 状态：第 w 周在城市 c -> dp[w][c] = 最大休假天数
3. 转移：dp[w][c] = max(dp[w-1][prev] + days[c][w])，其中 prev 可达 c
4. 初始：第0周只能从城市0出发（或从0飞到的城市）
5. 答案：max(dp[k-1][0..n-1])

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ DP（滚动数组优化） | O(n^2 * k) | O(n) | 最优解 |
| DP（完整二维表） | O(n^2 * k) | O(n*k) | 更直观 |
| DFS + 记忆化 | O(n^2 * k) | O(n*k) | 自顶向下 |

## 关键提示
1. 注意"可达"条件：flights[prev][c] == 1 或 prev == c（留在原地）
2. 初始化 dp 全为 -1（不可达状态），dp[0] = days[0][0]（起始在城市0）
3. 第0周可以从城市0飞到的城市也要初始化
4. 空间优化：每周只依赖上一周，可用滚动数组

## 解法详解

### 解法1: DP 滚动数组 ⭐
```cpp
class Solution {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size(), k = days[0].size();
        vector<int> prev(n, -1);
        // 第0周初始化：从城市0出发，可以留在0或飞到相邻城市
        for (int c = 0; c < n; ++c) {
            if (c == 0 || flights[0][c]) {
                prev[c] = days[c][0];
            }
        }
        
        for (int w = 1; w < k; ++w) {
            vector<int> curr(n, -1);
            for (int c = 0; c < n; ++c) {
                for (int p = 0; p < n; ++p) {
                    if (prev[p] == -1) continue;
                    if (p == c || flights[p][c]) {
                        curr[c] = max(curr[c], prev[p] + days[c][w]);
                    }
                }
            }
            prev = curr;
        }
        return *max_element(prev.begin(), prev.end());
    }
};
```

### 解法2: DFS + 记忆化
```cpp
class Solution {
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
        n = flights.size(); k = days[0].size();
        memo.assign(n, vector<int>(k, -1));
        return dfs(0, 0, flights, days);
    }
};
```

## 易错点
- ✗ 忘记第0周也可以从城市0飞走 → ✓ 题目说周一早上可以飞，第一周也算
- ✗ 用 0 初始化不可达状态 → ✓ 应该用 -1，因为 0 天休假也是合法的
- ✗ 只考虑 flights[prev][c]==1 → ✓ 还要考虑 prev==c（留在原地）
- ✗ flights[i][i]==0 但可以留在原地 → ✓ 对角线为0不代表不能停留

## 面试追问

**Q1: 如何确定状态和转移？**
状态 = (周数, 当前城市)，转移 = 枚举上周可能的出发城市，取最大值。

**Q2: 能否用 BFS 代替 DP？**
可以看作分层图最长路径问题，但 DP 更自然高效。BFS 适合最短路径，DP 适合最长路径。

**Q3: 如果城市数很大（n=10^4），如何优化？**
用邻接表代替邻接矩阵，转移时只遍历有航班的城市，但最坏情况仍是 O(n^2*k)。

## 相关题型
- [787. Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) - 分层图DP
- [1334. Find the City With the Smallest Number of Neighbors](https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/) - 图+DP
- [188. Best Time to Buy and Sell Stock IV](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/) - 多状态DP
