/*
 * LeetCode 916: 单词子集 (Word Subsets)
 *
 * 【题目本质】
 * 给定 words1 和 words2，找 words1 中满足"对 words2 中每个词 b，
 * b 都是 a 的字频子集"的所有词 a。
 * 核心技巧：将 words2 中所有词的字频需求合并（取 max），
 * 然后对 words1 逐个检查。
 *
 * 【解法总览】
 * 解法1: 暴力检查         — O(m*n*L) / O(26)   — 对每个a检查所有b
 * 解法2: 合并需求数组 ⭐  — O((m+n)*L) / O(26) — 面试首选
 */

// ============================================================
// 解法1: 暴力检查 — 对每个 a 逐个验证所有 b
// 时间: O(m * n * L)  空间: O(26)
//   m = |words1|, n = |words2|, L = 最大词长度(<=10)
//
// 【思路】
// 最直觉的方法：对 words1 中的每个词 a，遍历 words2 中的每个词 b，
// 统计 a 和 b 的字频，检查 b 的每个字母计数是否 <= a 的对应计数。
// 所有 b 都通过则 a 是"通用"的。
//
// 瓶颈：对每个 a 都重复遍历所有 b 并重复统计字频。
// ============================================================
class Solution1 {
public:
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        vector<string> result;

        for (auto& a : words1) {
            // 统计 a 的字频
            vector<int> freqA(26, 0);
            for (char c : a) freqA[c - 'a']++;

            bool universal = true;
            for (auto& b : words2) {
                // 统计 b 的字频
                vector<int> freqB(26, 0);
                for (char c : b) freqB[c - 'a']++;

                // 检查 b 是否是 a 的字频子集
                for (int i = 0; i < 26; i++) {
                    if (freqB[i] > freqA[i]) {
                        universal = false;
                        break;
                    }
                }
                if (!universal) break;
            }

            if (universal) result.push_back(a);
        }

        return result;
    }
};

// ============================================================
// 解法2: 合并需求数组 — 预处理 words2 的最大字频需求 ⭐ 面试首选
// 时间: O((m + n) * L)  空间: O(26)
//
// 【思路】
// 从暴力解的瓶颈出发：每个 a 都要遍历所有 b，能否只遍历一次 words2？
//
// 关键观察：a 是"通用"的条件等价于"对每个字母 ch，
// a 中 ch 的出现次数 >= words2 中所有词对 ch 需求的最大值"。
//
// 所以可以预处理 words2：对每个字母 ch，取所有 words2 中
// ch 出现次数的 max，得到一个合并后的"最大需求"数组 maxFreq。
// 然后对 words1 中每个词 a，只需检查 freqA 是否在每一位上 >= maxFreq。
//
// 合并示意 (words2 = ["lc", "eo"]):
//
//   "lc" → l=1, c=1
//   "eo" → e=1, o=1
//
//   maxFreq: c=1, e=1, l=1, o=1 (其他=0)
//
// 检查 words1:
//   "amazon" → a=2,m=1,z=1,o=1,n=1 → l=0<1 ✗
//   "apple" → a=1,p=2,l=1,e=1 → c=0<1 ✗
//   "leetcode" → l=1,e=3,t=1,c=1,o=1,d=1
//      → c=1>=1,e=3>=1,l=1>=1,o=1>=1 ✓
// ============================================================
class Solution2 {
public:
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        // 第一步：合并 words2 中所有词的字频需求
        // 对每个字母位置，取所有 b 中该字母出现次数的 max
        vector<int> maxFreq(26, 0);
        for (const auto& b : words2) {
            vector<int> freqB(26, 0);
            for (char c : b) freqB[c - 'a']++;
            // 合并：每个字母取 max（不是 sum！）
            for (int i = 0; i < 26; i++) {
                maxFreq[i] = max(maxFreq[i], freqB[i]);
            }
        }

        // 第二步：检查 words1 中每个词是否满足合并后的需求
        vector<string> result;
        for (const auto& a : words1) {
            // 统计 a 的字频
            vector<int> freqA(26, 0);
            for (char c : a) freqA[c - 'a']++;

            // 逐位检查 freqA[i] >= maxFreq[i]
            bool universal = true;
            for (int i = 0; i < 26; i++) {
                if (freqA[i] < maxFreq[i]) {
                    universal = false;
                    break;
                }
            }

            if (universal) result.push_back(a);
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间            | 空间   | 核心思想                    | 适用场景     |
// |---------------|----------------|--------|----------------------------|-------------|
// | 暴力检查      | O(m * n * L)   | O(26)  | 对每个 a 逐个检查所有 b     | 理解题意     |
// | 合并需求 ⭐   | O((m + n) * L) | O(26)  | 预处理合并 words2 的字频需求 | 面试首选     |
//
// ============================================================
// 【易错点】
//
// 1. 合并时取 max 而非 sum
//    ✗ maxFreq[i] += freqB[i];  → 需求过严，过多 a 被排除
//    ✓ maxFreq[i] = max(maxFreq[i], freqB[i]);
//    原因：a 需要"同时满足"所有 b，对字母 ch 只需满足
//    出现次数最多的那个 b 的需求即可。
//
// 2. 忘记对每个 b 独立统计字频
//    ✗ 把所有 b 的字符直接累加到同一个字频数组
//    ✓ 对每个 b 创建新的 freqB，然后和 maxFreq 取 max
//
// 3. 字频比较方向搞反
//    ✗ if (maxFreq[i] < freqA[i]) → 这是检查 a 是否是 maxFreq 的子集
//    ✓ if (freqA[i] < maxFreq[i]) → 检查 a 是否满足 maxFreq 的需求
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力解的瓶颈在哪？怎么优化？
// → 瓶颈是对每个 a 都遍历所有 b。优化：把 words2 合并成
//   一个需求数组，每个 a 只检查一次。
//
// Q2: 为什么合并用 max 不用 sum？
// → a 需要同时满足所有 b。如果 b1 需要 2 个 'a'，b2 需要 3 个 'a'，
//   a 有 3 个 'a' 就同时满足两者。sum 会要求 5 个，太严格了。
//
// Q3: 字符集大小如何影响复杂度？
// → 本题只有 26 个小写字母，字频统计和比较都是 O(26) 常数。
//   如果字符集更大（如 Unicode），需要用哈希表代替固定数组。
//
// Q4: words2 有重复词是否影响结果？
// → 不影响。取 max 操作对重复值是幂等的。
//
// ============================================================
// 【相关题型】
//
// - 49. Group Anagrams
//   同样需要字频统计。区别：49 是按字频分组，916 是字频的子集关系。
//
// - 383. Ransom Note
//   检查一个字符串的字频是否被另一个覆盖。是本题的简化版（一对而非多对）。
//
// - 350. Intersection of Two Arrays II
//   用字频的 min 操作求交集，与本题的 max 操作互为对偶。
// ============================================================
