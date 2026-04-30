# 1071. 字符串的最大公因子

## 核心思路

这道题本质上是把**整数 GCD（最大公约数）**的概念迁移到字符串上：如果存在一个字符串 `t` 能同时"整除"两个字符串，那么最长的 `t` 的长度一定是两个字符串长度的 GCD。

## 思维链

1. **读完题第一反应**：我需要找一个最长的字符串 `x`，它重复若干次能拼出 `str1`，也能重复若干次拼出 `str2`。那最暴力的方式是什么？——枚举所有可能的 `x`，逐个验证。

2. **暴力解怎么枚举**：`x` 一定是 `str1` 的前缀（因为 `str1` 由 `x` 重复拼成，第一段就是前缀）。`x` 的长度必须同时整除 `len1` 和 `len2`。所以我可以从最长的公因子长度往下枚举，找到第一个能同时整除两个字符串的就返回。

3. **瓶颈在哪**：暴力枚举每个候选长度，对每个长度都要验证两个字符串（线性扫描）。虽然对这道题的数据量够用，但有没有更优雅的方式？

4. **关键洞察**：如果存在公因子字符串 `x`，那 `str1 + str2` 和 `str2 + str1` 一定相等！因为 `str1 = x * a`，`str2 = x * b`，拼起来都是 `x` 重复 `a+b` 次。反过来，如果 `str1 + str2 ≠ str2 + str1`，那一定不存在公因子字符串。

5. **确定答案长度**：一旦确认存在公因子字符串，其最大长度就是 `gcd(len1, len2)`。这和整数 GCD 的性质完全对应——两个数的公因子的最大值就是它们的 GCD。

6. **最终方案**：先判断 `str1 + str2 == str2 + str1`，不等则返回空串；相等则返回 `str1` 的前 `gcd(len1, len2)` 个字符。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有公因子长度，逐个验证 | O(n·(n+m)) | O(n+m) | 能说出即可 |
| 辗转相减（递归） | 模拟整数 GCD 的辗转相减过程 | O(n+m) | O(n+m) 递归栈 | 加分项 |
| GCD + 拼接验证 | str1+str2==str2+str1 则取前 gcd(len) 个字符 | O(n+m) | O(n+m) | ⭐ 必须写出 |

## 关键提示

1. **"整除"的含义翻译**：字符串 `t` 能"整除" `s`，等价于 `len(s) % len(t) == 0` 且 `s` 由 `t` 重复 `len(s)/len(t)` 次构成。

2. **为什么 `str1 + str2 == str2 + str1` 是充要条件？** 试着用 `str1 = "ABCABC"`, `str2 = "ABC"` 验证：`"ABCABCABC" == "ABCABCABC"` ✓。再用 `str1 = "LEET"`, `str2 = "CODE"` 验证：`"LEETCODE" ≠ "CODELEET"` ✗。

3. **为什么答案长度是 gcd(len1, len2)？** 类比整数：12 和 8 的公因子有 1, 2, 4，最大是 4 = gcd(12,8)。字符串长度同理。

4. **辗转相减的直觉**：如果 `str1` 更长，去掉开头的 `str2` 部分，剩下的和 `str2` 继续找 GCD。这和 `gcd(a,b) = gcd(a-b, b)` 一模一样。

5. **易忽略的边界**：`str1 = "AAAAAB"`, `str2 = "AAA"` → 虽然长度 gcd=3，但 `str1` 不能由 `"AAA"` 的前3个字符重复构成（最后一个是 B），所以答案是空串。这正是拼接验证能帮你排除的。

## 解法详解

### 解法1: 暴力枚举 — O(n·(n+m)) / O(n+m)

**思考过程**：最直接的想法——公因子 `x` 一定是 `str1` 的前缀，长度必须同时整除两个字符串的长度。从最长的候选开始枚举（这样第一个满足的就是答案），对每个候选验证它是否能整除两个字符串。

