# 330. 按要求补齐数组 (Hard)

**Tags:** Greedy, Array

## 核心思路

贪心法：维护一个变量 `miss` 表示当前**无法表示的最小正整数**。如果数组当前元素 `nums[i] <= miss`，则可以用它扩展覆盖范围到 `miss + nums[i]`；否则必须补一个数 `miss` 本身，使覆盖范围翻倍到 `2 * miss`。

## 思维链

1. 初始时能表示的范围为空，miss = 1（第一个无法表示的正整数）
2. 如果 nums[i] <= miss：加入 nums[i] 后，原来能表示 [1, miss-1]，现在每个原和都可以 +nums[i]，覆盖扩展到 [1, miss + nums[i] - 1]，更新 miss += nums[i]
3. 如果 nums[i] > miss：有"空洞"，必须补一个数。补 miss 本身最贪心（覆盖范围最大翻倍），miss *= 2，patches++
4. 重复直到 miss > n

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| 贪心法 | O(m + log n) | O(1) | ⭐ |

其中 m 为 nums.length

## 关键提示

1. **miss 的含义**：[1, miss-1] 中的每个整数都能被当前数组的某个子集之和表示
2. **为什么补 miss 最优**：补比 miss 小的值不能填补 miss 这个空洞；补比 miss 大的值浪费了，不能覆盖 miss
3. **miss 必须用 long long**：n 最大 2^31 - 1，miss 翻倍过程中可能超过 int 范围
4. **数组已排序**：保证我们从小到大处理元素，确保贪心正确性
5. **复杂度分析**：每次补数时 miss 翻倍，最多补 O(log n) 次；遍历数组 O(m) 次。总计 O(m + log n)

## 解法详解

### 解法一：贪心 (唯一最优解)

```cpp
class Solution {
public:
    int minPatches(vector<int>& nums, int n) {
        long long miss = 1; // 当前无法表示的最小正整数
        int patches = 0;
        int i = 0;
        
        while (miss <= n) {
            if (i < nums.size() && nums[i] <= miss) {
                // nums[i] 可以使用，扩展覆盖范围
                miss += nums[i];
                i++;
            } else {
                // 必须补一个数，补 miss 本身最贪心
                miss += miss; // 等价于 miss *= 2
                patches++;
            }
        }
        
        return patches;
    }
};
```

**执行过程示例** (nums=[1,5,10], n=20):
```
初始: miss=1, patches=0
i=0: nums[0]=1 <= miss=1 → miss=1+1=2, i=1
i=1: nums[1]=5 > miss=2 → 补2, miss=2+2=4, patches=1
i=1: nums[1]=5 > miss=4 → 补4, miss=4+4=8, patches=2
i=1: nums[1]=5 <= miss=8 → miss=8+5=13, i=2
i=2: nums[2]=10 <= miss=13 → miss=13+10=23, i=3
miss=23 > n=20, 结束。patches=2
```

## 易错点

| 错误写法 | 正确写法 | 原因 |
|----------|----------|------|
| ✗ `int miss = 1` | ✓ `long long miss = 1` | n 最大 2^31-1，miss 翻倍时溢出 |
| ✗ `while (miss < n)` | ✓ `while (miss <= n)` | miss==n 时 n 本身还没被覆盖 |
| ✗ 补 miss+1 | ✓ 补 miss 本身 | 补 miss+1 无法覆盖 miss 这个空洞 |
| ✗ 先补数再检查数组 | ✓ 先检查数组再补数 | 数组中可能有可用元素，优先使用 |

## 面试追问

**Q1: 为什么贪心是正确的？**
→ 归纳证明：当 [1, miss-1] 已被覆盖时，能选的最大扩展就是补 miss（覆盖翻倍到 [1, 2*miss-1]），补任何更小值覆盖范围更小，更大值无法覆盖 miss。

**Q2: 如果数组未排序怎么办？**
→ 先排序 O(m log m)，总复杂度变为 O(m log m + log n)。排序是贪心正确性的前提。

**Q3: 如果不只覆盖 [1,n] 而是 [0,n] 呢？**
→ 空子集和为 0，所以 0 天然被覆盖。初始 miss=1 不变，逻辑完全相同。

## 相关题型

- [1798. 你能构造出连续值的最大数目](https://leetcode.com/problems/maximum-number-of-consecutive-values-you-can-make/) - 同样的贪心思路
- [1400. 构造 K 个回文字符串](https://leetcode.com/problems/construct-k-palindrome-strings/) - 贪心构造
- [455. 分发饼干](https://leetcode.com/problems/assign-cookies/) - 排序+贪心
