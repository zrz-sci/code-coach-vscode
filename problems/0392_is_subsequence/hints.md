# 392. 判断子序列

## 核心思路

本质上在问：能否在 `t` 中**从左到右**依次找到 `s` 的每个字符？这就是一个**贪心匹配**问题 —— 对于 `s` 的每个字符，在 `t` 中尽早匹配它，给后续字符留出最大空间。

## 思维链

1. **读完题第一反应**：子序列 = 保持相对顺序，可以跳过一些字符。最直觉的做法：两个指针分别指向 `s` 和 `t`，同时扫描。
2. **为什么贪心是对的？**：对于 `s[i]`，在 `t` 中越早匹配越好。如果跳过一个能匹配的位置，只会让后续字符的可选范围变小，不会变好。所以贪心选"第一次遇到就匹配"是最优的。
3. **Follow-up 的瓶颈在哪？**：如果有 10 亿个 `s` 要检查，每次都扫描整个 `t` 是 O(k × |t|)，太慢了。瓶颈在于：每次查询都要线性扫描 `t`。
4. **怎么突破？**：预处理 `t` —— 对 `t` 中每个字符出现的位置建索引。查询时，对 `s` 的每个字符，用**二分查找**在索引中找到"当前位置之后的最早出现位置"。这样每次查询变成 O(|s| × log|t|)。
5. **还有什么方法？**：DP 解法把问题转化为"最长公共子序列"的变体：如果 LCS(s, t) == |s|，则 s 是 t 的子序列。虽然不是最优，但展示了 DP 思维。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双指针贪心 | 两个指针同步扫描 | O(n + m) | O(1) | ⭐ 必须写出 |
| 预处理 + 二分（Follow-up） | 对 t 建字符索引，二分查找 | 预处理 O(m)，查询 O(\|s\| × log m) | O(m) | ⭐ 必须说出 |
| 动态规划 | dp[i][j] 表示 s[0..i-1] 是否为 t[0..j-1] 的子序列 | O(n × m) | O(n × m) → O(m) | 能说出即可 |

> n = |s|, m = |t|

## 关键提示

1. **贪心直觉**：在 `t` 中从左到右扫描，遇到与 `s` 当前字符匹配的就"消耗"掉，看最后 `s` 是否被完全消耗。
2. **双指针终止条件**：循环结束后，判断的是 `s` 的指针是否走到了末尾，而不是 `t` 的指针。
3. **Follow-up 关键**：如果 `t` 不变而 `s` 有很多个，应该把"对 `t` 的预处理"一次做完，后续查询只与 `|s|` 有关。
4. **二分查找的搜索目标**：在字符 `c` 的所有位置中，找到第一个 > 当前匹配位置的位置（`lower_bound`）。
5. **空串是任何字符串的子序列**：`s` 为空时直接返回 `true`。

## 解法详解

### 解法1: 双指针贪心 — O(n + m) / O(1) ⭐ 面试首选

**思考过程**：子序列要求"保持顺序"，那我就两个指针分别指向 `s` 和 `t`，同时向右走。如果当前字符相同，`s` 的指针前进（表示匹配成功一个）；无论是否匹配，`t` 的指针都前进。最后看 `s` 的指针是否走完。

```
s = "abc", t = "ahbgdc" 的匹配过程：

t:  a  h  b  g  d  c
    ↑        
s:  a  b  c
    ↑
匹配 'a'! i++, j++

t:  a  h  b  g  d  c
       ↑        
s:  a  b  c
       ↑
'h' ≠ 'b', j++

t:  a  h  b  g  d  c
          ↑        
s:  a  b  c
       ↑
匹配 'b'! i++, j++

t:  a  h  b  g  d  c
             ↑        
s:  a  b  c
          ↑
'g' ≠ 'c', j++

t:  a  h  b  g  d  c
                ↑        
s:  a  b  c
          ↑
'd' ≠ 'c', j++

t:  a  h  b  g  d  c
                   ↑        
s:  a  b  c
          ↑
匹配 'c'! i++, j++

i == 3 == s.size() → 全部匹配，返回 true
```

```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        int i = 0, j = 0;
        while (i < s.size() && j < t.size()) {
            if (s[i] == t[j]) {
                i++;  // 匹配成功，s 的指针前进
            }
            j++;  // t 的指针无论如何都前进
        }
        return i == s.size();  // s 是否被完全匹配
    }
};
```

