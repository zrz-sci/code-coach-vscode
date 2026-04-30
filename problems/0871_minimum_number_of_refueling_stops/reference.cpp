/*
 * LeetCode 871: Minimum Number of Refueling Stops
 * 解法一: 贪心 + 最大堆 (O(n log n))
 * 解法二: 动态规划 (O(n^2))
 *
 * 贪心策略: 尽量不加油，非加不可时从路过的加油站中选油量最大的。
 *          用最大堆维护"路过但未加油"的加油站油量。
 *
 * 时间复杂度: 解法一 O(n log n), 解法二 O(n^2)
 * 空间复杂度: O(n)
 */

// ===================== 解法一：贪心 + 最大堆 =====================

class Solution {
public:
    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        // 最大堆：存储路过的加油站的油量
        priority_queue<int> maxHeap;

        int fuel = startFuel;   // 当前剩余油量
        int prevPos = 0;        // 上一个位置（从起点0开始）
        int refuelCount = 0;    // 加油次数

        // 将终点加入作为哨兵（油量0），统一处理
        stations.push_back({target, 0});

        for (auto& station : stations) {
            int pos = station[0];
            int stationFuel = station[1];

            // 消耗从上一位置到当前加油站的油量
            fuel -= (pos - prevPos);

            // 如果油不够到达当前位置，从堆中取最大油量补充
            while (fuel < 0 && !maxHeap.empty()) {
                fuel += maxHeap.top();
                maxHeap.pop();
                refuelCount++;
            }

            // 如果堆空了还是不够，无法到达
            if (fuel < 0) {
                return -1;
            }

            // 将当前加油站的油量放入堆（备选）
            maxHeap.push(stationFuel);
            prevPos = pos;
        }

        return refuelCount;
    }
};

// ===================== 解法二：动态规划 =====================
// dp[j] = 恰好加 j 次油时能到达的最远距离

class SolutionDP {
public:
    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        int n = stations.size();

        // dp[j] = 加了 j 次油后能到达的最远距离
        // 初始化：加 0 次油能到达 startFuel 的距离
        vector<long long> dp(n + 1, 0);
        dp[0] = startFuel;

        for (int i = 0; i < n; i++) {
            long long pos = stations[i][0];
            long long fuel = stations[i][1];

            // 倒序更新，类似 0-1 背包
            // 从大到小枚举加油次数，避免同一站被重复使用
            for (int j = i + 1; j >= 1; j--) {
                // 如果加 j-1 次油能到达第 i 个加油站
                if (dp[j - 1] >= pos) {
                    // 在第 i 站加油后，能到达的最远距离
                    dp[j] = max(dp[j], dp[j - 1] + fuel);
                }
            }
        }

        // 找最小的加油次数使得能到达终点
        for (int j = 0; j <= n; j++) {
            if (dp[j] >= target) {
                return j;
            }
        }
        return -1;
    }
};

// ===================== 解法一的详细注释版 =====================
// 更详细地解释贪心过程

class SolutionDetailed {
public:
    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        int n = stations.size();
        priority_queue<int> maxHeap;
        int fuel = startFuel;
        int refuels = 0;
        int idx = 0;       // 当前考察的加油站索引
        int curPos = 0;    // 当前位置（最远能到达的位置）

        // 持续尝试前进直到到达终点
        // curPos 代表当前能到达的最远位置 = 之前的位置 + 剩余油量
        // 但更准确地说，fuel 就是"从当前 curPos 还能走多远"

        while (fuel < target - curPos) {
            // 油不够直接到终点

            // 把所有能到达的加油站的油量加入堆
            while (idx < n && stations[idx][0] <= curPos + fuel) {
                maxHeap.push(stations[idx][1]);
                idx++;
            }

            // 如果没有可用的加油站，无法继续
            if (maxHeap.empty()) {
                return -1;
            }

            // 贪心：选油量最大的加油站加油
            fuel += maxHeap.top();
            maxHeap.pop();
            refuels++;
        }

        return refuels;
    }
};
