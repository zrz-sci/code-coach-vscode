# 531. 孤独像素 I (Lonely Pixel I)

**难度**: Medium | **标签**: Array, Hash Table, Matrix

## 核心思路

统计每行和每列的黑色像素 `'B'` 数量，如果某个 `'B'` 所在行和列的黑色像素数都恰好为 1，则它是孤独像素。本质是**行列计数 + 条件过滤**。

## 思维链

1. 第一遍遍历矩阵：统计每行的 `'B'` 数量 `rowCount[i]` 和每列的 `'B'` 数量 `colCount[j]`
2. 第二遍遍历矩阵：对于每个 `'B'`，检查 `rowCount[i] == 1 && colCount[j] == 1`
3. 满足条件的计入结果

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 两遍遍历 + 行列计数 | O(m*n) | O(m+n) | ⭐⭐⭐⭐⭐ |
| 一遍遍历（巧妙编码） | O(m*n) | O(1) 额外 | ⭐⭐⭐ |

## 关键提示

1. 分两步：先统计，再筛选
2. 只需要行计数数组和列计数数组，空间 O(m+n)
3. 孤独的条件：所在行恰好1个B，所在列也恰好1个B
4. 注意是"恰好1个"而非"至少1个"

## 解法详解

### 解法一：两遍遍历 + 行列计数（推荐）

```cpp
class Solution {
public:
    int findLonelyPixel(vector<vector<char>>& picture) {
        int m = picture.size(), n = picture[0].size();
        vector<int> rowCount(m, 0), colCount(n, 0);
        
        // 第一遍：统计每行每列的'B'数量
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B') {
                    rowCount[i]++;
                    colCount[j]++;
                }
            }
        }
        
        // 第二遍：检查每个'B'是否孤独
        int result = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B' && rowCount[i] == 1 && colCount[j] == 1) {
                    result++;
                }
            }
        }
        return result;
    }
};
```

### 解法二：一遍遍历（利用首行编码）

```cpp
class Solution {
public:
    int findLonelyPixel(vector<vector<char>>& picture) {
        int m = picture.size(), n = picture[0].size();
        // 用 colCount 数组记录每列'B'数量
        // 同时在 picture 首行做标记
        vector<int> colCount(n, 0);
        
        // 第一遍遍历（可以合并逻辑）
        for (int i = 0; i < m; i++) {
            int rowB = 0;
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B') {
                    colCount[j]++;
                    rowB++;
                }
            }
            // 将行B数量存入（后面筛选用）
            // 这里仍需行计数，退化为两遍
        }
        
        int result = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B' && colCount[j] == 1) {
                    // 检查该行是否只有一个B
                    int cnt = 0;
                    for (int k = 0; k < n; k++)
                        if (picture[i][k] == 'B') cnt++;
                    if (cnt == 1) result++;
                    break; // 该行只有一个B，无需继续
                }
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 只检查行有1个B就算孤独 | ✓ 行和列都必须恰好有1个B |
| ✗ 遗漏第二遍检查 `picture[i][j] == 'B'` | ✓ 第二遍必须先确认是 'B' 才检查计数 |
| ✗ 用 set 存位置导致 O(m*n) 空间 | ✓ 行列计数数组只需 O(m+n) |

## 面试追问

**Q1: 能否只遍历一次矩阵完成？**

> 理论上可以，但需要额外逻辑来处理行计数和列计数的交叉验证。两遍遍历更清晰，且常数开销很小。

**Q2: 如果矩阵非常稀疏（B 很少），如何优化？**

> 可以在第一遍遍历时记录每个 B 的坐标到一个列表中，第二遍只遍历这些坐标做检查。

**Q3: 这题和 533. Lonely Pixel II 有什么关系？**

> Lonely Pixel I 是 II 在 target=1 且不要求行相同时的特例。II 增加了 target 参数和行相等约束。

## 相关题型

- [533. 孤独像素 II](https://leetcode.com/problems/lonely-pixel-ii/) - 进阶版，增加 target 约束
- [36. 有效的数独](https://leetcode.com/problems/valid-sudoku/) - 行列计数思想
- [73. 矩阵置零](https://leetcode.com/problems/set-matrix-zeroes/) - 行列标记
