# 28. 找出字符串中第一个匹配项的下标

## 核心思路

本质就是**字符串匹配**问题：在主串 haystack 中找模式串 needle 第一次出现的位置。暴力做法是逐个位置尝试匹配，而 KMP 算法通过预处理模式串的"自我重复"信息，在匹配失败时跳过不必要的比较。

## 思维链

1. **读完题第一反应**：最朴素的做法就是从 haystack 的每个位置 i 开始，逐字符和 needle 比对，完全匹配就返回 i。
2. **暴力解的瓶颈在哪？** 假设 haystack = "aaaaab"，needle = "aaab"。每次从新位置 i 开始都要比对 4 个字符才发现失败，然后 i 只前进 1 步。大量字符被重复比较，最坏 O(n·m)。
3. **怎么突破瓶颈？** 关键观察：当匹配到某个位置失败时，我们已经知道了 haystack 中"已匹配部分"的内容（它就是 needle 的前缀！）。能不能利用这个已知信息，让匹配指针不回退，而是让 needle 上的指针"智能跳转"？
4. **这就是 KMP 的核心思想**：预处理 needle 的 **next 数组**（也叫 failure function / 前缀函数），记录 needle 每个前缀的"最长相等前后缀长度"。匹配失败时，needle 的指针跳到 next 值指示的位置，避免重复比较。
5. **next 数组怎么构建？** 用 needle 自己和自己匹配（双指针），本质上也是一个字符串匹配的过程，只不过是 needle 匹配自身。
6. **最终**：主串指针 i 永远不回退，只有 needle 的指针 j 在失败时通过 next 数组跳转，总时间 O(n+m)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力匹配 | 逐位置尝试完整比对 | O(n·m) | O(1) | 能说出即可 |
| 内置函数 | 直接调用 `find()` / `strStr()` | O(n·m) | O(1) | ❌ 不可用 |
| KMP 算法 | 预处理 next 数组，失败智能跳转 | O(n+m) | O(m) | ⭐ 必须写出 |
| Rabin-Karp | 滚动哈希，O(1) 比较子串 | O(n+m) 期望 | O(1) | 加分项 |

> n = haystack.length, m = needle.length

## 关键提示

1. **暴力法的边界**：外层循环只需遍历到 `i <= n - m`，因为剩余长度不够 needle 时不可能匹配。
2. **KMP 的 next 数组理解**：`next[j]` 的含义是 needle[0..j-1] 这个子串中，最长的"既是前缀又是后缀"的长度。例如 needle = "abab"，next = [0,0,1,2]，因为 "ab" 既是 "abab" 的前缀又是后缀。
3. **为什么 next 数组能跳转？** 当 haystack[i] ≠ needle[j] 时，needle[0..j-1] 已经匹配成功。next[j] 告诉我们 needle 的前缀和后缀重叠了多少，所以可以直接从 next[j] 继续比较，跳过已确定匹配的部分。
4. **next 数组构建本质上是 needle 自匹配**：用两个指针 i 和 j，i 扫描 needle，j 记录当前最长匹配前缀长度，逻辑和主匹配几乎一样。
5. **Rabin-Karp 的关键**：选取一个好的哈希函数和模数，使得碰撞概率低。哈希匹配成功后还需要真实比对确认（或者接受极小概率的错误）。

```
KMP next 数组构建过程示例 (needle = "aabaaab"):
位置:  0 1 2 3 4 5 6
字符:  a a b a a a b
next: [0 1 0 1 2 2 3]

needle[0..4] = "aabaa"
  前缀: a, aa, aab, aaba
  后缀: a, aa, baa, abaa
  最长相等前后缀: "aa" → next[5 前一位] 看的是 next[4]=2

KMP 匹配过程示例:
haystack: a a b a a a b a a b
needle:   a a b a a a b
          ↑ ↑ ↑ ↑ ↑ ↑ ↑  → 完全匹配, 返回 0
```

## 解法详解

### 解法1: 暴力匹配 — O(n·m) / O(1)

**思考过程**：最直觉的做法——从 haystack 的每一个可能的起始位置 i，逐字符和 needle 比较。如果某个字符不同就放弃当前位置，尝试下一个。

```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        // 只需要遍历到 n-m，后面的起始位置长度不够
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            // 从位置 i 开始逐字符匹配 needle
            while (j < m && haystack[i + j] == needle[j]) {
                j++;
            }
            if (j == m) return i;  // needle 全部匹配成功
        }
        return -1;
    }
};
```

