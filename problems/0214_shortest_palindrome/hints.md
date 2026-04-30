# 214. 最短回文串

## 题目理解

给定字符串 `s`，只能在**前面**添加字符使其变成回文串。求最短的回文结果。

```
示例 1: s = "aacecaaa" -> "aaacecaaa"  (前面加 "a")
示例 2: s = "abcd"     -> "dcbabcd"    (前面加 "dcb")
```

**约束**: 0 <= s.length <= 5 * 10^4，仅小写字母

---

## 核心转化：找最长回文前缀

> 只能往前面加字符 => 找 s 从索引 0 开始的**最长回文前缀** `s[0..k]`，
> 把剩余后缀 `s[k+1..n-1]` 反转拼到前面。

```
s = "aacecaaa"

从头开始试哪些前缀是回文:
  "a"        回文  (len=1)
  "aa"       回文  (len=2)
  "aac"      否
  "aace"     否
  "aacec"    否
  "aaceca"   否
  "aacecaa"  回文  (len=7)  <- 最长回文前缀!
  "aacecaaa" 否

最长回文前缀 = "aacecaa" (长度 7)
剩余后缀 = "a"
答案 = reverse("a") + s = "a" + "aacecaaa" = "aaacecaaa"
```

```
s = "abcd"
最长回文前缀 = "a" (长度 1)
答案 = reverse("bcd") + "abcd" = "dcbabcd"
```

**暴力法 O(n^2)**：从长到短枚举，逐个检查是否回文。对 5*10^4 太慢。

**问题变成：如何 O(n) 找到最长回文前缀？**

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐度 |
|------|------|------|------|--------|
| KMP failure function | 构造 s+"#"+rev(s)，求 next 数组 | O(n) | O(n) | 面试首选 |
| Rabin-Karp rolling hash | 正反向哈希比较 | O(n) | O(1) | 空间优 |
| 暴力 | 枚举 + 判断回文 | O(n^2) | O(n) | 仅分析 |

---

## 渐进提示

### 提示 1 -- 回文前缀和字符串匹配的关系

如果 `s[0..k]` 是回文，那么 `s[0..k] == reverse(s[0..k])`。能否用字符串匹配的工具来检测这个条件？

<details>
<summary>展开</summary>

构造一个巧妙的字符串：

```
combined = s + "#" + reverse(s)
```

以 s = "aacecaaa" 为例：
```
s        = "aacecaaa"
reverse  = "aaacecaa"
combined = "aacecaaa#aaacecaa"
           |---s---| |--rev--|
```

**关键洞察**：
- combined 的**前缀**来自 s 的前缀
- combined 的**后缀**来自 reverse(s) 的后缀 = s 前缀的反转
- 如果 combined 的某个前缀 == 某个后缀，就意味着 s 的某个前缀等于它自身的反转，即该前缀是回文！

**分隔符 '#' 不可省略**！防止匹配跨越边界：
```
反例: s = "aaa"
不加分隔符: combined = "aaaaaa"
KMP 最后值 = 5 > len(s)=3  (错误!)

加分隔符: combined = "aaa#aaa"
KMP 最后值 = 3 (正确)
```

</details>

### 提示 2 -- KMP failure function 的含义

KMP 的 failure function（next 数组）是什么？为什么它能解决这个问题？

<details>
<summary>展开</summary>

对字符串 `t`，`next[i]` = `t[0..i]` 的**最长"既是前缀又是后缀"的子串长度**（不含自身）。

```
例: t = "aabaa"
  t[0..0] = "a"     -> next[0] = 0 (定义)
  t[0..1] = "aa"    -> 前缀"a" = 后缀"a" -> next[1] = 1
  t[0..2] = "aab"   -> 无匹配 -> next[2] = 0
  t[0..3] = "aaba"  -> 前缀"a" = 后缀"a" -> next[3] = 1
  t[0..4] = "aabaa" -> 前缀"aa" = 后缀"aa" -> next[4] = 2
```

对于 `combined = s + "#" + reverse(s)`：
- `next` 数组的**最后一个值** = combined 的最长公共前后缀长度
- = s 的某个前缀长度，且这个前缀是回文
- 这就是**最长回文前缀的长度**！

</details>

### 提示 3 -- Failure 数组详细构建过程

以 s = "aacecaaa" 为例，逐步画出 failure 数组。

<details>
<summary>展开</summary>

```
combined = "aacecaaa#aaacecaa"
索引:       0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
```

**构建 next 数组**（j 是匹配指针）：

