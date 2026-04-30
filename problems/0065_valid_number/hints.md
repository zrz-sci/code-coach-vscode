# 65. 有效数字 (Valid Number)

## 难度: Hard
## 标签: String

---

## 题目理解

给定一个字符串 s，判断它是否是一个**有效数字**。有效数字的定义非常精确：

- **整数**: 可选符号(+/-) + 一个或多个数字
- **小数**: 可选符号(+/-) + 以下三种之一：
  - 数字 + 点 (如 "4.")
  - 数字 + 点 + 数字 (如 "3.14")
  - 点 + 数字 (如 ".9")
- **有效数字** = (整数 | 小数) + 可选指数部分
- **指数部分** = (e|E) + 整数

关键测试用例理解：
- `"4."` -> true (小数点后可以没有数字)
- `".9"` -> true (小数点前可以没有数字)
- `"."` -> false (小数点前后至少要有一边有数字)
- `"e3"` -> false (e 前面必须有数字/小数)
- `"1e"` -> false (e 后面必须有整数)
- `"99e2.5"` -> false (e 后面不能是小数，必须是整数)

---

## 核心思路

这道题的关键在于**精确理解语法规则**，然后选择合适的解析策略。主流有两种方法：

1. **标志位扫描法**: 用 seenDigit、seenDot、seenE 三个布尔标志追踪状态，逐字符判断合法性
2. **DFA 确定性有限状态自动机**: 明确定义每个状态和转移规则，是更系统化的方法

两者时间空间都是 O(n)/O(1)，面试中标志位法更容易快速写对，DFA 法更能展示计算机科学功底。

---

## 思维链 (从观察到解法)

### 第一步: 分析语法结构
```
有效数字 → (整数 | 小数) [指数部分]
整数     → [+|-] 数字+
小数     → [+|-] (数字+ . | 数字+ . 数字+ | . 数字+)
指数部分 → (e|E) 整数
```

### 第二步: 提取关键约束
1. 符号(+/-)只能出现在**开头**或者 **e/E 后面**
2. 小数点最多出现一次，且**不能出现在 e 后面**
3. e/E 最多出现一次，且**前面必须有数字**
4. e/E 后面必须跟**整数**(可以带符号，但不能是小数)
5. 整个字符串至少要有一个数字

### 第三步: 选择解析策略
- 标志位法：用 seenDigit 追踪是否见过数字，遇到 e 时重置(因为 e 后面还需要数字)
- DFA 法：画出完整状态转移图，定义 9 个状态

### 第四步: 确定终态
- 标志位法：最终 seenDigit == true
- DFA 法：状态 2(整数)、3(整数后接点)、5(小数)、8(e后整数) 是合法终态

---

## 解法表
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| Solution1: 标志位扫描 | O(n) | O(1) | 简洁实用，面试首选 |
| Solution2: DFA 状态机 | O(n) | O(1) | 系统化，展示 CS 功底 |
| Solution3: 分段解析法 | O(n) | O(1) | 用 e 分割后分别验证 |

---

## 解法详解

### Solution1: 标志位扫描

**核心思想**: 用三个布尔标志 seenDigit, seenDot, seenE 记录扫描过程中的状态，对每种字符类型制定规则。

**规则清单**:
- **数字**: 标记 seenDigit = true
- **+/-**: 只能在位置 0 或紧跟 e/E 之后
- **.**: 不能重复，不能在 e 之后
- **e/E**: 不能重复，前面必须有数字；出现后 seenDigit 重置为 false
- **其他字符**: 直接返回 false

**seenDigit 重置的精妙之处**: 遇到 e 时把 seenDigit 设为 false，这样最终检查 seenDigit 时就能同时确保 e 后面有数字。

```
扫描 "-123.456e789":
  '-' -> i=0, 允许
  '1' -> seenDigit=true
  '2' -> seenDigit=true
  '3' -> seenDigit=true
  '.' -> seenDot=true
  '4' -> seenDigit=true
  '5' -> seenDigit=true
  '6' -> seenDigit=true
  'e' -> seenE=true, seenDigit=false(重置!)
  '7' -> seenDigit=true
  '8' -> seenDigit=true
  '9' -> seenDigit=true
  最终 seenDigit=true -> 合法
```

```cpp
class Solution1 {
public:
    bool isNumber(string s) {
        bool seenDigit = false, seenDot = false, seenE = false;
        for (int i = 0; i < s.size(); i++) {
            char c = s[i];
            if (isdigit(c)) {
                seenDigit = true;
            } else if (c == '+' || c == '-') {
                if (i > 0 && s[i-1] != 'e' && s[i-1] != 'E') return false;
            } else if (c == '.') {
                if (seenDot || seenE) return false;
                seenDot = true;
            } else if (c == 'e' || c == 'E') {
                if (seenE || !seenDigit) return false;
                seenE = true;
                seenDigit = false;
            } else {
                return false;
            }
        }
        return seenDigit;
    }
};
```

---

### Solution2: DFA 确定性有限状态自动机

**核心思想**: 定义 9 个状态，明确每种字符的状态转移规则。

**状态定义**:
```
State 0: 起始状态
State 1: 已读符号(+/-)
State 2: 已读整数部分的数字
State 3: 整数后接点(如 "3.")
State 4: 符号后直接接点，或起始直接接点(如 "+." 的中间态)
State 5: 小数部分有数字(如 "3.14" 或 ".9")
State 6: 已读 e/E
State 7: e/E 后接符号
State 8: e/E 后接数字(指数部分完成)
```

