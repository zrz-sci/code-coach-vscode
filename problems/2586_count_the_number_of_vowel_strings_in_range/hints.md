# 2586. 统计范围内的元音字符串数

## 核心思路

遍历 `words[left..right]` 范围内的字符串，检查每个字符串是否**以元音字母开头且以元音字母结尾**。满足条件就计数。元音字母集合为 `{a, e, i, o, u}`。

这是一道纯粹的**模拟/遍历**题，没有算法技巧，重点考察：
1. 字符串首尾字符的提取
2. 元音判断的高效实现
3. 范围遍历的边界处理

## 思维链

1. **读完题第一反应** → 遍历指定范围，检查每个字符串的首尾字符。很直观，没有复杂的数据结构或算法需求。

2. **"元音字符串"的定义** → 首字符是元音 **且** 尾字符是元音。两个条件都要满足。注意是"且"不是"或"。

3. **单字符字符串** → 比如 `"u"`，首字符和尾字符是同一个字符 `'u'`，它是元音，所以 `"u"` 是元音字符串。`front()` 和 `back()` 返回同一个字符。

4. **如何判断元音？** → 多种方式：
   - 方法A：`c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'`
   - 方法B：`string("aeiou").find(c) != string::npos`
   - 方法C：`set<char>{'a','e','i','o','u'}.count(c)`
   - 方法D：查表（预填充的 bool 数组）
   - 面试中方法A最直接，方法B最简洁。

5. **时间复杂度分析** → O(right - left + 1)。每个字符串只检查首尾两个字符，所以每个字符串的检查是 O(1)。总共检查 `right - left + 1` 个字符串。

6. **有没有更优的做法？** → 对于单次查询，O(n) 已经是最优。如果有**多次查询**（不同的 left, right），可以预处理前缀和：`prefix[i]` 表示 `words[0..i-1]` 中元音字符串的数量，每次查询 O(1)。但本题只需要一次查询。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 直接遍历 | 逐个检查首尾字符 | O(right-left+1) | O(1) | 必须掌握 |
| 前缀和 | 预处理后 O(1) 查询 | O(n) 预处理 + O(1) 查询 | O(n) | 多次查询时 |

## 关键提示

1. **首尾字符提取**：
   - C++: `words[i].front()` 和 `words[i].back()`，或者 `words[i][0]` 和 `words[i][words[i].size()-1]`
   - 注意 `front()` 和 `back()` 在空字符串上是未定义行为，但题目保证 `words[i].length >= 1`

2. **元音判断的多种实现**：
   - 最直接: `c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'`
   - 用 string: `string("aeiou").find(c) != string::npos`
   - 用 set: 预定义 `unordered_set<char> vowels{'a','e','i','o','u'}`
   - 用 lambda: 封装成 `auto isVowel = [](char c) { ... };` 提高可读性

3. **边界条件**：
   - `left == right`：只检查一个字符串
   - `left == 0, right == n-1`：检查整个数组
   - 字符串长度为 1：首字符 == 尾字符

4. **"0-indexed"数组**：题目明确说了 0-indexed，所以直接用 `for (int i = left; i <= right; i++)` 就行，不需要额外偏移。

5. **Follow-up 思考 — 前缀和优化**：如果这个函数要被多次调用（不同的 left/right），每次都遍历一遍太慢。可以预处理一个前缀和数组：`prefix[i]` = `words[0..i-1]` 中元音字符串的数量。查询 `[left, right]` 的答案就是 `prefix[right+1] - prefix[left]`，O(1)。

## 解法详解

### 解法1: 直接遍历 — 面试必写

**思考过程**: 最直接——遍历范围内的每个字符串，检查首尾是否都是元音。

```
示例1: words = ["are", "amy", "u"], left = 0, right = 2

  i=0: "are" → front='a'(元音), back='e'(元音) → 是 ✓ → count=1
  i=1: "amy" → front='a'(元音), back='y'(非元音) → 否 ✗
  i=2: "u"   → front='u'(元音), back='u'(元音) → 是 ✓ → count=2

  返回 2
```

```
示例2: words = ["hey", "aeo", "mu", "ooo", "artro"], left = 1, right = 4

  i=1: "aeo"   → front='a'(✓), back='o'(✓) → 是 → count=1
  i=2: "mu"    → front='m'(✗)               → 否
  i=3: "ooo"   → front='o'(✓), back='o'(✓) → 是 → count=2
  i=4: "artro" → front='a'(✓), back='o'(✓) → 是 → count=3

  返回 3
```

```cpp
class Solution {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        auto isVowel = [](char c) {
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };
        int count = 0;
        for (int i = left; i <= right; i++) {
            if (isVowel(words[i].front()) && isVowel(words[i].back()))
                count++;
        }
        return count;
    }
};
```

**关键点**:
- Lambda `isVowel` 封装元音判断逻辑，避免代码重复。
- `front()` 和 `back()` 分别获取首尾字符，比 `words[i][0]` 更语义化。
- 短路求值：如果首字符不是元音，`&&` 后面的 `isVowel(back())` 不会执行。

---

### 解法1b: 用 string.find 判断元音 — 更简洁

```cpp
class Solution {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        string vowels = "aeiou";
        int count = 0;
        for (int i = left; i <= right; i++) {
            if (vowels.find(words[i].front()) != string::npos &&
                vowels.find(words[i].back()) != string::npos)
                count++;
        }
        return count;
    }
};
```

