# 139. 单词拆分

## 核心思路

这道题本质上是一个**字符串分割可行性判断**问题：能否在字符串 `s` 的某些位置"切一刀"，使得切出来的每一段都是字典中的单词？这就是一个经典的**决策型 DP**——在每个位置决定"这一段用哪个单词匹配"。

## 思维链

1. **读完题第一反应**：从位置 0 开始，尝试用字典中的每个单词去匹配前缀，匹配上了就递归处理剩余部分。这就是暴力递归/回溯。

2. **暴力解的瓶颈**：对于 `s = "aaaa...a"`，字典 `["a", "aa", "aaa", ...]`，每个位置有多种匹配方式，递归分支呈指数级爆炸。但关键观察是——**从同一个位置 i 开始的子问题，答案是固定的**，被反复求解了！

3. **发现重叠子问题**：定义 `canBreak(i)` = "从位置 i 到末尾能否被拆分"。不同的匹配路径可能到达同一个位置 i，导致重复计算。→ **加记忆化**，从指数级降到 O(n²)。

4. **翻译成递推 DP**：定义 `dp[i]` = "前 i 个字符（即 `s[0..i-1]`）能否被拆分"。对每个位置 i，枚举最后一个单词的起始位置 j，如果 `dp[j] == true` 且 `s[j..i-1]` 在字典中，则 `dp[i] = true`。

5. **优化查找**：用 HashSet 存字典，使得单词查找 O(L)。进一步注意：字典中单词最长才 20，所以内层循环不需要从 0 枚举到 i，只需要往前看 maxLen 个位置。

6. **进阶：Trie 优化**：如果字典很大且有大量公共前缀，可以用 Trie 优化匹配过程。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | DFS 尝试每种前缀匹配 | O(2ⁿ) 最坏 | O(n) 栈深 | 能说出即可 |
| 记忆化搜索 | 递归 + memo 消除重叠子问题 | O(n² · L) | O(n) | 能说出即可 |
| 递推 DP | dp[i] 表示前 i 个字符能否拆分 | O(n · m · L) 或 O(n² · L) | O(n) | ⭐ 必须写出 |
| DP + Trie | 用 Trie 优化单词匹配 | O(n · maxLen) | O(Σ词长) | 加分项 |

> n = s.length, m = wordDict.size, L = 单词平均长度, maxLen = 最长单词长度

## 关键提示

- **提示1**：想一想，如果你知道 `s[0..j-1]` 可以被拆分（某个前缀是 OK 的），那你只需要检查 `s[j..i-1]` 是否是一个字典单词，就能判断 `s[0..i-1]` 是否 OK。

- **提示2**：`dp[0] = true` 代表空字符串可以被拆分（base case）。这不是"空字符串是字典单词"，而是"还没切任何字符"的初始状态。

- **提示3**：内层循环不需要让 j 从 0 遍历到 i。字典中最长单词才 20 个字符，所以 j 只需要从 `max(0, i - maxLen)` 开始。

- **提示4**：用 `unordered_set` 存字典，`s.substr(j, i-j)` 的查找是 O(i-j) 而非 O(1)，因为字符串哈希需要遍历字符。

- **提示5**：决策树可视化（以 `s = "leetcode"` 为例）：
```
位置 0: "leetcode"
├── 匹配 "leet" → 位置 4: "code"
│   └── 匹配 "code" → 位置 8: "" ✓ 成功!
├── 匹配 "lee"  → 不在字典中 ✗
├── 匹配 "le"   → 不在字典中 ✗
└── ...
```

## 解法详解

### 解法1: 暴力递归 — O(2ⁿ) / O(n)

**思考过程**：最自然的想法——从位置 0 开始，尝试字典中每个单词作为前缀匹配。匹配上了就从新位置继续递归。如果能走到字符串末尾，返回 true。

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        return dfs(s, 0, wordSet);
    }
    
    bool dfs(const string& s, int start, const unordered_set<string>& wordSet) {
        // 走到末尾，说明整个字符串都被成功拆分
        if (start == s.size()) return true;
        
        // 尝试从 start 开始，截取不同长度的前缀
        for (int end = start + 1; end <= (int)s.size(); end++) {
            string word = s.substr(start, end - start);
            // 如果这段是字典单词，且剩余部分也能拆分
            if (wordSet.count(word) && dfs(s, end, wordSet)) {
                return true;
            }
        }
        return false;
    }
};
```

**关键点**：这个解法会超时。对于 `s = "aaa...ab"`, `wordDict = ["a","aa","aaa",...]`，每个位置都有多种匹配方式，但最终都因为 'b' 失败。大量重复计算。

---

### 解法2: 记忆化搜索 — O(n² · L) / O(n)

**从解法1优化**：解法1的瓶颈是"同一个位置 start 被不同路径反复调用"。加一个 memo 数组记录每个位置的结果。

```
// 重叠子问题示例: s = "catsand", dict = ["cat","cats","and","sand"]
//
// 路径1: "cat" → dfs(3)  → "sand" 不行 → "s" 不行 → ...
// 路径2: "cats" → dfs(4) → "and" → dfs(7) ✓
//
// 如果字典还有 "ca" → "ca" → dfs(2) → "t" 不行 → ...
//                                     → "ts" 不行 → ...
// dfs(3), dfs(4) 等可能被多次调用
```

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        // memo[i]: 0=未访问, 1=可拆分, -1=不可拆分
        vector<int> memo(s.size(), 0);
        return dfs(s, 0, wordSet, memo);
    }
    
    bool dfs(const string& s, int start, 
             const unordered_set<string>& wordSet, vector<int>& memo) {
        if (start == s.size()) return true;
        if (memo[start] != 0) return memo[start] == 1;
        
        for (int end = start + 1; end <= (int)s.size(); end++) {
            if (wordSet.count(s.substr(start, end - start)) 
                && dfs(s, end, wordSet, memo)) {
                memo[start] = 1;
                return true;
            }
        }
        memo[start] = -1;
        return false;
    }
};
```

