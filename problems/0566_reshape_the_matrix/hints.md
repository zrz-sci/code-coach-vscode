# 566. Reshape the Matrix - 重塑矩阵

## 核心思路
将二维坐标展平为一维索引，再映射到新矩阵的二维坐标：`idx = i * n + j` -> `(idx / c, idx % c)`。

## 思维链
1. 先判断元素总数是否匹配：m * n == r * c
2. 按行遍历原矩阵，一维索引 idx 从 0 递增
3. 新矩阵的行 = idx / c，列 = idx % c
4. 本质就是一维线性存储到二维坐标的相互转换

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 一维索引映射 | O(m*n) | O(r*c) | 最简洁 |
| 双指针填充 | O(m*n) | O(r*c) | 直接操作行列指针 |

## 关键提示
1. 先检查 m*n != r*c，不相等直接返回原矩阵
2. 一维到二维映射公式：行 = idx / cols，列 = idx % cols
3. 输出矩阵就是结果本身，不算额外空间

## 解法详解

### 解法1: 一维索引映射 ⭐
```cpp
class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& mat, int r, int c) {
        int m = mat.size(), n = mat[0].size();
        if (m * n != r * c) return mat;
        
        vector<vector<int>> res(r, vector<int>(c));
        for (int idx = 0; idx < m * n; ++idx) {
            res[idx / c][idx % c] = mat[idx / n][idx % n];
        }
        return res;
    }
};
```

### 解法2: 双指针填充
```cpp
class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& mat, int r, int c) {
        int m = mat.size(), n = mat[0].size();
        if (m * n != r * c) return mat;
        
        vector<vector<int>> res(r, vector<int>(c));
        int ri = 0, ci = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                res[ri][ci] = mat[i][j];
                if (++ci == c) { ci = 0; ri++; }
            }
        }
        return res;
    }
};
```

## 易错点
- ✗ 忘记判断元素总数不匹配 → ✓ m*n != r*c 时返回原矩阵
- ✗ 用 idx/c 访问原矩阵 → ✓ 原矩阵用 idx/n 和 idx%n
- ✗ 混淆行列数 → ✓ 原矩阵列数 n，新矩阵列数 c

## 面试追问

**Q1: 能否不使用额外空间（原地 reshape）？**
不行，因为新旧矩阵维度不同，C++ 的 vector<vector<int>> 无法原地变形。在 C 语言中如果底层是连续内存则可以。

**Q2: 如果矩阵非常大，有什么优化？**
可以实现惰性求值（类似 numpy 的 view），不实际拷贝数据，只在访问时计算映射。

**Q3: 这道题的本质是什么？**
一维线性空间到二维矩阵坐标的双向映射，是矩阵操作的基础技巧。

## 相关题型
- [48. Rotate Image](https://leetcode.com/problems/rotate-image/) - 矩阵坐标变换
- [54. Spiral Matrix](https://leetcode.com/problems/spiral-matrix/) - 矩阵遍历
- [867. Transpose Matrix](https://leetcode.com/problems/transpose-matrix/) - 矩阵转置
