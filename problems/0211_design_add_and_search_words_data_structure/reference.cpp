/*
 * 【题目本质】
 * 设计支持通配符 '.' 的字典数据结构。
 * 核心：Trie 树 + DFS 回溯处理通配符匹配。
 *
 * 【解法总览】
 * 解法1: Trie + DFS 回溯 ⭐ — add O(L), search O(26^d * L), d 为 '.' 的数量
 * 解法2: 哈希分组 + 暴力匹配 — add O(1), search O(N*L) 最坏
 */

// ===================== 解法1: Trie + DFS 回溯 (推荐) =====================
// 思路：
// 1. 构建标准 26 叉 Trie 树，addWord 沿路径创建节点
// 2. search 时逐字符匹配：
//    - 普通字符：走对应子节点
//    - '.' 通配符：遍历所有非空子节点递归匹配
// 3. 到达单词末尾时检查 isEnd 标志
class Solution1 {
    // Trie 节点定义
    struct TrieNode {
        TrieNode* children[26] = {};  // 26 个小写字母的子节点
        bool isEnd = false;           // 是否是某个完整单词的结尾
    };

    TrieNode* root;

    // DFS 搜索：从 node 开始匹配 word[idx:]
    bool dfs(const string& word, int idx, TrieNode* node) {
        // 基准情况：匹配完所有字符
        if (idx == word.size()) return node->isEnd;

        char c = word[idx];
        if (c == '.') {
            // 通配符：尝试所有可能的子节点
            for (int i = 0; i < 26; i++) {
                if (node->children[i] && dfs(word, idx + 1, node->children[i])) {
                    return true;  // 任意一条路径匹配成功即可
                }
            }
            return false;  // 所有路径都不匹配
        } else {
            // 普通字符：走对应子节点
            int ci = c - 'a';
            return node->children[ci] && dfs(word, idx + 1, node->children[ci]);
        }
    }

public:
    Solution1() : root(new TrieNode()) {}

    // 将 word 插入 Trie，时间 O(L)
    void addWord(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
        }
        curr->isEnd = true;
    }

    // 搜索 word（支持 '.' 通配符）
    bool search(string word) {
        return dfs(word, 0, root);
    }
};

// ===================== 解法2: 哈希分组 + 暴力匹配 =====================
// 思路：
// 1. 按单词长度分组存储到 unordered_map<int, vector<string>>
// 2. search 时只遍历同长度的单词，逐字符比较
// 3. '.' 匹配任意字符直接跳过
// 优点：实现简单；缺点：search 最坏 O(N*L)
class Solution2 {
    unordered_map<int, vector<string>> words;

public:
    Solution2() {}

    void addWord(string word) {
        words[word.size()].push_back(word);
    }

    bool search(string word) {
        int len = word.size();
        // 只搜索同长度的单词
        for (const string& w : words[len]) {
            bool match = true;
            for (int i = 0; i < len; i++) {
                // '.' 匹配任意字符，其他字符必须完全相同
                if (word[i] != '.' && word[i] != w[i]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }
};

/*
 * 【解法对比】
 * | 解法 | addWord | search | 空间 | 适用场景 |
 * |------|---------|--------|------|---------|
 * | Trie+DFS ⭐ | O(L) | O(26^d * L) | O(totalChars*26) | 通用，面试首选 |
 * | 哈希分组 | O(1) | O(N*L) | O(N*L) | 实现简单 |
 *
 * 【易错点】
 * 1. DFS 终止条件必须检查 isEnd，而不是直接返回 true
 * 2. '.' 通配符需要遍历所有 26 个子节点，不能只检查第一个
 * 3. 访问子节点前必须判断非空，避免空指针崩溃
 * 4. TrieNode 的 children 数组需要初始化为 nullptr（用 {} 或 memset）
 *
 * 【面试追问】
 * Q1: 如何支持删除操作？ → 增加 count 字段，删除时递减
 * Q2: 全是 '.' 的最坏情况？ → O(26^L)，可以限制长度或按长度建索引剪枝
 * Q3: 内存优化？ → 用 unordered_map 代替 children[26] 数组，节省稀疏节点的空间
 */
