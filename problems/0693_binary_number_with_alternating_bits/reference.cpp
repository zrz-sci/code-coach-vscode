// 693. Binary Number with Alternating Bits
// 难度: Easy
// 标签: Bit Manipulation

/*
 * 题目描述：
 * 给定一个正整数 n，判断其二进制表示是否具有交替位模式
 * （即任意两个相邻位的值总是不同的）。
 *
 * 示例：
 * 5 (101) -> true
 * 7 (111) -> false
 * 10 (1010) -> true
 * 11 (1011) -> false
 *
 * 核心思路：
 * n ^ (n >> 1) 对于交替位数字会产生全 1 结果，
 * 用 x & (x+1) == 0 判断是否全 1。
 */

// ==================== 解法一：位运算一行解（推荐）====================
// 时间 O(1)，空间 O(1)
// 思路：n 与 n>>1 异或，交替位数字的结果是全 1
class Solution {
public:
    bool hasAlternatingBits(int n) {
        // 步骤 1: n ^ (n >> 1) 产生结果 x
        // 如果 n 是交替位，x 的每一位都是 1
        //
        // 例: n = 5 (101)
        //     n >> 1 = 2 (010)
        //     x = 101 ^ 010 = 111
        //
        // 例: n = 7 (111)
        //     n >> 1 = 3 (011)
        //     x = 111 ^ 011 = 100  (不是全1)
        long long x = n ^ ((long long)n >> 1);

        // 步骤 2: 判断 x 是否为全 1
        // 全 1 数的特征: x & (x + 1) == 0
        // 因为全 1 加 1 后进位变成 10...0，与原数没有公共位
        return (x & (x + 1)) == 0;
    }
};

// ==================== 解法二：逐位检查 ====================
// 时间 O(log n)，空间 O(1)
// 思路：逐位取出，检查相邻两位是否不同
class Solution_BitByBit {
public:
    bool hasAlternatingBits(int n) {
        // 取出最低位作为前一位的参考
        int prev = n & 1;
        n >>= 1;

        while (n > 0) {
            int curr = n & 1;
            // 如果当前位与前一位相同，不是交替位
            if (curr == prev) {
                return false;
            }
            prev = curr;
            n >>= 1;
        }

        return true;
    }
};

// ==================== 解法三：除法取模检查 ====================
// 时间 O(log n)，空间 O(1)
// 思路：用除法和取模代替位运算，逻辑完全等价
class Solution_DivMod {
public:
    bool hasAlternatingBits(int n) {
        int prev = n % 2;
        n /= 2;

        while (n > 0) {
            int curr = n % 2;
            if (curr == prev) {
                return false;
            }
            prev = curr;
            n /= 2;
        }

        return true;
    }
};

// ==================== 解法四：位运算变体 ====================
// 时间 O(1)，空间 O(1)
// 思路：利用交替位数字右移后相加会产生全 1 的特性
class Solution_AddShift {
public:
    bool hasAlternatingBits(int n) {
        // 对于交替位数字：
        // n     = ...10101
        // n>>1  = ...01010
        // n + (n>>1) = ...11111 (全1)
        //
        // 但需要注意溢出，使用 long long
        long long sum = (long long)n + (n >> 1);
        // 全 1 判断
        return (sum & (sum + 1)) == 0;
    }
};

// ==================== 解法五：异或连续检查 ====================
// 时间 O(log n)，空间 O(1)
// 思路：每次比较最低两位，然后右移1位继续
class Solution_PairCheck {
public:
    bool hasAlternatingBits(int n) {
        while (n > 0) {
            // 取最低位和次低位
            int bit0 = n & 1;
            int bit1 = (n >> 1) & 1;

            // 如果只剩一位（n < 2），直接通过
            if (n < 2) break;

            // 相邻两位必须不同
            if (bit0 == bit1) return false;

            n >>= 1;  // 只移动一位，因为每次比较 bit0 和 bit1
        }
        return true;
    }
};

/*
 * 复杂度分析：
 * - 解法一和四: O(1) 时间，O(1) 空间（常数次位运算）
 * - 解法二、三、五: O(log n) 时间，O(1) 空间
 *
 * 关键要点：
 * 1. n ^ (n >> 1) 对交替位产生全 1 是核心洞察
 * 2. 全 1 判断: x & (x + 1) == 0，这个技巧在多个位运算题中复用
 * 3. 注意 int 溢出: n >> 1 后与 n 相加可能溢出 int 范围
 * 4. 交替位数字只有两种模式: 0101... 和 1010...
 * 5. 32 位以内只有约 64 个交替位数字，可以预计算但没必要
 *
 * 面试推荐：解法一最简洁（一行核心逻辑），但要能解释原理。
 * 如果面试官要求不用 trick，解法二最稳妥。
 */
