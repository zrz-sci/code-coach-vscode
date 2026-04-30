/*
 * LeetCode 887: 鸡蛋掉落 (Super Egg Drop)
 *
 * 【题目本质】
 * k 个蛋、n 层楼，求最坏情况下确定临界楼层 f 的最少操作次数。
 * 经典 DP 换视角题：从"最少操作数"转为"最多能确定楼层数"。
 *
 * 【解法总览】
 * 解法1: 朴素 DP + 二分      — O(kn logn) / O(kn) — 直观但非最优
 * 解法2: 逆向 DP (二维)      — O(kn) / O(kn)     — 换视角递推 ⭐面试首选
 * 解法3: 逆向 DP (空间优化)  — O(kn) / O(k)      — 滚动数组
 * 解法4: 数学解 (组合数)     — O(k logn) / O(1)  — 理论最优
 */



// ============================================================
// 解法1: 朴素 DP + 二分优化
// 时间: O(kn logn)  空间: O(kn)
//
// 【思路】
// 直接定义 f(k,n) = k个蛋n层楼的最少操作数。
// 在第x层扔蛋:
//   碎了: f(k-1, x-1)  (下面x-1层, k-1个蛋)
//   没碎: f(k, n-x)    (上面n-x层, k个蛋)
//   最坏: max(碎, 没碎)
//   最优: min over x { 1 + max(f(k-1,x-1), f(k,n-x)) }
//
// 关键观察: 固定k和n, 当x增大时:
//   f(k-1, x-1) 单调递增
//   f(k, n-x) 单调递减
//   两曲线交叉处取 max 最小 → 可用二分搜索
//
// 逆向 DP 表 (朴素定义):
//
//   f(k,n):     n=0  n=1  n=2  n=3  n=4  n=5  n=6
//   k=1          0    1    2    3    4    5    6   ← 只能逐层试
//   k=2          0    1    2    2    3    3    3   ← 可以跳跃
//   k=3          0    1    2    2    3    3    3
//
// ============================================================
class Solution1 {
public:
    int superEggDrop(int k, int n) {
        // 记忆化搜索
        vector<vector<int>> memo(k + 1, vector<int>(n + 1, -1));
        return dp(k, n, memo);
    }

private:
    int dp(int eggs, int floors, vector<vector<int>>& memo) {
        // base cases
        if (floors <= 0) return 0;       // 0层楼不需要操作
        if (eggs == 1) return floors;     // 1个蛋只能从底层逐层试
        if (memo[eggs][floors] != -1) return memo[eggs][floors];

        int lo = 1, hi = floors;
        int ans = floors;  // 最坏情况不超过 floors

        // 二分搜索最优的扔蛋层 x
        // f(k-1, x-1) 关于 x 单调递增
        // f(k, n-x) 关于 x 单调递减
        // 找它们交叉处
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int breakCase = dp(eggs - 1, mid - 1, memo);    // 碎了
            int noBreakCase = dp(eggs, floors - mid, memo);  // 没碎

            if (breakCase < noBreakCase) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
            // 取两者 max 的最小值
            ans = min(ans, 1 + max(breakCase, noBreakCase));
        }

        return memo[eggs][floors] = ans;
    }
};

// ============================================================
// 解法2: 逆向 DP (二维) — 换视角: "t次k蛋最多确定多少层"
// 时间: O(t * k)  空间: O(t * k)  (t 最坏 O(n), 但通常远小于 n)
//
// 【视角转换——本题精髓】
// 不问 "k蛋n层需要几次?"
// 而问 "t次k蛋最多能确定几层?"
//
// 定义: dp[t][k] = t次操作、k个蛋，最多能确定的楼层数
//
// 递推: 在某层扔蛋——
//   碎了 → 往下找，剩 t-1次, k-1蛋 → 下面能确定 dp[t-1][k-1] 层
//   没碎 → 往上找，剩 t-1次, k蛋   → 上面能确定 dp[t-1][k] 层
//   加上当前扔的这一层:
//     dp[t][k] = dp[t-1][k-1] + dp[t-1][k] + 1
//
// 逆向 DP 表:
//
//   dp[t][k]:    k=1   k=2   k=3   k=4
//   t=0           0     0     0     0
//   t=1           1     1     1     1
//   t=2           2     3     3     3
//   t=3           3     6     7     7
//   t=4           4    10    14    15
//   t=5           5    15    25    30
//
//   dp[3][2] = dp[2][1] + dp[2][2] + 1 = 2 + 3 + 1 = 6
//   → 3次操作2蛋最多确定6层 → k=2,n=6 答案为3 ✓
//
//   dp[4][3] = dp[3][2] + dp[3][3] + 1 = 6 + 7 + 1 = 14
//   → 4次操作3蛋最多确定14层 → k=3,n=14 答案为4 ✓
//
// 答案: 找最小的 t 使得 dp[t][k] >= n
//
// 【填表方向】
//
//        k-1    k
//   t-1  [A]   [B]     dp[t][k] = A + B + 1
//   t          [C]     第 t 行只依赖第 t-1 行 → 逐行填写
//
// ============================================================
class Solution2 {
public:
    int superEggDrop(int k, int n) {
        // dp[t][j] = t次操作j个蛋能确定的最多楼层数
        // t 最大不超过 n (1个蛋逐层试)
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));

        int t = 0;
        while (dp[t][k] < n) {
            t++;
            for (int j = 1; j <= k; j++) {
                dp[t][j] = dp[t - 1][j - 1]  // 碎了: 下面能确定的层数
                         + dp[t - 1][j]        // 没碎: 上面能确定的层数
                         + 1;                   // 当前扔的这一层
            }
        }
        return t;
    }
};

