/*
 * LeetCode 809: 情感丰富的文字 (Expressive Words)
 *
 * 【题目本质】
 * 将 s 和每个 word 按连续相同字符分组，逐组比较：
 * 字符相同、s 的组长 >= word 的组长、且扩展后的组长 >= 3。
 *
 * 【解法总览】
 * 解法1: 双指针逐组对比 — O(n * L) / O(1) — 面试首选
 * 解法2: 预处理分组数组 — O(n * L) / O(L) — 代码更清晰
 *
 * 核心判断：对应组 (char, lenS) vs (char, lenW)
 *   char不同 → FAIL
 *   lenS == lenW → OK (精确匹配)
 *   lenS > lenW && lenS >= 3 → OK (合法扩展)
 *   其余 → FAIL
 */

// ============================================================
// 解法1: 双指针逐组对比
// 时间: O(n * L)  空间: O(1)  其中 n = words.size(), L = max字符串长度
//
// 【思路】
// 对每个 word，用双指针同时在 s 和 word 上滑动:
//   1. 找出 s 中下一组连续相同字符的 (字符, 长度)
//   2. 找出 word 中下一组的 (字符, 长度)
//   3. 比较：字符相同 && (长度相等 || (lenS>lenW && lenS>=3))
//
// 扩展规则: 扩展操作将组大小增加到 >=3。
// 所以如果 lenS=2, lenW=1，虽然 lenS>lenW，但 2<3，扩展非法。
//
// 例子:
// s = "heeellooo"  →  (h,1)(e,3)(l,2)(o,3)
// w = "hello"      →  (h,1)(e,1)(l,2)(o,1)
//
// 组1: (h,1) vs (h,1) → 1==1 → OK
// 组2: (e,3) vs (e,1) → 3>1 && 3>=3 → OK (扩展)
// 组3: (l,2) vs (l,2) → 2==2 → OK (精确匹配)
// 组4: (o,3) vs (o,1) → 3>1 && 3>=3 → OK (扩展)
// 全部匹配 → stretchy ✓
//
// s = "heeellooo"  →  (h,1)(e,3)(l,2)(o,3)
// w = "helo"       →  (h,1)(e,1)(l,1)(o,1)
//
// 组1: (h,1) vs (h,1) → OK
// 组2: (e,3) vs (e,1) → 3>=3 → OK
// 组3: (l,2) vs (l,1) → 2>1 但 2<3 → FAIL ✗
// ============================================================
class Solution1 {
public:
    int expressiveWords(string s, vector<string>& words) {
        int count = 0;
        for (const string& word : words) {
            if (isStretchy(s, word)) count++;
        }
        return count;
    }

    bool isStretchy(const string& s, const string& w) {
        int i = 0, j = 0;
        int n = s.size(), m = w.size();

        while (i < n && j < m) {
            // 当前组的字符必须相同
            if (s[i] != w[j]) return false;

            char ch = s[i];

            // 统计 s 中这一组的长度
            int lenS = 0;
            while (i < n && s[i] == ch) { i++; lenS++; }

            // 统计 w 中这一组的长度
            int lenW = 0;
            while (j < m && w[j] == ch) { j++; lenW++; }

            // 判断是否合法
            if (lenS < lenW) return false;               // word组更长，不可能
            if (lenS != lenW && lenS < 3) return false;  // 扩展了但<3，非法
        }

        // 两个字符串都必须完整匹配
        return i == n && j == m;
    }
};

// ============================================================
// 解法2: 预处理分组数组
// 时间: O(n * L)  空间: O(L)
//
// 【思路】
// 先将 s 预处理成分组列表 [(char, count), ...]，
// 然后对每个 word 也做分组，逐组对比。
// 代码更清晰，但多了 O(L) 的空间。
// ============================================================
class Solution2 {
public:
    // 将字符串分解为 [(字符, 连续个数)] 的列表
    vector<pair<char, int>> getGroups(const string& s) {
        vector<pair<char, int>> groups;
        int i = 0, n = s.size();
        while (i < n) {
            char ch = s[i];
            int count = 0;
            while (i < n && s[i] == ch) { i++; count++; }
            groups.push_back({ch, count});
        }
        return groups;
    }

    int expressiveWords(string s, vector<string>& words) {
        auto sGroups = getGroups(s);
        int count = 0;

        for (const string& word : words) {
            auto wGroups = getGroups(word);

            // 组数必须相同
            if (sGroups.size() != wGroups.size()) continue;

            bool ok = true;
            for (int k = 0; k < (int)sGroups.size(); k++) {
                auto [sc, slen] = sGroups[k];
                auto [wc, wlen] = wGroups[k];

                // 字符不同 → 不匹配
                if (sc != wc) { ok = false; break; }

                // s 的组比 word 短 → 不可能
                if (slen < wlen) { ok = false; break; }

                // 扩展了但不满足 >=3 → 非法
                if (slen != wlen && slen < 3) { ok = false; break; }
            }

            if (ok) count++;
        }

        return count;
    }
};

// ============================================================
// 【解法对比】
// | 解法 | 优点 | 缺点 |
// |------|------|------|
// | 双指针(Solution1) | O(1)空间，代码紧凑 | 逻辑稍复杂 |
// | 预处理分组(Solution2) | 逻辑清晰，易调试 | O(L)额外空间 |
//
// 两种本质相同，面试中用哪种都可以。
//
// 【易错点】
// 1. 忘记 >=3 约束:
//    ✗ lenS >= lenW 就认为合法
//    ✓ lenS > lenW 时还需 lenS >= 3
//
// 2. 精确匹配时误要求 >=3:
//    ✗ 所有情况都检查 lenS >= 3
//    ✓ lenS == lenW 时不需要 >=3 (没有扩展)
//
// 3. 忘记检查两个指针/数组都到末尾:
//    ✗ 循环结束就返回 true → s="ab" w="a" 会误判
//    ✓ 必须 i==n && j==m
//
// 4. 内层 while 忘记边界:
//    ✗ while (s[i] == ch) → 越界
//    ✓ while (i < n && s[i] == ch)
//
// 【面试追问】
// Q1: 扩展规则改为 >=2 怎么办？(把 lenS<3 改为 lenS<2)
// Q2: 如果允许收缩，判断条件怎么变？
//     (两个组长 >=3 时可以自由匹配)
// Q3: 能否预处理 s 的分组后复用，避免重复计算？
//     (Solution2 已经这样做了，s 只分组一次)
// ============================================================
