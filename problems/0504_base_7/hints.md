# 504. 七进制数

## 核心思路

**进制转换基础题**：反复对 7 取余得到每一位，再对 7 整除缩小数字，最后反转结果。注意处理负数和零的边界情况。

## 思维链

1. **理解进制转换**：任何十进制数转 k 进制，都是反复 `num % k` 得到当前最低位，`num / k` 去掉最低位
2. **负数处理**：先记录符号，取绝对值进行转换，最后加上负号
3. **零的特判**：0 的七进制就是 "0"，直接返回
4. **构建结果**：每次取余得到的是从低位到高位，需要最后反转字符串
5. **循环终止**：当 num 变为 0 时停止

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 迭代取余法 | O(log₇n) | O(log₇n) | ⭐⭐⭐⭐⭐ |
| 递归法 | O(log₇n) | O(log₇n) | ⭐⭐⭐ |

## 关键提示

- num 可能为负数，需要额外处理符号
- num = 0 需要特判，否则循环不会执行
- 取余结果是从低位到高位的顺序，需要反转
- C++ 中负数取余结果为负数（如 `-7 % 7 = 0`，`-8 % 7 = -1`），用绝对值避免

## 解法详解

### 解法1: 迭代取余法（推荐）

**思路**：反复对 7 取余和整除，收集每一位数字，最后反转拼接。

```cpp
class Solution {
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
```

### 解法2: 递归法

**思路**：递归地处理 `num / 7`，每层拼接当前位 `num % 7`。

```cpp
class Solution {
public:
    string convertToBase7(int num) {
        if (num < 0) return "-" + convertToBase7(-num);
        if (num < 7) return to_string(num);
        return convertToBase7(num / 7) + to_string(num % 7);
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不处理 num = 0 | ✓ `if (num == 0) return "0";` | 0 无法进入 while 循环 |
| ✗ 直接对负数取余 | ✓ 先取绝对值再转换 | C++ 负数取余可能为负 |
| ✗ 忘记反转结果 | ✓ `reverse(result.begin(), result.end())` | 取余得到的是逆序 |
| ✗ `result += (num % 7)` | ✓ `result += to_string(num % 7)` | 需要转为字符/字符串 |

## 面试追问

**Q1: 如何推广到任意进制转换？**

> 将 7 替换为参数 base 即可。如果 base > 10，需要用字母表示（如 16 进制的 A-F）。

**Q2: 如果不能用字符串反转，如何实现？**

> 可以先算出最高位的权值（7^k <= num），然后从高位到低位依次提取每一位。或者用递归法，天然从高位到低位输出。

**Q3: 如何将七进制字符串转回十进制？**

> 从左到右遍历，`result = result * 7 + digit`，即 Horner 法则。

## 相关题型

- [405. Convert a Number to Hexadecimal](https://leetcode.com/problems/convert-a-number-to-hexadecimal/) - 十六进制转换
- [171. Excel Sheet Column Number](https://leetcode.com/problems/excel-sheet-column-number/) - 26 进制转换
- [168. Excel Sheet Column Title](https://leetcode.com/problems/excel-sheet-column-title/) - 26 进制逆转换
