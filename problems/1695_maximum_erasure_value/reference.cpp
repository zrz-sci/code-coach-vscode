/*
 * LeetCode 1695: Maximum Erasure Value (删除子数组的最大得分)
 *
 * 【题目本质】
 * 找元素不重复的连续子数组的最大元素和。
 * 经典滑动窗口 + 哈希集合，与 LC 3 (最长无重复子串) 框架相同，
 * 区别仅在: LC 3 求最大长度, 本题求最大和。
 *
 * 【解法总览】
 * 解法1: 滑窗 + unordered_set — O(n) / O(n)      — 面试推荐
 * 解法2: 滑窗 + 数组计数     — O(n) / O(max_val)  — 常数更快
 * 解法3: 滑窗 + 前缀和       — O(n) / O(n)        — 另一种写法
 */

// ============================================================
// 解法1: 滑动窗口 + unordered_set — 面试推荐 ⭐
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 维护窗口 [left, right], 保证窗口内元素互不相同。
//
// 操作流程:
//   for right in 0..n-1:
//     1. 收缩: while nums[right] 在窗口中 → 移除 nums[left], left++
//     2. 扩展: 加入 nums[right], sum += nums[right]
//     3. 更新: ans = max(ans, sum)
//
// 滑动窗口过程 (nums = [4, 2, 4, 5, 6]):
//
//   right=0: nums[0]=4, 不在seen
//     窗口: [4]        sum=4   ans=4
//            L R
//
//   right=1: nums[1]=2, 不在seen
//     窗口: [4,2]      sum=6   ans=6
//            L   R
//
//   right=2: nums[2]=4, 4在seen中!
//     收缩: erase(4), sum-=4→2, left=1
//     → 4不在了, 退出while
//     扩展: insert(4), sum+=4→6
//     窗口: [2,4]      sum=6   ans=6
//              L  R
//
//   right=3: nums[3]=5, 不在seen
//     窗口: [2,4,5]    sum=11  ans=11
//              L     R
//
//   right=4: nums[4]=6, 不在seen
//     窗口: [2,4,5,6]  sum=17  ans=17
//              L        R
//
//   最终 ans=17, 子数组 [2,4,5,6]
//
// 为什么是 O(n)?
//   right 从 0 到 n-1, 共 n 步。
//   left 也只向右移, 总共最多 n 步。
//   while 中的 left++ 总计 ≤ n 次。
//   所以总操作 = O(n) + O(n) = O(n)。
// ============================================================
class Solution {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        unordered_set<int> seen; // 窗口内元素集合
        int left = 0;            // 窗口左边界
        int sum = 0;             // 窗口内元素和
        int ans = 0;             // 全局最大和

        for (int right = 0; right < (int)nums.size(); right++) {
            // ---- 收缩 ----
            // 如果 nums[right] 已在窗口中, 从左边移除直到不重复
            // 用 while 不是 if: 可能要移除多个元素
            // 例: 窗口[5,2,1], right指向2 → 需要移除5和2
            while (seen.count(nums[right])) {
                seen.erase(nums[left]);
                sum -= nums[left];
                left++;
            }

            // ---- 扩展 ----
            seen.insert(nums[right]);
            sum += nums[right];

            // ---- 更新 ----
            ans = max(ans, sum);
        }
        return ans;
    }
};

// ============================================================
// 解法2: 滑动窗口 + 数组计数 — 常数更快
// 时间: O(n)  空间: O(max_val) = O(10^4)
//
// 【思路】
// 元素值范围 [1, 10^4], 用 int cnt[10001] 代替 unordered_set。
// cnt[val] 记录 val 在窗口中出现的次数。
//
// 优势: 数组访问比哈希表快很多 (无哈希计算, 缓存友好)。
//
// 流程:
//   1. 先 cnt[nums[right]]++ 并 sum += nums[right] (加入右端)
//   2. while cnt[nums[right]] > 1 → cnt[nums[left]]--, sum-=, left++ (收缩)
//   3. ans = max(ans, sum)
//
// 注意: 收缩条件是 cnt[nums[right]]>1, 不是 cnt[nums[left]]>1
//       因为我们要消除 right 端的重复
// ============================================================
class Solution2 {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        // cnt[v] = v 在当前窗口 [left, right] 中出现的次数
        int cnt[10001] = {};
        int left = 0, sum = 0, ans = 0;

        for (int right = 0; right < (int)nums.size(); right++) {
            // 先加入右端
            cnt[nums[right]]++;
            sum += nums[right];

            // 收缩: nums[right] 出现了不止一次 → 从左端移除
            while (cnt[nums[right]] > 1) {
                cnt[nums[left]]--;
                sum -= nums[left];
                left++;
            }

            // 此时窗口内无重复, 更新答案
            ans = max(ans, sum);
        }
        return ans;
    }
};

