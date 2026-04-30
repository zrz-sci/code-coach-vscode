# 472. 连接词

## 核心思路

本质上是 **Word Break（单词拆分）问题的批量版**：对数组中的每个单词，判断它能否由数组中**其他更短的单词**拼接而成（至少两个）。关键在于高效判断"一个字符串能否被一组单词拼接"。

## 思维链

1. **读完题第一反应**：对每个单词 w，尝试把它拆成多个片段，每个片段都在 words 里且比 w 短。这不就是 LeetCode 139 Word Break 吗？只不过要对每个单词都做一遍。

2. **暴力思路**：把所有单词放进 HashSet，然后对每个单词做一次 Word Break 的 DP 判断。但直接这么做有个坑——一个单词自身也在集合里，拆分时不能"整个用自己"，必须拆成至少两段。

3. **怎么避免"用自己拆自己"？**：按长度排序！先处理短的单词，再处理长的。处理到单词 w 时，集合中只有比 w 短（或等长但之前加入的）的单词。如果 w 能被当前集合中的单词拼成，w 就是连接词，不加入集合；否则 w 是"基础词"，加入集合。

4. **Word Break DP 怎么做？**：对单词 w，定义 `dp[i]` = w 的前 i 个字符能否被集合中的单词拼成。转移：`dp[i] = true` 当存在某个 j < i 使得 `dp[j] == true` 且 `w[j..i-1]` 在集合中。

5. **进一步优化——Trie**：HashSet 查子串需要 O(L) 构造子串 + O(L) 哈希查找。Trie 可以避免构造子串，边遍历字符边在 Trie 上走，找到所有可能的前缀匹配点。

6. **DFS + 记忆化 也可以**：递归地尝试在每个位置切一刀，前半部分在字典里，后半部分递归判断。加记忆化避免重复计算。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 + HashSet + DP | 按长度排序，逐词做 Word Break DP | O(N·L²) | O(N·L) | ⭐ 必须写出 |
| Trie + DFS/DP | Trie 加速前缀匹配，DFS 递归拆分 | O(N·L²) 常数更优 | O(N·L) | 加分项 |
| HashSet + DFS + 记忆化 | 直接 DFS 尝试拆分 | O(N·L²) | O(N·L) | 能说出即可 |

> N = 单词个数, L = 单词最大长度 (≤30)

## 关键提示

1. **这道题是 Word Break (139) 的升级版**——如果你会 139，这题就是"排序 + 对每个词做一次 139"。
2. **按长度排序是关键技巧**：保证处理到一个单词时，所有比它短的单词都已在字典中。这自然解决了"不能用自己拆自己"的问题。
3. **DP 转移时内层循环的范围**：`j` 不需要从 0 遍历到 i，因为单词最长 30，所以只需检查 `max(0, i-maxLen)` 到 `i-1`，这是一个重要的常数优化。
4. **空字符串的特殊处理**：如果 words 中有空字符串，它不能作为连接词的组成部分（题目说"较短单词"），需要跳过。
5. **Trie 的优势在于枚举所有前缀匹配**：从位置 i 开始在 Trie 上走，每遇到一个 `isEnd` 就产生一个切分点，无需逐个长度构造子串。

```
示例 "catsdogcats" 的 DP 过程:
字典: {cat, cats, dog}

  c a t s d o g c a t s
  0 1 2 3 4 5 6 7 8 9 10
dp: T F F T T F F F F T T T
         ↑ ↑             ↑ ↑
      cat| cats+dog    cat| cats+dog+cats
         |                 |
    dp[3]=T: "cat"∈dict   dp[10]=T: "cats"∈dict && dp[7]=T
    dp[4]=T: "cats"∈dict  dp[11]=T: "cats"∈dict && dp[7]=T
    dp[7]=T: "dog"∈dict && dp[4]=T
```

## 解法详解

### 解法1: 排序 + HashSet + DP — O(N·L²) / O(N·L) ⭐ 面试首选

**思考过程**：

连接词一定比组成它的单词更长（或等长但那不可能，因为至少两个非空词）。所以如果按长度从短到长排序，处理到一个单词时，所有可能组成它的"基础词"已经在字典中了。

对每个单词做 Word Break 判断（经典 DP）：
- `dp[i]` 表示 word 的前 i 个字符能否被字典中的词拼成
- 如果 `dp[len]` 为 true，说明整个词能被拼成 → 是连接词
- 否则把这个词加入字典作为"基础词"

