/*
 * LeetCode 187: 重复的DNA序列 (Repeated DNA Sequences)
 *
 * 【题目本质】
 * 在一个只含 A, C, G, T 的字符串中，找出所有出现超过一次的长度为 10 的子串。
 * 核心问题：如何高效地比较和记录所有长度为 10 的子串？
 *
 * 【解法总览】
 * 解法1: 哈希表+子串       — O(10n) / O(10n) — 直观简洁，面试首选
 * 解法2: 滑动窗口+位编码   — O(n) / O(n)     — 进阶优化，展示位运算功底
 * 解法3: Rabin-Karp 滚动哈希 — O(n) / O(n)   — 通用框架，适合扩展到变长窗口
 */


// ============================================================
// 解法1: 哈希表 + 子串 — 最直观
// 时间: O(10n)  空间: O(10n)
//
// 【思路】
// 枚举所有长度为 10 的子串，用哈希表统计出现次数。
// 出现次数 == 2 时加入结果（保证不重复添加）。
//
// 示例: s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"
//   s[0..9]   = "AAAAACCCCC" → count 1
//   s[1..10]  = "AAAACCCCCA" → count 1
//   ...
//   s[10..19] = "AAAAACCCCC" → count 2 → 加入结果!
//   ...
//   s[16..25] = "CCCCCAAAAA" → count 2 → 加入结果!
//   结果: ["AAAAACCCCC", "CCCCCAAAAA"]
//
// 为什么 count == 2 而不是 > 1？
//   如果用 > 1，子串出现3次时会被添加2次（count=2和count=3时各一次）。
//   == 2 保证只在"第二次出现的那一刻"添加，恰好一次。
// ============================================================
class Solution1 {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};

        unordered_map<string, int> seen;
        vector<string> result;

        for (int i = 0; i + 10 <= (int)s.size(); i++) {
            string sub = s.substr(i, 10);
            seen[sub]++;
            // 恰好第二次出现时加入结果，避免重复
            if (seen[sub] == 2) {
                result.push_back(sub);
            }
        }
        return result;
    }
};

// ============================================================
// 解法1 变体: 用两个 set 代替 map
// 时间: O(10n)  空间: O(10n)
//
// seen: 记录见过的子串
// repeated: 记录已确认重复的子串（自动去重）
//
// 好处：不需要计数，逻辑更清晰
// ============================================================
class Solution1b {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};

        unordered_set<string> seen, repeated;

        for (int i = 0; i + 10 <= (int)s.size(); i++) {
            string sub = s.substr(i, 10);
            // insert 返回 pair<iterator, bool>
            // second == false 说明已经存在
            if (!seen.insert(sub).second) {
                repeated.insert(sub);  // set 自动去重
            }
        }
        return vector<string>(repeated.begin(), repeated.end());
    }
};

// ============================================================
// 解法2: 滑动窗口 + 位编码 — 面试加分 ⭐
// 时间: O(n)  空间: O(n)
//
// 【核心优化思路】
// DNA 只有 4 种碱基，每种可以用 2 bit 编码：
//   A = 00 (0)
//   C = 01 (1)
//   G = 10 (2)
//   T = 11 (3)
//
// 10 个碱基 = 20 bit，一个 int (32bit) 就够了。
// 这样哈希 key 从 string 变成 int，比较和哈希都是 O(1)。
//
// 滑动窗口更新：
//   hash = ((hash << 2) | encode(新字符)) & mask
//   mask = (1 << 20) - 1  (保留低 20 位)
//
// 详细图解：
//   旧窗口: [A C G T A C G T A C] G T ...  → hash 代表这10个字符
//   新窗口:  A [C G T A C G T A C G] T ...
//
//   hash << 2:     腾出右边 2 位给新字符
//   | encode('G'): 新字符编码填入右边 2 位
//   & mask:        砍掉超过 20 位的高位（旧窗口最左边字符自动丢弃）
//
// 为什么先处理前9个字符？
//   前9个字符还不够一个完整的10字符窗口，先建立初始哈希值。
//   从第10个字符（i=9）开始，每次滑动才是一个完整的窗口。
// ============================================================
class Solution2 {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};

        // 字符 → 2bit 编码
        // 用数组比 unordered_map 快（直接索引 vs 哈希查找）
        int encode[128] = {};
        encode['A'] = 0;  // 00
        encode['C'] = 1;  // 01
        encode['G'] = 2;  // 10
        encode['T'] = 3;  // 11

        // mask: 低 20 位全 1，用来保留窗口内的 10 个字符
        // 10 字符 * 2 bit = 20 bit
        int mask = (1 << 20) - 1;  // 0xFFFFF = 0b 0000 1111 1111 1111 1111 1111

        int hash = 0;
        unordered_map<int, int> seen;
        vector<string> result;

        // 构建前 9 个字符的初始哈希（还不够一个完整窗口）
        for (int i = 0; i < 9; i++) {
            hash = (hash << 2) | encode[(int)s[i]];
        }

        // 从第 10 个字符开始，每次加入一个字符形成完整窗口
        for (int i = 9; i < (int)s.size(); i++) {
            // 新字符进入窗口：左移腾位 + 新编码 + mask截断
            hash = ((hash << 2) | encode[(int)s[i]]) & mask;

            seen[hash]++;
            if (seen[hash] == 2) {
                // i 是窗口右端，i-9 是窗口左端，长度 10
                result.push_back(s.substr(i - 9, 10));
            }
        }
        return result;
    }
};

