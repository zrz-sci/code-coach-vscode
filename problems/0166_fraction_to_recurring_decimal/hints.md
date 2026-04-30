# 166. 分数到小数

## 核心思路
本质是模拟长除法过程，用哈希表记录每个余数第一次出现的位置来检测循环节——当同一个余数再次出现时，说明小数开始循环。

## 思维链
1. **回忆手算除法**：从小学的竖式除法出发。商的整数部分 = numerator / denominator，余数 = numerator % denominator。
2. **小数部分**：余数 * 10 后继续除，这就是长除法的本质。
3. **循环检测**：如果某个余数之前出现过，说明从那个位置开始会重复。用哈希表 remainder -> position 记录。
4. **符号处理**：正负号单独处理，计算过程用绝对值。
5. **溢出处理**：numerator = INT_MIN 时取绝对值会溢出，需用 long long。
6. **整除情况**：如果余数为 0，直接返回，没有小数部分或小数部分有限。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 模拟长除法 + 哈希表 | 余数循环检测 | O(d) | O(d) | ⭐ 必知 |

其中 d 是小数部分的长度（循环节长度），保证 < 10^4。

## 关键提示
1. **循环的本质**：长除法中，余数的取值范围是 [0, denominator-1]，最多 denominator 个不同余数。一旦余数重复，后面的除法结果一定重复。
2. **用 long long**：-2^31 取绝对值会溢出 int（因为 2^31 > INT_MAX）。
3. **符号处理**：异号为负。用 XOR：`(numerator < 0) ^ (denominator < 0)`。
4. **余数为 0 就停止**：说明小数是有限的。
5. **括号插入**：当检测到重复余数时，在哈希表记录的位置插入 '('，末尾加 ')'。

```
长除法示例: 4 / 333

  4 / 333 = 0 余 4

  小数部分:
  余数 4 → 40 / 333 = 0 余 40   → 记录 {4 → 位置0}
  余数 40 → 400 / 333 = 1 余 67  → 记录 {40 → 位置1}
  余数 67 → 670 / 333 = 2 余 4   → 记录 {67 → 位置2}
  余数 4 → 已经在位置0出现过!

  小数串: "012", 循环从位置0开始
  结果: "0.(012)"
```

## 解法详解

### 解法1: 模拟长除法 + 哈希表

**思路过程**：
- 先处理符号和整数部分
- 然后模拟长除法：每次 remainder * 10 / denominator 得到一位小数
- 用 unordered_map 记录每个余数 -> 它对应的小数位置
- 当余数重复时，在对应位置插入括号

```cpp
class Solution1 {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";
        
        string result;
        
        // 处理符号：异号为负
        if ((numerator < 0) ^ (denominator < 0)) {
            result += '-';
        }
        
        // 用 long long 防止 INT_MIN 取绝对值溢出
        long long num = llabs((long long)numerator);
        long long den = llabs((long long)denominator);
        
        // 整数部分
        result += to_string(num / den);
        long long remainder = num % den;
        
        if (remainder == 0) return result;  // 整除
        
        result += '.';
        
        // 余数 -> 该余数第一次出现时在 result 中的位置
        unordered_map<long long, int> remainderPos;
        
        while (remainder != 0) {
            // 检查余数是否出现过
            if (remainderPos.count(remainder)) {
                // 在循环起始位置插入 '('
                result.insert(remainderPos[remainder], "(");
                result += ')';
                return result;
            }
            
            // 记录当前余数和对应位置
            remainderPos[remainder] = result.size();
            
            // 模拟长除法的一步
            remainder *= 10;
            result += to_string(remainder / den);
            remainder %= den;
        }
        
        return result;  // 有限小数
    }
};
```

**复杂度分析**：
- 时间 O(d)：d 是小数长度，最多为 denominator（余数种类数）
- 空间 O(d)：哈希表存储余数位置

### 解法2: 相同逻辑，用 StringBuilder 风格 (更清晰)

**思路过程**：
- 将小数部分单独构建，最后拼接
- 逻辑更清晰，但本质完全一样

```cpp
class Solution2 {
public:
    string fractionToDecimal(int numerator, int denominator) {
        if (numerator == 0) return "0";
        
        string ans;
        
        // 符号处理
        bool negative = (numerator < 0) ^ (denominator < 0);
        if (negative) ans += '-';
        
        long long num = llabs((long long)numerator);
        long long den = llabs((long long)denominator);
        
        // 整数部分
        ans += to_string(num / den);
        long long rem = num % den;
        if (rem == 0) return ans;
        
        ans += '.';
        
        // 小数部分
        string decimal;
        unordered_map<long long, int> seen;  // 余数 → 小数串中的位置
        
        while (rem != 0) {
            if (seen.count(rem)) {
                // 找到循环节
                int start = seen[rem];
                // 在循环开始处插入括号
                ans += decimal.substr(0, start);
                ans += '(';
                ans += decimal.substr(start);
                ans += ')';
                return ans;
            }
            
            seen[rem] = decimal.size();
            rem *= 10;
            decimal += to_string(rem / den);
            rem %= den;
        }
        
        ans += decimal;
        return ans;
    }
};
```

## 易错点

1. **INT_MIN 取绝对值溢出**
```
✗ 错误：long long num = abs(numerator);
  // abs(INT_MIN) 溢出 int

✓ 正确：long long num = llabs((long long)numerator);
  // 先转 long long 再取绝对值
```

2. **忘记处理 numerator == 0**
```
✗ 错误：没有特判 0/5 → 输出 "-0" 或其他错误
✓ 正确：if (numerator == 0) return "0";
```

3. **符号判断逻辑错误**
```
✗ 错误：if (numerator < 0 || denominator < 0)
  // 同号负数会误判

✓ 正确：if ((numerator < 0) ^ (denominator < 0))
  // XOR：异号才为负
```

4. **余数用 int 导致溢出**
```
✗ 错误：int remainder = num % den; remainder *= 10;
  // remainder*10 可能溢出 int

✓ 正确：long long remainder = ...; remainder *= 10;
```

## 面试追问

**Q1: 循环节的最大长度是多少？**
A: 最多为 denominator - 1。因为余数取值范围为 [1, denominator-1]，最多这么多个不同余数。

**Q2: 如何证明有理数的小数表示一定有限或循环？**
A: 长除法的余数集合有限（[0, den-1]），所以要么余数变为 0（有限），要么某个余数重复（循环）。这就是鸽巢原理。

**Q3: 如果要求反过来——把循环小数转回分数呢？**
A: 设循环节长度为 k，非循环部分长度为 m。用代数方法：设 x = 0.abc(def)，则 10^m * x - 10^(m+k) * x 可以消去循环部分。

## 相关题型
- [2. 两数相加](../0002_add_two_numbers/) - 模拟运算 + 进位处理
- [43. 字符串相乘](../0043_multiply_strings/) - 模拟乘法
- [29. 两数相除](../0029_divide_two_integers/) - 除法模拟，溢出处理
