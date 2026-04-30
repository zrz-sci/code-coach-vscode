// LeetCode 676: Implement Magic Dictionary - Reference Solution
// 难度: Medium
// 标签: Design, Trie, Hash Table, String

// ============================================================
// 解法一: 按长度分组 + 暴力逐字符比较（推荐面试解法）
// ============================================================
// 核心思路:
//   - buildDict: 将单词按长度存入 HashMap
//   - search: 遍历同长度单词，逐字符比较差异数
//   - 恰好 1 个字符不同则返回 true
//
// 时间复杂度: buildDict O(N*L), search O(N*L)
//   N = 字典单词数(<=100), L = 单词长度(<=100)
// 空间复杂度: O(N*L)

class MagicDictionary {
public:
    unordered_map<int, vector<string>> lenMap;

    MagicDictionary() {}

    void buildDict(vector<string> dictionary) {
        for (auto& word : dictionary) {
            lenMap[word.size()].push_back(word);
        }
    }

    bool search(string searchWord) {
        int len = searchWord.size();
        if (lenMap.find(len) == lenMap.end()) return false;

        for (auto& word : lenMap[len]) {
            int diff = 0;
            for (int i = 0; i < len; i++) {
                if (word[i] != searchWord[i]) {
                    diff++;
                    if (diff > 1) break;  // 提前剪枝
                }
            }
            if (diff == 1) return true;
        }
        return false;
    }
};

// ============================================================
// 解法二: Trie + DFS（进阶方案，适合大词典）
// ============================================================
// 核心思路:
//   - 建 Trie 树存储所有字典单词
//   - search 时在 Trie 上 DFS，允许恰好一次"走错路"
//   - 到达终点时检查: 是否用了恰好一次不同 + 该节点是否是某单词末尾
//
// 时间复杂度: buildDict O(N*L), search O(26*L) 最坏
// 空间复杂度: O(N*L*26)

class MagicDictionary2 {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };

    TrieNode* root;

public:
    MagicDictionary2() {
        root = new TrieNode();
    }

    void buildDict(vector<string> dictionary) {
        for (auto& word : dictionary) {
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
    }

    bool search(string searchWord) {
        return dfs(root, searchWord, 0, false);
    }

private:
    // changed: 是否已经修改了一个字符
    bool dfs(TrieNode* node, const string& word, int idx, bool changed) {
        if (!node) return false;

        // 到达末尾：要求恰好改了一次 + 是单词结尾
        if (idx == (int)word.size()) {
            return changed && node->isEnd;
        }

        int target = word[idx] - 'a';

        if (changed) {
            // 已经用过一次修改机会，后续必须完全匹配
            return dfs(node->children[target], word, idx + 1, true);
        }

        // 还没用过修改机会，尝试所有26个分支
        for (int c = 0; c < 26; c++) {
            if (!node->children[c]) continue;

            if (c == target) {
                // 不修改，继续匹配
                if (dfs(node->children[c], word, idx + 1, false))
                    return true;
            } else {
                // 用掉修改机会，走一个不同的分支
                if (dfs(node->children[c], word, idx + 1, true))
                    return true;
            }
        }

        return false;
    }
};

// ============================================================
// 解法三: 通配符哈希（预处理优化查询）
// ============================================================
// 核心思路:
//   - buildDict: 对每个单词的每个位置替换为 '*'，存入 map
//     同时记录每个模式对应的原始单词集合
//   - search: 逐位替换为 '*'，检查模式是否存在
//     若存在，还需确认对应的原词中至少有一个 != searchWord
//
// 时间复杂度: buildDict O(N*L^2), search O(L^2)
// 空间复杂度: O(N*L^2)

class MagicDictionary3 {
    // 模式 -> 产生该模式的原始单词集合
    unordered_map<string, vector<string>> patternMap;

public:
    MagicDictionary3() {}

    void buildDict(vector<string> dictionary) {
        for (auto& word : dictionary) {
            for (int i = 0; i < (int)word.size(); i++) {
                string pattern = word;
                pattern[i] = '*';
                patternMap[pattern].push_back(word);
            }
        }
    }

    bool search(string searchWord) {
        for (int i = 0; i < (int)searchWord.size(); i++) {
            string pattern = searchWord;
            pattern[i] = '*';

            auto it = patternMap.find(pattern);
            if (it != patternMap.end()) {
                // 检查是否存在不同于 searchWord 的原词
                for (auto& word : it->second) {
                    if (word != searchWord) return true;
                }
            }
        }
        return false;
    }
};
