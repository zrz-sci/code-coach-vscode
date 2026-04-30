/*
 * 【题目本质】
 * 给定已排序正整数数组和目标 n，求最少补几个数使得 [1,n] 中每个整数
 * 都能被数组某个子集之和表示。
 * 核心是贪心: 维护"当前能覆盖的最小缺口 miss"，补 miss 本身使覆盖翻倍。
 *
 * 【解法总览】
 * 解法1: 贪心法 — O(m + log n)/O(1) ⭐唯一最优解
 */

// ===================== 解法1: 贪心 =====================
// 思路:
//   miss = 当前无法表示的最小正整数
//   不变式: [1, miss-1] 中每个数都已可表示
//   - 若 nums[i] <= miss: 加入后覆盖扩展到 [1, miss+nums[i]-1]
//   - 若 nums[i] > miss 或数组用完: 补 miss 本身，覆盖翻倍到 [1, 2*miss-1]
// 时间: O(m + log n) 其中 m = nums.size()
// 空间: O(1)
class Solution1 {
public:
    int minPatches(vector<int>& nums, int n) {
        long long miss = 1; // 用 long long 防止翻倍溢出
        int patches = 0;
        int i = 0;
        int m = nums.size();

        while (miss <= n) {
            if (i < m && (long long)nums[i] <= miss) {
                // 当前数组元素可用，扩展覆盖范围
                miss += nums[i];
                i++;
            } else {
                // 必须补一个数: 补 miss 本身最贪心
                // 覆盖从 [1, miss-1] 扩展到 [1, 2*miss-1]
                miss += miss;
                patches++;
            }
        }

        return patches;
    }
};

// ===================== 解法2: 带详细注释的贪心 =====================
// 同一算法，增加了执行过程追踪，用于理解
class Solution2 {
public:
    int minPatches(vector<int>& nums, int n) {
        long long miss = 1;
        int patches = 0, i = 0;

        while (miss <= (long long)n) {
            if (i < (int)nums.size() && nums[i] <= miss) {
                // nums[i] 在当前覆盖范围内
                // 原来能表示 [1, miss-1]
                // 每个原来的和 s 都可以变成 s + nums[i]
                // 新增覆盖 [nums[i], miss-1+nums[i]]
                // 与原来的 [1, miss-1] 合并为 [1, miss+nums[i]-1]
                miss += nums[i++];
            } else {
                // 有空洞: miss 无法被表示
                // 补 miss 是最优选择:
                //   补更小的值 → 仍无法覆盖 miss
                //   补更大的值 → 覆盖范围不如补 miss 大
                miss <<= 1; // miss *= 2
                patches++;
            }
        }

        return patches;
    }
};

/*
 * 【解法对比】
 * 本题只有贪心一种最优解法。
 * 关键理解: 为什么补 miss 本身是最优的？
 * - 补任何 x < miss 的值: x 已经可以表示，不增加新覆盖
 *   (不对，x 和原有的和组合确实能扩展，但 miss 仍是空洞)
 * - 补 miss: 覆盖范围直接翻倍 [1, 2*miss-1]
 * - 补任何 x > miss: 无法覆盖 miss 本身，下次还得补
 *
 * 【易错点】
 * 1. miss 用 int → n 最大 2^31-1，翻倍溢出
 * 2. while 条件用 < 而非 <= → miss==n 时 n 还没被覆盖
 * 3. 忘记 i < m 的边界检查 → 数组用完后仍需继续补数
 * 4. nums[i] 比较时未转 long long → 隐式转换可能有符号问题
 *
 * 【面试追问】
 * Q1: 贪心正确性证明？→ 归纳: [1,miss-1] 已覆盖时，补 miss 得到最大扩展
 * Q2: 数组未排序？→ 先排序，O(m log m + log n)
 * Q3: 最多补几个？→ O(log n)，因为每次补数 miss 翻倍
 * Q4: 和 LC 1798 的关系？→ 1798 不需补数，只用给定硬币，但贪心思路完全一致
 */
