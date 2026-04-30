// ============================================================
// 634. Find the Derangement of An Array
// ============================================================
// 错位排列 (Derangement): n 个元素的排列中，没有任何元素在原位的方案数
// 递推公式: D(n) = (n-1) * (D(n-1) + D(n-2))
// 基态: D(0) = 1, D(1) = 0, D(2) = 1
// 约束: 1 <= n <= 10^6, 结果对 10^9+7 取模
// ============================================================

// ============================================================
// 解法一：DP 递推 + 空间优化（推荐）
// ============================================================
// 思路：
//   只需维护前两个状态 prev2=D(i-2) 和 prev1=D(i-1)
//   逐步滚动计算到 D(n)
//
// 递推公式推导（组合意义）：
//   元素 n 不能放在位置 n，有 (n-1) 种选择，设放在位置 k
//   - 元素 k 放在位置 n（交换）-> 剩余 n-2 个元素错位排列 -> D(n-2)
//   - 元素 k 不放在位置 n -> 等价于 n-1 个元素的错位排列 -> D(n-1)
//   故 D(n) = (n-1) * (D(n-1) + D(n-2))
//
// 时间: O(n)  空间: O(1)
class Solution {
public:
    int findDerangement(int n) {
        if (n == 1) return 0;
        if (n == 2) return 1;

        const int MOD = 1e9 + 7;
        long long prev2 = 0; // D(1) = 0
        long long prev1 = 1; // D(2) = 1

        for (int i = 3; i <= n; ++i) {
            long long curr = (long long)(i - 1) * ((prev1 + prev2) % MOD) % MOD;
            prev2 = prev1;
            prev1 = curr;
        }
        return (int)prev1;
    }
};


// ============================================================
// 解法二：DP 数组
// ============================================================
// 思路：
//   用数组 dp[i] 存储 D(i)，直接递推
//   优点是代码直观，方便调试和打印中间状态
//
// 时间: O(n)  空间: O(n)
class Solution2 {
public:
    int findDerangement(int n) {
        if (n == 1) return 0;
        if (n == 2) return 1;

        const int MOD = 1e9 + 7;
        vector<long long> dp(n + 1);
        dp[1] = 0;
        dp[2] = 1;

        for (int i = 3; i <= n; ++i) {
            dp[i] = (long long)(i - 1) * ((dp[i - 1] + dp[i - 2]) % MOD) % MOD;
        }
        return (int)dp[n];
    }
};


// ============================================================
// 解法三：另一种递推 D(n) = n * D(n-1) + (-1)^n
// ============================================================
// 思路：
//   利用容斥原理可以导出另一个等价递推：
//   D(n) = n * D(n-1) + (-1)^n
//
//   验证：
//   D(1) = 1 * D(0) + (-1)^1 = 1 * 1 - 1 = 0
//   D(2) = 2 * D(1) + (-1)^2 = 2 * 0 + 1 = 1
//   D(3) = 3 * D(2) + (-1)^3 = 3 * 1 - 1 = 2
//   D(4) = 4 * D(3) + (-1)^4 = 4 * 2 + 1 = 9
//
//   这个公式只需要一个前驱状态，代码更简洁
//   但取模时 (-1)^n 需要处理：奇数时加 MOD-1（等价于 -1 mod MOD）
//
// 时间: O(n)  空间: O(1)
class Solution3 {
public:
    int findDerangement(int n) {
        const int MOD = 1e9 + 7;
        long long prev = 1; // D(0) = 1 (空排列)

        for (int i = 1; i <= n; ++i) {
            // D(i) = i * D(i-1) + (-1)^i
            prev = ((long long)i * prev % MOD + (i % 2 == 0 ? 1 : MOD - 1)) % MOD;
        }
        return (int)prev;
    }
};


// ============================================================
// 解法四：容斥原理直接计算
// ============================================================
// 思路：
//   D(n) = sum(k=0 to n) [(-1)^k * n! / k!]
//        = n! * sum(k=0 to n) [(-1)^k / k!]
//
//   由于在模运算下除法需要模逆元，直接计算 n!/k! = n*(n-1)*...*(k+1)
//   可以从 n! 开始逐步除以 k 得到各项
//
//   实现时维护 factorial = n!，逐步除以 k 得到 n!/k!
//   交替加减各项
//
// 时间: O(n)  空间: O(1)
class Solution4 {
public:
    int findDerangement(int n) {
        const long long MOD = 1e9 + 7;

        // 先计算 n!
        long long factorial = 1;
        for (int i = 2; i <= n; ++i) {
            factorial = factorial * i % MOD;
        }

        // 快速幂求模逆元
        auto power = [&](long long base, long long exp, long long mod) -> long long {
            long long result = 1;
            base %= mod;
            while (exp > 0) {
                if (exp & 1) result = result * base % mod;
                base = base * base % mod;
                exp >>= 1;
            }
            return result;
        };

        long long result = 0;
        long long term = factorial; // n!/0! = n!

        for (int k = 0; k <= n; ++k) {
            if (k > 0) {
                // term = n!/k! = term(k-1) / k = term(k-1) * modInverse(k)
                term = term % MOD * power(k, MOD - 2, MOD) % MOD;
            }
            // (-1)^k * n!/k!
            if (k % 2 == 0) {
                result = (result + term) % MOD;
            } else {
                result = (result - term + MOD) % MOD;
            }
        }

        return (int)result;
    }
};


// ============================================================
// 各解法对比
// ============================================================
// 解法一（推荐）: O(n) 时间, O(1) 空间, 代码简洁
// 解法二: O(n) 时间, O(n) 空间, 适合调试
// 解法三: O(n) 时间, O(1) 空间, 公式更简洁但取模处理稍复杂
// 解法四: O(n log MOD) 时间（模逆元需要快速幂）, O(1) 空间, 展示容斥原理
//
// 错位排列数列前几项:
// D(0) = 1, D(1) = 0, D(2) = 1, D(3) = 2, D(4) = 9
// D(5) = 44, D(6) = 265, D(7) = 1854, D(8) = 14833
//
// 渐近公式: D(n) ~ n!/e (取最近整数)
// 当 n 足够大时, D(n)/n! -> 1/e ~ 0.3679
