// 792. Number of Matching Subsequences
// 难度: Medium
// 标签: Trie, Array, Hash Table, String, Binary Search, Dynamic Programming, Sorting

// ============================================================
// 解法一：字符桶并行推进 (推荐面试首选)
// 时间 O(S + sum(len_i)), 空间 O(W)
// ============================================================
// 核心: 将所有单词按"当前需要匹配的字符"分到 26 个桶中,
// 遍历 s 一次, 同时推进所有单词的匹配进度。
class Solution {
public:
    int numMatchingSubseq(string s, vector<string>& words) {
        // buckets[c] 存储所有"当前需要字符 c"的单词
        // pair<int,int>: (words 中的下标, 当前匹配到的字符位置)
        vector<vector<pair<int,int>>> buckets(26);

        // 初始化: 每个 word 按首字符入桶
        for (int i = 0; i < (int)words.size(); ++i) {
            int c = words[i][0] - 'a';
            buckets[c].emplace_back(i, 0);
        }

        int count = 0;

        // 遍历 s 的每个字符
        for (char ch : s) {
            int c = ch - 'a';
            // 取出当前桶中所有元素 (必须先取出, 因为可能往同一桶加新元素)
            auto waiting = move(buckets[c]);
            // buckets[c] 被 move 后变空

            for (auto& [wi, ci] : waiting) {
                ci++;  // 当前字符已匹配, 推进指针
                if (ci == (int)words[wi].size()) {
                    // 整个单词匹配完成
                    count++;
                } else {
                    // 按下一个需要的字符重新入桶
                    int nextChar = words[wi][ci] - 'a';
                    buckets[nextChar].emplace_back(wi, ci);
                }
            }
        }

        return count;
    }
};

// ============================================================
// 解法二：二分查找 (预处理字符位置)
// 时间 O(S + sum(len_i * logS)), 空间 O(S)
// ============================================================
class Solution2 {
public:
    int numMatchingSubseq(string s, vector<string>& words) {
        // 预处理: 记录 s 中每个字符出现的所有位置
        vector<vector<int>> charPos(26);
        for (int i = 0; i < (int)s.size(); ++i) {
            charPos[s[i] - 'a'].push_back(i);
        }

        int count = 0;

        for (const string& word : words) {
            if (isSubsequence(word, charPos)) {
                count++;
            }
        }

        return count;
    }

private:
    bool isSubsequence(const string& word, const vector<vector<int>>& charPos) {
        int cur = -1;  // 当前在 s 中匹配到的位置

        for (char c : word) {
            const auto& positions = charPos[c - 'a'];
            // 找第一个 > cur 的位置
            auto it = upper_bound(positions.begin(), positions.end(), cur);
            if (it == positions.end()) {
                return false;  // s 中没有足够的字符 c
            }
            cur = *it;  // 更新匹配位置
        }

        return true;
    }
};

// ============================================================
// 解法三：暴力双指针 (适合小规模)
// 时间 O(W * S), 空间 O(1)
// ============================================================
class Solution3 {
public:
    int numMatchingSubseq(string s, vector<string>& words) {
        int count = 0;

        for (const string& word : words) {
            if (isSubseq(word, s)) {
                count++;
            }
        }

        return count;
    }

private:
    bool isSubseq(const string& word, const string& s) {
        int wi = 0;
        for (int si = 0; si < (int)s.size() && wi < (int)word.size(); ++si) {
            if (s[si] == word[wi]) {
                wi++;
            }
        }
        return wi == (int)word.size();
    }
};

// ============================================================
// 解法四：桶方法 + 去重优化
// 时间 O(S + sum(unique_len_i)), 空间 O(W)
// ============================================================
// 当 words 中有大量重复时, 先去重, 匹配后乘以出现次数
class Solution4 {
public:
    int numMatchingSubseq(string s, vector<string>& words) {
        // 去重统计
        unordered_map<string, int> wordCount;
        for (const string& w : words) {
            wordCount[w]++;
        }

        // 对不重复的单词构建桶
        vector<string> uniqueWords;
        vector<int> multipliers;
        for (auto& [w, cnt] : wordCount) {
            uniqueWords.push_back(w);
            multipliers.push_back(cnt);
        }

        // 桶并行推进
        vector<vector<pair<int,int>>> buckets(26);
        for (int i = 0; i < (int)uniqueWords.size(); ++i) {
            buckets[uniqueWords[i][0] - 'a'].emplace_back(i, 0);
        }

        int count = 0;
        for (char ch : s) {
            auto waiting = move(buckets[ch - 'a']);
            for (auto& [wi, ci] : waiting) {
                ci++;
                if (ci == (int)uniqueWords[wi].size()) {
                    count += multipliers[wi];
                } else {
                    buckets[uniqueWords[wi][ci] - 'a'].emplace_back(wi, ci);
                }
            }
        }

        return count;
    }
};
