// LeetCode 953: Verifying an Alien Dictionary
// 解法一：映射表 + 逐对比较（推荐）
// 时间复杂度: O(S), S = 所有单词的总字符数
// 空间复杂度: O(1) - 固定大小的映射表

class Solution {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        // 步骤1：构建字母优先级映射表
        // order_map[c] = 字母 c 在外星字典中的优先级（越小越靠前）
        int order_map[26];
        for (int i = 0; i < 26; i++) {
            order_map[order[i] - 'a'] = i;
        }

        // 步骤2：逐对比较相邻单词
        for (int i = 0; i < (int)words.size() - 1; i++) {
            if (!isOrdered(words[i], words[i + 1], order_map)) {
                return false;
            }
        }

        return true;
    }

private:
    // 检查 word1 是否 <= word2（按外星字典序）
    bool isOrdered(const string& word1, const string& word2, int order_map[]) {
        int len1 = word1.size(), len2 = word2.size();
        int minLen = min(len1, len2);

        for (int j = 0; j < minLen; j++) {
            int c1 = order_map[word1[j] - 'a'];
            int c2 = order_map[word2[j] - 'a'];

            if (c1 < c2) {
                // word1 确定小于 word2，合法
                return true;
            }
            if (c1 > c2) {
                // word1 大于 word2，不合法
                return false;
            }
            // c1 == c2，继续比较下一个字符
        }

        // 所有公共前缀字符都相同
        // 较短的单词必须在前面（"app" < "apple"）
        return len1 <= len2;
    }
};


// 解法一的内联版本（不用辅助函数）
// 逻辑完全相同，更紧凑

class Solution1b {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        int orderMap[26];
        for (int i = 0; i < 26; i++) {
            orderMap[order[i] - 'a'] = i;
        }

        for (int i = 0; i + 1 < (int)words.size(); i++) {
            const string& w1 = words[i];
            const string& w2 = words[i + 1];
            bool found = false;

            for (int j = 0; j < (int)min(w1.size(), w2.size()); j++) {
                if (orderMap[w1[j] - 'a'] < orderMap[w2[j] - 'a']) {
                    found = true;  // w1 < w2 确定
                    break;
                }
                if (orderMap[w1[j] - 'a'] > orderMap[w2[j] - 'a']) {
                    return false;  // w1 > w2，违反顺序
                }
            }

            // 如果没有找到决定性字符，检查长度
            if (!found && w1.size() > w2.size()) {
                return false;  // "apple" > "app"
            }
        }

        return true;
    }
};


// 解法二：替换字符后用标准字典序比较
// 思路：将外星字母映射为标准字母后，直接用 string 的 <= 比较
// 时间复杂度: O(S)
// 空间复杂度: O(S) - 需要存储替换后的单词

class Solution2 {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        // 构建替换表：外星字母 -> 标准字母
        // order 中第 i 个字母在标准中对应 'a' + i
        char replaceMap[26];
        for (int i = 0; i < 26; i++) {
            replaceMap[order[i] - 'a'] = 'a' + i;
        }

        // 将每个单词转换为标准字典序
        vector<string> transformed(words.size());
        for (int i = 0; i < (int)words.size(); i++) {
            transformed[i] = words[i];
            for (char& c : transformed[i]) {
                c = replaceMap[c - 'a'];
            }
        }

        // 用标准字典序比较
        for (int i = 0; i + 1 < (int)transformed.size(); i++) {
            if (transformed[i] > transformed[i + 1]) {
                return false;
            }
        }

        return true;
    }
};


// 解法三：利用 std::is_sorted 和自定义比较器
// 最简洁的写法
// 时间复杂度: O(S)
// 空间复杂度: O(1)

class Solution3 {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        int rank[26];
        for (int i = 0; i < 26; i++) {
            rank[order[i] - 'a'] = i;
        }

        // 自定义比较器：按外星字典序比较两个字符串
        auto alienLessOrEqual = [&](const string& a, const string& b) -> bool {
            int n = min(a.size(), b.size());
            for (int i = 0; i < n; i++) {
                if (rank[a[i] - 'a'] != rank[b[i] - 'a']) {
                    return rank[a[i] - 'a'] < rank[b[i] - 'a'];
                }
            }
            return a.size() <= b.size();
        };

        // 检查相邻对是否都满足 <=
        for (int i = 0; i + 1 < (int)words.size(); i++) {
            if (!alienLessOrEqual(words[i], words[i + 1])) {
                return false;
            }
        }
        return true;
    }
};
