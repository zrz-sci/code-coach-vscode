# 325. 和等于 k 的最长子数组长度 (Medium)

**Tags:** Array, Hash Table, Prefix Sum

## 核心思路

利用**前缀和 + 哈希表**将"子数组和等于 k"转化为"两个前缀和之差等于 k"的查找问题。关键在于只记录每个前缀和**第一次出现**的位置，以保证子数组长度最大化。

## 思维链

1. 暴力枚举所有子数组 O(n^2) 太慢 → 需要优化
2. 子数组和 = prefix[j] - prefix[i] = k → 对于每个 j，查找 prefix[j] - k 是否出现过
3. 用哈希表记录前缀和 → 位置映射
4. 要求最长子数组 → 只保留前缀和第一次出现的索引（越早出现，子数组越长）
5. 注意初始化 map[0] = -1（空前缀的位置）

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| 前缀和 + 哈希表 | O(n) | O(n) | ⭐ |
| 暴力枚举 | O(n^2) | O(1) | - |

## 关键提示

1. **前缀和定义**：prefix[i] = nums[0] + nums[1] + ... + nums[i-1]，则 sum(i..j) = prefix[j+1] - prefix[i]
2. **哈希表作用**：存储 {前缀和值 → 最早出现的索引}
3. **只记录首次**：如果同一个前缀和出现多次，只保留第一次的索引，这样 j - i 才最大
4. **初始化**：map[0] = -1 表示"前缀和为 0 在下标 -1 处出现"，处理从头开始的子数组
5. **元素可以为负数**：因此不能用滑动窗口（窗口扩大/缩小时和的单调性不成立）

## 解法详解

### 解法一：前缀和 + 哈希表 (推荐)

```cpp
class Solution {
public:
    int maxSubArrayLen(vector<int>& nums, int k) {
        unordered_map<long long, int> prefixMap; // 前缀和 → 最早出现的索引
        prefixMap[0] = -1; // 空前缀，位置 -1
        
        long long sum = 0;
        int maxLen = 0;
        
        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];
            
            // 查找是否存在 prefix = sum - k
            if (prefixMap.count(sum - k)) {
                maxLen = max(maxLen, i - prefixMap[sum - k]);
            }
            
            // 只记录第一次出现的位置
            if (!prefixMap.count(sum)) {
                prefixMap[sum] = i;
            }
        }
        
        return maxLen;
    }
};
```

### 解法二：暴力枚举

```cpp
class Solution {
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
```

## 易错点

| 错误写法 | 正确写法 | 原因 |
|----------|----------|------|
| ✗ 不初始化 `map[0]=-1` | ✓ `prefixMap[0] = -1` | 漏掉从数组开头开始的子数组 |
| ✗ 每次都更新 `map[sum]=i` | ✓ 只在首次出现时记录 | 更新后 i 变大，子数组变短 |
| ✗ 用 `int` 存前缀和 | ✓ 用 `long long` | nums[i] 范围 +-10^4，n 达 2*10^5，和可能溢出 |
| ✗ 尝试用滑动窗口 | ✓ 用前缀和+哈希 | 数组含负数，窗口和不单调 |

## 面试追问

**Q1: 为什么不能用滑动窗口？**
→ 滑动窗口要求窗口扩大时和单调增大、缩小时和单调减小。数组含负数时，加入元素可能让和变小，无法判断应该扩大还是缩小窗口。

**Q2: 如果数组全是正数，能否用滑动窗口？**
→ 可以。全正数时窗口和单调，左右指针向右移动即可，时间 O(n)、空间 O(1)。

**Q3: 如果要求的是"和等于 k 的子数组个数"怎么办？**
→ 类似 LC 560，哈希表存的不再是索引而是前缀和出现的次数 `map[sum]++`，每次累加 `map[sum-k]`。

## 相关题型

- [560. 和为 K 的子数组](https://leetcode.com/problems/subarray-sum-equals-k/) - 前缀和+哈希计数
- [525. 连续数组](https://leetcode.com/problems/contiguous-array/) - 前缀和+哈希 (0/1 数组)
- [209. 长度最小的子数组](https://leetcode.com/problems/minimum-size-subarray-sum/) - 正数滑动窗口
- [862. 和至少为 K 的最短子数组](https://leetcode.com/problems/shortest-subarray-with-at-least-k-sum/) - 前缀和+单调队列
