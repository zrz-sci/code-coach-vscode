/*
 * 【题目本质】
 * 给定字符串，生成所有不重复的回文排列。
 * 核心思路：只排列一半 + 中间字符 + 反转另一半。
 *
 * 【解法总览】
 * 解法1: next_permutation 枚举 - 代码最短 ⭐
 * 解法2: 回溯 + 频率数组 - 面试常考，天然去重
 * 解法3: 回溯 + 排序去重 - 经典回溯模板
 */

// ============================================================
// 解法1: next_permutation ⭐ 推荐（代码最简洁）
// 思路：
//   1. 统计频率，检查可行性
//   2. 构造半串（每个字符取一半），找到中间字符（如果有）
//   3. 排序半串，用 next_permutation 枚举所有排列
//   4. 每个排列 + mid + 反转 = 回文
// 时间 O((n/2)! * n)，空间 O(n)
// ============================================================
class Solution1 {
public:
    vector<string> generatePalindromes(string s) {
        // 统计频率
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;

        // 构造半串和中间字符
        string mid = "", half = "";
        for (auto& [ch, cnt] : freq) {
            if (cnt & 1) {
                if (!mid.empty()) return {}; // 两个以上奇数次 -> 无回文
                mid = string(1, ch);
            }
            half += string(cnt / 2, ch);
        }

        // 排序后枚举所有排列
        sort(half.begin(), half.end());
        vector<string> res;
        do {
            // 半串 + mid + 反转半串
            res.push_back(half + mid + string(half.rbegin(), half.rend()));
        } while (next_permutation(half.begin(), half.end()));

        return res;
    }
};

// ============================================================
// 解法2: 回溯 + 频率数组（天然去重）
// 思路：
//   将频率减半作为可用次数，回溯填充每个位置
//   由于是按字符遍历 map，同一位置不会重复选同一字符
// 时间 O((n/2)!)，空间 O(n)
// ============================================================
class Solution2 {
public:
    vector<string> result;

    void backtrack(unordered_map<char, int>& freq, string& path,
                   int halfLen, const string& mid) {
        if ((int)path.size() == halfLen) {
            // 构造完整回文：path + mid + reverse(path)
            string rev(path.rbegin(), path.rend());
            result.push_back(path + mid + rev);
            return;
        }

        for (auto& [ch, cnt] : freq) {
            if (cnt > 0) {
                cnt--;
                path.push_back(ch);
                backtrack(freq, path, halfLen, mid);
                path.pop_back();
                cnt++;
            }
        }
    }

    vector<string> generatePalindromes(string s) {
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;

        // 检查可行性 & 构造
        string mid = "";
        int halfLen = 0;
        for (auto& [ch, cnt] : freq) {
            if (cnt & 1) {
                if (!mid.empty()) return {};
                mid = string(1, ch);
            }
            freq[ch] = cnt / 2; // 频率减半
            halfLen += cnt / 2;
        }

        string path;
        backtrack(freq, path, halfLen, mid);
        return result;
    }
};

// ============================================================
// 解法3: 回溯 + 排序去重（经典模板）
// 思路：
//   构造半串后排序，回溯时用 used 数组 + 跳过相邻重复
//   这是 47. Permutations II 的标准去重手法
// 时间 O((n/2)!)，空间 O(n)
// ============================================================
class Solution3 {
public:
    vector<string> result;

    void backtrack(string& half, vector<bool>& used, string& path,
                   const string& mid) {
        if (path.size() == half.size()) {
            string rev(path.rbegin(), path.rend());
            result.push_back(path + mid + rev);
            return;
        }

        for (int i = 0; i < (int)half.size(); i++) {
            if (used[i]) continue;
            // 去重：相同字符且前一个未使用则跳过
            if (i > 0 && half[i] == half[i - 1] && !used[i - 1]) continue;

            used[i] = true;
            path.push_back(half[i]);
            backtrack(half, used, path, mid);
            path.pop_back();
            used[i] = false;
        }
    }

    vector<string> generatePalindromes(string s) {
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;

        string mid = "", half = "";
        for (auto& [ch, cnt] : freq) {
            if (cnt & 1) {
                if (!mid.empty()) return {};
                mid = string(1, ch);
            }
            half += string(cnt / 2, ch);
        }

        sort(half.begin(), half.end());
        vector<bool> used(half.size(), false);
        string path;
        backtrack(half, used, path, mid);
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法              | 时间          | 空间  | 特点               |
 * |------------------|-------------|------|-------------------|
 * | next_permutation | O((n/2)!*n) | O(n) | 代码最短            |
 * | 频率回溯          | O((n/2)!)   | O(n) | 天然去重，面试展示   |
 * | 排序去重回溯       | O((n/2)!)   | O(n) | 经典模板，通用性强   |
 *
 * 【易错点】
 * 1. 对整个字符串排列而非半串
 *    -> 半串排列 + 镜像翻转才能保证回文
 * 2. 频率回溯时忘记把 cnt 减半
 *    -> freq[ch] = cnt/2 是关键步骤
 * 3. 排序去重条件写反
 *    -> 应该是 !used[i-1]（跳过同层重复），不是 used[i-1]
 * 4. 忘记 mid 可以为空
 *    -> 偶数长度字符串没有中间字符
 *
 * 【面试追问】
 * Q: 为什么排序去重时用 !used[i-1] 而不是 used[i-1]？
 * A: !used[i-1] 表示同一层（同一递归深度）前面的相同字符没被选，
 *    说明当前是重复选择的第二次，应跳过。
 *    used[i-1] 也能正确去重但会多剪一些不必要的分支。
 *
 * Q: n=16 时最大排列数？
 * A: 半串长度 8，最坏 8! = 40320，非常小，不会超时。
 */
