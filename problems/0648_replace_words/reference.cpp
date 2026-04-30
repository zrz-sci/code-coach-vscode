// LeetCode 648: Replace Words
// 解法一：Trie（前缀树）
// 时间复杂度: O(D + S)  空间复杂度: O(D)
// 其中 D = 词根总字符数, S = sentence 总字符数

// ============================================================
// 核心思想：
// 将所有词根插入 Trie，然后对句子中的每个单词在 Trie 中
// 查找最短匹配前缀。Trie 的结构天然保证最短前缀优先。
// ============================================================

class Solution {
public:
    // Trie 节点定义
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;

        TrieNode() : isEnd(false) {
            memset(children, 0, sizeof(children));
        }
    };

    string replaceWords(vector<string>& dictionary, string sentence) {
        // 第一步：构建 Trie
        TrieNode* root = new TrieNode();
        for (const string& word : dictionary) {
            insertTrie(root, word);
        }

        // 第二步：分割 sentence 为单词，逐词查找替换
        string result;
        istringstream iss(sentence);
        string word;
        bool first = true;

        while (iss >> word) {
            if (!first) result += ' ';
            first = false;

            // 在 Trie 中查找最短词根前缀
            string replaced = searchShortestRoot(root, word);
            result += replaced;
        }

        return result;
    }

private:
    void insertTrie(TrieNode* root, const string& word) {
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

    string searchShortestRoot(TrieNode* root, const string& word) {
        TrieNode* node = root;
        for (int i = 0; i < (int)word.size(); ++i) {
            int idx = word[i] - 'a';

            // 如果当前节点是某个词根的末尾，返回最短前缀
            if (node->isEnd) {
                return word.substr(0, i);
            }

            // 如果字符不存在于 Trie 中，无匹配词根
            if (!node->children[idx]) {
                return word;
            }

            node = node->children[idx];
        }

        // 整个单词遍历完：检查最后节点是否为词根末尾
        if (node->isEnd) return word;
        return word;  // 无匹配词根，返回原词
    }
};

// ============================================================
// 解法二：HashSet 暴力前缀匹配
// 时间复杂度: O(S * W)  空间复杂度: O(D)
// 其中 W = 单个单词最大长度
// ============================================================

class Solution_HashSet {
public:
    string replaceWords(vector<string>& dictionary, string sentence) {
        // 将所有词根放入 HashSet
        unordered_set<string> rootSet(dictionary.begin(), dictionary.end());

        string result;
        istringstream iss(sentence);
        string word;
        bool first = true;

        while (iss >> word) {
            if (!first) result += ' ';
            first = false;

            // 从长度 1 开始尝试前缀
            string replacement = word;
            for (int len = 1; len <= (int)word.size(); ++len) {
                string prefix = word.substr(0, len);
                if (rootSet.count(prefix)) {
                    replacement = prefix;
                    break;  // 找到最短的就停
                }
            }

            result += replacement;
        }

        return result;
    }
};

// ============================================================
// 解法三：排序 + 逐词匹配（不用 Trie 的替代方案）
// 时间复杂度: O(D log D + S * W)  空间复杂度: O(D)
// ============================================================

class Solution_Sort {
public:
    string replaceWords(vector<string>& dictionary, string sentence) {
        // 按长度排序词根，短的在前
        sort(dictionary.begin(), dictionary.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
        });

        string result;
        istringstream iss(sentence);
        string word;
        bool first = true;

        while (iss >> word) {
            if (!first) result += ' ';
            first = false;

            string replacement = word;
            for (const string& root : dictionary) {
                // 如果词根长度已经超过当前单词，后面的更长，直接停
                if (root.size() > word.size()) break;

                // 检查是否是前缀
                if (word.substr(0, root.size()) == root) {
                    replacement = root;
                    break;  // 已经按长度排序，第一个匹配的就是最短的
                }
            }

            result += replacement;
        }

        return result;
    }
};

// ============================================================
// 手动模拟（Trie 解法）：
// dictionary = ["cat","bat","rat"]
// sentence = "the cattle was rattled by the battery"
//
// Trie 构建：
//         root
//        / | \
//       c  b  r
//       |  |  |
//       a  a  a
//       |  |  |
//      t*  t*  t*
//
// 逐词查找：
// "the"    -> t 不在 Trie root 的 children 中 -> 保留 "the"
// "cattle" -> c->a->t(isEnd=true) -> 替换为 "cat"
// "was"    -> w 不在 Trie -> 保留 "was"
// "rattled"-> r->a->t(isEnd=true) -> 替换为 "rat"
// "by"     -> b 存在，但 b->children['y'-'a'] = null -> 保留 "by"
// "the"    -> 同上保留
// "battery"-> b->a->t(isEnd=true) -> 替换为 "bat"
//
// 结果: "the cat was rat by the bat"
// ============================================================
