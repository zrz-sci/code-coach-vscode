// 784. Letter Case Permutation
// 枚举所有字母大小写的组合

// ============================================================
// 解法一: 回溯法 DFS (推荐面试写法)
// 时间复杂度: O(2^k * n), 空间复杂度: O(n) 递归栈
// k = 字母数量, n = 字符串长度
// ============================================================
class Solution {
public:
    vector<string> letterCasePermutation(string s) {
        vector<string> result;
        backtrack(s, 0, result);
        return result;
    }

    void backtrack(string& s, int idx, vector<string>& result) {
        // base case: 所有位置处理完毕
        if (idx == (int)s.size()) {
            result.push_back(s);
            return;
        }

        // 不管是字母还是数字, 先直接递归(保持原样)
        backtrack(s, idx + 1, result);

        // 如果是字母, 切换大小写后再递归一次
        if (isalpha(s[idx])) {
            s[idx] ^= 32; // 大小写切换: 'a'^32='A', 'A'^32='a'
            backtrack(s, idx + 1, result);
            s[idx] ^= 32; // 恢复(回溯)
        }
    }
};

// ============================================================
// 解法二: BFS 迭代法
// 时间复杂度: O(2^k * n), 空间复杂度: O(2^k * n)
// 逐字符扩展结果集
// ============================================================
class Solution_BFS {
public:
    vector<string> letterCasePermutation(string s) {
        vector<string> result = {s};

        for (int i = 0; i < (int)s.size(); ++i) {
            if (!isalpha(s[i])) continue; // 数字跳过

            // 对当前结果集中的每个字符串, 翻转位置i的大小写
            int sz = result.size();
            for (int j = 0; j < sz; ++j) {
                string copy = result[j];
                copy[i] ^= 32; // 切换大小写
                result.push_back(copy);
            }
        }

        return result;
    }
};

// ============================================================
// 解法三: 位掩码枚举
// 时间复杂度: O(2^k * n), 空间复杂度: O(2^k * n)
// 给每个字母分配一个bit, 枚举所有2^k种掩码
// ============================================================
class Solution_BitMask {
public:
    vector<string> letterCasePermutation(string s) {
        // 找出所有字母位置
        vector<int> letterPos;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (isalpha(s[i])) {
                letterPos.push_back(i);
            }
        }

        int k = letterPos.size();
        int total = 1 << k; // 2^k 种组合
        vector<string> result;

        for (int mask = 0; mask < total; ++mask) {
            string curr = s;
            for (int i = 0; i < k; ++i) {
                int pos = letterPos[i];
                if (mask & (1 << i)) {
                    // 该位为1: 转大写
                    curr[pos] = toupper(curr[pos]);
                } else {
                    // 该位为0: 转小写
                    curr[pos] = tolower(curr[pos]);
                }
            }
            result.push_back(curr);
        }

        return result;
    }
};

// ============================================================
// 解法四: 递归 (更简洁的写法)
// 时间复杂度: O(2^k * n), 空间复杂度: O(2^k * n)
// 利用返回值而非引用参数
// ============================================================
class Solution_Recursive {
public:
    vector<string> letterCasePermutation(string s) {
        return helper(s, 0);
    }

    vector<string> helper(string& s, int idx) {
        // base case
        if (idx == (int)s.size()) {
            return {s};
        }

        // 当前字符不是字母, 直接处理下一个
        if (!isalpha(s[idx])) {
            return helper(s, idx + 1);
        }

        // 当前字符是字母: 两种分叉
        vector<string> result;

        // 分叉1: 保持原样
        auto res1 = helper(s, idx + 1);
        result.insert(result.end(), res1.begin(), res1.end());

        // 分叉2: 切换大小写
        s[idx] ^= 32;
        auto res2 = helper(s, idx + 1);
        result.insert(result.end(), res2.begin(), res2.end());
        s[idx] ^= 32; // 恢复

        return result;
    }
};
