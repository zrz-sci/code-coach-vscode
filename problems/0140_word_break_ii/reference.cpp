/*
 * LeetCode 140: Word Break II (单词拆分 II)
 *
 * 【题目本质】
 * 在字符串 s 中找出所有合法的单词分割方式，使得每一段都是字典中的单词。
 * 这是 139 题(只判断能否拆分)的扩展：不仅要判断，还要列举所有方案。
 *
 * 【与 139 题的关键区别】
 * 139: dp[i] = bool → 只需要 O(n²) DP
 * 140: 需要所有方案 → 必须回溯穷举，DP 只能辅助剪枝
 *
 * 【解法总览】
 * 解法1: 暴力回溯 (DFS)                  — O(2^n·n) / O(n)     — 最直觉
 * 解法2: 记忆化回溯 (DFS + memo)          — O(2^n·n) / O(2^n·n) — 面试首选 ⭐
 * 解法3: DP预处理 + 回溯                  — O(2^n·n) / O(2^n·n) — 追问剪枝
 * 解法4: Trie + 记忆化回溯                — O(2^n·n) / O(2^n·n) — 追问大字典
 */

/*
 * 决策树可视化 (以示例1为例):
 * s = "catsanddog", dict = {"cat","cats","and","sand","dog"}
 *
 *                        dfs(0)
 *                       /       \
 *            "cat"+dfs(3)     "cats"+dfs(4)
 *                |                 |
 *          "sand"+dfs(7)     "and"+dfs(7)    ← dfs(7) 出现两次!
 *                |                 |
 *           "dog"+dfs(10)    "dog"+dfs(10)   ← dfs(10) 出现两次!
 *                |                 |
 *              [完成]            [完成]
 *
 * 输出: ["cat sand dog", "cats and dog"]
 *
 * 记忆化的价值: dfs(7) 和 dfs(10) 各只需计算一次
 */

// ============================================================
// 解法1: 暴力回溯 — 不带记忆化的 DFS
// 时间: O(2^n · n)  空间: O(n) 递归栈 + O(结果总长) 输出
//
// 【思路】
// 最直觉的做法：从位置0开始，枚举所有可能的第一个单词，
// 匹配成功就递归处理剩余部分。到达末尾说明找到一种合法分割。
// 这就是标准的回溯框架：选择 → 递归 → 撤销
//
// 【瓶颈】
// 没有任何缓存，同一个起始位置可能从不同路径被重复访问。
// 例如上面决策树中 dfs(7) 被调用了两次，每次都重新计算。
// ============================================================
class Solution1 {
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        vector<string> result;
        vector<string> path; // 当前路径上已选的单词
        dfs(s, 0, dict, path, result);
        return result;
    }

private:
    void dfs(const string& s, int start,
             const unordered_set<string>& dict,
             vector<string>& path, vector<string>& result) {
        // base case: 到达末尾，路径上的单词构成一个合法句子
        if (start == (int)s.size()) {
            string sentence;
            for (int i = 0; i < (int)path.size(); i++) {
                if (i > 0) sentence += " ";
                sentence += path[i];
            }
            result.push_back(sentence);
            return;
        }

        // 枚举从 start 开始的所有可能单词长度
        for (int end = start + 1; end <= (int)s.size(); end++) {
            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                path.push_back(word);             // 选择
                dfs(s, end, dict, path, result);  // 递归到下一个位置
                path.pop_back();                  // 撤销选择
            }
        }
    }
};

// ============================================================
// 解法2: 记忆化回溯 — DFS + memo ⭐ 面试首选
// 时间: O(2^n · n) 最坏  空间: O(2^n · n)
//
// 【思路】
// 解法1的瓶颈：同一个 start 位置可能从多条路径到达，每次重算。
// 解决：用 memo[start] 缓存 "s[start..n-1] 的所有合法句子列表"。
//
// 与解法1的核心区别：
// - 函数签名变了：dfs 返回 vector<string>，而不是 void + 外部收集
// - 加了 memo 判断：算过直接返回
//
// 【关键细节】
// base case 必须返回 {""} 而不是 {}。
// 原因：调用方需要在返回结果前面拼接单词。如果返回 {}，
// for 循环不执行，等于这条路径被丢弃了。
// 返回 {""}，调用方才能拼出 word + "" = word（最后一个单词）。
// ============================================================
class Solution2 {
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        unordered_map<int, vector<string>> memo;
        return dfs(s, 0, dict, memo);
    }

