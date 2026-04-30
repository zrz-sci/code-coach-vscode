# 831. 隐藏个人信息 (Masking Personal Information)

## 核心思路

本题本质上是：**判断输入是邮箱还是电话号码，然后按规则进行字符串处理和格式化**。纯字符串模拟题，关键在于正确解析和处理边界。

## 思维链

1. **读完题的第一反应**：两种格式（邮箱/电话），需要先判断类型，再分别处理。判断依据：包含 '@' 就是邮箱，否则是电话。

2. **邮箱处理**：
   - 找到 '@' 的位置，分离 name 和 domain
   - name 和 domain 全部转小写
   - name 只保留首尾字符，中间替换为 5 个 '*'

3. **电话处理**：
   - 提取所有数字字符，忽略 '+', '-', '(', ')', ' '
   - 最后 4 位保留，其余用 '*' 掩码
   - 根据国家码长度（0-3 位）选择格式

4. **电话号码的格式化规则**：
   - 10 位：`***-***-XXXX`
   - 11 位：`+*-***-***-XXXX`
   - 12 位：`+**-***-***-XXXX`
   - 13 位：`+***-***-***-XXXX`

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 直接模拟 | 判断类型 → 分别处理 | O(n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **判断邮箱 vs 电话**：只需检查字符串中是否包含 '@'。包含 → 邮箱，不包含 → 电话。

2. **邮箱处理的细节**：
   - name 即使只有 2 个字符（如 "AB"），中间也要放 5 个 '*'
   - 大小写全部转为小写（包括 domain）

3. **电话处理的细节**：
   - 先提取所有数字，得到一个纯数字字符串
   - 最后 4 位是本地号码，保留原值
   - 国家码长度 = 总位数 - 10

4. **ASCII 示意图 — 邮箱处理**：
```
输入: "LeetCode@LeetCode.com"
        ^^^^^^^^ ^^^^^^^^^^^^
        name     domain

步骤:
  1. 找 '@' → 位置 8
  2. name = "LeetCode" → 小写 "leetcode" → 首尾 "l" + "*****" + "e"
  3. domain = "LeetCode.com" → 小写 "leetcode.com"
  4. 拼接: "l*****e@leetcode.com"
```

5. **电话号码处理示意**：
```
输入: "1(234)567-890"
提取数字: "1234567890" → 10位
国家码: 0位
格式: "***-***-7890"

输入: "+86(10)12345678"
提取数字: "861012345678" → 12位
国家码: 2位 → "+**-"
格式: "+**-***-***-5678"
```

## 解法详解

### 解法1: 直接模拟 — O(n) / O(n) ⭐ 面试唯一解法

**思考过程**：

分两个子函数处理邮箱和电话。邮箱用 `find('@')` 分割。电话先过滤非数字字符，再按长度格式化。

```cpp
class Solution {
public:
    string maskPII(string s) {
        if (s.find('@') != string::npos) {
            return maskEmail(s);
        } else {
            return maskPhone(s);
        }
    }
    
private:
    string maskEmail(string& s) {
        // 全部转小写
        for (char& c : s) c = tolower(c);
        int at = s.find('@');
        string name = s.substr(0, at);
        string domain = s.substr(at); // 包含 '@'
        return string(1, name.front()) + "*****" + string(1, name.back()) + domain;
    }
    
    string maskPhone(string& s) {
        // 提取所有数字
        string digits;
        for (char c : s) {
            if (isdigit(c)) digits += c;
        }
        
        // 最后4位保留
        string last4 = digits.substr(digits.size() - 4);
        string local = "***-***-" + last4;
        
        int countryLen = digits.size() - 10;
        if (countryLen == 0) return local;
        return "+" + string(countryLen, '*') + "-" + local;
    }
};
```

**关键点**：
- `tolower` 处理大小写转换
- `isdigit` 过滤非数字字符
- 国家码长度决定前缀格式

## 易错点

1. **邮箱 name 只有 2 个字符时**：如 "AB@qq.com"，name="AB"，掩码后是 "a*****b@qq.com"（中间仍然是 5 个 '*'，不是 0 个）。

2. **忘记把 domain 也转小写**：题目要求整个邮箱地址转小写，不只是 name。

3. **电话号码中混合了各种分隔符**：`+`, `-`, `(`, `)`, ` ` 都要跳过。不能只过滤 `-`。

4. **国家码为 0 时不加 '+' 前缀**：10 位号码是本地号码，格式是 `***-***-XXXX`，没有 `+` 号。

5. **substr 的参数搞混**：`s.substr(pos, len)` vs `s.substr(pos)`（后者取到末尾）。

## 面试追问

**Q1: 如何判断输入是邮箱还是电话？**
→ 检查是否包含 '@'。题目保证输入是合法的邮箱或电话，所以这个判断是充分的。

**Q2: 如果要支持更多国家的电话号码格式呢？**
→ 需要一个"国家码 → 格式模板"的映射表。本质上是查表 + 字符串格式化。

**Q3: 如何处理 Unicode 字符（如中文名字的邮箱）？**
→ 需要用 Unicode 感知的字符串操作（如 wstring 或 ICU 库）。ASCII 的 `tolower` 不适用于 Unicode。

**Q4: 这道题的测试用例怎么设计？**
→ 边界：name 最短（2 字符）、电话恰好 10/11/12/13 位、包含各种分隔符组合、大小写混合。

## 相关题型

- **468. 验证IP地址 (Validate IP Address)** — 同样是字符串解析和格式判断。区别：468 需要验证格式正确性，本题只需要格式化输出。

- **8. 字符串转换整数 (String to Integer)** — 字符串解析 + 边界处理。复用"逐字符扫描 + 条件分支"的模式。

- **1108. IP 地址无效化 (Defanging an IP Address)** — 简单的字符串替换。本题是更复杂的"格式化 + 掩码"操作。
