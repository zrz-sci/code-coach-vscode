/*
 * LeetCode 311: Sparse Matrix Multiplication (稀疏矩阵的乘法)
 *
 * 【题目本质】
 * 矩阵乘法，但矩阵大量元素为 0。核心是跳过涉及 0 的无效乘法运算。
 *
 * 【解法总览】
 * 解法1: 暴力三重循环       — O(m·k·n) / O(1) — 标准矩阵乘法，未利用稀疏性
 * 解法2: 调换循环顺序+跳零  — O(m·k·n) 最坏 / O(1) — 面试首选，简洁利用稀疏性
 * 解法3: 预处理非零元素      — O(非零乘法次数) / O(非零元素数) — 极度稀疏时最优
 *
 * 【矩阵乘法公式回顾】
 * result[i][j] = Σ(p=0..k-1) mat1[i][p] * mat2[p][j]
 *
 * 示例:
 *   mat1 (2×3):       mat2 (3×3):       result (2×3):
 *   [1  0  0]         [7  0  0]         [7   0  0]
 *   [-1 0  3]    ×    [0  0  0]    =    [-7  0  3]
 *                     [0  0  1]
 *
 *   result[0][0] = 1*7 + 0*0 + 0*0 = 7
 *   result[1][0] = -1*7 + 0*0 + 3*0 = -7
 *   result[1][2] = -1*0 + 0*0 + 3*1 = 3
 */

// ============================================================
// 解法1: 暴力三重循环 — 标准矩阵乘法
// 时间: O(m·k·n)  空间: O(1) 额外（不算输出矩阵）
//
// 【思路】
// 直接套矩阵乘法公式，三重循环 i→j→p。
// 每一次乘法都做，即使一个操作数是 0。
// 完全没利用稀疏性，但代码最简单，适合验证正确性。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> multiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2) {
        int m = mat1.size(), k = mat1[0].size(), n = mat2[0].size();
        vector<vector<int>> result(m, vector<int>(n, 0));
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int p = 0; p < k; p++) {
                    result[i][j] += mat1[i][p] * mat2[p][j];
                }
            }
        }
        return result;
    }
};

// ============================================================
// 解法2: 调换循环顺序 + 跳零 — 面试首选 ⭐
// 时间: O(m·k·n) 最坏，实际远小于  空间: O(1) 额外
//
// 【思路】
// 解法1 的循环顺序是 i→j→p，mat1[i][p] 在最内层循环里，
// 即使它为 0，也要先进入 j 循环才能判断。
//
// 关键优化：把循环顺序改为 i→p→j。
// 这样 mat1[i][p] 在 j 循环之外，如果为 0，整个 j 循环都跳过。
//
// 循环顺序对比：
//   i→j→p: 对每个(i,j)遍历所有p，无法提前跳过
//   i→p→j: 先固定 mat1[i][p]，为0就 continue，省掉 n 次乘法
//
// 为什么正确？
//   result[i][j] 是一个累加和（+= 操作），累加顺序不影响结果。
//   调换循环只是改变了累加到 result[i][j] 的顺序，不影响最终值。
// ============================================================
class Solution2 {
public:
    vector<vector<int>> multiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2) {
        int m = mat1.size(), k = mat1[0].size(), n = mat2[0].size();
        vector<vector<int>> result(m, vector<int>(n, 0));
        
        for (int i = 0; i < m; i++) {
            for (int p = 0; p < k; p++) {
                // 核心：mat1[i][p] 为 0 时，0 * mat2[p][j] = 0 对所有 j
                // 一次 continue 跳过 n 次乘法
                if (mat1[i][p] == 0) continue;
                
                for (int j = 0; j < n; j++) {
                    // 可选优化：mat2[p][j] 为 0 也跳过这一次
                    if (mat2[p][j] == 0) continue;
                    result[i][j] += mat1[i][p] * mat2[p][j];
                }
            }
        }
        return result;
    }
};