private:
    // 返回 s[start..n-1] 的所有合法句子
    vector<string> dfs(const string& s, int start,
                       const unordered_set<string>& dict,
                       unordered_map<int, vector<string>>& memo) {
        // 命中缓存 → 直接返回，避免重复计算
        if (memo.count(start)) return memo[start];

        vector<string> sentences;

        // base case: 到达末尾
        // 为什么返回 {""} 不是 {}？
        // 因为调用方代码: sentences.push_back(word + " " + tail)
        // 如果 rest 为空列表 {}，for 循环不执行 → 路径丢失
        // 如果 rest = {""}，for 循环执行一次 → word 被正确添加
        if (start == (int)s.size()) {
            sentences.push_back("");
            return sentences;
        }

        for (int end = start + 1; end <= (int)s.size(); end++) {
            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                // 递归获取剩余部分的所有句子
                vector<string> rest = dfs(s, end, dict, memo);
                for (const string& tail : rest) {
                    // 拼接当前单词和后续句子
                    // tail 为空串时不加空格（最后一个单词的情况）
                    if (tail.empty()) {
                        sentences.push_back(word);
                    } else {
                        sentences.push_back(word + " " + tail);
                    }
                }
            }
        }

        // 缓存结果（即使 sentences 为空也缓存，避免下次重算）
        memo[start] = sentences;
        return sentences;
    }
};

// ============================================================
// 解法3: DP预处理 + 带剪枝的回溯
// 时间: O(n² + 2^n·n)  空间: O(n + 2^n·n)
//
// 【思路】
// 先用 139 题的 DP 预处理: reachable[i] 表示 s[i..n-1] 是否可以被拆分。
// 然后在回溯时，如果 reachable[end] == false，直接跳过该分支。
//
// 这在"大部分位置不可拆分"的场景下效果显著：
// 例如 s = "catsandog"，在发现 "og" 不可拆后，
// 所有到达 "og" 位置的路径都被提前剪掉。
// ============================================================
class Solution3 {
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size();

        // 第一步: 预处理 reachable[i] = s[i..n-1] 是否可拆
        // 从右往左 DP: reachable[n] = true (空串可拆)
        vector<bool> reachable(n + 1, false);
        reachable[n] = true;
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i + 1; j <= n; j++) {
                if (reachable[j] && dict.count(s.substr(i, j - i))) {
                    reachable[i] = true;
                    break;
                }
            }
        }

        // 整个字符串不可拆 → 直接返回空
        if (!reachable[0]) return {};

        // 第二步: 带剪枝的回溯
        vector<string> result;
        vector<string> path;
        backtrack(s, 0, dict, reachable, path, result);
        return result;
    }

private:
    void backtrack(const string& s, int start,
                   const unordered_set<string>& dict,
                   const vector<bool>& reachable,
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
            // 剪枝: s[end..n-1] 不可拆 → 跳过
            // 这是本解法相比解法1的核心改进
            if (!reachable[end]) continue;

            string word = s.substr(start, end - start);
            if (dict.count(word)) {
                path.push_back(word);
                backtrack(s, end, dict, reachable, path, result);
                path.pop_back();
            }
        }
    }
};

// ============================================================
// 解法4: Trie + 记忆化回溯
// 时间: O(2^n · n) 最坏  空间: O(Σ|w| + 2^n·n)
//
// 【思路】
// 用 Trie 替代 HashSet 存字典。优势：
// 1. 逐字符扫描时同步走 Trie，无需反复 substr
// 2. 当某个前缀不在 Trie 中时立即 break → 提前终止
// 3. 对于大字典(百万单词)场景，Trie 共享前缀，空间更优
//
// 和解法2的区别：只改了"如何判断一个子串是否是字典单词"
// 解法2: dict.count(s.substr(start, len)) → O(len) 创建子串 + O(len) 哈希
// 解法4: Trie 逐字符走，累计 O(len)，且能在中途 break
// ============================================================
class Solution4 {
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
        for (int i = start; i < (int)s.size(); i++) {
            int idx = s[i] - 'a';
            // Trie 中无此前缀 → 更长的子串也不可能在字典中 → break
            if (!node->children[idx]) break;
            node = node->children[idx];

            if (node->isEnd) {
                // s[start..i] 是一个完整的字典单词
                string word = s.substr(start, i - start + 1);
                vector<string> rest = dfs(s, i + 1);
                for (const string& tail : rest) {
                    if (tail.empty()) {
                        sentences.push_back(word);
                    } else {
                        sentences.push_back(word + " " + tail);
                    }
                }
            }
            // 即使 isEnd 为 true，也继续尝试更长的前缀
            // 因为可能有更长的单词 (如 "pine" 和 "pineapple" 都在字典中)
        }

