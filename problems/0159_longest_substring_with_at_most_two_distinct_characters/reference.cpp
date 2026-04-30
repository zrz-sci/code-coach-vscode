/*
 * 【题目本质】
 * 滑动窗口经典问题：在字符串中找最长的窗口，使得窗口内不同字符种类 <= 2。
 * 核心是维护窗口的"不变量"（invariant）：distinct count <= 2。
 *
 * 【解法总览】
 * 解法1: 滑动窗口 + 哈希计数  O(n)/O(1) ⭐ 面试首选（通用模板）
 * 解法2: 滑动窗口 + 最后出现位置  O(n)/O(1) 更巧妙，面试加分
 *
 * 窗口滑动过程（示例 s = "eceba"）:
 *
 *   e  c  e  b  a
 *   [e]              freq: {e:1}        -> 1
 *   [e  c]           freq: {e:1, c:1}   -> 2
 *   [e  c  e]        freq: {e:2, c:1}   -> 3 ***
 *   [e  c  e  b]     freq: {e:2,c:1,b:1}-> 违规! 收缩
 *      [c  e  b]     freq: {e:1,c:1,b:1}-> 还违规
 *         [e  b]     freq: {e:1, b:1}   -> 2
 *         [e  b  a]  freq: {e:1,b:1,a:1}-> 违规! 收缩
 *            [b  a]  freq: {b:1, a:1}   -> 2
 *   答案 = 3
 */

// ===================== 解法1: 滑动窗口 + 哈希计数 =====================
// 思路：这是"至多K种字符"的通用模板，K=2是特例
// 为什么选哈希表：需要 O(1) 查询窗口内字符种类数
class Solution1 {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        unordered_map<char, int> freq;  // 窗口内字符频率
        int left = 0, ans = 0;

        for (int right = 0; right < (int)s.size(); ++right) {
            freq[s[right]]++;

            // 为什么用 while：收缩一次可能不够，需要连续删直到满足不变量
            while (freq.size() > 2) {
                freq[s[left]]--;
                // 关键：频率为0必须删key，否则 size() 不准
                if (freq[s[left]] == 0) {
                    freq.erase(s[left]);
                }
                left++;
            }

            // 窗口 [left, right] 此时合法
            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};

// ===================== 解法2: 滑动窗口 + 最后出现位置 =====================
// 思路：不记频率，记每个字符最后出现的下标
// 优势：淘汰字符时 left 可以直接跳跃，不用逐步收缩
class Solution2 {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        unordered_map<char, int> lastPos;  // 字符 -> 最后出现的下标
        int left = 0, ans = 0;

        for (int right = 0; right < (int)s.size(); ++right) {
            lastPos[s[right]] = right;

            // 超过2种字符时，淘汰"最后出现位置最小"的字符
            if (lastPos.size() > 2) {
                int minIdx = INT_MAX;
                char minChar = 0;
                for (auto& [ch, idx] : lastPos) {
                    if (idx < minIdx) {
                        minIdx = idx;
                        minChar = ch;
                    }
                }
                // 为什么 left = minIdx + 1：被淘汰字符在 minIdx 及之前的
                // 所有出现都已被排除，新窗口从 minIdx+1 开始
                left = minIdx + 1;
                lastPos.erase(minChar);
            }

            ans = max(ans, right - left + 1);
        }

        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 哈希计数 | O(n) | O(1) | 通用模板，推广到K种 |
 * | 最后位置 | O(n) | O(1) | left跳跃式收缩更快 |
 *
 * 【易错点】
 * 1. freq[s[left]]-- 后忘记 erase → map.size() 永远不减
 * 2. 用 if 替代 while 做收缩 → 可能收缩不够
 * 3. 窗口长度写成 right-left 少了 +1
 *
 * 【面试追问】
 * Q1: 推广到至多 K 种？ → 改 > 2 为 > k，就是 LC 340
 * Q2: 要求恰好 K 种？ → atMost(K) - atMost(K-1)
 * Q3: 如果字符是 Unicode？ → 哈希表键变大但窗口内最多 K+1 个键，仍 O(1)
 */
