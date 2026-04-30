/*
 * LeetCode 866: Prime Palindrome
 * 解法: 枚举奇数位回文数 + 素数判定
 *
 * 核心数学性质:
 *   除了 11 以外，所有偶数位回文数都能被 11 整除，因此不可能是素数。
 *   只需枚举奇数位回文数并检查是否为素数。
 *
 * 时间复杂度: O(sqrt(UPPER) * UPPER^{1/4}) 约等于 O(10^4 * 10^2)
 * 空间复杂度: O(1)
 */

// ===================== 解法一：构造回文数 + 素数检查 =====================

class Solution {
public:
    int primePalindrome(int n) {
        // 特殊情况：小素数直接处理
        if (n <= 2) return 2;
        if (n <= 3) return 3;
        if (n <= 5) return 5;
        if (n <= 7) return 7;
        if (n <= 11) return 11;

        // 从这里开始 n >= 12，只需枚举奇数位回文数
        // 奇数位回文数位数: 3, 5, 7, 9
        // 对于 k 位奇数回文 (k = 2*half - 1)，枚举前 half 位

        for (int digits = 3; digits <= 9; digits += 2) {
            int half = (digits + 1) / 2;
            int start = pow(10, half - 1); // 前半部分最小值
            int end = pow(10, half);        // 前半部分最大值（不含）

            for (int prefix = start; prefix < end; prefix++) {
                // 由前半部分构造完整回文数
                long long palindrome = buildPalindrome(prefix, digits);

                // 超出合理范围则停止
                if (palindrome > 2e8) break;

                // 检查是否 >= n 且为素数
                if (palindrome >= n && isPrime(palindrome)) {
                    return (int)palindrome;
                }
            }
        }

        return -1; // 题目保证有解，不会到这里
    }

private:
    // 根据前半部分构造奇数位回文数
    // 例如 prefix=123, digits=5 -> 12321
    // 例如 prefix=12, digits=3 -> 121
    long long buildPalindrome(int prefix, int digits) {
        string s = to_string(prefix);
        string rev = s.substr(0, s.size() - 1);
        reverse(rev.begin(), rev.end());
        return stoll(s + rev);
    }

    // 试除法判断素数
    bool isPrime(long long x) {
        if (x < 2) return false;
        if (x == 2) return true;
        if (x % 2 == 0) return false;
        if (x == 3) return true;
        if (x % 3 == 0) return false;
        // 只需检查 6k +/- 1 形式的因子
        for (long long i = 5; i * i <= x; i += 6) {
            if (x % i == 0 || x % (i + 2) == 0) return false;
        }
        return true;
    }
};

// ===================== 解法二：直接跳过偶数位范围 =====================
// 更简洁的实现：从 n 开始逐个检查，但跳过偶数位数的范围

class SolutionSkipEven {
public:
    int primePalindrome(int n) {
        if (n <= 2) return 2;

        // 从 n 开始，如果当前数的位数是偶数且 > 11，跳到下一个奇数位数范围
        int x = n;
        while (true) {
            // 跳过偶数位范围（除了 11）
            x = skipToOddDigits(x);

            if (isPalindrome(x) && isPrime(x)) {
                return x;
            }
            x++;
        }
    }

private:
    // 如果 x 在偶数位范围内（且不是 11），跳到下一个奇数位最小值
    int skipToOddDigits(int x) {
        if (x == 11) return 11;
        int len = numDigits(x);
        if (len % 2 == 0) {
            // 跳到 10^len 的位置（即 len+1 位的最小值）
            // 例如 x 是 4 位数 -> 跳到 10000
            return (int)pow(10, len);
        }
        return x;
    }

    int numDigits(int x) {
        int count = 0;
        while (x > 0) { count++; x /= 10; }
        return count;
    }

    bool isPalindrome(int x) {
        if (x < 0) return false;
        string s = to_string(x);
        int l = 0, r = s.size() - 1;
        while (l < r) {
            if (s[l] != s[r]) return false;
            l++; r--;
        }
        return true;
    }

    bool isPrime(int x) {
        if (x < 2) return false;
        if (x == 2 || x == 3) return true;
        if (x % 2 == 0 || x % 3 == 0) return false;
        for (long long i = 5; i * i <= x; i += 6) {
            if (x % i == 0 || x % (i + 2) == 0) return false;
        }
        return true;
    }
};
