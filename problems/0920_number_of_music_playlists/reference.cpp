/*
 * LeetCode 920: 播放列表的数量 (Number of Music Playlists)
 *
 * 【题目本质】
 * 有 n 首不同的歌，要做长度为 goal 的播放列表，满足：
 * (1) 每首歌至少播一次 (2) 同一首歌重播前必须间隔 k 首其他歌
 * 求不同播放列表的数量 (mod 10^9+7)。
 * 经典计数 DP: dp[i][j] = 列表长 i、有 j 首不同歌的方案数。
 *
 * 【解法总览】
 * 解法1: 二维 DP ⭐          — O(goal*n) / O(goal*n) — 面试首选
 * 解法2: 空间优化 DP         — O(goal*n) / O(n)      — 进阶优化
 * 解法3: 容斥原理            — O(n*log(goal)) / O(n) — 数学方法
 */

// ============================================================
// 解法1: 二维 DP — 状态追踪列表长度和已用歌曲数 ⭐ 面试首选
// 时间: O(goal * n)  空间: O(goal * n)
//
// 【思路】
// 定义 dp[i][j] = 播放列表长度为 i，包含恰好 j 首不同歌的方案数。
//
// 对于第 i 个位置，有两种选择：
//
// 选择1 — 放一首新歌（从 dp[i-1][j-1] 转移）:
//   之前用了 j-1 首不同的歌，现在放第 j 首新歌。
//   可选新歌数 = n - (j-1) = n - j + 1
//   贡献: dp[i-1][j-1] * (n - j + 1)
//
// 选择2 — 放一首旧歌（从 dp[i-1][j] 转移）:
//   之前已经用了 j 首不同的歌，现在重播其中一首。
//   但约束: 最近 k 首不能重播。
//   播放列表最后 k 个位置占了 k 首不同的歌（约束保证），
//   所以可选旧歌数 = max(j - k, 0)
//   贡献: dp[i-1][j] * max(j - k, 0)
//
// 状态转移方程:
//   dp[i][j] = dp[i-1][j-1] * (n-j+1) + dp[i-1][j] * max(j-k, 0)
//
// 边界: dp[0][0] = 1（空列表，0首歌，1种方式）
// 答案: dp[goal][n]（列表长 goal，恰好 n 首不同的歌）
//
// 过程示意 (n=2, goal=3, k=0):
//
//   dp[0][0] = 1
//
//   dp[1][1] = dp[0][0]*(2-1+1) + dp[0][1]*max(1-0,0)
//            = 1*2 + 0*1 = 2
//
//   dp[2][1] = dp[1][0]*(2-1+1) + dp[1][1]*max(1-0,0)
//            = 0*2 + 2*1 = 2
//   dp[2][2] = dp[1][1]*(2-2+1) + dp[1][2]*max(2-0,0)
//            = 2*1 + 0*2 = 2
//
//   dp[3][1] = dp[2][0]*2 + dp[2][1]*1 = 0 + 2 = 2
//   dp[3][2] = dp[2][1]*1 + dp[2][2]*2 = 2 + 4 = 6
//              ↑ 注意: dp[2][2]*max(2-0,0) = 2*2 = 4
//
//   答案: dp[3][2] = 6 ✓
//   (6种: [1,1,2],[1,2,1],[2,1,1],[2,2,1],[2,1,2],[1,2,2])
// ============================================================
class Solution1 {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        const int MOD = 1e9 + 7;

        // dp[i][j]: 播放列表长 i, 包含 j 首不同歌的方案数
        vector<vector<long long>> dp(goal + 1, vector<long long>(n + 1, 0));
        dp[0][0] = 1;  // 空列表, 0首歌, 1种方式

