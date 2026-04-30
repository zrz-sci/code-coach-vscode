// 【题目本质】
// 将 2n 个整数分成 n 对，使 sum(min(ai, bi)) 最大化。
// 核心：排序后相邻配对，取偶数下标元素之和。

// 【解法总览】
// Solution1: 排序 + 偶数下标求和 — O(n log n) 时间，O(1) 空间 ⭐
// Solution2: 计数排序 — O(n + k) 时间，O(k) 空间

// ===================== Solution1: 排序 + 偶数下标 =====================
// 思路：排序后相邻元素差最小，配对浪费最少
// min(nums[0], nums[1]) + min(nums[2], nums[3]) + ... = nums[0] + nums[2] + ...
class Solution1 {
public:
    int arrayPairSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int sum = 0;
        for (int i = 0; i < nums.size(); i += 2) {
            sum += nums[i];
        }
        return sum;
    }
};

// ===================== Solution2: 计数排序 =====================
// 思路：值域 [-10^4, 10^4]，用计数数组排序，交替 pick/skip
class Solution2 {
public:
    int arrayPairSum(vector<int>& nums) {
        vector<int> cnt(20001, 0);
        for (int x : nums) cnt[x + 10000]++;

        int sum = 0;
        bool pick = true;
        for (int i = 0; i <= 20000; i++) {
            while (cnt[i] > 0) {
                if (pick) sum += i - 10000;
                pick = !pick;
                cnt[i]--;
            }
        }
        return sum;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 优势 |
// |------|------|------|------|
// | Solution1 排序 | O(n log n) | O(1) | 简单直接，面试首选 |
// | Solution2 计数排序 | O(n + k) | O(k) | 值域有限时理论更快 |

// 【易错点】
// 1. 步长必须是 2（i += 2），不是 1
// 2. 计数排序的偏移量：值域 [-10000, 10000] → 加 10000 映射到 [0, 20000]
// 3. 贪心正确性证明：排序后相邻配对使每对差值之和最小

// 【面试追问】
// Q1: 为什么相邻配对最优？→ 反证法，非相邻配对会让 min 更小
// Q2: 值域很大时？→ 计数排序不可行，只能比较排序
// Q3: 求 max(ai,bi) 总和最小？→ 同样相邻配对，取奇数下标和
