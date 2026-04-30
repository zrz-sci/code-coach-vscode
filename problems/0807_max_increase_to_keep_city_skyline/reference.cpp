/*
 * LeetCode 807: 保持城市天际线 (Max Increase to Keep City Skyline)
 *
 * 【题目本质】
 * 每个建筑 grid[r][c] 能增高到的最大值 = min(rowMax[r], colMax[c])，
 * 因为它同时受行天际线和列天际线两个方向的约束。
 *
 * 【解法总览】
 * 解法1: 贪心 + 行列最大值预计算 — O(n^2) / O(n) — 唯一正解
 *
 * 关键洞察：天际线 = 每行/每列的最大值投影。
 * 每个建筑独立受两个方向约束，取 min 保证不突破任一天际线。
 */

// ============================================================
// 解法1: 贪心 + 行列最大值 — 预计算 rowMax 和 colMax
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 1. 天际线定义:
//    - 从东/西方向看: 天际线 = 每行的最大值 rowMax[r]
//    - 从南/北方向看: 天际线 = 每列的最大值 colMax[c]
//
// 2. 约束分析:
//    建筑 grid[r][c] 增高后的高度 h 必须满足:
//      h <= rowMax[r]  (否则行天际线改变)
//      h <= colMax[c]  (否则列天际线改变)
//    => h <= min(rowMax[r], colMax[c])
//
// 3. 为什么取 min 而不是 max?
//    取 max 的话，超出 min 的那个方向的天际线就被突破了。
//    例如: rowMax=9, colMax=7, 增到 9 则列天际线从 7 变成 9 ✗
//
// 4. 正确性保证:
//    差值 min(rowMax[r], colMax[c]) - grid[r][c] >= 0
//    因为 grid[r][c] <= rowMax[r] 且 grid[r][c] <= colMax[c]
//
// 模拟过程:
// grid = [[3,0,8,4],     rowMax = [8, 7, 9, 3]
//         [2,4,5,7],     colMax = [9, 4, 8, 7]
//         [9,2,6,3],
//         [0,3,1,0]]
//
// 新矩阵 = [[min(8,9), min(8,4), min(8,8), min(8,7)],   = [[8,4,8,7],
//           [min(7,9), min(7,4), min(7,8), min(7,7)],      [7,4,7,7],
//           [min(9,9), min(9,4), min(9,8), min(9,7)],      [9,4,8,7],
//           [min(3,9), min(3,4), min(3,8), min(3,7)]]      [3,3,3,3]]
//
// 增量 = (8-3)+(4-0)+(8-8)+(7-4)
//       +(7-2)+(4-4)+(7-5)+(7-7)
//       +(9-9)+(4-2)+(8-6)+(7-3)
//       +(3-0)+(3-3)+(3-1)+(3-0) = 35
// ============================================================
class Solution1 {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<int> rowMax(n, 0), colMax(n, 0);

        // 第一遍: 预计算每行最大值和每列最大值
        // rowMax[r] = 东/西方向天际线在第 r 行的高度
        // colMax[c] = 南/北方向天际线在第 c 列的高度
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                rowMax[r] = max(rowMax[r], grid[r][c]);
                colMax[c] = max(colMax[c], grid[r][c]);
            }
        }

        // 第二遍: 计算每个建筑的最大增量并累加
        int totalIncrease = 0;
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                // 这个建筑最多能增到两个天际线中较矮的那个
                int limit = min(rowMax[r], colMax[c]);
                totalIncrease += limit - grid[r][c];
            }
        }

        return totalIncrease;
    }
};

// ============================================================
// 解法1 变体: 一遍遍历 (先求 max，再在同一循环中计算)
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 思路完全一样，只是代码组织不同。
// 第一遍必须完整遍历才能得到所有 rowMax/colMax，
// 所以无法真正合并为"一遍"，但可以用 STL 简化代码。
// ============================================================
class Solution1_Variant {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        int n = grid.size();

        // 用 *max_element 简洁地求每行最大值
        vector<int> rowMax(n);
        for (int r = 0; r < n; r++) {
            rowMax[r] = *max_element(grid[r].begin(), grid[r].end());
        }

        // 求每列最大值
        vector<int> colMax(n, 0);
        for (int c = 0; c < n; c++) {
            for (int r = 0; r < n; r++) {
                colMax[c] = max(colMax[c], grid[r][c]);
            }
        }

        // 累加增量
        int result = 0;
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                result += min(rowMax[r], colMax[c]) - grid[r][c];
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
// 本题只有一种核心思路。两个变体只是代码风格不同:
// - Solution1: 手动在双重循环中同时求 rowMax 和 colMax
// - Solution1_Variant: 用 *max_element 分开求行列最大值
//
// 【易错点】
// 1. 只考虑一个方向的约束:
//    ✗ totalIncrease += rowMax[r] - grid[r][c]
//    ✓ totalIncrease += min(rowMax[r], colMax[c]) - grid[r][c]
//
// 2. 忘记预计算，在内层循环重复求最大值:
//    ✗ for 每个格子: 遍历整行整列求 max → O(n^3)
//    ✓ 预计算 rowMax/colMax → O(n^2)
//
// 3. 混淆 min 和 max:
//    ✗ max(rowMax[r], colMax[c]) → 会突破较小方向的天际线
//    ✓ min(rowMax[r], colMax[c]) → 保证两个方向都不被突破
//
// 4. colMax 初始化遗漏:
//    ✗ 忘了初始化 colMax 为 0
//    ✓ vector<int> colMax(n, 0) 或在循环前置零
//
// 【面试追问】
// Q1(核心): 为什么取 min？(多约束取交集，保证两个方向都不越界)
// Q2(变体): 如果允许降低建筑高度，答案怎么变？
//           (原始最大值所在建筑不能降，否则天际线改变)
// Q3(扩展): 非方阵 m×n 需要改动吗？
//           (不需要，rowMax 大小 m，colMax 大小 n，逻辑不变)
// ============================================================
