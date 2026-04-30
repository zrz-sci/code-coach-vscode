# 796. 旋转字符串

## 核心思路

本题本质上在问：**s 的所有旋转结果中，是否存在一个等于 goal？** 关键洞察是——把 s 拼接两次（s+s），所有旋转结果都会作为子串出现在其中。

## 思维链

1. **读完题第一反应**：旋转就是把第一个字符移到末尾，最朴素的做法就是模拟——逐次旋转 s，每次和 goal 比较。s 长度为 n，最多旋转 n 次就回到原串。

2. **暴力解的瓶颈在哪？** 每次旋转需要 O(n) 构造新串，比较也需要 O(n)，共 n 次旋转，总共 O(n²)。虽然本题 n≤100 能过，但有没有更优雅的方式？

3. **关键观察**：写出 s="abcde" 的所有旋转：
   ```
   abcde  ← 原始
   bcdea  ← 旋转1次
   cdeab  ← 旋转2次
   deabc  ← 旋转3次
   eabcd  ← 旋转4次
   ```
   现在把 s 拼接自身：`s+s = "abcdeabcde"`。仔细看——上面每一个旋转结果，都是 `"abcdeabcde"` 的一个长度为 5 的子串！

4. **为什么 s+s 包含所有旋转？** 旋转 k 次的结果 = `s[k..n-1] + s[0..k-1]`。而 `(s+s)[k..k+n-1]` 恰好就是这个拼接。所以只需检查 `goal` 是否是 `s+s` 的子串。

5. **别忘了长度检查**：如果 `s.length != goal.length`，直接返回 false。否则 goal 可能是 s+s 的子串但长度不对（比如 s="ab", goal="a"，`"abab".find("a")` 会成功但显然不是旋转）。

6. **进阶**：如果面试官追问字符串匹配的底层实现，可以聊 KMP / Rabin-Karp 等 O(n) 算法。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 模拟旋转 | 逐次旋转比较 | O(n²) | O(n) | 能说出即可 |
| s+s 子串法 | 检查 goal 是否是 s+s 的子串 | O(n)* | O(n) | ⭐ 必须写出 |
| KMP 匹配 | 手写 KMP 在 s+s 中查找 goal | O(n) | O(n) | 加分项 |

> *注：`string::find` 最坏 O(n²)，但实际多数实现接近 O(n)。严格 O(n) 需要 KMP/Rabin-Karp。

## 关键提示

- **提示1**：s 一共有多少种不同的旋转结果？最多 n 种（旋转 n 次回到原串）。
- **提示2**：把 s 写两遍拼在一起，观察旋转结果和这个拼接串的关系。
- **提示3**：检查子串之前，先想想有没有必要检查的前提条件（长度？）。
- **提示4**：如果 n 很大（10^6 级别），暴力模拟旋转就不够了，需要高效的字符串匹配算法。

## 解法详解

### 解法1: 模拟旋转 — O(n²) / O(n)

**思考过程**：最直觉的做法——题目说"旋转若干次"，那就真的旋转，每旋转一次就和 goal 比一下。

```cpp
class Solution {
public:
    bool rotateString(string s, string goal) {
        if (s.size() != goal.size()) return false;
        
        int n = s.size();
        for (int i = 0; i < n; i++) {
            // 旋转 i 次后的结果 = s[i..n-1] + s[0..i-1]
            if (s.substr(i) + s.substr(0, i) == goal) {
                return true;
            }
        }
        return false;
    }
};
```

**关键点**：
- `substr` 每次创建新字符串，加上字符串比较，每轮 O(n)，共 n 轮 → O(n²)。
- 也可以不用 substr，而是用一个变量维护当前旋转后的串（每次把首字符移到末尾），效果一样。

---

### 解法2: s+s 子串法 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**：解法1的问题是要逐个枚举旋转位置。核心洞察——`s+s` 包含了 s 的所有旋转结果作为子串。

```
// s = "abcde"
// s+s = "abcdeabcde"
//        ^^^^^         → abcde (旋转0次)
//         ^^^^^        → bcdea (旋转1次)
//          ^^^^^       → cdeab (旋转2次) ← goal!
//           ^^^^^      → deabc (旋转3次)
//            ^^^^^     → eabcd (旋转4次)
```

```cpp
class Solution {
public:
    bool rotateString(string s, string goal) {
        // 长度不同，不可能通过旋转得到
        if (s.size() != goal.size()) return false;
        
        // s+s 包含 s 的所有旋转，检查 goal 是否是其子串
        return (s + s).find(goal) != string::npos;
    }
};
```