```
i=0: char='a', next[0] = 0 (定义)

i=1: char='a', j=next[0]=0
     combined[1]='a' == combined[0]='a' -> j=1
     next[1] = 1

i=2: char='c', j=next[1]=1
     combined[2]='c' != combined[1]='a'
     j=next[0]=0
     combined[2]='c' != combined[0]='a'
     next[2] = 0

i=3: char='e', j=0
     'e' != 'a' -> next[3] = 0

i=4: char='c', j=0
     'c' != 'a' -> next[4] = 0

i=5: char='a', j=0
     'a' == 'a' -> j=1
     next[5] = 1

i=6: char='a', j=1
     combined[6]='a' == combined[1]='a' -> j=2
     next[6] = 2

i=7: char='a', j=2
     combined[7]='a' != combined[2]='c'
     j=next[1]=1
     combined[7]='a' == combined[1]='a' -> j=2
     next[7] = 2

i=8: char='#', j=2
     '#' != combined[2]='c'
     j=next[1]=1
     '#' != combined[1]='a'
     j=next[0]=0
     '#' != combined[0]='a'
     next[8] = 0

i=9: char='a', j=0
     'a' == 'a' -> j=1
     next[9] = 1

i=10: char='a', j=1
      'a' == combined[1]='a' -> j=2
      next[10] = 2

i=11: char='a', j=2
      'a' != combined[2]='c'
      j=next[1]=1
      'a' == combined[1]='a' -> j=2
      next[11] = 2

i=12: char='c', j=2
      'c' == combined[2]='c' -> j=3
      next[12] = 3

i=13: char='e', j=3
      'e' == combined[3]='e' -> j=4
      next[13] = 4

i=14: char='c', j=4
      'c' == combined[4]='c' -> j=5
      next[14] = 5

i=15: char='a', j=5
      'a' == combined[5]='a' -> j=6
      next[15] = 6

i=16: char='a', j=6
      'a' == combined[6]='a' -> j=7
      next[16] = 7
```

**汇总 failure 数组**:

```
索引:     0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16
字符:     a  a  c  e  c  a  a  a  #  a   a   a   c   e   c   a   a
next[i]:  0  1  0  0  0  1  2  2  0  1   2   2   3   4   5   6   7
                                                                  ^
                                                       最后值 = 7
```

**最长回文前缀长度 = 7**，即 `s[0..6] = "aacecaa"` 是回文。

答案 = reverse(s[7..7]) + s = reverse("a") + "aacecaaa" = **"aaacecaaa"**

</details>

### 提示 4 -- 第二个例子验证

<details>
<summary>展开</summary>

s = "abcd"

```
combined = "abcd#dcba"

索引:     0  1  2  3  4  5  6  7  8
字符:     a  b  c  d  #  d  c  b  a
next[i]:  0  0  0  0  0  0  0  0  1
                                   ^
                        最后值 = 1
```

最长回文前缀长度 = 1（只有 "a"）。

答案 = reverse("bcd") + "abcd" = **"dcbabcd"**

</details>

### 提示 5 -- Rolling Hash 替代方案

不用 KMP，用 Rolling Hash 也能 O(n) 找最长回文前缀。

<details>
<summary>展开</summary>

同时维护正向和反向的哈希值：

```
forward_hash  = s[0]*B^i + s[1]*B^(i-1) + ... + s[i]*B^0   (正向读)
backward_hash = s[0]*B^0 + s[1]*B^1     + ... + s[i]*B^i    (反向读)
```

如果 `forward == backward`，则 `s[0..i]` 是回文。

**更新公式**：
```
forward  = forward * B + c       // 新字符加到末尾
backward = backward + c * power  // 新字符加到最高位
power    = power * B
```

**示例 s = "aacecaaa" (B=31, a=1)**:

```
i=0: c=1(a), fwd=1, bwd=1           -> 相等! longest=1 "a"
i=1: c=1(a), fwd=32, bwd=32         -> 相等! longest=2 "aa"
i=2: c=3(c), fwd=995, bwd=2915      -> 不等
i=3: c=5(e), fwd=30850, bwd=151870  -> 不等
i=4: c=3(c), fwd=956353, bwd=...    -> 不等
i=5: c=1(a), fwd=..., bwd=...       -> 不等
i=6: c=1(a), fwd=919055265, bwd=919055265 -> 相等! longest=7 "aacecaa"
i=7: c=1(a), fwd!=bwd               -> 不等

最终 longest = 7
```

**优点**: 空间 O(1)，比 KMP 省空间
**缺点**: 有哈希冲突的微小概率（可用双哈希解决）

</details>

---

## 关键思维模式

### 1. "只能往前加" --> "找最长回文前缀"

这是最核心的问题转化。回文前缀越长，需要添加的字符越少，结果越短。

### 2. KMP failure function 的非标准应用

KMP 通常用于字符串搜索。这里它被用来找"最长公共前后缀"，而不是直接做模式匹配。通过构造 `s + "#" + rev(s)`，把回文前缀问题巧妙地转化为前后缀匹配问题。

### 3. 分隔符的必要性

`#` 防止 next 数组的值超过 s 的长度。这是一个容易忘记的关键细节，面试中必须提到。

---

## 完整代码 (C++)

### 解法 1: KMP (首选)

