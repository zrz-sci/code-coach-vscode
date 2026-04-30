/*
 * 【题目本质】
 * 判断是否存在长度至少为 2 的连续子数组，其和为 k 的倍数。
 * 核心数学：同余定理 — prefix[j] % k == prefix[i] % k 则 sum(i+1..j) 是 k 的倍数。
 *
 * 【解法总览】
 * Solution1: 前缀和 + 同余哈希  O(n) / O(min(n,k))  ⭐推荐
 * Solution2: 暴力前缀和         O(n^2) / O(n)
 */

// ===================== Solution1: 前缀和 + 同余哈希 =====================
// 思路：记录每个余数首次出现的索引，余数重复出现且间距 >= 2 即找到答案
class Solution1 {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        // 余数 -> 首次出现的索引
        unordered_map<int, int> remainderIndex;
        remainderIndex[0] = -1; // 关键初始化：前缀和余数为0在虚拟位置-1

        int prefixSum = 0;
        for (int i = 0; i < nums.size(); i++) {
            prefixSum += nums[i];
            int remainder = prefixSum % k;

            if (remainderIndex.count(remainder)) {
                if (i - remainderIndex[remainder] >= 2) {
                    return true;
                }
                // 不更新——保留最早的索引
            } else {
                remainderIndex[remainder] = i;
            }
        }
        return false;
    }
};

// ===================== Solution2: 暴力前缀和 =====================
// 思路：计算前缀和后枚举所有长度 >= 2 的子数组
class Solution2 {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        for (int i = 0; i < n; i++) {
            for (int j = i + 2; j <= n; j++) {
                long long sum = prefix[j] - prefix[i];
                if (sum % k == 0) return true;
            }
        }
        return false;
    }
};

/*
 * 【解法对比】
 * Solution1: O(n) 一次遍历，利用同余定理，面试首选
 * Solution2: O(n^2) 暴力，大数据量超时，仅用于验证
 *
 * 【易错点】
 * 1. 忘记初始化 remainderIndex[0] = -1，遗漏从索引 0 开始的子数组
 * 2. 余数相同时更新了索引（应保留首次出现的位置）
 * 3. 长度判断写成 >= 1 而非 >= 2
 * 4. 前缀和溢出——需要注意用 long long（暴力解法中）
 *
 * 【面试追问】
 * Q1: 为什么初始化 {0: -1}？
 * Q2: 同余定理的数学证明？
 * Q3: 如何扩展为"计算满足条件的子数组个数"？
 */
