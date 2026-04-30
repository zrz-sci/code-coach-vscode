# 378. Kth Smallest Element in a Sorted Matrix - 提示与详解

## 难度：Medium | 标签：Array, Binary Search, Matrix, Sorting, Heap (Priority Queue)

---

## 一、核心思路

本题有两种经典思路：

1. **最小堆（Min-Heap）**：将矩阵看作 n 条有序链表的合并，用堆做 K 路归并
2. **二分搜索值域**（面试首选）：在值域 [min, max] 上二分，利用矩阵有序性 O(n) 统计 <= mid 的元素个数

**核心洞察**：矩阵每行、每列都有序，从左下角出发可以 O(n) 统计 <= target 的元素个数。

---

## 二、思维链：从题目到解法

### Step 1：分析矩阵有序性

```
矩阵示例: [[1,5,9],[10,11,13],[12,13,15]]

     col 0  col 1  col 2
row 0:  1     5      9      → 行内递增
row 1: 10    11     13      → 行内递增
row 2: 12    13     15      → 行内递增
        ↓     ↓      ↓
       列内递增

最小值 = matrix[0][0] = 1 (左上角)
最大值 = matrix[n-1][n-1] = 15 (右下角)
```

### Step 2：为什么不能简单排序？

直接把矩阵所有元素排序取第 k 个：O(n^2 * log(n^2)) 时间，O(n^2) 空间。
题目要求空间复杂度优于 O(n^2)，所以需要利用矩阵的有序性。

### Step 3：二分思路 —— 搜索空间收缩图

```
值域: [1 ........................... 15]
       lo                            hi

第1次二分: mid = 8
统计 <= 8 的元素个数 = 2 (只有1和5)
如果 count < k=8: lo = 9

      [9 ........................... 15]
       lo                            hi

第2次二分: mid = 12
统计 <= 12 的元素个数 = 5 (1,5,9,10,11,12 => 但12出现一次 => count=6)
等等，让我精确计算...

matrix = [[1,5,9],[10,11,13],[12,13,15]], k=8

mid=8:  <= 8 的元素: {1,5} => count=2 < 8 => lo=9
        搜索空间: [9, 15]

mid=12: <= 12 的元素: {1,5,9,10,11,12} => count=6 < 8 => lo=13
        搜索空间: [13, 15]

mid=14: <= 14 的元素: {1,5,9,10,11,12,13,13} => count=8 >= 8 => hi=14
        搜索空间: [13, 14]

mid=13: <= 13 的元素: {1,5,9,10,11,12,13,13} => count=8 >= 8 => hi=13
        搜索空间: [13, 13]

lo == hi == 13 => 答案是 13
```

### Step 4：如何 O(n) 统计 <= mid 的元素个数？

**从左下角出发的阶梯路径**：

```
统计 <= 13 的元素个数：

     col 0  col 1  col 2
      1      5      9       全部 <= 13
     10     11     13       全部 <= 13
     12     13     15       12,13 <= 13; 15 > 13

从左下角 (2,0) 开始：
  matrix[2][0] = 12 <= 13 → 这列有 3 个 <= 13, count+=3, c++
  matrix[2][1] = 13 <= 13 → 这列有 3 个 <= 13, count+=3, c++
  matrix[2][2] = 15 > 13  → r--, 即 r=1
  matrix[1][2] = 13 <= 13 → 这列有 2 个 <= 13, count+=2, c++
  c=3, 超出范围, 结束

count = 3 + 3 + 2 = 8 ✓

路径示意（从左下角出发）：
     col 0  col 1  col 2
      1      5      9
     10     11    [13] ← 拐弯
     12     13    [15] ← 向上走
      ↑      ↑
     起点   向右走
```

### Step 5：为什么 lo 一定是矩阵中存在的值？

关键证明：当 lo == hi 时，lo 一定出现在矩阵中。
- 如果 lo 不在矩阵中，那么 count(lo) == count(lo-1)
- 但二分保证了 count(lo-1) < k <= count(lo)
- 如果 lo 不在矩阵中，count 不变，矛盾
- 所以 lo 一定是矩阵中的一个元素

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 二分搜索值域 | 二分+左下角统计 | O(n * log(max-min)) | O(1) | 面试首选 |
| 最小堆 K路归并 | 堆维护n个行指针 | O(k * log n) | O(n) | k较小时高效 |
| 直接排序 | 展平+排序 | O(n^2 * log n) | O(n^2) | 暴力备选 |

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：二分的是什么？</summary>