// ============================================================
// 解法3: 逆向 DP 空间优化 — 滚动数组
// 时间: O(t * k)  空间: O(k)
//
// 【思路】
// dp[t] 只依赖 dp[t-1]，所以可以只保留一维数组。
// 关键: dp[j] = dp_old[j-1] + dp_old[j] + 1
//       更新 dp[j] 时需要 dp[j-1] 的旧值 (t-1 时的)
//       如果从左到右更新，dp[j-1] 已经是新值了!
//       → 必须从右到左更新，保证 dp[j-1] 还是旧值
//
// 更新顺序示意:
//
//   从右往左: dp[k], dp[k-1], ..., dp[1]
//   更新 dp[j] 时:
//     dp[j-1] 还没被更新 → 保存的是 t-1 时的旧值 ✓
//     dp[j] 自己保存的也是 t-1 时的旧值 ✓
//     所以 dp[j] = dp[j-1](旧) + dp[j](旧) + 1  完全正确
//
// ============================================================
class Solution3 {
public:
    int superEggDrop(int k, int n) {
        // dp[j] = 当前 t 次操作、j 个蛋能确定的最多楼层
        vector<int> dp(k + 1, 0);

        int t = 0;
        while (dp[k] < n) {
            t++;
            // 从右往左更新，防止覆盖需要的旧值
            for (int j = k; j >= 1; j--) {
                dp[j] = dp[j - 1]  // dp_old[j-1]: 碎了的分支 (旧值)
                       + dp[j]      // dp_old[j]:   没碎的分支 (旧值)
                       + 1;         // 当前扔的这一层
            }
        }
        return t;
    }
};

// ============================================================
// 解法4: 数学解 — 组合数求和 + 二分搜索
// 时间: O(k * log n)  空间: O(1)
//
// 【数学观察】
// dp[t][k] 实际上等于组合数之和:
//   dp[t][k] = C(t,1) + C(t,2) + ... + C(t,k)
//
// 证明 (归纳法):
//   dp[t][k] = dp[t-1][k-1] + dp[t-1][k] + 1
//   由 Pascal 恒等式: C(t-1,i-1) + C(t-1,i) = C(t,i)
//   所以 dp[t-1][k-1] + dp[t-1][k]
//     = sum_{i=1}^{k-1} C(t-1,i) + sum_{i=1}^{k} C(t-1,i)
//     = ... (通过 Pascal 恒等式化简)
//     = sum_{i=1}^{k} C(t,i) - 1
//   加上 +1 就得 dp[t][k] = sum_{i=1}^{k} C(t,i)
//
// 由于 sum_{i=1}^{k} C(t,i) 关于 t 单调递增，
// 可以二分搜索找最小的 t 使之 >= n。
//
// ============================================================
class Solution4 {
public:
    int superEggDrop(int k, int n) {
        // 二分搜索最小的 t 使得 f(t,k) >= n
        int lo = 1, hi = n;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (computeFloors(mid, k) >= n) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    }

private:
    // 计算 C(t,1) + C(t,2) + ... + C(t,k)
    // 用递推计算组合数避免大数: C(t,i) = C(t,i-1) * (t-i+1) / i
    long long computeFloors(int t, int k) {
        long long sum = 0;
        long long comb = 1;  // C(t, i), 从 C(t,0)=1 开始递推
        for (int i = 1; i <= k && i <= t; i++) {
            comb = comb * (t - i + 1) / i;
            sum += comb;
            if (sum >= 1e9) return sum;  // 已经超过可能的 n，提前返回防溢出
        }
        return sum;
    }
};

