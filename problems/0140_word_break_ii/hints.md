# 140. 单词拆分 II

## 核心思路

本题本质上是一个**字符串分割穷举问题**：在字符串 `s` 中找出所有可能的切割方式，使得每一段都是字典中的单词。和 139 题（Word Break I，只判断能否拆分）不同，本题需要**返回所有方案**，所以不能只用 DP 判断 true/false，必须回溯穷举所有路径。

## 与 139 题的关系

| | 139. Word Break | 140. Word Break II |
|---|---|---|
| 问什么 | 能不能拆 (bool) | 所有拆法 (列举) |
| 核心算法 | DP | 回溯 + 记忆化 |
| 关键区别 | dp[i] 只存 true/false | 需要存从位置 i 出发的所有句子列表 |

139 的 DP 可以作为本题的**预处理/剪枝**：先用 DP 判断 s[i..n-1] 是否可拆，如果不可拆就直接跳过，避免无效回溯。

## 思维链

1. **读完题第一反应**：这是一个"所有方案"问题 → 想到回溯/DFS 穷举。从位置 0 开始，尝试匹配字典中的每个单词，匹配成功就递归处理剩余部分。

2. **暴力回溯的瓶颈**：如果 `s = "aaaa...a"`，字典 `["a", "aa", "aaa", ...]`，每个位置都有多种切法，最坏情况指数级。但注意到**同一个起始位置 i，对应的所有句子列表是固定的**，不同路径可能到达相同位置 i → 存在**重叠子问题**。

3. **怎么突破**：加**记忆化**！用 `memo[i]` 缓存"从位置 i 开始的所有合法句子列表"。如果 `memo[i]` 已算过，直接返回。

4. **查找优化**：每次判断子串是否在字典中，用 **HashSet** 可以 O(L) 查找（L 是单词长度），比遍历字典快。

5. **进一步优化（可选）**：可以用 **Trie 前缀树** 替代 HashSet，在逐字符扫描时同步在 Trie 上移动，避免反复 substr。

6. **剪枝（可选但推荐）**：先用 139 题的 DP 预处理 `canBreak[i]`，在回溯时如果 `canBreak[j]` 为 false 就跳过该分支。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力回溯 | DFS 枚举所有切割 | O(2^n · n) | O(2^n · n) | 能说出即可 |
| 记忆化回溯 | DFS + memo 缓存子问题 | O(2^n · n) 最坏，实际远小 | O(2^n · n) | ⭐ 必须写出 |
| DP预处理+回溯 | 先判断可行性再回溯 | 同上+O(n²)预处理 | 同上 | 加分项 |
| Trie+记忆化回溯 | Trie 加速前缀匹配 | 同上 | 同上+Trie空间 | 加分项 |

> 注：因为要列举所有方案，最坏情况答案本身就是指数级的（如 "aaa...a" 配 ["a","aa","aaa",...]），所以时间复杂度的上界无法避免。记忆化的价值在于**避免重复计算同一子问题**。

## 关键提示

1. **这不是纯 DP 题**：139 题只问"能否拆"，一个 bool DP 就够了。本题问"所有方案"，必须穷举，DP 只能辅助剪枝。

2. **记忆化的 key 是起始位置**：`dfs(i)` 返回 `s[i..n-1]` 的所有合法句子列表。同一个 `i` 不需要算两次。

3. **用 HashSet 存字典**：判断子串是否为字典单词，HashSet 比线性遍历快。

4. **注意空串边界**：当 `i == n` 时，表示一种合法分割已完成，应返回一个包含空串的列表 `{""}` 而不是空列表 `{}`。这是最容易出错的地方！

5. **决策树可视化**（以示例1为例）：
```
s = "catsanddog", dict = {"cat","cats","and","sand","dog"}

                     dfs(0)
                    /       \
          "cat"+dfs(3)    "cats"+dfs(4)
              |                |
        "sand"+dfs(7)    "and"+dfs(7)
              |                |
         "dog"+dfs(10)   "dog"+dfs(10)
              |                |
            [✓]              [✓]
              
结果: ["cat sand dog", "cats and dog"]

注意: dfs(7) 被调用了两次 → 记忆化的价值所在!
      dfs(10) 也被调用两次 → 命中缓存
```

## 解法详解

### 解法1: 暴力回溯 — O(2^n · n) / O(n · 2^n)

**思考过程**：最直觉的做法。从位置 0 开始，枚举所有可能的第一个单词，然后递归处理剩余字符串。每次递归到字符串末尾，说明找到一种合法分割。

