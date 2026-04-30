# 233. 数字 1 的个数 (Number of Digit One)

## 核心思路

给定整数 n，统计 0 ~ n 所有数字中，数字 1 总共出现了多少次。暴力逐个数统计是 O(n log n)，n 可达 10^9，完全不可行。

本题的本质是**逐位数学分析**：对 n 的每一位（个位、十位、百位...），独立计算 0 ~ n 中"该位上恰好是 1"的数有多少个，然后把所有位的贡献加起来。

这是一类经典的**数位统计**问题，核心技巧是把数字按位拆分，对每一位分情况讨论。

## 思维链

1. **读完题第一反应**：暴力枚举 1 到 n 的每个数，数每个数中有几个 1。时间 O(n * 位数)，n = 10^9 时约 10^10 次操作，太慢。

2. **换个视角 —— 按"位"统计而不是按"数"统计**：不要问"每个数有几个 1"，而是问"在个位上，有多少个数的个位是 1？在十位上呢？百位呢？"

3. **对每一位独立分析**：
   - 设当前分析的位的权值为 `factor`（1, 10, 100, 1000, ...）
   - 把 n 分成三部分：高位 `high`、当前位 `cur`、低位 `low`
   - 例如 n = 31456，分析百位 (factor = 100)：high = 31, cur = 4, low = 56

4. **三种情况推导**（核心）：
   - **cur == 0**：当前位要为 1，高位只能取 0 ~ (high-1)，低位可取 0 ~ (factor-1)。总贡献 = `high * factor`
   - **cur == 1**：高位取 0 ~ (high-1) 时低位任意 → `high * factor`；高位取 high 时低位只能取 0 ~ low → `low + 1`。总贡献 = `high * factor + low + 1`
   - **cur >= 2**：高位可取 0 ~ high，低位任意。总贡献 = `(high + 1) * factor`

5. **遍历所有位**：factor 从 1 开始，每次乘 10，直到 factor > n。把每一位的贡献累加起来就是答案。

## 核心公式详解

以 n = 31456 为例，分析**百位** (factor = 100)：

```
n = 3 1 4 5 6
        ^
    high=31  cur=4  low=56

我们要统计: 0~31456 中百位为 1 的数有多少个?
即 ??1?? 的形式, 其中:
- ?? (高位) 可取 00~31
- 百位固定为 1
- ?? (低位) 可取 00~99

由于 cur=4 >= 2:
  高位 00~30: 低位可取 00~99 → 31 * 100 = 3100 个
  高位 31:    百位最大是4 >= 2, 所以百位为1时低位可取 00~99 → 1 * 100 = 100 个
  总计: (31+1) * 100 = 3200
  即 (high+1) * factor
```

**再看 cur == 1 的例子**，n = 31156：
```
high=31, cur=1, low=56, factor=100

高位 00~30: 百位为1, 低位 00~99 → 31*100 = 3100
高位 31:    百位为1(=cur), 低位只能 00~56 → 56+1 = 57
总计: 31*100 + 56 + 1 = 3157
即 high * factor + low + 1
```

**再看 cur == 0 的例子**，n = 31056：
```
high=31, cur=0, low=56, factor=100

高位 00~30: 百位为1, 低位 00~99 → 31*100 = 3100
高位 31:    百位为0 < 1, 不可能百位取1 → 0
总计: 31*100 = 3100
即 high * factor
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 逐位数学分析 | 按位拆分，分三种情况 | O(log n) | O(1) | ⭐ 必须掌握 |
| 暴力枚举 | 遍历 1~n 每个数统计 | O(n log n) | O(1) | 理解题意用 |
| 数位 DP | 通用数位 DP 框架 | O(log n) | O(log n) | 通用但复杂 |

## 关键提示

1. **三种情况的边界**：cur == 0 / cur == 1 / cur >= 2 是本题的核心公式。理解这三种情况后，代码只有十几行。

2. **用 factor 遍历每一位**：factor = 1, 10, 100, 1000, ...，循环条件 `factor <= n`。

3. **拆分公式**：`high = n / (factor * 10)`，`cur = (n / factor) % 10`，`low = n % factor`。

4. **整数溢出**：factor * 10 可能超过 int 范围（n 可达 10^9，factor 可达 10^9，factor * 10 = 10^10）。必须用 `long long`。

5. **推广到任意数字**：如果要统计数字 d (0~9) 的出现次数，公式完全相同，只需把"cur 和 1 比较"改成"cur 和 d 比较"。但 d = 0 时需要特殊处理（high 不能从 0 开始，因为没有前导零）。

## 解法详解

### 解法1: 逐位数学分析 — O(log n) / O(1) ⭐ 面试首选

**思路**：对每一位独立计算该位上出现 1 的次数，三种情况对应三个公式，累加所有位的贡献。

```cpp
class Solution {
public:
    int countDigitOne(int n) {
        long long count = 0;
        for (long long factor = 1; factor <= n; factor *= 10) {
            long long high = n / (factor * 10);
            long long cur  = (n / factor) % 10;
            long long low  = n % factor;
            
            if (cur == 0) {
                count += high * factor;
            } else if (cur == 1) {
                count += high * factor + low + 1;
            } else {
                count += (high + 1) * factor;
            }
        }
        return (int)count;
    }
};
```

### 解法2: 暴力枚举 — O(n log n) / O(1) (仅用于验证)

```cpp
class Solution {
public:
    int countDigitOne(int n) {
        int count = 0;
        for (int i = 1; i <= n; i++) {
            int num = i;
            while (num > 0) {
                if (num % 10 == 1) count++;
                num /= 10;
            }
        }
        return count;
    }
};
```

### 解法3: 数位 DP — O(log n) / O(log n)

**思路**：通用数位 DP 框架。`dp(pos, cnt, tight)` 表示从第 pos 位开始填，已经有 cnt 个 1，是否受上界约束。这是更通用的方法，可以处理各种"统计满足某条件的数的个数"问题。

```cpp
class Solution {
    int digits[10];
    int memo[10][10][2];
    int len;
    
