// 【题目本质】
// 二维矩阵的 reshape 操作：将 m*n 矩阵按行优先顺序重排为 r*c 矩阵。
// 核心是一维索引 idx 到二维坐标 (idx/cols, idx%cols) 的映射。

// 【解法总览】
// Solution1: 一维索引映射  O(m*n) / O(r*c)
// Solution2: 双指针填充    O(m*n) / O(r*c)

// ===================== Solution 1 =====================
// 一维索引映射法
// 用统一的 idx 同时映射原矩阵和新矩阵的坐标
class Solution1 {
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

// ===================== Solution 2 =====================
// 双指针填充法
// 维护新矩阵的行列指针，逐元素填入
class Solution2 {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& mat, int r, int c) {
        int m = mat.size(), n = mat[0].size();
        if (m * n != r * c) return mat;

        vector<vector<int>> res(r, vector<int>(c));
        int ri = 0, ci = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                res[ri][ci] = mat[i][j];
                if (++ci == c) {
                    ci = 0;
                    ri++;
                }
            }
        }
        return res;
    }
};

// 【解法对比】
// Solution1: 一行映射公式搞定，代码最短，面试推荐
// Solution2: 指针操作直观，适合不熟悉 idx 映射的场景

// 【易错点】
// 1. 先检查 m*n != r*c，不等时返回原矩阵
// 2. 原矩阵用 n 做除/模，新矩阵用 c 做除/模，不能混淆
// 3. 注意题目返回的是新矩阵（不是原地修改）

// 【面试追问】
// Q1: 能否O(1)空间？C++的vector<vector>无法原地reshape，底层连续内存可以
// Q2: 大矩阵优化？惰性求值/view模式，不拷贝数据只映射坐标
// Q3: 本质是什么？一维线性索引 <-> 二维坐标的互转，矩阵操作基础
