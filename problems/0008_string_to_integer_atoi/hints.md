# 8. 字符串转换整数 (atoi)

## 核心思路

这道题本质上不是算法题，而是一道**模拟 + 边界处理**题：按照明确的规则逐字符解析字符串，核心难点在于**溢出检测**——如何在乘以 10 和加数字之前判断结果是否会超出 32 位整数范围。

## 思维链

1. **读完题第一反应**：题目已经把算法步骤列出来了（跳空格 → 读符号 → 读数字 → 截断），按步骤模拟即可。这不是"想算法"的题，而是"写对每个细节"的题。

2. **最朴素做法**：用一个指针 `i` 从左到右扫描，先跳空格，再看符号，再逐位读数字累加到结果中。时间已经是 O(n)，没有优化空间的问题。

3. **真正的难点在哪？** —— **溢出检测**。当我们累加 `result = result * 10 + digit` 时，`result * 10` 可能就已经溢出了。不能等溢出了再截断（C++ 中有符号整数溢出是未定义行为）。

4. **怎么在溢出前检测？** —— 在做 `result = result * 10 + digit` 之前，检查：
   - 如果 `result > INT_MAX / 10`，那么 `result * 10` 必然溢出
   - 如果 `result == INT_MAX / 10` 且 `digit > INT_MAX % 10`，那么加上 digit 后溢出
   - 满足以上任一条件，直接返回 `INT_MAX` 或 `INT_MIN`

5. **另一种思路：用 `long long` 存中间结果**，每步检查是否越界。代码更简单，但面试官可能追问"如果不允许用 long long 呢？"

6. **还有一种思路：有限状态机 (DFA)**，把所有可能的字符输入和状态转移画成表格，代码非常简洁且不容易遗漏。这是工程上最健壮的写法。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 (long long) | 逐字符模拟，用 long long 检测溢出 | O(n) | O(1) | 能说出即可 |
| 线性扫描 (纯 int) | 逐字符模拟，乘法前检测溢出 | O(n) | O(1) | ⭐ 必须写出 |
| 有限状态机 (DFA) | 定义状态转移表，自动处理所有情况 | O(n) | O(1) | 加分项 |

## 关键提示

1. **溢出检测的时机**：不是在 `result * 10 + digit` 之后检查，而是在**之前**检查。C++ 中有符号整数溢出是**未定义行为**，一旦发生就来不及了。

2. **INT_MAX = 2147483647, INT_MIN = -2147483648**。注意 `INT_MAX / 10 = 214748364`，`INT_MAX % 10 = 7`。当 `result == 214748364` 且下一位 `digit > 7` 时就该截断了。

3. **负数的溢出**：`-2147483648` 的绝对值比 `2147483647` 大 1。但我们可以统一用正数逻辑处理，因为当 `result > INT_MAX / 10` 或 `(result == INT_MAX / 10 && digit > 7)` 时，无论正负都该截断——负数截断到 INT_MIN，正数截断到 INT_MAX。

4. **符号只能出现一次**：`"+-12"` 应该返回 0，因为读了 `+` 后下一个字符 `-` 不是数字。

5. **前导零无影响**：`"0042"` → `result = 0*10+0 = 0, 0*10+0 = 0, 0*10+4 = 4, 4*10+2 = 42`，自然处理。

## 解法详解

### 解法1: 线性扫描 (long long) — O(n) / O(1)

**思考过程**：最直觉的做法——用 `long long` 存结果，这样中间计算不会溢出（因为输入最多 200 个字符，但实际有效数字不会太多），每步累加后检查是否越界即可。

```cpp
class Solution {
public:
    int myAtoi(string s) {
        int i = 0, n = s.size();
        
        // Step 1: 跳过前导空格
        while (i < n && s[i] == ' ') i++;
        
        // Step 2: 读取符号
        int sign = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            sign = (s[i] == '-') ? -1 : 1;
            i++;
        }
        
        // Step 3: 逐位读取数字
        long long result = 0;
        while (i < n && s[i] >= '0' && s[i] <= '9') {
            result = result * 10 + (s[i] - '0');
            
            // 每步检查是否越界，提前截断避免 long long 也溢出
            if (result * sign <= INT_MIN) return INT_MIN;
            if (result * sign >= INT_MAX) return INT_MAX;
            i++;
        }
        
        return (int)(result * sign);
    }
};
```