// ============================================================
// 解法3: Rabin-Karp 风格滚动哈希
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 把 DNA 看成 4 进制数（base = 4）。
// 窗口滑动时:
//   hash = (hash - encode[离开字符] * power) * base + encode[进入字符]
//   其中 power = base^(窗口长度-1) = 4^9
//
// 与解法2的关系：
//   解法2 用位运算（<< 2 就是 * 4，& mask 就是 % 2^20）
//   解法3 用算术运算（* 4, - 最高位）
//   本质上是等价的，只是表达方式不同。
//
// 解法3的优势：
//   更容易推广到大字符集（如 26 个字母），此时 2bit 编码不够用，
//   但 Rabin-Karp 只需改变 base 值即可。
//   这正是 LeetCode 1044（最长重复子串）使用的框架。
//
// 为什么这里不需要取模？
//   窗口长度固定为 10，base = 4，最大 hash = 4^10 - 1 = 1048575
//   完全在 int 范围内（2^31 - 1 > 2 * 10^9），不会溢出。
//   但如果窗口很长或 base 很大，就需要取模，此时要处理哈希冲突。
// ============================================================
class Solution3 {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};

        unordered_map<char, int> encode = {{'A',0}, {'C',1}, {'G',2}, {'T',3}};
        int base = 4;

        // power = base^(窗口长度-1) = 4^9
        // 用于去掉滑出窗口的最高位字符
        long long power = 1;
        for (int i = 0; i < 9; i++) power *= base;
        // power = 4^9 = 262144

        long long hash = 0;
        unordered_map<long long, int> seen;
        vector<string> result;

        // 计算第一个窗口 s[0..9] 的哈希值
        for (int i = 0; i < 10; i++) {
            hash = hash * base + encode[s[i]];
        }
        seen[hash] = 1;

        // 从 s[1..10] 开始滑动
        for (int i = 10; i < (int)s.size(); i++) {
            // 去掉最高位（滑出窗口的字符 s[i-10]）
            // 乘以 base（所有剩余字符左移一位）
            // 加上新字符（进入窗口的字符 s[i]）
            hash = (hash - encode[s[i - 10]] * power) * base + encode[s[i]];

            seen[hash]++;
            if (seen[hash] == 2) {
                result.push_back(s.substr(i - 9, 10));
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间       | 空间    | Key类型  | 常数因子 |
// |-----------------|-----------|---------|---------|---------|
// | 哈希表+子串     | O(10n)    | O(10n)  | string  | 较大     |
// | 位编码          | O(n)      | O(n)    | int     | 最小     |
// | Rabin-Karp      | O(n)      | O(n)    | int     | 中等     |
//
// 面试策略：先写解法1（简洁正确），若面试官追问优化，讲解法2。
//
// ============================================================
// 【易错点】
//
// 1. 去重逻辑:
//    ✗ if (seen[sub] > 1) result.push_back(sub);
//      → 出现3次的子串会被加入2次
//    ✓ if (seen[sub] == 2) result.push_back(sub);
//      → 只在第2次出现时加入，恰好1次
//
// 2. 位编码 mask 错误:
//    ✗ mask = (1 << 10) - 1   → 只有10位，但需要20位
//    ✓ mask = (1 << 20) - 1   → 10字符 * 2bit = 20bit
//
// 3. 滑动窗口初始化:
//    ✗ 从 i=0 就开始统计完整窗口 → 前9个位置窗口不完整
//    ✓ 先处理前9个字符构建初始哈希，从 i=9 开始
//
// 4. Rabin-Karp 的 power 值:
//    ✗ power = base^10 → 这是11字符窗口的最高位权重
//    ✓ power = base^9  → 10字符窗口，最高位权重是 base^9
//
// 5. 边界检查:
//    s.size() <= 10 → 直接返回空（不可能有重复的10字符子串）
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 两种去重方式哪个更好？
//     map计数 vs 两个set。map更优雅（一个数据结构），两个set
//     更直观。面试中选你写得快的那个。
//
// Q2: 如果窗口长度是 k 而不是 10？
//     k 较小时位编码可行（2k <= 64，用 long long）。
//     k 很大时必须用 Rabin-Karp + 取模（防溢出），
//     但要处理哈希冲突（false positive），这正是第 1044 题的场景。
//
// Q3: 位编码的映射为什么用 encode['A']=0 而不是其他值？
//     任意4个不同的2bit值都行。0123最自然。
//     一个巧妙映射：(s[i] >> 1) & 3 可以区分 ACGT（利用 ASCII 码特性）。
//
// Q4: 如何不用 substr 就得到结果字符串？
//     可以在位编码方案中，从 int hash 反向解码出字符串。
//     每次取最高2位，映射回 ACGT。但面试中直接 substr 更清晰。
//
// ============================================================
// 【相关题型】
//
// - 28. 找出字符串中第一个匹配项的下标:
//   Rabin-Karp 原始应用 — 单模式串匹配，滚动哈希 O(n+m)。
//
// - 1044. 最长重复子串:
//   本题的"进阶"版本。窗口长度不固定，需二分+Rabin-Karp。
//   核心的"判断长度k的重复子串是否存在"与本题逻辑一致。
//
// - 438. 找到字符串中所有字母异位词:
//   同样是固定窗口滑动+哈希，但匹配条件是字符频率相同而非完全匹配。
//
// - 49. 字母异位词分组:
//   字符串编码+哈希分组。编码方式不同（排序或计数数组），但框架类似。
// ============================================================
