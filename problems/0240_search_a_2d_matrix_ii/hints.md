# 240. 搜索二维矩阵 II

## 核心思路

本质上是在一个**行有序、列有序但整体不完全有序**的矩阵中查找目标值。关键是找到一个"有方向性"的起点，使得每一步比较都能**排除一整行或一整列**。

## 思维链

1. **读完题第一反应**：最简单的方式就是遍历整个矩阵，逐个比较，O(m×n)。但题目说"高效算法"，肯定不够。

2. **暴力解的瓶颈在哪？** 每个元素都要看一遍，完全没利用"行有序、列有序"这个性质。

3. **怎么利用有序性？** 每行有序 → 可以对每行做二分查找，O(m·log n)。这比暴力好了，但还没同时利用"列有序"。

4. **能否同时利用行和列的有序性？** 想象站在矩阵的**右上角**：当前值比 target 大 → 往左走（排除当前列）；当前值比 target 小 → 往下走（排除当前行）。每一步排除一行或一列，最多走 m+n 步！

5. **为什么右上角有效？** 因为右上角是一个"十字路口"：往左数值变小，往下数值变大，形成了类似 BST 的判断结构。左下角同理。而左上角（两个方向都变大）和右下角（两个方向都变小）无法做出有效判断。

6. **还有没有其他方法？** 分治法：取矩阵中心，比较后可以排除四个象限中的一个，递归处理剩余三个象限。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力遍历 | 逐个检查 | O(mn) | O(1) | 能说出即可 |
| 逐行二分 | 对每行做二分查找 | O(m·log n) | O(1) | 能说出即可 |
| 右上角出发（Z字查找） | 利用行列双有序性 | O(m+n) | O(1) | ⭐ 必须写出 |
| 分治法 | 排除1/4区域递归 | O(n^log₂3) ≈ O(n^1.58) | O(log n) | 加分项 |

## 关键提示

- **提示1**：在矩阵中找一个特殊的起始位置，使得向两个方向移动时，一个方向值增大，另一个方向值减小（类似 BST 的左小右大）。

- **提示2**：右上角 `matrix[0][n-1]` 和左下角 `matrix[m-1][0]` 都是这样的"分叉点"。

- **提示3**：矩阵示意图（从右上角出发找 target=5）：
```
//  矩阵 (m=5, n=5):
//   1   4   7  11 [15] ← 起点(右上角)
//   2   5   8  12  19    15>5 → 左移
//   3   6   9  16  22
//  10  13  14  17  24
//  18  21  23  26  30
//
//   1   4   7 [11]       11>5 → 左移
//   1   4  [7]            7>5 → 左移
//   1  [4]                4<5 → 下移
//   2  [5]                5==5 → 找到!
```

- **提示4**：对于分治法，想想把矩阵分成四个象限。如果中心值 > target，右下象限一定都 > target，可以排除。

- **提示5**：注意这道题和 [74. 搜索二维矩阵](https://leetcode.cn/problems/search-a-2d-matrix/) 的区别：74 题每行首元素大于上一行末元素（可以拉成一维二分），本题没有这个性质。

## 解法详解

### 解法1: 暴力遍历 — O(mn) / O(1)

**思考过程**：不利用任何性质，直接扫描所有元素。

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                if (matrix[i][j] == target) return true;
            }
        }
        return false;
    }
};
```

**关键点**：完全没利用有序性，m=n=300 时约 9 万次比较，能过但不是面试答案。

---

### 解法2: 逐行二分查找 — O(m·log n) / O(1)

**从解法1优化**：解法1 对每行都做线性扫描 O(n)。既然每行有序，可以用二分查找将每行的查找降到 O(log n)。

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        for (int i = 0; i < matrix.size(); i++) {
            // 利用行有序性：二分查找当前行
            int lo = 0, hi = matrix[0].size() - 1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (matrix[i][mid] == target) return true;
                else if (matrix[i][mid] < target) lo = mid + 1;
                else hi = mid - 1;
            }
        }
        return false;
    }
};
```