**关键点**:
- `vowels.find(c)` 在长度为 5 的字符串中查找，实际是 O(1)。
- 代码更简洁，但 `find` + `npos` 比较啰嗦。面试中两种写法都可以。

---

### 解法1c: 用 unordered_set 判断元音 — 最清晰

```cpp
class Solution {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u'};
        int count = 0;
        for (int i = left; i <= right; i++) {
            if (vowels.count(words[i].front()) && vowels.count(words[i].back()))
                count++;
        }
        return count;
    }
};
```

**关键点**:
- `unordered_set::count()` 返回 0 或 1，可以直接用作布尔值。
- 语义最清晰："元音集合中是否包含这个字符"。
- 对于只有 5 个元素的集合，哈希表的开销反而比直接比较大，但在面试中清晰度更重要。

---

### 解法2: 前缀和 — 多次查询优化

**思考过程**: 如果同一个 `words` 数组会被多次查询（不同的 left/right），每次遍历太慢。预处理前缀和后每次查询 O(1)。

```
预处理示例: words = ["are", "amy", "u", "hey", "aeo"]

  isVowelString: [1, 0, 1, 0, 1]  (are=是, amy=否, u=是, hey=否, aeo=是)
  prefix:        [0, 1, 1, 2, 2, 3]

  查询 [0,2]: prefix[3] - prefix[0] = 2 - 0 = 2
  查询 [1,4]: prefix[5] - prefix[1] = 3 - 1 = 2
  查询 [2,2]: prefix[3] - prefix[2] = 2 - 1 = 1
```

```cpp
class Solution {
public:
    int vowelStrings(vector<string>& words, int left, int right) {
        int n = words.size();
        auto isVowel = [](char c) {
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };

        // 预处理前缀和
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] +
                (isVowel(words[i].front()) && isVowel(words[i].back()) ? 1 : 0);
        }

        // O(1) 查询
        return prefix[right + 1] - prefix[left];
    }
};
```

**关键点**:
- `prefix[i]` 表示 `words[0..i-1]` 中元音字符串的数量。
- 查询 `[left, right]` = `prefix[right+1] - prefix[left]`。
- 预处理 O(n)，每次查询 O(1)。
- 本题只有一次查询，所以前缀和是"杀鸡用牛刀"。但如果面试官追问多次查询场景，这是标准答案。

## 解法对比

|  | 直接遍历 | 前缀和 |
|--|---------|--------|
| 时间（单次查询） | O(right-left+1) | O(n) 预处理 + O(1) |
| 时间（k次查询） | O(k * n) | O(n + k) |
| 空间 | O(1) | O(n) |
| 实现难度 | 极简 | 简单 |
| 适用场景 | 单次查询 | 多次查询 |

## 易错点

1. **忘记尾字符也要是元音**：
   - 错: 只检查 `isVowel(words[i].front())` → "amy" 也被计入
   - 对: 同时检查 `isVowel(front()) && isVowel(back())`

2. **范围写错**：
   - 错: `for (int i = left; i < right; ...)` → 少检查一个
   - 对: `for (int i = left; i <= right; ...)` → 闭区间 [left, right]

3. **忽略单字符字符串**：
   - 容易以为 "u" 只有一个字符不算，但 front()=='u' 和 back()=='u' 都是元音 → 算元音字符串。

4. **元音集合写错**：
   - 错: 忘记 'u'，或者多加了 'y'
   - 对: `{a, e, i, o, u}` 恰好 5 个

5. **大小写问题**：
   - 题目说 "words[i] consists of only lowercase English letters"，所以不用考虑大写。
   - 但如果面试官改成可能有大写，记得 `tolower(c)` 转换。

## 面试追问

**Q1: 如果要处理多次查询（不同的 left, right），怎么优化？**
> 预处理前缀和数组。`prefix[i]` = `words[0..i-1]` 中元音字符串数量。每次查询 `prefix[right+1] - prefix[left]`，O(1)。预处理 O(n)，k 次查询总共 O(n+k)。

**Q2: 如果 words 数组很大（10^6），有什么需要注意的？**
> 直接遍历解法不需要额外空间，效率最高。但如果有多次查询，前缀和的 O(n) 额外空间是值得的。另外，`string.find()` 在每次调用时创建临时 `string` 对象可能有额外开销，用 lambda 的直接比较更高效。

**Q3: 如何让元音判断更高效？**
> 对于 ASCII 字符，可以用一个 `bool vowel[128]` 查找表，预填充后查询是 O(1) 且无分支。但对于只有 5 个元音的情况，编译器通常会将 `||` 链优化得很好。

**Q4: 这道题和 2559. Count Vowel Strings in Ranges 有什么关系？**
> 2559 题是本题的"多查询版"——给你多组 `[left, right]` 查询。解法就是本题的前缀和扩展：预处理一次，每次查询 O(1)。

## 相关题型

- **2559. 统计范围内的元音字符串数 II** — 本题的多查询版本，必须用前缀和。预处理后每次 O(1)。
- **1456. 定长子串中元音的最大数目** — 滑动窗口 + 元音判断，窗口内统计元音字符数。
- **345. 反转字符串中的元音字母** — 双指针 + 元音判断，原地反转字符串中的元音。
- **1839. 所有元音按顺序排列的最长子字符串** — 滑动窗口 + 元音顺序检查，更复杂的元音题。