// ============================================================
// 解法3: 滑动窗口 + 前缀和
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 用前缀和数组代替累加变量 sum。
// 窗口和 = prefix[right+1] - prefix[left]。
//
// 这样不需要在收缩时手动减 sum, 直接用前缀和查询。
// 代码更干净但需要额外 O(n) 空间。
// ============================================================
class Solution3 {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        int n = nums.size();

        // 前缀和: prefix[i] = nums[0] + ... + nums[i-1]
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i + 1] = prefix[i] + nums[i];

        unordered_set<int> seen;
        int left = 0, ans = 0;

        for (int right = 0; right < n; right++) {
            // 收缩: 移除重复
            while (seen.count(nums[right])) {
                seen.erase(nums[left]);
                left++;
            }
            // 扩展
            seen.insert(nums[right]);
            // 窗口和直接用前缀和求 (不需要维护 sum 变量)
            ans = max(ans, prefix[right + 1] - prefix[left]);
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间  | 空间        | 优势                         | 劣势                    |
// |--------------|------|------------|-----------------------------|-----------------------|
// | set 判重      | O(n) | O(n)       | 通用, 不依赖值域范围           | 哈希表常数因子较大       |
// | 数组计数      | O(n) | O(max_val) | 最快, 缓存友好               | 仅适用于值域有限的情况    |
// | 前缀和+set   | O(n) | O(n)       | 不需要手动维护 sum            | 额外前缀和数组          |
//
// 面试选择: 先写解法1 (set), 如果面试官问优化就提数组计数法。
//
// ============================================================
// 【与 LC 3 的对比】
//
// | 维度     | LC 3 (最长无重复子串)      | LC 1695 (最大擦除值)          |
// |---------|--------------------------|------------------------------|
// | 输入     | 字符串                    | 正整数数组                    |
// | 目标     | max(right - left + 1)    | max(sum of window)           |
// | 窗口逻辑 | 完全相同                  | 完全相同                      |
// | 扩展操作 | 无额外操作                | sum += nums[right]           |
// | 收缩操作 | 无额外操作                | sum -= nums[left]            |
// | 更新答案 | ans = max(ans, len)      | ans = max(ans, sum)          |
//
// 如果你会 LC 3, 本题就是 "LC 3 + 维护窗口和"。
//
// ============================================================
// 【易错点】
//
// 1. 用 if 代替 while 收缩:
//    ✗ if (seen.count(nums[right])) { 只移除一个 }
//      → 窗口 [5,2,1], right=2时只移除5, 但2还在窗口中!
//    ✓ while (seen.count(nums[right])) { 持续收缩 }
//
// 2. 收缩时忘记更新 sum:
//    ✗ left++ 但不 sum -= nums[left-1] → sum 偏大
//    ✓ 先 sum -= nums[left], 再 left++
//    (或: sum -= nums[left]; seen.erase(nums[left]); left++;)
//
// 3. 数组计数法收缩条件搞反:
//    ✗ while (cnt[nums[left]] > 1) → left的元素可能只出现1次
//    ✓ while (cnt[nums[right]] > 1) → 要消除right端的重复
//
// 4. 混淆 "最大长度" 和 "最大和":
//    ✗ ans = max(ans, right - left + 1) → 这是 LC 3 的目标
//    ✓ ans = max(ans, sum) → 本题要最大和
//
// 5. 数组大小不够:
//    ✗ int cnt[10000] → 值10000会越界
//    ✓ int cnt[10001] (下标 0~10000)
//
// ============================================================
// 【面试追问】
//
// Q1: 时间复杂度怎么分析?
//     left 和 right 各从 0 移到 n-1, left 不回退。
//     while 中 left++ 的总次数 ≤ n。
//     所以 right 的 n 步 + left 的 ≤ n 步 = O(2n) = O(n)。
//
// Q2: 如果元素可以为负?
//     滑窗失效! 加入负数可能让和变小, 但后续正数可能弥补。
//     需要不同算法 (如 DP 或更复杂的数据结构)。
//
// Q3: 如果要求子数组长度至少为 k?
//     滑窗时, 只在 right-left+1 >= k 时才更新 ans。
//     或用定长窗口 + 可变扩展的组合方法。
//
// Q4: unordered_set 的最坏时间 O(n) 影响吗?
//     理论最坏 O(n²)(极端哈希冲突), 实际几乎不会。
//     用数组计数法 (解法2) 可以保证严格 O(n)。
//
// ============================================================
// 【相关题型】
//
// - 3. 无重复字符的最长子串:
//   本题的"模板题"。窗口逻辑完全相同, 只是目标从长度变和。
//
// - 209. 长度最小的子数组:
//   另一经典滑窗。目标是最小长度使和≥target (本题是最大和使元素不重复)。
//
// - 904. 水果成篮:
//   最多2种元素的最长子数组。滑窗+计数。
//
// - 992. K个不同整数的子数组:
//   进阶滑窗。恰好K个 = 最多K个 - 最多K-1个。
//
// - 2461. 长度为K子数组中的最大和:
//   固定窗口大小K, 元素不重复的最大和。本题是可变窗口版。
// ============================================================
