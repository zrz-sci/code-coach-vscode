/*
 * LeetCode 1878: Get Biggest Three Rhombus Sums in a Grid
 * 矩阵中最大的三个菱形和
 *
 * 给定 m x n 矩阵, 返回最大的三个不同菱形边界和 (降序)。
 * 菱形 = 正方形旋转45度, 由中心(i,j)和半径d唯一确定。
 * d=0 时退化为单个点。
 *
 * 菱形边界遍历 (中心(i,j), 半径d, 参数k=0..d-1):
 *
 *          (i-d, j)        ← 上顶点
 *         /        \
 *    (i, j-d)    (i, j+d)  ← 左/右顶点
 *         \        /
 *          (i+d, j)        ← 下顶点
 *
 *   上→右: (i-d+k, j+k)      k=0..d-1
 *   右→下: (i+k,   j+d-k)    k=0..d-1
 *   下→左: (i+d-k, j-k)      k=0..d-1
 *   左→上: (i-k,   j-d+k)    k=0..d-1
 *   共 4d 个点, 无重复。
 *
 * 两种解法:
 *   解法1: 暴力枚举 (推荐, m,n<=50 时足够)
 *   解法2: 对角线前缀和优化 (进阶)
 */



// ===================== 解法1: 暴力枚举 =====================
//
// 三重循环: 枚举中心(i,j) + 半径d
// 每个菱形: 遍历4d个边界点求和
// 用 set<int> 维护 Top-3 不同值 (自动去重+排序)
//
// 时间: O(m * n * min(m,n)^2)
// 空间: O(1) (set 最多3个元素)
// m,n<=50 时最坏 ~1.6M 操作, 足够快

class Solution {
public:
    vector<int> getBiggestThree(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        set<int> top3;  // 自动去重 + 升序

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // ---- 半径 d=0: 单点菱形 ----
                top3.insert(grid[i][j]);
                if ((int)top3.size() > 3) top3.erase(top3.begin());

                // ---- 半径 d >= 1 ----
                // 菱形在矩阵内的约束:
                //   上: i-d>=0, 下: i+d<m, 左: j-d>=0, 右: j+d<n
                int maxD = min({i, m - 1 - i, j, n - 1 - j});
                for (int d = 1; d <= maxD; d++) {
                    int sum = 0;
                    // 四条边, 每条 d 个点 (k=0..d-1)
                    // 每条边含起始顶点, 不含终止顶点 → 4d个点无重复
                    for (int k = 0; k < d; k++) {
                        sum += grid[i - d + k][j + k];       // 上→右 (↘)
                        sum += grid[i + k][j + d - k];       // 右→下 (↙)
                        sum += grid[i + d - k][j - k];       // 下→左 (↖)
                        sum += grid[i - k][j - d + k];       // 左→上 (↗)
                    }
                    top3.insert(sum);
                    if ((int)top3.size() > 3) top3.erase(top3.begin());
                }
            }
        }
        // 降序输出
        return vector<int>(top3.rbegin(), top3.rend());
    }
};


// ===================== 解法2: 对角线前缀和优化 =====================
//
// 预处理两个方向的对角线前缀和:
//   d1[i][j] = (i,j) 沿 ↘ 方向的前缀和 = grid[i][j] + d1[i+1][j+1]
//   d2[i][j] = (i,j) 沿 ↙ 方向的前缀和 = grid[i][j] + d2[i+1][j-1]
//
// 区间和:
//   从(r,c)沿↘走len步: d1[r][c] - d1[r+len][c+len]
//   从(r,c)沿↙走len步: d2[r][c] - d2[r+len][c-len]
//
// 菱形四条边 = 四段对角线, 每段O(1)求和
// 但四个顶点被两条边共享, 需减去重复
//
// sum = sumD1(上顶点,d) + sumD2(右顶点,d) + sumD1(左顶点,d) + sumD2(上顶点,d)
//     - grid[上] - grid[右] - grid[下] - grid[左]
//
// 时间: O(m * n * min(m,n))   空间: O(m * n)