```cpp
class Solution {
public:
    string gcdOfStrings(string str1, string str2) {
        int len1 = str1.size(), len2 = str2.size();
        // 从最长的公因子长度开始，往短了试
        for (int i = min(len1, len2); i >= 1; i--) {
            // 候选长度必须同时整除两个字符串长度
            if (len1 % i != 0 || len2 % i != 0) continue;
            
            string candidate = str1.substr(0, i);
            if (check(str1, candidate) && check(str2, candidate)) {
                return candidate;
            }
        }
        return "";
    }
    
    // 验证 s 是否由 t 重复若干次构成
    bool check(const string& s, const string& t) {
        int n = s.size(), m = t.size();
        for (int i = 0; i < n; i++) {
            if (s[i] != t[i % m]) return false;
        }
        return true;
    }
};
```

**关键点**：枚举顺序是从大到小，第一个满足条件的就是"最大公因子字符串"。如果从小到大枚举，需要记录所有满足的取最大值。

---

### 解法2: 辗转相减（递归） — O(n+m) / O(n+m)

**从整数 GCD 类比**：`gcd(a, b) = gcd(a-b, b)` 当 `a > b`。对字符串也一样：如果 `str1` 以 `str2` 开头，就去掉这个前缀，继续递归。

```
// 辗转相减示意:
// str1 = "ABABAB", str2 = "ABAB"
// 
// Round1: str1 以 str2 开头? "ABABAB" starts with "ABAB" → ✓
//         去掉前缀: str1 = "AB", str2 = "ABAB"
//         现在 str2 更长，交换角色
//
// Round2: str2 以 str1 开头? "ABAB" starts with "AB" → ✓
//         去掉前缀: str2 = "AB", str1 = "AB"
//
// Round3: str1 == str2 == "AB" → 返回 "AB"
```

```cpp
class Solution {
public:
    string gcdOfStrings(string str1, string str2) {
        // 基准情况：两个字符串相等，公因子就是自身
        if (str1 == str2) return str1;
        
        // 保证 str1 是较长的那个
        if (str1.size() < str2.size()) swap(str1, str2);
        
        // str1 必须以 str2 开头，否则不存在公因子
        if (str1.substr(0, str2.size()) != str2) return "";
        
        // 辗转相减：去掉 str1 的 str2 前缀，继续递归
        return gcdOfStrings(str1.substr(str2.size()), str2);
    }
};
```

**关键点**：这个方法的精妙之处在于它不需要提前知道 GCD 长度，递归过程自然收敛到答案。但要注意，如果开头不匹配就必须立即返回空串。

---

### 解法3: GCD + 拼接验证 — O(n+m) / O(n+m) ⭐ 面试首选

**核心洞察**：如果两个字符串有公因子字符串 `x`，那么 `str1 = x × a`，`str2 = x × b`，所以：
- `str1 + str2 = x × (a+b)`
- `str2 + str1 = x × (b+a)`

两者一定相等。**反过来**，如果 `str1 + str2 == str2 + str1`，可以证明一定存在公因子字符串，且最大长度就是 `gcd(len1, len2)`。

```
// 验证示意:
// str1 = "ABCABC" (len=6), str2 = "ABC" (len=3)
// 
// str1 + str2 = "ABCABCABC"
// str2 + str1 = "ABCABCABC"  → 相等 ✓
// 
// gcd(6, 3) = 3 → 答案 = str1[0..2] = "ABC"
//
// ---
// str1 = "LEET" (len=4), str2 = "CODE" (len=4)
// 
// str1 + str2 = "LEETCODE"
// str2 + str1 = "CODELEET"  → 不相等 ✗ → 返回 ""
```

```cpp
class Solution {
public:
    string gcdOfStrings(string str1, string str2) {
        // 拼接验证：不相等说明不存在公因子字符串
        if (str1 + str2 != str2 + str1) return "";
        
        // 存在公因子，最大长度一定是 gcd(len1, len2)
        return str1.substr(0, gcd(str1.size(), str2.size()));
    }
};
```

