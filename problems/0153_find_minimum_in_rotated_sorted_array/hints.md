# 153. 寻找旋转排序数组中的最小值 (Find Minimum in Rotated Sorted Array)

## 核心思路

旋转排序数组可以看作两个有序子段拼接，最小值就在"断裂点"。利用 **二分搜索**，每次比较 `nums[mid]` 和 `nums[hi]`，判断最小值在左半还是右半，逐步缩小搜索空间。

**关键判断**：
- `nums[mid] > nums[hi]` → 最小值在右半段（`mid+1` 到 `hi`）
- `nums[mid] <= nums[hi]` → 最小值在左半段（`lo` 到 `mid`，含 mid）

## 思维链

1. **读完题第一反应** → 找最小值？直接 O(n) 扫一遍就完了。但题目要求 O(log n)，必须用二分。
2. **二分搜索需要什么？** → 需要一个**判定条件**，能把搜索空间分成"一定不在"和"可能在"两半。
3. **旋转数组的结构是什么？** → 原始升序数组 `[1,2,3,4,5,6,7]` 旋转后变成 `[4,5,6,7,|1,2,3]`，由一个"大段"和一个"小段"拼接，最小值在小段的开头。
4. **怎么判断 mid 在哪一段？** → 比较 `nums[mid]` 和 `nums[hi]`：
   - `nums[mid] > nums[hi]` → mid 在大段，最小值在 mid 右边
   - `nums[mid] <= nums[hi]` → mid 在小段（或没有旋转），最小值在 mid 左边或就是 mid
5. **为什么比较 nums[hi] 而不是 nums[lo]？** → 如果数组没有旋转（纯升序），`nums[lo] < nums[mid]` 但最小值在 lo，用 lo 比较会导致误判。

## 搜索空间收缩图解

```
示例: nums = [4, 5, 6, 7, 0, 1, 2]

初始搜索空间:
  [4, 5, 6, 7, 0, 1, 2]
   lo          mid       hi
   0           3          6
   nums[mid]=7 > nums[hi]=2 → 最小值在右半！ lo = mid + 1 = 4

第二轮:
  [4, 5, 6, 7, 0, 1, 2]
                  lo mid  hi
                  4   5    6
   nums[mid]=1 <= nums[hi]=2 → 最小值在左半(含mid)！ hi = mid = 5

第三轮:
  [4, 5, 6, 7, 0, 1, 2]
                  lo  hi
                  4    5
                  mid
   nums[mid]=0 <= nums[hi]=1 → 最小值在左半(含mid)！ hi = mid = 4

退出: lo == hi == 4 → nums[4] = 0 ✓


示例: nums = [3, 4, 5, 1, 2]

  Round 1:  [3, 4, 5, 1, 2]       lo=0, hi=4, mid=2
             lo    mid     hi      nums[2]=5 > nums[4]=2 → lo=3

  Round 2:  [3, 4, 5, 1, 2]       lo=3, hi=4, mid=3
                      lo  hi       nums[3]=1 <= nums[4]=2 → hi=3
                      mid

  退出: lo == hi == 3 → nums[3] = 1 ✓


示例: nums = [11, 13, 15, 17] (未旋转)

  Round 1:  [11, 13, 15, 17]      lo=0, hi=3, mid=1
             lo  mid      hi      nums[1]=13 <= nums[3]=17 → hi=1

  Round 2:  [11, 13, 15, 17]      lo=0, hi=1, mid=0
             lo  hi                nums[0]=11 <= nums[1]=13 → hi=0
             mid

  退出: lo == hi == 0 → nums[0] = 11 ✓


搜索空间收缩总结:

  情况A: nums[mid] > nums[hi]
  ┌─────────────────────────────────┐
  │  [大段...  mid  ...] [小段...]  │
  │           ↑                     │
  │     mid在大段，断裂点在右边     │
  │     lo = mid + 1               │
  └─────────────────────────────────┘

  情况B: nums[mid] <= nums[hi]
  ┌─────────────────────────────────┐
  │  [...] [小段... mid ... hi]     │
  │                  ↑              │
  │     mid在小段(或无旋转)         │
  │     最小值可能就是 mid          │
  │     hi = mid (不是 mid-1!)     │
  └─────────────────────────────────┘
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 二分（比较 mid 和 hi） | 标准做法 | O(log n) | O(1) | 必须写出 |
| 二分（比较 mid 和 lo） | 可行但需小心 | O(log n) | O(1) | 了解即可 |
| 线性扫描 | 遍历找最小 | O(n) | O(1) | 说出但不写 |

## 关键提示

1. **为什么比较 `nums[hi]` 不比较 `nums[lo]`？**
   - 未旋转时 `[1,2,3,4,5]`：`nums[lo]=1 < nums[mid]=3`，但最小值在 lo。如果用 `nums[mid] > nums[lo]` → `lo = mid+1` 就跳过了最小值！
   - 用 `nums[hi]` 没有这个问题：未旋转时 `nums[mid] < nums[hi]` → `hi = mid`，正确保留了最小值。

2. **`hi = mid` 不是 `hi = mid - 1`**：因为 `nums[mid]` 可能就是最小值，不能跳过它。

3. **`lo = mid + 1`**：因为 `nums[mid] > nums[hi]` 时，`mid` 一定不是最小值（右边有更小的），可以安全跳过。

4. **循环终止条件 `lo < hi`**：不是 `lo <= hi`。终止时 `lo == hi`，指向最小值。

5. **无重复元素**：题目保证元素互不相同，所以不需要处理 `nums[mid] == nums[hi]` 的情况（那是 LC 154 的内容）。

## 解法详解

### 解法1: 二分（比较 mid 和 hi） — O(log n) / O(1) ⭐ 必须掌握

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        int lo = 0, hi = nums.size() - 1;
        
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            
            if (nums[mid] > nums[hi]) {
                // mid 在大段，断裂点（最小值）在 mid 右边
                lo = mid + 1;
            } else {
                // mid 在小段或无旋转，最小值在 mid 或 mid 左边
                hi = mid;
            }
        }
        
        return nums[lo]; // lo == hi，指向最小值
    }
};
```