**关键点**：memo 用 int 而非 bool，因为 bool 无法区分"未计算"和"计算结果为 false"。也可以用 `unordered_map<int,bool>` 或 `vector<optional<bool>>`。

---

### 解法3: 递推 DP — O(n · min(n, maxLen) · L) / O(n) ⭐ 面试首选

**从记忆化翻译成递推**：自底向上填表。`dp[i]` 表示 `s[0..i-1]`（前 i 个字符）能否被拆分。

```
// DP 填表过程 (s = "leetcode", dict = {"leet", "code"}):
//
// 下标:  0  1  2  3  4  5  6  7  8
// dp:   [T] F  F  F  ?  F  F  F  ?
//        ↑              ↑           ↑
//      空串           "leet"     "leetcode"
//      base          dp[0]=T     dp[4]=T
//                    +"leet"     +"code"
//                    =true       =true
//
// 填表方向: i 从 1 到 n, 对每个 i 枚举 j
//
// dp[4]: j=0, s[0..3]="leet" ∈ dict, dp[0]=T → dp[4]=T ✓
// dp[8]: j=4, s[4..7]="code" ∈ dict, dp[4]=T → dp[8]=T ✓
```

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int n = s.size();
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        
        // 预计算最长单词长度，限制内层循环范围
        int maxLen = 0;
        for (const string& w : wordDict) {
            maxLen = max(maxLen, (int)w.size());
        }
        
        // dp[i] = s的前i个字符能否被拆分
        vector<bool> dp(n + 1, false);
        dp[0] = true;  // 空字符串是合法的起点
        
        for (int i = 1; i <= n; i++) {
            // j 是最后一个单词的起始位置
            // 单词长度 = i - j，不超过 maxLen
            for (int j = max(0, i - maxLen); j < i; j++) {
                // dp[j] 为 true 意味着 s[0..j-1] 已经可以拆分
                // 再检查 s[j..i-1] 是否是字典单词
                if (dp[j] && wordSet.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;  // 只要找到一种拆法就够了
                }
            }
        }
        
        return dp[n];
    }
};
```

**关键点**：
- `dp[0] = true` 是 base case，代表"什么都不选"的状态，不是说空串在字典中。
- `break` 很重要：一旦 dp[i] 确定为 true，无需继续枚举 j。
- `maxLen` 剪枝将内层从 O(n) 降到 O(maxLen)，本题 maxLen ≤ 20，效果显著。

---

### 解法4: DP + Trie — O(n · maxLen) / O(Σ词长)（加分项）

**思路**：解法3中 `substr + HashSet` 查找每次要创建子串。用 Trie 可以从位置 j 开始逐字符向前/向后匹配，避免 substr 开销，且天然支持前缀共享。

```
// Trie 结构示例 (dict = ["leet", "lee", "code"]):
//
//  root
//  ├── l → e → e → t*   (* 表示 isEnd)
//  │            └── *
//  └── c → o → d → e*
//
// 匹配 s = "leetcode":
// 从位置0开始沿 Trie 走: l→e→e(匹配"lee")→t(匹配"leet")
// 从位置4开始沿 Trie 走: c→o→d→e(匹配"code")
```

```cpp
class Solution {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    
    bool wordBreak(string s, vector<string>& wordDict) {
        // 构建 Trie
        TrieNode* root = new TrieNode();
        for (const string& word : wordDict) {
            TrieNode* node = root;
            for (char c : word) {
                int idx = c - 'a';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
            }
            node->isEnd = true;
        }
        
        int n = s.size();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        
        for (int i = 0; i < n; i++) {
            if (!dp[i]) continue;  // 如果位置 i 不可达，跳过
            
            // 从位置 i 开始，沿 Trie 逐字符匹配
            TrieNode* node = root;
            for (int j = i; j < n; j++) {
                int idx = s[j] - 'a';
                if (!node->children[idx]) break;  // Trie 中无此路径
                node = node->children[idx];
                if (node->isEnd) {
                    dp[j + 1] = true;  // s[i..j] 是字典单词
                }
            }
        }
        
        return dp[n];
    }
};
```

**关键点**：Trie 方法从每个可达位置 `i` 出发，沿 Trie 往后匹配，碰到 isEnd 就标记 `dp[j+1] = true`。匹配长度最多 maxLen，且如果 Trie 中没有对应路径会提前 break，比 HashSet 的 substr 更高效。

---

## 解法对比

| | 暴力递归 | 记忆化搜索 | 递推 DP ⭐ | DP + Trie |
|---|---|---|---|---|
| **时间** | O(2ⁿ) | O(n² · L) | O(n · maxLen · L) | O(n · maxLen) |
| **空间** | O(n) 栈 | O(n) | O(n) | O(Σ词长) |
| **思路** | 最直觉 | 消除重复 | 自底向上 | 优化匹配 |
| **面试** | 展示理解 | 过渡 | **首选** | 追问加分 |

- 面试中写**递推 DP + HashSet + maxLen 剪枝**即可，这是最简洁的方案。
- Trie 方案适合追问"如果字典特别大怎么办"或"如果有大量公共前缀"。
- 记忆化搜索和递推 DP 本质相同，一个自顶向下一个自底向上。

## 易错点

1. **忘记 `dp[0] = true`**
   - ✗ `dp[0] = false` → 所有 dp[i] 都无法从 base case 推导出来，全部为 false
   - ✓ `dp[0] = true`，含义是"空前缀是合法的拆分起点"

2. **substr 参数搞混**
   - ✗ `s.substr(j, i)` — 第二个参数是长度不是终点
   - ✓ `s.substr(j, i - j)` — 从位置 j 开始，长度为 i - j

3. **内层循环方向搞反**
   - ✗ `for (int j = i; j >= 0; j--)` 然后检查 `s.substr(i, j - i)` — 越界
   - ✓ 搞清楚 j 是"最后一个单词的起始位置"还是"结束位置"，画图验证

4. **没有 break 导致 TLE（虽然不影响正确性）**
   - 一旦 `dp[i] = true`，继续枚举 j 是浪费时间
   - 加 `break` 是常数优化，但在极端情况下差异很大

5. **用 `set` 代替 `unordered_set`**
   - `set` 查找 O(L·log m)，`unordered_set` 查找 O(L)
   - 虽然都能 AC，但面试中选择 `unordered_set` 体现了对复杂度的关注

## 面试追问

**Q1: 暴力递归为什么会超时？能否用一个例子说明重叠子问题？**

> `s = "aaab"`, `dict = ["a", "aa", "aaa"]`。从位置 0 匹配 "a" 到位置 1，匹配 "aa" 到位置 2，或匹配 "aaa" 到位置 3。从位置 1 又可以匹配 "a" 到位置 2，匹配 "aa" 到位置 3... 多条路径都会到达位置 2、位置 3，导致 `dfs(2)`, `dfs(3)` 被重复计算。

**Q2: 这道题和完全背包有什么关系？**

> 可以理解为：字典中的单词是"物品"（可重复使用），字符串 s 是"背包"，问能否恰好装满。但与标准完全背包不同的是，这里的"装入顺序"有要求（拼接顺序必须和 s 一致），所以是**排列型完全背包**，外层循环是容量（位置 i），内层循环是物品（单词），而不是反过来。

**Q3: 如果要返回所有可能的拆分方式（LeetCode 140），代码怎么改？**

> 将 `dp[i]` 从 bool 改为记录所有能到达位置 i 的前一个断点 j（或用回溯 + 记忆化）。本题只判断 true/false，140 题需要回溯收集所有路径。核心 DP 框架不变，但需要额外存储路径信息。

**Q4: 如果字典特别大（百万级别），有大量公共前缀，怎么优化？**

> 用 Trie 代替 HashSet。Trie 天然共享公共前缀，匹配时只需逐字符走 Trie，匹配到 isEnd 就找到一个单词。时间从 O(n · m · L) 降到 O(n · maxLen)。

## 相关题型

- **140. 单词拆分 II** — 本题的 Follow-up。复用相同的 DP/递归框架，区别：139 只判断 true/false，140 要收集所有拆分方案（回溯 + 记忆化）。
- **322. 零钱兑换** — 同为完全背包变体。322 是"凑出金额的最少硬币数"（最值型），139 是"能否拆分"（可行性型），DP 转移从 `min` 变成 `||`。
- **377. 组合总和 Ⅳ** — 排列型完全背包。外层循环 target、内层循环物品。和本题"外层位置 i、内层枚举单词"的结构完全一致。
- **472. 连接词** — 复用本题的 `wordBreak` 作为子过程。对字典中每个词判断"能否由其他词拼接而成"。