        for (int i = 1; i <= goal; i++) {
            for (int j = 1; j <= min(i, n); j++) {
                // 选择1: 放一首新歌
                // 之前有 j-1 首不同歌，还有 n-(j-1) 首新歌可选
                dp[i][j] = (dp[i - 1][j - 1] * (n - j + 1)) % MOD;

                // 选择2: 放一首旧歌
                // 已有 j 首不同歌，最近 k 首在冷却期不能选
                // 可选旧歌数 = max(j - k, 0)
                dp[i][j] = (dp[i][j] + dp[i - 1][j] * max(j - k, 0)) % MOD;
            }
        }

        // 列表长 goal，恰好用了 n 首不同的歌
        return (int)dp[goal][n];
    }
};

// ============================================================
// 解法2: 空间优化 DP — 滚动数组
// 时间: O(goal * n)  空间: O(n)
//
// 【思路】
// dp[i] 只依赖 dp[i-1]，经典的滚动数组优化。
// 用两个一维数组 prev 和 curr 交替使用。
// ============================================================
class Solution2 {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        const int MOD = 1e9 + 7;

        // 滚动数组
        vector<long long> prev(n + 1, 0), curr(n + 1, 0);
        prev[0] = 1;

        for (int i = 1; i <= goal; i++) {
            // 每轮开始清零 curr
            fill(curr.begin(), curr.end(), 0);

            for (int j = 1; j <= min(i, n); j++) {
                // 放新歌: 从 prev[j-1] 转移
                curr[j] = (prev[j - 1] * (n - j + 1)) % MOD;

                // 放旧歌: 从 prev[j] 转移
                curr[j] = (curr[j] + prev[j] * max(j - k, 0)) % MOD;
            }

            swap(prev, curr);
        }

        return (int)prev[n];
    }
};

// ============================================================
// 解法3: 容斥原理 — 数学公式
// 时间: O(n^2) 或 O(n * log(goal))  空间: O(n)
//
// 【思路】
// 设 F(j) = 用恰好 j 首歌（每首至少1次）填满长度 goal 的列表数。
// 设 G(j) = 用"最多 j 首歌"（无需每首至少1次）填满长度 goal 的列表数。
//
// G(j) 比较容易计算: 列表的每个位置，在满足 k 间隔约束下，
// 从 j 首歌中选择。第1~k个位置必须放不同的歌（排列），
// 第 k+1 个开始每个位置有 j-k 种选择。
// 所以 G(j) = j * (j-1) * ... * (j-k+1) * (j-k)^(goal-k)
//           = P(j, k) * (j-k)^(goal-k)
//
// 然后通过容斥:
// F(n) = sum_{j=0}^{n} (-1)^j * C(n,j) * G(n-j)
//
// 这种方法时间更优但推导复杂，面试中不推荐。
// ============================================================
class Solution3 {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        const long long MOD = 1e9 + 7;

        // 快速幂
        auto power = [&](long long base, long long exp) -> long long {
            long long result = 1;
            base %= MOD;
            while (exp > 0) {
                if (exp & 1) result = result * base % MOD;
                base = base * base % MOD;
                exp >>= 1;
            }
            return result;
        };

        // 预计算阶乘和逆阶乘
        vector<long long> fact(n + 1), inv_fact(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = fact[i - 1] * i % MOD;
        }
        inv_fact[n] = power(fact[n], MOD - 2);
        for (int i = n - 1; i >= 0; i--) {
            inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
        }

        // 容斥计算
        long long result = 0;
        for (int j = 0; j <= n; j++) {
            long long sign = (j % 2 == 0) ? 1 : MOD - 1;

            // C(n, j) = n! / (j! * (n-j)!)
            long long comb = fact[n] % MOD * inv_fact[j] % MOD * inv_fact[n - j] % MOD;

            // G(n-j) = P(n-j, min(k, n-j)) * max(n-j-k, 0)^(goal-min(k,n-j))
            // 简化: 对每个 m = n-j, G(m) = m! / (m-min(k,m))! * max(m-k,0)^(goal-min(k,m))
            int m = n - j;
            if (m < k) continue; // G(m) = 0 when m < k (can't fill k distinct slots)

            // P(m, k) = m * (m-1) * ... * (m-k+1) = m! / (m-k)!
            long long perm = fact[m] % MOD * inv_fact[m - k] % MOD;
            long long rep = power(m - k, goal - k);

            result = (result + sign % MOD * comb % MOD * perm % MOD * rep % MOD) % MOD;
        }