        memo[start] = sentences;
        return sentences;
    }

public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        root = new TrieNode();
        for (const string& w : wordDict) insert(w);
        memo.clear();
        return dfs(s, 0);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 避免重复计算 | 前缀剪枝 | 可行性剪枝 | 推荐场景 |
// |------|------------|---------|-----------|---------|
// | 1.暴力回溯 | ✗ | ✗ | ✗ | 理解思路 |
// | 2.记忆化回溯 | ✓ memo | ✗ | ✗ | 面试首选 ⭐ |
// | 3.DP+回溯 | ✗ | ✗ | ✓ reachable | 大量不可拆 |
// | 4.Trie+记忆化 | ✓ memo | ✓ break | ✗ | 大字典场景 |
//
// 面试推荐: 先写解法2，被追问优化再说解法3/4的思路。
//
// 【易错点】
// 1. base case 返回 {} 而非 {""}:
//    ✗ if (start == n) return {};
//    ✓ if (start == n) return {""};
//    原因: 调用方的 for(tail : rest) 循环在 rest={} 时不执行，
//    导致最后一个合法单词无法被添加到结果中。
//
// 2. 拼接时末尾多空格:
//    ✗ sentences.push_back(word + " " + tail);  // tail="" 时多一个空格
//    ✓ sentences.push_back(tail.empty() ? word : word + " " + tail);
//
// 3. 记忆化没缓存空结果:
//    如果某位置无法拆分，memo[start] 应该存空 vector。
//    写法 memo[start] = sentences; 在 sentences 为空时也会缓存，这是正确的。
//    错误做法：只在 sentences 非空时才缓存 → 下次又重新计算。
//
// 4. Trie 解法中找到一个单词后就 break:
//    ✗ if (node->isEnd) { ... break; }   // "pine" 匹配后跳过 "pineapple"
//    ✓ if (node->isEnd) { ... }           // 继续走，尝试更长的匹配
//
// 【面试追问】
// Q1(基础): 这道题和139的区别？为什么不能只用DP？
//    → 139 只需 bool，本题要所有方案。方案数可能指数级，必须回溯穷举。
//
// Q2(优化): 记忆化具体缓存了什么？key 和 value 分别是什么？
//    → key = 起始位置 i，value = s[i..n-1] 的所有合法句子列表。
//      同一个位置不管从多少条路径到达，结果都一样。
//
// Q3(剪枝): 如果绝大部分拆分都是不可行的，怎么优化？
//    → 先用139的DP预处理 reachable[i]，回溯时跳过不可达位置。
//
// Q4(大字典): 字典有百万单词怎么优化？
//    → 用Trie。逐字符走 Trie，前缀不存在时 break。避免大量无效 substr。
//
// Q5(变体): 只要方案数不要具体方案？
//    → 纯DP: dp[i] = Σ dp[j] (对所有 j<i 且 s[j..i-1]∈dict)。O(n²)。
//
// ============================================================
// 【空间优化说明】
//
// 本题是回溯 + 记忆化搜索，不是传统 DP 填表问题，没有"滚动数组"
// 空间优化的概念。
//
// - 解法2（记忆化回溯）：memo[start] 存储"s[start..n-1] 的所有合法句子列表"。
//   每个位置的结果是一组字符串（最坏情况指数级），无法用常数个变量替代。
// - 解法3（DP + 回溯）：reachable[i] 是 O(n) 的辅助数组，已是一维最优；
//   回溯部分的空间由结果数量决定，无法压缩。
//
// 本质原因：题目要求列举所有方案，方案数本身可能是指数级的，
// 空间下界由输出大小决定，与 DP 优化无关。
// ============================================================
