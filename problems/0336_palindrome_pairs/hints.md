# 336. Palindrome Pairs - 回文对

## 核心思路

给定一组唯一字符串，找出所有拼接后为回文的 (i,j) 对。关键洞察：对每个单词，将其拆分为前缀和后缀，如果前缀是回文且后缀的反转存在于字典中（或反之），则构成回文对。用哈希表存储所有单词的反转，实现高效查找。

## 思维链

1. **暴力枚举**：O(n^2 * k) 检查所有 (i,j) 对 → 太慢
2. **反转查找**：如果 word[i] 的反转 == word[j]，则 word[i]+word[j] 是回文 → 只覆盖等长情况
3. **前后缀分解**：word 拆为 prefix + suffix，如果 suffix 是回文且 reverse(prefix) 在字典中，则拼接后是回文
4. **对称处理**：同时考虑 word 放前面和放后面两种情况
5. **空字符串**：空串与任何本身是回文的字符串构成回文对

## 解法概览

| 解法 | 时间 | 空间 | 特点 | 推荐 |
|------|------|------|------|------|
| 哈希表 + 前后缀分解 | O(n * k^2) | O(n * k) | 实现简洁 | ⭐⭐⭐ |
| Trie + 前后缀分解 | O(n * k^2) | O(n * k) | 更优雅但代码长 | ⭐⭐ |

## 关键提示

1. 用 `unordered_map<string, int>` 存储每个单词反转后对应的下标
2. 对每个单词 word[i]，枚举所有拆分点 j：prefix = word[0..j-1], suffix = word[j..end]
3. 两种情况：
   - 如果 prefix 是回文，且 reverse(suffix) 在字典中 → [dict[rev_suffix], i]
   - 如果 suffix 是回文，且 reverse(prefix) 在字典中 → [i, dict[rev_prefix]]
4. 注意去重：j=0 和 j=len 时会覆盖完整单词匹配，需要避免 i==j 的情况

## 解法详解

### 解法一：哈希表 + 前后缀分解（推荐）

```cpp
class Solution {
public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        unordered_map<string, int> dict;
        for (int i = 0; i < words.size(); i++) {
            string rev = words[i];
            reverse(rev.begin(), rev.end());
            dict[rev] = i;
        }

        vector<vector<int>> result;
        for (int i = 0; i < words.size(); i++) {
            string& w = words[i];
            int n = w.size();

            for (int j = 0; j <= n; j++) {
                string prefix = w.substr(0, j);
                string suffix = w.substr(j);

                // Case 1: prefix 是回文，rev(suffix) 存在 → [rev(suffix), i]
                if (isPalindrome(prefix)) {
                    auto it = dict.find(suffix);
                    if (it != dict.end() && it->second != i) {
                        result.push_back({it->second, i});
                    }
                }

                // Case 2: suffix 是回文，rev(prefix) 存在 → [i, rev(prefix)]
                // j != n 避免重复计算（j=n 时 suffix 为空，与 j=0 的 prefix 为空重复）
                if (j != n && isPalindrome(suffix)) {
                    auto it = dict.find(prefix);
                    if (it != dict.end() && it->second != i) {
                        result.push_back({i, it->second});
                    }
                }
            }
        }
        return result;
    }

private:
    bool isPalindrome(const string& s) {
        int l = 0, r = s.size() - 1;
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }
};
```

**复杂度**：时间 O(n * k^2)（n 个单词，每个长度 k，枚举拆分点 + 回文判断），空间 O(n * k)

### 解法二：Trie

```cpp
class Solution {
public:
    struct TrieNode {
        int idx = -1;  // 到此为止是某个反转单词的末尾
        vector<int> palins;  // 从此往后是回文的单词下标
        TrieNode* children[26] = {};
    };

    vector<vector<int>> palindromePairs(vector<string>& words) {
        TrieNode* root = new TrieNode();

        // 将每个单词的反转插入 Trie
        for (int i = 0; i < words.size(); i++) {
            string rev = words[i];
            reverse(rev.begin(), rev.end());
            insertTrie(root, rev, i, words[i]);
        }

        vector<vector<int>> result;
        for (int i = 0; i < words.size(); i++) {
            searchTrie(root, words[i], i, result);
        }
        return result;
    }

private:
    void insertTrie(TrieNode* root, const string& rev, int idx,
                    const string& orig) {
        TrieNode* cur = root;
        for (int i = 0; i < rev.size(); i++) {
            // 剩余部分是回文，记录下标
            if (isPalindrome(rev, i, rev.size() - 1))
                cur->palins.push_back(idx);
            int c = rev[i] - 'a';
            if (!cur->children[c])
                cur->children[c] = new TrieNode();
            cur = cur->children[c];
        }
        cur->idx = idx;
        cur->palins.push_back(idx);
    }

    void searchTrie(TrieNode* root, const string& word, int idx,
                    vector<vector<int>>& result) {
        TrieNode* cur = root;
        for (int i = 0; i < word.size(); i++) {
            if (cur->idx >= 0 && cur->idx != idx &&
                isPalindrome(word, i, word.size() - 1))
                result.push_back({idx, cur->idx});
            int c = word[i] - 'a';
            if (!cur->children[c]) return;
            cur = cur->children[c];
        }
        for (int j : cur->palins) {
            if (j != idx) result.push_back({idx, j});
        }
    }

    bool isPalindrome(const string& s, int l, int r) {
        while (l < r) {
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }
};
```

## 易错点

- ✗ 忘记处理空字符串 → ✓ 空串与任何回文串构成回文对，前后缀分解 j=0 和 j=n 自然覆盖
- ✗ 结果中包含 (i,i) → ✓ 检查 `it->second != i`
- ✗ Case 2 中 j==n 导致重复 → ✓ 添加 `j != n` 条件
- ✗ 用 `dict[word]` 而不是 `dict[reverse(word)]` → ✓ 哈希表存的是反转后的字符串

## 面试追问

**Q1: 为什么 Case 2 需要 j != n 的条件？**
→ 当 j=n 时，suffix="" 是回文，prefix=整个单词，与 Case 1 中 j=0 时 prefix="" 是回文、suffix=整个单词的情况会产生重复配对。

**Q2: 时间复杂度能否优化到 O(n*k)？**
→ 理论上 Trie + Manacher 预处理回文判断可以达到 O(n*k)，但实现复杂度较高，面试中 O(n*k^2) 已足够。

**Q3: 如果 words 中有重复怎么办？**
→ 题目保证 unique，若允许重复需要改用 multimap 或额外处理。

## 相关题型

- [5. Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/) - 回文基础
- [214. Shortest Palindrome](https://leetcode.com/problems/shortest-palindrome/) - KMP + 回文
- [131. Palindrome Partitioning](https://leetcode.com/problems/palindrome-partitioning/) - 回文分割
