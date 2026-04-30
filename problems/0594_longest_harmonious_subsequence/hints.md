# 594. 最长和谐子序列 (Longest Harmonious Subsequence)

## 核心思路
"和谐子序列"的定义是最大值与最小值之差恰好为 1。由于子序列可以任意选取元素，这意味着和谐子序列中只包含**两种相邻整数值 x 和 x+1**，且两种值都必须至少出现一次。问题转化为：用**哈希表计数**，找出哪一对相邻值 `(x, x+1)` 的出现总次数最多。

## 思维链
1. 子序列不要求连续，可以从数组中任意挑选元素
2. "最大值 - 最小值 = 1" 意味着只能包含两种值，且差为 1
3. 如果只包含一种值（如 `[1,1,1]`），差为 0，不满足条件
4. 统计每个数字的出现次数 -> 遍历每个 key x -> 检查 x+1 是否存在
5. 如果 x+1 存在，`count[x] + count[x+1]` 就是以 x, x+1 为值的最长和谐子序列长度
6. 取所有这样的配对中的最大值

## 解法概览
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| 哈希表计数 | O(n) | O(n) | 最优，面试首选 |
| 排序 + 滑动窗口 | O(n log n) | O(1) | 不用额外空间（排序除外） |
| 排序 + 双指针 | O(n log n) | O(1) | 与滑窗等价的另一种写法 |

## 关键提示

### 提示 1 - 问题转化
和谐子序列中只有两种值 x 和 x+1（差恰好为 1）。所以答案 = max(count[x] + count[x+1]) for all x where x+1 also exists in array。

### 提示 2 - 哈希表一次遍历
```
unordered_map<int, int> count;
for (int num : nums) count[num]++;
```
然后遍历 map 中的每个 key，检查 key+1 是否存在。

### 提示 3 - 排序法的滑动窗口思路
排序后，所有相同值聚集在一起。用两个指针 left 和 right 维护窗口，当 `nums[right] - nums[left] > 1` 时收缩左端。当差恰好为 1 时更新答案。

### 提示 4 - 边界情况
- 所有元素相同 -> 返回 0（差为 0，不是 1）
- 数组为空或只有一个元素 -> 返回 0
- 负数同样适用，`(-2, -1)` 差为 1

## 解法详解

### 解法一：哈希表计数（推荐）
```
class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }

        int result = 0;
        for (auto& [val, cnt] : count) {
            if (count.count(val + 1)) {
                result = max(result, cnt + count[val + 1]);
            }
        }
        return result;
    }
};
```
**逻辑说明**：
- 第一遍统计频率
- 第二遍枚举每个值 x，检查 x+1 是否存在
- 只查 x+1 不查 x-1，避免重复计算（每对只被统计一次）

### 解法二：排序 + 滑动窗口
```
class Solution {
public:
    int findLHS(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int result = 0, left = 0;

        for (int right = 0; right < (int)nums.size(); right++) {
            // 收缩左端直到差 <= 1
            while (nums[right] - nums[left] > 1) {
                left++;
            }
            // 差恰好为 1 时才更新（差为 0 不算）
            if (nums[right] - nums[left] == 1) {
                result = max(result, right - left + 1);
            }
        }
        return result;
    }
};
```
**逻辑说明**：
- 排序后，窗口 [left, right] 内的最小值是 nums[left]，最大值是 nums[right]
- 如果差 > 1，移动 left
- 只有差恰好为 1 时才能构成和谐子序列

### 解法三：一次遍历哈希表（边插入边查询）
```
class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> count;
        int result = 0;

        for (int num : nums) {
            count[num]++;
            // 检查 num-1 和 num+1
            if (count.count(num - 1))
                result = max(result, count[num] + count[num - 1]);
            if (count.count(num + 1))
                result = max(result, count[num] + count[num + 1]);
        }
        return result;
    }
};
```

## 易错点
1. **差为 0 不是和谐子序列**：`[1,1,1]` 应返回 0 而非 3，因为 max-min=0 不等于 1
2. **只查 x+1 或只查 x-1**：在两遍法中只查一个方向即可；但一遍法需要双向查
3. **忘记 x+1 必须存在**：如果只有一种值，不能计入结果
4. **排序法中差为 0 误更新**：窗口内差为 0 时不应更新答案
5. **负数处理**：`(-3, -2)` 差为 1，哈希表天然支持负数 key

## 面试追问
1. **如果要求差恰好为 k（而非 1）？** - 同样的哈希表方法，检查 x+k 是否存在
2. **如果要求连续子数组而非子序列？** - 需要滑动窗口，但条件变为窗口内最大-最小=1
3. **能否 O(1) 空间？** - 排序法只需 O(1) 额外空间（如果允许修改输入）
4. **如何处理极大数据量（不能全部放内存）？** - 外部排序或 MapReduce 分桶统计
5. **如果允许差为 0 或 1？** - 差为 0 时就是出现次数最多的单个值

## 相关题型
- [128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) - 最长连续序列（值连续）
- [485. Max Consecutive Ones](https://leetcode.com/problems/max-consecutive-ones/) - 最长连续 1
- [532. K-diff Pairs in an Array](https://leetcode.com/problems/k-diff-pairs-in-an-array/) - 差为 k 的数对
- [1. Two Sum](https://leetcode.com/problems/two-sum/) - 哈希表查找配对
- [424. Longest Repeating Character Replacement](https://leetcode.com/problems/longest-repeating-character-replacement/) - 滑动窗口经典
