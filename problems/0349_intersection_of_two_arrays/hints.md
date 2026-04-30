# 349. 两个数组的交集

## 核心思路

本质上就是问：**哪些元素同时出现在两个数组中？** 结果要去重。这是一个"查找"问题——对于数组1中的每个元素，判断它是否存在于数组2中。

## 思维链

1. **读完题第一反应**：对 nums1 中的每个元素，遍历 nums2 看是否存在 → 暴力双循环 O(m×n)
2. **暴力解的瓶颈**：对于 nums1 的每个元素，在 nums2 中查找需要 O(n)，总共 O(m×n)。**瓶颈在"查找"这个操作**。
3. **怎么加速查找？** 两条路：
   - **哈希表**：把 nums2 放进 set，查找从 O(n) → O(1)，总体 O(m+n)
   - **排序+二分**：排序 nums2 后二分查找，查找从 O(n) → O(log n)，总体 O(m·log n + n·log n)
4. **还有第三条路**：两个数组都排序，然后用**双指针**同步扫描，像归并排序一样，O(m·log m + n·log n)
5. **去重怎么处理？** 用 set 天然去重；或者排序后跳过重复元素

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力双循环 | 逐个检查是否在另一个数组中 | O(m×n) | O(min(m,n)) | 能说出即可 |
| 哈希表(Set) | 一个数组建 set，遍历另一个查找 | O(m+n) | O(m) | ⭐ 必须写出 |
| 排序+双指针 | 两数组排序后同步扫描 | O(m·log m + n·log n) | O(1)* | 加分项 |
| 排序+二分查找 | 一个数组排序，另一个逐个二分查找 | O((m+n)·log n) | O(1)* | 加分项 |

> *不计输出数组的空间

## 关键提示

1. **去重是关键**：题目要求结果中每个元素唯一，无论哪种解法都要处理去重——用 set 是最自然的方式
2. **谁建 set 谁排序？** 把**较小的数组**放进 set / 排序后被查找，可以优化空间和常数
3. **双指针的移动逻辑**：两数组都排好序后，小的那个指针前进（因为当前值在另一个数组中不可能再匹配到）
4. **数据范围提示**：值域 [0, 1000]，其实可以用大小 1001 的布尔数组代替 hash set，但面试中 set 写法更通用
5. 想想这道题和 [350. 两个数组的交集 II](https://leetcode.com/problems/intersection-of-two-arrays-ii/) 的区别——350 不需要去重，需要保留出现次数

## 解法详解

### 解法1: 暴力双循环 — O(m×n) / O(min(m,n))

**思考过程**：最直觉的方法——对 nums1 中的每个元素，检查它是否出现在 nums2 中。如果出现了且还没加入结果，就加入。

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> resultSet; // 用 set 自动去重
        for (int x : nums1) {
            for (int y : nums2) {
                if (x == y) {
                    resultSet.insert(x);
                    break; // 找到一个匹配就够了，不用继续
                }
            }
        }
        return vector<int>(resultSet.begin(), resultSet.end());
    }
};
```

**关键点**：找到匹配后要 `break`，否则会做无用功。用 set 存结果来自动去重，比手动判断是否已存在更简洁。

---

### 解法2: 哈希表(Set) — O(m+n) / O(m) ⭐ 面试首选

**从解法1优化**：暴力解的瓶颈是"对 nums1 每个元素，在 nums2 中线性查找"——查找操作 O(n)。**用哈希集合把查找降到 O(1)**。

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 第一步：把 nums1 的元素全部放入 set（自动去重）
        unordered_set<int> set1(nums1.begin(), nums1.end());
        
        unordered_set<int> resultSet;
        // 第二步：遍历 nums2，检查是否在 set1 中存在
        for (int x : nums2) {
            if (set1.count(x)) {       // O(1) 查找！
                resultSet.insert(x);    // 用 set 存结果，自动去重
            }
        }
        
        return vector<int>(resultSet.begin(), resultSet.end());
    }
};
```

**为什么这样更快？**
- 建 set：遍历 nums1 一次，O(m)
- 查找：遍历 nums2 一次，每次 O(1) 查找，总共 O(n)
- 总体：O(m + n)，比暴力的 O(m × n) 好得多

**优化细节**：可以把**较小的数组**放入 set，减少空间开销：
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 确保 nums1 是较小的那个，减少 set 大小
        if (nums1.size() > nums2.size()) return intersection(nums2, nums1);
        
        unordered_set<int> set1(nums1.begin(), nums1.end());
        unordered_set<int> resultSet;
        for (int x : nums2) {
            if (set1.count(x)) {
                resultSet.insert(x);
            }
        }
        return vector<int>(resultSet.begin(), resultSet.end());
    }
};
```

---

### 解法3: 排序 + 双指针 — O(m·log m + n·log n) / O(1)

**不同思路**：如果不想用额外空间（哈希表），可以先排序，然后用双指针同步扫描——就像归并两个有序数组一样。

```
// 示例：nums1 = [4,9,5], nums2 = [9,4,9,8,4]
// 
// 排序后：
//   nums1 = [4, 5, 9]
//   nums2 = [4, 4, 8, 9, 9]
//             i→
//             j→
//
// Step1: nums1[0]=4 == nums2[0]=4 → 加入结果 [4], i++, j++
// Step2: nums1[1]=5 > nums2[1]=4  → j++（小的前进）
// Step3: nums1[1]=5 < nums2[2]=8  → i++（小的前进）
// Step4: nums1[2]=9 > nums2[2]=8  → j++
// Step5: nums1[2]=9 == nums2[3]=9 → 加入结果 [4,9], i++, j++
// 结束！结果 = [4, 9]
```

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        
        vector<int> result;
        int i = 0, j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] < nums2[j]) {
                i++;                    // nums1 当前值小，前进追赶
            } else if (nums1[i] > nums2[j]) {
                j++;                    // nums2 当前值小，前进追赶
            } else {
                // 相等 → 交集元素！但要去重
                if (result.empty() || result.back() != nums1[i]) {
                    result.push_back(nums1[i]);
                }
                i++;
                j++;
            }
        }
        return result;
    }
};
```

