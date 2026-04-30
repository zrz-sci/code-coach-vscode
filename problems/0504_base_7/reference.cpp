/*
 * 【题目本质】
 * 经典进制转换：十进制转七进制。反复取余、整除，注意负数和零。
 *
 * 【解法总览】
 * 解法1: 迭代取余法  O(log₇n) / O(log₇n)  ⭐推荐
 * 解法2: 递归法      O(log₇n) / O(log₇n)
 */

// ===================== 解法1: 迭代取余法 =====================
// 思路：反复 %7 得到每一位（低位到高位），最后反转
class Solution1 {
public:
    string convertToBase7(int num) {
        if (num == 0) return "0";

        bool negative = (num < 0);
        num = abs(num);

        string result;
        while (num > 0) {
            result += to_string(num % 7);
            num /= 7;
        }

        if (negative) result += '-';
        reverse(result.begin(), result.end());
        return result;
    }
};

// ===================== 解法2: 递归法 =====================
// 思路：递归处理高位 num/7，每层拼接当前位 num%7
// 递归天然从高位到低位输出，无需反转
class Solution2 {
public:
    string convertToBase7(int num) {
        if (num < 0) return "-" + convertToBase7(-num);
        if (num < 7) return to_string(num);
        return convertToBase7(num / 7) + to_string(num % 7);
    }
};

/*
 * 【解法对比】
 * 解法1（迭代）：显式循环，需要反转字符串，逻辑清晰直接
 * 解法2（递归）：代码更精简，天然高位在前无需反转，但有栈开销
 * 实际面试两种都可，解法1 更常见
 *
 * 【易错点】
 * 1. num = 0 必须特判，否则循环体不执行返回空串
 * 2. C++ 对负数取余结果为负（-8 % 7 = -1），必须先取绝对值
 * 3. 迭代法取余顺序是低位→高位，最后要 reverse
 * 4. result += (num % 7) 会被当作 char 的 ASCII 值，应用 to_string
 *
 * 【面试追问】
 * Q1: 推广到任意 base → 将 7 替换为 base，base>10 用字母
 * Q2: 不用 reverse → 递归法天然从高到低；或先算最高位权值
 * Q3: 七进制转回十进制 → Horner 法则：result = result * 7 + digit
 */
