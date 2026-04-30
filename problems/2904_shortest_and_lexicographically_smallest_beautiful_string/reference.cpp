// 解法1: 滑动窗口 - 经典变长窗口写法
// 时间 O(n^2), 空间 O(n)
//
// 核心: 右指针扩展计数 '1'，达到 k 个后收缩左端去掉前导 '0'
// 收缩后的窗口是候选美丽子串，比较取最短且字典序最小
//
// 收缩策略:
// - ones > k: 去掉左端字符(包括 '1')直到 ones == k
// - ones == k 且 s[left] == '0': 去掉前导 0 使窗口更短
// - ones == k 且 s[left] == '1': 不能再收缩，否则 ones < k
class Solution1 {
public:
    string shortestBeautifulSubstring(string s, int k) {
        int n = s.size();
        int ones = 0;       // 窗口内 '1' 的计数
        string result = "";  // 最优答案

        for (int left = 0, right = 0; right < n; right++) {
            // 扩展: 右指针纳入新字符
            if (s[right] == '1') ones++;

            // 收缩: 去掉前导 '0' 或多余的 '1'
            // 条件1: ones > k -> 必须去掉 '1' 才能恢复到 k
            // 条件2: s[left] == '0' -> 去掉前导 0 可以缩短窗口
            while (ones > k || (left <= right && s[left] == '0')) {
                if (s[left] == '1') {
                    if (ones == k) break; // 恰好 k 个，不能再去
                    ones--;               // ones > k，去掉一个 '1'
                }
                left++;
            }

            // 检查是否为美丽子串
            if (ones == k) {
                string candidate = s.substr(left, right - left + 1);
                // 更新答案: 更短 或 等长但字典序更小
                if (result.empty() ||
                    candidate.size() < result.size() ||
                    (candidate.size() == result.size() && candidate < result)) {
                    result = candidate;
                }
            }
        }
        return result;
    }
};

// 解法2: 预处理 '1' 位置 - 直接枚举候选区间
// 时间 O(n^2), 空间 O(n)
//
// 核心观察: 最短美丽子串一定以 '1' 开头、以 '1' 结尾
// (否则去掉首尾的 '0' 可以更短且仍有 k 个 '1')
//
// 所以: 收集所有 '1' 的位置 pos，
// 枚举每对 (pos[i], pos[i+k-1]) 作为第 1 和第 k 个 '1'
// 子串 = s[pos[i]..pos[i+k-1]], 长度 = pos[i+k-1] - pos[i] + 1
class Solution2 {
public:
    string shortestBeautifulSubstring(string s, int k) {
        // 第一步: 收集所有 '1' 的位置
        vector<int> pos;
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == '1') pos.push_back(i);
        }

        // 特判: '1' 的总数不足 k
        if ((int)pos.size() < k) return "";

        string result = "";
        int minLen = INT_MAX;

        // 第二步: 枚举每对起止 '1'
        for (int i = 0; i + k - 1 < (int)pos.size(); i++) {
            int start = pos[i];          // 第 i 个 '1' 的位置 (子串起点)
            int end = pos[i + k - 1];    // 第 i+k-1 个 '1' 的位置 (子串终点)
            int len = end - start + 1;

            // 更新: 更短 或 等长但字典序更小
            if (len < minLen) {
                minLen = len;
                result = s.substr(start, len);
            } else if (len == minLen) {
                string candidate = s.substr(start, len);
                if (candidate < result) {
                    result = candidate;
                }
            }
        }

        return result;
    }
};

// 解法3: 暴力枚举所有子串
// 时间 O(n^3), 空间 O(n)
//
// 对每个起点 i，向右扫描直到找到第 k 个 '1'
// 此时 s[i..j] 就是以 i 开头的最短美丽子串
// 在所有候选中取最短且字典序最小
class Solution3 {
public:
    string shortestBeautifulSubstring(string s, int k) {
        int n = s.size();
        string result = "";

        for (int i = 0; i < n; i++) {
            int ones = 0;
            for (int j = i; j < n; j++) {
                if (s[j] == '1') ones++;

                if (ones == k) {
                    // 找到了以 i 开头的最短美丽子串
                    string candidate = s.substr(i, j - i + 1);
                    if (result.empty() ||
                        candidate.size() < result.size() ||
                        (candidate.size() == result.size() && candidate < result)) {
                        result = candidate;
                    }
                    break; // 对固定 i，第一个恰好 k 个就是最短的
                }
            }
        }
        return result;
    }
};

/*
 * 解法对比:
 * ┌──────────────────────┬────────┬────────┬──────────────────────────┐
 * │ 解法                 │ 时间   │ 空间   │ 特点                     │
 * ├──────────────────────┼────────┼────────┼──────────────────────────┤
 * │ Solution1 滑动窗口   │ O(n^2) │ O(n)   │ 经典窗口，面试推荐       │
 * │ Solution2 预处理位置 │ O(n^2) │ O(n)   │ 思路清晰，利用了关键观察 │
 * │ Solution3 暴力枚举   │ O(n^3) │ O(n)   │ 最简单，n<=100 可通过    │
 * └──────────────────────┴────────┴────────┴──────────────────────────┘
 *
 * 图解 (Example 1: s = "100011001", k = 3):
 *
 *   '1' 位置: [0, 4, 5, 8]
 *
 *   Solution2 枚举:
 *     i=0: s[0..5] = "100011"  长度 6
 *     i=1: s[4..8] = "11001"   长度 5 <- 更短! 答案
 *
 *   Solution1 滑动窗口:
 *     right=5: ones=3, 窗口="100011"   候选(len=6)
 *     right=8: ones=4>3, 收缩到 left=4
 *              窗口="11001"             候选(len=5) <- 更短! 答案
 *
 * 关键观察: 最短美丽子串一定以 '1' 开头和结尾
 *   证明: 如果以 '0' 开头，去掉它不改变 '1' 数量但更短 -> 矛盾
 *
 * 测试验证:
 *   "100011001", k=3 -> "11001"
 *   "1011",      k=2 -> "11"
 *   "000",       k=1 -> ""
 *   "1",         k=1 -> "1"
 *   "11111",     k=3 -> "111"
 *   "10101",     k=2 -> "101"
 *
 * 易错点:
 * 1. 总 '1' 数量 < k 时返回空字符串
 * 2. 收缩时 ones==k 且 s[left]=='1' 不能继续收缩
 * 3. 字典序比较只在长度相同时进行
 * 4. substr 的参数是 (起始位置, 长度) 不是 (起始, 终止)
 *
 * 面试追问:
 * Q: 为什么最短子串以 '1' 开头和结尾?
 *    -> 去掉首尾 '0' 不改变 '1' 数量但更短
 * Q: O(n) 能做到吗?
 *    -> 找最短间距 O(m)，但字典序比较最坏 O(n)
 * Q: 恰好 k 个 '0' 呢?
 *    -> 相同滑动窗口框架，改计数条件
 */
