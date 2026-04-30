# 3107. 使数组中位数等于 K 的最少操作数 (Minimum Operations to Make Median of Array Equal to K)

## 难度: Medium | 标签: Greedy, Array, Sorting

---

## 题目理解

给定整数数组 `nums` 和非负整数 `k`。每次操作可以将任一元素 **+1 或 -1**。求使数组**中位数**等于 `k` 的**最少操作次数**。

**中位数定义**: 数组排序后的中间元素。若长度为偶数，取中间两个的**较大者**。

```
长度 5: [a, b, c, d, e] 排序后中位数 = c (下标 2 = n/2)
长度 6: [a, b, c, d, e, f] 排序后中位数 = d (下标 3 = n/2, 取较大者)
统一公式: m = n / 2
```

**操作理解**: 把元素从 `x` 变成 `y` 需要 `|x - y|` 次操作 (绝对值距离)。

---

## 思维链 -- 从暴力到贪心

### Step 1: 中位数是排序后的固定位置

无论怎么操作元素，中位数永远是**排序后下标 `m = n/2` 处的元素**。

所以问题变成: **调整元素使得排序后 `nums[m] == k`**。

### Step 2: 只把中位数改成 k 够吗？

**不够!** 修改中位数后需要重新排序，可能导致中位数位置被其他元素占据。

```
排序后: [2, 5, 5, 6, 8]    m=2, nums[m]=5, 需要改成 k=4
如果只把 nums[2]=5 改成 4: [2, 5, 4, 6, 8]
重新排序: [2, 4, 5, 6, 8]   中位数 = 5, 不是 4!

原因: nums[1]=5 > k=4, 排序后它跑到了中位数位置。
所以还需要把 nums[1]=5 也减到 4。
```

### Step 3: 关键洞察 -- 只需处理一侧

排序后: `nums[0] <= nums[1] <= ... <= nums[m] <= ... <= nums[n-1]`

**情况 A: `k <= nums[m]` (需要减小中位数)**

```
排序后:  [ ... 左半部分 ... | nums[m] | ... 右半部分 ... ]
                              ^  需要减小到 k

左半部分: 可能有元素 > k，必须减到 k
  否则排序后这些元素会跑到 m 位置，中位数仍 > k

右半部分: 所有元素 >= nums[m] >= k，已经满足 >= k
  不需要动 (它们在 m 右边，不影响中位数)

结论: 从 m 向左扫，把所有 > k 的减到 k
```

**情况 B: `k > nums[m]` (需要增大中位数)**

```
排序后:  [ ... 左半部分 ... | nums[m] | ... 右半部分 ... ]
                              ^  需要增大到 k

右半部分: 可能有元素 < k，必须加到 k
  否则排序后这些元素会跑到 m 位置，中位数仍 < k

左半部分: 所有元素 <= nums[m] < k，已经满足 <= k
  不需要动

结论: 从 m 向右扫，把所有 < k 的加到 k
```

### Step 4: 为什么可以提前终止 (break)？

排序后数组单调不减:
- 情况 A: 从 m 向左，遇到 `nums[i] <= k` 就能停，因为更左的一定 `<= nums[i] <= k`
- 情况 B: 从 m 向右，遇到 `nums[i] >= k` 就能停，因为更右的一定 `>= nums[i] >= k`

---

## 图解演示

### 示例 1: `nums = [2,5,6,8,5], k = 4`

```
排序后: [2, 5, 5, 6, 8]
         0  1  2  3  4
               ^
               m=2, nums[m]=5

k=4 < nums[m]=5 -> 情况 A: 从 m 向左扫

i=2: nums[2]=5 > 4?  是  -> ops += 5-4 = 1    总计 = 1
i=1: nums[1]=5 > 4?  是  -> ops += 5-4 = 1    总计 = 2
i=0: nums[0]=2 > 4?  否  -> break

答案 = 2

验证: [2, 5, 5, 6, 8] -> 把两个5减到4 -> [2, 4, 4, 6, 8]
排序: [2, 4, 4, 6, 8]  中位数 = 4 = k  正确!
```

