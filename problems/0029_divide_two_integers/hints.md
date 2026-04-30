# 29. 两数相除

## 核心思路

本质上是在**不用乘法/除法/取余**的前提下，用**加法和位运算**模拟除法过程。关键洞察：除法就是"被除数里能装多少个除数"，而倍增思想可以让我们跳着数，从 O(商) 降到 O(log²n)。

## 思维链

1. **读完题第一反应**：除法 = 减法的累积。10 ÷ 3 就是 10 里能减掉几个 3？→ 每次减一个 3，减了几次就是商。这就是暴力法。

2. **暴力解的瓶颈**：如果 dividend = 2^31-1, divisor = 1，商是 ~21亿，循环 21 亿次会超时。瓶颈在于每次只减去 **一个** divisor，太慢了。

3. **怎么突破瓶颈？** 不要一个一个减，能不能一次减掉 **很多个** divisor？想到"倍增"：先尝试减去 divisor×2^k（用左移实现"乘以2的幂"），从大到小试，类似二进制分解。

4. **倍增的具体做法**：
   - 用 `divisor << k` 代替 `divisor × 2^k`（左移 = 乘以2，不违反规则）
   - 找到最大的 k 使得 `divisor << k <= dividend`
   - 商加上 `1 << k`，被除数减去 `divisor << k`
   - 重复直到被除数 < 除数

5. **溢出问题是这道题最大的坑**：`-2^31` 取绝对值会溢出 int！解决方案：全程用**负数**做运算（因为负数范围比正数大一个），或者用 `long long`。

6. **符号处理**：记录结果的正负号，转换后全程用同号数运算。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力减法 | 每次减一个除数 | O(商) | O(1) | 能说出即可 |
| 倍增法(long long) | 每次减去 divisor×2^k | O(log²n) | O(1) | ⭐ 必须写出 |
| 倍增法(纯int负数) | 全程负数域避免溢出 | O(log²n) | O(1) | 加分项 |

## 关键提示

1. **除法的本质**：`dividend ÷ divisor = ?` 等价于"从 dividend 里最多能取走多少个 divisor"。

2. **倍增加速**：不要一个个减，尝试把 divisor 翻倍（左移），一次减掉尽可能多的 divisor。想想二进制表示：商 = 某些 2 的幂之和。

3. **溢出陷阱**：`INT_MIN = -2147483648`，取绝对值 `abs(INT_MIN)` 会溢出！只有一种情况商会溢出：`-2^31 ÷ (-1) = 2^31 > INT_MAX`。

4. **为什么用负数域？** 负数的范围是 `[-2^31, -1]`，比正数 `[1, 2^31-1]` 多一个数。全程在负数域运算就不会溢出。

5. **左移的溢出风险**：`divisor << k` 可能溢出，需要在移位前检查 `divisor >= 某个下界` 才能安全左移。

```
// 倍增过程可视化: 43 ÷ 3
//
// Round1: 3<<0=3, 3<<1=6, 3<<2=12, 3<<3=24, 3<<4=48>43 STOP
//         减去 3<<3=24, 商+=1<<3=8, 余数=43-24=19
//
// Round2: 3<<0=3, 3<<1=6, 3<<2=12, 3<<3=24>19 STOP
//         减去 3<<2=12, 商+=1<<2=4, 余数=19-12=7
//
// Round3: 3<<0=3, 3<<1=6, 3<<2=12>7 STOP
//         减去 3<<1=6, 商+=1<<1=2, 余数=7-6=1
//
// Round4: 3>1 STOP → 余数1 < 除数3, 结束
//
// 商 = 8+4+2 = 14, 余数 = 1
// 验证: 14×3+1 = 43 ✓
```

## 解法详解

### 解法1: 暴力减法 — O(商) / O(1)

**思考过程**：除法最原始的定义——从被除数里一个一个减去除数，数减了几次。