```cpp
class Solution {
public:
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        // 按长度排序：短词先处理，作为"基础词"进字典
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });
        
        vector<string> result;
        unordered_set<string> wordSet;
        
        for (const string& word : words) {
            // 空字符串跳过
            if (word.empty()) continue;
            
            // 对当前单词做 Word Break 判断
            if (canBreak(word, wordSet)) {
                result.push_back(word);
            } else {
                // 不是连接词，加入字典作为基础词
                wordSet.insert(word);
            }
        }
        return result;
    }
    
private:
    bool canBreak(const string& word, const unordered_set<string>& wordSet) {
        if (wordSet.empty()) return false;
        
        int n = word.size();
        // dp[i] = word的前i个字符能否被字典中的词拼成
        vector<bool> dp(n + 1, false);
        dp[0] = true;  // 空前缀可以被拼成（base case）
        
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                // 如果前j个字符能拼成，且 word[j..i-1] 在字典中
                if (dp[j] && wordSet.count(word.substr(j, i - j))) {
                    dp[i] = true;
                    break;  // 已经确定dp[i]=true，不需要继续尝试其他j
                }
            }
        }
        return dp[n];
    }
};
```

**关键点**：
- 排序保证了"用来拼的词一定比当前词短"，自动避免了"用自己拼自己"。
- `dp[0] = true` 是关键初始化：空前缀是合法的起点。
- 虽然时间复杂度写 O(N·L²)，但 L ≤ 30，所以内层循环最多 30×30 = 900 次，实际非常快。

### 解法2: Trie + DFS — O(N·L²) / O(N·L)

**从解法1优化**：解法1中 `wordSet.count(word.substr(j, i-j))` 每次需要构造子串并做哈希查找。Trie 可以从某个位置开始，逐字符在树上走，同时发现所有可能的切分点，避免反复构造子串。

```cpp
class Solution {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    
    TrieNode* root = new TrieNode();
    
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
    
    // DFS: 从 word[start] 开始，判断 word[start..end] 能否被 Trie 中的词拼成
    // count 记录已经用了几个词
    bool dfs(const string& word, int start, vector<int>& memo) {
        if (start == (int)word.size()) return true;
        if (memo[start] != -1) return memo[start];
        
        TrieNode* node = root;
        for (int i = start; i < (int)word.size(); i++) {
            int idx = word[i] - 'a';
            if (!node->children[idx]) break;  // Trie 中无此路径
            node = node->children[idx];
            
            if (node->isEnd) {
                // word[start..i] 是字典中的一个词，递归处理剩余部分
                if (dfs(word, i + 1, memo)) {
                    memo[start] = 1;
                    return true;
                }
            }
        }
        memo[start] = 0;
        return false;
    }
    
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        // 按长度排序
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });
        
        vector<string> result;
        for (const string& word : words) {
            if (word.empty()) continue;
            
            // memo[i] = -1(未访问), 0(不可拼), 1(可拼)
            vector<int> memo(word.size(), -1);
            if (dfs(word, 0, memo)) {
                result.push_back(word);
            } else {
                insert(word);  // 不是连接词，加入 Trie
            }
        }
        return result;
    }
};
```

**关键点**：
- Trie 的优势：从位置 `start` 开始沿 Trie 走，每遇到 `isEnd=true` 就是一个合法切分点。一次遍历就能找到所有以 `word[start]` 开头的字典词。
- 记忆化 `memo[start]`：相同起始位置不会重复计算。
- 同样利用排序保证短词先入 Trie。

### 解法3: HashSet + DFS + 记忆化 — O(N·L²) / O(N·L)

**不排序版本**：先把所有词放入 HashSet，对每个词做 DFS 判断能否被拆分为至少两个字典中的词。

```cpp
class Solution {
public:
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        unordered_set<string> wordSet(words.begin(), words.end());
        vector<string> result;
        
        for (const string& word : words) {
            if (word.empty()) continue;
            // 判断时需要防止用自己拆自己：要求至少拆成2段
            vector<int> memo(word.size(), -1);
            if (dfs(word, 0, wordSet, memo)) {
                result.push_back(word);
            }
        }
        return result;
    }
    
private:
    bool dfs(const string& word, int start, 
             const unordered_set<string>& wordSet, vector<int>& memo) {
        if (start == (int)word.size()) return true;
        if (memo[start] != -1) return memo[start];
        
        // 注意：i 最多到 word.size()-1（不取整个剩余部分，除非 start > 0）
        // 当 start == 0 时，不能取整个 word（否则就是用自己拼自己）
        int end = (start == 0) ? (int)word.size() - 1 : (int)word.size();
        
        for (int i = start + 1; i <= end; i++) {
            string prefix = word.substr(start, i - start);
            if (wordSet.count(prefix) && dfs(word, i, wordSet, memo)) {
                memo[start] = 1;
                return true;
            }
        }
        memo[start] = 0;
        return false;
    }
};
```

