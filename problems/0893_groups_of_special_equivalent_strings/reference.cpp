/*
 * LeetCode 893: Groups of Special-Equivalent Strings (特殊等价字符串组)
 *
 * 【题目本质】
 * 分组归类问题：偶数位字符可自由交换，奇数位字符可自由交换。
 * 两个字符串等价 ⇔ 偶数位字符多重集相同 AND 奇数位字符多重集相同。
 * 为每个字符串构造规范键，用 set 统计不同键的数量。
 *
 * 【解法总览】
 * 解法1: 排序构造键 + Set     — O(N·L·logL) / O(N·L) — 面试首选
 * 解法2: 计数数组构造键       — O(N·L) / O(N·L)     — 追问时给
 */

// ============================================================
// 解法1: 排序构造键 + Set — 偶数位排序 + 奇数位排序作为规范键
// 时间: O(N * L * logL)  空间: O(N * L)
//
// 【思路】
// 对每个字符串:
//   1. 提取偶数下标 (0,2,4,...) 的字符 → even_chars
//   2. 提取奇数下标 (1,3,5,...) 的字符 → odd_chars
//   3. 分别排序
//   4. 规范键 = sorted(even_chars) + sorted(odd_chars)
//
// 所有字符串等长，所以偶数位和奇数位的字符数固定，
// 拼接不会产生歧义，不需要分隔符。
//
// 示例: words = ["abcd","cdab","cbad","xyzz","zzxy","zzyx"]
//
//   "abcd": even={a,c} → "ac", odd={b,d} → "bd" → key="acbd"
//   "cdab": even={c,a} → "ac", odd={d,b} → "bd" → key="acbd"  ← 同组
//   "cbad": even={c,a} → "ac", odd={b,d} → "bd" → key="acbd"  ← 同组
//   "xyzz": even={x,z} → "xz", odd={y,z} → "yz" → key="xzyz"
//   "zzxy": even={z,x} → "xz", odd={z,y} → "yz" → key="xzyz"  ← 同组
//   "zzyx": even={z,y} → "yz", odd={z,x} → "xz" → key="yzxz"
//
//   不同的键: {"acbd", "xzyz", "yzxz"} → 3 组 ✓
// ============================================================
class Solution1 {
public:
    int numSpecialEquivGroups(vector<string>& words) {
        set<string> groups;

        for (const string& w : words) {
            // 分别收集偶数位和奇数位的字符
            string even_chars, odd_chars;
            for (int i = 0; i < (int)w.size(); i++) {
                if (i % 2 == 0) {
                    even_chars += w[i];
                } else {
                    odd_chars += w[i];
                }
            }

            // 排序使得多重集相同的字符串产生相同的键
            sort(even_chars.begin(), even_chars.end());
            sort(odd_chars.begin(), odd_chars.end());

            // 规范键 = 偶数位排序结果 + 奇数位排序结果
            groups.insert(even_chars + odd_chars);
        }

        // set 中不同键的数量 = 组数
        return groups.size();
    }
};

// ============================================================
// 解法2: 计数数组构造键 — 用字符频率代替排序
// 时间: O(N * L)  空间: O(N * L)
//
// 【思路】
// 与解法1的区别：不排序，改用计数。
//
// 对每个字符串维护一个长度 52 的计数数组:
//   - count[0..25]:  偶数位上 a~z 各出现了多少次
//   - count[26..51]: 奇数位上 a~z 各出现了多少次
//
// 技巧: (i % 2) * 26 + (ch - 'a')
//   - i 是偶数 → offset = 0  → 存入 count[0..25]
//   - i 是奇数 → offset = 26 → 存入 count[26..51]
//
// 将 52 个计数编码成字符串作为键。
// 注意编码时必须用分隔符，否则 "1,12" 和 "11,2" 无法区分。
//
// 复杂度优势: 去掉了排序的 O(L logL) 因子。
// 但 L ≤ 20 时差异可以忽略。
// ============================================================
class Solution2 {
public:
    int numSpecialEquivGroups(vector<string>& words) {
        set<string> groups;

        for (const string& w : words) {
            // 52 维计数数组: 前 26 偶数位, 后 26 奇数位
            vector<int> count(52, 0);

            for (int i = 0; i < (int)w.size(); i++) {
                // 偶数下标: (i%2)*26 = 0  → count[0..25]
                // 奇数下标: (i%2)*26 = 26 → count[26..51]
                count[(i % 2) * 26 + (w[i] - 'a')]++;
            }

            // 将计数数组编码为字符串
            // 用逗号分隔防止歧义
            string key;
            for (int c : count) {
                key += to_string(c) + ",";
            }

            groups.insert(key);
        }

        return groups.size();
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间           | 空间     | 特点              |
// |---------------|---------------|---------|------------------|
// | 排序构造键     | O(N·L·logL)   | O(N·L)  | ⭐面试首选，简洁   |
// | 计数数组构造键 | O(N·L)        | O(N·L)  | 追问优化时给出     |
//
// 面试建议:
//   先写排序方法 → 追问优化 → 计数数组方法
//   关键是能清晰说出等价条件的本质:
//   "偶数位多重集相同 + 奇数位多重集相同"
//
// 【易错点】
// 1. 偶数/奇数位搞反:
//    ✗ 认为 index 1 是偶数位（从 1 开始计数）
//    ✓ 题目用 0-indexed，index 0,2,4... 是偶数位
//
// 2. 忘记排序:
//    ✗ 直接用偶数位字符串作键 → "ac" 和 "ca" 产生不同的键
//    ✓ 排序后 "ac" 和 "ac" 相同 → 正确归类
//
// 3. 规范键构造不当:
//    ✗ 只用偶数位排序结果作键，忽略奇数位
//    ✓ 必须同时包含偶数位和奇数位信息
//
// 4. 计数编码缺少分隔符:
//    ✗ "1120" 无法区分是 [1,1,2,0] 还是 [11,20] 还是 [1,12,0]
//    ✓ "1,1,2,0," 用逗号分隔，保证无歧义
//
// 5. 误用 unordered_set<vector<int>>:
//    ✗ 编译错误，标准库不提供 vector<int> 的 hash
//    ✓ 改用 set<vector<int>> 或编码成 string 再用 unordered_set<string>
//
// 【面试追问 Interview Follow-ups】
// Q1: 为什么偶数位字符可以达成任意排列？
// A1: 任意两个偶数位可以交换。通过有限次交换可以实现任意排列
//     （等价于冒泡排序——冒泡排序只用相邻交换就能排序，
//     本题甚至允许任意两个偶数位交换，更强）。
//
// Q2: 这题和 LC 49 字母异位词分组的关系？
// A2: LC 49 是"整个字符串排序后作为键"，本题是"偶数位和奇数位分别排序后作为键"。
//     方法论完全一样，只是键的构造规则不同。
//
// Q3: 如果题目改成"可以交换任意两个距离为 k 的倍数的位置"呢？
// A3: 那就是按 position % k 分成 k 组，每组内的字符可以自由排列。
//     规范键 = sorted(group_0) + sorted(group_1) + ... + sorted(group_{k-1})
//     本题是 k=2 的特例。
// ============================================================
