// LeetCode 479: Largest Palindrome Product
// 核心：从大到小枚举回文数，验证能否分解为两个 n 位数的乘积
//
// 思路：
// 1. 回文数由上半部分 half 决定，从 10^n-1 递减枚举
// 2. 将 half 镜像拼接构造完整 2n 位回文数
// 3. 检查该回文数能否分解为两个 n 位数的乘积
// 4. 第一个能分解的就是最大回文数乘积
//
// 时间 O(10^n) 约，空间 O(1)

// ==================== 解法1: 字符串构造回文数（推荐）====================
class Solution {
public:
    int largestPalindrome(int n) {
        // 特殊情况：1 位数最大回文数 = 9 = 3 * 3
        if (n == 1) return 9;

        long upper = (long)pow(10, n) - 1;     // n 位数上界
        long lower = (long)pow(10, n - 1);     // n 位数下界

        // 从大到小枚举回文数的上半部分
        for (long half = upper; half >= lower; --half) {
            // 构造完整回文数：half + reverse(half)
            string s = to_string(half);
            string rev = s;
            reverse(rev.begin(), rev.end());
            long palindrome = stol(s + rev);

            // 验证：尝试将回文数分解为两个 n 位数的乘积
            // 从 upper 向下枚举较大的因子
            for (long i = upper; i * i >= palindrome; --i) {
                if (palindrome % i == 0) {
                    long other = palindrome / i;
                    // 确认另一个因子也在 n 位数范围内
                    if (other >= lower && other <= upper) {
                        return (int)(palindrome % 1337);
                    }
                }
            }
        }

        return 0;  // 不会到达这里
    }
};

// ==================== 解法2: 数学构造回文数（避免字符串）====================
// 不使用 to_string/stol，纯数学拼接
class Solution_Math {
public:
    int largestPalindrome(int n) {
        if (n == 1) return 9;

        long upper = (long)pow(10, n) - 1;
        long lower = (long)pow(10, n - 1);

        for (long half = upper; half >= lower; --half) {
            // 数学方法构造回文数
            // 将 half 的每一位反转拼接到后面
            long palindrome = half;
            long temp = half;
            while (temp > 0) {
                palindrome = palindrome * 10 + temp % 10;
                temp /= 10;
            }

            // 验证分解
            for (long i = upper; i * i >= palindrome; --i) {
                if (palindrome % i == 0) {
                    long quotient = palindrome / i;
                    if (quotient >= lower && quotient <= upper) {
                        return (int)(palindrome % 1337);
                    }
                }
            }
        }

        return 0;
    }
};

// ==================== 验证示例 ====================
// n=2: upper=99, lower=10
// half=99: palindrome=9999, 无法分解为两个两位数乘积
// half=98: palindrome=9889, 无法分解
// ...
// half=90: palindrome=9009, 9009 = 99 * 91, 两个都是两位数！
// 答案 = 9009 % 1337 = 987

// ==================== 复杂度分析 ====================
// 时间：O(10^n) — 外层枚举 half，内层验证
//   实际上通常前几个回文数就能找到答案
// 空间：O(1) — 不算字符串转换的话
//
// n=8 时回文数最大约 10^16，long long (9.2*10^18) 足够
// 因子验证的终止条件 i*i >= palindrome 很重要
//   当 i*i < palindrome 时，palindrome/i > i > upper，不可能