```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        // 特殊情况：唯一可能溢出的商
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        // 确定结果符号
        bool negative = (dividend > 0) != (divisor > 0);
        
        // 转为 long long 绝对值运算
        long long a = abs((long long)dividend);
        long long b = abs((long long)divisor);
        
        int result = 0;
        while (a >= b) {  // 每次减一个 b
            a -= b;
            result++;
        }
        
        return negative ? -result : result;
    }
};
```

**关键点**：当 dividend = INT_MIN, divisor = 1 时循环 2^31 次，**必定超时**。这个解法只是为了理解题意。

---

### 解法2: 倍增法(long long版) — O(log²n) / O(1) ⭐ 面试首选

**从解法1优化**：解法1每次只减一个 divisor，太慢。倍增思想：每轮找到最大的 `k`，使 `divisor << k <= 剩余被除数`，一次减去 `divisor × 2^k`，商累加 `2^k`。

为什么是 O(log²n)？外层循环最多 log(n) 轮（每轮至少减去一半），内层找 k 最多 log(n) 次，总共 O(log²n)。

```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        // 唯一溢出情况: -2^31 / -1 = 2^31 > INT_MAX
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        // 记录符号，然后转成正数(用long long防溢出)
        bool negative = (dividend > 0) != (divisor > 0);
        long long a = abs((long long)dividend);
        long long b = abs((long long)divisor);
        
        int result = 0;
        while (a >= b) {
            long long temp = b;  // temp = divisor 的倍增版
            int power = 0;
            // 倍增: 找到最大的 2^power 使得 b * 2^power <= a
            // 用 temp << 1 代替 temp * 2 (左移=乘以2)
            while (a >= (temp << 1)) {
                temp <<= 1;
                power++;
            }
            // 此时 temp = b * 2^power, 且 temp <= a < temp*2
            result += (1 << power);  // 商加上 2^power
            a -= temp;               // 被除数减去这一大块
        }
        
        return negative ? -result : result;
    }
};
```

**关键点**：
- `temp << 1` 可能溢出 long long？不会，因为 a 最大 2^31，temp 不会超过 a。
- 为什么外层循环 while(a >= b)？因为减完一大块后剩余的部分可能还能再减。

---

### 解法3: 倍增法(纯int负数域) — O(log²n) / O(1) — 加分项

**动机**：面试官可能追问"能否不用 long long？"。技巧：把所有数转成**负数**运算，因为 int 负数域 `[-2^31, -1]` 比正数域 `[1, 2^31-1]` 大一个，`INT_MIN` 不需要取绝对值。

```cpp
class Solution {
public:
    int divide(int dividend, int divisor) {
        // 唯一溢出情况
        if (dividend == INT_MIN && divisor == -1) return INT_MAX;
        
        bool negative = (dividend > 0) != (divisor > 0);
        
        // 全部转为负数（不会溢出！）
        int a = dividend > 0 ? -dividend : dividend;  // a <= 0
        int b = divisor > 0 ? -divisor : divisor;      // b <= 0
        
        int result = 0;
        // 注意：负数比较方向反了, a <= b 等价于 |a| >= |b|
        while (a <= b) {
            int temp = b;
            int power = 0;
            // 防止 temp<<1 溢出: temp 必须 >= INT_MIN/2 才能安全左移
            // 负数左移是变得更小(绝对值更大)
            while (temp >= (INT_MIN >> 1) && a <= (temp << 1)) {
                temp <<= 1;
                power++;
            }
            result += (1 << power);
            a -= temp;  // 负数减负数 = 绝对值变小
        }
        
        return negative ? -result : result;
    }
};
```

**关键点**：
- 负数域中 `a <= b` 表示 `|a| >= |b|`，比较方向全部反过来。
- `temp >= (INT_MIN >> 1)` 是左移的溢出保护：如果 temp 已经小于 `INT_MIN/2`，再左移就溢出了。

## 解法对比