**进一步优化**：可以利用列有序性做剪枝——如果当前行的第一个元素 > target，后面所有行都不用看了（因为列也有序，下面的行更大）。同理，如果当前行最后一个元素 < target，这行可以跳过。

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size(), n = matrix[0].size();
        for (int i = 0; i < m; i++) {
            // 剪枝：当前行最小值已经 > target，后续行更大，直接结束
            if (matrix[i][0] > target) break;
            // 剪枝：当前行最大值 < target，这行不可能有，跳过
            if (matrix[i][n - 1] < target) continue;
            
            int lo = 0, hi = n - 1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (matrix[i][mid] == target) return true;
                else if (matrix[i][mid] < target) lo = mid + 1;
                else hi = mid - 1;
            }
        }
        return false;
    }
};
```

**关键点**：只利用了行有序性做二分，列有序性只用于剪枝，没有同时利用两者的结构性。

---

### 解法3: 右上角出发（Z字查找） — O(m+n) / O(1) ⭐ 面试首选

**从解法2的瓶颈出发**：解法2 每行独立二分，无法利用"前一行的搜索结果"来缩小当前行的范围。我们需要一种方法能**同时利用行有序和列有序**。

**核心观察**：从右上角 `(0, n-1)` 出发：
- 当前值 == target → 找到
- 当前值 > target → 左移（排除当前列，因为下面的都更大）
- 当前值 < target → 下移（排除当前行，因为左边的都更小）

每步排除一行或一列，总共最多 m+n 步。

```
//  搜索 target=14 的过程:
//
//   1   4   7  11 [15]  15>14 → 左移 (col: 4→3)
//   2   5   8 [12] 19   12<14 → 下移 (row: 1→2)
//   3   6   9 [16] 22   16>14 → 左移 (col: 3→2)
//   3   6  [9]          9<14  → 下移 (row: 2→3)
//  10  13 [14]          14==14 → 找到! 位置(3,2)
```

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size(), n = matrix[0].size();
        // 从右上角出发
        int row = 0, col = n - 1;
        
        while (row < m && col >= 0) {
            if (matrix[row][col] == target) {
                return true;
            } else if (matrix[row][col] > target) {
                col--;  // 当前值太大，排除这一列（下面更大）
            } else {
                row++;  // 当前值太小，排除这一行（左边更小）
            }
        }
        return false;
    }
};
```

**为什么正确？**
- 左移排除的是当前列：因为列从上到下递增，`matrix[row][col] > target` 意味着这一列从 row 往下的所有值都 > target。
- 下移排除的是当前行：因为行从左到右递增，`matrix[row][col] < target` 意味着这一行从 col 往左的所有值都 < target。
- 每步至少排除一行或一列，不会遗漏。

**关键点**：也可以从左下角 `(m-1, 0)` 出发，方向反转（大了往上，小了往右），同样 O(m+n)。

---

### 解法4: 分治法 — O(n^log₂3) / O(log(mn))

**另一种思路**：将矩阵分成四个象限，利用比较结果排除其中一个象限，对剩余三个象限递归。

```
//  矩阵分四个象限:
//  ┌───────┬───────┐
//  │ 左上  │ 右上  │
//  │ (min) │       │
//  ├───────┼───────┤
//  │       │ 右下  │
//  │ 左下  │ (max) │
//  └───────┴───────┘
//
//  取中心点 matrix[midR][midC]:
//  - 如果 > target: 右下象限全部 > target，排除
//    递归: 左上、右上、左下
//  - 如果 < target: 左上象限全部 < target，排除
//    递归: 右上、左下、右下
//  - 如果 == target: 找到
```

```cpp
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size(), n = matrix[0].size();
        return divideAndConquer(matrix, target, 0, 0, m - 1, n - 1);
    }
    
private:
    bool divideAndConquer(vector<vector<int>>& matrix, int target,
                          int r1, int c1, int r2, int c2) {
        // 越界：搜索区域无效
        if (r1 > r2 || c1 > c2) return false;
        
        // 剪枝：target 不在当前子矩阵的值域 [min, max] 内
        if (target < matrix[r1][c1] || target > matrix[r2][c2]) return false;
        
        // 取中间行
        int midR = r1 + (r2 - r1) / 2;
        
        // 在中间行上二分查找，找到 <= target 的最右列
        int lo = c1, hi = c2;
        while (lo <= hi) {
            int midC = lo + (hi - lo) / 2;
            if (matrix[midR][midC] == target) return true;
            else if (matrix[midR][midC] < target) lo = midC + 1;
            else hi = midC - 1;
        }
        // 此时 hi 是中间行中 <= target 的最右列，lo 是 > target 的最左列
        
        // 左下区域: [midR+1, r2] x [c1, hi] — 中间行左半部分下方
        // 右上区域: [r1, midR-1] x [lo, c2] — 中间行右半部分上方
        return divideAndConquer(matrix, target, midR + 1, c1, r2, hi) ||
               divideAndConquer(matrix, target, r1, lo, midR - 1, c2);
    }
};
```

