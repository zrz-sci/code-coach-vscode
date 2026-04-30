# 306. 累加数 (Additive Number)

**难度**: Medium | **标签**: String, Backtracking

## 核心思路

本题的本质是**枚举前两个数 + 验证序列**。只要确定了前两个数 num1 和 num2，后面的序列就完全确定了（每个数必须等于前两个数之和）。因此核心在于：枚举所有可能的 (num1, num2) 组合，然后贪心地验证剩余部分是否能形成合法的累加序列。

关键难点在于：
1. 如何处理大数（长度可达35位，超出 long long 范围）
2. 如何正确处理前导零的约束
3. 枚举的边界条件

## 思维链

1. **观察**：累加序列一旦确定前两个数，后续所有数都唯一确定
2. **枚举**：用两层循环枚举第一个数的长度 i 和第二个数的长度 j
3. **约束**：第一个数和第二个数的长度都不能超过总长度的一半（否则第三个数无法容纳）
4. **前导零**：除了数字 "0" 本身，不允许以 '0' 开头
5. **验证**：从第三个位置开始，计算 num1 + num2 的字符串，检查是否匹配
6. **大数加法**：用字符串模拟加法，避免溢出

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 枚举 + 字符串加法 | O(n^3) | O(n) | ⭐⭐⭐⭐⭐ | 最直观、最安全的解法 |
| 枚举 + 数值计算 | O(n^2) | O(1) | ⭐⭐⭐ | 简单但有溢出风险 |

## 关键提示

1. 第一个数的长度范围：`[1, n/2]`（至少要给后面留两个数的空间）
2. 第二个数的长度范围：`[1, (n-i)/2]`（剩余空间至少要能放下第三个数）
3. `max(len1, len2) <= 剩余长度`：和一定不会比两个加数中较大的那个更短
4. 字符串加法是处理大数的标准方法，也是面试追问的重点
5. 前导零处理：`if (s.length() > 1 && s[0] == '0') return false;`

## 解法详解

### 解法一：枚举 + 字符串加法（推荐）

```cpp
class Solution {
public:
    bool isAdditiveNumber(string num) {
        int n = num.size();
        // 枚举第一个数的长度 i 和第二个数的长度 j
        for (int i = 1; i <= n / 2; i++) {
            for (int j = 1; max(i, j) <= n - i - j; j++) {
                if (check(num, i, j)) return true;
            }
        }
        return false;
    }
    
private:
    bool check(const string& num, int len1, int len2) {
        string s1 = num.substr(0, len1);
        string s2 = num.substr(len1, len2);
        // 前导零检查
        if (s1.size() > 1 && s1[0] == '0') return false;
        if (s2.size() > 1 && s2[0] == '0') return false;
        
        int start = len1 + len2;
        while (start < (int)num.size()) {
            string sum = addStrings(s1, s2);
            // 检查 num 从 start 位置开始是否以 sum 开头
            if (num.compare(start, sum.size(), sum) != 0) return false;
            start += sum.size();
            s1 = s2;
            s2 = sum;
        }
        return start == (int)num.size();
    }
    
    string addStrings(const string& a, const string& b) {
        string result;
        int carry = 0;
        int i = a.size() - 1, j = b.size() - 1;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            result.push_back('0' + sum % 10);
            carry = sum / 10;
        }
        reverse(result.begin(), result.end());
        return result;
    }
};
```

### 解法二：枚举 + 数值回溯

```cpp
class Solution {
public:
    bool isAdditiveNumber(string num) {
        int n = num.size();
        for (int i = 1; i <= n / 2; i++) {
            if (i > 1 && num[0] == '0') break; // 前导零
            for (int j = 1; max(i, j) <= n - i - j; j++) {
                if (j > 1 && num[i] == '0') break; // 前导零
                long long n1 = stoll(num.substr(0, i));
                long long n2 = stoll(num.substr(i, j));
                if (verify(num, i + j, n1, n2)) return true;
            }
        }
        return false;
    }
    
private:
    bool verify(const string& num, int start, long long n1, long long n2) {
        if (start == (int)num.size()) return true;
        long long sum = n1 + n2;
        string sumStr = to_string(sum);
        if (num.compare(start, sumStr.size(), sumStr) != 0) return false;
        return verify(num, start + sumStr.size(), n2, sum);
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `for (int i = 1; i < n; i++)` | ✓ `for (int i = 1; i <= n/2; i++)` | 第一个数长度不超过总长一半 |
| ✗ 不检查前导零 | ✓ `if (s.size() > 1 && s[0] == '0') return false` | 题目明确禁止前导零 |
| ✗ 用 `stoi/stoll` 处理大数 | ✓ 用字符串加法 | num.length 最大35位，超出 long long |
| ✗ `start < num.size()` 就返回 true | ✓ 必须 `start == num.size()` 才返回 true | 必须完整匹配整个字符串 |
| ✗ 只检查序列有3个数 | ✓ 验证到字符串末尾 | 必须消耗完所有字符 |

## 面试追问

**Q1: 如何处理大数溢出？**
→ 使用字符串加法替代数值运算。逐位相加，进位处理。这也是 LC 415 Add Strings 的标准做法。

**Q2: 时间复杂度分析？**
→ 外层两重循环枚举 O(n^2) 种 (len1, len2) 组合，内层验证每次字符串加法 O(n)，总计 O(n^3)。空间 O(n) 用于存储中间字符串。

**Q3: 如果要求输出一个合法的累加序列而不只是判断是否存在，如何修改？**
→ 在 check 函数中用一个 vector 记录每个数，找到合法序列时返回该 vector。只需修改返回类型并在验证过程中收集数字。

## 相关题型

- [415. Add Strings](https://leetcode.com/problems/add-strings/) - 字符串加法（本题的子问题）
- [842. Split Array into Fibonacci Sequence](https://leetcode.com/problems/split-array-into-fibonacci-sequence/) - 几乎相同的思路，但要求输出具体序列
- [509. Fibonacci Number](https://leetcode.com/problems/fibonacci-number/) - 斐波那契基础
- [93. Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses/) - 类似的字符串分割枚举