不是二分下标，而是二分"值"。搜索空间是 [matrix[0][0], matrix[n-1][n-1]]，即矩阵中的最小值到最大值。

</details>

<details>
<summary>提示 2：对于一个给定的 mid，如何判断它是否 >= 第 k 小的元素？</summary>

统计矩阵中 <= mid 的元素个数 count。如果 count >= k，说明 mid >= 第k小的元素，应该缩小右边界。

</details>

<details>
<summary>提示 3：如何高效统计 <= mid 的元素个数？</summary>

利用矩阵的行列有序性。从左下角 (n-1, 0) 出发：
- 如果当前元素 <= mid：这一列从顶到当前行都 <= mid，count += row+1，向右移动
- 如果当前元素 > mid：向上移动
时间复杂度 O(n)，因为最多走 2n 步（n 步向右 + n 步向上）。

</details>

<details>
<summary>提示 4：二分结束时的结果一定在矩阵中吗？</summary>

是的。当 lo == hi 时，lo 一定是矩阵中存在的值。直觉上：如果 lo 不在矩阵中，count(lo) == count(lo-1)，但二分保证了 count(lo-1) < k <= count(lo)，所以 count 必须在 lo 处增加，意味着 lo 一定在矩阵中。

</details>

<details>
<summary>提示 5：堆解法的思路是什么？</summary>

把矩阵看作 n 条有序链表（每行一条），做 K 路归并。用最小堆维护每行当前最小的未处理元素。每次从堆中弹出最小值，然后将同行的下一个元素入堆。弹出 k 次后就是答案。

</details>

---

## 五、解法详解

### 解法一：二分搜索值域 + 左下角统计（面试首选）

```cpp
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();
        int lo = matrix[0][0], hi = matrix[n-1][n-1];

        // 二分搜索值域
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int count = countLessOrEqual(matrix, mid, n);

            if (count < k) {
                lo = mid + 1;  // 第k小的值 > mid
            } else {
                hi = mid;      // 第k小的值 <= mid
            }
        }
        return lo;  // lo == hi 时就是答案
    }

private:
    // 从左下角出发，O(n) 统计矩阵中 <= mid 的元素个数
    int countLessOrEqual(vector<vector<int>>& matrix, int mid, int n) {
        int count = 0;
        int r = n - 1, c = 0;  // 从左下角出发

        while (r >= 0 && c < n) {
            if (matrix[r][c] <= mid) {
                count += r + 1;  // 这一列从 row 0 到 row r 都 <= mid
                c++;             // 向右移动
            } else {
                r--;             // 向上移动
            }
        }
        return count;
    }
};
```

**搜索空间收缩可视化**：

```
matrix = [[1,5,9],[10,11,13],[12,13,15]], k=8

值域: [1, 15]
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Round 1: lo=1, hi=15, mid=8
  count(<=8) = 2 < 8  =>  lo=9
  搜索空间: [9━━━━━━━━━━━━━━━15]

Round 2: lo=9, hi=15, mid=12
  count(<=12) = 6 < 8  =>  lo=13
  搜索空间: [13━━━━━15]

Round 3: lo=13, hi=15, mid=14
  count(<=14) = 8 >= 8  =>  hi=14
  搜索空间: [13━━14]

Round 4: lo=13, hi=14, mid=13
  count(<=13) = 8 >= 8  =>  hi=13
  搜索空间: [13]

lo == hi == 13 => return 13 ✓
```

### 解法二：最小堆 K 路归并

```cpp
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();

        // 最小堆：{值, 行, 列}
        auto cmp = [](const tuple<int,int,int>& a, const tuple<int,int,int>& b) {
            return get<0>(a) > get<0>(b);
        };
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, decltype(cmp)> pq(cmp);

        // 初始化：每行的第一个元素入堆
        for (int i = 0; i < min(n, k); i++) {
            pq.push({matrix[i][0], i, 0});
        }

        int result = 0;
        // 弹出 k 次
        for (int i = 0; i < k; i++) {
            auto [val, row, col] = pq.top();
            pq.pop();
            result = val;

            // 将同行的下一个元素入堆
            if (col + 1 < n) {
                pq.push({matrix[row][col + 1], row, col + 1});
            }
        }

        return result;
    }
};
```

