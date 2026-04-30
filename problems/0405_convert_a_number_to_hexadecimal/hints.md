# 405. Convert a Number to Hexadecimal - 数字转换为十六进制数

## 核心思路
将 32 位整数转为十六进制字符串。关键洞察：**位运算处理**，每次取最低 4 位（`num & 0xf`），映射到 '0'-'9','a'-'f'，然后右移 4 位。负数的补码表示在位运算中自然处理（转为 unsigned）。

## 思维链
1. 十六进制每位对应 4 个二进制位
2. 每次取 `num & 0xf` 得到最低 4 位的值 (0-15)
3. 映射到对应字符，右移 4 位继续处理
4. 负数处理：C++ 中将 int 转为 `unsigned int`，补码自动生效
5. 特殊情况：num=0 直接返回 "0"

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 位运算逐位提取 | O(8) = O(1) | O(1) | 最优解，位运算经典 |
| ⭐ 查表法 | O(8) = O(1) | O(1) | 用字符数组映射 |

## 关键提示
1. **负数补码**：`(unsigned int)num` 或直接位运算，C++ 对 int 右移保留符号位会出问题，转 unsigned 解决
2. **去前导零**：从低位构建字符串后反转，或者跳过高位的零
3. **最多 8 位**：32 位整数 = 8 个十六进制位，循环最多 8 次
4. **禁止用库函数**：不能用 `sprintf`、`stringstream` 等

## 解法详解

### 解法一：位运算 + 字符映射
```cpp
class Solution {
public:
    string toHex(int num) {
        if (num == 0) return "0";
        string hex = "0123456789abcdef";
        string res;
        unsigned int n = num;  // 处理负数
        while (n) {
            res = hex[n & 0xf] + res;
            n >>= 4;
        }
        return res;
    }
};
```

### 解法二：从高位到低位处理
```cpp
class Solution {
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
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `int n = num; n >>= 4;` 对负数 | `unsigned int n = num;` | 有符号右移会填充符号位，导致死循环 |
| 忘记处理 `num == 0` | 特判返回 "0" | 否则循环不执行，返回空串 |
| `res += hex[n & 0xf]` 然后不反转 | `res = hex[n & 0xf] + res` 或最后 reverse | 从低位提取，需要反转得到正确顺序 |

## 面试追问

**Q1: 为什么用 unsigned int 处理负数？**
> C++ 中有符号整数右移是算术右移（填充符号位），-1 右移仍是 -1，导致死循环。转 unsigned 后变为逻辑右移（填充 0），最终变为 0 退出循环。

**Q2: 不用 unsigned 还有什么方法？**
> 控制循环次数：固定循环 8 次（32/4=8），或用 `num & 0xf` 提取后用 `num = (unsigned int)num >> 4` 显式转换。

**Q3: 如果要实现十进制转任意进制呢？**
> 对于 base 进制，每次 `num % base` 取最低位，`num /= base`。但负数处理更复杂，需要先取绝对值再加负号。十六进制用位运算更优。

## 相关题型
- [168. Excel Sheet Column Title](https://leetcode.com/problems/excel-sheet-column-title/) - 进制转换
- [171. Excel Sheet Column Number](https://leetcode.com/problems/excel-sheet-column-number/) - 逆进制转换
- [504. Base 7](https://leetcode.com/problems/base-7/) - 七进制转换
