# 468. 验证IP地址

## 核心思路

这道题本质上是**字符串解析 + 规则校验**：按分隔符拆分字符串，然后对每一段逐一检查是否满足 IPv4 或 IPv6 的格式规则。没有算法技巧，关键是把规则理清楚、边界处理干净。

## 思维链

1. **读完题第一反应**：这不就是按 `.` 或 `:` 分割字符串，然后逐段验证吗？→ 是的，本题没有复杂算法，就是细致的字符串处理。

2. **先判断是 IPv4 候选还是 IPv6 候选**：字符串里包含 `.` → 可能是 IPv4；包含 `:` → 可能是 IPv6；两者都有或都没有 → "Neither"。

3. **IPv4 的验证规则有哪些？**
   - 恰好 4 段（被 `.` 分割）
   - 每段是纯数字
   - 每段不能为空
   - 每段数值在 [0, 255] 范围内
   - 不能有前导零（"01" 不行，但 "0" 可以）

4. **IPv6 的验证规则有哪些？**
   - 恰好 8 段（被 `:` 分割）
   - 每段长度 1~4
   - 每段只包含十六进制字符：0-9, a-f, A-F
   - 允许前导零

5. **实现的核心难点**：不是算法层面的，而是边界情况——尾部分隔符（如 `"1.1.1.1."`）、空段（如 `"2001::85a3"`）、非法字符等。

6. **字符串分割的坑**：C++ 标准库没有直接的 `split` 函数，需要手写。注意 `"1.1.1.1."` 用某些 split 方法会丢掉尾部空段。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 分割+逐段验证 | 按分隔符 split，逐段 check 规则 | O(n) | O(n) | ⭐ 必须写出 |
| 逐字符状态机 | 不 split，遍历字符逐个判断状态转移 | O(n) | O(1) | 加分项 |

> 两种解法本质相同，区别在于是否显式 split。面试中推荐解法1（清晰易写），解法2 空间更优但代码复杂。

## 关键提示

1. **先用分隔符判断类型**：字符串中有 `.` → 走 IPv4 校验；有 `:` → 走 IPv6 校验。这样把问题分成两条独立路径。

2. **IPv4 前导零陷阱**：`"0"` 是合法的，但 `"00"`, `"01"`, `"001"` 都是非法的。判断方法：长度 > 1 且首字符为 '0'。

3. **split 的尾部空段**：`"1.1.1.1."` 分割后应该得到 5 段（最后一段为空），如果你的 split 函数吃掉了尾部空段，会误判为合法。

4. **IPv6 不需要检查数值范围**：因为最多 4 位十六进制 = 最大 FFFF = 65535，题目只要求每段 1~4 位合法十六进制字符，不需要转数值。

5. **不要忽略空段**：`"::"` 在本题中是非法的（本题不处理 IPv6 缩写），任何空段都应该返回 "Neither"。

## 解法详解

### 解法1: 分割 + 逐段验证 — O(n) / O(n) ⭐ 面试首选

**思考过程**：最自然的做法——先 split，再逐段 check。把"验证IP"这个大问题拆成"验证每一段"的小问题。

```cpp
class Solution {
public:
    string validIPAddress(string queryIP) {
        if (queryIP.find('.') != string::npos) {
            return isValidIPv4(queryIP) ? "IPv4" : "Neither";
        }
        if (queryIP.find(':') != string::npos) {
            return isValidIPv6(queryIP) ? "IPv6" : "Neither";
        }
        return "Neither";
    }
    
private:
    // 按指定分隔符拆分字符串，保留空段
    vector<string> split(const string& s, char delimiter) {
        vector<string> parts;
        string cur;
        for (char c : s) {
            if (c == delimiter) {
                parts.push_back(cur);
                cur.clear();
            } else {
                cur += c;
            }
        }
        parts.push_back(cur); // 别忘了最后一段
        return parts;
    }
    
    bool isValidIPv4(const string& ip) {
        vector<string> parts = split(ip, '.');
        if (parts.size() != 4) return false; // 必须恰好4段
        
        for (const string& part : parts) {
            if (part.empty() || part.size() > 3) return false;
            
            // 每个字符必须是数字
            for (char c : part) {
                if (!isdigit(c)) return false;
            }
            
            // 前导零检查：长度>1 且首字符为'0' → 非法
            if (part.size() > 1 && part[0] == '0') return false;
            
            // 数值范围 [0, 255]
            int val = stoi(part);
            if (val < 0 || val > 255) return false;
        }
        return true;
    }
    
    bool isValidIPv6(const string& ip) {
        vector<string> parts = split(ip, ':');
        if (parts.size() != 8) return false; // 必须恰好8段
        
        string hexChars = "0123456789abcdefABCDEF";
        for (const string& part : parts) {
            // 每段长度 1~4
            if (part.empty() || part.size() > 4) return false;
            
            // 每个字符必须是合法十六进制
            for (char c : part) {
                if (hexChars.find(c) == string::npos) return false;
            }
        }
        return true;
    }
};
```

**关键点**：
- `split` 函数在循环结束后要 `push_back(cur)` 收集最后一段
- IPv4 中 `stoi` 之前必须先检查字符合法性，否则非数字字符会导致异常
- IPv4 前导零判断要排除 `"0"` 本身（单独一个 0 是合法的）

### 解法2: 逐字符遍历（无 split）— O(n) / O(1)

**从解法1优化**：解法1 需要额外的 split 产生 O(n) 空间。我们可以用两个指针直接在原字符串上逐段定位和验证，省去 split 的空间开销。