// ============================================================
// 测试代码
// ============================================================
int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;
    Solution4 sol4;

    struct TestCase {
        int k, n, expected;
    };

    vector<TestCase> tests = {
        {1, 2, 2},    // 1蛋2层: 逐层试, 需要2次
        {2, 6, 3},    // 2蛋6层: dp[3][2]=6>=6
        {3, 14, 4},   // 3蛋14层: dp[4][3]=14>=14
        {1, 1, 1},    // 1蛋1层: 试1次
        {2, 1, 1},    // 2蛋1层: 试1次
        {2, 100, 14}, // 2蛋100层
        {4, 10000, 14}, // 大规模测试
    };

    cout << "=== 解法1: 朴素DP+二分 ===" << endl;
    for (auto& t : tests) {
        int result = sol1.superEggDrop(t.k, t.n);
        cout << "k=" << t.k << " n=" << t.n
             << " expected=" << t.expected << " got=" << result
             << (result == t.expected ? " PASS" : " FAIL") << endl;
    }

    cout << "\n=== 解法2: 逆向DP(二维) ===" << endl;
    for (auto& t : tests) {
        int result = sol2.superEggDrop(t.k, t.n);
        cout << "k=" << t.k << " n=" << t.n
             << " got=" << result
             << (result == t.expected ? " PASS" : " FAIL") << endl;
    }

    cout << "\n=== 解法3: 逆向DP(空间优化) ===" << endl;
    for (auto& t : tests) {
        int result = sol3.superEggDrop(t.k, t.n);
        cout << "k=" << t.k << " n=" << t.n
             << " got=" << result
             << (result == t.expected ? " PASS" : " FAIL") << endl;
    }

    cout << "\n=== 解法4: 数学解(组合数) ===" << endl;
    for (auto& t : tests) {
        int result = sol4.superEggDrop(t.k, t.n);
        cout << "k=" << t.k << " n=" << t.n
             << " got=" << result
             << (result == t.expected ? " PASS" : " FAIL") << endl;
    }

    return 0;
}

/*
 * ============================================================
 * 总结
 * ============================================================
 *
 * 【解法对比】
 * ┌──────────────────────┬───────────────┬────────┬─────────────────────────┐
 * │ 解法                 │ 时间          │ 空间   │ 适用场景                │
 * ├──────────────────────┼───────────────┼────────┼─────────────────────────┤
 * │ 1. 朴素DP+二分       │ O(kn logn)    │ O(kn)  │ 直觉理解，面试可讲思路  │
 * │ 2. 逆向DP(二维) ⭐   │ O(t*k)        │ O(t*k) │ 面试首选，思路清晰      │
 * │ 3. 逆向DP(空间优化)  │ O(t*k)        │ O(k)   │ follow-up 空间优化      │
 * │ 4. 数学解(组合数)    │ O(k logn)     │ O(1)   │ 理论最优，竞赛/加分项   │
 * └──────────────────────┴───────────────┴────────┴─────────────────────────┘
 *   (t = 最终答案，通常远小于 n)
 *
 * 【易错点】
 * 1. 解法1: 二分搜索找交叉点时，breakCase == noBreakCase 的边界要取 min
 *    而非只走一侧，否则漏解。
 * 2. 解法2: while 循环条件是 dp[t][k] < n，不是 <=，否则多算一次。
 * 3. 解法3: 滚动数组必须从右往左更新 (j=k→1)，从左往右会覆盖旧值导致
 *    dp[j-1] 已经变成新值，递推公式全错。
 * 4. 解法4: 组合数递推 comb = comb * (t-i+1) / i 必须先乘后除，且要
 *    提前返回防 long long 溢出。
 * 5. 视角转换是本题精髓: 把"k蛋n层最少几次"变成"t次k蛋最多几层"，
 *    很多人卡在朴素DP的 O(kn^2) 上不知道怎么优化。
 *
 * 【面试追问】
 * Q1: 朴素DP为什么可以用二分优化? 请画出 breakCase 和 noBreakCase 关于 x 的曲线。
 *     → breakCase=f(k-1,x-1) 单调递增，noBreakCase=f(k,n-x) 单调递减，
 *       max(两者) 在交叉处取最小值，经典"凸函数找极值"用二分。
 *
 * Q2: 逆向DP的递推 dp[t][k] = dp[t-1][k-1] + dp[t-1][k] + 1 和 Pascal
 *     三角形有什么关系? 能否用组合数直接计算?
 *     → dp[t][k] = C(t,1)+C(t,2)+...+C(t,k)，由 Pascal 恒等式
 *       C(t-1,i-1)+C(t-1,i)=C(t,i) 可证。对 t 单调递增，可二分求解。
 *
 * Q3: 如果蛋有不同强度 (蛋A在x层碎，蛋B在y层碎，x!=y)，还能用这个框架吗?
 *     → 不能。本题假设所有蛋强度相同 (同一临界楼层 f)，才有对称递推。
 *       若蛋强度不同，需要建模为信息论中的自适应搜索问题，复杂度大增。
 */
