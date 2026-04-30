/*
 * LeetCode 1365: How Many Numbers Are Smaller Than the Current Number
 * (有多少小于当前数字的数字)
 *
 * 【题目本质】
 * 对数组中每个元素，统计有多少个其他元素严格小于它。
 * 值域 [0, 100] 非常小，可用计数排序 + 前缀和做到 O(n)。
 *
 * 【解法总览】
 * 解法1: 暴力双重循环      — O(n^2) / O(1)      — 基线
 * 解法2: 排序 + 哈希       — O(n log n) / O(n)   — 通用
 * 解法3: 计数排序 + 前缀和 — O(n + k) / O(k)     — 最优，推荐
 *
 * 【手动推演】
 *
 *   nums = [8, 1, 2, 2, 3]
 *
 *   Step 1: count[1]=1, count[2]=2, count[3]=1, count[8]=1
 *
 *   Step 2: 前缀和 (严格小于 v 的个数)
 *     prefix[0] = 0
 *     prefix[1] = 0 + count[0] = 0
 *     prefix[2] = 0 + count[1] = 1   ← 比2小的有: {1}
 *     prefix[3] = 1 + count[2] = 3   ← 比3小的有: {1,2,2}
 *     ...
 *     prefix[8] = 4                   ← 比8小的有: {1,2,2,3}
 *
 *   Step 3: 查询
 *     nums[0]=8 → 4, nums[1]=1 → 0, nums[2]=2 → 1
 *     nums[3]=2 → 1, nums[4]=3 → 3
 *
 *   结果: [4, 0, 1, 1, 3] ✓
 */


// ============================================================
// 解法1: 暴力双重循环 — 基线解法
// 时间: O(n^2)  最多 500*500 = 250000 次比较
// 空间: O(1)    不算输出数组
//
// 【思路】
// 对每个 nums[i]，遍历整个数组统计 nums[j] < nums[i] 的个数。
// 不需要特判 j != i，因为 nums[j] == nums[i] 时不满足 < 条件。
//
// 【适用场景】
// n 很小（≤ 500）时完全可以，面试中作为 O(n^2) 基线引出优化。
// ============================================================
class Solution1 {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            int cnt = 0;
            for (int j = 0; j < n; j++) {
                // j == i 时 nums[j] == nums[i]，不满足 <，自动跳过
                if (nums[j] < nums[i]) cnt++;
            }
            res[i] = cnt;
        }
        return res;
    }
};


// ============================================================
// 解法2: 排序 + 哈希 — 通用解法
// 时间: O(n log n)  排序主导
// 空间: O(n)        排序副本 + 哈希表
//
// 【思路】
// 排序后，每个值第一次出现的索引 = 比它小的元素个数。
//
// 为什么是"第一次出现"？
//   排序后: [1, 2, 2, 3, 8]
//   值2第一次出现在 index=1，表示有1个元素比2小（就是那个1）
//   如果取最后一次出现(index=2)，就变成"2个比2小"，多算了另一个2
//
// 【适用场景】
// 值域很大时（如 10^9），计数排序不适用，这个方法仍然可行。
// ============================================================
class Solution2 {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int n = nums.size();
        vector<int> sorted_nums = nums;  // 拷贝排序，不破坏原数组
        sort(sorted_nums.begin(), sorted_nums.end());

        // 记录每个值第一次出现的位置
        unordered_map<int, int> firstIdx;
        for (int i = 0; i < n; i++) {
            // 只在不存在时写入，确保是"第一次出现"
            if (firstIdx.find(sorted_nums[i]) == firstIdx.end()) {
                firstIdx[sorted_nums[i]] = i;
            }
        }

        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            res[i] = firstIdx[nums[i]];
        }
        return res;
    }
};


