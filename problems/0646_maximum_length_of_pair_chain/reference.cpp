// LeetCode 646: Maximum Length of Pair Chain
// 解法一：贪心（按右端点排序）
// 时间复杂度: O(n log n)  空间复杂度: O(1)（排序空间除外）

// ============================================================
// 核心思想：
// 活动选择问题的经典贪心：按右端点排序后，依次选择不与
// 当前链冲突的数对。选择结束最早的数对，给后续留出最大空间。
// ============================================================

class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        // 按右端点升序排序
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        int count = 0;
        int curEnd = INT_MIN;  // 当前链末尾的右端点

        for (auto& p : pairs) {
            // 如果当前数对的左端点 > 链末尾的右端点，可以接上
            // 注意是严格大于（题目要求 b < c）
            if (p[0] > curEnd) {
                count++;
                curEnd = p[1];
            }
        }

        return count;
    }
};

// ============================================================
// 解法二：动态规划（类 LIS）
// 时间复杂度: O(n^2)  空间复杂度: O(n)
// ============================================================

class Solution_DP {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        int n = pairs.size();

        // 按左端点排序（DP 需要保证扫描顺序的正确性）
        sort(pairs.begin(), pairs.end());

        // dp[i] = 以 pairs[i] 结尾的最长链长度
        vector<int> dp(n, 1);

        int maxLen = 1;
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                // 如果 pairs[j] 的右端点 < pairs[i] 的左端点，可以衔接
                if (pairs[j][1] < pairs[i][0]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            maxLen = max(maxLen, dp[i]);
        }

        return maxLen;
    }
};

// ============================================================
// 解法三：DP + 二分优化（类 LIS O(nlogn) 解法）
// 时间复杂度: O(n log n)  空间复杂度: O(n)
// ============================================================

class Solution_DPBinary {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        // 按左端点排序，左端点相同按右端点降序
        // （降序是为了防止左端点相同的数对被错误计入链中）
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] == b[0]) return a[1] > b[1];
            return a[0] < b[0];
        });

        // tails[i] = 长度为 i+1 的链中，末尾数对的最小右端点
        vector<int> tails;

        for (auto& p : pairs) {
            // 找到 tails 中第一个 >= p[0] 的位置
            // （因为我们需要严格 tail < p[0]）
            int pos = lower_bound(tails.begin(), tails.end(), p[0]) - tails.begin();

            if (pos == (int)tails.size()) {
                tails.push_back(p[1]);
            } else {
                tails[pos] = min(tails[pos], p[1]);
            }
        }

        return tails.size();
    }
};

// ============================================================
// 手动模拟（贪心解法）：pairs = [[1,2],[2,3],[3,4]]
//
// 排序后（按右端点）：[[1,2],[2,3],[3,4]]
//
// curEnd = INT_MIN, count = 0
//
// i=0: p=[1,2], 1 > INT_MIN => true
//      count=1, curEnd=2
//
// i=1: p=[2,3], 2 > 2 => false (严格大于)
//      跳过
//
// i=2: p=[3,4], 3 > 2 => true
//      count=2, curEnd=4
//
// 返回 2
// 链为 [1,2] -> [3,4]
// ============================================================

// ============================================================
// 手动模拟（DP 解法）：pairs = [[1,2],[7,8],[4,5]]
//
// 排序后（按左端点）：[[1,2],[4,5],[7,8]]
// dp = [1, 1, 1]
//
// i=1: j=0, pairs[0][1]=2 < pairs[1][0]=4 => true
//      dp[1] = max(1, dp[0]+1) = 2
//
// i=2: j=0, pairs[0][1]=2 < pairs[2][0]=7 => true
//      dp[2] = max(1, dp[0]+1) = 2
//      j=1, pairs[1][1]=5 < pairs[2][0]=7 => true
//      dp[2] = max(2, dp[1]+1) = 3
//
// 返回 max(dp) = 3
// 链为 [1,2] -> [4,5] -> [7,8]
// ============================================================
