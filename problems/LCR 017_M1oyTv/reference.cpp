/*
 * LCR 017: 最小覆盖子串 (同主站 LeetCode 76)
 * Minimum Window Substring
 *
 * 【题目本质】
 * 在字符串 s 中找最短子串，使其包含 t 中所有字符（含重复）。
 * "最短" + "包含目标" → 经典滑动窗口: 右扩张找合法，左收缩找最短。
 *
 * 【解法总览】
 * 解法1: 滑动窗口 + 数组    — O(|s|+|t|) / O(128) — 面试首选 ⭐
 * 解法2: 滑动窗口 + HashMap — O(|s|+|t|) / O(字符集) — 通用写法
 * 解法3: 滑动窗口 + 单计数器 — O(|s|+|t|) / O(128) — 简洁变体
 */


// ============================================================
// 解法1: 滑动窗口 + 数组 — 面试首选 ⭐
// 时间: O(|s| + |t|)  空间: O(128) = O(1)
//
// 【思路】
// 维护窗口 [left, right]:
// 1. right 扩张 → 加入 s[right], 更新 window 计数
// 2. 检查合法  → formed == required 时窗口覆盖了 t 所有字符
// 3. left 收缩 → 尝试缩短窗口, 同时更新答案
//
// 【关键变量】
// need[c]:     t 中字符 c 需要的数量
// window[c]:   当前窗口中字符 c 的数量
// required:    t 中有多少种不同字符
// formed:      当前窗口中有多少种字符已满足需求
// 当 formed == required → 窗口合法 (覆盖了 t 的所有字符)
//
// 【formed 的增减时机 — 最易出错的地方】
// 增加: window[c]++ 后, 如果 window[c] == need[c] → formed++
//       (恰好满足, 不是 >=, 因为超过需求不用再加)
// 减少: window[c]-- 前, 如果 window[c] == need[c] → formed--
//       (即将不满足, 在减之前判断)
//
// 窗口滑动过程 (s = "ADOBECODEBANC", t = "ABC"):
//
//   need: A=1, B=1, C=1 (required=3)
//
//   ┌─────────────────────┐
//   │  A  D  O  B  E  C   │  O  D  E  B  A  N  C
//   └─────────────────────┘
//   left=0, right=5, formed=3/3 → 合法! len=6, 记录
//   收缩: 移除'A' → formed=2 → 不合法, 停止收缩
//
//   ... 右扩张到 right=10 ('A') ...
//
//   ┌────────────────────────────────────┐
//   │  D  O  B  E  C  O  D  E  B  A     │  N  C
//   └────────────────────────────────────┘
//   left=1, right=10, formed=3/3 → 合法! len=10
//   收缩: 移除'D','O','B' → left=4, 仍合法! len=7
//   收缩: 移除'E' → left=5, 仍合法! len=6
//   收缩: 移除'C' → formed=2 → 不合法, 停止
//
//   ... 右扩张到 right=12 ('C') ...
//
//                               ┌──────────────┐
//   A  D  O  B  E  C  O  D  E  │  B  A  N  C  │
//                               └──────────────┘
//   left=9, right=12, formed=3/3 → 合法! len=4 ← 最小!
//   收缩: 移除'B' → formed=2 → 不合法
//
//   答案: s.substr(9, 4) = "BANC"
// ============================================================
class Solution1 {
public:
    string minWindow(string s, string t) {
        // Step 1: 统计 t 中每个字符的需求量
        vector<int> need(128, 0);   // need[c] = t 中字符 c 的数量
        vector<int> window(128, 0); // window[c] = 窗口中字符 c 的数量
        int required = 0;           // t 中不同字符的种类数

        for (char c : t) {
            if (need[c] == 0) required++; // 新出现的字符种类
            need[c]++;
        }

        // Step 2: 滑动窗口
        int formed = 0;  // 已满足需求的字符种类数
        int left = 0;
        int minLen = INT_MAX, minStart = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            // --- 扩张: 加入 s[right] ---
            char c = s[right];
            window[c]++;

            // 如果 c 是 t 中需要的, 且窗口中 c 的数量恰好达到需求
            if (need[c] > 0 && window[c] == need[c]) {
                formed++;
            }

            // --- 收缩: 当窗口合法时, 尝试缩短 ---
            while (formed == required) {
                // 更新答案
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minStart = left;
                }

                // 移除 s[left], 准备收缩
                char d = s[left];
                // 先判断: 移除后是否会导致某字符不满足
                if (need[d] > 0 && window[d] == need[d]) {
                    formed--; // 即将不满足
                }
                window[d]--;
                left++;
            }
        }

        // Step 3: 返回结果
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};

// ============================================================
// 解法2: 滑动窗口 + HashMap — 通用写法
// 时间: O(|s| + |t|)  空间: O(|字符集|)
//
// 【思路】
// 与解法1 逻辑完全相同, 用 unordered_map 替代数组。
// 优点: 支持任意字符集 (Unicode 等)。
// 缺点: 哈希表比数组慢 (哈希计算 + 可能的冲突)。
//
// 面试中推荐先写这个版本 (更直观), 再提到可以用数组优化。
// ============================================================
class Solution2 {
public:
    string minWindow(string s, string t) {
        unordered_map<char, int> need, window;
        for (char c : t) need[c]++;
        int required = need.size(); // 不同字符种类数 (.size() 自动去重)

        int formed = 0, left = 0;
        int minLen = INT_MAX, minStart = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            char c = s[right];
            window[c]++;
            if (need.count(c) && window[c] == need[c]) {
                formed++;
            }

            while (formed == required) {
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minStart = left;
                }
                char d = s[left++];
                if (need.count(d) && window[d] == need[d]) {
                    formed--;
                }
                window[d]--;
            }
        }
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};

