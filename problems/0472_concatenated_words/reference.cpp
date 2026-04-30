/*
 * LeetCode 472: 连接词 (Concatenated Words)
 *
 * 【题目本质】
 * Word Break (139) 的批量版：对数组中每个单词，判断能否被数组中其他更短的单词拼接而成（至少两个）。
 *
 * 【解法总览】
 * 解法1: 排序 + HashSet + DP     — O(N·L²) / O(N·L) — 面试首选 ⭐
 * 解法2: Trie + DFS + 记忆化      — O(N·L²) / O(N·L) — 加分项，Trie 加速前缀匹配
 * 解法3: HashSet + DFS + 记忆化   — O(N·L²) / O(N·L) — 不排序版本，需手动防自拼
 *
 * 其中 N = 单词个数 (≤10^4)，L = 单词最大长度 (≤30)
 */

// ============================================================
// 解法1: 排序 + HashSet + DP — 面试首选
// 时间: O(N·L²)  空间: O(N·L)
//
// 【思路】
// 核心观察：连接词一定比组成它的每个基础词更长。
// 所以按长度从短到长排序后，处理到一个单词时，
// 字典中只有比它短的词（不可能用自己拼自己）。
//
// 对每个单词做一次 Word Break 的经典 DP：
//   dp[i] = 前i个字符能否被字典中的词拼成
//   dp[0] = true（空前缀，base case）
//   dp[i] = true，若存在 j 使 dp[j]=true 且 word[j..i-1] ∈ 字典
//
// 如果 dp[len]=true，说明该词是连接词；否则把它加入字典。
//
// DP 填表过程示例 ("catsdogcats", 字典 = {cat, cats, dog}):
//
//   索引:  0  1  2  3  4  5  6  7  8  9 10 11
//   字符:     c  a  t  s  d  o  g  c  a  t  s
//   dp:    T  F  F  T  T  F  F  T  F  F  T  T
//                   ↑  ↑        ↑        ↑  ↑
//              "cat"匹配  "dog"匹配  "cat"匹配
//             "cats"匹配           "cats"匹配
//
//   dp[3]=T: dp[0]=T, "cat"∈dict ✓
//   dp[4]=T: dp[0]=T, "cats"∈dict ✓
//   dp[7]=T: dp[4]=T, "dog"∈dict ✓
//   dp[10]=T: dp[7]=T, "cat"∈dict ✓
//   dp[11]=T: dp[7]=T, "cats"∈dict ✓  → 是连接词！
// ============================================================
class Solution1 {
public:
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        // 按长度排序：短词先入字典，保证处理长词时不会自拼
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });
        
        vector<string> result;
        unordered_set<string> wordSet;
        
        for (const string& word : words) {
            if (word.empty()) continue;  // 空字符串不是有效基础词也不是连接词
            
            if (canBreak(word, wordSet)) {
                result.push_back(word);
                // 注意：连接词不加入字典
                // 因为如果后面的词需要它，它的组成部分已经在字典里了
            } else {
                wordSet.insert(word);  // 基础词，加入字典
            }
        }
        return result;
    }
    
private:
    bool canBreak(const string& word, const unordered_set<string>& wordSet) {
        if (wordSet.empty()) return false;  // 字典为空，不可能拼成
        
        int n = word.size();
        // dp[i] = word 的前 i 个字符能否被字典中的词拼成
        vector<bool> dp(n + 1, false);
        dp[0] = true;  // 空前缀 — 这是启动转移的关键 base case
        
        for (int i = 1; i <= n; i++) {
            // j 是切分点：word[j..i-1] 作为最后一个词
            for (int j = 0; j < i; j++) {
                // dp[j] 为 true 意味着前 j 个字符已经能被拼成
                // 再检查 word[j..i-1] 是否在字典中
                if (dp[j] && wordSet.count(word.substr(j, i - j))) {
                    dp[i] = true;
                    break;  // 只要找到一种拆法就够了
                }
            }
        }
        return dp[n];  // 整个单词能否被拼成
    }
};


// ============================================================
// 解法2: Trie + DFS + 记忆化 — Trie 加速前缀匹配
// 时间: O(N·L²)  空间: O(N·L)
//
// 【思路】
// 解法1中 wordSet.count(word.substr(j, i-j)) 每次需要：
//   1. 构造子串 O(L)
//   2. 计算哈希 O(L)
//   3. 查找 O(1) 均摊
//
// Trie 的优势：从位置 start 开始沿 Trie 逐字符走，
// 每遇到 isEnd=true 就是一个合法切分点。
// 一次遍历同时发现所有以 word[start] 开头的字典词前缀匹配。
//
// 决策树示例 ("catsdogcats"):
//
//   start=0: Trie 上走 c→a→t (isEnd!) → 递归 start=3
//                         →s (isEnd!) → 递归 start=4
//   start=3: Trie 上走 s→d... 无匹配 → 失败
//   start=4: Trie 上走 d→o→g (isEnd!) → 递归 start=7
//   start=7: Trie 上走 c→a→t (isEnd!) → 递归 start=10
//                         →s (isEnd!) → 递归 start=11
//   start=10: Trie 上走 s → 无 isEnd → 失败
//   start=11: == word.size() → 成功！ 返回 true
// ============================================================
class Solution2 {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    
    TrieNode* root;
    
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
    