**堆的工作过程**：

```
matrix = [[1,5,9],[10,11,13],[12,13,15]], k=8

初始堆: [1(0,0), 10(1,0), 12(2,0)]

Pop 1(0,0) → push 5(0,1)    堆: [5, 10, 12]      k=1
Pop 5(0,1) → push 9(0,2)    堆: [9, 10, 12]      k=2
Pop 9(0,2) → 行末无更多     堆: [10, 12]          k=3
Pop 10(1,0) → push 11(1,1)  堆: [11, 12]          k=4
Pop 11(1,1) → push 13(1,2)  堆: [12, 13]          k=5
Pop 12(2,0) → push 13(2,1)  堆: [13, 13]          k=6
Pop 13(1,2) → 行末无更多     堆: [13]              k=7
Pop 13(2,1) → push 15(2,2)  堆: [15]              k=8 → 答案=13
```

### 解法三：直接排序（暴力）

```cpp
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        vector<int> all;
        for (auto& row : matrix) {
            for (int x : row) {
                all.push_back(x);
            }
        }
        sort(all.begin(), all.end());
        return all[k - 1];
    }
};
```

---

## 六、左下角统计法详解（图解）

```
统计 <= mid 的元素个数（以 mid=12 为例）

矩阵:
     c0   c1   c2
r0 [  1    5    9 ]
r1 [ 10   11   13 ]
r2 [ 12   13   15 ]

从 (r=2, c=0) 即左下角开始：

Step 1: matrix[2][0]=12 <= 12
        count += (2+1) = 3   [1,10,12 这列都 <= 12]
        c++ → c=1
        
        ┌─────┬─────┬─────┐
        │  1✓ │  5  │  9  │
        ├─────┼─────┼─────┤
        │ 10✓ │ 11  │ 13  │
        ├─────┼─────┼─────┤
        │ 12✓ │ 13  │ 15  │  ← 当前位置
        └─────┴─────┴─────┘
        count=3, 向右走

Step 2: matrix[2][1]=13 > 12
        r-- → r=1
        
        ┌─────┬─────┬─────┐
        │  1✓ │  5  │  9  │
        ├─────┼─────┼─────┤
        │ 10✓ │ 11  │ 13  │  ← 当前位置
        ├─────┼─────┼─────┤
        │ 12✓ │     │     │
        └─────┴─────┴─────┘
        count=3, 向上走

Step 3: matrix[1][1]=11 <= 12
        count += (1+1) = 2   [5,11 这列都 <= 12]
        c++ → c=2
        
        ┌─────┬─────┬─────┐
        │  1✓ │  5✓ │  9  │
        ├─────┼─────┼─────┤
        │ 10✓ │ 11✓ │ 13  │  ← 当前位置向右
        ├─────┼─────┼─────┤
        │ 12✓ │     │     │
        └─────┴─────┴─────┘
        count=5, 向右走

Step 4: matrix[1][2]=13 > 12
        r-- → r=0
        
Step 5: matrix[0][2]=9 <= 12
        count += (0+1) = 1   [9 <= 12]
        c++ → c=3 (超出范围)
        
最终 count = 3 + 2 + 1 = 6
```

---

## 七、解法对比

| 维度 | 二分搜索值域 | 最小堆 K路归并 | 直接排序 |
|------|-------------|---------------|---------|
| 时间复杂度 | O(n * log(max-min)) | O(k * log n) | O(n^2 * log n) |
| 空间复杂度 | O(1) | O(n) | O(n^2) |
| k 很小时 | 较慢（仍需二分） | 很快 | 较慢 |
| k 很大时 | 稳定 | 慢（k接近n^2） | 较慢 |
| 面试推荐 | ⭐⭐⭐ | ⭐⭐ | ⭐ |
| Follow-up O(1)空间 | 满足 | 不满足 | 不满足 |

