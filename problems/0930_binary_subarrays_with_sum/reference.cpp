/*
 * LeetCode 930: Binary Subarrays With Sum (和相同的二元子数组)
 *
 * 【题目本质】
 * 计算二进制数组中和为 goal 的非空子数组个数。
 * 经典的前缀和+哈希表模式，或滑动窗口的 atMost 技巧。
 *
 * 【解法总览】
 * 解法1: 前缀和 + 哈希表 — O(n) / O(n) — 通用解法，面试首选
 * 解法2: 滑动窗口 atMost — O(n) / O(1) — 利用非负特性，空间更优
 */

// ============================================================
// 解法1: 前缀和 + 哈希表
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 与 560 题（和为K的子数组）完全相同的模板。
//
// 核心等式: sum(i..j) = prefix[j+1] - prefix[i]
// 如果 prefix[j+1] - prefix[i] == goal
// 则 prefix[i] == prefix[j+1] - goal
//
// 算法：
//   维护一个哈希表 prefixCount，记录每个前缀和出现的次数。
//   遍历数组，累加前缀和。对每个 prefix：
//     1. 查表: prefix - goal 出现了几次 → 就有几个满足条件的子数组
//     2. 存表: prefixCount[prefix]++
//
// 关键初始化: prefixCount[0] = 1
//   表示空前缀（前0个元素的和为0）出现过1次。
//   这处理了"从数组开头到当前位置的和恰好等于 goal"的情况。
//
// 示例: nums = [1,0,1,0,1], goal = 2
//   j=0: prefix=1, 查1-2=-1→0个, map={0:1,1:1}
//   j=1: prefix=1, 查1-2=-1→0个, map={0:1,1:2}
//   j=2: prefix=2, 查2-2=0 →1个, map={0:1,1:2,2:1}
//   j=3: prefix=2, 查2-2=0 →1个, map={0:1,1:2,2:2}
//   j=4: prefix=3, 查3-2=1 →2个, map={0:1,1:2,2:2,3:1}
//   total = 0+0+1+1+2 = 4 ✓
//
// 示例: nums = [0,0,0,0,0], goal = 0
//   j=0: prefix=0, 查0-0=0→1个, map={0:2}
//   j=1: prefix=0, 查0-0=0→2个, map={0:3}
//   j=2: prefix=0, 查0-0=0→3个, map={0:4}
//   j=3: prefix=0, 查0-0=0→4个, map={0:5}
//   j=4: prefix=0, 查0-0=0→5个, map={0:6}
//   total = 1+2+3+4+5 = 15 ✓
// ============================================================
class Solution1 {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        // key: 前缀和值, value: 出现次数
        unordered_map<int, int> prefixCount;
        prefixCount[0] = 1; // 空前缀：和为0出现1次

        int prefix = 0; // 当前前缀和
        int result = 0;

        for (int num : nums) {
            prefix += num;

            // 查找: 之前有多少个前缀和等于 prefix - goal
            // 每一个这样的前缀和都对应一个满足条件的子数组
            auto it = prefixCount.find(prefix - goal);
            if (it != prefixCount.end()) {
                result += it->second;
            }

            // 存入: 记录当前前缀和（供后续元素查找）
            prefixCount[prefix]++;
        }

        return result;
    }
};

// ============================================================
// 解法2: 滑动窗口 — atMost(goal) - atMost(goal-1)
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 直接用滑动窗口求"恰好等于 goal"的子数组数很难，
// 因为窗口和等于 goal 时左边界不唯一（中间可能有0）。
//
// 技巧：
//   count(== goal) = count(<= goal) - count(<= goal - 1)
//
// count(<= X) 可以用标准滑动窗口求：
//   维护窗口 [left, right]，使窗口和 <= X。
//   每当右指针扩展后和 > X，收缩左指针。
//   以 right 为右端点的合法子数组数 = right - left + 1
//
// 为什么 right - left + 1？
//   所有左端点在 [left, left+1, ..., right] 的子数组
//   的和都 <= X（因为 left 是最小的合法左端点，
//   更大的左端点只会让和更小/不变）。
//
// 为什么滑动窗口可以用？
//   数组元素全非负 → 右移 right 使和增大或不变，
//   右移 left 使和减小或不变 → 满足单调性。
//
// 示例: nums = [1,0,1,0,1], goal = 2
//   atMost(2) = 14, atMost(1) = 10 → 14 - 10 = 4 ✓
// ============================================================
class Solution2 {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        // 恰好等于 goal = 至多 goal 减去 至多 goal-1
        return atMost(nums, goal) - atMost(nums, goal - 1);
    }

private:
    // 计算和 <= goal 的非空子数组个数
    int atMost(vector<int>& nums, int goal) {
        // 特殊情况: goal < 0 时不存在和为负的子数组（元素全非负）
        if (goal < 0) return 0;

        int n = nums.size();
        int left = 0;
        int windowSum = 0;
        int count = 0;

        for (int right = 0; right < n; right++) {
            windowSum += nums[right];

            // 收缩左指针直到窗口和 <= goal
            while (windowSum > goal) {
                windowSum -= nums[left];
                left++;
            }

            // 以 right 为右端点，左端点 ∈ [left, right] 的子数组都满足
            count += right - left + 1;
        }

        return count;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间  | 空间  | 通用性              | 适用场景         |
// |----------------|-------|-------|--------------------|-----------------|
// | 前缀和+哈希表   | O(n)  | O(n)  | 任意整数数组均可     | 通用首选         |
// | 滑动窗口 atMost | O(n)  | O(1)  | 仅限非负整数数组     | 需要 O(1) 空间时 |
//
// 面试推荐：先讲前缀和（通用、好写），再讲滑动窗口（展示空间优化能力）。
//
// ============================================================
// 【易错点】
//
// 1. 前缀和忘记初始化 prefixCount[0] = 1:
//    ✗ 漏掉从数组开头开始、和恰好等于 goal 的子数组
//    ✓ prefixCount[0] = 1 表示空前缀的和为0出现过1次
//
// 2. 滑动窗口 atMost 未处理 goal < 0:
//    ✗ goal=0 时 atMost(-1) 的 while 循环可能导致 left > right
//    ✓ goal < 0 时直接返回 0
//
// 3. 滑动窗口贡献量写成 1 而非 right-left+1:
//    ✗ count++ → 只计了以 right 结尾的一个子数组
//    ✓ count += right - left + 1 → 计所有合法左端点
//
// 4. 前缀和方法先存后查:
//    ✗ 当 goal==0 时 prefix-goal==prefix，会匹配自己刚存入的值
//    ✓ 必须先查再存
//
// ============================================================
// 【相关题型】
//
// - 560. 和为 K 的子数组:
//   本题的通用版本。代码几乎完全相同。
//
// - 992. K 个不同整数的子数组:
//   同样用 atMost(K) - atMost(K-1) 技巧，
//   但统计维度从"和"变为"不同整数个数"。
//
// - 1248. 统计「优美子数组」:
//   恰好含 K 个奇数的子数组数量。
//   将奇数看作1、偶数看作0，就退化为本题。
// ============================================================