// ============================================================
// 解法3: 滑动窗口 + 单计数器 — 简洁变体
// 时间: O(|s| + |t|)  空间: O(128)
//
// 【思路】
// 只用一个数组 count[128], 初始化为 t 中每个字符的频次。
// 用一个整数 total 表示"还差多少个字符才能覆盖 t"。
//
// 扩张时: count[s[right]]--
//   如果减之前 count > 0, 说明消耗了一个有效字符 → total--
// 收缩时: count[s[left]]++
//   如果加之后 count > 0, 说明释放了一个有效字符 → total++
//
// total == 0 时窗口合法。
//
// 这种写法代码更短, 但需要仔细理解 count 的语义:
//   count[c] > 0: 窗口还需要 count[c] 个字符 c
//   count[c] == 0: 字符 c 恰好满足
//   count[c] < 0: 窗口中字符 c 多余了 |count[c]| 个
// ============================================================
class Solution3 {
public:
    string minWindow(string s, string t) {
        int count[128] = {};
        for (char c : t) count[c]++;
        int total = t.size(); // 还需要的字符总数

        int left = 0;
        int minLen = INT_MAX, minStart = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            // 加入 s[right]: 如果 count > 0, 说明是有效消耗
            if (count[s[right]] > 0) total--;
            count[s[right]]--;

            // 当 total == 0, 窗口覆盖了 t 的所有字符
            while (total == 0) {
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minStart = left;
                }
                // 移除 s[left]
                count[s[left]]++;
                // 如果加回后 count > 0, 说明释放了一个有效字符
                if (count[s[left]] > 0) total++;
                left++;
            }
        }
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 数据结构    | 代码量 | 可读性 | 时间       | 空间 |
// |---------------|-----------|--------|--------|-----------|------|
// | 数组+formed   | int[128]  | 中     | 高     | O(|s|+|t|)| O(1) |
// | HashMap+formed| unordered | 中     | 高     | O(|s|+|t|)| O(k) |
// | 单计数器      | int[128]  | 短     | 中     | O(|s|+|t|)| O(1) |
//
// 面试推荐: 解法1(数组) 或 解法2(HashMap), formed 变量清晰表达语义。
// 解法3 更简洁但面试时不易解释清楚。
//
// ============================================================
// 【易错点】
//
// 1. formed 增减时机错误:
//    ✗ 每次 window[c]++ 都 formed++ (formed 会超过 required)
//    ✓ 只在 window[c] 恰好等于 need[c] 时 formed++
//    ✗ 每次 window[c]-- 都 formed-- (formed 会变负)
//    ✓ 只在 window[c] 恰好等于 need[c] (减之前) 时 formed--
//
// 2. 减少判断在 window-- 之后:
//    ✗ window[d]--; if (window[d] < need[d]) formed--;
//       这样如果 window 从 3 减到 2, need=1, 也会 formed--
//    ✓ if (window[d] == need[d]) formed--; window[d]--;
//       只在"恰好不满足"的临界点减
//
// 3. 忘记处理找不到的情况:
//    ✗ return s.substr(minStart, minLen); // minLen=INT_MAX 时崩溃
//    ✓ return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
//
// 4. t 中有重复字符:
//    t = "AAB" → need[A]=2, need[B]=1, required=2(两种字符)
//    窗口中必须有至少 2 个 A 和 1 个 B
//    如果只用 set 去重 t, 会丢失 A 的重复需求
//
// 5. 窗口长度差1:
//    ✗ right - left
//    ✓ right - left + 1
//
// ============================================================
// 【面试追问】(递进链)
//
// Q1: "formed 变量有什么好处? 不用行不行?"
//     不用 formed, 每次需要遍历 need 表检查 window[c] >= need[c],
//     这是 O(字符集) 的。formed 是 O(1) 增量维护, 极大加速。
//
// Q2: "为什么 left 只往右走? 会不会漏掉最优解?"
//     单调性保证: 窗口越大越容易合法, 越小越难合法。
//     如果 left 回退, 窗口变大, 不可能得到更短的覆盖子串。
//     所以 left 只需向右移动, 不会漏解。
//
// Q3: "解法3 的单计数器版本, count 的语义如何理解?"
//     count[c] = "还需要多少个 c 才能满足需求"
//     正数: 还差; 零: 恰好; 负数: 多余。
//     total 是所有字符的总欠缺量, total=0 表示全部满足。
//
// Q4: "如果要返回所有最短覆盖子串呢?"
//     记录 minLen, 每次找到合法窗口时:
//     如果 len < minLen: 清空结果列表, 更新 minLen, 加入新答案
//     如果 len == minLen: 加入结果列表
//
// ============================================================
