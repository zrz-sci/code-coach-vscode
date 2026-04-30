// LeetCode 966: Vowel Spellchecker
// 解法一：三层哈希表 —— 推荐解法
// 时间 O((M+N)*L)，空间 O(M*L)

class Solution {
public:
    vector<string> spellchecker(vector<string>& wordlist, vector<string>& queries) {
        // 第一层：精确匹配（case-sensitive）
        unordered_set<string> exactSet(wordlist.begin(), wordlist.end());

        // 第二层：忽略大小写匹配 - key 是全小写，value 是词表中第一个匹配
        unordered_map<string, string> caseMap;

        // 第三层：元音模糊匹配 - key 是小写+元音替换为 '*'，value 是第一个匹配
        unordered_map<string, string> vowelMap;

        // 预处理词表（正序遍历，只保留第一次出现的映射）
        for (const string& word : wordlist) {
            string lower = toLower(word);
            string vkey = toVowelKey(lower);

            // emplace 在 key 已存在时不会覆盖，保证保留第一个
            caseMap.emplace(lower, word);
            vowelMap.emplace(vkey, word);
        }

        // 处理查询
        vector<string> result;
        result.reserve(queries.size());

        for (const string& query : queries) {
            // 优先级 1: 精确匹配
            if (exactSet.count(query)) {
                result.push_back(query);
                continue;
            }

            // 优先级 2: 忽略大小写匹配
            string lower = toLower(query);
            auto it = caseMap.find(lower);
            if (it != caseMap.end()) {
                result.push_back(it->second);
                continue;
            }

            // 优先级 3: 元音模糊匹配
            string vkey = toVowelKey(lower);
            auto it2 = vowelMap.find(vkey);
            if (it2 != vowelMap.end()) {
                result.push_back(it2->second);
                continue;
            }

            // 无匹配
            result.push_back("");
        }

        return result;
    }

private:
    // 将字符串转为全小写
    string toLower(const string& s) {
        string result = s;
        for (char& c : result) {
            if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';
            }
        }
        return result;
    }

    // 判断字符是否是元音（输入已经是小写）
    bool isVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

    // 将小写字符串中的元音替换为 '*'
    string toVowelKey(const string& lower) {
        string result = lower;
        for (char& c : result) {
            if (isVowel(c)) {
                c = '*';
            }
        }
        return result;
    }
};


// ================================================================
// 解法二：单次遍历暴力法（适合理解逻辑）
// 时间 O(M*N*L)，空间 O(1) 额外空间（不含结果）
// ================================================================

class Solution2 {
public:
    vector<string> spellchecker(vector<string>& wordlist, vector<string>& queries) {
        vector<string> result;

        for (const string& query : queries) {
            string match = findMatch(wordlist, query);
            result.push_back(match);
        }

        return result;
    }

private:
    string findMatch(const vector<string>& wordlist, const string& query) {
        string caseMatch = "";    // 第一个忽略大小写匹配
        string vowelMatch = "";   // 第一个元音模糊匹配

        for (const string& word : wordlist) {
            // 精确匹配（最高优先级），找到直接返回
            if (word == query) {
                return query;
            }

            // 忽略大小写匹配（只保留第一个）
            if (caseMatch.empty() && equalsIgnoreCase(word, query)) {
                caseMatch = word;
            }

            // 元音模糊匹配（只保留第一个）
            if (vowelMatch.empty() && vowelMatch_check(word, query)) {
                vowelMatch = word;
            }
        }

        // 按优先级返回
        if (!caseMatch.empty()) return caseMatch;
        if (!vowelMatch.empty()) return vowelMatch;
        return "";
    }

    bool equalsIgnoreCase(const string& a, const string& b) {
        if (a.size() != b.size()) return false;
        for (int i = 0; i < (int)a.size(); i++) {
            if (tolower(a[i]) != tolower(b[i])) return false;
        }
        return true;
    }

    bool isVowel(char c) {
        c = tolower(c);
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

    // 检查两个词在忽略大小写+元音替换后是否相同
    bool vowelMatch_check(const string& word, const string& query) {
        if (word.size() != query.size()) return false;
        for (int i = 0; i < (int)word.size(); i++) {
            char w = tolower(word[i]);
            char q = tolower(query[i]);
            if (w == q) continue;
            // 两个字符不同，必须都是元音位置才能匹配
            if (!isVowel(w) || !isVowel(q)) return false;
        }
        return true;
    }
};


// ================================================================
// 解法三：使用 transform 和 lambda 的现代 C++ 风格
// 时间 O((M+N)*L)，空间 O(M*L)
// ================================================================

class Solution3 {
public:
    vector<string> spellchecker(vector<string>& wordlist, vector<string>& queries) {
        // 辅助 lambda
        auto lower = [](string s) {
            for (auto& c : s) c = tolower(c);
            return s;
        };

        auto devowel = [&lower](const string& s) {
            string t = lower(s);
            for (auto& c : t) {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    c = '#';
                }
            }
            return t;
        };

        // 构建三层查找结构
        unordered_set<string> exact(wordlist.begin(), wordlist.end());
        unordered_map<string, string> cap;
        unordered_map<string, string> vow;

        for (auto& w : wordlist) {
            cap.emplace(lower(w), w);
            vow.emplace(devowel(w), w);
        }

        // 处理查询
        vector<string> ans;
        ans.reserve(queries.size());

        for (auto& q : queries) {
            if (exact.count(q)) {
                ans.push_back(q);
            } else if (cap.count(lower(q))) {
                ans.push_back(cap[lower(q)]);
            } else if (vow.count(devowel(q))) {
                ans.push_back(vow[devowel(q)]);
            } else {
                ans.push_back("");
            }
        }

        return ans;
    }
};