---

## 八、易错点

### Bug 1：二分的是下标而非值

```cpp
// 错误：二分下标 [0, n*n-1]
int lo = 0, hi = n * n - 1;
// 正确：二分值域
int lo = matrix[0][0], hi = matrix[n-1][n-1];
```

### Bug 2：统计时从右上角出发方向搞反

```cpp
// 从右上角出发也可以，但逻辑不同
// 右上角 (0, n-1)：
// matrix[r][c] <= mid → count += c+1, r++（向下走，这行有c+1个<=mid）
// matrix[r][c] > mid  → c--（向左走）
```

### Bug 3：count += r + 1 写成 count += r

```cpp
// 错误：count += r   (少算了 row 0 的元素)
// 正确：count += r + 1  (row 0 到 row r 共 r+1 个)
```

### Bug 4：二分条件写反

```cpp
// 错误：count <= k 时收缩右边界
if (count <= k) hi = mid;  // 应该是 count < k 时收缩左边界
// 正确：
if (count < k) lo = mid + 1;
else hi = mid;
```

### Bug 5：mid 溢出

```cpp
// 当 matrix 值为 10^9 级别时
int mid = (lo + hi) / 2;  // 可能溢出！
int mid = lo + (hi - lo) / 2;  // 安全写法
```

---

## 九、面试追问（递进链）

### Q1：为什么二分结束时 lo 一定在矩阵中？

二分循环的不变量是 count(lo-1) < k <= count(lo)。当 lo == hi 时：
- 如果 lo 不在矩阵中，那么 count(lo) == count(lo-1)，这与 count(lo-1) < k <= count(lo) 矛盾
- 所以 lo 一定是矩阵中的某个元素

### Q2：能否用 O(1) 空间解决？

二分搜索值域就是 O(1) 空间：只需要几个变量（lo, hi, mid, count, r, c），不需要额外数据结构。

### Q3：如果矩阵只有行有序（列不一定有序），怎么办？

- 堆解法仍然有效（K路归并 n 个有序行）
- 二分解法需要修改统计方法：每行用二分搜索 upper_bound，统计 <= mid 的个数
- 时间变为 O(n * log n * log(max-min))

### Q4：如果矩阵有重复元素，影响吗？

不影响。题目明确说"第 k 小的元素（排序后的第 k 个，不是第 k 个不同元素）"。
二分统计的 count 天然处理了重复元素。

### Q5：这道题和 "Find K-th Smallest Pair Distance" (LC 719) 有什么关系？

两者都是"二分答案 + 统计计数"的模式：
- 378：二分值，统计矩阵中 <= mid 的元素数
- 719：二分距离，统计 <= mid 的 pair 数

### Q6：题目 Follow-up 提到的 O(n) 时间算法是什么？

是 "Selection in X+Y and Matrices with Sorted Rows and Columns" 论文中的算法。
思路是结合 median-of-medians 选择算法和矩阵结构，但面试中不会考到这个深度。

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 240 | Search a 2D Matrix II | 同样利用矩阵有序性，左下角搜索 |
| 373 | Find K Pairs with Smallest Sums | K路归并，最小堆 |
| 668 | Kth Smallest Number in Multiplication Table | 二分答案 + 统计计数 |
| 719 | Find K-th Smallest Pair Distance | 二分答案经典题 |
| 786 | K-th Smallest Prime Fraction | 二分答案 + 双指针统计 |
| 23 | Merge k Sorted Lists | K路归并模板题 |

---

## 十一、总结

```
               378. Kth Smallest in Sorted Matrix
              /                                \
    解法1: 二分搜索值域                    解法2: 最小堆K路归并
         |                                      |
    二分: [min, max]                      初始化: 每行首元素入堆
         |                                      |
    统计: <= mid 几个元素?                 循环: 弹出k次
         |                                      |
    左下角阶梯走法 O(n)                    每次弹出后压入同行下一个
         |                                      |
    count < k → lo=mid+1                  第k次弹出即答案
    count >= k → hi=mid
         |
    lo==hi 即答案
```

**记忆口诀**：值域二分猜答案，左下阶梯做统计；小了右移加一列，大了上走减一行。
