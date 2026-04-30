/*
 * LeetCode 29: 两数相除 (Divide Two Integers)
 *
 * 【题目本质】
 * 不用乘法/除法/取余，实现整数除法。
 * 核心思想：除法 = "被除数里能装多少个除数"，用倍增加速计数。
 *
 * 【解法总览】
 * 解法1: 暴力减法 — O(商) / O(1) — 直觉，但太慢
 * 解法2: 倍增法(long long) — O(log²n) / O(1) — 面试首选
 * 解法3: 倍增法(纯int负数域) — O(log²n) / O(1) — 不用long long的进阶版
 */

// ============================================================
// 解法1: 暴力减法
// 时间: O(商), 最坏 O(2^31)  空间: O(1)
//
// 【思路】
// 除法最原始的定义: 从被除数里一个一个减去除数，数减了几次。
// 比如 10÷3: 10-3=7, 7-3=4, 4-3=1, 1<3停止 → 减了3次，商=3
//
// 为什么要讲这个：理解"除法就是重复减法"是后续优化的基础。
// 瓶颈：dividend=INT_MAX, divisor=1 时要减 2^31 次 → 超时
// ============================================================
class Solution1 {
public:
    int divide(int dividend, int divisor) {
        // 唯一可能溢出的情况: -2^31 / -1 = 2^31 > INT_MAX
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        // 确定结果符号: 异号为负
        bool negative = (dividend > 0) != (divisor > 0);
        
        // 转为 long long 取绝对值（INT_MIN 取绝对值会溢出 int）
        long long a = abs((long long)dividend);
        long long b = abs((long long)divisor);
        
        int result = 0;
        while (a >= b) {
            a -= b;       // 每次减去一个除数
            result++;     // 计数+1
        }
        
        return negative ? -result : result;
    }
};

// ============================================================
// 解法2: 倍增法 (long long 版) — 面试首选 ⭐
// 时间: O(log²n)  空间: O(1)
//
// 【思路】
// 解法1每次只减一个divisor太慢。能不能一次减掉很多个？
//
// 倍增思想：尝试减去 divisor×1, divisor×2, divisor×4, ...
// 找到最大的 2^k 使得 divisor×2^k <= 剩余被除数，
// 然后商 += 2^k，被除数 -= divisor×2^k，重复。
//
// 关键：divisor×2^k 用左移 divisor<<k 实现，没有用乘法！
//
// 过程可视化: 43 ÷ 3
//
// Round1: 尝试 3, 6, 12, 24, [48>43 停]
//         减去24, 商+=8, 余=19
// Round2: 尝试 3, 6, 12, [24>19 停]
//         减去12, 商+=4, 余=7
// Round3: 尝试 3, 6, [12>7 停]
//         减去6, 商+=2, 余=1
// Round4: [3>1 停] → 结束
// 商 = 8+4+2 = 14 ✓
//
// 复杂度分析:
// 外层: 每轮至少减去当前余数的一半以上 → 最多 O(logn) 轮
// 内层: 每轮倍增找 k → 最多 O(logn) 次
// 总计: O(log²n), 实际上界 O(32×32) = O(1024)
// ============================================================
class Solution2 {
public:
    int divide(int dividend, int divisor) {
        // 特判唯一溢出: -2147483648 / -1
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        // 异号则结果为负
        bool negative = (dividend > 0) != (divisor > 0);
        
        // 转 long long 后取绝对值，避免 INT_MIN 溢出
        long long a = abs((long long)dividend);
        long long b = abs((long long)divisor);
        
        int result = 0;
        
        while (a >= b) {
            long long temp = b;   // temp 是 divisor 的倍增版本
            int power = 0;        // 记录倍增了几次
            
            // 倍增: temp从b开始不断翻倍, 直到再翻倍就超过a
            // 为什么用 temp<<1 而不是 temp*2? 题目禁止乘法，左移是位运算
            while (a >= (temp << 1)) {
                temp <<= 1;      // temp *= 2
                power++;          // 倍增次数+1
            }
            
            // 此时 temp = b * 2^power, 且 temp <= a < temp*2
            result += (1 << power);  // 商加上 2^power（这一轮减掉了这么多个divisor）
            a -= temp;               // 被除数减去这一大块
        }
        
        return negative ? -result : result;
    }
};