**逐行解析**:
- `lo + (hi - lo) / 2`：防溢出写法，等价于 `(lo + hi) / 2`。
- `nums[mid] > nums[hi]`：说明 mid 到 hi 之间有断裂，最小值在 `[mid+1, hi]`。
- `else hi = mid`：最小值在 `[lo, mid]`，注意包含 mid。
- `return nums[lo]`：循环结束时 `lo == hi`，两者都指向最小值。

---

### 解法2: 二分（比较 mid 和 lo） — O(log n) / O(1)

**另一种写法，不太推荐但值得理解**:

```
为什么比较 lo 更复杂？

nums = [1, 2, 3, 4, 5] (未旋转)
lo=0, mid=2, hi=4
nums[mid]=3 > nums[lo]=1  →  最小值应该在左半...
                                但如果 lo = mid+1 就跳过了！

必须额外检查: 如果 nums[lo] <= nums[hi]，说明没旋转，直接返回 nums[lo]。
```

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        int lo = 0, hi = nums.size() - 1;
        
        while (lo < hi) {
            // 如果当前区间已经有序，直接返回 lo
            if (nums[lo] < nums[hi]) return nums[lo];
            
            int mid = lo + (hi - lo) / 2;
            
            if (nums[mid] >= nums[lo]) {
                lo = mid + 1; // mid 在大段
            } else {
                hi = mid; // mid 在小段
            }
        }
        
        return nums[lo];
    }
};
```

**关键点**:
- 需要额外的 `nums[lo] < nums[hi]` 判断来处理未旋转的情况。
- 解法1 不需要这个判断，更简洁，所以推荐解法1。

## 面试应对

### 面试官可能追问

**Q: 如果数组有重复元素？（LC 154）**

A: 当 `nums[mid] == nums[hi]` 时，无法判断最小值在左还是右。解决方法：`hi--`（只缩小一步）。最坏时间退化到 O(n)，例如 `[1,1,1,1,0,1,1,1]`。

```
LC 154 的额外逻辑:
if (nums[mid] > nums[hi])     lo = mid + 1;
else if (nums[mid] < nums[hi]) hi = mid;
else                            hi--;  // 无法判断，保守缩小
```

**Q: 如果要在旋转数组中搜索目标值？（LC 33）**

A: 先用二分找到最小值的位置（旋转点），然后在正确的半段中做标准二分搜索。或者直接用一次二分，根据 `mid` 和 `target` 相对于旋转点的位置来判断。

**Q: 为什么不和 nums[lo] 比？能详细解释吗？**

```
反例: nums = [1, 2, 3] (未旋转)

