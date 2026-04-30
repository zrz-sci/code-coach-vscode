/*
 * 【题目本质】
 * 26 进制转 10 进制，但 A=1（不是 A=0），本质是从 1 开始编号的进制转换。
 * 与 #168 (Excel Sheet Column Title) 互为逆操作。
 *
 * 【解法总览】
 * 解法1: Horner 法则（从左到右累乘）— 最简洁
 * 解法2: 幂次求和（从右到左）— 更直观理解权重
 */

// ===================== 解法1: Horner 法则 =====================
// 思路：从左到右处理，类似十进制转换 "123" = ((1*10)+2)*10+3
// 关键：用 c - 'A' + 1 而非 c - 'A'，因为 A 对应 1 不是 0
class Solution1 {
public:
    int titleToNumber(string columnTitle) {
        int result = 0;
        for (char c : columnTitle) {
            // 每次左移一位（乘 26）再加上当前字符的值
            // 为什么 +1？因为 A=1 不是 A=0
            result = result * 26 + (c - 'A' + 1);
        }
        return result;
    }
};

// ===================== 解法2: 幂次求和 =====================
// 思路：从右到左，每一位乘以 26 的对应幂次
// "ZY" = Z*26^1 + Y*26^0 = 26*26 + 25 = 701
class Solution2 {
public:
    int titleToNumber(string columnTitle) {
        int result = 0;
        long base = 1;  // 26^0，从最低位开始
        for (int i = columnTitle.size() - 1; i >= 0; i--) {
            result += (columnTitle[i] - 'A' + 1) * base;
            base *= 26;  // 每左移一位，权重增大 26 倍
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Horner 法则 | O(n) | O(1) | 最简洁，不需要额外变量 |
 * | 幂次求和 | O(n) | O(1) | 更直观理解每位权重 |
 *
 * 【易错点】
 * 1. c - 'A' + 1 而非 c - 'A'：A 对应 1 不是 0
 * 2. 这不是标准 26 进制！标准进制从 0 开始，这里从 1 开始
 * 3. 整数溢出：最大 "FXSHRXW" = INT_MAX，用 int 刚好够
 *
 * 【面试追问】
 * Q1: 逆操作怎么做？→ #168，注意每次取余前先 n-- 来处理 1-based 偏移
 * Q2: 为什么 Horner 法则更好？→ 避免显式 pow 计算，O(1) 额外空间
 * Q3: 如果进制不是 26 而是任意 k？→ 把 26 替换为 k，'A' 替换为 base 字符
 */
