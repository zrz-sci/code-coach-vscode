/*
 * LeetCode 139: Word Break (单词拆分)
 *
 * 【题目本质】
 * 判断字符串 s 能否在某些位置切割，使得每一段都是字典中的单词。
 * 本质是"可行性判断的完全背包 DP"——字典单词可重复使用，拼接顺序有要求。
 *
 * 【解法总览】
 * 解法1: 暴力递归 (DFS)          — O(2^n) / O(n)      — 最直觉，展示问题本质
 * 解法2: 记忆化搜索 (自顶向下 DP) — O(n²·L) / O(n)    — 消除重叠子问题
 * 解法3: 递推 DP (自底向上)       — O(n·maxLen·L) / O(n) — 面试首选 ⭐
 * 解法4: DP + Trie               — O(n·maxLen) / O(Σ词长) — 进阶加分
 */

// ============================================================
// 解法1: 暴力递归 — 从左到右尝试每种前缀匹配
// 时间: O(2^n) 最坏   空间: O(n) 递归栈
//
// 【思路】
// 从位置 start 开始，尝试截取不同长度的前缀：
//   如果 s[start..end-1] 是字典单词，就递归处理 s[end..]
//   如果走到末尾返回 true，所有尝试都失败返回 false。
//
// 决策树 (s = "leetcode", dict = {"leet", "code"}):
//
//     dfs(0): "leetcode"
//     ├── "l" ✗ (不在字典)
//     ├── "le" ✗
//     ├── "lee" ✗
//     ├── "leet" ✓ → dfs(4): "code"
//     │   ├── "c" ✗
//     │   ├── "co" ✗
//     │   ├── "cod" ✗
//     │   └── "code" ✓ → dfs(8): "" → return true ✓
//     ├── "leetc" ✗
//     └── ...
//
// 瓶颈: 没有记忆化，同一个 start 可能被不同路径重复调用。
// ============================================================
class Solution1 {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        return dfs(s, 0, wordSet);
    }

private:
    bool dfs(const string& s, int start, const unordered_set<string>& wordSet) {
        if (start == (int)s.size()) return true;  // 整个字符串已匹配完

        for (int end = start + 1; end <= (int)s.size(); end++) {
            // 截取 s[start..end-1] 检查是否在字典中
            if (wordSet.count(s.substr(start, end - start))) {
                if (dfs(s, end, wordSet)) return true;
            }
        }
        return false;  // 所有前缀都不行
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 在解法1基础上加 memo 消除重叠子问题
// 时间: O(n² · L)   空间: O(n)
//   n 个不同的状态，每个状态枚举 O(n) 个终点，substr O(L)
//
// 【思路】
// 解法1的问题: 对于 s="aaab", dict=["a","aa","aaa"]
//   dfs(0) → 匹配"a"→dfs(1), 匹配"aa"→dfs(2), 匹配"aaa"→dfs(3)
//   dfs(1) → 匹配"a"→dfs(2), 匹配"aa"→dfs(3)
//   dfs(2) 被 dfs(0) 和 dfs(1) 都调用了！→ 重叠子问题
//
// 加 memo[start] 缓存: 
//   0 = 未计算, 1 = 可拆分, -1 = 不可拆分
//   避免用 bool，因为 bool 无法区分"未计算"和"结果为false"
// ============================================================
class Solution2 {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        vector<int> memo(s.size(), 0);  // 0=未访问
        return dfs(s, 0, wordSet, memo);
    }

private:
    bool dfs(const string& s, int start,
             const unordered_set<string>& wordSet, vector<int>& memo) {
        if (start == (int)s.size()) return true;
        if (memo[start] != 0) return memo[start] == 1;  // 已计算过，直接返回

        for (int end = start + 1; end <= (int)s.size(); end++) {
            if (wordSet.count(s.substr(start, end - start))
                && dfs(s, end, wordSet, memo)) {
                memo[start] = 1;
                return true;
            }
        }
        memo[start] = -1;  // 标记: 从 start 开始不可拆分
        return false;
    }
};