// ============================================================
// 解法3: 预处理非零元素（稀疏表示） — 极度稀疏时最优
// 时间: 预处理 O(m·k + k·n)，乘法 O(Σ |sparseA[i]| * |sparseB[p]|)
// 空间: O(非零元素总数)
//
// 【思路】
// 解法2 仍需遍历 mat1 每行的所有 k 个元素来寻找非零项。
// 如果矩阵极其稀疏（如 10000×10000 但只有 100 个非零元素），
// 大量时间浪费在检查 0 上。
//
// 优化：预处理出每行的非零元素列表，直接遍历非零元素。
// 这是稀疏矩阵 CSR (Compressed Sparse Row) 格式的简化版。
//
// 预处理示例：
//   mat1 = [[1, 0, 0],     →  sparseA[0] = [(0, 1)]
//           [-1, 0, 3]]        sparseA[1] = [(0, -1), (2, 3)]
//
//   mat2 = [[7, 0, 0],     →  sparseB[0] = [(0, 7)]
//           [0, 0, 0],         sparseB[1] = []
//           [0, 0, 1]]         sparseB[2] = [(2, 1)]
//
// 计算 result[1]:
//   sparseA[1] = [(0,-1), (2,3)]
//   对 (p=0, valA=-1): 遍历 sparseB[0] = [(j=0, valB=7)]
//                       → result[1][0] += (-1)*7 = -7
//   对 (p=2, valA=3):  遍历 sparseB[2] = [(j=2, valB=1)]
//                       → result[1][2] += 3*1 = 3
//   result[1] = [-7, 0, 3] ✓
// ============================================================
class Solution3 {
public:
    vector<vector<int>> multiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2) {
        int m = mat1.size(), k = mat1[0].size(), n = mat2[0].size();
        vector<vector<int>> result(m, vector<int>(n, 0));
        
        // 预处理 mat1：sparseA[i] = mat1 第 i 行的非零元素 (列号, 值)
        vector<vector<pair<int,int>>> sparseA(m);
        for (int i = 0; i < m; i++) {
            for (int p = 0; p < k; p++) {
                if (mat1[i][p] != 0) {
                    sparseA[i].emplace_back(p, mat1[i][p]);
                }
            }
        }
        
        // 预处理 mat2：sparseB[p] = mat2 第 p 行的非零元素 (列号, 值)
        // 注意：按行存储，因为乘法中 mat1 的列索引 p 对应 mat2 的行索引 p
        vector<vector<pair<int,int>>> sparseB(k);
        for (int p = 0; p < k; p++) {
            for (int j = 0; j < n; j++) {
                if (mat2[p][j] != 0) {
                    sparseB[p].emplace_back(j, mat2[p][j]);
                }
            }
        }
        
        // 核心：只在非零元素之间做乘法
        for (int i = 0; i < m; i++) {
            for (auto& [p, valA] : sparseA[i]) {
                // valA = mat1[i][p]，保证非零
                for (auto& [j, valB] : sparseB[p]) {
                    // valB = mat2[p][j]，保证非零
                    // 两个操作数都非零，这次乘法才有意义
                    result[i][j] += valA * valB;
                }
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   - 唯一区别是循环顺序从 i→j→p 变为 i→p→j
//   - 加了一行 if (mat1[i][p] == 0) continue;
//   - 这一行在稀疏矩阵上能跳过大量无效乘法
//
// 解法2 vs 解法3:
//   - 解法2 仍需遍历 mat1 每行的 k 个元素来找非零项
//   - 解法3 预处理后直接遍历非零元素，连检查 0 的时间都省了
//   - 当矩阵极其稀疏时（如 99% 是零），解法3 优势明显
//   - 代价是额外的预处理空间和代码复杂度
//
// 面试建议：
//   - 先写解法2（简洁有效，一行优化解释清楚）
//   - 被追问时升级到解法3（展示 CSR 格式的理解）
//
// ============================================================
// 【易错点】
//
// 1. 循环顺序调换后用了赋值而非累加：
//    ✗ result[i][j] = mat1[i][p] * mat2[p][j];
//    ✓ result[i][j] += mat1[i][p] * mat2[p][j];
//    原因：同一个 result[i][j] 被不同 p 值多次更新，必须累加。
//
// 2. 预处理 mat2 时搞混了行列：
//    ✗ sparseB[j] 存 mat2 的第 j 列
//    ✓ sparseB[p] 存 mat2 的第 p 行
//    原因：乘法中 mat1[i][p] 对应 mat2[p][*]，p 是行索引。
//
// 3. 维度搞混导致越界：
//    ✗ result 初始化为 (m, vector<int>(k, 0))
//    ✓ result 初始化为 (m, vector<int>(n, 0))
//    原因：mat1 是 m×k, mat2 是 k×n, 结果是 m×n。
//
// ============================================================
// 【面试追问】
//
// Q1 (基础): 为什么调换循环顺序能优化？
// → mat1[i][p] 在 j 循环外层，为 0 时一次 continue 跳过 n 次乘法。
//
// Q2 (深入): 两种跳零（mat1 跳零 vs mat2 跳零）哪个更重要？
// → mat1 跳零更重要，因为一次 skip 省 n 次操作（整个 j 循环）；
//   mat2 跳零只省 1 次操作。如果只选一个优化，选 mat1。
//
// Q3 (进阶): 极大极稀疏矩阵（10000×10000，100 个非零元素）怎么处理？
// → 用解法3 预处理非零元素，甚至不需要开 10000×10000 的数组，
//   用哈希表 map<pair<int,int>, int> 存结果，只存非零结果。
//
// Q4 (变体): 如果 mat1 固定，频繁更换 mat2 做乘法，怎么优化？
// → 预处理 mat1 为稀疏表示一次性完成，每次新 mat2 只需重新预处理
//   mat2 并做非零乘法。mat1 的预处理开销被摊销。
// ============================================================
