/*
 * 【题目本质】
 * 在字典中找最长的（相同长度取字典序最小的）字符串，使其是 s 的子序列。
 * 核心操作：双指针判断子序列关系。
 *
 * 【解法总览】
 * Solution1: 双指针逐个检查  O(n * S) / O(1)  ⭐推荐（无需排序）
 * Solution2: 排序 + 双指针   O(n*logn + n*S) / O(1)
 */

// ===================== Solution1: 双指针逐个检查（无需排序）=====================
// 思路：遍历字典每个单词，判断是否是 s 的子序列，维护最优答案
class Solution1 {
public:
    bool isSubseq(const string& s, const string& word) {
        int i = 0; // word 的指针
        for (int j = 0; i < word.size() && j < s.size(); j++) {
            if (word[i] == s[j]) i++;
        }
        return i == word.size();
    }

    string findLongestWord(string s, vector<string>& dictionary) {
        string result = "";
        for (auto& word : dictionary) {
            if (isSubseq(s, word)) {
                // 更长，或等长但字典序更小
                if (word.size() > result.size() ||
                    (word.size() == result.size() && word < result)) {
                    result = word;
                }
            }
        }
        return result;
    }
};

// ===================== Solution2: 排序 + 双指针 =====================
// 思路：先排序（长度降序、字典序升序），第一个匹配的即为答案
class Solution2 {
public:
    bool isSubseq(const string& s, const string& word) {
        int i = 0;
        for (int j = 0; i < word.size() && j < s.size(); j++) {
            if (word[i] == s[j]) i++;
        }
        return i == word.size();
    }

    string findLongestWord(string s, vector<string>& dictionary) {
        sort(dictionary.begin(), dictionary.end(), [](const string& a, const string& b) {
            return a.size() != b.size() ? a.size() > b.size() : a < b;
        });

        for (auto& word : dictionary) {
            if (isSubseq(s, word)) return word;
        }
        return "";
    }
};

/*
 * 【解法对比】
 * Solution1: 无排序开销，需完整遍历字典，代码更简洁
 * Solution2: 排序后可提前返回，但排序本身 O(n*logn)
 * 实际性能相近，Solution1 推荐度更高
 *
 * 【易错点】
 * 1. 比较规则遗漏字典序——相同长度必须取字典序最小
 * 2. 子序列方向搞反——是 word 为 s 的子序列，不是 s 为 word 的子序列
 * 3. 排序比较函数写错——应该长度降序、字典序升序
 *
 * 【面试追问】
 * Q1: 不排序 vs 排序哪个更好？
 * Q2: 如果字典固定、s 多次查询，如何优化？
 * Q3: 子序列匹配贪心的正确性证明？
 */