**关键点**：
- 外层边界是 `i <= n - m` 而不是 `i < n`，避免越界的同时也剪枝。
- 对于大多数实际场景（随机字符串），暴力法接近 O(n)，因为失败通常在前几个字符就发生了。但最坏 case（如 haystack = "aaa...a", needle = "aaa...ab"）确实是 O(n·m)。

### 解法2: KMP 算法 — O(n+m) / O(m) ⭐ 面试首选

**从解法1优化**：暴力法的瓶颈是匹配失败后 haystack 的指针要回退到 i+1 重新来。KMP 的核心洞察是：**匹配失败时，我们已经知道 haystack 中已匹配部分的内容**（它就是 needle 的一段前缀），利用这个信息让 haystack 的指针不回退。

```
// 暴力法在匹配失败时:
// haystack: ...X A B A B A C...
//               ↑ i 回退到 i+1
// needle:       A B A B A D
//                         ↑ j=5 失败
//
// KMP: 已匹配的 "ABABA" 中，最长相等前后缀是 "ABA"(长度3)
// 所以 needle 右移，让 j 跳到 3，i 不回退:
// haystack: ...X A B A B A C...
//                       ↑ i 不动
// needle:           A B A B A D
//                       ↑ j=3 继续比较
```

```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m == 0) return 0;
        
        // --- 第一步：构建 next 数组 (前缀函数) ---
        // next[j] = needle[0..j-1] 的最长相等前后缀长度
        vector<int> next(m, 0);
        // j: 前缀末尾（同时也是当前最长前后缀长度）
        // i: 后缀末尾，从 1 开始（next[0] 必定为 0）
        for (int i = 1, j = 0; i < m; i++) {
            // 不匹配时，j 回退到 next[j-1]
            // 为什么？因为 needle[0..j-1] 已匹配，其最长前后缀是 next[j-1]
            while (j > 0 && needle[i] != needle[j]) {
                j = next[j - 1];
            }
            // 匹配成功，前缀长度 +1
            if (needle[i] == needle[j]) {
                j++;
            }
            next[i] = j;
        }
        
        // --- 第二步：在 haystack 中匹配 needle ---
        // i 扫描 haystack，j 扫描 needle
        for (int i = 0, j = 0; i < n; i++) {
            // 失败时 j 通过 next 数组跳转，不回退 i
            while (j > 0 && haystack[i] != needle[j]) {
                j = next[j - 1];
            }
            if (haystack[i] == needle[j]) {
                j++;
            }
            // needle 全部匹配完成
            if (j == m) {
                return i - m + 1;  // 起始下标
            }
        }
        return -1;
    }
};
```

**关键点**：
- next 数组的构建和主匹配的代码结构**几乎一模一样**！都是"不匹配就通过 next 跳转，匹配就前进"。理解了一个就理解了另一个。
- `while (j > 0 && ...)` 中的 `j > 0` 保证 j 不会变成负数。当 j = 0 且不匹配时，只移动 i，相当于从头开始匹配。

### 解法3: Rabin-Karp (滚动哈希) — O(n+m) 期望 / O(1)

**不同角度的优化思路**：暴力法每次比较 m 个字符来判断子串是否匹配。能不能 O(1) 判断？用**哈希**！把长度为 m 的子串映射成一个数字，滑动窗口更新哈希值。

```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m > n) return -1;
        
        // 哈希参数：base 和 mod
        long long base = 26, mod = 1e9 + 7;
        
        // 计算 base^(m-1) % mod，用于滑动窗口去除最高位
        long long power = 1;
        for (int i = 0; i < m - 1; i++) {
            power = power * base % mod;
        }
        
        // 计算 needle 的哈希值和 haystack 第一个窗口的哈希值
        long long needleHash = 0, windowHash = 0;
        for (int i = 0; i < m; i++) {
            needleHash = (needleHash * base + needle[i]) % mod;
            windowHash = (windowHash * base + haystack[i]) % mod;
        }
        
        // 滑动窗口
        for (int i = 0; i <= n - m; i++) {
            if (windowHash == needleHash) {
                // 哈希匹配，还需要真实比对防止碰撞
                if (haystack.substr(i, m) == needle) {
                    return i;
                }
            }
            // 滑动窗口：去掉最左字符，加入新字符
            if (i < n - m) {
                windowHash = ((windowHash - haystack[i] * power % mod + mod) % mod 
                              * base + haystack[i + m]) % mod;
            }
        }
        return -1;
    }
};
```