**关键点**：
- 不排序的代价是：需要额外逻辑防止"用自己拆自己"。当 `start == 0` 时，第一段不能取整个 word。
- 相比排序版，逻辑稍复杂但思路更直接。

## 解法对比

| | 排序 + HashSet + DP | Trie + DFS | HashSet + DFS |
|---|---|---|---|
| 核心数据结构 | HashSet + DP数组 | Trie + 记忆化 | HashSet + 记忆化 |
| 需要排序？ | ✅ 必须 | ✅ 必须 | ❌ 不需要 |
| 防止用自己 | 排序自动解决 | 排序自动解决 | 手动判断 start==0 |
| 代码复杂度 | 最简单 | 最复杂（Trie实现） | 中等 |
| 实际速度 | 快（L≤30，substr开销小） | 更快（避免substr） | 快 |
| **面试推荐** | ⭐ 首选 | 加分项 | 备选 |

**选择建议**：面试中首选解法1（排序+DP），代码最短最清晰。如果面试官追问优化或你想展示 Trie 能力，再给解法2。

## 易错点

1. **忘记排序导致"用自己拼自己"**
   - ✗ 不排序直接把所有词放入 set，对 "cat" 做 Word Break → `dp[3] = true`（cat 在 set 里），但 "cat" 本身不是连接词
   - ✓ 按长度排序，处理 "cat" 时 set 为空或只有更短的词，不会匹配自己

2. **`dp[0]` 初始化错误**
   - ✗ `dp[0] = false` → 永远无法启动转移
   - ✓ `dp[0] = true`，空前缀是合法起点

3. **空字符串未跳过**
   - ✗ 空字符串 `""` 进入 canBreak，`dp[0]=true` 且 `n=0`，直接返回 true → 被当成连接词
   - ✓ 遇到空字符串直接 `continue`

4. **DFS 版本忘记记忆化**
   - ✗ 纯 DFS 不加 memo → 指数级时间复杂度，TLE
   - ✓ `memo[start]` 记录从 start 位置开始能否拼成

5. **substr 的参数搞混**
   - ✗ `word.substr(j, i)` — 第二个参数是长度不是结束位置
   - ✓ `word.substr(j, i - j)` — 从 j 开始取 i-j 个字符

## 面试追问

**Q1: 这题和 Word Break (139) 有什么关系？**
> Word Break 是判断一个字符串能否被给定字典拆分。本题是对数组中的每个词做一次 Word Break，额外约束是"至少拆两段"且"字典就是数组中的其他词"。排序后逐词处理，自然满足"用比自己短的词"。

**Q2: 为什么按长度排序就能避免"用自己拼自己"？**
> 因为连接词至少由两个非空短词组成，所以连接词一定比组成它的每个词都长。排序后处理到 word 时，字典中只有比 word 短的词（等长词因为都是不同的，不可能两个短词拼成等长的），所以不会用自己拼自己。

**Q3: 如果单词长度可以很长（比如到 10^5），解法有什么变化？**
> 当 L 很大时，O(L²) 的 DP 内层循环就成了瓶颈。Trie 的优势会更明显——从每个位置出发在 Trie 上走，只在 Trie 路径存在时才继续，相当于自动剪枝。另外可以用 **Aho-Corasick 自动机**（多模式匹配）来一次性找到所有匹配位置。

**Q4: 如果允许单词重叠（比如 "abc" + "cab" 拼成 "abcab" 但中间 "c" 共用），该怎么做？**
> 那就不是简单的拼接问题了，变成了"字符串覆盖"问题，需要考虑前后缀重叠。可以用 KMP 的 failure function 或后缀自动机来处理。

## 相关题型

- **139. Word Break** — 本题的单一版本。学会 139 的 DP 后，472 只需要加一个排序+循环外壳。代码差异：139 的字典是给定的，472 的字典是动态构建的。
- **140. Word Break II** — 139 的"输出所有方案"版本，需要回溯收集路径。复用同样的 DP/DFS 框架，区别在于收集路径而非只返回 bool。
- **208. Implement Trie** — Trie 的基础实现，解法2 直接复用 Trie 的 insert 和查找逻辑。
- **212. Word Search II** — 也是 Trie + DFS 的经典应用，在矩阵上搜索多个单词。复用 Trie 加速前缀匹配的技巧。