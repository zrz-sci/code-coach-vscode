/*
 * LeetCode 799: 香槟塔 (Champagne Tower)
 * 难度: Medium
 * 标签: Dynamic Programming, Simulation
 *
 * 【题目本质】
 * 金字塔形杯塔，从顶部倒入 poured 杯香槟。
 * 每个杯子最多盛 1 杯，溢出均分给左下和右下。
 * 求第 query_row 行第 query_glass 个杯子的液面比例。
 *
 * 核心: 自顶向下模拟溢出，dp[r][c] 记录收到的总量（可超 1）。
 *
 * 【解法总览】
 * 解法一: 二维 DP 模拟      O(R^2) / O(R^2)  逻辑最清晰
 * 解法二: 滚动数组优化       O(R^2) / O(R)    空间最优
 */


/*
 * ============================================================
 * 解法一: 二维 DP 模拟（面试首选）
 * ============================================================
 *
 * 思路:
 *   dp[r][c] = 第 r 行第 c 个杯子收到的液体总量（可能 > 1）
 *   初始: dp[0][0] = poured
 *   转移: 若 dp[r][c] > 1.0, 溢出量 = dp[r][c] - 1.0
 *         一半流向 dp[r+1][c], 一半流向 dp[r+1][c+1]
 *   答案: min(1.0, dp[query_row][query_glass])
 *
 * 举例: poured = 2, query_row = 1, query_glass = 1
 *
 *   第 0 行: dp[0][0] = 2
 *     > 1, overflow = 1
 *     dp[1][0] += 0.5 -> 0.5
 *     dp[1][1] += 0.5 -> 0.5
 *
 *   答案: min(1.0, dp[1][1]) = min(1.0, 0.5) = 0.5
 *
 * 举例: poured = 4, query_row = 2, query_glass = 1
 *
 *   第 0 行: dp[0][0] = 4
 *     overflow = 3, 各 1.5 流下
 *
 *   第 1 行: dp[1][0] = 1.5, dp[1][1] = 1.5
 *     dp[1][0]: overflow = 0.5, 各 0.25 流下
 *     dp[1][1]: overflow = 0.5, 各 0.25 流下
 *
 *   第 2 行: dp[2][0] = 0.25, dp[2][1] = 0.25+0.25 = 0.5, dp[2][2] = 0.25
 *
 *   答案: min(1.0, dp[2][1]) = 0.5
 *
 * 时间: O(R^2), 空间: O(R^2), R = query_row
 */
class Solution1 {
public:
    double champagneTower(int poured, int query_row, int query_glass) {
        // dp[r][c] = 第 r 行第 c 个杯子收到的液体总量
        // 第 r 行有 r+1 个杯子 (c: 0 to r)
        // 分配 query_row+1 行, 每行最多 query_row+2 列以防越界
        vector<vector<double>> dp(query_row + 1,
                                   vector<double>(query_row + 2, 0.0));
        dp[0][0] = poured;

        for (int r = 0; r < query_row; r++) {
            for (int c = 0; c <= r; c++) {
                if (dp[r][c] > 1.0) {
                    double overflow = dp[r][c] - 1.0;
                    dp[r + 1][c]     += overflow / 2.0;  // 左下
                    dp[r + 1][c + 1] += overflow / 2.0;  // 右下
                }
            }
        }

        // 收到量可能 > 1, 实际盛放最多 1
        return min(1.0, dp[query_row][query_glass]);
    }
};


/*
 * ============================================================
 * 解法二: 滚动数组优化
 * ============================================================
 *
 * 思路:
 *   第 r+1 行只依赖第 r 行 -> 用一维数组滚动
 *   每行处理完后用新数组替换旧数组
 *
 * 注意:
 *   - 新行必须初始化为 0 再累加溢出量
 *   - 不能原地修改（会影响同行其他杯子的溢出计算）
 *     实际上同行杯子互不影响（c 处的溢出不影响 c+1 处的溢出），
 *     但溢出的目标位置 c+1 与当前行的 c+1 杯重叠，所以必须分开
 *
 * 具体做法:
 *   cur = [poured]
 *   for r in 0..query_row-1:
 *     next = [0] * (r+2)  // 第 r+1 行有 r+2 个杯子
 *     for c in 0..r:
 *       if cur[c] > 1:
 *         overflow = cur[c] - 1
 *         next[c] += overflow / 2
 *         next[c+1] += overflow / 2
 *     cur = next
 *   return min(1.0, cur[query_glass])
 *
 * 时间: O(R^2), 空间: O(R)
 */