**关键点**：
- 哈希碰撞后必须真实比对，否则可能误判。
- 减法时要 `+mod` 防止负数。
- 期望 O(n+m)，但最坏 O(n·m)（碰撞频繁时退化）。

## 解法对比

| | 暴力匹配 | KMP | Rabin-Karp |
|---|---|---|---|
| 时间 | O(n·m) 最坏 | O(n+m) 确定 | O(n+m) 期望 |
| 空间 | O(1) | O(m) | O(1) |
| 最坏情况 | 重复字符退化 | 依然 O(n+m) | 哈希碰撞退化 |
| 代码复杂度 | 极简单 | 中等 | 中等 |
| 面试推荐 | 入门展示 | **首选** | 讲思路即可 |

**什么时候选哪个？**
- 面试中，暴力法说完思路后直接过渡到 KMP。KMP 是字符串匹配的经典必会算法。
- Rabin-Karp 在多模式匹配场景更有优势（面试中如果追问"如果有多个 needle 怎么办？"可以提）。

## 易错点

1. **暴力法外层循环边界写错**
   - ✗ `for (int i = 0; i < n; i++)` — 当 i > n-m 时 `haystack[i+j]` 越界
   - ✓ `for (int i = 0; i <= n - m; i++)` — 剩余长度不够就不用试了
   - 注意：当 n < m 时，`n - m` 在 unsigned 下会溢出为巨大正数！所以用 `int` 类型或先判断 `if (m > n) return -1;`

2. **KMP 的 next 数组回退逻辑漏掉 while**
   - ✗ `if (j > 0 && needle[i] != needle[j]) j = next[j-1];` — 只回退一次，可能回退不够
   - ✓ `while (j > 0 && needle[i] != needle[j]) j = next[j-1];` — 必须循环回退到匹配或 j=0

3. **KMP 返回匹配起始位置算错**
   - ✗ `return i;` — 这是匹配结束位置
   - ✓ `return i - m + 1;` — 起始位置 = 结束位置 - needle长度 + 1

4. **Rabin-Karp 哈希减法产生负数**
   - ✗ `windowHash = (windowHash - haystack[i] * power) % mod * base + ...`
   - ✓ `windowHash = ((windowHash - haystack[i] * power % mod + mod) % mod) * base + ...`

## 面试追问

**Q1: 暴力法的最坏情况是什么？为什么是 O(n·m)？**
→ haystack = "aaaa...a"（全 a），needle = "aaa...ab"（m-1个a加b）。每个位置都匹配到最后一个字符才失败，共 n-m+1 个位置，每次比较 m 次。

**Q2: KMP 的 next 数组是什么含义？为什么它能让 i 不回退？**
→ next[j] 表示 needle[0..j-1] 的最长相等前后缀长度。匹配失败时，已匹配部分 = needle 前缀。前后缀重叠部分不需要重新比较，直接从 next[j-1] 继续。因为 haystack 中这段已经和 needle 前缀确认相等了。

**Q3: 如果要找 haystack 中 needle 的所有出现位置呢？**
→ 在 KMP 匹配成功 `j == m` 后，不 return，而是记录 `i - m + 1` 到结果集，然后 `j = next[j-1]` 继续匹配。其余代码不变。时间仍为 O(n+m)。

**Q4: 如果有多个 needle 需要同时匹配怎么办？**
→ 可以用 Aho-Corasick 算法（多模式匹配），本质是在 Trie 上构建失败指针，类似 KMP 的 next 数组但在 Trie 结构上。或者用 Rabin-Karp 对每个 needle 算哈希。

## 相关题型

- **459. 重复的子字符串** — 直接复用 KMP 的 next 数组。如果 `n % (n - next[n-1]) == 0`，说明整个字符串由重复子串构成。
- **214. 最短回文串** — 在 `s + "#" + reverse(s)` 上求 next 数组，next 的最后一个值就是 s 的最长回文前缀长度。
- **686. 重复叠加字符串匹配** — 需要判断 b 是否是 a 重复若干次后的子串，可以用本题的 KMP 在拼接后的字符串上匹配。
- **1392. 最长快乐前缀** — 直接就是求 next 数组最后一个值对应的前缀。