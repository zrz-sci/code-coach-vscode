# 4. 寻找两个正序数组的中位数

## 核心思路

本题本质上是：在两个已排序数组中，找到一条"分割线"，将所有元素分成个数相等的左右两半，使得左半部分的最大值 ≤ 右半部分的最小值。中位数就藏在这条分割线的两侧。

## 思维链

1. **读完题第一反应**：把两个数组合并成一个有序数组，直接取中位数。简单粗暴，O(m+n)。
2. **暴力解的瓶颈**：合并需要 O(m+n) 时间和空间。题目要求 O(log(m+n))，看到 log 就要想二分。
3. **怎么用二分？**—— 思路A：中位数就是第 k 小的元素（k=(m+n)/2），每次排除 k/2 个元素，O(log(m+n))。
4. **怎么用二分？**—— 思路B（更精妙）：中位数把合并后的数组分成等长的左右两半。我们不需要真的合并，只需要在较短的数组上二分，确定"从 nums1 取几个元素放左半边"，nums2 需要取多少个就自动确定了。
5. **验证分割的正确性**：左半边的最大值 ≤ 右半边的最小值 ⟺ `nums1[i-1] <= nums2[j]` 且 `nums2[j-1] <= nums1[i]`。不满足时根据大小关系移动二分边界。
6. **边界处理**：某个数组取 0 个或全部放左边的情况，用 ±∞ 处理。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 合并排序 | 合并两个有序数组，取中间 | O(m+n) | O(m+n) | 能说出即可 |
| 双指针 | 不真正合并，走 (m+n)/2 步 | O(m+n) | O(1) | 能说出即可 |
| 找第K小 | 每次排除 k/2 个元素 | O(log(m+n)) | O(log(m+n)) | ⭐ 必须写出 |
| 划分数组 | 在短数组上二分找分割线 | O(log(min(m,n))) | O(1) | 加分项 |

## 关键提示

1. **提示1**：中位数的定义 → 它就是第 `(m+n+1)/2` 小的元素（奇数），或者第 `(m+n)/2` 和第 `(m+n)/2+1` 小的平均（偶数）。能否快速找"第 k 小"？

2. **提示2**：两个有序数组各取前 k/2 个元素比较。如果 `nums1[k/2-1] < nums2[k/2-1]`，那么 `nums1[0..k/2-1]` 这些元素不可能是第 k 小的（因为最多只有 k-2 个元素比它们小），可以安全排除。

3. **提示3（划分法核心）**：假设合并后的数组被切成左右两半，左半有 `half = (m+n+1)/2` 个元素。如果从 nums1 取 i 个，那么从 nums2 必须取 `j = half - i` 个。只需要在较短的数组上二分 i。

4. **提示4**：划分法中，分割线合法的条件是什么？画图理解：

```
nums1:  [...  A[i-1] | A[i]  ...]
nums2:  [...  B[j-1] | B[j]  ...]
                左半边 | 右半边

合法条件: A[i-1] <= B[j] 且 B[j-1] <= A[i]
```

5. **提示5**：为什么在**较短**的数组上二分？因为 `j = half - i`，如果 i 的范围太大（超过 half），j 会变成负数。在较短数组上二分可以保证 j 始终合法。

## 解法详解

### 解法1: 合并排序 — O(m+n) / O(m+n)

**思考过程**: 最直觉的做法——两个有序数组合并就是归并排序的 merge 步骤，合并后直接按下标取中位数。

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> merged;
        int i = 0, j = 0;
        // 归并排序的 merge 过程
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] <= nums2[j]) merged.push_back(nums1[i++]);
            else merged.push_back(nums2[j++]);
        }
        while (i < nums1.size()) merged.push_back(nums1[i++]);
        while (j < nums2.size()) merged.push_back(nums2[j++]);
        
        int n = merged.size();
        if (n % 2 == 1) return merged[n / 2];
        return (merged[n / 2 - 1] + merged[n / 2]) / 2.0;
    }
};
```

**关键点**: 这个解法不满足题目要求的 O(log(m+n))，但它是正确性基准，面试中先说这个展示你理解了题意。

---

### 解法2: 双指针不合并 — O(m+n) / O(1)

**从解法1优化**: 解法1的空间瓶颈是创建了合并数组。其实我们不需要整个合并数组，只需要走到中间位置的元素即可。

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        int total = m + n;
        int prev = 0, curr = 0; // 只保存最近两个值
        int p1 = 0, p2 = 0;
        
        // 走 total/2 + 1 步，记录最后两个值
        for (int count = 0; count <= total / 2; count++) {
            prev = curr;
            if (p1 < m && (p2 >= n || nums1[p1] <= nums2[p2])) {
                curr = nums1[p1++];
            } else {
                curr = nums2[p2++];
            }
        }
        
        if (total % 2 == 1) return curr;
        return (prev + curr) / 2.0;
    }
};
```