        return (int)(result % MOD);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间            | 空间          | 核心思想                | 适用场景    |
// |-----------------|----------------|---------------|------------------------|-----------|
// | 二维 DP ⭐       | O(goal * n)    | O(goal * n)   | 状态=长度+歌曲数        | 面试首选   |
// | 空间优化 DP      | O(goal * n)    | O(n)          | 滚动数组               | 进阶优化   |
// | 容斥原理         | O(n*log(goal)) | O(n)          | G(j) 函数 + 容斥       | 竞赛/数学  |
//
// 面试首选二维 DP。如果面试官追问空间优化，给出滚动数组版本。
// 容斥方法在竞赛中有优势（n * log(goal) 比 goal * n 快），
// 但推导复杂，面试中不推荐。
//
// ============================================================
// 【易错点】
//
// 1. 旧歌选择数忘记取 max
//    ✗ dp[i][j] += dp[i-1][j] * (j - k)  // j < k 时为负数！
//    ✓ dp[i][j] += dp[i-1][j] * max(j - k, 0)
//
// 2. 取模遗漏导致溢出
//    dp 值可以很大，每步乘法后必须取模。
//    long long 可以容纳两个 10^9 级别数的乘积（10^18 < 9.2*10^18），
//    但连续乘法不取模就会溢出。
//
// 3. 初始化 dp[0][0] = 1 被遗忘
//    所有状态都从 dp[0][0] 递推，如果为 0 则全部为 0。
//
// 4. j 的循环范围
//    ✗ for (int j = 0; j <= n; j++)  // j=0 时 dp[i][0] 恒为 0 (i>0)
//    ✓ for (int j = 1; j <= min(i, n); j++)
//    j 不能超过 i（列表长 i 最多 i 首不同歌），也不能超过 n。
//
// 5. 新歌选择数是 n-j+1 而非 n-j
//    从 dp[i-1][j-1] 转移时，之前有 j-1 首不同歌。
//    新歌数 = n - (j-1) = n - j + 1，不是 n - j。
//
// ============================================================
// 【面试追问】
//
// Q1: 状态转移中两种选择的含义？
// → 新歌: 引入一首从未出现的歌，选择数 = n - (已用歌数)。
//   旧歌: 重播一首已出现的歌，选择数 = 已用歌数 - 冷却期内的歌数。
//
// Q2: k=0 和 k=n-1 的特殊情况？
// → k=0: 无冷却，旧歌选择数 = j（随便选）。列表很灵活。
//   k=n-1: 同一首歌重播前必须把其他所有歌都播一遍。
//   几乎就是排列问题。
//
// Q3: 这个 DP 的子问题数量是多少？状态总数？
// → goal * n 个状态，每个状态 O(1) 转移，总时间 O(goal * n)。
//
// Q4: 能否用记忆化搜索替代 bottom-up？
// → 完全可以。top-down 用 memo[i][j] 缓存 dfs(i, j)。
//   但 bottom-up 更容易做空间优化（滚动数组）。
//
// ============================================================
// 【相关题型】
//
// - 70. Climbing Stairs
//   最基础的线性 DP。本题是二维扩展版，但"每步选择"的框架类似。
//
// - 96. Unique Binary Search Trees
//   计数 DP，答案依赖于子问题的组合。结构不同但 DP 思维框架一致。
//
// - 1269. Number of Ways to Stay in the Same Place After Some Steps
//   类似的二维 DP 计数问题（步数 × 位置），转移方程结构相似。
// ============================================================