```cpp
class Solution {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;

        string rev = s;
        reverse(rev.begin(), rev.end());
        string combined = s + "#" + rev;

        // 计算 KMP failure function
        int n = combined.size();
        vector<int> next(n, 0);

        for (int i = 1; i < n; i++) {
            int j = next[i - 1];
            while (j > 0 && combined[i] != combined[j])
                j = next[j - 1];
            if (combined[i] == combined[j])
                j++;
            next[i] = j;
        }

        // next 最后一个值 = 最长回文前缀长度
        int longestPalinPrefix = next.back();
        string suffix = s.substr(longestPalinPrefix);
        reverse(suffix.begin(), suffix.end());
        return suffix + s;
    }
};
```

### 解法 2: Rolling Hash

```cpp
class Solution {
public:
    string shortestPalindrome(string s) {
        if (s.empty()) return s;

        long long BASE = 31, MOD = 1e9 + 7;
        long long fwd = 0, bwd = 0, power = 1;
        int longest = 0;

        for (int i = 0; i < (int)s.size(); i++) {
            int c = s[i] - 'a' + 1;
            fwd = (fwd * BASE + c) % MOD;
            bwd = (bwd + (long long)c * power) % MOD;
            power = (power * BASE) % MOD;

            if (fwd == bwd)
                longest = i + 1;
        }

        string suffix = s.substr(longest);
        reverse(suffix.begin(), suffix.end());
        return suffix + s;
    }
};
```

---

## KMP failure function 的时间复杂度证明

```
虽然有嵌套的 while 循环，但整体是 O(n):

- 外层 for 循环: i 每次 +1，共 n 次
- 每次 for 中 j 最多 +1
- while 循环每次让 j 至少 -1
- j 的总增量 <= n (每次 for 最多 +1)
- j 的总减量 <= 总增量 <= n
- 所以 while 总执行次数 <= n
- 总时间 = O(n) + O(n) = O(n)

这是"摊还分析"(amortized analysis) 的经典应用。
面试中能清楚说明这一点是加分项。
```

---

## 常见错误

### 1. 忘记分隔符 '#'

```
反例: s = "aaa"
不加分隔符: combined = "aaaaaa"
next 最后值 = 5 > len(s)=3  -> 错误!
  (会认为前缀长度5可以匹配，但 s 只有3个字符)

加分隔符: combined = "aaa#aaa"
next 最后值 = 3  -> 正确!
```

### 2. 空串处理

s 为空时直接返回空字符串。否则 `reverse` 和 `substr` 可能出错。

### 3. 已是回文的情况

s = "aba" 时最长回文前缀就是整个 s，不需要添加字符，直接返回原串。

### 4. Rolling Hash 溢出

`forward * BASE` 可能溢出 int。必须用 `long long` 和取模。

### 5. KMP next 数组的写法差异

```
写法A (本题): next[0] = 0, next[i] 表示 s[0..i] 的最长公共前后缀
写法B (部分教材): next[0] = -1, 含义略有不同
面试中用哪种都可以，但要保持一致。
```

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 5 | Longest Palindromic Substring | 回文基础，中心扩展/Manacher |
| 28 | Find the Index of the First Occurrence | KMP 模板题 |
| 336 | Palindrome Pairs | 回文 + Trie/哈希 |
| 1392 | Longest Happy Prefix | KMP failure function 直接应用 |
| 516 | Longest Palindromic Subsequence | 回文 + DP |

---

## 面试追问

**Q1: 为什么分隔符 '#' 不可省略？**
> `s = "aaa"` 时，不加分隔符 combined = "aaaaaa"，next 最后值 = 5 > len(s) = 3，错误地认为前缀长度5可以匹配。加分隔符后 combined = "aaa#aaa"，next 最后值 = 3，正确。

**Q2: KMP failure function 的 while 循环为什么是 O(n)？**
> 摊还分析：外层 for 每次让 j 最多 +1，总增量 <= n；while 每次让 j 至少 -1，总减量 <= 总增量 <= n。所以 while 总执行次数 <= n，整体 O(n)。

**Q3: Rolling Hash 和 KMP 相比各有什么优劣？**
> KMP 优势：确定性正确，无哈希冲突。Rolling Hash 优势：空间 O(1)，代码更简短。面试可二选一，推荐 KMP。

**Q4: 如果允许在任意位置插入字符呢？**
> 那是不同的题目（需要 DP，类似最长回文子序列），本题约束只能在前面插入。

---

## 面试沟通策略

1. **开场**："关键洞察是找从索引 0 开始的最长回文前缀，可以用 KMP 或 Rolling Hash 做到 O(n)。"

2. **选算法**：面试官想看 KMP 就用 KMP；想要简洁实现，Rolling Hash 代码更短且空间 O(1)。

3. **必画图**：构造 `s + "#" + reverse(s)` 并手动填 next 数组是面试官最想看的推导过程。

4. **复杂度**：KMP 的 while 看起来是嵌套循环，但通过摊还分析是 O(n)。这个分析要能说清楚。

5. **Follow-up**：如果面试官问"能在任意位置插入呢？"那是不同的题目（需要 DP），本题约束只能在前面插入。