**关键点**: `prev` 记录倒数第二个，`curr` 记录最后一个。奇数长度返回 `curr`，偶数返回两者平均。

---

### 解法3: 找第K小 — O(log(m+n)) / O(log(m+n)) ⭐ 面试首选

**从解法2优化**: 解法2逐步走一个元素太慢，能不能一次跳过多个？

核心观察：要找第 k 小的元素，分别看两个数组第 k/2 个元素。较小的那一侧的前 k/2 个元素一定不是第 k 小的，可以全部排除。

```
// 找第 k=7 小的示例:
// nums1: [1, 3, 5, 7, 9]    比较 nums1[3]=7
//         ~~~~~~~              vs
// nums2: [2, 4, 6, 8, 10]   比较 nums2[3]=8
//         ~~~~~~~
// 
// nums1[3] < nums2[3]，说明 nums1[0..3] 最多排第6
// 排除 nums1 前 k/2=3 个，k 缩小为 7-3=4
// 
// 下一轮:
// nums1: [7, 9]              比较 nums1[1]=9
//         ~~                    vs
// nums2: [2, 4, 6, 8, 10]   比较 nums2[1]=4
//         ~~
// nums2[1] < nums1[1]，排除 nums2 前 2 个，k=4-2=2
//
// 继续...直到 k=1，取两数组当前首元素的较小值
```

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        int total = m + n;
        // 统一处理奇偶：奇数找第 (total+1)/2，偶数找第 total/2 和 total/2+1 的均值
        if (total % 2 == 1) {
            return findKth(nums1, 0, nums2, 0, total / 2 + 1);
        } else {
            return (findKth(nums1, 0, nums2, 0, total / 2) +
                    findKth(nums1, 0, nums2, 0, total / 2 + 1)) / 2.0;
        }
    }
    
