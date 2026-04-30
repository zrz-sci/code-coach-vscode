// 【题目本质】
// 从词库中选取 L 个等长单词（可重复），使得第 k 行和第 k 列读出的字符串相同。
// 核心：回溯逐行选词，利用已选行的列字符推导下一行的前缀约束，用 Trie/HashMap 快速查找候选词。

// 【解法总览】
// Solution1: Trie + 回溯 O(N*26^L) / O(N*L) ⭐推荐 - Trie 每个节点存经过的词索引
// Solution2: HashMap + 回溯 O(N*26^L) / O(N*L^2) - 所有前缀子串映射到词索引列表

// ===================== Solution1: Trie + 回溯 =====================
// 关键推导：选定前 row 行后，第 row 行的前缀 = square[0][row] + square[1][row] + ... + square[row-1][row]
class Solution1 {
    struct TrieNode {
        vector<int> wordIds; // 经过此节点的所有词索引
        TrieNode* ch[26] = {};
    };

    TrieNode* root;

    void insert(const string& word, int id) {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->ch[idx]) cur->ch[idx] = new TrieNode();
            cur = cur->ch[idx];
            cur->wordIds.push_back(id);
        }
    }

    const vector<int>& search(const string& prefix) {
        static const vector<int> empty;
        TrieNode* cur = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!cur->ch[idx]) return empty;
            cur = cur->ch[idx];
        }
        return cur->wordIds;
    }

public:
    vector<vector<string>> wordSquares(vector<string>& words) {
        root = new TrieNode();
        int L = words[0].size();
        for (int i = 0; i < (int)words.size(); i++)
            insert(words[i], i);

        vector<vector<string>> result;
        vector<string> square;

        function<void(int)> backtrack = [&](int row) {
            if (row == L) {
                result.push_back(square);
                return;
            }
            // 构造第 row 行的前缀约束
            string prefix;
            for (int i = 0; i < row; i++)
                prefix += square[i][row];

            // 查找所有匹配前缀的候选词
            for (int id : search(prefix)) {
                square.push_back(words[id]);
                backtrack(row + 1);
                square.pop_back();
            }
        };

        backtrack(0);
        return result;
    }
};

// ===================== Solution2: HashMap + 回溯 =====================
// 用 HashMap 存储所有前缀到词索引的映射，省去 Trie 实现
class Solution2 {
public:
    vector<vector<string>> wordSquares(vector<string>& words) {
        unordered_map<string, vector<int>> prefixMap;
        int L = words[0].size();

        // 预处理：每个词的所有前缀都映射到该词索引
        for (int i = 0; i < (int)words.size(); i++) {
            for (int len = 0; len <= L; len++) {
                prefixMap[words[i].substr(0, len)].push_back(i);
            }
        }

        vector<vector<string>> result;
        vector<string> square;

        function<void(int)> backtrack = [&](int row) {
            if (row == L) {
                result.push_back(square);
                return;
            }
            string prefix;
            for (int i = 0; i < row; i++)
                prefix += square[i][row];

            auto it = prefixMap.find(prefix);
            if (it == prefixMap.end()) return;

            for (int id : it->second) {
                square.push_back(words[id]);
                backtrack(row + 1);
                square.pop_back();
            }
        };

        backtrack(0);
        return result;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | Solution1 | O(N*26^L) | O(N*L) | Trie 空间更优，前缀查找自然 |
// | Solution2 | O(N*26^L) | O(N*L^2) | 存储所有子串，实现简单 |

// 【易错点】
// 1. 前缀推导：square[i][row] 是已选第 i 行在第 row 列的字符，不是 square[row][i]
// 2. Trie 中每个节点必须存经过的所有词索引（不只是叶节点），否则前缀查找不完整
// 3. 空前缀情况：第 0 行没有约束，应该返回所有单词
// 4. 同一个单词可以重复使用（题目允许）

// 【面试追问】
// Q1: 回溯的搜索空间有多大？
// A1: 最坏 O(N^L)（每行 N 个选择，L 行），但前缀剪枝大幅缩减。
//     由于 L<=4 且前缀约束强，实际搜索空间很小。
//
// Q2: 如何进一步优化？
// A2: 可以在 Trie 节点中记录子树是否为空，提前终止。
//     也可以对词按前缀分组后排序，用二分查找替代 Trie。
//
// Q3: 这题和 N-Queens 问题的相似点？
// A3: 都是逐行填充 + 回溯，约束来自已填行。N-Queens 的约束是列/对角线，
//     Word Squares 的约束是列字符必须构成有效前缀。
