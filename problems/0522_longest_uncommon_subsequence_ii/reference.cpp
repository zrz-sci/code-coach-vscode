/*
 * 【题目本质】
 * 在字符串数组中找最长的"特殊序列"——即不是其他任何字符串子序列的字符串。
 * 关键洞察：答案一定是某个完整字符串本身，不会是某个字符串的真子序列。
 *
 * 【解法总览】
 * Solution1: 排序 + 子序列检查  O(n^2 * L) / O(1)  ⭐推荐
 */

// ===================== Solution1: 排序 + 子序列检查 =====================
// 思路：按长度降序排序，对每个字符串检查它是否不是其他任何字符串的子序列
class Solution1 {
public:
    bool isSubseq(const string& a, const string& b) {
        // 判断 a 是否是 b 的子序列
        int i = 0;
        for (int j = 0; i < a.size() && j < b.size(); j++) {
            if (a[i] == b[j]) i++;
        }
        return i == a.size();
    }

    int findLUSlength(vector<string>& strs) {
        // 按长度降序排序，优先检查长字符串
        sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
            return a.size() > b.size();
        });

        for (int i = 0; i < strs.size(); i++) {
            bool isUncommon = true;
            for (int j = 0; j < strs.size(); j++) {
                if (i != j && isSubseq(strs[i], strs[j])) {
                    isUncommon = false;
                    break;
                }
            }
            if (isUncommon) return strs[i].size();
        }
        return -1;
    }
};

// ===================== Solution2: 哈希去重 + 子序列检查 =====================
// 思路：先用哈希统计重复字符串，重复的一定不是答案，再检查唯一字符串
class Solution2 {
public:
    bool isSubseq(const string& a, const string& b) {
        int i = 0;
        for (int j = 0; i < a.size() && j < b.size(); j++) {
            if (a[i] == b[j]) i++;
        }
        return i == a.size();
    }

    int findLUSlength(vector<string>& strs) {
        unordered_map<string, int> freq;
        for (auto& s : strs) freq[s]++;

        // 按长度降序排序
        sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
            return a.size() > b.size();
        });

        for (int i = 0; i < strs.size(); i++) {
            // 出现多次的字符串一定不是特殊序列
            if (freq[strs[i]] > 1) continue;

            bool isUncommon = true;
            for (int j = 0; j < strs.size(); j++) {
                if (i != j && isSubseq(strs[i], strs[j])) {
                    isUncommon = false;
                    break;
                }
            }
            if (isUncommon) return strs[i].size();
        }
        return -1;
    }
};

/*
 * 【解法对比】
 * Solution1: 简洁直接，排序后逐个检查
 * Solution2: 增加去重优化，跳过重复字符串
 * 两者时间复杂度相同 O(n^2 * L)，Solution2 实际运行可能更快
 *
 * 【易错点】
 * 1. 忘记 i != j 跳过自身比较（自己一定是自己的子序列）
 * 2. 混淆"子序列"和"子串"——子序列可以不连续
 * 3. 认为答案可能是某个字符串的真子序列（实际上一定是完整字符串）
 *
 * 【面试追问】
 * Q1: 为什么答案一定是完整字符串？
 * Q2: 如何 O(n) 判断子序列关系？
 * Q3: 如果数据量很大如何优化？
 */
