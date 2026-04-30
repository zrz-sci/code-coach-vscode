# 308. 二维区域和检索 - 矩阵可修改 (Range Sum Query 2D - Mutable)

**难度**: Medium | **标签**: Design, Binary Indexed Tree, Segment Tree, Array, Matrix

## 核心思路

本题是 307 题的**二维扩展**：需要在二维矩阵上同时支持**单点更新**和**矩形区域求和**。核心思想是将一维的树状数组（BIT）推广到二维：

- 一维 BIT：`tree[x]` 管理一段连续区间的和
- 二维 BIT：`tree[x][y]` 管理一个矩形区域的和

二维 BIT 的操作就是在两个维度分别做 lowbit 跳跃，update 和 query 各嵌套两层循环。

## 思维链

1. **回顾一维**：307 题用 BIT 实现 O(log n) 的单点更新和前缀和查询
2. **二维推广**：对行和列分别建立 BIT，形成二维树状数组
3. **前缀和**：`query(r, c)` 返回 `[0,0]` 到 `[r,c]` 矩形的和
4. **容斥原理**：`sumRegion(r1,c1,r2,c2) = q(r2,c2) - q(r1-1,c2) - q(r2,c1-1) + q(r1-1,c1-1)`
5. **更新传递差值**：与一维相同，update 传的是 `val - matrix[row][col]`

## 解法概览

| 解法 | 初始化 | update | sumRegion | 推荐度 | 说明 |
|------|--------|--------|-----------|--------|------|
| 二维 BIT | O(mn log m log n) | O(log m log n) | O(log m log n) | ⭐⭐⭐⭐⭐ | 代码简洁，效率高 |
| 二维线段树 | O(mn) | O(log m log n) | O(log m log n) | ⭐⭐⭐ | 实现复杂，面试不推荐 |
| 按行 BIT | O(mn log n) | O(log n) | O(m log n) | ⭐⭐⭐ | 折中方案 |

## 关键提示

1. **二维 BIT 索引从 (1,1) 开始**：数组大小 `(m+1) x (n+1)`
2. **update 嵌套循环**：外层对行做 lowbit 跳跃，内层对列做 lowbit 跳跃
3. **query 嵌套循环**：同样两层 lowbit 反向跳跃
4. **容斥原理**：矩形区域和 = 四个前缀和的加减
5. **保存原始矩阵**：用于计算 update 时的差值

## 解法详解

### 解法一：二维树状数组 BIT（推荐）

```cpp
class NumMatrix {
    vector<vector<int>> tree;
    vector<vector<int>> mat;
    int m, n;
    
    int lowbit(int x) { return x & (-x); }
    
    void add(int r, int c, int delta) {
        for (int i = r; i <= m; i += lowbit(i))
            for (int j = c; j <= n; j += lowbit(j))
                tree[i][j] += delta;
    }
    
    int query(int r, int c) {
        int sum = 0;
        for (int i = r; i > 0; i -= lowbit(i))
            for (int j = c; j > 0; j -= lowbit(j))
                sum += tree[i][j];
        return sum;
    }
    
public:
    NumMatrix(vector<vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        mat = matrix;
        tree.assign(m + 1, vector<int>(n + 1, 0));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                add(i + 1, j + 1, matrix[i][j]);
    }
    
    void update(int row, int col, int val) {
        add(row + 1, col + 1, val - mat[row][col]);
        mat[row][col] = val;
    }
    
    int sumRegion(int row1, int col1, int row2, int col2) {
        return query(row2 + 1, col2 + 1)
             - query(row1, col2 + 1)
             - query(row2 + 1, col1)
             + query(row1, col1); // 容斥原理
    }
};
```

### 解法二：按行建 BIT

```cpp
class NumMatrix {
    vector<vector<int>> trees; // 每行一个 BIT
    vector<vector<int>> mat;
    int m, n;
    
    int lowbit(int x) { return x & (-x); }
    
    void add(int row, int col, int delta) {
        for (int j = col; j <= n; j += lowbit(j))
            trees[row][j] += delta;
    }
    
    int query(int row, int col) {
        int sum = 0;
        for (int j = col; j > 0; j -= lowbit(j))
            sum += trees[row][j];
        return sum;
    }
    
public:
    NumMatrix(vector<vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        mat = matrix;
        trees.assign(m, vector<int>(n + 1, 0));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                add(i, j + 1, matrix[i][j]);
    }
    
    void update(int row, int col, int val) {
        add(row, col + 1, val - mat[row][col]);
        mat[row][col] = val;
    }
    
    int sumRegion(int row1, int col1, int row2, int col2) {
        int sum = 0;
        for (int i = row1; i <= row2; i++)
            sum += query(i, col2 + 1) - query(i, col1);
        return sum;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 二维 BIT 从 (0,0) 开始 | ✓ 从 (1,1) 开始，数组大小 (m+1)x(n+1) | lowbit(0)=0 死循环 |
| ✗ `add(row, col, val)` 传新值 | ✓ `add(row+1, col+1, val - mat[row][col])` | BIT 的 add 是累加 |
| ✗ 忘记容斥的加回项 | ✓ `+query(row1, col1)` 不能漏 | 减多了要加回来 |
| ✗ 不保存原始矩阵 | ✓ 用 mat 保存，update 时计算差值 | 差值 = 新值 - 旧值 |
| ✗ query 边界传错 | ✓ `query(row2+1, col2+1) - query(row1, col2+1) - ...` | 0-indexed → 1-indexed |

## 面试追问

**Q1: 二维 BIT 的时空复杂度？**
→ 初始化 O(mn log m log n)；update O(log m * log n)；sumRegion O(log m * log n)。空间 O(mn)。

**Q2: 如何从一维 BIT 推广到二维？**
→ 一维 BIT 的 add/query 是单层 lowbit 循环。二维就是嵌套两层：外层对行做 lowbit 跳跃，内层对列做 lowbit 跳跃。本质是对每个行区间，再维护一个列方向的 BIT。

**Q3: 如果需要支持矩形区域的批量更新（区间更新 + 区间查询），怎么做？**
→ 二维差分 + 二维 BIT。维护差分矩阵 d[i][j]，区间更新时修改四个角的差分值，查询时用两个二维 BIT 还原前缀和。

## 相关题型

- [307. Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/) - 一维版本
- [304. Range Sum Query 2D - Immutable](https://leetcode.com/problems/range-sum-query-2d-immutable/) - 二维不可变版本（前缀和）
- [315. Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/) - BIT 经典应用