### 示例 2: `nums = [2,5,6,8,5], k = 7`

```
排序后: [2, 5, 5, 6, 8]
         0  1  2  3  4
               ^
               m=2, nums[m]=5

k=7 > nums[m]=5 -> 情况 B: 从 m 向右扫

i=2: nums[2]=5 < 7?  是  -> ops += 7-5 = 2    总计 = 2
i=3: nums[3]=6 < 7?  是  -> ops += 7-6 = 1    总计 = 3
i=4: nums[4]=8 < 7?  否  -> break

答案 = 3

验证: [2, 5, 5, 6, 8] -> 把5加到7,6加到7 -> [2, 7, 7, 7, 8]
排序: [2, 7, 7, 7, 8]  中位数 = 7 = k  正确!
```

### 示例 3: `nums = [1,2,3,4,5,6], k = 4`

```
排序后: [1, 2, 3, 4, 5, 6]
         0  1  2  3  4  5
                  ^
                  m=3, nums[m]=4

k=4 == nums[m]=4 -> 进入情况 A (k <= nums[m])

i=3: nums[3]=4 > 4?  否  -> break

答案 = 0 (已经满足)
```

---

## 贪心正确性证明

**为什么只处理一侧是最优的？**

以情况 A (k < nums[m]) 为例，反证法:

假设存在一个更优方案，也修改了右侧某元素 `x` (x >= nums[m] > k)。

如果把 x 减小:
- 如果减到 >= k: 它还在 m 右边，对中位数没影响，白花操作
- 如果减到 < k: 它跑到 m 左边，把原来 m 左边某元素挤到 m 位置。该元素 <= nums[m]，可能 > k，需要更多操作

所以修改右侧不会使总操作减少。只需处理左侧 (含中位数自身)。

情况 B 对称推理。

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 排序 + 单侧贪心 | 排序后从中位数向一侧扫描 | O(n log n) | O(1) | 面试首选 |
| nth_element + 贪心 | 快速选择找中位数后扫描 | O(n) 期望 | O(1) | 理论最优 |
| 统一写法 (无分支) | 排序后遍历左右两侧 | O(n log n) | O(1) | 代码简洁 |

---

## 解法详解

### 解法1: 排序 + 单侧贪心 -- 面试首选

```cpp
class Solution {
public:
    long long minOperationsToMakeMedianK(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int m = n / 2;  // 中位数位置 (偶数长度取较大者)
        long long ops = 0;
        
        if (k <= nums[m]) {
            // 情况 A: 需要减小中位数及左侧 > k 的元素
            for (int i = m; i >= 0; i--) {
                if (nums[i] > k) {
                    ops += (long long)(nums[i] - k);
                } else {
                    break;  // 排序后更左的一定 <= nums[i] <= k
                }
            }
        } else {
            // 情况 B: 需要增大中位数及右侧 < k 的元素
            for (int i = m; i < n; i++) {
                if (nums[i] < k) {
                    ops += (long long)(k - nums[i]);
                } else {
                    break;  // 排序后更右的一定 >= nums[i] >= k
                }
            }
        }
        
        return ops;
    }
};
```

### 解法2: 统一写法 (无 if-else 分支)

```cpp
class Solution {
public:
    long long minOperationsToMakeMedianK(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int m = n / 2;
        long long ops = 0;
        
        // 处理中位数本身
        ops += abs((long long)nums[m] - k);
        
        // 处理左侧 [0, m-1]: 如果 > k, 需要减到 k
        for (int i = 0; i < m; i++) {
            if (nums[i] > k)
                ops += (long long)(nums[i] - k);
        }
        
        // 处理右侧 [m+1, n-1]: 如果 < k, 需要加到 k
        for (int i = m + 1; i < n; i++) {
            if (nums[i] < k)
                ops += (long long)(k - nums[i]);
        }
        
        return ops;
    }
};
```

