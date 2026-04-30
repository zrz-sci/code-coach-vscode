/*
 * LeetCode 289: Game of Life (生命游戏)
 *
 * 标签: Array, Matrix, Simulation, Bit Manipulation
 * 难度: Medium
 *
 * 三种解法：
 * 1. 额外矩阵（暴力） — O(mn) / O(mn)
 * 2. 原地状态编码（最优） — O(mn) / O(1)
 * 3. 稀疏矩阵（无限棋盘 Follow-up）
 */

// ============================================================
// 解法1: 额外矩阵（暴力） — O(mn) / O(mn)
// 创建副本保存旧状态，根据副本计算新状态写回原矩阵
//
// 思路：
// 1. 深拷贝 board → copy
// 2. 遍历每个位置，从 copy 读邻居旧状态
// 3. 根据 4 条规则计算新状态，写入 board
//
// 优点：逻辑清晰，不容易出错
// 缺点：O(mn) 额外空间
// ============================================================
class Solution_v1 {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();
        // 深拷贝
        vector<vector<int>> copy = board;

        // 8 个方向偏移量
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                // 统计活邻居数（从 copy 读取旧状态）
                int liveNeighbors = 0;
                for (int d = 0; d < 8; d++) {
                    int nr = r + dx[d], nc = c + dy[d];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                        liveNeighbors += copy[nr][nc];
                    }
                }

                // 应用四条规则
                if (copy[r][c] == 1) {
                    // 规则 1: 活细胞，少于 2 个活邻居 → 死（欠人口）
                    // 规则 3: 活细胞，多于 3 个活邻居 → 死（过度拥挤）
                    if (liveNeighbors < 2 || liveNeighbors > 3) {
                        board[r][c] = 0;
                    }
                    // 规则 2: 活细胞，2 或 3 个活邻居 → 活（存活）
                    // 不需要操作，board[r][c] 已经是 1
                } else {
                    // 规则 4: 死细胞，恰好 3 个活邻居 → 活（繁殖）
                    if (liveNeighbors == 3) {
                        board[r][c] = 1;
                    }
                }
            }
        }
    }
};


// ============================================================
// 解法2: 原地状态编码 — O(mn) / O(1)  ⭐ 面试最优解
//
// 核心思想：用 bit 编码同时存储新旧状态
// - bit0 (最低位) = 当前/旧状态
// - bit1 (次低位) = 下一步/新状态
//
// 编码表：
//   当前值  含义             十进制
//   00     旧死 → 新死        0
//   01     旧活 → 新死        1
//   10     旧死 → 新活        2
//   11     旧活 → 新活        3
//
// 关键操作：
// - 读旧状态: board[r][c] & 1  (取 bit0)
// - 写新状态: board[r][c] |= 2 (设 bit1 为 1)
// - 提取新状态: board[r][c] >>= 1 (右移，bit1 → bit0)
//
// 为什么可行？
// 因为我们只在 bit1 上写入，而读取始终看 bit0。
// 两个操作互不干扰，所以可以一轮遍历完成编码。
// ============================================================
class Solution_v2 {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();

        // 用 {-1, 0, 1} 数组生成 8 个方向
        // 比写 8 个方向数组更简洁
        int dirs[] = {-1, 0, 1};

        // ---- 第一轮遍历：编码新状态到 bit1 ----
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                // 统计活邻居数
                // 关键：用 & 1 只看 bit0（旧状态）
                int live = 0;
                for (int dr : dirs) {
                    for (int dc : dirs) {
                        if (dr == 0 && dc == 0) continue; // 跳过自身
                        int nr = r + dr, nc = c + dc;
                        // 边界检查 + 读旧状态
                        if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                            live += (board[nr][nc] & 1);
                        }
                    }
                }

                // 根据规则决定新状态
                // 只有"新状态为活"时才需要操作（设 bit1）
                if (board[r][c] & 1) {
                    // ---- 当前是活细胞 ----
                    // 规则 2: 2 或 3 个活邻居 → 存活
                    if (live == 2 || live == 3) {
                        board[r][c] |= 2; // 设 bit1 = 1，编码变为 11(3)
                    }
                    // 规则 1 & 3: <2 或 >3 → 死亡
                    // 不操作，bit1 保持 0，编码为 01(1)
                } else {
                    // ---- 当前是死细胞 ----
                    // 规则 4: 恰好 3 个活邻居 → 复活
                    if (live == 3) {
                        board[r][c] |= 2; // 设 bit1 = 1，编码变为 10(2)
                    }
                    // 否则保持死，bit1 保持 0，编码为 00(0)
                }
            }
        }

        // ---- 第二轮遍历：提取新状态 ----
        // 右移一位：bit1 → bit0，丢弃旧的 bit0
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                board[r][c] >>= 1;
            }
        }
    }
};


