# 304. 二维区域和检索 - 矩阵不可变 (Range Sum Query 2D - Immutable) - 提示与详解

## 题目理解

设计 `NumMatrix` 类，支持：
- **构造函数**：给定 m x n 矩阵 matrix
- **sumRegion(row1, col1, row2, col2)**：O(1) 时间返回子矩形 (row1,col1) 到 (row2,col2) 的元素总和

**关键约束**：
- 1 <= m, n <= 200
- -10^4 <= matrix[i][j] <= 10^4（有负数）
- sumRegion 最多调用 10^4 次
- **必须 O(1) 查询**

**示例**：
```
matrix:
3  0  1  4  2
5  6  3  2  1
1  2  0  1  5
4  1  0  1  7
1  0  3  0  5

sumRegion(2,1,4,3) = 2+0+1 + 1+0+1 + 0+3+0 = 8
```

---

## 核心思路

这是**二维前缀和**的经典应用。核心思想是把"一维前缀和"推广到二维：

1. **一维前缀和**：prefix[i] = sum(nums[0..i-1])，区间和 = prefix[r+1] - prefix[l]
2. **二维前缀和**：prefix[i][j] = sum of 从(0,0)到(i-1,j-1)的矩形，子矩形和通过**容斥原理**计算

关键公式（容斥原理）：

**构建**：
```
prefix[i][j] = prefix[i-1][j] + prefix[i][j-1] - prefix[i-1][j-1] + matrix[i-1][j-1]
```

**查询**：
```
sumRegion(r1,c1,r2,c2) = prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1]
```

---

## 思维链：从暴力到最优

### Step 1: 暴力查询 -- 每次遍历子矩形

每次 sumRegion 遍历 (row1,col1) 到 (row2,col2) 的所有元素求和。

时间：构造 O(1)，查询 O(m*n)。10^4 次查询 * 200*200 = 4*10^8，太慢。

### Step 2: 一维前缀和 -- 按行预处理

对每一行计算前缀和。查询时对 row1 到 row2 的每一行用 O(1) 算出该行的列区间和，再相加。

时间：构造 O(m*n)，查询 O(m)。改进了但不是 O(1)。

### Step 3: 二维前缀和 -- O(1) 查询

**关键洞察**：用容斥原理把二维前缀和分解。

定义 `prefix[i][j]` = 从 (0,0) 到 (i-1,j-1) 这个矩形的所有元素之和。

**构建过程**（用容斥组合）：
```
prefix[i][j] = 上方 + 左方 - 左上方(重叠) + 当前格
             = prefix[i-1][j] + prefix[i][j-1] - prefix[i-1][j-1] + matrix[i-1][j-1]
```

**查询过程**（用容斥拆解）：
```
sumRegion(r1,c1,r2,c2)
  = 整个大矩形 - 上方多出 - 左方多出 + 左上角(减多了加回来)
  = prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1]
```

### Step 4: padding 技巧 -- 消除边界判断

prefix 数组大小设为 (m+1) x (n+1)，第 0 行和第 0 列全为 0。这样 i-1 和 j-1 永远不会越界，不需要特判 i=0 或 j=0 的情况。

---

## 解法概览

| 解法 | 构造时间 | 查询时间 | 空间 | 特点 |
|------|---------|---------|------|------|
| 解法1: 二维前缀和 ⭐ | O(mn) | O(1) | O(mn) | 唯一正解 |
| 解法2: 行前缀和 | O(mn) | O(m) | O(mn) | 不满足 O(1) 要求 |

---

## 关键提示

### 提示 1: 容斥原理的图形化理解
把前缀和想象为面积。要求子矩形面积 = 大矩形面积 - 上方矩形 - 左方矩形 + 左上角矩形（因为被减了两次）。

