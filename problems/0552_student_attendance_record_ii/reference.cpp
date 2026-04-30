/*
 * LeetCode 552: Student Attendance Record II (学生出勤记录 II)
 * 难度: Hard
 * 标签: Dynamic Programming
 *
 * =============================================
 * 解法一: DP 滚动数组 (推荐)
 * =============================================
 *
 * 核心思路:
 *   将两个合法约束 (A<2, 连续L<3) 编码为 DP 状态:
 *   dp[a][l] = 已用 a 个 A, 末尾连续 l 个 L 的方案数
 *   a ∈ {0, 1}, l ∈ {0, 1, 2} → 共 6 个状态
 *
 *   每天的转移:
 *     放 P: dp[a][l] → ndp[a][0]     (连续 L 重置)
 *     放 L: dp[a][l] → ndp[a][l+1]   (需 l+1 < 3)
 *     放 A: dp[0][l] → ndp[1][0]     (需 a == 0)
 *
 * 时间: O(n) — 每天转移 6 个状态
 * 空间: O(1) — 两组 dp[2][3]
 */

class Solution {
public:
    int checkRecord(int n) {
        const int MOD = 1e9 + 7;
        // dp[a][l]: 当前已用 a 个 A, 末尾连续 l 个 L 的方案数
        long dp[2][3] = {};
        dp[0][0] = 1;  // 初始: 空串, 0 个 A, 0 个连续 L

        for (int day = 0; day < n; day++) {
            long ndp[2][3] = {};

            for (int a = 0; a < 2; a++) {
                for (int l = 0; l < 3; l++) {
                    if (dp[a][l] == 0) continue;

                    // 放 P: 连续 L 重置为 0, A 不变
                    ndp[a][0] = (ndp[a][0] + dp[a][l]) % MOD;

                    // 放 L: 连续 L +1, 但不能达到 3
                    if (l + 1 < 3) {
                        ndp[a][l + 1] = (ndp[a][l + 1] + dp[a][l]) % MOD;
                    }

                    // 放 A: A 次数 +1 (必须 a==0), 连续 L 重置
                    if (a + 1 < 2) {
                        ndp[a + 1][0] = (ndp[a + 1][0] + dp[a][l]) % MOD;
                    }
                }
            }
            memcpy(dp, ndp, sizeof(dp));
        }

        // 汇总所有合法状态
        long ans = 0;
        for (int a = 0; a < 2; a++)
            for (int l = 0; l < 3; l++)
                ans = (ans + dp[a][l]) % MOD;

        return (int)ans;
    }
};

/*
 * trace: n = 2
 *
 * 初始: dp[0][0] = 1
 *
 * Day 1:
 *   从 (0,0)=1:
 *     放 P → ndp[0][0] += 1 = 1   → "P"
 *     放 L → ndp[0][1] += 1 = 1   → "L"
 *     放 A → ndp[1][0] += 1 = 1   → "A"
 *   dp = { {1,1,0}, {1,0,0} }
 *
 * Day 2:
 *   从 (0,0)=1 ("P"):
 *     P → ndp[0][0]+=1=1, L → ndp[0][1]+=1=1, A → ndp[1][0]+=1=1
 *   从 (0,1)=1 ("L"):
 *     P → ndp[0][0]+=1=2, L → ndp[0][2]+=1=1, A → ndp[1][0]+=1=2
 *   从 (1,0)=1 ("A"):
 *     P → ndp[1][0]+=1=3, L → ndp[1][1]+=1=1, A → 不行(a=1)
 *   dp = { {2,1,1}, {3,1,0} }
 *
 * 答案 = 2+1+1+3+1+0 = 8 ✓
 */


/*
 * =============================================
 * 解法二: 6 变量展开 (更高效)
 * =============================================
 *
 * 直接用 6 个变量, 避免数组和循环开销
 * 每天的转移展开为 6 条赋值语句
 * 优点: 常数因子更小, 实际更快
 * 缺点: 代码较长, 扩展性差
 */

class Solution2 {
public:
    int checkRecord(int n) {
        const int MOD = 1e9 + 7;
        // 6 个状态: (a次A, l个连续L)
        long a0l0 = 1, a0l1 = 0, a0l2 = 0;
        long a1l0 = 0, a1l1 = 0, a1l2 = 0;

        for (int day = 0; day < n; day++) {
            long n_a0l0 = (a0l0 + a0l1 + a0l2) % MOD;  // P after any (0,*)
            long n_a0l1 = a0l0;                           // L after (0,0)
            long n_a0l2 = a0l1;                           // L after (0,1)
            long n_a1l0 = (a0l0 + a0l1 + a0l2            // A after any (0,*)
                         + a1l0 + a1l1 + a1l2) % MOD;     // + P after any (1,*)
            long n_a1l1 = a1l0;                           // L after (1,0)
            long n_a1l2 = a1l1;                           // L after (1,1)

            a0l0 = n_a0l0; a0l1 = n_a0l1; a0l2 = n_a0l2;
            a1l0 = n_a1l0; a1l1 = n_a1l1; a1l2 = n_a1l2;
        }

        return (int)((a0l0 + a0l1 + a0l2 + a1l0 + a1l1 + a1l2) % MOD);
    }
};