```cpp
class Solution {
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        vector<string> result;
        vector<string> path; // 当前路径上选的单词
        dfs(s, 0, dict, path, result);
        return result;
    }
    
    void dfs(const string& s, int start, const unordered_set<string>& dict,
             vector<string>& path, vector<string>& result) {
        if (start == s.size()) {
            // 拼接路径上的所有单词，用空格连接
            string sentence;
            for (int i = 0; i < path.size(); i++) {
                if (i > 0) sentence += " ";
                sentence += path[i];
            }
            result.push_back(sentence);
            return;
        }
        // 枚举从 start 开始的所有可能单词
        for (int end = start + 1; end <= (int)s.size(); end++) {
            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                path.push_back(word);       // 选择
                dfs(s, end, dict, path, result); // 递归
                path.pop_back();            // 撤销
            }
        }
    }
};
```

**关键点**：这个解法没有记忆化，如果 `s = "aaaa...a"` 且字典包含各种长度的 "a" 串，会有大量重复计算。但对于题目给定的 `s.length <= 20`，其实也能通过。

### 解法2: 记忆化回溯 — O(2^n · n) 最坏 / O(2^n · n) ⭐ 面试首选

**从解法1优化**：解法1中，`dfs(7)` 可能从多个不同路径到达，每次都重新计算。我们用 `memo[i]` 缓存"从位置 i 到末尾的所有合法句子列表"，避免重复计算。

```cpp
class Solution {
public:
    unordered_map<int, vector<string>> memo;
    
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        return dfs(s, 0, dict);
    }
    
    // 返回 s[start..n-1] 的所有合法句子
    vector<string> dfs(const string& s, int start, 
                       const unordered_set<string>& dict) {
        // 命中缓存，直接返回
        if (memo.count(start)) return memo[start];
        
        vector<string> sentences;
        
        // base case: 到达末尾，返回包含空串的列表
        // 为什么是 {""} 不是 {}？因为调用方要在前面拼接单词
        // 如果返回 {}，调用方的 for 循环不会执行，等于丢失了这条路径
        if (start == (int)s.size()) {
            sentences.push_back("");
            return sentences;
        }
        
        for (int end = start + 1; end <= (int)s.size(); end++) {
            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                // 递归获取剩余部分的所有句子
                vector<string> rest = dfs(s, end, dict);
                for (const string& tail : rest) {
                    // 拼接: 当前单词 + 空格 + 后续句子
                    // 如果 tail 是空串(到达末尾)，不加空格
                    sentences.push_back(
                        tail.empty() ? word : word + " " + tail
                    );
                }
            }
        }
        
        memo[start] = sentences; // 缓存结果
        return sentences;
    }
};
```

**关键点**：
- base case 返回 `{""}` 而不是 `{}`，这是最核心的细节
- `memo` 的 key 是位置 `start`，value 是所有从该位置开始的合法句子列表
- 时间复杂度：最坏情况答案数量本身就是指数级，记忆化只避免重复子问题，不改变最坏复杂度

### 解法3: DP预处理 + 回溯 — 加分项

**优化点**：在回溯之前，先用 139 题的 DP 判断 `s[i..n-1]` 是否可以被拆分。如果不能拆分，直接剪枝，避免无效递归。

```cpp
class Solution {
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size();
        
        // ============ 第一步: 139 题的 DP 预处理 ============
        // canBreak[i] = true 表示 s[0..i-1] 可以被拆分
        vector<bool> canBreak(n + 1, false);
        canBreak[0] = true;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (canBreak[j] && dict.count(s.substr(j, i - j))) {
                    canBreak[i] = true;
                    break;
                }
            }
        }
        
        // 如果整个字符串都不能拆分，直接返回空
        if (!canBreak[n]) return {};
        
        // ============ 第二步: 带剪枝的回溯 ============
        vector<string> result;
        vector<string> path;
        backtrack(s, 0, dict, canBreak, path, result);
        return result;
    }
    
    void backtrack(const string& s, int start, 
                   const unordered_set<string>& dict,
                   const vector<bool>& canBreak,
                   vector<string>& path, vector<string>& result) {
        if (start == (int)s.size()) {
            string sentence;
            for (int i = 0; i < (int)path.size(); i++) {
                if (i > 0) sentence += " ";
                sentence += path[i];
            }
            result.push_back(sentence);
            return;
        }
        
        for (int end = start + 1; end <= (int)s.size(); end++) {
            // 剪枝: 如果 s[end..n-1] 不可拆分，跳过
            // canBreak[end] 表示 s[0..end-1] 可拆，但我们需要的是 s[end..n-1] 可拆
            // 所以这里不能直接用 canBreak[end]，需要换一种 DP 定义
            // 或者我们用反向 DP: reachable[i] = s[i..n-1] 可拆
            // 这里简化处理：直接检查当前 word 是否在字典中就够了
            // （更精确的剪枝见下面注释）
            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                path.push_back(word);
                backtrack(s, end, dict, canBreak, path, result);
                path.pop_back();
            }
        }
    }
};
```

> **更精确的剪枝版本**：预处理 `reachEnd[i]` 表示从位置 i 开始是否可以到达末尾。在回溯时，如果 `!reachEnd[end]`，直接跳过。

### 解法4: Trie + 记忆化回溯 — 加分项

