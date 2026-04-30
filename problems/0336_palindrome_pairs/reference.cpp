/*
 * 【题目本质】
 * 找出所有 (i,j) 使得 words[i]+words[j] 是回文串。
 * 核心：对每个单词做前后缀分解，用哈希表快速匹配反转字符串。
 *
 * 【解法总览】
 * Solution1: 哈希表 + 前后缀分解 — O(n*k^2) 时间 ⭐推荐
 * Solution2: Trie + 前后缀分解 — O(n*k^2) 时间（Trie 版本）
 */

// ===== Solution1: 哈希表 + 前后缀分解 =====
// 思路：
// 1. 将所有单词的反转存入哈希表
// 2. 对每个单词枚举拆分点：prefix + suffix
//    - 若 prefix 是回文且 rev(suffix) 在字典中 → [dict[suffix], i]
//    - 若 suffix 是回文且 rev(prefix) 在字典中 → [i, dict[prefix]]
class Solution1 {
public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        // 存储每个单词反转后 → 原始下标
        unordered_map<string, int> dict;
        for (int i = 0; i < (int)words.size(); i++) {
            string rev = words[i];
            reverse(rev.begin(), rev.end());
            dict[rev] = i;
        }

        vector<vector<int>> result;
        for (int i = 0; i < (int)words.size(); i++) {
            const string& w = words[i];
            int n = w.size();

            for (int j = 0; j <= n; j++) {
                string prefix = w.substr(0, j);
                string suffix = w.substr(j);

                // Case 1: prefix 是回文，在字典中找 suffix
                // 拼接结果: rev(suffix) + prefix + suffix → 回文
                if (isPalin(prefix)) {
                    auto it = dict.find(suffix);
                    if (it != dict.end() && it->second != i)
                        result.push_back({it->second, i});
                }

                // Case 2: suffix 是回文，在字典中找 prefix
                // 拼接结果: prefix + suffix + rev(prefix) → 回文
                // j != n 避免与 Case 1 的 j=0 重复
                if (j != n && isPalin(suffix)) {
                    auto it = dict.find(prefix);
                    if (it != dict.end() && it->second != i)
                        result.push_back({i, it->second});
                }
            }
        }
        return result;
    }

private:
    bool isPalin(const string& s) {
        int l = 0, r = (int)s.size() - 1;
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }
};

// ===== Solution2: Trie + 前后缀分解 =====
// 思路：将每个单词反转后插入 Trie，搜索时沿 Trie 匹配
// 优点：避免大量 substr 和哈希操作
class Solution2 {
public:
    struct TrieNode {
        int wordIdx = -1;           // 此节点对应某个反转单词的结尾
        vector<int> palindromeSuffixIds; // 从此往后剩余部分是回文的单词下标
        TrieNode* ch[26] = {};
    };

    vector<vector<int>> palindromePairs(vector<string>& words) {
        TrieNode* root = new TrieNode();

        // 插入所有单词的反转
        for (int i = 0; i < (int)words.size(); i++) {
            string rev = words[i];
            reverse(rev.begin(), rev.end());
            TrieNode* cur = root;
            for (int j = 0; j < (int)rev.size(); j++) {
                if (isPalin(rev, j, rev.size() - 1))
                    cur->palindromeSuffixIds.push_back(i);
                int c = rev[j] - 'a';
                if (!cur->ch[c]) cur->ch[c] = new TrieNode();
                cur = cur->ch[c];
            }
            cur->wordIdx = i;
            cur->palindromeSuffixIds.push_back(i);
        }

        // 搜索
        vector<vector<int>> result;
        for (int i = 0; i < (int)words.size(); i++) {
            TrieNode* cur = root;
            for (int j = 0; j < (int)words[i].size(); j++) {
                // 单词比 Trie 路径长：Trie 到头了，剩余部分是回文
                if (cur->wordIdx >= 0 && cur->wordIdx != i &&
                    isPalin(words[i], j, words[i].size() - 1))
                    result.push_back({i, cur->wordIdx});
                int c = words[i][j] - 'a';
                if (!cur->ch[c]) { cur = nullptr; break; }
                cur = cur->ch[c];
            }
            if (!cur) continue;
            // 单词匹配完，Trie 中剩余部分是回文的
            for (int idx : cur->palindromeSuffixIds) {
                if (idx != i) result.push_back({i, idx});
            }
        }
        return result;
    }

private:
    bool isPalin(const string& s, int l, int r) {
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }
};

/*
 * 【解法对比】
 * Solution1: 哈希表法 — 实现简洁，面试首选
 *   - 优点：代码短，逻辑清晰
 *   - 缺点：大量 substr 产生临时字符串
 * Solution2: Trie 法 — 更优雅的数据结构方案
 *   - 优点：避免字符串拷贝，利于扩展
 *   - 缺点：代码量大，面试时间紧张
 *
 * 【易错点】
 * 1. 哈希表存的是反转字符串，不是原始字符串
 * 2. Case 2 需要 j != n 避免与 Case 1 的 j=0 重复
 * 3. 必须检查 it->second != i，防止单词与自己配对
 * 4. 空字符串处理：空串本身是回文，j=0 和 j=n 自然覆盖
 *
 * 【面试追问】
 * Q1: 为什么要存反转而不是原串？
 *     → words[i]+words[j] 是回文 ↔ words[j] 的某部分是 words[i] 某部分的反转
 * Q2: j != n 条件的意义？
 *     → j=n 时 suffix=""（回文），prefix=整词；与 j=0 时 prefix=""，suffix=整词 重复
 * Q3: 如何处理空字符串？
 *     → 空串 reverse 还是空串，j=0 时 prefix="" isPalin=true，suffix=整词查字典即可
 */