private:
    // 在 nums1[s1:] 和 nums2[s2:] 中找第 k 小的元素
    int findKth(vector<int>& nums1, int s1, vector<int>& nums2, int s2, int k) {
        // 边界1：nums1 已经用完，直接在 nums2 中取
        if (s1 >= (int)nums1.size()) return nums2[s2 + k - 1];
        // 边界2：nums2 已经用完，直接在 nums1 中取
        if (s2 >= (int)nums2.size()) return nums1[s1 + k - 1];
        // 边界3：k=1，取两个数组当前首元素的较小值
        if (k == 1) return min(nums1[s1], nums2[s2]);
        
        // 各看前 k/2 个元素的末尾
        // 如果某个数组剩余不足 k/2 个，设为 INT_MAX（保证另一边被排除）
        int half = k / 2;
        int val1 = (s1 + half - 1 < (int)nums1.size()) 
                   ? nums1[s1 + half - 1] : INT_MAX;
        int val2 = (s2 + half - 1 < (int)nums2.size()) 
                   ? nums2[s2 + half - 1] : INT_MAX;
        
        if (val1 <= val2) {
            // nums1 的前 half 个不可能是第 k 小，排除
            return findKth(nums1, s1 + half, nums2, s2, k - half);
        } else {
            // nums2 的前 half 个不可能是第 k 小，排除
            return findKth(nums1, s1, nums2, s2 + half, k - half);
        }
    }
};
```

**关键点**: 
- 每次排除 k/2 个元素，k 至少减半 → O(log k) = O(log(m+n))
- 数组剩余不足 k/2 个时设为 INT_MAX，巧妙保证排除另一侧
- 递归深度 O(log(m+n))，空间也是 O(log(m+n))

---

### 解法4: 划分数组（二分分割线）— O(log(min(m,n))) / O(1) 加分项

**思路转换**: 不再找"第 k 小"，而是直接找一条分割线，把两个数组的元素分成个数相等的左右两半。

```
// 核心思想图解:
//
// nums1:  [1, 3, | 5, 7]        i=2 (nums1 取前2个到左半)
// nums2:  [2, | 4, 6, 8, 10]    j=1 (nums2 取前1个到左半)
//         -------- | ----------
//         左半边     右半边
//         {1,2,3}   {4,5,6,7,8,10}
//
// 左半边共 half = (4+5+1)/2 = 5 个? 不对，这里 i+j=3
// 实际 half=(9+1)/2=5, 所以需要 j=5-2=3:
//
// nums1:  [1, 3, | 5, 7]        i=2
// nums2:  [2, 4, 6, | 8, 10]    j=3
//         ----------- | ---------
//         左: {1,2,3,4,6}  右: {5,7,8,10}
//
// 检查: nums1[i-1]=3 <= nums2[j]=8 ✓
//       nums2[j-1]=6 <= nums1[i]=5 ✗ → i 太小，需要右移
//
// i=3:
// nums1:  [1, 3, 5, | 7]        i=3
// nums2:  [2, 4, | 6, 8, 10]    j=2
//         --------- | ----------
//         左: {1,2,3,4,5}  右: {6,7,8,10}
//
// 检查: nums1[i-1]=5 <= nums2[j]=6 ✓
//       nums2[j-1]=4 <= nums1[i]=7 ✓ → 找到合法分割!
//
// 中位数 = max(左半最大) = max(5,4) = 5
```

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // 保证在较短数组上二分，避免 j 越界
        if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
        
        int m = nums1.size(), n = nums2.size();
        int half = (m + n + 1) / 2; // 左半边的元素个数（+1 使奇数时左半多一个）
        
        // 在 nums1 上二分：i 表示 nums1 取前 i 个放左半边
        // i 的范围 [0, m]：0 表示 nums1 一个都不取，m 表示全取
        int lo = 0, hi = m;
        
        while (lo <= hi) {
            int i = lo + (hi - lo) / 2; // nums1 的分割点
            int j = half - i;           // nums2 的分割点（自动确定）
            
            // 分割线两侧的四个关键值
            // 用 INT_MIN/INT_MAX 处理 i=0, i=m, j=0, j=n 的边界
            int nums1Left  = (i == 0) ? INT_MIN : nums1[i - 1];
            int nums1Right = (i == m) ? INT_MAX : nums1[i];
            int nums2Left  = (j == 0) ? INT_MIN : nums2[j - 1];
            int nums2Right = (j == n) ? INT_MAX : nums2[j];
            
            if (nums1Left <= nums2Right && nums2Left <= nums1Right) {
                // 找到合法分割！
                if ((m + n) % 2 == 1) {
                    return max(nums1Left, nums2Left); // 奇数：左半最大值
                } else {
                    return (max(nums1Left, nums2Left) + 
                            min(nums1Right, nums2Right)) / 2.0; // 偶数：均值
                }
            } else if (nums1Left > nums2Right) {
                // nums1 左侧太大，i 需要左移
                hi = i - 1;
            } else {
                // nums2 左侧太大，即 nums1 左侧太小，i 需要右移
                lo = i + 1;
            }
        }
        
        return -1; // 不会到这里（题目保证有解）
    }
};
```

**关键点**:
- **为什么在较短数组上二分**：`j = half - i`，如果 m > half，则 i 取大值时 j 变负数。在较短数组上二分保证 `0 <= j <= n`。
- **half = (m+n+1)/2 中的 +1**：使得总数为奇数时，左半边多一个元素，中位数直接是 `max(左半边)`。
- **INT_MIN/INT_MAX 哨兵值**：i=0 意味着 nums1 不贡献左半边，此时 nums1Left 应该"不存在"（用 INT_MIN 代替，它不影响 max 判断）。