**状态转移表**:
```
          digit    +/-     .      e/E    other
State 0:   2       1      4       -       -
State 1:   2       -      4       -       -
State 2:   2       -      3       6       -
State 3:   5       -      -       6       -
State 4:   5       -      -       -       -
State 5:   5       -      -       6       -
State 6:   8       7      -       -       -
State 7:   8       -      -       -       -
State 8:   8       -      -       -       -
("-" 表示非法转移，返回 false)
```

**合法终态**: {2, 3, 5, 8}
- State 2: 纯整数 (如 "123")
- State 3: 整数后接点 (如 "4.")
- State 5: 完整小数 (如 ".9", "3.14")
- State 8: 指数部分完成 (如 "2e10")

```cpp
class Solution2 {
public:
    bool isNumber(string s) {
        int state = 0;
        for (char c : s) {
            if (isdigit(c)) {
                if (state == 0 || state == 1 || state == 2) state = 2;
                else if (state == 3 || state == 4 || state == 5) state = 5;
                else if (state == 6 || state == 7 || state == 8) state = 8;
                else return false;
            } else if (c == '+' || c == '-') {
                if (state == 0) state = 1;
                else if (state == 6) state = 7;
                else return false;
            } else if (c == '.') {
                if (state == 0 || state == 1) state = 4;
                else if (state == 2) state = 3;
                else return false;
            } else if (c == 'e' || c == 'E') {
                if (state == 2 || state == 3 || state == 5) state = 6;
                else return false;
            } else return false;
        }
        return state == 2 || state == 3 || state == 5 || state == 8;
    }
};
```

---

### Solution3: 分段解析法

**核心思想**: 先按 e/E 分割，前半部分验证是否为合法整数或小数，后半部分(如果有)验证是否为合法整数。

```cpp
class Solution3 {
public:
    bool isNumber(string s) {
        // 按 e/E 分割
        int ePos = -1;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == 'e' || s[i] == 'E') {
                if (ePos != -1) return false; // 多个 e
                ePos = i;
            }
        }
        if (ePos == -1) {
            return isDecimalOrInteger(s);
        } else {
            string before = s.substr(0, ePos);
            string after = s.substr(ePos + 1);
            return isDecimalOrInteger(before) && isInteger(after);
        }
    }

private:
    bool isInteger(const string& s) {
        if (s.empty()) return false;
        int start = 0;
        if (s[0] == '+' || s[0] == '-') start = 1;
        if (start == (int)s.size()) return false;
        for (int i = start; i < s.size(); i++) {
            if (!isdigit(s[i])) return false;
        }
        return true;
    }

    bool isDecimalOrInteger(const string& s) {
        if (s.empty()) return false;
        int start = 0;
        if (s[0] == '+' || s[0] == '-') start = 1;
        bool hasDot = false, hasDigit = false;
        for (int i = start; i < s.size(); i++) {
            if (isdigit(s[i])) hasDigit = true;
            else if (s[i] == '.') {
                if (hasDot) return false;
                hasDot = true;
            } else return false;
        }
        return hasDigit;
    }
};
```

---

## 易错点

1. **"4." 和 ".9" 都合法**: 小数点的前后只要有一边有数字即可，但 "." 单独不行
2. **e 后面必须是整数**: "99e2.5" 是非法的，指数部分不能包含小数点
3. **符号位位置限制**: +/- 只能在 index 0 或紧跟 e/E 之后，"1-2" 不合法
4. **seenDigit 重置**: 遇到 e 后 seenDigit 必须重置为 false，否则 "1e" 会被误判为合法
5. **非法字符提前退出**: 遇到字母(非e/E)等非法字符要立即返回 false
6. **空字符串**: 虽然约束说 length >= 1，但仍需注意 e 后面子串可能为空

---

## DFA 状态转移图 (ASCII 可视化)

```
         digit         digit        digit
  +--> [State 2] --.-> [State 3] -d-> [State 5]
  |      ^  |        \                  |
  |      |  |digit    \e/E         e/E  |
  |      +--+          \  +--------+    |
  |                     v  v            v
[State 0] --+/-- > [State 1]       [State 6] --+/-- > [State 7]
  |                                  |   ^               |
  |    .                        digit|   |          digit|
  +-------> [State 4] --digit-> [State 5]          [State 8]
            (需要后续数字)                              ^  |
                                                       +--+digit
合法终态: {2, 3, 5, 8}
```

---

## 面试追问递进链

1. **"用正则怎么写？"**
   -> `^[+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$`
   -> 解释: 可选符号 + (整数可选点可选小数 | 点后小数) + 可选指数

2. **"DFA 有多少状态？状态转移表能画出来吗？"**
   -> 9 个状态 (0-8)，合法终态是 {2, 3, 5, 8}
   -> 能画出完整的 5x9 转移表

3. **"如果允许首尾空格怎么办？"**
   -> 先 trim 首尾空格，或者在 DFA 中增加 "空格" 输入类型和对应状态

4. **"如何扩展支持十六进制 (0x...)？"**
   -> 检测 "0x" 前缀后切换到十六进制模式，允许 a-fA-F 字符

5. **"如何用这个验证器做错误提示？"**
   -> 在每个 return false 处记录错误位置和原因，提供用户友好的提示信息

---

## 相关题型

- [8. String to Integer (atoi)](../0008_string_to_integer_atoi/) - 字符串解析，类似的边界条件处理
- [10. Regular Expression Matching](../0010_regular_expression_matching/) - 状态机思想
- [44. Wildcard Matching](../0044_wildcard_matching/) - 模式匹配