**为什么这就够了？** 拼接验证已经保证了"结构兼容性"——两个字符串由同一个基本单元重复构成。而 `gcd(len1, len2)` 给出最大的那个公因子长度，对应的前缀就是答案。

---

## 解法对比

| | 暴力枚举 | 辗转相减 | GCD + 拼接 |
|---|---|---|---|
| 核心思想 | 穷举验证 | 模拟辗转相减法 | 数学性质一步到位 |
| 时间 | O(n·(n+m)) | O(n+m) | O(n+m) |
| 代码量 | 中等 | 简洁但递归 | 最简洁（3行） |
| 理解难度 | 最容易 | 需要理解递归 | 需要理解为什么拼接验证是充要条件 |
| 面试推荐 | 起步展示理解 | 加分项 | **首选** |

## 易错点

1. **忘记拼接验证就直接取 GCD 长度**
   - ✗ `return str1.substr(0, gcd(len1, len2));` — 对 `"AAAAAB"` 和 `"AAA"` 会返回 `"AAA"`（错误！）
   - ✓ 先检查 `str1 + str2 == str2 + str1`，不等返回空串
   - 原因：`gcd(6,3)=3`，但 `"AAAAAB"` 根本不能由任何长度为3的字符串重复构成

2. **暴力枚举时只检查整除长度，不验证字符内容**
   - ✗ `if (len1 % i == 0 && len2 % i == 0) return str1.substr(0, i);`
   - ✓ 还需要验证候选字符串重复后确实等于 `str1` 和 `str2`

3. **辗转相减忘记处理不匹配的情况**
   - ✗ 直接 `return gcdOfStrings(str1.substr(str2.size()), str2);` 不检查前缀
   - ✓ 必须先检查 `str1` 以 `str2` 开头，否则返回 `""`

## 面试追问

**Q1: 为什么公因子字符串的长度一定整除两个字符串的长度？**
> 如果 `x` 重复 `a` 次得到 `str1`，那么 `len1 = a * len(x)`，同理 `len2 = b * len(x)`。所以 `len(x)` 是 `len1` 和 `len2` 的公因子。

**Q2: 为什么 `str1 + str2 == str2 + str1` 是存在公因子字符串的充要条件？**
> **必要性**：上面已经解释了。**充分性**：如果拼接相等，可以证明 `str1` 和 `str2` 都由 `str1[0..gcd-1]` 重复构成。证明的关键：设 `g = gcd(len1, len2)`，由拼接相等可得 `str1[i] = str1[i % g]` 对所有 `i` 成立（利用 `str1[i] = (str1+str2)[i] = (str2+str1)[i]` 反复推导位置关系）。

**Q3: 如果不用库函数 `gcd`，你怎么实现？**
> 辗转相除法：`gcd(a, b) = gcd(b, a % b)`，当 `b == 0` 时返回 `a`。时间 O(log(min(a,b)))。

**Q4: 能否不创建新字符串（避免 O(n+m) 的拼接开销）来做拼接验证？**
> 可以。不真正拼接，而是用双指针逐字符比较：比较 `str1[i] + str2[j]` 和 `str2[i] + str1[j]` 对应位置是否相等。这样空间从 O(n+m) 降到 O(1)。

## 相关题型

- **LeetCode 1979. 找出数组的最大公约数** — 直接复用整数 GCD 函数，理解 GCD 的基本概念
- **LeetCode 459. 重复的子字符串** — 判断一个字符串是否由某个子串重复构成，和本题的 `check` 函数逻辑相同；也可以用 `(s+s).find(s, 1) != s.size()` 的拼接技巧，和本题的拼接验证思想如出一辙
- **LeetCode 2185. 统计包含给定前缀的字符串** — 前缀匹配的基础练习，辗转相减法中的前缀检查可以复用