**关键点**：
- 两行代码搞定，优雅且高效。
- **必须先检查长度**，否则 s="ab", goal="a" 会返回 true（"abab" 包含 "a"）。

---

### 解法3: KMP 字符串匹配 — O(n) / O(n)

**为什么还需要这个？** `string::find` 的最坏时间复杂度是 O(n·m)。如果面试官追问严格 O(n) 的做法，就需要 KMP。

**KMP 核心思想**：预处理 pattern（goal）的 "部分匹配表"（next 数组），利用已匹配的信息避免回退 text 的指针。

```cpp
class Solution {
public:
    bool rotateString(string s, string goal) {
        if (s.size() != goal.size()) return false;
        if (s.empty()) return true;
        
        int n = s.size();
        string text = s + s;  // 在 text 中找 goal
        
        // 构建 goal 的 next 数组（最长相等前后缀长度）
        vector<int> next(n, 0);
        for (int i = 1, j = 0; i < n; i++) {
            while (j > 0 && goal[i] != goal[j]) j = next[j - 1];
            if (goal[i] == goal[j]) j++;
            next[i] = j;
        }
        
        // KMP 匹配：在 text 中查找 goal
        for (int i = 0, j = 0; i < 2 * n; i++) {
            while (j > 0 && text[i] != goal[j]) j = next[j - 1];
            if (text[i] == goal[j]) j++;
            if (j == n) return true;  // 完整匹配
        }
        return false;
    }
};
```

**关键点**：
- KMP 在面试中不一定要手写，但能讲清原理是加分项。
- next 数组的含义：`next[i]` = `goal[0..i]` 中最长的相等前后缀长度。

## 解法对比

| | 解法1: 模拟旋转 | 解法2: s+s 子串 | 解法3: KMP |
|---|---|---|---|
| 时间 | O(n²) | O(n)~O(n²)* | 严格 O(n) |
| 空间 | O(n) | O(n) | O(n) |
| 代码量 | 中等 | 极少（2行核心） | 较多 |
| 面试推荐 | 作为起点说明理解 | ⭐ 首选 | 追问时加分 |

> *取决于 `find` 的实现。C++ 标准未规定具体算法，GCC 用的是类似朴素匹配但有优化。

## 易错点

1. **忘记长度检查**
   - ✗ 直接 `return (s+s).find(goal) != string::npos;`
   - ✓ 先判断 `s.size() != goal.size()` 返回 false
   - 原因：s="abc", goal="ab"，"abcabc" 包含 "ab" 但 "ab" 不是 "abc" 的旋转

2. **模拟旋转时循环次数错误**
   - ✗ `for (int i = 0; i < n - 1; i++)` — 漏掉了旋转 0 次（原串本身）
   - ✓ `for (int i = 0; i < n; i++)` — 包括不旋转的情况

3. **空串边界**
   - `s = ""`, `goal = ""` → 应返回 true（题目保证长度≥1，但防御性编程习惯好）

## 面试追问

**Q1: 暴力解法的时间复杂度是多少？能优化吗？**
→ 暴力 O(n²)（n 次旋转 × 每次 O(n) 比较）。优化：利用 s+s 包含所有旋转的性质，转化为子串查找。

**Q2: 为什么 s+s 一定包含所有旋转结果？能证明吗？**
→ 旋转 k 次 = `s[k..n-1] + s[0..k-1]`，而 `(s+s)[k..k+n-1]` = `s[k..n-1] + s[0..k-1]`（第一份的后半 + 第二份的前半），恰好一致。k 取 0 到 n-1 覆盖所有旋转。

**Q3: 如果字符串很长（10^6），`find` 最坏是 O(n²)，怎么保证 O(n)？**
→ 用 KMP 或 Rabin-Karp 在 s+s 中匹配 goal。KMP 预处理 O(n)，匹配 O(n)，总计 O(n)。

**Q4: 这个 "拼接后找子串" 的技巧还能用在哪？**
→ 任何循环/环形结构的问题。比如判断两个循环链表是否等价、环形数组的子串匹配等。核心思想是 **"复制一份破环为链"**。

## 相关题型

- **28. 找出字符串中第一个匹配项的下标 (Find the Index of the First Occurrence)** — 本题的子串查找本质就是这道题。复用 KMP / `find` 模板。
- **459. 重复的子字符串 (Repeated Substring Pattern)** — 同样用 s+s 技巧！判断 s 是否由重复子串构成 = 检查 `(s+s)[1..2n-2]` 是否包含 s。复用完全相同的"拼接找子串"思路。
- **686. 重复叠加字符串匹配 (Repeated String Match)** — 类似思路，需要 a 重复若干次后包含 b，本质也是循环子串匹配。