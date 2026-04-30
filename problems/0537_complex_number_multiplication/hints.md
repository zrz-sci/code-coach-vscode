# 537. 复数乘法

## 核心思路

解析两个复数字符串 `"a+bi"` 得到实部和虚部，然后用复数乘法公式 `(a+bi)(c+di) = (ac-bd) + (ad+bc)i` 计算结果，最后格式化输出。

## 思维链

1. 复数格式固定为 `"real+imaginaryi"`
2. 用 `+` 号分割字符串，提取实部 a 和虚部 b
3. 虚部末尾有 `i`，需要去掉再转整数
4. 套用公式：实部 = ac - bd，虚部 = ad + bc
5. 按格式拼接结果字符串

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 字符串解析 + 公式 | O(n) | O(1) | ⭐⭐⭐ |
| sscanf 解析 | O(n) | O(1) | ⭐⭐ |

## 关键提示

1. 复数乘法公式：`(a+bi)(c+di) = (ac-bd) + (ad+bc)i`
2. `i^2 = -1`，所以虚部相乘变为负实部
3. 注意虚部可能为负数，如 `"1+-1i"` 中虚部是 -1
4. 输出格式严格为 `"real+imaginaryi"`，虚部为负时自带负号

## 解法详解

### 解法一：字符串解析 + 公式（推荐）

```cpp
class Solution {
public:
    string complexNumberMultiply(string num1, string num2) {
        auto parse = [](const string& s) -> pair<int, int> {
            int plus = s.find('+');
            int real = stoi(s.substr(0, plus));
            int imag = stoi(s.substr(plus + 1, s.size() - plus - 2)); // 去掉末尾 'i'
            return {real, imag};
        };

        auto [a, b] = parse(num1);
        auto [c, d] = parse(num2);

        int realPart = a * c - b * d;
        int imagPart = a * d + b * c;

        return to_string(realPart) + "+" + to_string(imagPart) + "i";
    }
};
```

### 解法二：sscanf 解析

```cpp
class Solution {
public:
    string complexNumberMultiply(string num1, string num2) {
        int a, b, c, d;
        sscanf(num1.c_str(), "%d+%di", &a, &b);
        sscanf(num2.c_str(), "%d+%di", &c, &d);

        int realPart = a * c - b * d;
        int imagPart = a * d + b * c;

        return to_string(realPart) + "+" + to_string(imagPart) + "i";
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `s.substr(plus+1)` 直接转int | `s.substr(plus+1, len-2)` 去掉 'i' | 虚部末尾有字符 'i' |
| 实部 = a*c + b*d | 实部 = a*c - b*d | i^2 = -1，注意符号 |
| 输出 `"real+imaginaryi"` 忘记负数情况 | `to_string` 自动处理负号 | 如 `"0+-2i"` 是合法格式 |

## 面试追问

**Q1: 如果要实现复数除法呢？**
→ `(a+bi)/(c+di) = (ac+bd)/(c^2+d^2) + (bc-ad)/(c^2+d^2)i`，分子分母同乘共轭。

**Q2: 如何扩展支持复数加减乘除的完整类？**
→ 设计 Complex 类，重载 +, -, *, / 运算符，内部存 real 和 imag。

**Q3: sscanf 和手动解析各有什么优劣？**
→ sscanf 更简洁但不够灵活；手动解析更可控，适应格式变化。

## 相关题型

- [LeetCode 43. Multiply Strings](https://leetcode.com/problems/multiply-strings/) - 字符串乘法
- [LeetCode 415. Add Strings](https://leetcode.com/problems/add-strings/) - 字符串加法
- [LeetCode 67. Add Binary](https://leetcode.com/problems/add-binary/) - 二进制加法