// ============================================================
// 解法3: 稀疏矩阵（无限棋盘 Follow-up）
//
// 适用场景：棋盘无限大，活细胞稀疏分布
//
// 思路：
// - 不用二维数组，用 set<pair<int,int>> 存活细胞坐标
// - 每轮只处理活细胞及其邻居（候选区域）
// - 用 map 统计每个候选位置的活邻居数
// - 根据规则生成下一轮的活细胞集合
//
// 时间: O(L) L=活细胞数
// 空间: O(L)
// ============================================================
class Solution_v3 {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();

        // 收集当前活细胞坐标
        set<pair<int,int>> alive;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (board[r][c] == 1) {
                    alive.insert({r, c});
                }
            }
        }

        // 统计每个候选位置的活邻居数
        // 候选位置 = 所有活细胞及其邻居
        map<pair<int,int>, int> neighborCount;
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (auto& [r, c] : alive) {
            for (int d = 0; d < 8; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                // 对于有限棋盘，仍需边界检查
                // 对于无限棋盘，去掉这个检查即可
                if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                    neighborCount[{nr, nc}]++;
                }
            }
        }

        // 生成下一轮的新状态
        // 先清零棋盘
        for (int r = 0; r < m; r++)
            for (int c = 0; c < n; c++)
                board[r][c] = 0;

        for (auto& [pos, count] : neighborCount) {
            auto [r, c] = pos;
            if (alive.count(pos)) {
                // 活细胞: 2 或 3 个活邻居存活
                if (count == 2 || count == 3) {
                    board[r][c] = 1;
                }
            } else {
                // 死细胞: 恰好 3 个活邻居复活
                if (count == 3) {
                    board[r][c] = 1;
                }
            }
        }

        // 处理活邻居为 0 的活细胞（它们不在 neighborCount 中）
        // 活邻居 = 0 < 2，所以死亡，board 已经清零，无需操作
    }
};


// ============================================================
// 主解法（提交用）— 使用解法2
// ============================================================
class Solution {
public:
    void gameOfLife(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();
        int dirs[] = {-1, 0, 1};

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                int live = 0;
                for (int dr : dirs) {
                    for (int dc : dirs) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                            live += (board[nr][nc] & 1);
                        }
                    }
                }
                if (board[r][c] & 1) {
                    if (live == 2 || live == 3) board[r][c] |= 2;
                } else {
                    if (live == 3) board[r][c] |= 2;
                }
            }
        }
        for (int r = 0; r < m; r++)
            for (int c = 0; c < n; c++)
                board[r][c] >>= 1;
    }
};

// ============================================================
// 总结区块
// ============================================================
/*
 * 【解法对比】
 * | 解法              | 时间    | 空间    | 特点                          |
 * |------------------|---------|---------|-------------------------------|
 * | 额外矩阵(解法1)  | O(mn)   | O(mn)   | 最安全，面试写不出原地时兜底     |
 * | 状态编码(解法2)   | O(mn)   | O(1)    | 面试最优解，bit 操作核心考点     |
 * | 稀疏矩阵(解法3)   | O(L)   | O(L)    | 无限棋盘 follow-up 专用        |
 *
 * 面试首选: 解法2(状态编码)。O(1) 空间是本题最大亮点。
 * Follow-up 必答: 解法3(稀疏矩阵)，面试官几乎必问无限棋盘。
 *
 * 【易错点清单】
 * 1. 同步更新 vs 异步更新: 所有细胞必须同时更新！
 *    → 不能边算边改，否则后面的邻居读到新状态
 * 2. bit 编码读旧状态: 必须用 board[r][c] & 1 取 bit0
 *    → 直接用 board[r][c] 会读到已编码的中间值(2或3)
 * 3. 右移提取新状态: board[r][c] >>= 1 不是 board[r][c] >> 1
 *    → 必须写回！
 * 4. 方向数组跳过自身: {-1,0,1} 嵌套循环时 (0,0) 是自身
 *    → 必须 if (dr==0 && dc==0) continue
 * 5. 规则 2 不需要操作: 活细胞 2-3 个活邻居存活
 *    → bit0 已经是 1，只需设 bit1=1 即 |= 2
 *
 * 【面试追问链】
 * Q1: 如何原地更新？（本题核心）
 *     → 用 bit1 存新状态、bit0 存旧状态，两轮遍历
 * Q2: 如果棋盘是无限大的怎么办？
 *     → 稀疏表示：只存活细胞坐标(set)，每轮只处理活细胞+邻居
 *       用 map 统计候选位置的活邻居数，O(L) 时间空间
 * Q3: 如果要模拟 k 步，有没有加速方法？
 *     → HashLife 算法：对 2^k 步用 memoization 加速
 *       利用棋盘局部重复的特性，时间可降到 O(L * log k)
 */
