# 275. H-Index II (H 指数 II)

**难度**: Medium | **标签**: Array, Binary Search

## 核心思路

数组已排序，要求 O(log n)，显然用**二分查找**。关键在于理解 h-index 的含义：找最大的 h，使得至少有 h 篇论文的引用次数 >= h。排序数组中，从位置 mid 开始到末尾共有 `n - mid` 篇论文，若 `citations[mid] >= n - mid`，则 h-index 至少为 `n - mid`。

## 思维链

1. **H-Index 定义**: 最大的 h，使得至少 h 篇论文引用 >= h
2. **排序数组性质**: 位置 mid 之后（含）有 `n - mid` 篇论文
3. **二分条件**: `citations[mid] >= n - mid` 说明从 mid 开始的论文都满足
4. **搜索目标**: 找最左的 mid 使得 `citations[mid] >= n - mid`，答案就是 `n - mid`

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 二分查找 | O(log n) | O(1) | ⭐ |
| 线性扫描(不满足题意) | O(n) | O(1) | |

## 关键提示

```
示例: citations = [0, 1, 3, 5, 6],  n = 5

索引:    0  1  2  3  4
引用:    0  1  3  5  6
论文数:  5  4  3  2  1   (n - mid)

检查每个位置:
  mid=0: citations[0]=0 >= 5? No
  mid=1: citations[1]=1 >= 4? No  
  mid=2: citations[2]=3 >= 3? Yes! --> h = n-mid = 3 ✓
  mid=3: citations[3]=5 >= 2? Yes  --> h = 2 (更小)
  mid=4: citations[4]=6 >= 1? Yes  --> h = 1 (更小)

二分找第一个 citations[mid] >= n-mid 的位置:
  答案 = n - mid = 5 - 2 = 3

二分过程:
  lo=0, hi=4
  mid=2: citations[2]=3 >= 3 → hi=2
  lo=0, hi=2
  mid=1: citations[1]=1 >= 4? No → lo=2
  lo=2, hi=2 → 答案 = n-lo = 3
```

## 解法详解

### 解法一: 二分查找 (推荐)

```cpp
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        int lo = 0, hi = n - 1;
        
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (citations[mid] >= n - mid) {
                hi = mid - 1;  // 尝试找更左的位置 (更大的 h)
            } else {
                lo = mid + 1;  // 当前位置不够，向右找
            }
        }
        
        return n - lo;  // lo 就是第一个满足条件的位置
    }
};
```

### 解法二: 线性扫描 (对比用)

```cpp
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        for (int i = 0; i < n; i++) {
            if (citations[i] >= n - i) {
                return n - i;
            }
        }
        return 0;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 返回 `lo` | ✓ 返回 `n - lo` | lo 是索引位置，答案是从 lo 到末尾的论文数 |
| ✗ `citations[mid] > n - mid` | ✓ `citations[mid] >= n - mid` | 等于也满足条件 |
| ✗ 所有都不满足时返回 -1 | ✓ 返回 0 (n - n = 0) | lo 最终等于 n，n - n = 0 |
| ✗ 二分找最后满足的位置 | ✓ 找第一个满足的位置(最左) | 越左 n-mid 越大，h-index 越大 |

## 面试追问

**Q1: 与 274. H-Index 的区别?**
--> 274 数组未排序，需要先排序 O(n log n) 或用计数排序 O(n)。275 已排序，直接二分 O(log n)。

**Q2: 为什么找最左满足位置而不是最右?**
--> 最左位置对应最大的 `n - mid`，即最大的 h-index。越靠左，后面论文越多。

**Q3: 如果所有引用都是 0 怎么办?**
--> lo 一路到 n，返回 n - n = 0，正确。

## 相关题型

- [274. H-Index](../0274_h_index/) - 未排序版本
- [34. Find First and Last Position](../0034_find_first_and_last_position_of_element_in_sorted_array/) - 二分查找模板
- [35. Search Insert Position](../0035_search_insert_position/) - 二分查找插入位置
- [278. First Bad Version](../0278_first_bad_version/) - 二分查找第一个满足条件