**关键点**：终止条件是 `i == s.size()`，不是 `j == t.size()`。如果 `j` 走完而 `i` 没走完，说明 `t` 中凑不齐 `s` 的所有字符。

---

### 解法2: 预处理 + 二分查找（Follow-up）— O(m) 预处理 + O(|s| × log m) 查询 / O(m)

**从解法1的 Follow-up 出发**：解法1 每次查询都要扫描整个 `t`。如果有 10 亿个 `s` 要查，每次都 O(m) 太慢了。

**核心思想**：预处理 `t`，为每个字符记录它出现的所有位置。查询时，对于 `s` 中的每个字符，用二分查找在对应位置列表中找到"大于当前位置"的最小位置。

```
t = "ahbgdc"
预处理索引:
  'a' → [0]
  'h' → [1]
  'b' → [2]
  'g' → [3]
  'd' → [4]
  'c' → [5]

查询 s = "abc":
  prev = -1
  's[0]='a': 在 [0] 中找 > -1 的最小值 → 0, prev = 0
  's[1]='b': 在 [2] 中找 > 0  的最小值 → 2, prev = 2
  's[2]='c': 在 [5] 中找 > 2  的最小值 → 5, prev = 5
  全部找到 → true
```

```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        // 预处理：记录 t 中每个字符出现的所有位置
        vector<vector<int>> idx(26);
        for (int i = 0; i < t.size(); i++) {
            idx[t[i] - 'a'].push_back(i);
        }
        
        int prev = -1;  // 上一次匹配的位置
        for (char c : s) {
            auto& positions = idx[c - 'a'];
            // 在 positions 中找第一个 > prev 的位置
            auto it = upper_bound(positions.begin(), positions.end(), prev);
            if (it == positions.end()) {
                return false;  // 找不到，s 不是 t 的子序列
            }
            prev = *it;  // 更新匹配位置
        }
        return true;
    }
};
```

**关键点**：
- 用 `upper_bound` 而不是 `lower_bound`，因为要找**严格大于** `prev` 的位置。
- 预处理只做一次，之后每次查询与 `t` 的长度无关（只与 `s` 的长度和字符分布有关）。

---

### 解法3: 动态规划 — O(n × m) / O(n × m)

**思考过程**：把"s 是否为 t 的子序列"看作"s 和 t 的最长公共子序列(LCS)是否等于 |s|"。虽然 overkill，但 DP 的思路能推广到更复杂的问题。

**状态定义**：`dp[i][j]` = `s[0..i-1]` 和 `t[0..j-1]` 的最长公共子序列长度。

**状态转移**：
- `s[i-1] == t[j-1]` → `dp[i][j] = dp[i-1][j-1] + 1`
- `s[i-1] != t[j-1]` → `dp[i][j] = dp[i][j-1]`（注意：这里不是 `max(dp[i-1][j], dp[i][j-1])`，因为 s 中的每个字符都必须匹配，我们不能跳过 s 的字符，只能跳过 t 的字符）

```
dp 填表过程 (s="abc", t="ahbgdc"):

       ""  a  h  b  g  d  c
   ""   0  0  0  0  0  0  0
    a   0  1  1  1  1  1  1
    b   0  1  1  2  2  2  2
    c   0  1  1  2  2  2  3

dp[3][6] = 3 == len(s) → true
```

```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        int n = s.size(), m = t.size();
        // dp[i][j]: s[0..i-1] 和 t[0..j-1] 的 LCS 长度
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s[i - 1] == t[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = dp[i][j - 1];  // 只跳过 t 的字符
                }
            }
        }
        return dp[n][m] == n;
    }
};
```

**空间优化**：`dp[i][j]` 只依赖 `dp[i-1][j-1]` 和 `dp[i][j-1]`，即上一行和当前行左边。可以用一维数组 + 变量优化。

```cpp
class Solution {
public:
    bool isSubsequence(string s, string t) {
        int n = s.size(), m = t.size();
        vector<int> dp(m + 1, 0);
        
        for (int i = 1; i <= n; i++) {
            int prev = 0;  // 保存 dp[i-1][j-1]
            for (int j = 1; j <= m; j++) {
                int temp = dp[j];  // 暂存，下一轮作为 prev
                if (s[i - 1] == t[j - 1]) {
                    dp[j] = prev + 1;
                } else {
                    dp[j] = dp[j - 1];
                }
                prev = temp;
            }
        }
        return dp[m] == n;
    }
};
```