**关键点**：即使用了 `long long`，如果输入是 `"999...999"`（200个9），不提前截断的话 long long 也会溢出。所以**每步都要检查**，不能等循环结束再检查。

---

### 解法2: 线性扫描 (纯 int 溢出检测) — O(n) / O(1) ⭐ 面试首选

**从解法1改进**：面试官常追问"不用 long long 怎么做？" 核心技巧：在做 `result = result * 10 + digit` **之前**，检查这个操作是否会溢出。

```cpp
class Solution {
public:
    int myAtoi(string s) {
        int i = 0, n = s.size();
        
        // Step 1: 跳过前导空格
        while (i < n && s[i] == ' ') i++;
        
        // Step 2: 读取符号
        int sign = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            sign = (s[i] == '-') ? -1 : 1;
            i++;
        }
        
        // Step 3: 逐位读取数字，在乘法前检测溢出
        int result = 0;
        // INT_MAX = 2147483647, 所以 INT_MAX/10 = 214748364, INT_MAX%10 = 7
        while (i < n && s[i] >= '0' && s[i] <= '9') {
            int digit = s[i] - '0';
            
            // 关键：溢出检测必须在 result*10+digit 之前
            // 情况1: result > INT_MAX/10，则 result*10 必然 > INT_MAX
            // 情况2: result == INT_MAX/10 且 digit > 7，则 result*10+digit > INT_MAX
            if (result > INT_MAX / 10 || 
                (result == INT_MAX / 10 && digit > INT_MAX % 10)) {
                return (sign == 1) ? INT_MAX : INT_MIN;
            }
            
            result = result * 10 + digit;
            i++;
        }
        
        return result * sign;
    }
};
```

**关键点**：
- 溢出检测用的是**正数的上界** `INT_MAX`，然后根据 `sign` 决定返回 `INT_MAX` 还是 `INT_MIN`
- 为什么负数也能用正数上界？因为 `|-2147483648|` 比 `2147483647` 只大 1，当 `digit > 7` 时无论正负都该截断。如果 `digit == 8` 且 `sign == -1`，实际上 `-2147483648` 是合法的，但我们返回 `INT_MIN = -2147483648` 也是对的！

---

### 解法3: 有限状态机 (DFA) — O(n) / O(1)

**为什么要用状态机？** 前面的解法用了很多 if-else 来处理各种情况，逻辑分散在代码各处。状态机把所有逻辑浓缩到一张**状态转移表**中，代码简洁且不容易遗漏边界情况。这在工程中处理复杂解析逻辑时非常实用。

```
// 状态转移图:
//
//  状态          空格    +/-     数字    其他
//  ─────────────────────────────────────────
//  start    →   start   signed  number   end
//  signed   →   end     end     number   end
//  number   →   end     end     number   end
//  end      →   end     end     end      end
```

```cpp
class Solution {
public:
    int myAtoi(string s) {
        // 状态: 0=start, 1=signed, 2=number, 3=end
        //          space  sign  digit  other
        int table[4][4] = {
            {0, 1, 2, 3},  // start
            {3, 3, 2, 3},  // signed
            {3, 3, 2, 3},  // number
            {3, 3, 3, 3},  // end
        };
        
        int state = 0, sign = 1, result = 0;
        
        for (char c : s) {
            // 确定输入类型
            int input;
            if (c == ' ')                    input = 0;
            else if (c == '+' || c == '-')   input = 1;
            else if (c >= '0' && c <= '9')   input = 2;
            else                             input = 3;
            
            // 状态转移
            state = table[state][input];
            
            // 根据状态执行动作
            if (state == 1) {
                sign = (c == '-') ? -1 : 1;
            } else if (state == 2) {
                int digit = c - '0';
                if (result > INT_MAX / 10 || 
                    (result == INT_MAX / 10 && digit > INT_MAX % 10)) {
                    return (sign == 1) ? INT_MAX : INT_MIN;
                }
                result = result * 10 + digit;
            } else if (state == 3) {
                break; // 提前退出
            }
        }
        
        return result * sign;
    }
};
```

