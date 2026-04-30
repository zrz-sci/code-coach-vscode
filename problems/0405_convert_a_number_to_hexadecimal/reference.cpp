/*
 * 405. Convert a Number to Hexadecimal
 *
 * 【题目本质】
 * 将 32 位有符号整数转为十六进制字符串，负数用补码表示。
 * 核心考点：位运算 + 进制转换 + 负数补码处理。
 *
 * 【解法总览】
 * Solution1: 位运算逐位提取（从低到高）— O(1) 时间/空间
 * Solution2: 位运算从高到低     — O(1) 时间/空间
 */

// ===================== Solution 1: 从低位到高位提取 =====================
// 思路：每次取最低 4 位 (num & 0xf)，映射到十六进制字符，右移 4 位
//       用 unsigned int 处理负数的补码
// 时间：O(8) = O(1) | 空间：O(1)
class Solution1 {
public:
    string toHex(int num) {
        if (num == 0) return "0";
        string hex = "0123456789abcdef";
        string res;
        unsigned int n = num;  // 关键：转 unsigned 处理负数
        while (n) {
            res = hex[n & 0xf] + res;  // 前插，自动得到正确顺序
            n >>= 4;
        }
        return res;
    }
};

// ===================== Solution 2: 从高位到低位处理 =====================
// 思路：固定 8 位十六进制，从最高 4 位开始提取，跳过前导零
// 时间：O(8) = O(1) | 空间：O(1)
class Solution2 {
public:
    string toHex(int num) {
        if (num == 0) return "0";
        string hex = "0123456789abcdef";
        string res;
        unsigned int n = num;
        bool leading = true;
        for (int i = 7; i >= 0; i--) {
            int digit = (n >> (i * 4)) & 0xf;
            if (digit != 0) leading = false;
            if (!leading) res += hex[digit];
        }
        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 从低到高 | O(1) | O(1) | 简洁，需要前插或反转 |
 * | 从高到低 | O(1) | O(1) | 天然顺序，需处理前导零 |
 *
 * 【易错点】
 * 1. 必须用 unsigned int，否则负数右移填充符号位导致死循环
 * 2. num=0 特判，否则循环不执行返回空串
 * 3. 题目要求小写字母，映射表用 'a'-'f' 不是 'A'-'F'
 * 4. 不允许使用 sprintf/stringstream 等库函数
 *
 * 【面试追问】
 * Q1: 算术右移 vs 逻辑右移？ => 有符号填符号位，无符号填0
 * Q2: 不转 unsigned 怎么办？ => 固定循环 8 次避免死循环
 * Q3: 推广到任意进制？ => num%base 取位, num/=base, 但负数需额外处理
 */