// ============================================================
// 解法3: 计数排序 + 前缀和 — 最优解，推荐
// 时间: O(n + k)  k = 101（值域大小）
// 空间: O(k)      两个大小 101 的数组
//
// 【思路】
// 1. count[v] = 数组中等于 v 的元素个数
// 2. prefix[v] = count[0] + count[1] + ... + count[v-1]
//              = 严格小于 v 的元素总数
// 3. 对 nums[i]，答案 = prefix[nums[i]]
//
// 【关键推导】
// 为什么 prefix[v] = prefix[v-1] + count[v-1] 而不是 + count[v]？
//   prefix[v] 表示"严格小于 v"的个数
//   = "小于 v-1 的个数" + "等于 v-1 的个数"
//   = prefix[v-1] + count[v-1]
//   如果用 count[v]，就变成"小于等于 v"了
//
// 【前缀和构造过程】(以 nums = [8,1,2,2,3] 为例)
//
//   count[]: ..., count[1]=1, count[2]=2, count[3]=1, ..., count[8]=1
//
//   prefix[0] = 0              (没有值 < 0)
//   prefix[1] = 0 + count[0]   = 0 + 0 = 0   (没有值 < 1)
//   prefix[2] = 0 + count[1]   = 0 + 1 = 1   ({1} 小于 2)
//   prefix[3] = 1 + count[2]   = 1 + 2 = 3   ({1,2,2} 小于 3)
//   prefix[4] = 3 + count[3]   = 3 + 1 = 4   ({1,2,2,3} 小于 4)
//   ...
//   prefix[8] = 4 + 0 + ... = 4               ({1,2,2,3} 小于 8)
//
// 【边界验证: 全相同元素】
//   nums = [7,7,7,7]
//   count[7] = 4, 其余 = 0
//   prefix[7] = count[0]+...+count[6] = 0
//   每个元素答案都是 0 ✓
// ============================================================
class Solution3 {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        // Step 1: 统计频率
        int count[101] = {};
        for (int x : nums) count[x]++;

        // Step 2: 构造前缀和
        // prefix[i] = count[0] + count[1] + ... + count[i-1]
        //           = 严格小于 i 的元素个数
        int prefix[101] = {};
        for (int i = 1; i <= 100; i++)
            prefix[i] = prefix[i - 1] + count[i - 1];

        // Step 3: 查询每个元素的答案
        vector<int> res;
        res.reserve(nums.size());
        for (int x : nums) res.push_back(prefix[x]);
        return res;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度       | 暴力           | 排序+哈希       | 计数排序+前缀和   |
// |-----------|---------------|----------------|-----------------|
// | 时间       | O(n^2)        | O(n log n)     | O(n + k), k=101 |
// | 空间       | O(1)          | O(n)           | O(k) = O(101)   |
// | 值域限制   | 无             | 无              | 需要值域小       |
// | 代码量     | 最短           | 中等            | 中等             |
//
// 推荐: 本题值域 [0,100]，计数排序+前缀和是最优解。
// 如果面试中值域很大(如10^9)，退化到排序+哈希。
//
// ============================================================
// 【易错点】
//
// 1. 前缀和方向错误
//    ✗ prefix[i] = prefix[i-1] + count[i]   → 这是"小于等于 i"
//    ✓ prefix[i] = prefix[i-1] + count[i-1]  → 这才是"严格小于 i"
//
// 2. 排序法取最后一次出现而非第一次
//    ✗ for (int i = 0; i < n; i++) firstIdx[sorted[i]] = i;
//      → 相同值被覆盖成最后一个索引
//    ✓ if (!firstIdx.count(sorted[i])) firstIdx[sorted[i]] = i;
//
// 3. prefix 数组大小
//    ✗ int prefix[100]   → 访问 prefix[100] 越界
//    ✓ int prefix[101]   → 索引范围 0~100
//
// ============================================================
// 【面试追问】
//
// Q1: 值域很大（如 10^9）时怎么办？
//     → 排序+哈希 O(n log n)，或先离散化再计数排序
//
// Q2: 如果要求"小于等于"而非"严格小于"？
//     → prefix[i] = prefix[i-1] + count[i] 即可
//
// Q3: 这个方法和桶排序有什么关系？
//     → 本质就是桶排序的思想，每个值一个桶，前缀和统计排名
//
// Q4: 时间 O(n+k) 中 k=101，可以认为是 O(n) 吗？
//     → 可以。k 是常数（由题目约束确定），不随 n 变化
// ============================================================