**关键点**：状态机的优势在于**可扩展性**——如果规则变了（比如允许数字中间有逗号），只需修改状态转移表，不需要改代码结构。

## 解法对比

| | 解法1 (long long) | 解法2 (纯 int) | 解法3 (DFA) |
|---|---|---|---|
| 核心思路 | 用更大类型避免溢出 | 乘法前数学检测 | 状态转移表驱动 |
| 代码复杂度 | 最简单 | 中等 | 结构最清晰 |
| 溢出处理 | 每步比较 long long | 乘法前预判 | 同解法2 |
| 面试推荐 | 可作为初版 | ⭐ 首选 | 追问时展示 |
| 可扩展性 | 差 | 差 | 好（改表即可） |

**什么时候选哪个？**
- 快速写出：选解法1，先用 long long 过掉
- 面试标准答案：选解法2，展示溢出检测能力
- 展示工程思维：提到解法3，说明"如果解析规则更复杂，用 DFA 更健壮"

## 易错点

1. **溢出检测放错位置**
   - ✗ `result = result * 10 + digit; if (result > INT_MAX) ...` — 有符号整数溢出是**未定义行为**，此时 result 的值不可预测
   - ✓ `if (result > INT_MAX / 10 || ...) return ...; result = result * 10 + digit;` — 在操作之前检测

2. **负数溢出边界差 1**
   - ✗ 认为 `digit > 7` 时负数也一定溢出 — 实际上 `-2147483648` 是合法的 `INT_MIN`
   - ✓ 但我们返回 `INT_MIN` 的值恰好就是 `-2147483648`，所以用正数上界统一检测是**巧合地正确**的

3. **符号后紧跟非数字**
   - ✗ 没处理 `"+-12"` 或 `"+abc"` 的情况
   - ✓ 读完符号后，如果下一个字符不是数字，while 循环直接不进入，result = 0，正确

4. **忘记 break 或提前返回**
   - ✗ 遇到非数字字符后继续扫描后面的数字（如 `"0-1"` 返回 -1）
   - ✓ while 条件中 `s[i] >= '0' && s[i] <= '9'` 自然在遇到 `-` 时停止

5. **long long 版本也可能溢出**
   - ✗ 在循环内不检查就继续累加（200个字符的数字 long long 也装不下）
   - ✓ 每步都检查 `result * sign` 是否越界，及时截断

## 面试追问

**Q1: 为什么不能先算出结果再截断？**
> 因为 C++ 中有符号整数溢出是未定义行为（UB），不是简单地 wrap around。编译器可能做任何事情，包括优化掉你的溢出检查。所以必须在溢出发生**之前**检测。

**Q2: 如果不允许用 long long，怎么检测溢出？**
> 在做 `result = result * 10 + digit` 之前，检查 `result > INT_MAX / 10` 或 `(result == INT_MAX / 10 && digit > 7)`。这是通过**数学不等式变换**来避免实际溢出发生。（解法2）

**Q3: 如果输入规则变得更复杂（比如允许十六进制、允许下划线分隔），你会怎么设计？**
> 用有限状态机（DFA）。把所有合法的字符类型和状态转移画成一张表，代码只需一个循环 + 查表。新增规则只需修改表，不改代码逻辑。这就是解法3的思路，也是编译器词法分析的原理。

**Q4: `INT_MIN` 的绝对值比 `INT_MAX` 大 1，你的代码能正确处理吗？**
> 能。当 `result == 214748364` 且 `digit == 8` 时，检测条件 `digit > 7` 成立，返回 `INT_MIN = -2147483648`，恰好是正确答案。如果 `digit == 7`，则 `result = 2147483647`，乘以 `sign = -1` 得到 `-2147483647`，也正确。

## 相关题型

- **7. 整数反转 (Reverse Integer)** — 复用完全相同的**溢出检测模板**：`result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)`。区别是那道题的数字来源是整数的末位而不是字符串。
- **65. 有效数字 (Valid Number)** — 本题的升级版，规则更复杂（科学计数法、小数点等），非常适合用 DFA 解法。学会本题解法3后可以直接复用状态机框架。
- **67. 二进制求和 (Add Binary)** — 复用"逐位处理 + 进位"的模式，但不需要溢出检测。