// ============================================================
// 解法3: 倍增法 (纯 int 负数域) — 不使用 long long
// 时间: O(log²n)  空间: O(1)
//
// 【思路】
// 面试追问: "如果不能用 long long 怎么办?"
//
// 核心问题: INT_MIN = -2147483648, 取绝对值2147483648超出int范围
// 解决方案: 把所有数转成负数运算!
//   - int负数范围 [-2^31, -1] 比正数范围 [1, 2^31-1] 大一个
//   - INT_MIN 本身就是负数，不需要转换
//
// 注意点:
//   1. 负数域中，绝对值越大数越小: -10 < -3
//   2. a <= b 等价于 |a| >= |b|（比较方向全部反过来）
//   3. 左移负数是绝对值变大(数值变小): -3<<1 = -6
//   4. 左移前要检查溢出: temp >= INT_MIN/2 才能安全左移
// ============================================================
class Solution3 {
public:
    int divide(int dividend, int divisor) {
        // 特判唯一溢出情况
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        bool negative = (dividend > 0) != (divisor > 0);
        
        // 转为负数（正数变负数不会溢出，负数保持不变）
        int a = dividend > 0 ? -dividend : dividend;  // a <= 0
        int b = divisor > 0 ? -divisor : divisor;      // b <= 0
        
        int result = 0;
        
        // 在负数域: a <= b 意味着 |a| >= |b|，即还能继续减
        while (a <= b) {
            int temp = b;
            int power = 0;
            
            // 溢出保护: temp >= INT_MIN >> 1 确保 temp<<1 不会溢出
            // 因为 INT_MIN >> 1 = -1073741824, 如果 temp < 这个值
            // 再左移就会超出 int 负数下界
            while (temp >= (INT_MIN >> 1) && a <= (temp << 1)) {
                temp <<= 1;    // 绝对值翻倍(数值变得更小/更负)
                power++;
            }
            
            result += (1 << power);
            a -= temp;  // 负数减负数: a-temp, 因为temp<0, 所以是a+|temp|, 绝对值变小
        }
        
        return negative ? -result : result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 用 long long? | 代码复杂度 |
// |------|------|------|---------------|-----------|
// | 暴力减法 | O(商)~O(2^31) | O(1) | 是 | 简单 |
// | 倍增(long long) | O(log²n) | O(1) | 是 | 中等 |
// | 倍增(纯int负数) | O(log²n) | O(1) | 否 | 较高 |
//
// 面试建议:
// - 先口述暴力思路表明理解题意，然后直接写解法2
// - 如果面试官追问"不用long long"，再写解法3
// - 解法2代码简洁，出错概率最低，是最优选择
//
// ============================================================
// 【易错点】
//
// 1. abs(INT_MIN) 溢出:
//    ✗ int a = abs(dividend);  // 当 dividend=INT_MIN 时溢出
//    ✓ long long a = abs((long long)dividend);
//
// 2. 忘记特判 INT_MIN / -1:
//    这是唯一一种商溢出的情况 (-2^31 / -1 = 2^31 > INT_MAX)
//    必须在函数开头特判返回 INT_MAX
//
// 3. 纯int版左移溢出:
//    ✗ while (a <= (temp << 1))  // temp太小时左移溢出
//    ✓ while (temp >= (INT_MIN >> 1) && a <= (temp << 1))
//
// 4. 符号判断运算符优先级:
//    ✗ bool neg = dividend > 0 != divisor > 0;  // != 优先级高于 >
//    ✓ bool neg = (dividend > 0) != (divisor > 0);
//
// 5. 负数域比较方向搞反:
//    正数域: a >= b 表示还能减
//    负数域: a <= b 表示还能减（绝对值关系反了）
//
// ============================================================
// 【面试追问】
//
// Q1: "暴力法的时间复杂度是多少？什么输入最坏？"
//     → O(商), 最坏 INT_MAX/1 = 2^31 次。瓶颈是每次只减一个除数。
//
// Q2: "倍增法为什么是 O(log²n)？能否优化到 O(logn)？"
//     → 外层O(logn)轮 × 内层O(logn)倍增 = O(log²n)。
//       优化: 预计算所有 divisor<<k，从高位到低位贪心试减，
//       每个位只试一次，总共 O(32) = O(logn)。
//
// Q3: "不用 long long 怎么处理 INT_MIN？"
//     → 转负数域运算。负数范围 [-2^31, -1] 覆盖所有 int，
//       INT_MIN 不需要取绝对值，全程负数运算后再处理符号。
//
// Q4: "这道题和快速幂(Pow(x,n))有什么联系？"
//     → 都是倍增思想。快速幂把 n 次乘法降到 logn 次，
//       本题把"商"次减法降到 log²n 次。
//       本质都是利用二进制分解: 将目标拆成 2^k 之和。
// ============================================================
