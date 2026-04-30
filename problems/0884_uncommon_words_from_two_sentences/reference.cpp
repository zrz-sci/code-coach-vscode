/*
 * LeetCode 884: Uncommon Words from Two Sentences
 *
 * 解法一：合并词频统计
 *
 * 思路：
 * "Uncommon" 等价于"在两句话合并后总共只出现一次"。
 * 将两个句子的单词全部放入同一个词频表，筛选频次为 1 的单词。
 *
 * 时间复杂度：O(n + m)
 * 空间复杂度：O(n + m)
 */

class Solution {
public:
    vector<string> uncommonFromSentences(string s1, string s2) {
        // 使用哈希表统计所有单词的词频
        unordered_map<string, int> wordCount;

        // 解析 s1 中的单词
        // istringstream 自动按空格分割，跳过连续空格
        istringstream iss1(s1);
        string word;
        while (iss1 >> word) {
            wordCount[word]++;
        }

        // 解析 s2 中的单词
        istringstream iss2(s2);
        while (iss2 >> word) {
            wordCount[word]++;
        }

        // 筛选出词频恰好为 1 的单词
        vector<string> result;
        for (auto& [w, cnt] : wordCount) {
            if (cnt == 1) {
                result.push_back(w);
            }
        }

        return result;
    }
};

/*
 * 详细示例分析：
 *
 * 示例 1：s1 = "this apple is sweet", s2 = "this apple is sour"
 *
 * 词频统计：
 *   "this"  -> 2 (s1 + s2)
 *   "apple" -> 2 (s1 + s2)
 *   "is"    -> 2 (s1 + s2)
 *   "sweet" -> 1 (仅 s1)
 *   "sour"  -> 1 (仅 s2)
 *
 * 筛选词频 == 1：["sweet", "sour"]
 *
 * 示例 2：s1 = "apple apple", s2 = "banana"
 *
 * 词频统计：
 *   "apple"  -> 2 (s1 中出现两次)
 *   "banana" -> 1 (仅 s2)
 *
 * 筛选词频 == 1：["banana"]
 * 注意："apple" 虽然不在 s2 中，但在 s1 中出现了两次，不是 uncommon。
 */

/*
 * 解法二：手动字符串分割（不使用 istringstream）
 *
 * 对于面试中可能不记得 istringstream 的情况，
 * 可以手动按空格分割字符串。
 */
class Solution_ManualSplit {
public:
    vector<string> uncommonFromSentences(string s1, string s2) {
        unordered_map<string, int> wordCount;

        // 辅助函数：按空格分割并统计词频
        auto countWords = [&](const string& s) {
            int n = s.size();
            int start = 0;
            for (int i = 0; i <= n; i++) {
                if (i == n || s[i] == ' ') {
                    if (i > start) {
                        string w = s.substr(start, i - start);
                        wordCount[w]++;
                    }
                    start = i + 1;
                }
            }
        };

        countWords(s1);
        countWords(s2);

        vector<string> result;
        for (auto& [w, cnt] : wordCount) {
            if (cnt == 1) {
                result.push_back(w);
            }
        }

        return result;
    }
};

/*
 * 解法三：双集合对比法
 *
 * 分别统计两个句子的词频，然后交叉检查。
 * 虽然逻辑等价，但代码更冗余。仅作为对比理解。
 */
class Solution_TwoMaps {
public:
    vector<string> uncommonFromSentences(string s1, string s2) {
        unordered_map<string, int> freq1, freq2;

        // 分别统计两个句子的词频
        istringstream iss1(s1);
        string word;
        while (iss1 >> word) freq1[word]++;

        istringstream iss2(s2);
        while (iss2 >> word) freq2[word]++;

        vector<string> result;

        // 在 s1 中出现恰好一次，且不在 s2 中出现
        for (auto& [w, cnt] : freq1) {
            if (cnt == 1 && freq2.find(w) == freq2.end()) {
                result.push_back(w);
            }
        }

        // 在 s2 中出现恰好一次，且不在 s1 中出现
        for (auto& [w, cnt] : freq2) {
            if (cnt == 1 && freq1.find(w) == freq1.end()) {
                result.push_back(w);
            }
        }

        return result;
    }
};

/*
 * 为什么"合并词频"和"分开比较"是等价的？
 *
 * 设 f(w) = w 在合并后的总词频。
 *
 * f(w) == 1 成立的充要条件：
 *   w 在 s1 中出现 0 次且在 s2 中出现 1 次
 *   或
 *   w 在 s1 中出现 1 次且在 s2 中出现 0 次
 *
 * 这恰好是 uncommon 的定义：
 * "出现在某个句子中恰好一次，且不出现在另一个句子中"
 *
 * 所以合并词频 == 1 是最简洁的判断条件。
 */