| | 暴力减法 | 倍增(long long) | 倍增(纯int负数) |
|---|---|---|---|
| 时间 | O(商), 最坏 O(2^31) | O(log²n) | O(log²n) |
| 空间 | O(1) | O(1) | O(1) |
| 溢出处理 | 用long long | 用long long | 纯int,负数域 |
| 代码难度 | 简单 | 中等 | 较难 |
| 面试推荐 | 说说思路即可 | **首选** | 追问时展示 |

解法2是面试最佳平衡：思路清晰、代码简洁、不易出错。解法3在面试官明确要求"不能用 long long"时再写。

## 易错点

1. **✗ 忘记处理 `INT_MIN / -1` 溢出**
   ```cpp
   // ✗ 直接运算，-2147483648 / -1 = 2147483648 溢出int
   return abs(dividend) / abs(divisor);
   ```
   ```cpp
   // ✓ 特判这个唯一的溢出情况
   if (dividend == INT_MIN && divisor == -1) return INT_MAX;
   ```

2. **✗ `abs(INT_MIN)` 溢出**
   ```cpp
   // ✗ INT_MIN的绝对值2147483648超出int范围
   int a = abs(dividend);
   ```
   ```cpp
   // ✓ 先转long long再取绝对值
   long long a = abs((long long)dividend);
   ```

3. **✗ 倍增时 `temp << 1` 溢出没检查**
   ```cpp
   // ✗ 纯int版本，temp可能左移后溢出
   while (a <= (temp << 1)) { ... }
   ```
   ```cpp
   // ✓ 加溢出保护
   while (temp >= (INT_MIN >> 1) && a <= (temp << 1)) { ... }
   ```

4. **✗ 符号判断写错**
   ```cpp
   // ✗ 如果 dividend=0 时，0>0 是 false，这样也没问题
   // 但如果用 dividend < 0 != divisor < 0 可能有优先级问题
   bool neg = dividend < 0 != divisor < 0;  // 运算符优先级错误!
   ```
   ```cpp
   // ✓ 加括号明确优先级
   bool neg = (dividend > 0) != (divisor > 0);
   ```

## 面试追问

**Q1: 暴力法为什么不行？瓶颈在哪？**
→ 每次只减一个除数，当商很大（如 INT_MAX/1）时要循环 2^31 次。瓶颈是"每次只前进一步"。

**Q2: 倍增法为什么是 O(log²n) 而不是 O(logn)？能优化到 O(logn) 吗？**
→ 外层循环最多 32 轮（每轮确定商的一个二进制位），内层每轮从头倍增找最大 k，最多 32 次。总共 O(32×32) = O(log²n)。可以优化：从最高位开始往下试，不需要每轮重新从头倍增。

```cpp
// O(logn) 版本思路：
// 预计算 divisor<<0, divisor<<1, ..., divisor<<31
// 从高位到低位贪心地减，类似二进制长除法
```

**Q3: 如果不允许使用 long long，怎么处理溢出？**
→ 转到负数域运算（解法3）。负数范围比正数大一个，INT_MIN 不需要取绝对值，所有运算在负数域完成后再处理符号。

**Q4: 这道题和位运算有什么关系？**
→ 商的本质是一个整数，可以用二进制表示。每一轮倍增确定商的某一个二进制位是 0 还是 1。左移 `<<` 就是乘以 2，`1 << k` 就是 2^k，整个过程完全用加法和位运算完成，没有乘除。

## 相关题型

- **LeetCode 50. Pow(x, n)** — 同样使用**倍增/快速幂**思想：不是乘 n 次 x，而是通过平方倍增到 O(logn)。本题的倍增减法和快速幂的"倍增乘法"是同一个思维模式。
- **LeetCode 371. 两整数之和** — 同样限制不能用某个运算符（加法），用位运算模拟。练习用位运算替代基本运算的思路。
- **LeetCode 191. 位1的个数** — 练习 `n & (n-1)` 等位运算技巧，帮助理解位操作的底层逻辑。