**优化点**：用 Trie 替代 HashSet，在逐字符扫描时同步走 Trie，避免反复创建子串。当 Trie 节点无对应子节点时提前终止。

```cpp
class Solution {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    
    TrieNode* root;
    unordered_map<int, vector<string>> memo;
    
    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx])
                node->children[idx] = new TrieNode();
            node = node->children[idx];
        }
        node->isEnd = true;
    }
    
    vector<string> dfs(const string& s, int start) {
        if (memo.count(start)) return memo[start];
        
        vector<string> sentences;
        if (start == (int)s.size()) {
            sentences.push_back("");
            return sentences;
        }
        
        TrieNode* node = root;
        for (int end = start; end < (int)s.size(); end++) {
            int idx = s[end] - 'a';
            if (!node->children[idx]) break; // Trie 中无此前缀，提前终止
            node = node->children[idx];
            
            if (node->isEnd) { // 找到一个完整单词
                string word = s.substr(start, end - start + 1);
                vector<string> rest = dfs(s, end + 1);
                for (const string& tail : rest) {
                    sentences.push_back(
                        tail.empty() ? word : word + " " + tail
                    );
                }
            }
        }
        
        memo[start] = sentences;
        return sentences;
    }
    
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        root = new TrieNode();
        for (const string& w : wordDict) insert(w);
        return dfs(s, 0);
    }
};
```

**关键点**：Trie 的优势在于——当某个前缀不在字典中时，可以立即 `break`，不用继续尝试更长的子串。对于字典很大的场景尤其有效。

## 解法对比

| | 暴力回溯 | 记忆化回溯 ⭐ | DP预处理+回溯 | Trie+记忆化 |
|---|---|---|---|---|
| 避免重复计算 | ✗ | ✓ memo 缓存 | 部分（剪枝） | ✓ memo 缓存 |
| 前缀剪枝 | ✗ | ✗ | ✗ | ✓ Trie break |
| 代码复杂度 | 简单 | 中等 | 中等 | 较高 |
| 面试推荐度 | 作为起点 | **首选** | 追问时说 | 追问时说 |

## 易错点

1. **base case 返回 `{}` 而不是 `{""}`**
   - ✗ `if (start == n) return {};` → 调用方的 for 循环不执行，路径丢失
   - ✓ `if (start == n) return {""};` → 调用方能正确拼接 `word + "" = word`

2. **拼接时忘记处理末尾空格**
   - ✗ `sentences.push_back(word + " " + tail);` → 最后一个单词后面会多一个空格
   - ✓ `tail.empty() ? word : word + " " + tail` → 末尾无多余空格

3. **memo 没有缓存空结果**
   - 如果某个位置无法拆分，`memo[start]` 应该存一个空 vector，否则下次又会重新计算
   - 解法2 中 `memo[start] = sentences;` 即使 sentences 为空也会缓存，这是正确的

4. **和 139 题混淆**
   - 139 题 `dp[i]` 是 bool，本题需要存所有方案列表
   - 139 题的 DP 在本题中只能做预处理剪枝，不能直接用来构建答案

## 面试追问

**Q1（基础理解）**：这道题和 139 题的区别是什么？为什么不能只用 DP？
> 139 只需要判断 true/false，一个 bool DP 就够。本题要求列举所有方案，方案数可能是指数级的，必须用回溯穷举。DP 可以辅助剪枝但不能替代回溯。

**Q2（优化）**：暴力回溯的瓶颈在哪？记忆化具体缓存了什么？
> 瓶颈在于同一个起始位置可能从不同路径到达，每次重新计算。记忆化缓存了"从位置 i 到末尾的所有合法句子列表"。注意这里缓存的是整个列表而不是 bool。

**Q3（追问）**：如果字典非常大（百万级单词），怎么优化前缀匹配？
> 用 Trie。在逐字符扫描时同步走 Trie，当某个前缀不存在时立即 break，避免无效 substr + HashSet 查询。同时 Trie 天然支持共享前缀，空间更优。

**Q4（变体）**：如果只需要返回方案数而不是所有方案，怎么做？
> 可以用纯 DP：`dp[i]` 表示 s[0..i-1] 的拆分方案数，转移方程 `dp[i] = Σ dp[j]`（对所有 j < i 且 s[j..i-1] 在字典中）。O(n²) 时间。

## 相关题型

- **139. Word Break** — 本题的简化版，只判断能否拆分。复用相同的"枚举切割位置"思路，但只用 bool DP 而不用回溯。本题可以用 139 的 DP 做预处理剪枝。
- **131. Palindrome Partitioning** — 同样是"字符串所有切割方案"，区别是判断条件从"在字典中"变成"是回文"。复用完全相同的回溯框架。
- **472. Concatenated Words** — 对字典中每个单词做 Word Break 判断，复用 139 的 DP 作为子程序。
- **212. Word Search II** — Trie + 回溯在矩阵上搜索单词，复用本题的 Trie 构建思路。