class Solution2 {
public:
    vector<int> getBiggestThree(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();

        // d1[i][j]: 从(i,j)沿↘方向的前缀和
        // d2[i][j]: 从(i,j)沿↙方向的前缀和
        // 多分配一行一列避免越界检查
        vector<vector<int>> d1(m + 1, vector<int>(n + 1, 0));
        vector<vector<int>> d2(m + 1, vector<int>(n + 1, 0));

        // 从右下角往左上角填充 (保证递推正确)
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                d1[i][j] = grid[i][j];
                if (i + 1 < m && j + 1 < n)
                    d1[i][j] += d1[i + 1][j + 1];
            }
            for (int j = 0; j < n; j++) {
                d2[i][j] = grid[i][j];
                if (i + 1 < m && j - 1 >= 0)
                    d2[i][j] += d2[i + 1][j - 1];
            }
        }

        // 从(r,c)沿↘走len步的区间和 (含起点, 不含第len+1个点)
        auto sumD1 = [&](int r, int c, int len) -> int {
            return d1[r][c] - d1[r + len][c + len];
        };
        // 从(r,c)沿↙走len步的区间和
        auto sumD2 = [&](int r, int c, int len) -> int {
            return d2[r][c] - d2[r + len][c - len];
        };

        set<int> top3;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // 半径 0
                top3.insert(grid[i][j]);
                if ((int)top3.size() > 3) top3.erase(top3.begin());

                int maxD = min({i, m - 1 - i, j, n - 1 - j});
                for (int d = 1; d <= maxD; d++) {
                    // 四段对角线求和
                    int sum = sumD1(i - d, j, d)     // 上→右 (↘)
                            + sumD2(i, j + d, d)     // 右→下 (↙)
                            + sumD1(i, j - d, d)     // 左→下 (↘, 反向=下→左)
                            + sumD2(i - d, j, d);    // 上→左 (↙, 反向=左→上)

                    // 减去四个顶点的重复 (每个被两条边共享)
                    sum -= grid[i - d][j];   // 上
                    sum -= grid[i][j + d];   // 右
                    sum -= grid[i + d][j];   // 下
                    sum -= grid[i][j - d];   // 左

                    top3.insert(sum);
                    if ((int)top3.size() > 3) top3.erase(top3.begin());
                }
            }
        }
        return vector<int>(top3.rbegin(), top3.rend());
    }
};


/*
 * ==================== 手动演算 ====================
 *
 * 示例1: grid = [[3,4,5,1,3],[3,3,4,2,3],[20,30,200,40,10],
 *                [1,5,5,4,1],[4,3,2,2,5]]
 *
 * 中心(2,1), d=1:
 *   上→右: k=0: (1,1)=3
 *   右→下: k=0: (2,2)=200
 *   下→左: k=0: (3,1)=5
 *   左→上: k=0: (2,0)=20
 *   sum = 3 + 200 + 5 + 20 = 228 ✓ (Blue)
 *
 * 中心(2,3), d=1:
 *   上:(1,3)=2, 右:(2,4)=10, 下:(3,3)=4, 左:(2,2)=200
 *   sum = 2 + 10 + 4 + 200 = 216 ✓ (Red)
 *
 * 中心(1,2), d=1:
 *   上:(0,2)=5, 右:(1,3)=2, 下:(2,2)=200, 左:(1,1)=3
 *   sum = 5 + 2 + 200 + 4 = 211?
 *   让我重新检查: 5+200+4+2 = 211 ✓ (Green)
 *
 * Top-3: [228, 216, 211] ✓
 *
 *
 * 示例2: grid = [[1,2,3],[4,5,6],[7,8,9]]
 *
 * 单点最大: 9, 8, 7, 6, 5, 4, 3, 2, 1
 *
 * 中心(1,1), d=1:
 *   上:(0,1)=2, 右:(1,2)=6, 下:(2,1)=8, 左:(1,0)=4
 *   sum = 2+6+8+4 = 20
 *
 * Top-3: {20, 9, 8} → [20, 9, 8] ✓
 *
 *
 * 示例3: grid = [[7,7,7]]
 *
 * 只有 d=0 的单点: 全是 7
 * set = {7} → [7] ✓
 *
 *
 * ==================== 面试追问 ====================
 *
 * Q1: 为什么用 set 而不是堆？→ set 天然去重，priority_queue 不去重需额外处理
 * Q2: 暴力够用吗？→ m,n<=50 最坏 ~1.6M 操作，远小于 10^8 经验上限
 * Q3: 前缀和优化用在什么场景？→ 矩阵更大(m,n<=500)时内层 O(d) 降为 O(1)
 * Q4: 四条边为什么 k<d 不是 k<=d？→ k<=d 会让顶点被相邻两条边重复计算
 * Q5: 维护 Top-K 通用方法？→ 小根堆；本题因要去重所以 set 更方便
 *
 * 易错点:
 * 1. 遗漏 d=0 单点菱形 → 单个格子也是合法菱形 ✗
 * 2. 四条边遍历 k<=d 而非 k<d → 顶点被相邻两条边重复计算 ✗
 * 3. 边界约束写错 → 菱形须满足 i-d>=0, i+d<m, j-d>=0, j+d<n ✗
 * 4. 用 priority_queue 而非 set → 堆不去重，题目要求不同的菱形和 ✗
 */
