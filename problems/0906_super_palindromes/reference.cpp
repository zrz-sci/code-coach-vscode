/*
 * LeetCode 906: Super Palindromes
 *
 * 解法1: 枚举回文根 (面试首选)
 * 时间: O(W^{1/4} * log W)  空间: O(log W)
 * 其中 W = 10^18
 *
 * 思路: 枚举种子(1~99999)→构造回文数x→计算x^2→检查x^2是否回文且在范围内
 */

// ==================== 解法1: 枚举回文根 ====================

class Solution {
public:
    int superpalindromesInRange(string left, string right) {
        long long L = stoll(left);
        long long R = stoll(right);
        int count = 0;

        // 种子范围: [1, 99999]
        // 种子对应回文数 x 最多 10 位 (x <= ~10^9)
        // x^2 最大约 10^18，在 long long 范围内

        // ---- 枚举奇数位回文 ----
        // 种子 "abc" → 回文 "abcba" (2*len-1 位)
        for (long long seed = 1; seed < 100000; seed++) {
            string s = to_string(seed);
            // 构造奇数位回文: s + reverse(s 去掉最后一位)
            string palin = s;
            for (int i = (int)s.size() - 2; i >= 0; i--) {
                palin += s[i];
            }

            long long x = stoll(palin);
            long long sq = x * x;

            // 如果 x^2 已经超过右边界，后续种子只会更大，终止
            if (sq > R) break;

            // 检查 x^2 是否在范围内且是回文数
            if (sq >= L && isPalindrome(sq)) {
                count++;
            }
        }

        // ---- 枚举偶数位回文 ----
        // 种子 "abc" → 回文 "abccba" (2*len 位)
        for (long long seed = 1; seed < 100000; seed++) {
            string s = to_string(seed);
            // 构造偶数位回文: s + reverse(s 完整)
            string palin = s;
            for (int i = (int)s.size() - 1; i >= 0; i--) {
                palin += s[i];
            }

            long long x = stoll(palin);
            long long sq = x * x;

            if (sq > R) break;
            if (sq >= L && isPalindrome(sq)) {
                count++;
            }
        }

        return count;
    }

private:
    // 判断一个 long long 是否是回文数
    bool isPalindrome(long long n) {
        if (n < 0) return false;
        string s = to_string(n);
        int l = 0, r = (int)s.size() - 1;
        while (l < r) {
            if (s[l] != s[r]) return false;
            l++;
            r--;
        }
        return true;
    }
};

// ==================== 解法2: 合并循环版本 ====================

/*
 * 和解法1逻辑相同，但把奇偶回文合并到一个循环中
 * 代码更紧凑，但需要注意 break/continue 的区别
 */

class Solution_Merged {
public:
    int superpalindromesInRange(string left, string right) {
        long long L = stoll(left);
        long long R = stoll(right);
        int count = 0;

        for (long long seed = 1; seed < 100000; seed++) {
            string s = to_string(seed);

            // 构造奇数位回文
            string oddP = s;
            for (int i = (int)s.size() - 2; i >= 0; i--) {
                oddP += s[i];
            }
            long long x1 = stoll(oddP);
            long long sq1 = x1 * x1;
            if (sq1 <= R && sq1 >= L && isPalindrome(sq1)) {
                count++;
            }

            // 构造偶数位回文
            string evenP = s;
            for (int i = (int)s.size() - 1; i >= 0; i--) {
                evenP += s[i];
            }
            long long x2 = stoll(evenP);
            long long sq2 = x2 * x2;
            if (sq2 <= R && sq2 >= L && isPalindrome(sq2)) {
                count++;
            }
        }

        return count;
    }

private:
    bool isPalindrome(long long n) {
        string s = to_string(n);
        int l = 0, r = (int)s.size() - 1;
        while (l < r) {
            if (s[l] != s[r]) return false;
            l++;
            r--;
        }
        return true;
    }
};

// ==================== 解法3: 数值反转判断回文(无字符串) ====================

/*
 * 和解法1相同的枚举策略，但用数值方式判断回文
 * 避免字符串转换的开销(微优化)
 */

class Solution_NumReverse {
public:
    int superpalindromesInRange(string left, string right) {
        long long L = stoll(left);
        long long R = stoll(right);
        int count = 0;

        // 奇数位回文
        for (long long seed = 1; seed < 100000; seed++) {
            long long x = buildOddPalindrome(seed);
            long long sq = x * x;
            if (sq > R) break;
            if (sq >= L && isPalindromeNum(sq)) {
                count++;
            }
        }

        // 偶数位回文
        for (long long seed = 1; seed < 100000; seed++) {
            long long x = buildEvenPalindrome(seed);
            long long sq = x * x;
            if (sq > R) break;
            if (sq >= L && isPalindromeNum(sq)) {
                count++;
            }
        }

        return count;
    }

private:
    // 用纯数值方式构造奇数位回文
    // seed=123 → 12321
    long long buildOddPalindrome(long long seed) {
        long long result = seed;
        seed /= 10; // 去掉最后一位
        while (seed > 0) {
            result = result * 10 + seed % 10;
            seed /= 10;
        }
        return result;
    }

    // 用纯数值方式构造偶数位回文
    // seed=123 → 123321
    long long buildEvenPalindrome(long long seed) {
        long long result = seed;
        while (seed > 0) {
            result = result * 10 + seed % 10;
            seed /= 10;
        }
        return result;
    }

    // 数值方式判断回文
    bool isPalindromeNum(long long n) {
        if (n < 0) return false;
        if (n == 0) return true;
        if (n % 10 == 0) return false; // 末尾为0不可能是回文(除了0本身)

        long long reversed = 0;
        long long original = n;
        while (n > 0) {
            reversed = reversed * 10 + n % 10;
            n /= 10;
        }
        return original == reversed;
    }
};

/*
 * ==================== 复杂度分析 ====================
 *
 * 设 W = 10^18 (数值上限)
 *
 * x^2 <= W  →  x <= W^{1/2} = 10^9
 * x 是回文数，由前半部分(种子)确定
 * 种子位数 = ceil(digits(x) / 2) <= ceil(10/2) = 5
 * 种子范围: [1, 99999]，约 10^5 个
 *
 * 每个种子:
 * - 构造回文: O(log x) ≈ O(10)
 * - 计算平方: O(1)
 * - 判断回文: O(log(x^2)) ≈ O(18)
 *
 * 总时间: O(10^5 * 18) ≈ O(2 * 10^6) — 非常快
 *
 * 空间: O(log W) 用于字符串临时变量
 *
 * ==================== 实际超级回文数数量 ====================
 *
 * 在 [1, 10^18] 范围内只有约 70 个超级回文数。
 * 前几个: 1, 4, 9, 121, 484, 10201, 12321, ...
 *
 * 面试策略:
 * 1. 先分析: 直接枚举 [L,R] 不可行 (10^18)
 * 2. 突破口: 从"根"(被平方的回文数)入手
 * 3. 进一步: 回文数由种子确定，种子只有~10^5个
 * 4. 实现: 对每个种子构造奇偶回文 → 平方 → 检查
 */