**关键点**：
- 分治的精妙在于通过中间行的二分将矩阵切成两个不重叠的子矩阵（左下和右上），每个子矩阵都严格小于原矩阵。
- 时间复杂度分析：T(n) = 2T(n/2) + O(log n)，解得 O(n^log₂3) ≈ O(n^1.58)（对于 m≈n 的情况）。
- 实际面试中不太常考，但能展示对分治的深刻理解。

## 解法对比

| | 暴力 | 逐行二分 | Z字查找 ⭐ | 分治 |
|---|---|---|---|---|
| 时间 | O(mn) | O(m·log n) | O(m+n) | O(n^1.58) |
| 空间 | O(1) | O(1) | O(1) | O(log n) |
| 利用的性质 | 无 | 行有序 | 行+列有序 | 行+列有序 |
| 代码复杂度 | 最简单 | 简单 | 非常简单 | 较复杂 |
| 面试推荐度 | 低 | 中 | **高** | 中（加分） |

**关键区别**：
- Z字查找是最优解中最简洁的，面试时首选。思路一旦理解，代码只有 10 行。
- 逐行二分在 m 远小于 n 时有优势（O(m·log n) 可能小于 O(m+n)）。
- 分治法适合展示算法思维深度，但实际编码较复杂。

## 易错点

1. **起点选错**：
   - ✗ 从左上角 `(0,0)` 出发 — 向右和向下都是变大，无法判断方向
   - ✗ 从右下角 `(m-1,n-1)` 出发 — 向左和向上都是变小，同样无法判断
   - ✓ 只能从右上角 `(0,n-1)` 或左下角 `(m-1,0)` 出发

2. **边界条件写反**：
   - ✗ `while (row >= 0 && col < n)` — 这是左下角出发的条件
   - ✓ 右上角出发：`while (row < m && col >= 0)`

3. **分治法的子区域划分错误**：
   - ✗ 简单地把四个象限递归三个 — 会有大量重叠，复杂度退化
   - ✓ 通过中间行二分，切成两个**不重叠**的子矩阵

4. **与 LeetCode 74 混淆**：
   - LeetCode 74 的矩阵可以当做一维有序数组直接二分 O(log(mn))
   - 本题(240)不能这样做，因为行与行之间没有"首元素大于上一行末元素"的关系

## 面试追问

**Q1: 暴力解的复杂度是多少？能优化到多少？**
→ 暴力 O(mn)。利用每行有序可以做逐行二分 O(m·log n)。但最优是 O(m+n) 的 Z 字查找。

**Q2: 为什么从右上角出发有效，左上角不行？**
→ 右上角向左变小、向下变大，形成类似 BST 的结构，每步能排除一行或一列。左上角两个方向都变大，无法判断该往哪走。左下角也可以（向上变小、向右变大）。

**Q3: 如果矩阵是方阵(n×n)，你能做到比 O(n) 更好吗？**
→ 不能。任何确定性算法在最坏情况下至少需要 Ω(n) 次比较（可以用对抗论证证明）。但分治法的 O(n^1.58) 在 n 很大时反而比 O(m+n) 差（当 m≈n 时）。所以 Z 字查找在最坏情况下已经是最优的。

**Q4: 这道题和 LeetCode 74 的区别是什么？如果面试官给的是 74 题的矩阵条件，怎么做？**
→ 74 题额外保证每行首元素 > 上一行末元素，所以整个矩阵拉成一维是完全有序的，可以直接一次二分 O(log(mn))。本题只保证行内有序和列内有序，不能拉成一维。

**Q5: 如果需要查找多次（大量查询），如何预处理？**
→ 可以将矩阵所有元素放入 HashSet，O(mn) 预处理，之后每次查询 O(1)。或者如果查询有范围，可以预处理前缀结构。

## 相关题型

- **[74. 搜索二维矩阵](https://leetcode.cn/problems/search-a-2d-matrix/)** — 更强的有序条件(行尾<下行首)，可以用一次二分 O(log(mn))。本题的 Z 字查找也可以解 74 题，但杀鸡用牛刀。
- **[378. 有序矩阵中第 K 小的元素](https://leetcode.cn/problems/kth-smallest-element-in-a-sorted-matrix/)** — 同样的矩阵结构，但问的是第 K 小而非查找。复用"行列双有序"的性质，用堆或二分答案解决。
- **[668. 乘法表中第 K 小的数](https://leetcode.cn/problems/kth-smallest-number-in-multiplication-table/)** — 矩阵满足行列有序（乘法表），二分答案 + 逐行计数，复用"有序矩阵"的 count 思路。