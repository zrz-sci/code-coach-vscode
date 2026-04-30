/*
 * 【题目本质】
 * 判断两个字符串的编辑距离是否恰好为 1。
 * 编辑距离为 1 意味着恰好一次替换（等长）、插入或删除（长度差1）。
 * 关键是分类讨论 + 找第一个不同位置。
 *
 * 【解法总览】
 * 解法1: 分类讨论 + substr 比较  O(n)/O(n) ⭐ 代码最简洁
 * 解法2: 双指针逐字符比较       O(n)/O(1)   面试推荐（无额外空间）
 *
 * 分类讨论图示:
 *
 *   |len(s)-len(t)| > 1  →  false (不可能一步到位)
 *   |len(s)-len(t)| = 0  →  替换场景: 找第一个不同，剩余必须全同
 *   |len(s)-len(t)| = 1  →  插入场景: 找第一个不同，短串从i、长串从i+1继续比较
 *
 *   s = "ab"    t = "acb"     (插入)
 *        ^           ^^
 *   位置1不同 → s[1:] == t[2:] → "b" == "b" ✓
 *
 *   s = "abc"   t = "adc"     (替换)
 *        ^           ^
 *   位置1不同 → s[2:] == t[2:] → "c" == "c" ✓
 */

// ===================== 解法1: 分类讨论 + substr =====================
// 思路：找到第一个分歧点后，用 substr 一次性比较剩余部分
// 为什么先保证 s 较短：消除"插入/删除"两种情况的重复代码
class Solution1 {
public:
    bool isOneEditDistance(string s, string t) {
        int ns = s.size(), nt = t.size();
        // 统一成 s 较短的情况，减少分支
        if (ns > nt) return isOneEditDistance(t, s);

        // 长度差 > 1：不可能一步操作
        if (nt - ns > 1) return false;

        for (int i = 0; i < ns; ++i) {
            if (s[i] != t[i]) {
                if (ns == nt) {
                    // 替换：跳过这一个字符，剩余必须完全相同
                    return s.substr(i + 1) == t.substr(i + 1);
                } else {
                    // 插入/删除：t在位置i多了一个字符
                    // s从i开始 应该等于 t从i+1开始
                    return s.substr(i) == t.substr(i + 1);
                }
            }
        }

        // 遍历完 s 没发现不同
        // 如果 t 恰好多一个尾部字符 → true，否则两串相同 → false
        return nt - ns == 1;
    }
};

// ===================== 解法2: 双指针法 (O(1) 空间) =====================
// 思路：不用 substr，逐字符比较避免额外空间
// 为什么不用 DP：编辑距离 DP 是 O(mn)，这里只需判断==1
class Solution2 {
public:
    bool isOneEditDistance(string s, string t) {
        int ns = s.size(), nt = t.size();
        if (ns > nt) return isOneEditDistance(t, s);
        if (nt - ns > 1) return false;

        bool foundDiff = false;
        int i = 0, j = 0;

        while (i < ns && j < nt) {
            if (s[i] != t[j]) {
                if (foundDiff) return false;  // 第二次不同 → 超过1步
                foundDiff = true;
                if (ns == nt) {
                    i++; j++;  // 替换：两个指针都跳过
                } else {
                    j++;      // 插入：只跳过 t 中多出的字符
                }
            } else {
                i++; j++;
            }
        }

        // foundDiff=false 且遍历完 → 要么完全相同(false)，要么t多一个尾字符(true)
        return foundDiff || (nt - ns == 1);
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | substr比较 | O(n) | O(n) | 代码简洁，利用 substr 一行比较 |
 * | 双指针 | O(n) | O(1) | 无额外空间，面试推荐 |
 *
 * 【易错点】
 * 1. s == t 时应返回 false（编辑距离=0，不是1）
 * 2. s="" t="" 应返回 false（不是 one edit）
 * 3. 等长时发现第一个不同就返回 true → 错！后面可能还有不同
 * 4. 忘记处理"前缀完全相同但 t 多一个尾字符"的情况
 *
 * 【面试追问】
 * Q1: 和 LC 72 编辑距离的区别？ → 72 求最小编辑次数(DP, O(mn))，本题利用==1约束 O(n)
 * Q2: 推广到编辑距离 <= k？ → 对角线 DP，O(nk) 时间
 * Q3: 能否用位运算加速？ → 比较阶段可以用 SIMD 加速，但面试不需要
 */
