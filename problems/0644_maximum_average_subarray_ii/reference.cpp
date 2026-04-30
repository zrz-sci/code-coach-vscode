// LeetCode 644: Maximum Average Subarray II
// 解法：二分答案 + 前缀和判定
// 时间复杂度: O(n * log((max-min)/eps))  空间复杂度: O(1)

// ============================================================
// 核心思想：
// 二分搜索答案 mid，将问题转化为：
// "是否存在长度 >= k 的子数组，使得每个元素减去 mid 后的和 >= 0？"
// 通过前缀和 + 维护最小前缀和来 O(n) 判定。
// ============================================================

class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();

        // 第一步：确定二分范围
        double left = *min_element(nums.begin(), nums.end());
        double right = *max_element(nums.begin(), nums.end());

        // 第二步：二分搜索答案
        // 迭代足够多次保证精度（也可以用 right - left > 1e-5）
        while (right - left > 1e-5) {
            double mid = (left + right) / 2.0;

            if (canAchieve(nums, k, mid)) {
                left = mid;   // mid 可行，答案 >= mid
            } else {
                right = mid;  // mid 不可行，答案 < mid
            }
        }

        return left;
    }

private:
    // 判定函数：是否存在长度 >= k 的子数组，平均值 >= target
    // 等价于：将每个元素减去 target 后，是否存在长度 >= k 的子数组和 >= 0
    bool canAchieve(vector<int>& nums, int k, double target) {
        int n = nums.size();

        // 计算前缀和（每个元素减去 target）
        // sum 代表当前位置的前缀和
        // prevSum 代表位置 i-k 处的前缀和
        // minPrev 代表 prefixSum[0..i-k] 的最小值
        double sum = 0;         // 当前前缀和
        double prevSum = 0;     // 位置 i-k 的前缀和
        double minPrev = 0;     // 前缀和最小值（初始为 prefixSum[0] = 0）

        // 先累加前 k 个元素
        for (int i = 0; i < k; ++i) {
            sum += nums[i] - target;
        }

        // 如果前 k 个元素减去 target 的和 >= 0，已经找到
        if (sum >= 0) return true;

        // 继续扫描后续元素
        for (int i = k; i < n; ++i) {
            sum += nums[i] - target;          // 扩展当前前缀和
            prevSum += nums[i - k] - target;  // 前缀和[i-k] 的值

            // 更新 [0..i-k] 范围内的最小前缀和
            minPrev = min(minPrev, prevSum);

            // 如果当前前缀和减去最小前缀和 >= 0，说明存在合法子数组
            if (sum - minPrev >= 0) return true;
        }

        return false;
    }
};

// ============================================================
// 解法二：暴力枚举（仅供理解，会超时）
// 时间复杂度: O(n^2)  空间复杂度: O(1)
// ============================================================

class Solution_Brute {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();
        double maxAvg = INT_MIN;

        for (int i = 0; i < n; ++i) {
            double sum = 0;
            for (int j = i; j < n; ++j) {
                sum += nums[j];
                int len = j - i + 1;
                if (len >= k) {
                    maxAvg = max(maxAvg, sum / len);
                }
            }
        }

        return maxAvg;
    }
};

// ============================================================
// 手动模拟示例：nums = [1,12,-5,-6,50,3], k = 4
//
// 二分范围: left = -6, right = 50
//
// 假设 mid = 22:
//   减去 mid: [-21, -10, -27, -28, 28, -19]
//   前 k=4 个和: -21+(-10)+(-27)+(-28) = -86 < 0
//   继续: i=4, sum=-86+28=-58, prevSum=-21, minPrev=min(0,-21)=-21
//         -58-(-21) = -37 < 0
//   i=5, sum=-58+(-19)=-77, prevSum=-21+(-10)=-31, minPrev=min(-21,-31)=-31
//         -77-(-31) = -46 < 0
//   => mid=22 不可行, right=22
//
// 假设 mid = 12.75:
//   减去 mid: [-11.75, -0.75, -17.75, -18.75, 37.25, -9.75]
//   前 k=4 个和: -11.75+(-0.75)+(-17.75)+(-18.75) = -49 < 0
//   i=4, sum=-49+37.25=-11.75, prevSum=-11.75
//         minPrev=min(0,-11.75)=-11.75
//         -11.75-(-11.75)=0 >= 0  => 可行!
//   对应子数组 [12,-5,-6,50], 平均值=51/4=12.75
//
// 最终收敛到 left = 12.75
// ============================================================