**注意**: 这种写法不能 break (因为没排序保证单调方向的扫描)。但代码更简洁，不需要分情况讨论。

### 解法3: nth_element -- O(n) 期望

```cpp
class Solution {
public:
    long long minOperationsToMakeMedianK(vector<int>& nums, int k) {
        int n = nums.size();
        int m = n / 2;
        
        // nth_element: 把中位数放到正确位置
        // 保证 [0, m) <= nums[m] <= [m+1, n)
        // 但两侧内部不一定有序
        nth_element(nums.begin(), nums.begin() + m, nums.end());
        
        long long ops = 0;
        
        if (k <= nums[m]) {
            // 遍历 [0, m], 所有 > k 的需要减到 k
            // 不能 break (内部无序)
            for (int i = 0; i <= m; i++) {
                if (nums[i] > k)
                    ops += (long long)(nums[i] - k);
            }
        } else {
            // 遍历 [m, n), 所有 < k 的需要加到 k
            for (int i = m; i < n; i++) {
                if (nums[i] < k)
                    ops += (long long)(k - nums[i]);
            }
        }
        
        return ops;
    }
};
```

---

## 易错点

### 1. 数据类型溢出 -- 最常见错误

```
nums[i] 最大 10^9, k 最大 10^9
差值最大 10^9, 累加 2*10^5 个差值 -> 最大 2*10^14
int 范围约 2.1*10^9, 会溢出!

必须用 long long:
ops += (long long)(nums[i] - k);  // 显式转换
```

### 2. 中位数位置

```
偶数长度取较大的: m = n / 2 (不是 (n-1) / 2)

n=5: [0,1,2,3,4] -> m=2 (中间元素)
n=6: [0,1,2,3,4,5] -> m=3 (中间两个取较大者)
```

### 3. k == nums[m] 时的处理

```
k == nums[m] 时进入情况 A (k <= nums[m])
循环条件 nums[i] > k? 由于 nums[m] == k, 不满足, 直接 break
结果 ops = 0, 正确!
```

### 4. 解法1可以 break, 解法2和3不能

```
解法1: 排序后从 m 向一侧扫描, 排序保证单调, 遇到不满足条件就能停
解法3: nth_element 后内部无序, 不能 break, 必须全扫
解法2: 左右分开遍历, 也不能 break (不是从中位数出发的)
```

---

## 复杂度分析

| 解法 | 时间 | 空间 |
|------|------|------|
| 排序 + 贪心 | O(n log n) | O(1) |
| nth_element + 贪心 | O(n) 期望 | O(1) |
| 统一写法 | O(n log n) | O(1) |

瓶颈在排序。扫描部分是 O(n)。

---

## 面试追问

**Q1: 为什么只处理一侧？**
> 排序后中位数在固定位置 m。如果 k < nums[m]，右侧所有元素 >= nums[m] > k，已经满足 >= k 的约束，修改它们不会使中位数更接近 k。只需把中位数及左侧 > k 的元素减到 k。

**Q2: 能否 O(n) 解决？**
> 可以。用 `nth_element` 找到中位数（O(n) 期望），然后遍历相应半边累加差值。整体 O(n)。但面试中排序方案更稳。

**Q3: 如果操作是 +2 或 -2？**
> 操作次数变为 `ceil(|x - k| / 2)`。思路不变，只需修改单次操作的代价计算。

**Q4: 如果数组动态增删元素？**
> 用两个堆 (max-heap + min-heap) 维护中位数，类似 LC 295 Find Median from Data Stream。

---

## 相关题目

| 题号 | 题目 | 关系 |
|------|------|------|
| 462 | Minimum Moves to Equal Array Elements II | 类似，所有元素变成中位数 |
| 2033 | Minimum Operations to Make a Uni-Value Grid | 网格版本 |
| 2448 | Minimum Cost to Make Array Equal | 带权版本 |
| 295 | Find Median from Data Stream | 动态维护中位数 |
