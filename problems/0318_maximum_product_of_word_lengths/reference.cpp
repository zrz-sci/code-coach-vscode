/*
 * 【题目本质】
 * 找两个无公共字母的单词使长度乘积最大
 * 核心：用 bitmask 将"是否有公共字母"的判断优化到 O(1)
 *
 * 【解法总览】
 * Solution1: 位掩码 + 枚举        ⭐⭐⭐⭐⭐  O(n^2 + nL) / O(n)
 * Solution2: 位掩码 + HashMap 去重 ⭐⭐⭐⭐    O(K^2 + nL) / O(K)，K=不同 mask 数
 */

// ===================== Solution1: 位掩码 + 双重枚举（推荐） =====================
// 每个单词用 26 位 bitmask 表示含有哪些字母
// 两个单词无公共字母 ⟺ mask[i] & mask[j] == 0
class Solution1 {
public:
    int maxProduct(vector<string>& words) {
        int n = words.size();
        vector<int> mask(n, 0);

        // Step 1: 预处理 bitmask
        // 对于 "abc"：mask = ...000111 (a=bit0, b=bit1, c=bit2)
        for (int i = 0; i < n; i++)
            for (char c : words[i])
                mask[i] |= (1 << (c - 'a'));

        // Step 2: 枚举所有对，检查是否无公共字母
        int maxProd = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // 注意括号！== 优先级高于 &
                if ((mask[i] & mask[j]) == 0) {
                    int prod = (int)(words[i].size() * words[j].size());
                    maxProd = max(maxProd, prod);
                }
            }
        }
        return maxProd;
    }
};

// ===================== Solution2: 位掩码 + HashMap 去重优化 =====================
// 多个单词可能有相同的字母集合 → 只需保留最长的
// 例如 "ab" 和 "aabb" 的 mask 相同，只需保留长度 4 的
class Solution2 {
public:
    int maxProduct(vector<string>& words) {
        // mask → 该 mask 对应的最大单词长度
        unordered_map<int, int> maskToLen;

        for (auto& w : words) {
            int m = 0;
            for (char c : w)
                m |= (1 << (c - 'a'));
            // 相同 mask 只保留最长
            maskToLen[m] = max(maskToLen[m], (int)w.size());
        }

        int maxProd = 0;
        for (auto& [m1, l1] : maskToLen) {
            for (auto& [m2, l2] : maskToLen) {
                if ((m1 & m2) == 0)
                    maxProd = max(maxProd, l1 * l2);
            }
        }
        return maxProd;
    }
};

/*
 * 【解法对比】
 * ┌──────────────────────┬──────────────┬──────────┬────────────────────────┐
 * │ 解法                 │ 时间         │ 空间     │ 特点                   │
 * ├──────────────────────┼──────────────┼──────────┼────────────────────────┤
 * │ Solution1 直接枚举   │ O(n^2 + nL)  │ O(n)     │ 简洁直观，面试首选     │
 * │ Solution2 去重优化   │ O(K^2 + nL)  │ O(K)     │ 有重复 mask 时更快     │
 * └──────────────────────┴──────────────┴──────────┴────────────────────────┘
 * K = 不同 bitmask 的数量，最坏 2^26 但实际远小于 n
 *
 * 【易错点】
 * 1. 运算符优先级：(mask[i] & mask[j]) == 0 必须加括号，== 优先级高于 &
 * 2. 返回的是单词长度的乘积，不是 mask 的乘积
 * 3. size() 返回 size_t（无符号），两个相乘可能溢出 → 转 int 或用 long long
 * 4. 没有满足条件的对时返回 0（初始值）
 * 5. bitmask 只需 26 位，int（32 位）足够，不需要 long long
 *
 * 【面试追问】
 * Q1: 为什么用 bitmask？→ 26 个字母 ≤ 32 位 int，交集判断 O(1)
 * Q2: 时间复杂度？→ O(n^2 + nL)，预处理 O(nL) + 枚举 O(n^2)
 * Q3: 输出具体单词？→ 更新 maxProd 时记录 best_i, best_j
 */