    // pos: 当前处理的位 (从高到低)
    // cnt: 已经出现的 1 的个数
    // tight: 当前是否受上界约束
    int dp(int pos, int cnt, bool tight) {
        if (pos == len) return cnt;
        if (memo[pos][cnt][tight] != -1) return memo[pos][cnt][tight];
        
        int limit = tight ? digits[pos] : 9;
        int result = 0;
        for (int d = 0; d <= limit; d++) {
            result += dp(pos + 1, 
                        cnt + (d == 1 ? 1 : 0),
                        tight && (d == limit));
        }
        return memo[pos][cnt][tight] = result;
    }
    
public:
    int countDigitOne(int n) {
        if (n <= 0) return 0;
        len = 0;
        int tmp = n;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        reverse(digits, digits + len);
        memset(memo, -1, sizeof(memo));
        return dp(0, 0, true);
    }
};
```

**关键点**：
- 数位 DP 是通用框架，可以扩展到"统计 0~n 中满足任意数位条件的数"
- 但对本题来说，数学方法更简洁高效
- 面试中先说数学方法，追问通用方法再说数位 DP

## 完整推导：为什么是这三个公式

设 n 的十进制表示中，当前位 (factor 位) 的数字为 cur，左边的数为 high，右边的数为 low。

我们要数"0 ~ n 中，factor 位上恰好为 1 的整数个数"。这样的数形如：`XXXXX 1 YYYYY`，其中 XXXXX 代表高位（范围决定于 cur），YYYYY 代表低位。

```
当 cur == 0: (当前位是 0，"够不到" 1)
  高位: 0, 1, 2, ..., high-1  (不能取 high，因为 high 对应的 cur=0 < 1)
  低位: 0, 1, 2, ..., factor-1 (任意)
  总数: high * factor

当 cur == 1: (当前位恰好是 1，"刚好踩线")
  高位 0 ~ high-1: 低位任意 → high * factor
  高位 = high: cur=1=1，低位只能取 0 ~ low → low + 1
  总数: high * factor + low + 1

当 cur >= 2: (当前位超过 1，"完整覆盖")
  高位: 0, 1, 2, ..., high  (可以取到 high，因为 cur >= 2 > 1)
  低位: 0, 1, 2, ..., factor-1 (任意)
  总数: (high + 1) * factor
```

## 解法对比

| | 逐位数学 ⭐ | 暴力枚举 | 数位 DP |
|---|---|---|---|
| 时间 | O(log n) | O(n log n) | O(10 * log n * log n) |
| 空间 | O(1) | O(1) | O(log^2 n) |
| 推导难度 | 中等（需理解三种情况） | 无 | 较高（通用框架） |
| 代码量 | 最少 | 最少 | 最多 |
| 面试推荐 | **首选** | 仅验证 | 追问通用方法时 |

## 易错点

1. **整数溢出**
   - `factor * 10` 在 factor = 10^9 时溢出 int！
   - 必须用 `long long factor`
   - 或者条件改为 `factor <= n / 10`（但需要单独处理最高位）

2. **n = 0 直接返回 0**
   - 循环条件 `factor <= n`，当 n = 0 时循环不执行，自然返回 0

3. **混淆 high 的范围**
   - cur == 0: 高位只能取到 high - 1（不含 high），因为 high 对应的当前位是 0
   - cur >= 2: 高位可以取到 high（含 high），因为当前位 > 1，可以"放心"取 1

4. **忘记 cur == 1 时的额外贡献**
   - `low + 1` 容易忘记 +1（低位从 0 开始算，所以 0 ~ low 共 low + 1 个）

## 面试追问

**Q1（理解公式）**：为什么要分三种情况？能不能统一成一个公式？
> 三种情况的本质是"当前位能不能取到 1"：cur=0 时取不到，cur=1 时刚好踩线（受低位限制），cur>=2 时完整覆盖。可以统一为 `count += (high + (cur > 1 ? 1 : 0)) * factor + (cur == 1 ? low + 1 : 0)`，但分开写更清晰。

**Q2（推广）**：如果要统计数字 d 的出现次数（d 可以是 0~9 任意值），怎么改？
> 把"cur 和 1 比较"改成"cur 和 d 比较"即可。但 d = 0 时需要特殊处理：高位不能从 0 开始（没有前导零），所以 high 的范围从 1 开始而不是 0。

**Q3（数位 DP）**：有没有更通用的方法处理这类"统计 0~n 中满足某条件的数"的问题？
> 数位 DP。状态 `dp(pos, cnt, tight)` 表示从第 pos 位开始，已经有 cnt 个目标数字，是否受上界限制。通用框架可以处理各种数位约束问题。

**Q4（验证）**：怎么验证你的公式是对的？
> 用暴力 O(n log n) 方法对小 n 做验证。例如 n = 13 时，暴力数出 1, 10, 11(两个), 12, 13 共 6 个 1。公式也应该给出 6。

## 相关题型

- **172. Factorial Trailing Zeroes** — 同样是数学分析问题，统计阶乘中 5 的因子个数。复用"按因子/位分析"的数学思维。
- **400. Nth Digit** — 找第 n 位数字，需要同样的"按位数分组计数"思路来定位。
- **357. Count Numbers with Unique Digits** — 数位相关的计数问题，可用数学公式或数位 DP。
- **面试题 17.06. 2出现的次数** — 和本题完全相同的思路，只是把 1 换成 2。公式中的比较对象从 1 变为 2。
