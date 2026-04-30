/*
 * 【题目本质】
 * 找和等于 k 的最长连续子数组。
 * 因为含负数，不能滑动窗口，核心是"前缀和 + 哈希表查找差值"。
 *
 * 【解法总览】
 * 解法1: 前缀和 + 哈希表 — O(n)/O(n) ⭐推荐
 * 解法2: 暴力枚举         — O(n^2)/O(1)
 */

// ===================== 解法1: 前缀和 + 哈希表 =====================
// 思路: prefix[j] - prefix[i] = k => 对每个 j 查找 prefix[j]-k 是否存在
//       只记录首次出现位置以最大化子数组长度
// 时间: O(n)  空间: O(n)
class Solution1 {
public:
    int maxSubArrayLen(vector<int>& nums, int k) {
        unordered_map<long long, int> prefixMap;
        prefixMap[0] = -1; // 空前缀位于 -1

        long long sum = 0;
        int maxLen = 0;

        for (int i = 0; i < (int)nums.size(); i++) {
            sum += nums[i];

            // 查找 sum - k 是否在哈希表中
            if (prefixMap.count(sum - k)) {
                maxLen = max(maxLen, i - prefixMap[sum - k]);
            }

            // 只在首次出现时记录（保证子数组尽量长）
            if (!prefixMap.count(sum)) {
                prefixMap[sum] = i;
            }
        }

        return maxLen;
    }
};

// ===================== 解法2: 暴力枚举 =====================
// 思路: 枚举所有 (i, j) 子数组，计算区间和
// 时间: O(n^2)  空间: O(1)
class Solution2 {
public:
    int maxSubArrayLen(vector<int>& nums, int k) {
        int n = nums.size(), maxLen = 0;
        for (int i = 0; i < n; i++) {
            long long sum = 0;
            for (int j = i; j < n; j++) {
                sum += nums[j];
                if (sum == k) {
                    maxLen = max(maxLen, j - i + 1);
                }
            }
        }
        return maxLen;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 前缀和+哈希 | O(n) | O(n) | 最优解，一次遍历 |
 * | 暴力枚举 | O(n^2) | O(1) | 简单直观，小数据可用 |
 *
 * 【易错点】
 * 1. 忘记初始化 prefixMap[0] = -1，导致从头开始的子数组被遗漏
 * 2. 每次都更新 prefixMap[sum] = i（应该只记录首次，否则子数组变短）
 * 3. 前缀和用 int 存储导致溢出（nums[i]最大10^4，n最大2*10^5）
 * 4. 误用滑动窗口 — 含负数时窗口和不单调，无法使用
 *
 * 【面试追问】
 * Q1: 全正数能否 O(1) 空间？→ 滑动窗口，双指针
 * Q2: 改成求"个数"? → LC 560，哈希表记次数而非索引
 * Q3: prefix 可能重复吗？→ 会，含负数时和可回到之前值
 */
