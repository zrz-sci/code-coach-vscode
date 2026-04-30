// 745. Prefix and Suffix Search
// 难度: Hard
// 标签: Design, Trie, Array, Hash Table, String

// ============================================================
// 解法一：HashMap 枚举所有前缀后缀组合 - 推荐
// 构造: O(n * L^2)  查询: O(L)  空间: O(n * L^2)
// ============================================================
// 利用 words[i].length <= 7 的约束，每个单词最多 49 个 (prefix, suffix) 组合
// 用 pref + "#" + suff 作为 HashMap key，值为最大下标

class WordFilter {
public:
    unordered_map<string, int> lookup;

    WordFilter(vector<string>& words) {
        int n = words.size();
        for (int k = 0; k < n; ++k) {
            const string& w = words[k];
            int L = w.size();
            // 枚举所有前缀（包括空前缀为了安全，虽然题目保证 pref >= 1）
            for (int i = 0; i <= L; ++i) {
                string pref = w.substr(0, i);
                for (int j = 0; j <= L; ++j) {
                    string suff = w.substr(L - j);
                    // key = pref + "#" + suff
                    lookup[pref + "#" + suff] = k;
                }
            }
        }
    }

    int f(string pref, string suff) {
        string key = pref + "#" + suff;
        auto it = lookup.find(key);
        return it != lookup.end() ? it->second : -1;
    }
};

// ============================================================
// 解法二：后缀包装 Trie
// 构造: O(n * L^2)  查询: O(L)  空间: O(n * L^2)
// ============================================================
// 对每个 word，将 word[j:] + '#' + word 的所有变体插入 Trie
// 查询 suff + '#' + pref
// 每个 Trie 节点维护经过它的最大 word index

class WordFilter {
public:
    // Trie 节点
    struct TrieNode {
        int children[27];  // 26 letters + '#' (index 26)
        int maxIdx;        // 经过此节点的最大 word index
        TrieNode() : maxIdx(-1) {
            memset(children, -1, sizeof(children));
        }
    };

    vector<TrieNode> nodes;

    int charIdx(char c) {
        return c == '#' ? 26 : c - 'a';
    }

    void insert(const string& s, int wordIdx) {
        int cur = 0;
        nodes[cur].maxIdx = max(nodes[cur].maxIdx, wordIdx);
        for (char c : s) {
            int ci = charIdx(c);
            if (nodes[cur].children[ci] == -1) {
                nodes[cur].children[ci] = nodes.size();
                nodes.emplace_back();
            }
            cur = nodes[cur].children[ci];
            nodes[cur].maxIdx = max(nodes[cur].maxIdx, wordIdx);
        }
    }

    int search(const string& s) {
        int cur = 0;
        for (char c : s) {
            int ci = charIdx(c);
            if (nodes[cur].children[ci] == -1) return -1;
            cur = nodes[cur].children[ci];
        }
        return nodes[cur].maxIdx;
    }

    WordFilter(vector<string>& words) {
        // 预分配节点空间：每个单词最多 L+1 个后缀变体，每个变体长度 2L+1
        // 最坏节点数约 n * L * (2L+1)，但共享前缀可大幅减少
        nodes.reserve(200000);
        nodes.emplace_back();  // root

        int n = words.size();
        for (int k = 0; k < n; ++k) {
            const string& w = words[k];
            int L = w.size();
            // 对每个后缀起始位置 j，插入 w[j:] + '#' + w
            for (int j = 0; j <= L; ++j) {
                string wrapped = w.substr(j) + '#' + w;
                insert(wrapped, k);
            }
        }
    }

    int f(string pref, string suff) {
        string query = suff + '#' + pref;
        return search(query);
    }
};

// ============================================================
// 解法三：双 Trie + 集合交集（了解即可）
// 构造: O(n * L)  查询: O(n) 最坏  空间: O(n * L)
// ============================================================
// 建两棵 Trie：
// - prefTrie: 正向插入每个 word，每个节点维护经过的 word indices 集合
// - suffTrie: 反向插入每个 word，每个节点维护经过的 word indices 集合
// 查询时分别搜索得到两个集合，取交集找最大值

class WordFilter {
public:
    struct TrieNode {
        int children[26];
        vector<int> indices;  // 经过此节点的 word indices
        TrieNode() { memset(children, -1, sizeof(children)); }
    };

    vector<TrieNode> prefNodes, suffNodes;

    void insertPref(const string& w, int idx) {
        int cur = 0;
        prefNodes[cur].indices.push_back(idx);
        for (char c : w) {
            int ci = c - 'a';
            if (prefNodes[cur].children[ci] == -1) {
                prefNodes[cur].children[ci] = prefNodes.size();
                prefNodes.emplace_back();
            }
            cur = prefNodes[cur].children[ci];
            prefNodes[cur].indices.push_back(idx);
        }
    }

    void insertSuff(const string& w, int idx) {
        int cur = 0;
        suffNodes[cur].indices.push_back(idx);
        for (int i = w.size() - 1; i >= 0; --i) {
            int ci = w[i] - 'a';
            if (suffNodes[cur].children[ci] == -1) {
                suffNodes[cur].children[ci] = suffNodes.size();
                suffNodes.emplace_back();
            }
            cur = suffNodes[cur].children[ci];
            suffNodes[cur].indices.push_back(idx);
        }
    }

    WordFilter(vector<string>& words) {
        prefNodes.reserve(100000);
        suffNodes.reserve(100000);
        prefNodes.emplace_back();
        suffNodes.emplace_back();

        for (int i = 0; i < (int)words.size(); ++i) {
            insertPref(words[i], i);
            insertSuff(words[i], i);
        }
    }

    int f(string pref, string suff) {
        // 在 prefTrie 中搜索 pref
        int cur = 0;
        for (char c : pref) {
            int ci = c - 'a';
            if (prefNodes[cur].children[ci] == -1) return -1;
            cur = prefNodes[cur].children[ci];
        }
        const vector<int>& prefSet = prefNodes[cur].indices;

        // 在 suffTrie 中搜索 suff（反向）
        cur = 0;
        for (int i = suff.size() - 1; i >= 0; --i) {
            int ci = suff[i] - 'a';
            if (suffNodes[cur].children[ci] == -1) return -1;
            cur = suffNodes[cur].children[ci];
        }
        const vector<int>& suffSet = suffNodes[cur].indices;

        // 从后往前找交集中的最大值
        int i = prefSet.size() - 1, j = suffSet.size() - 1;
        while (i >= 0 && j >= 0) {
            if (prefSet[i] == suffSet[j]) return prefSet[i];
            else if (prefSet[i] > suffSet[j]) --i;
            else --j;
        }
        return -1;
    }
};

// ============================================================
// 测试用例分析
// ============================================================
// 输入: words = ["apple"], f("a", "e")
// HashMap: 构造时存入 "a#e" -> 0 等49种组合
// 查询 "a#e" => 返回 0  正确
//
// 输入: words = ["apple", "ape"], f("a", "e")
// HashMap: "a#e" 先被设为0, 后被 "ape" 的 "a#e" 覆盖为1
// 但 "apple" 也有 "a#e"，如果 apple 在前，ape 在后，返回 1
//
// Trie 方案:
// "apple" -> 插入 "apple#apple", "pple#apple", "ple#apple", "le#apple", "e#apple"
// 查询 "e#a" -> 沿 Trie 走 e -> # -> a, 返回该节点的 maxIdx