---

## 解法对比

| | 双指针贪心 | 预处理+二分 | DP |
|---|---|---|---|
| 时间 | O(n + m) | 预处理 O(m)，查询 O(n log m) | O(n × m) |
| 空间 | O(1) | O(m) | O(n × m) → O(m) |
| 适用场景 | 单次查询 | 多次查询同一个 t | 需要扩展到更复杂问题 |
| 面试推荐 | ⭐ 首选写出 | ⭐ Follow-up 必须说出 | 能说思路即可 |

**选择策略**：
- 单次查询 → 双指针，简洁高效
- 多次查询（Follow-up） → 预处理 + 二分
- 想展示 DP 功底 / 题目要求 DP → 解法3

## 易错点

1. **终止条件判断错对象**：
   - ✗ `return j == t.size();` — j 走完只说明 t 被扫完了，不代表 s 匹配完了
   - ✓ `return i == s.size();` — 只有 s 的指针走到末尾才说明全部匹配

2. **二分查找用错函数**：
   - ✗ `lower_bound(positions.begin(), positions.end(), prev)` — 会找到等于 `prev` 的位置，导致同一个位置被重复使用
   - ✓ `upper_bound(positions.begin(), positions.end(), prev)` — 找到严格大于 `prev` 的第一个位置

3. **DP 转移方程搞成标准 LCS**：
   - ✗ `dp[i][j] = max(dp[i-1][j], dp[i][j-1])` — 标准 LCS 允许跳过 s 的字符
   - ✓ `dp[i][j] = dp[i][j-1]` — 子序列问题中 s 的每个字符都必须匹配，不能跳过 s，只能跳过 t

4. **空串没处理**：
   - `s` 为空 → 是任何字符串的子序列，返回 `true`（双指针代码天然处理了这个情况：`i == 0 == s.size()`）

## 面试追问

**Q1: 这道题最直接的思路是什么？时间复杂度多少？**
> 双指针贪心。两个指针分别从 s 和 t 的开头出发，匹配则 s 指针前进，t 指针始终前进。O(n + m) 时间，O(1) 空间。

**Q2 (Follow-up): 如果有 10 亿个 s 要查询，同一个 t，怎么优化？**
> 预处理 t：为每个字符记录出现位置列表。查询时对 s 的每个字符做二分查找。预处理 O(m) 一次完成，每次查询 O(|s| × log m)，比重复扫描 t 高效得多。

**Q3: 能否用 DP 来做？和 LCS 的关系是什么？**
> 可以。"s 是 t 的子序列"等价于"LCS(s, t) == |s|"。但标准 LCS 的转移允许同时跳过两个字符串的字符，而这里只能跳过 t 的字符，所以转移方程简化为 `dp[i][j] = dp[i][j-1]`（不匹配时）。时间 O(nm)，不如双指针高效，但思路可推广。

**Q4: 如果 t 也在动态变化（插入/删除字符），怎么高效判断子序列？**
> 这是更高级的问题。可以考虑平衡 BST 或线段树维护 t 的字符位置索引，支持动态更新和查询"某字符在位置 p 之后的最早出现位置"。

## 相关题型

- **[1143. 最长公共子序列 (LCS)](https://leetcode.com/problems/longest-common-subsequence/)** — 本题的 DP 解法就是 LCS 的特例。学会本题的 DP 后，LCS 只需把转移改成 `max(dp[i-1][j], dp[i][j-1])`（允许跳过两侧字符）。
- **[792. 匹配子序列的单词数](https://leetcode.com/problems/number-of-matching-subsequences/)** — 本题 Follow-up 的直接应用。多个 s 查询同一个 t，直接复用"预处理 + 二分"方案。
- **[524. 通过删除字母匹配到字典中最长单词](https://leetcode.com/problems/longest-word-in-dictionary-through-deleting/)** — 对字典中的每个单词判断是否为 s 的子序列，复用双指针方案。
- **[2486. 追加字符以获得子序列](https://leetcode.com/problems/append-characters-to-string-to-make-subsequence/)** — 复用双指针框架，区别：匹配结束后 s 还剩多少没匹配的字符就是答案。