// ============================================================
// 解法3: 递推 DP — 自底向上填表 ⭐ 面试首选
// 时间: O(n · maxLen · L)   空间: O(n)
//   外层 n 次，内层最多 maxLen 次，每次 substr 花 O(L)
//   当 maxLen 很小 (题目限制 ≤ 20) 时，接近 O(n · L)
//
// 【思路】
// 将记忆化搜索翻译成递推:
//   dp[i] = s 的前 i 个字符 s[0..i-1] 能否被拆分
//   转移: dp[i] = OR{ dp[j] && s[j..i-1] ∈ dict }  for j ∈ [max(0,i-maxLen), i)
//   base: dp[0] = true (空前缀，拆分起点)
//
// 填表过程 (s = "leetcode", dict = {"leet", "code"}):
//
//   位置 i:   0   1   2   3   4   5   6   7   8
//   dp[i]:  [T]  F   F   F   T   F   F   F   T
//            ↑                ↑                ↑
//         空串(base)      dp[0]=T          dp[4]=T
//                        +s[0..3]="leet"  +s[4..7]="code"
//
// dp[4] 怎么得到的:
//   j=0: dp[0]=true, s.substr(0,4)="leet" ∈ dict → dp[4]=true ✓
//
// dp[8] 怎么得到的:
//   j=4: dp[4]=true, s.substr(4,4)="code" ∈ dict → dp[8]=true ✓
//
// 答案: dp[8] = true
// ============================================================
class Solution3 {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int n = s.size();
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());

        // 预计算最长单词长度，用来剪枝内层循环
        // 因为 s[j..i-1] 的长度不可能超过字典中最长单词
        int maxLen = 0;
        for (const string& w : wordDict) {
            maxLen = max(maxLen, (int)w.size());
        }

        vector<bool> dp(n + 1, false);
        dp[0] = true;  // 空前缀是合法起点

        for (int i = 1; i <= n; i++) {
            // j 是"最后一个单词"的起始位置
            // 单词长度 = i - j，不超过 maxLen
            for (int j = max(0, i - maxLen); j < i; j++) {
                if (dp[j] && wordSet.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;  // 找到一种拆法就够了，无需继续
                }
            }
        }

        return dp[n];
    }
};

// ============================================================
// 解法4: DP + Trie — 用前缀树优化单词匹配
// 时间: O(n · maxLen)   空间: O(Σ词长) Trie 存储
//
// 【思路】
// 解法3 每次调用 s.substr(j, i-j) 创建新字符串再查 HashSet，
// substr 本身是 O(长度) 的。
//
// 用 Trie 可以避免 substr: 从每个可达位置 i 出发，
// 沿 Trie 逐字符向后匹配 s[i], s[i+1], ...
// 碰到 isEnd=true 就标记 dp[j+1] = true。
// 匹配长度最多 maxLen，且 Trie 中无此路径时提前 break。
//
// Trie 结构 (dict = {"leet", "code"}):
//   root
//   ├── c → o → d → e*       (* 表示 isEnd)
//   └── l → e → e → t*
//
// 从位置 0 出发沿 Trie 走:
//   s[0]='l' → root->l → s[1]='e' → l->e → s[2]='e' → e->e → s[3]='t' → e->t (isEnd!)
//   → dp[4] = true
//
// 从位置 4 出发沿 Trie 走:
//   s[4]='c' → root->c → s[5]='o' → c->o → s[6]='d' → o->d → s[7]='e' → d->e (isEnd!)
//   → dp[8] = true
// ============================================================
class Solution4 {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };

    bool wordBreak(string s, vector<string>& wordDict) {
        // 建 Trie
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
            if (!dp[i]) continue;  // 位置 i 不可达，跳过

            // 从位置 i 开始，沿 Trie 逐字符匹配
            TrieNode* node = root;
            for (int j = i; j < n; j++) {
                int idx = s[j] - 'a';
                if (!node->children[idx]) break;  // Trie 中无此前缀，提前终止
                node = node->children[idx];
                if (node->isEnd) {
                    // s[i..j] 是字典中的单词
                    dp[j + 1] = true;
                }
            }
        }

        // 释放 Trie 内存（面试中可省略，但写上更规范）
        // 实际中可用 unique_ptr 或统一内存池
        return dp[n];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 核心思想 | 面试建议 |
