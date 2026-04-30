# 374. Guess Number Higher or Lower - 猜数字大小

## 核心思路
经典二分查找：在 [1, n] 范围内，每次猜中间值，根据 guess() API 返回值缩小范围。

## 思维链
1. 搜索空间 [1, n]，每次取中点 mid
2. 调用 guess(mid) 获取反馈：
   - 返回 0：猜对了，返回 mid
   - 返回 -1：mid 太大，搜索 [lo, mid-1]
   - 返回 1：mid 太小，搜索 [mid+1, hi]
3. 二分查找保证 O(log n) 找到答案
4. 注意 mid 计算要防溢出：`lo + (hi - lo) / 2`

## 解法概览表 ⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 二分查找 | O(log n) | O(1) | 标准解法 |
| 三分查找 | O(log₃ n) | O(1) | 理论常数更优但实际差距微小 |

## 关键提示
1. **防溢出**：`mid = lo + (hi - lo) / 2` 而非 `(lo + hi) / 2`
2. **API 语义**：guess 返回 -1 表示你猜大了（num > pick），1 表示猜小了
3. **n 可达 2^31-1**：lo + hi 会溢出 int
4. **循环条件**：`lo <= hi`，不是 `lo < hi`

## 解法详解

### 解法一：标准二分查找（推荐）⭐
**思路**：维护搜索区间 [lo, hi]，根据 guess 反馈不断缩半。

```cpp
class Solution {
public:
    int guessNumber(int n) {
        int lo = 1, hi = n;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int res = guess(mid);
            if (res == 0) return mid;
            else if (res == -1) hi = mid - 1; // 猜大了
            else lo = mid + 1;                // 猜小了
        }
        return -1; // 不会到这里
    }
};
```

### 解法二：三分查找
**思路**：将区间三等分，每次排除 1/3。

```cpp
class Solution {
public:
    int guessNumber(int n) {
        long lo = 1, hi = n;
        while (lo <= hi) {
            long mid1 = lo + (hi - lo) / 3;
            long mid2 = hi - (hi - lo) / 3;
            int r1 = guess(mid1), r2 = guess(mid2);
            if (r1 == 0) return mid1;
            if (r2 == 0) return mid2;
            if (r1 == -1) hi = mid1 - 1;
            else if (r2 == 1) lo = mid2 + 1;
            else { lo = mid1 + 1; hi = mid2 - 1; }
        }
        return lo;
    }
};
```

## 易错点
- ✗ `int mid = (lo + hi) / 2` 当 lo+hi > INT_MAX 时溢出 → ✓ `lo + (hi - lo) / 2`
- ✗ guess 返回值语义搞反（-1 是猜大了不是猜小了）→ ✓ 仔细读题：-1 表示 num > pick
- ✗ 循环条件写成 `lo < hi` 导致漏掉 lo==hi 的情况 → ✓ 使用 `lo <= hi`
- ✗ 缩小范围时不排除 mid（`hi = mid` 而非 `hi = mid - 1`）→ ✓ 已确定 mid 不是答案时必须排除

## 面试追问

**Q1: 为什么用 `lo + (hi - lo) / 2` 而不是 `(lo + hi) / 2`？**
A: 当 lo 和 hi 都接近 INT_MAX 时，lo + hi 会整型溢出。`lo + (hi - lo) / 2` 不会溢出因为 hi - lo 一定非负且在 int 范围内。

**Q2: 如果 guess API 有调用次数限制怎么办？**
A: 二分查找最多调用 ceil(log2(n)) 次（约 31 次对于 n = 2^31-1）。如果限制更严格可以考虑插值搜索或指数搜索，但对均匀分布二分已是最优。

**Q3: 这道题和二分查找模板的关系是什么？**
A: 这是最标准的二分查找模板应用。区间 [lo, hi]，条件判断三分支（找到/左移/右移），适用于单调有序搜索空间。是学习二分查找的入门题。

## 相关题型
- [35. Search Insert Position](../0035_search_insert_position/) - 二分查找变体
- [278. First Bad Version](../0278_first_bad_version/) - 类似 API 调用的二分
- [375. Guess Number Higher or Lower II](../0375_guess_number_higher_or_lower_ii/) - 进阶博弈版本
- [69. Sqrt(x)](../0069_sqrtx/) - 二分搜索应用
