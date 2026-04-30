# 273. 整数转换英文表示

## 核心思路

这道题本质上是一个**分治/递归**问题：英语中数字的读法天然按照每三位一组（Billion、Million、Thousand）划分，每组内部的处理逻辑完全相同（百位 + 十位 + 个位），因此只需要写好"三位数以内怎么转英文"，然后按千、百万、十亿分段拼接即可。

## 思维链

1. **读完题第一反应**：英文数字的读法有规律吗？想想我们怎么读 `1,234,567,891` — "One Billion, Two Hundred Thirty Four Million, Five Hundred Sixty Seven Thousand, Eight Hundred Ninety One"。每三位一组！

2. **发现分段规律**：英语数字按 1000 为单位分段，每一段加一个后缀（Thousand / Million / Billion）。在每个三位段内部，读法都是 "X Hundred Y"，其中 Y 是一个两位数的读法。

3. **两位数怎么处理？** 这是关键细节：
   - 1-9：直接查表（One, Two, ..., Nine）
   - 10-19：特殊！必须单独查表（Ten, Eleven, ..., Nineteen）
   - 20-99：十位查表（Twenty, Thirty, ...）+ 个位查表

4. **递归结构自然浮现**：`numberToWords(num)` 可以递归处理——先处理最高的千位段，加上后缀，然后递归处理剩下的部分。或者迭代地每次取 `num % 1000`。

5. **边界情况**：`num = 0` 要特殊处理，返回 `"Zero"`（这是唯一会输出 Zero 的场景，递归中间遇到 0 是跳过的）。

6. **空格处理**：拼接字符串时多余的空格是最常见的 bug 来源，需要仔细处理。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 迭代分段 | 每次取 num%1000，用辅助函数转三位数 | O(1) | O(1) | ⭐ 必须写出 |
| 递归分段 | 递归处理 Billion→Million→Thousand→个位 | O(1) | O(1) | ⭐ 必须写出 |

> 注：因为 num ≤ 2^31 - 1 ≈ 2.1×10^9，最多只有 4 个三位段，所以时间和空间都是常数级的。

## 关键提示

1. **三位一组是核心**：英语数字的读法按 `Billion (10^9)` → `Million (10^6)` → `Thousand (10^3)` 分段，每段内部处理逻辑一样。

2. **10-19 是特殊区间**：不能拆成十位+个位，必须整体查表。这是最容易出 bug 的地方。

3. **辅助函数只需要处理 0-999**：写一个 `threeDigits(num)` 函数，处理百位、十位、个位。

4. **空格陷阱**：当某一段是 0 时（如 1,000,000），不应该输出 "Million"，需要跳过。拼接时前后多余空格要清理。

5. **num=0 是唯一的特殊情况**：递归/迭代中 0 是"什么都不输出"，只有输入本身是 0 时才输出 "Zero"。

```
数字分段示意图:
  2,147,483,647  (2^31 - 1)
  ↓
  [2] Billion [147] Million [483] Thousand [647]
   ↓             ↓              ↓            ↓
  "Two"    "One Hundred    "Four Hundred  "Six Hundred
            Forty Seven"   Eighty Three"  Forty Seven"
```

## 解法详解

### 解法1: 迭代分段 — O(1) / O(1) ⭐ 面试首选

**思考过程**：既然英文数字按千位分段，我们就从低位到高位，每次取 `num % 1000` 处理一个三位段，加上对应的后缀（"", "Thousand", "Million", "Billion"），然后 `num /= 1000`。

核心在于写好辅助函数 `helper(num)`：把 0-999 的数字转成英文字符串。

```cpp
class Solution {
public:
    string numberToWords(int num) {
        if (num == 0) return "Zero";
        
        // 1-19 的英文（下标0不用）
        vector<string> below20 = {
            "", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
            "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
            "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
        };
        // 整十的英文（下标0,1不用）
        vector<string> tens = {
            "", "", "Twenty", "Thirty", "Forty", "Fifty",
            "Sixty", "Seventy", "Eighty", "Ninety"
        };
        // 千位后缀，从低到高
        vector<string> thousands = {"", "Thousand", "Million", "Billion"};
        
        string result;
        int i = 0; // 当前是第几个千位段
        
        while (num > 0) {
            int chunk = num % 1000; // 取最低三位
            if (chunk != 0) {
                // 转换这三位，加上千位后缀
                string part = helper(chunk, below20, tens);
                if (!thousands[i].empty()) {
                    part += " " + thousands[i];
                }
                // 拼到结果前面（因为从低位开始处理）
                if (result.empty()) {
                    result = part;
                } else {
                    result = part + " " + result;
                }
            }
            num /= 1000;
            i++;
        }
        
        return result;
    }
    
private:
    // 把 1-999 的数转成英文（保证 num > 0）
    string helper(int num, vector<string>& below20, vector<string>& tens) {
        if (num == 0) return "";
        
        string result;
        
        // 百位
        if (num >= 100) {
            result += below20[num / 100] + " Hundred";
            num %= 100;
        }
        
        // 十位和个位
        if (num >= 20) {
            if (!result.empty()) result += " ";
            result += tens[num / 10];
            if (num % 10 != 0) {
                result += " " + below20[num % 10];
            }
        } else if (num > 0) {
            // 1-19 直接查表
            if (!result.empty()) result += " ";
            result += below20[num];
        }
        
        return result;
    }
};
```