    // 从 word[start] 开始，在 Trie 上走，判断后缀能否被拼成
    bool dfs(const string& word, int start, vector<int>& memo) {
        if (start == (int)word.size()) return true;  // 整个单词都被覆盖了
        if (memo[start] != -1) return memo[start];   // 记忆化：避免重复计算
        
        TrieNode* node = root;
        for (int i = start; i < (int)word.size(); i++) {
            int idx = word[i] - 'a';
            if (!node->children[idx]) break;  // Trie 中无此路径，剪枝
            node = node->children[idx];
            
            // word[start..i] 是字典中的一个完整词
            if (node->isEnd) {
                if (dfs(word, i + 1, memo)) {
                    memo[start] = 1;
                    return true;
                }
            }
        }
        memo[start] = 0;  // 从 start 开始无法拼成
        return false;
    }
    
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        root = new TrieNode();
        
        // 排序：短词先入 Trie，处理长词时不会自拼
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });
        
        vector<string> result;
        for (const string& word : words) {
            if (word.empty()) continue;
            
            // memo[i]: -1=未访问, 0=不可拼, 1=可拼
            vector<int> memo(word.size(), -1);
            if (dfs(word, 0, memo)) {
                result.push_back(word);
            } else {
                insert(word);  // 基础词，加入 Trie
            }
        }
        return result;
    }
};


// ============================================================
// 解法3: HashSet + DFS + 记忆化（不排序版本）
// 时间: O(N·L²)  空间: O(N·L)
//
// 【思路】
// 如果不想排序，可以先把所有词放入 HashSet，
// 然后对每个词做 DFS 判断能否拆成至少两段。
//
// 关键问题：怎么防止"用自己拼自己"？
// → 当 start == 0 时，第一段不能是整个 word。
//   即 i 最多到 word.size()-1，不能取 word.size()。
// → 当 start > 0 时，最后一段可以到 word.size()，
//   因为前面已经至少用了一个词了。
//
// 这个 "start == 0 时少取一个字符" 的技巧是关键。
// ============================================================
class Solution3 {
public:
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        unordered_set<string> wordSet(words.begin(), words.end());
        vector<string> result;
        
        for (const string& word : words) {
            if (word.empty()) continue;
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
        
        // 关键：start==0 时不能取整个 word（防止自拼）
        // start>0 时可以取到末尾（前面已用至少一个词）
        int maxEnd = (start == 0) ? (int)word.size() - 1 : (int)word.size();
        
        for (int i = start + 1; i <= maxEnd; i++) {
            string sub = word.substr(start, i - start);
            if (wordSet.count(sub) && dfs(word, i, wordSet, memo)) {
                memo[start] = 1;
                return true;
            }
        }
        memo[start] = 0;
        return false;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1(排序+DP)     | 解法2(Trie+DFS)     | 解法3(HashSet+DFS) |
// |-------------|-------------------|--------------------|--------------------|
// | 需要排序     | ✅                | ✅                 | ❌                 |
// | 防自拼方式   | 排序自动解决       | 排序自动解决        | start==0 时少取    |
// | 代码量       | 最少              | 最多(Trie实现)      | 中等               |
// | 实际性能     | 很快(L≤30)        | 更快(避免substr)    | 很快               |
// | 面试推荐     | ⭐ 首选           | 加分项              | 备选               |
//
// 【易错点】
//
// 1. dp[0] 初始化错误:
//    ✗ dp[0] = false → 永远无法启动转移，所有词都判为非连接词
//    ✓ dp[0] = true → 空前缀可以作为合法起点
//
// 2. 忘记排序导致自拼:
//    ✗ 不排序 + 全部入 set + Word Break → "cat" 被判为连接词(自己匹配自己)
//    ✓ 排序后逐词处理，处理 "cat" 时 set 中无 "cat"
//
// 3. 空字符串未跳过:
//    ✗ "" 进入 canBreak，dp[0]=true 且 n=0 → dp[0]=true → 返回 true
//    ✓ 遇到 word.empty() 直接 continue
//
// 4. DFS 版本忘记记忆化:
//    ✗ 不加 memo → 对 "aaa...a"(30个a) 可能指数级递归
//    ✓ memo[start] 缓存结果，每个位置最多计算一次
//
// 5. substr 参数搞混:
//    ✗ word.substr(j, i) — 第二个参数是结束位置
//    ✓ word.substr(j, i - j) — 第二个参数是长度
//
// 6. 连接词也加入字典:
//    ✗ 无论是否为连接词都 insert → 可能让后续判断产生不必要的复杂性
//    ✓ 连接词不加入字典（它的组成部分已在字典中，足够拼出依赖它的更长词）
//    注意：其实连接词加不加入字典对正确性没影响，但不加更清晰。
//
// 【面试追问】
//
// Q1: 这题和 Word Break (139) 有什么关系？
//     → 139 是判断一个字符串能否被给定字典拆分。
//       472 是"排序 + 对每个词做一次 139"，额外约束是字典动态构建、至少拆两段。
//
// Q2: 为什么排序就能避免自拼？
//     → 连接词 = 至少两个非空短词拼成，所以 len(连接词) > len(任一组成词)。
//       按长度排序后，处理到 word 时字典中只有更短的词，自然不含 word 自己。
//
// Q3: L 很大时（如 10^5）怎么优化？
//     → Trie 的优势更明显：沿 Trie 走，路径不存在就立刻停。
//       极端情况可考虑 Aho-Corasick 多模式匹配，一次扫描找出所有匹配位置。
//
// Q4: 能否不排序但用 DP？
//     → 可以，把所有词放入 set，对每个词做 DP 判断。
//       DP 内层循环时确保 word[0..n-1] 整个不被作为一次匹配（即 j=0 到 i=n 的情况跳过）。
//       解法3 的 DFS 版本就是这个思路。
// ============================================================