```
+-----+-----+
|  D  |  C  |
+-----+-----+  <- row1
|  B  |  A  |
+-----+-----+  <- row2
      ^     ^
     col1  col2

A = 整体 - B - C + D
  = prefix[r2+1][c2+1] - prefix[r2+1][c1] - prefix[r1][c2+1] + prefix[r1][c1]
```

### 提示 2: prefix 的下标偏移
prefix 比 matrix 多一行一列（padding），所以 matrix[i][j] 对应 prefix[i+1][j+1]。查询时 row1/col1 不需要 +1，row2/col2 需要 +1。

### 提示 3: 构建顺序
从左到右、从上到下遍历即可，因为 prefix[i][j] 只依赖 prefix[i-1][j]、prefix[i][j-1]、prefix[i-1][j-1]，这三个都已经算过了。

### 提示 4: 不可变 vs 可变
本题矩阵不可变（Immutable），所以预处理一次即可。如果矩阵可变（LeetCode 308），需要使用二维树状数组或线段树。

---

## 解法详解

### 解法 1: 二维前缀和（唯一正解）⭐

**完整代码**：
```cpp
class NumMatrix {
public:
    vector<vector<int>> prefix;

    NumMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        prefix.assign(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                prefix[i][j] = prefix[i-1][j] + prefix[i][j-1]
                             - prefix[i-1][j-1] + matrix[i-1][j-1];
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        return prefix[row2+1][col2+1] - prefix[row1][col2+1]
             - prefix[row2+1][col1] + prefix[row1][col1];
    }
};
```

**思考过程**：
1. 先理解一维前缀和：prefix[i] = prefix[i-1] + nums[i-1]
2. 推广到二维：prefix[i][j] 等于"上方 + 左方 - 左上角 + 当前格"
3. 查询同理：子矩形 = 大矩形 - 上方多余 - 左方多余 + 减多的左上角

**前缀和构建模拟**（以示例为例）：

```
原始矩阵 matrix:             前缀和 prefix (多一行一列padding):
3  0  1  4  2               0   0   0   0   0   0
5  6  3  2  1               0   3   3   4   8  10
1  2  0  1  5               0   8  14  18  24  27
4  1  0  1  7               0   9  17  21  28  36
1  0  3  0  5               0  13  22  26  34  49
                            0  14  23  30  38  58

验证 prefix[3][3]:
= prefix[2][3] + prefix[3][2] - prefix[2][2] + matrix[2][2]
= 18 + 17 - 14 + 0 = 21
含义: sum of 3x3 子矩形 from (0,0) to (2,2)
= 3+0+1 + 5+6+3 + 1+2+0 = 21 (正确)

查询 sumRegion(2,1,4,3):
= prefix[5][4] - prefix[2][4] - prefix[5][1] + prefix[2][1]
= 38 - 24 - 14 + 8 = 8 (正确)
```

**关键点**：
- prefix 大小 (m+1) x (n+1)，第 0 行/列全 0，消除边界判断
- 构建和查询都用容斥原理：加加减减
- 下标偏移：matrix[i][j] 对应 prefix[i+1][j+1]

---

### 解法 2: 行前缀和（不满足 O(1)，用于对比理解）

**完整代码**：
```cpp
class NumMatrix {
public:
    vector<vector<int>> rowPrefix;

    NumMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        rowPrefix.assign(m, vector<int>(n + 1, 0));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                rowPrefix[i][j + 1] = rowPrefix[i][j] + matrix[i][j];
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        int sum = 0;
        for (int i = row1; i <= row2; i++)
            sum += rowPrefix[i][col2 + 1] - rowPrefix[i][col1];
        return sum;
    }
};
```

**思考过程**：
1. 对每一行独立计算前缀和
2. 查询时逐行计算列区间和，再累加
3. 查询 O(m)，不满足题目 O(1) 要求
4. 但有助于理解从一维到二维的推导过程

**关键点**：
- 比暴力好（每行 O(1) 求列区间和），但还不够
- 可以作为面试中"先说不够优的方案再优化"的过渡

