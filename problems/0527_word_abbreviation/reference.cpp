/*
 * 【题目本质】
 * 为每个单词生成最短的唯一缩写。缩写格式：前缀 + 中间字符数 + 尾字母。
 * 有冲突时逐步增加前缀长度，直到所有缩写唯一。
 *
 * 【解法总览】
 * Solution1: 贪心迭代解冲突    O(n * L) 摊销 / O(n)      ⭐推荐
 * Solution2: 分组 + Trie      O(n * L) / O(n * L)
 */

// ===================== Solution1: 贪心迭代解冲突 =====================
// 思路：初始最短缩写，发现冲突则增加前缀长度，重复直到唯一
class Solution1 {
public:
    string makeAbbr(const string& word, int prefixLen) {
        int n = word.size();
        int abbrLen = n - prefixLen - 1; // 中间被缩写的字符数
        if (abbrLen <= 1) return word;    // 缩写不比原词短，保留原词
        return word.substr(0, prefixLen) + to_string(abbrLen) + word.back();
    }

    vector<string> wordsAbbreviation(vector<string>& words) {
        int n = words.size();
        vector<string> result(n);
        vector<int> prefix(n, 1); // 每个词的前缀长度

        // 初始化缩写
        for (int i = 0; i < n; i++) {
            result[i] = makeAbbr(words[i], 1);
        }

        // 迭代解冲突
        for (int i = 0; i < n; i++) {
            while (true) {
                vector<int> conflicts;
                for (int j = i + 1; j < n; j++) {
                    if (result[i] == result[j]) {
                        conflicts.push_back(j);
                    }
                }
                if (conflicts.empty()) break;

                conflicts.push_back(i); // 自己也要增加前缀
                for (int idx : conflicts) {
                    prefix[idx]++;
                    result[idx] = makeAbbr(words[idx], prefix[idx]);
                }
            }
        }
        return result;
    }
};

// ===================== Solution2: 分组 + Trie =====================
// 思路：按(长度, 首字母, 尾字母)分组，同组建 Trie 找最短唯一前缀
class Solution2 {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        int count = 0;
    };

    string makeAbbr(const string& word, int prefixLen) {
        int n = word.size();
        int abbrLen = n - prefixLen - 1;
        if (abbrLen <= 1) return word;
        return word.substr(0, prefixLen) + to_string(abbrLen) + word.back();
    }

    vector<string> wordsAbbreviation(vector<string>& words) {
        int n = words.size();
        vector<string> result(n);

        // 按 (长度, 首字母, 尾字母) 分组——只有这三者相同才可能冲突
        unordered_map<string, vector<int>> groups;
        for (int i = 0; i < n; i++) {
            string key = to_string(words[i].size()) + "_" + words[i].front() + words[i].back();
            groups[key].push_back(i);
        }

        for (auto& [key, indices] : groups) {
            // 建 Trie
            TrieNode* root = new TrieNode();
            for (int idx : indices) {
                TrieNode* node = root;
                for (char c : words[idx]) {
                    if (!node->children[c - 'a'])
                        node->children[c - 'a'] = new TrieNode();
                    node = node->children[c - 'a'];
                    node->count++;
                }
            }

            // 找最短唯一前缀
            for (int idx : indices) {
                TrieNode* node = root;
                int prefixLen = 0;
                for (char c : words[idx]) {
                    node = node->children[c - 'a'];
                    prefixLen++;
                    if (node->count == 1) break;
                }
                result[idx] = makeAbbr(words[idx], prefixLen);
            }

            // 清理 Trie 内存（面试中可省略）
            // deleteTree(root);
        }
        return result;
    }
};

/*
 * 【解法对比】
 * Solution1: 简洁直观，迭代解冲突，无需额外数据结构
 * Solution2: Trie 一次确定最短前缀，对大量冲突更高效
 *
 * 【易错点】
 * 1. makeAbbr 中忘记判断缩写不比原词短的情况（abbrLen <= 1 时返回原词）
 * 2. 分组 key 遗漏尾字母——不同尾字母的词不会冲突
 * 3. Trie 中 count 忘记在插入时递增
 * 4. 冲突解决时忘记把自身 (i) 也加入冲突列表
 *
 * 【面试追问】
 * Q1: 为什么按 (长度, 首字母, 尾字母) 分组就够了？
 * Q2: 迭代法最坏需要几轮？
 * Q3: Trie 方法的空间如何优化？
 */