---

## 解法对比

| | 合并排序 | 双指针 | 找第K小 | 划分数组 |
|---|---|---|---|---|
| 时间 | O(m+n) | O(m+n) | O(log(m+n)) | O(log(min(m,n))) |
| 空间 | O(m+n) | O(1) | O(log(m+n)) | O(1) |
| 思维难度 | ★☆☆☆☆ | ★★☆☆☆ | ★★★☆☆ | ★★★★☆ |
| 面试推荐 | 开场说 | 优化说 | ⭐首选写 | 加分写 |

**怎么选**：
- 面试时先说解法1展示理解，然后直接写解法3（找第K小），因为它思路直观、代码好写。
- 解法4（划分数组）是最优解但边界处理复杂，面试时间紧的话解法3足够。
- 解法3 和 解法4 的本质区别：解法3 是"每次排除一半"，解法4 是"直接二分找分割位置"。

## 易错点

1. **✗ `findKth` 中 k/2 溢出数组**：
   ```cpp
   // 错误：不检查数组剩余长度直接访问
   int val1 = nums1[s1 + half - 1]; // 可能越界！
   ```
   **✓ 正确做法**：剩余不足 half 时设为 `INT_MAX`：
   ```cpp
   int val1 = (s1 + half - 1 < nums1.size()) ? nums1[s1 + half - 1] : INT_MAX;
   ```

2. **✗ 划分法忘记交换使 nums1 为较短数组**：
   ```cpp
   // 如果 nums1 更长，j = half - i 可能为负
   int j = half - i; // j < 0 → 越界！
   ```
   **✓ 正确做法**：开头加 `if (m > n) swap`。

3. **✗ 奇偶处理混淆**：
   ```cpp
   // 错误：奇数长度也取平均
   return (findKth(..., total/2) + findKth(..., total/2+1)) / 2.0;
   // 当 total=5 时，total/2=2，找的是第2和第3小，而不是第3小
   ```
   **✓ 正确做法**：奇数只找 `total/2 + 1`（第 k 小是 1-indexed）。

4. **✗ 划分法的 half 写成 `(m+n)/2` 而不是 `(m+n+1)/2`**：
   - 区别在奇数总长度时左半边是否多一个元素
   - 用 `(m+n+1)/2` 统一处理奇偶更简洁

## 面试追问

**Q1**: "暴力 O(m+n) 的做法能说一下吗？" 
→ 归并排序的 merge 过程，合并后取 `arr[(m+n)/2]`。不满足题目 O(log) 要求，但展示理解。

**Q2**: "怎么优化到 O(log(m+n))？"
→ 找第 K 小：中位数就是第 (m+n+1)/2 小的元素。利用两个数组有序的性质，每次比较两个数组的第 k/2 个元素，排除较小一侧的 k/2 个元素，k 每轮减半。

**Q3**: "能否做到 O(log(min(m,n)))？空间 O(1)？"
→ 划分数组法：在较短数组上二分找分割线。i 确定后 j 自动确定，只需验证分割合法性。时间只取决于较短数组长度，空间 O(1)。

**Q4**: "如果两个数组的长度差距很大（如 m=10, n=10^6），哪个解法更优？"
→ 划分数组法（解法4）显著更优，因为它只在长度为 10 的数组上二分，O(log 10) ≈ 3 次比较。而找第K小法是 O(log(10+10^6)) ≈ 20 次。

## 相关题型

- **LeetCode 215. 数组中的第K个最大元素** — 本题的"找第K小"子问题，但那道题只有一个数组，可以用快速选择 O(n)。
- **LeetCode 378. 有序矩阵中第K小的元素** — 复用"二分答案"思想：二分一个值 mid，数两个数组中 ≤ mid 的元素个数。
- **LeetCode 23. 合并K个升序链表** — 多路归并的泛化版本，本题解法1（归并两路）是它的特例。
- **LeetCode 295. 数据流的中位数** — 动态维护中位数，用两个堆（大顶堆+小顶堆），本质和解法4的"分成左右两半"思想一致。