用 nums[lo] 比较:
  lo=0, hi=2, mid=1
  nums[mid]=2 > nums[lo]=1 → lo = mid+1 = 2
  return nums[2] = 3  ← 错误！最小值是 1

用 nums[hi] 比较:
  lo=0, hi=2, mid=1
  nums[mid]=2 <= nums[hi]=3 → hi = mid = 1
  lo=0, hi=1, mid=0
  nums[mid]=1 <= nums[hi]=2 → hi = mid = 0
  return nums[0] = 1  ← 正确！
```

**Q: 这个二分模板的循环不变量是什么？**

A: **最小值始终在 `[lo, hi]` 范围内**。
- 初始时：`lo=0, hi=n-1`，最小值一定在数组中。
- `lo = mid + 1` 时：`nums[mid] > nums[hi]`，mid 不可能是最小值，安全排除。
- `hi = mid` 时：mid 可能是最小值，必须保留。
- 终止时：`lo == hi`，范围缩小到一个元素，就是最小值。

## 二分模板分类

```
本题使用的是"找左边界"模板:

while (lo < hi) {          ← 终止条件: lo == hi
    mid = lo + (hi-lo)/2;
    if (condition)
        lo = mid + 1;      ← 排除 mid
    else
        hi = mid;           ← 保留 mid
}
return lo;                  ← lo == hi 就是答案

另一种常见模板"标准二分":
while (lo <= hi) {          ← 终止条件: lo > hi
    mid = lo + (hi-lo)/2;
    if (found) return mid;
    if (go_right) lo = mid + 1;
    else hi = mid - 1;
}
return -1;                  ← 没找到

本题用第一种模板，因为:
1. 不存在"精确匹配"的情况，我们要的是一个范围收缩到一个点
2. hi = mid（不是 mid-1），因为 mid 可能就是答案
3. 循环结束时 lo == hi，自然就是最小值的位置
```

## 易错点

1. **比较对象是 `nums[hi]` 不是 `nums[lo]`**：用 `nums[lo]` 在未旋转时会出错。
2. **`hi = mid` 不是 `hi = mid - 1`**：mid 可能就是最小值。
3. **`lo < hi` 不是 `lo <= hi`**：用 `<=` 会死循环（当 `lo == hi` 时 `mid == lo == hi`，`hi = mid` 不减少搜索空间）。
4. **不需要处理 `nums[mid] == nums[hi]`**：题目保证无重复元素（有重复是 LC 154）。

## 相关题目

| 题目 | 关联 |
|------|------|
| [154. 寻找旋转排序数组中的最小值 II](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array-ii/) | 有重复元素 |
| [33. 搜索旋转排序数组](https://leetcode.com/problems/search-in-rotated-sorted-array/) | 在旋转数组中搜索目标值 |
| [81. 搜索旋转排序数组 II](https://leetcode.com/problems/search-in-rotated-sorted-array-ii/) | 有重复 + 搜索 |
| [852. 山脉数组的峰顶索引](https://leetcode.com/problems/peak-index-in-a-mountain-array/) | 类似的二分判定逻辑 |

## Follow-up 思考

1. **旋转了 0 次的情况？** → 题目说旋转 1~n 次，旋转 n 次等于原数组，所以纯升序也是合法输入。我们的解法正确处理。
2. **能否返回最小值的索引而非值？** → `return lo` 而不是 `return nums[lo]`。
3. **找最大值？** → 最大值在最小值的前一个位置：`(minIdx - 1 + n) % n`。