// |------|------|------|----------|----------|
// | 暴力递归 | O(2^n) | O(n) | DFS穷举 | 展示理解 |
// | 记忆化   | O(n²·L) | O(n) | +memo消重 | 过渡讲解 |
// | 递推DP   | O(n·maxLen·L) | O(n) | 自底向上 | ⭐首选 |
// | DP+Trie  | O(n·maxLen) | O(Σ词长) | Trie优化匹配 | 追问加分 |
//
// 面试中直接写解法3（递推DP + HashSet + maxLen剪枝），代码最简洁。
// 如果追问"字典很大怎么办"，再升级到解法4。
//
// 【易错点】
//
// 1. dp[0] 忘记初始化为 true
//    ✗ dp[0] = false → 所有 dp[i] 都推不出来，全 false
//    ✓ dp[0] = true  → 空前缀是合法起点
//
// 2. substr 参数: s.substr(pos, len)，第二个参数是长度不是终点
//    ✗ s.substr(j, i)      → 当 i > 实际长度时越界或截取过多
//    ✓ s.substr(j, i - j)  → 从 j 开始截取长度为 i-j
//
// 3. 记忆化用 bool 数组无法区分"未计算"和"结果为false"
//    ✗ vector<bool> memo(n, false) → memo[i]=false 到底是没算还是不可拆？
//    ✓ vector<int> memo(n, 0)     → 0=未算, 1=可拆, -1=不可拆
//
// 4. 内层循环没有 maxLen 剪枝
//    ✗ for(j=0; j<i; j++) → 当 s 很长时，j 从 0 到 i 遍历，浪费
//    ✓ for(j=max(0,i-maxLen); j<i; j++) → 单词长度不超过 maxLen
//
// 5. 找到 dp[i]=true 后没有 break
//    虽然不影响正确性，但极端情况（如 s 全是 'a'）差异很大
//
// 【面试追问 — 递进链】
//
// Q1: 暴力递归为什么超时？画出重叠子问题的例子。
//     → s="aaab", dict=["a","aa","aaa"]
//     → dfs(2) 被 dfs(0)→"aa"→dfs(2) 和 dfs(0)→"a"→dfs(1)→"a"→dfs(2) 
//       两条路径调用
//
// Q2: 这道题和完全背包的关系？
//     → 单词可重复使用 = 完全背包
//     → 但拼接有顺序要求 = 排列型：外层循环位置(容量)，内层循环单词(物品)
//     → 如果调换内外层循环，就变成组合型，不保证顺序
//
// Q3: 如果要输出所有拆分方案（LeetCode 140）怎么改？
//     → 用回溯 + 记忆化。memo[i] 存的不是 bool，而是从位置 i 开始的所有拆分结果
//     → 或先跑一遍本题判断可行性，再回溯收集路径（剪枝）
//
// Q4: 如果字典有百万个单词、大量公共前缀？
//     → 用 Trie（解法4），共享公共前缀存储，匹配时逐字符走，无需 substr
//
// ============================================================
// 【空间优化说明】
//
// 解法3/4 的 DP 数组 dp[i] 已经是一维 O(n) 空间。
// dp[i] 的转移依赖所有 dp[j] (j < i 且 i-j <= maxLen)，不是只依赖前 1~2 个
// 状态，因此无法用滚动变量（如 prev/curr 两个变量）进一步压缩。
//
// 具体来说：dp[i] = OR{ dp[j] && s[j..i-1] ∈ dict }，j 的范围可以跨越
// 整个 maxLen 长度。例如字典中有长度为 1 和长度为 20 的单词时，
// dp[21] 同时依赖 dp[20] 和 dp[1]，两端都需要保留。
//
// 结论：O(n) 一维 DP 已是最优空间，无法进一步压缩。
// ============================================================