**关键点**：
- **为什么双指针不会漏解？** 因为两个数组都有序，如果 `nums1[i] < nums2[j]`，那么 `nums1[i]` 不可能和 `nums2[j]` 之后的元素匹配（后面更大），所以 `i` 可以安全前进。
- **去重逻辑**：`result.back() != nums1[i]` 确保不重复添加。因为数组已排序，重复元素相邻，只需和上一个加入的比较。

---

### 解法4: 排序 + 二分查找 — O((m+n)·log n) / O(1)

**另一个角度**：只排序 nums2，然后对 nums1 中每个元素在 nums2 中二分查找。

```
// 在排序后的 nums2 = [4, 4, 8, 9, 9] 中查找 5:
// Round1: [4, 4, 8, 9, 9]  mid=8 > 5  → right=mid-1
//                  ↑
// Round2: [4, 4]  mid=4 < 5  → left=mid+1
//          ↑
// Round3: left > right → 不存在!
```

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 去重 nums1，避免重复查找
        unordered_set<int> set1(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        
        vector<int> result;
        for (int x : set1) {
            // 在有序的 nums2 中二分查找 x
            if (binary_search(nums2.begin(), nums2.end(), x)) {
                result.push_back(x);
            }
        }
        return result;
    }
};
```

**什么时候选这个？** 当 nums2 很大但 nums1 很小时，排序 nums2 的 O(n·log n) 可以分摊，每次查找只要 O(log n)，可能比建哈希表更缓存友好。

## 解法对比

| | 暴力 | 哈希表(Set) | 排序+双指针 | 排序+二分 |
|---|---|---|---|---|
| 时间 | O(m×n) | O(m+n) | O(m·log m + n·log n) | O((m+n)·log n) |
| 空间 | O(min(m,n)) | O(m) | O(1)* | O(m) |
| 去重方式 | set存结果 | set存结果 | 排序后跳过重复 | set去重nums1 |
| 适用场景 | 数据极小 | **通用首选** | 要求O(1)空间 | nums2很大 |

**面试建议**：先说哈希表解法，再被追问"能否不用额外空间"时给出排序+双指针。

## 易错点

1. **忘记去重**：
   - ✗ 用 `vector` 存结果但不检查是否已存在 → 结果中可能有重复元素
   - ✓ 用 `unordered_set` 存结果自动去重，或排序后用 `result.back() != x` 判断

2. **双指针去重写法错误**：
   - ✗ `if (result.back() != nums1[i])` 但 `result` 可能为空 → 访问 `back()` 未定义行为
   - ✓ `if (result.empty() || result.back() != nums1[i])` 先判断非空

3. **混淆 349 和 350**：
   - 349 要求结果唯一（用 set）
   - 350 允许重复出现（用 map 计数）
   - ✗ 在 349 中用 `unordered_map` 计数并保留多次 → 不符合题意

## 面试追问

**Q1: 暴力解能过吗？瓶颈在哪？**
> 数据量 ≤ 1000，O(m×n) = O(10^6) 能过。但瓶颈在对每个元素的线性查找——这正是哈希表可以优化的地方。

**Q2: 如果两个数组都已排序，最优做法是什么？**
> 双指针 O(m+n)，不需要额外空间。比哈希表的 O(m) 空间更好。这也是面试官常见的追问——"如果输入有序能否更优？"

**Q3: 如果 nums1 很小但 nums2 极大（百亿级），存不进内存怎么办？**
> 把 nums1 放入内存中的 set，然后分块读取 nums2 做查找。或者对 nums2 做外排序后用二分查找。核心思想：让内存中只放较小的数据集。

**Q4: 如果不要求去重（即 350. 两个数组的交集 II），代码怎么改？**
> set 换成 map 记录每个元素出现次数，匹配时次数减一。双指针解法中去掉跳过重复的逻辑即可。

## 相关题型

- **[350. 两个数组的交集 II](https://leetcode.com/problems/intersection-of-two-arrays-ii/)** — 本题的进阶版，不去重，需要保留出现次数。代码改动：`unordered_set` → `unordered_map<int,int>` 计数，匹配时 `count--`
- **[1. 两数之和](https://leetcode.com/problems/two-sum/)** — 复用同样的"哈希表加速查找"思路，区别：这里查找的是"是否存在"，Two Sum 查找的是"target - num 是否存在"
- **[2540. 最小公共值](https://leetcode.com/problems/minimum-common-value/)** — 两个已排序数组找最小公共元素，直接复用双指针解法，找到第一个就返回