/*
 * 【题目本质】
 * 在字符串中找最长子串，使得子串内不同字符数 <= k。
 * 典型的"可变长度滑动窗口 + 约束条件"问题。
 *
 * 【解法总览】
 * 解法1: 滑动窗口 + HashMap频次统计   O(n) / O(k) ⭐推荐
 * 解法2: 滑动窗口 + 不缩小窗口优化     O(n) / O(k)  代码更精简
 */

// ===================== Solution 1: 滑动窗口 + HashMap =====================
// 思路：右指针扩张，左指针在字符种类 > k 时收缩
// 时间 O(n)，空间 O(k)
class Solution1 {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        if (k == 0) return 0;

        unordered_map<char, int> freq;  // 字符 -> 窗口内出现次数
        int left = 0, ans = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            freq[s[right]]++;

            // 当不同字符数超过 k，收缩左边界
            while ((int)freq.size() > k) {
                char c = s[left];
                if (--freq[c] == 0) {
                    freq.erase(c);  // 关键：频次归零必须删除键
                }
                left++;
            }

            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};

// ===================== Solution 2: 不缩小窗口优化 =====================
// 思路：窗口只扩大不缩小。当 freq.size() > k 时，左右各移一步，
//       窗口大小不变。最终窗口大小就是答案。
// 原理：我们只关心"最大"合法窗口长度，不合法时维持窗口大小即可。
// 时间 O(n)，空间 O(k)
class Solution2 {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        if (k == 0) return 0;

        unordered_map<char, int> freq;
        int left = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            freq[s[right]]++;

            // 注意：这里用 if 而非 while
            // 不合法时只移动 left 一步，窗口大小不缩小
            if ((int)freq.size() > k) {
                char c = s[left];
                if (--freq[c] == 0) {
                    freq.erase(c);
                }
                left++;
            }
        }

        // 最终窗口大小 = s.size() - left
        return (int)s.size() - left;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 | O(n) | O(k) | 直观易懂，面试首选 | while 循环稍复杂 |
 * | Solution2 | O(n) | O(k) | 代码更精简 | 理解"不缩小"需要推理 |
 *
 * 【易错点】
 * 1. k == 0 必须特判，否则 while 循环会一直收缩导致错误
 * 2. 频次减为 0 后必须 erase，否则 map.size() 不能正确反映字符种类数
 * 3. 条件是 size() > k（严格大于），不是 >= k
 * 4. Solution2 中用 if 替代 while 是核心区别，需要面试时解释原理
 *
 * 【面试追问】
 * Q1: "恰好 k 个不同字符" → 用 f(k) - f(k-1) 的减法技巧
 * Q2: 如果数据是流式的，如何在线处理？ → 本解法天然支持在线，逐字符处理
 * Q3: 能否用数组替代 HashMap？ → 可以，用 int[256] + 一个 count 变量记录种类数
 */