```cpp
class Solution {
public:
    string validIPAddress(string queryIP) {
        if (queryIP.find('.') != string::npos) {
            return checkIPv4(queryIP) ? "IPv4" : "Neither";
        }
        if (queryIP.find(':') != string::npos) {
            return checkIPv6(queryIP) ? "IPv6" : "Neither";
        }
        return "Neither";
    }
    
private:
    bool checkIPv4(const string& ip) {
        int n = ip.size();
        // 不能以 '.' 开头或结尾
        if (ip[0] == '.' || ip[n - 1] == '.') return false;
        
        int segCount = 0; // 段数计数
        int i = 0;
        while (i < n) {
            int j = i;
            // 找到这一段的结束位置
            while (j < n && ip[j] != '.') j++;
            
            int len = j - i;
            if (len == 0 || len > 3) return false; // 空段或太长
            
            // 检查每个字符是数字
            for (int k = i; k < j; k++) {
                if (!isdigit(ip[k])) return false;
            }
            
            // 前导零
            if (len > 1 && ip[i] == '0') return false;
            
            // 数值范围
            int val = stoi(ip.substr(i, len));
            if (val > 255) return false;
            
            segCount++;
            i = j + 1; // 跳过分隔符
        }
        return segCount == 4;
    }
    
    bool checkIPv6(const string& ip) {
        int n = ip.size();
        if (ip[0] == ':' || ip[n - 1] == ':') return false;
        
        string hexChars = "0123456789abcdefABCDEF";
        int segCount = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && ip[j] != ':') j++;
            
            int len = j - i;
            if (len == 0 || len > 4) return false;
            
            for (int k = i; k < j; k++) {
                if (hexChars.find(ip[k]) == string::npos) return false;
            }
            
            segCount++;
            i = j + 1;
        }
        return segCount == 8;
    }
};
```

**关键点**：
- 先检查首尾不能是分隔符，避免尾部空段的遗漏
- `i = j + 1` 在最后一段后会让 `i = n + 1`，循环自然结束
- 注意 `while (i < n)` 的终止条件，如果末尾有分隔符，`segCount` 会多算

## 解法对比

| | 解法1 (split+验证) | 解法2 (逐字符) |
|---|---|---|
| 代码清晰度 | ⭐ 高，逻辑分层清楚 | 中，指针操作多 |
| 空间 | O(n) split 结果 | O(1) 额外空间 |
| 面试推荐度 | ⭐ 首选 | 追问"能否 O(1) 空间"时用 |
| 易错程度 | 低（分割后逐段检查） | 中（边界处理多） |

**面试中优先写解法1**：逻辑清晰不容易出错，面试官更看重正确性和代码组织。

## 易错点

1. **split 丢掉尾部空段**
   - ✗ 用 `istringstream >> token` 或某些 split 实现会忽略 `"1.1.1.1."` 尾部的空段，误判为只有 4 段
   - ✓ 手写 split 时，循环结束后要 `push_back(cur)` 收集最后一段

2. **IPv4 前导零判断遗漏单独的 "0"**
   - ✗ `if (part[0] == '0') return false;` → 把合法的 `"0"` 也判非法了
   - ✓ `if (part.size() > 1 && part[0] == '0') return false;` → 只有多位数以 0 开头才非法

3. **IPv4 未检查字符合法性就 stoi**
   - ✗ 直接 `stoi(part)` → 如果 part 包含非数字如 `"1e1"`，`stoi` 会把 `"1"` 解析为 1（不报错！），导致误判合法
   - ✓ 先逐字符检查 `isdigit(c)`，全部通过后再 `stoi`

4. **IPv6 忘记大小写都合法**
   - ✗ 只检查 `'a'-'f'` 忘了 `'A'-'F'`
   - ✓ 合法字符集是 `"0123456789abcdefABCDEF"`

5. **空字符串或只有分隔符的情况**
   - ✗ 没处理 `""`, `"."`, `":"` 这类输入
   - ✓ split 后检查 `parts.size()` 和每段是否为空可以自然覆盖

## 面试追问

**Q1: 你的解法时间复杂度是多少？能更快吗？**
> O(n)，n 是字符串长度。每个字符最多被访问常数次。已经是最优了——至少要读一遍输入。

**Q2: 能否不用额外空间（不 split）？**
> 可以。用双指针在原字符串上定位每一段的 start 和 end，原地验证（解法2）。空间从 O(n) 降到 O(1)。

**Q3: 如果要支持 IPv6 的缩写形式（如 `::` 代表连续的全零段），怎么改？**
> 这是实际网络编程中的需求。核心改动：
> - 允许最多出现一个 `::`（多个非法）
> - `::` 代表 1~7 个全零段，所以 split 后段数 = 8 - 被省略的段数
> - 特殊情况：`::1` (开头), `fe80::` (结尾), `::` (全零)
> 实现上需要先检查 `::` 的位置和出现次数，再动态计算期望段数。

**Q4: 如果输入量极大（每秒百万次验证），有什么工程优化？**
> - 避免字符串拷贝：用 `string_view` 替代 `substr`
> - 避免 `stoi`：手动累加数值，省去函数调用开销
> - 提前短路：长度超过 39（IPv6 最大）或 15（IPv4 最大）直接返回 Neither

## 相关题型

- **93. 复原 IP 地址** — 本题的逆问题：给定纯数字字符串，用回溯枚举所有合法的 IPv4 分割方式。复用本题的 IPv4 段验证逻辑（数值 0-255、无前导零）。
- **8. 字符串转换整数 (atoi)** — 同类型的字符串解析题，核心也是逐字符处理 + 边界条件（溢出、前导空格、正负号）。
- **65. 有效数字** — 更复杂的字符串验证题，推荐用有限状态机方法，本题也可以用状态机但 overkill。