class Solution2 {
public:
    double champagneTower(int poured, int query_row, int query_glass) {
        vector<double> cur = {(double)poured};

        for (int r = 0; r < query_row; r++) {
            // 下一行有 r+2 个杯子
            vector<double> next(r + 2, 0.0);

            for (int c = 0; c <= r; c++) {
                if (cur[c] > 1.0) {
                    double overflow = cur[c] - 1.0;
                    next[c]     += overflow / 2.0;
                    next[c + 1] += overflow / 2.0;
                }
            }

            cur = next;
        }

        return min(1.0, cur[query_glass]);
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法              | 时间    | 空间    | 特点                    |
 * |-------------------|---------|---------|------------------------|
 * | 二维DP (解法一)    | O(R^2)  | O(R^2)  | 逻辑直观，可回溯查看任意杯 |
 * | 滚动数组 (解法二)  | O(R^2)  | O(R)    | 空间最优，但只能查目标行   |
 *
 * 面试推荐: 解法一。代码简洁，逻辑直观。
 * 如果面试官追问空间优化，再给出解法二。
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 混淆"收到量"和"实际盛放量":
 *    错: dp[r+1][c] += min(overflow/2, 1.0)  (过早截断)
 *    对: dp[r+1][c] += overflow / 2.0  (先累加, 最后截断)
 *    原因: 一个杯子可能从左上和右上各收到液体, 必须全部累加后
 *          再判断是否溢出
 *
 * 2. 整数除法:
 *    错: overflow / 2   (C++ 中 int/int 截断小数)
 *    对: overflow / 2.0  (浮点运算)
 *    注: dp 声明为 double 时, overflow 已经是 double, 除以 2 也是浮点
 *        但显式写 2.0 更安全
 *
 * 3. 忘记最终截断:
 *    错: return dp[query_row][query_glass]
 *    对: return min(1.0, dp[query_row][query_glass])
 *    原因: 最后一行的杯子如果收到 > 1 的液体, 实际只盛 1
 *
 * 4. 数组维度错误:
 *    错: dp 每行固定长度 query_row
 *    对: 第 r 行有 r+1 个杯子, 但为了溢出到第 r+1 行,
 *        列的维度至少 query_row+2
 *
 * 5. 滚动数组中忘记初始化:
 *    错: 复用上一行数组 (残留旧数据)
 *    对: 每次创建新的 next 数组并初始化为 0
 *
 * 6. poured 的类型:
 *    错: dp[0][0] = poured (int 赋值给 double, 无精度问题)
 *    对: 实际上 int -> double 无精度损失 (poured <= 10^9 < 2^53)
 *        但建议显式 (double)poured 表明意图
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 为什么不能用数学公式直接算？
 * -> 如果没有容量限制, 流量分布等于 Pascal 三角形乘以 poured/2^r。
 *    但容量限制 1 会阻断部分流量, 改变下游分配, 破坏了数学规律。
 *    必须逐层模拟才能准确计算。
 *
 * Q2: 如果杯子容量不是 1 而是 cap[r][c] 呢？
 * -> 溢出条件改为 dp[r][c] > cap[r][c],
 *    overflow = dp[r][c] - cap[r][c]。其余不变。
 *
 * Q3: 能否提前终止？
 * -> 如果某一行所有杯子的收到量都 <= 1 (无溢出),
 *    后续行全为 0, 可以直接返回 0.0 (如果目标在更深行)。
 *    这对 poured 较小的情况有显著加速。
 *
 * Q4: 如果要查询多个杯子的液面呢？
 * -> 用二维 DP (解法一), 模拟到最深的 query_row 后,
 *    可以直接查任意 (r, c) 的值。滚动数组只能查最后一行。
 * ============================================================
 */