/*
 * =============================================
 * 解法三: 矩阵快速幂 (进阶, n 极大时使用)
 * =============================================
 *
 * 将 6 个状态的线性转移表示为 6x6 矩阵 M
 * dp_n = M^n * dp_0
 * 用矩阵快速幂在 O(6^3 * log n) = O(216 * log n) 时间内求解
 *
 * 状态顺序: (0,0), (0,1), (0,2), (1,0), (1,1), (1,2)
 *
 * 转移矩阵 M:
 *   (0,0) ← P from (0,0),(0,1),(0,2)          → row 0: [1,1,1,0,0,0]
 *   (0,1) ← L from (0,0)                       → row 1: [1,0,0,0,0,0]
 *   (0,2) ← L from (0,1)                       → row 2: [0,1,0,0,0,0]
 *   (1,0) ← A from (0,*) + P from (1,*)        → row 3: [1,1,1,1,1,1]
 *   (1,1) ← L from (1,0)                       → row 4: [0,0,0,1,0,0]
 *   (1,2) ← L from (1,1)                       → row 5: [0,0,0,0,1,0]
 *
 * 面试中 DP 滚动数组即可, 矩阵快速幂是加分项
 */

class Solution3 {
public:
    typedef vector<vector<long>> Mat;
    int MOD = 1e9 + 7;

    Mat multiply(const Mat& A, const Mat& B) {
        int sz = A.size();
        Mat C(sz, vector<long>(sz, 0));
        for (int i = 0; i < sz; i++)
            for (int k = 0; k < sz; k++)
                if (A[i][k])
                    for (int j = 0; j < sz; j++)
                        C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
        return C;
    }

    Mat matpow(Mat M, int p) {
        int sz = M.size();
        Mat result(sz, vector<long>(sz, 0));
        for (int i = 0; i < sz; i++) result[i][i] = 1;
        while (p > 0) {
            if (p & 1) result = multiply(result, M);
            M = multiply(M, M);
            p >>= 1;
        }
        return result;
    }

    int checkRecord(int n) {
        Mat M = {
            {1, 1, 1, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1},
            {0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 0},
        };
        Mat result = matpow(M, n);
        long ans = 0;
        for (int i = 0; i < 6; i++)
            ans = (ans + result[i][0]) % MOD;
        return (int)ans;
    }
};


/*
 * =============================================
 * 解法对比
 * =============================================
 *
 * | 解法           | 时间      | 空间 | 特点             |
 * |---------------|-----------|------|-----------------|
 * | DP 滚动数组    | O(n)      | O(1) | 推荐, 清晰易懂   |
 * | 6 变量展开     | O(n)      | O(1) | 更快, 无数组开销  |
 * | 矩阵快速幂     | O(log n)  | O(1) | n 极大时, 加分项  |
 *
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * 1. 忘记取模: 中间值 long 也可能很大, 每步都要 % MOD
 *    ✗ ndp[a][0] += dp[a][l]
 *    ✓ ndp[a][0] = (ndp[a][0] + dp[a][l]) % MOD
 *
 * 2. 放 A 时忘记重置连续 L:
 *    ✗ ndp[a+1][l] += dp[a][l]     L 应该重置
 *    ✓ ndp[a+1][0] += dp[a][l]     连续 L 变为 0
 *
 * 3. 用 int 而非 long: 两个接近 10^9 的数相加溢出
 *    ✗ int dp[2][3]
 *    ✓ long dp[2][3]
 *
 * 4. 初始状态错误: dp[0][0] 应为 1 (空串), 不是全 0
 *    ✗ 所有 dp 初始化为 0
 *    ✓ dp[0][0] = 1
 *
 * 5. 6变量展开时赋值顺序错误: 必须全部算完再赋值
 *    ✗ 边算边更新 a0l0, 导致后续计算用了新值
 *    ✓ 先算到临时变量 n_xxx, 再统一赋值
 *
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 为什么空间可以 O(1)?
 *     → 第 i 天的状态只依赖第 i-1 天的 6 个值
 *     → 用两组 dp[2][3] 交替, 或直接 6 个变量
 *
 * Q2: 矩阵快速幂怎么做?
 *     → 6 个状态间转移是线性的, 写成 6x6 矩阵 M
 *     → dp_n = M^n * dp_0, 用快速幂 O(6^3 log n) = O(log n)
 *     → 当 n 极大 (如 10^18) 时是唯一可行方案
 *
 * Q3: 如果限制改为 "A 最多 k 次, 连续 L 最多 m 个"?
 *     → 状态变为 dp[k+1][m+1], 共 (k+1)*(m+1) 个状态
 *     → 转移逻辑不变, 时间 O(n * k * m)
 *
 * 关联题目:
 *   551. Student Attendance Record I — 本题简化版 (判断)
 *   935. Knight Dialer — 类似状态机 DP
 *   790. Domino and Tromino Tiling — 类似状态转移
 *   1220. Count Vowels Permutation — 类似状态转移计数
 *   70.  Climbing Stairs — 基础线性 DP
 */
