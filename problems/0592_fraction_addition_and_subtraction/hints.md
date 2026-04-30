# 592. 分数加减运算 (Fraction Addition and Subtraction)

## 核心思路
将字符串中的分数逐个解析出来，用**通分累加**的方式依次合并到一个累加器中，每次合并后立刻用 GCD 化简，最终输出最简分数。核心难点在于字符串解析——需要正确处理正负号和 `/` 分隔符。

## 思维链
1. 如何从字符串 `"-1/2+1/2+1/3"` 中把每个分数拆出来？
2. 注意第一个分数可能没有 `+` 前缀，但一定有 `-` 或直接是数字
3. 两个分数做加法：`a/b + c/d = (a*d + c*b) / (b*d)`
4. 每一步累加后必须化简，否则分母会指数级膨胀
5. 化简用 GCD：`g = gcd(|numerator|, denominator)`，分子分母同除
6. 最终保证分母为正数（题意已保证输入合法）
7. 结果为 0 时输出 `"0/1"`

## 解法概览
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| istringstream 流解析 | O(n) | O(1) | 最简洁，利用 C++ 流自动处理正负号 |
| 手动 index 扫描 | O(n) | O(1) | 面试可手写，更底层理解 |
| 正则表达式提取 | O(n) | O(n) | 简洁但面试不推荐 |

## 关键提示

### 提示 1 - istringstream 解析技巧
`istringstream` 的 `>>` 运算符读取 int 时会**自动跳过前导空白并处理正负号**。因此 `ss >> num >> slash >> den` 可以直接解析 `"-1/2"` 这样的格式——其中 `num = -1`，`slash = '/'`，`den = 2`。连续的 `"+1/3"` 同样可行，因为 `+` 被当作正号。

### 提示 2 - 通分公式
```
a/b + c/d = (a * d + c * b) / (b * d)
```
注意 `c` 已经携带了符号（可能为负），所以减法天然被处理了。

### 提示 3 - 化简与边界
- 用 `gcd(abs(numerator), denominator)` 化简——GCD 要求非负输入
- 累加器初始化为 `0/1` 而非 `0/0`
- 分母保证为正（题目限定分母 1~10），不需要额外处理符号

### 提示 4 - 手动解析方法
如果不用流，可以手动扫描：遇到 `+` 或 `-` 时标记分数的起始位置，用 `stoi` + `find('/')` 分别提取分子分母。注意第一个字符不是 `+`/`-` 时要补一个 `+`。

## 解法详解

### 解法一：istringstream 流解析（推荐）
```
class Solution {
public:
    string fractionAddition(string expression) {
        int numer = 0, denom = 1;
        istringstream ss(expression);
        int n, d;
        char slash;
        while (ss >> n >> slash >> d) {
            numer = numer * d + n * denom;
            denom = denom * d;
            int g = abs(__gcd(numer, denom));
            numer /= g;
            denom /= g;
        }
        return to_string(numer) + "/" + to_string(denom);
    }
};
```
**逻辑说明**：
- `ss >> n` 读取整数时自动处理 `+` / `-` 符号前缀
- `>> slash` 吞掉 `/` 字符
- `>> d` 读取分母
- 循环自动在流耗尽时结束

### 解法二：手动索引扫描
```
class Solution {
public:
    string fractionAddition(string expression) {
        int numer = 0, denom = 1;
        int i = 0, len = expression.size();
        while (i < len) {
            // 提取符号
            int sign = 1;
            if (expression[i] == '+' || expression[i] == '-') {
                sign = (expression[i] == '-') ? -1 : 1;
                i++;
            }
            // 提取分子
            int j = i;
            while (j < len && expression[j] != '/') j++;
            int n = sign * stoi(expression.substr(i, j - i));
            i = j + 1; // 跳过 '/'
            // 提取分母
            j = i;
            while (j < len && expression[j] != '+' && expression[j] != '-') j++;
            int d = stoi(expression.substr(i, j - i));
            i = j;
            // 通分累加
            numer = numer * d + n * denom;
            denom = denom * d;
            int g = abs(__gcd(numer, denom));
            numer /= g;
            denom /= g;
        }
        return to_string(numer) + "/" + to_string(denom);
    }
};
```

## 易错点
1. **忘记处理第一个分数的符号**：第一个分数如 `"1/2"` 没有 `+` 前缀，手动解析时容易漏掉
2. **GCD 传入负数**：`__gcd(-6, 4)` 的行为不统一，必须对分子取绝对值
3. **不化简导致溢出**：分母乘积 `b*d` 在多个分数累加后会爆 int，必须每步化简
4. **分子为 0 的情况**：`0/1` 化简后分母应保持 1，`gcd(0, x) = x`，结果正确
5. **混淆子序列/子串**：此题不是子串问题，但输入是完整表达式，不要遗漏任何分数

## 面试追问
1. **如果分母可以为 0 怎么处理？** - 需要加异常检测，返回错误或 "undefined"
2. **如何支持乘除法？** - 需要运算符优先级解析，可用栈或递归下降
3. **输入中有空格怎么办？** - istringstream 自动跳过空白，手动解析需显式跳过
4. **如果数值范围超出 int？** - 使用 long long，或在每步化简后检查范围
5. **如何验证输入表达式合法？** - 用有限状态机或正则表达式预校验

## 相关题型
- [224. Basic Calculator](https://leetcode.com/problems/basic-calculator/) - 含括号的表达式计算
- [227. Basic Calculator II](https://leetcode.com/problems/basic-calculator-ii/) - 含乘除的表达式计算
- [166. Fraction to Recurring Decimal](https://leetcode.com/problems/fraction-to-recurring-decimal/) - 分数转小数
- [2. Add Two Numbers](https://leetcode.com/problems/add-two-numbers/) - 链表模拟加法
- [43. Multiply Strings](https://leetcode.com/problems/multiply-strings/) - 字符串大数乘法