**关键点**：
- `chunk != 0` 的判断确保不会输出空的段（如 1000000 不会输出多余的 Thousand）
- 从低位到高位处理，结果需要拼到前面
- helper 内部对 1-19 整体查表，避免拆分 10-19

### 解法2: 递归分段 — O(1) / O(1)

**从解法1优化**：换一个视角，从高位到低位递归。对于一个数 num，先检查它是否 >= Billion，是则递归处理 `num / 1000000000`，加上 "Billion"，再递归处理余数。这样代码更简洁，不需要反转拼接顺序。

```cpp
class Solution {
public:
    string numberToWords(int num) {
        if (num == 0) return "Zero";
        return helper(num);
    }
    
private:
    vector<string> below20 = {
        "", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
        "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
        "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
    };
    vector<string> tens = {
        "", "", "Twenty", "Thirty", "Forty", "Fifty",
        "Sixty", "Seventy", "Eighty", "Ninety"
    };
    
    // 递归处理任意正整数，返回英文（不含前后空格）
    string helper(int num) {
        if (num == 0) return "";
        
        string result;
        
        if (num >= 1000000000) {
            // Billion 级别
            result = helper(num / 1000000000) + " Billion";
            int rem = num % 1000000000;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 1000000) {
            result = helper(num / 1000000) + " Million";
            int rem = num % 1000000;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 1000) {
            result = helper(num / 1000) + " Thousand";
            int rem = num % 1000;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 100) {
            result = below20[num / 100] + " Hundred";
            int rem = num % 100;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 20) {
            result = tens[num / 10];
            if (num % 10 > 0) result += " " + below20[num % 10];
        } else {
            // 1-19 直接查表
            result = below20[num];
        }
        
        return result;
    }
};
```

**关键点**：
- 递归天然从高位到低位，不需要反转
- 每层只处理一个量级，剩余部分递归处理
- `rem > 0` 的判断避免了多余空格和空串拼接

## 解法对比

| | 迭代分段 | 递归分段 |
|---|---|---|
| 处理方向 | 低位→高位（需要前插拼接） | 高位→低位（自然顺序） |
| 代码量 | 稍多（需要反向拼接） | 更简洁 |
| 思维难度 | 略低（循环直觉） | 略高（递归思维） |
| 面试推荐 | ✓ 都可以 | ✓ 都可以 |

两种解法本质相同，只是遍历方向不同。面试中哪种先想到就用哪种。

## 易错点

1. **忘记 num=0 的特殊处理**
   - ✗ 直接进入循环/递归，`num=0` 时返回空字符串 `""`
   - ✓ 在入口处特判 `if (num == 0) return "Zero";`

2. **10-19 被错误拆分**
   - ✗ 把 15 拆成 `tens[1] + below20[5]` → "Ten Five"
   - ✓ 当 `num < 20` 时整体查 `below20[num]` → "Fifteen"

3. **某三位段为 0 时仍输出后缀**
   - ✗ 1000000 → "One Million **Thousand**"（因为千位段是 000 但仍输出了 Thousand）
   - ✓ `if (chunk != 0)` 才拼接后缀

4. **多余空格**
   - ✗ "One Hundred  Twenty"（百位和十位之间两个空格）
   - ✓ 每次拼接时精确控制空格，或最后用 trim 清理

5. **below20 数组大小错误**
   - ✗ 数组只到 below20[9]，没包含 10-19
   - ✓ below20 大小应为 20，下标 0-19

## 面试追问

**Q1（基础理解）**：为什么按三位分段？
> 因为英语数字系统以千为单位（Thousand, Million, Billion），每三位的读法规则完全相同，只需加不同后缀。

**Q2（细节追问）**：如果要支持负数怎么办？
> 先判断 `num < 0`，输出 "Negative "，然后对 `abs(num)` 处理。注意 INT_MIN 的绝对值溢出，需要用 `long` 或先处理最高位段。

**Q3（变体）**：如果要转换为中文表示呢（一万、一亿）？
> 中文按四位分段（万、亿），每四位内部逻辑也不同（有"零"的插入规则）。核心框架相同但分段大小改为 10000，且需要处理连续零的合并。

**Q4（工程追问）**：如何增加对小数的支持？
> 整数部分用现有逻辑，小数部分逐位读出（"Point One Two Three"），或者按分数形式处理（如货币场景 "Dollars and Cents"）。

## 相关题型

- **12. 整数转罗马数字** — 同样是"分段映射"的思路，区别是罗马数字用贪心从大到小匹配，本题按千位段分治。
- **13. 罗马数字转整数** — 反向转换，练习字符串解析。
- **168. Excel表列名称** — 类似的进制转换思路（26进制 vs 1000进制），复用"取余+除法"的分段处理框架。
- **8. 字符串转换整数 (atoi)** — 反方向：英文→数字，考察字符串解析和边界处理。