---

## 解法对比

| 维度 | 解法1: 二维前缀和 ⭐ | 解法2: 行前缀和 |
|------|---------------------|----------------|
| 构造时间 | O(mn) | O(mn) |
| 查询时间 | **O(1)** | O(m) |
| 空间 | O(mn) | O(mn) |
| 满足要求 | 是 | 否 |
| 核心技巧 | 容斥原理 | 一维前缀和 |

---

## 易错点

### Bug 1: prefix 下标偏移搞混
```cpp
// 错误：没有 +1，导致越界或计算错误
prefix[i][j] = prefix[i-1][j] + prefix[i][j-1]
             - prefix[i-1][j-1] + matrix[i][j];
// matrix[i][j] 应该是 matrix[i-1][j-1]!

// 正确：
prefix[i][j] = prefix[i-1][j] + prefix[i][j-1]
             - prefix[i-1][j-1] + matrix[i-1][j-1];
```

### Bug 2: 查询公式符号写错
```cpp
// 错误：容斥的加减号搞反
return prefix[r2+1][c2+1] + prefix[r1][c2+1]
     - prefix[r2+1][c1] - prefix[r1][c1];
// 应该是 -prefix[r1][c2+1] 和 +prefix[r1][c1]

// 正确：
return prefix[r2+1][c2+1] - prefix[r1][c2+1]
     - prefix[r2+1][c1] + prefix[r1][c1];
```

### Bug 3: 忘记 padding
```cpp
// 错误：prefix 和 matrix 同大小，i=0 或 j=0 时越界
vector<vector<int>> prefix(m, vector<int>(n, 0));
prefix[i][j] = prefix[i-1][j] + ...  // i=0 时 i-1=-1 越界!

// 正确：多一行一列
prefix.assign(m + 1, vector<int>(n + 1, 0));
```

### Bug 4: int 溢出（大矩阵场景）
```cpp
// 本题 200*200 矩阵最大和 = 200*200*10^4 = 4*10^8，int 够用
// 但如果 n 更大或值更大，需要 long long
```

---

## 面试追问

### Q1: 如果矩阵可以修改（update 某个元素），怎么做？
**A1**: 二维前缀和在更新时需要 O(mn) 重建。更好的方案是**二维树状数组（Binary Indexed Tree）**，更新和查询都是 O(log m * log n)。这就是 LeetCode 308 的内容。

### Q2: 如何把二维前缀和推广到三维？
**A2**: 三维前缀和用 8 项容斥：prefix[i][j][k] 的构建需要加3个面、减3条边、加1个角。查询同理。公式更复杂但原理一样。

### Q3: 如果要求的不是矩形区域，而是圆形或任意形状，怎么办？
**A3**: 前缀和只能处理矩形（轴对齐）区域。任意形状需要对每个查询遍历区域内的所有点，或者用更复杂的数据结构（如 KD-Tree）。但对于"给定若干查询，每个查询一个圆"这类问题，通常只能 O(面积) 处理每个查询。

---

## 相关题型

| 题目 | 关联 | 复用点 |
|------|------|--------|
| [303. 区域和检索 - 数组不可变](../0303_range_sum_query_immutable/) | 一维版本 | 一维前缀和是本题的基础 |
| [307. 区域和检索 - 数组可修改](../0307_range_sum_query_mutable/) | 可变版本 | 需要树状数组/线段树 |
| [1314. 矩阵区域和](../1314_matrix_block_sum/) | 直接应用 | 用二维前缀和计算每个格子的邻域和 |
| [363. 矩形区域不超过K的最大数值和](../0363_max_sum_of_rectangle_no_larger_than_k/) | 进阶 | 前缀和 + 有序集合/二分搜索 |
| [835. 图像重叠](../0835_image_overlap/) | 偏移匹配 | 可以用二维前缀和优化重叠计算 |
