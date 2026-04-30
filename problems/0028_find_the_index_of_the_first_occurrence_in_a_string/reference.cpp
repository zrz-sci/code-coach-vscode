/*
 * LeetCode 28: 找出字符串中第一个匹配项的下标
 *              (Find the Index of the First Occurrence in a String)
 *
 * 【题目本质】
 * 经典字符串匹配问题：在主串 haystack 中找模式串 needle 第一次出现的起始下标。
 *
 * 【解法总览】
 * 解法1: 暴力匹配     — O(n·m) / O(1)   — 最直觉，逐位置尝试
 * 解法2: KMP 算法      — O(n+m) / O(m)   — 经典最优，面试必会
 * 解法3: Rabin-Karp    — O(n+m)期望 / O(1) — 滚动哈希，多模式匹配时更有优势
 */

// ============================================================
// 解法1: 暴力匹配 — 逐位置尝试完整比对
// 时间: O(n·m)  空间: O(1)
//
// 【思路】
// 最直觉的方法：从 haystack 的每个位置 i 开始，逐字符与 needle 比较。
// 如果所有 m 个字符都匹配，返回 i。
//
// 瓶颈：当 haystack 和 needle 有大量重复前缀时（如 "aaaa...a" 匹配 "aaa...ab"），
// 每次失败后 i 只前进 1，之前比较过的信息全部浪费，最坏 O(n·m)。
//
// 匹配过程示例:
// haystack: s a d b u t s a d
// needle:   s a d
//
// i=0: s==s, a==a, d==d → 匹配成功, 返回 0
// ============================================================
class Solution1 {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        // 外层边界：i 最多到 n-m，再往后剩余长度不够 needle
        // 注意 n-m 可能为负，但 n,m >= 1 且用 int 不会溢出为正
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            // 从 haystack[i] 开始，逐字符比较 needle
            while (j < m && haystack[i + j] == needle[j]) {
                j++;
            }
            // j == m 说明 needle 的每个字符都匹配了
            if (j == m) return i;
        }
        return -1;
    }
};

// ============================================================
// 解法2: KMP 算法 — 预处理 next 数组，匹配失败时智能跳转
// 时间: O(n+m)  空间: O(m)
//
// 【思路】
// 暴力法的瓶颈：匹配失败时，haystack 的指针 i 要回退，导致重复比较。
//
// 核心洞察：匹配失败时，haystack[i-j..i-1] 已经和 needle[0..j-1] 相等。
// 如果 needle[0..j-1] 内部有"最长相等前后缀"(长度为 k)，那么
// needle 的前 k 个字符 = haystack 中已匹配部分的后 k 个字符，
// 可以直接让 j 跳到 k 继续比较，i 不回退。
//
// next 数组: next[j] = needle[0..j] 的最长相等前后缀长度
//
// next 数组构建示例 (needle = "aabaaab"):
//
//   下标:  0  1  2  3  4  5  6
//   字符:  a  a  b  a  a  a  b
//   next: [0, 1, 0, 1, 2, 2, 3]
//
//   - next[0] = 0  "a" 没有真前后缀
//   - next[1] = 1  "aa" → 前缀"a" = 后缀"a", 长度1
//   - next[2] = 0  "aab" → 无相等前后缀
//   - next[3] = 1  "aaba" → "a" = "a", 长度1
//   - next[4] = 2  "aabaa" → "aa" = "aa", 长度2
//   - next[5] = 2  "aabaaa" → "aa" = "aa", 长度2 (不是3,因为"aab"≠"aaa")
//   - next[6] = 3  "aabaaab" → "aab" = "aab", 长度3
//
// KMP 匹配过程示例:
// haystack: a a b a a a b x ...
// needle:   a a b a a a b
//           ↑ ↑ ↑ ↑ ↑ ↑ ↑   全部匹配, j==7==m, 返回 i-m+1 = 7-7 = 0
//
// 匹配失败时的跳转示例:
// haystack: a a b a a a a b a a b ...
//                         ↑ i=6, haystack[6]='a'
// needle:   a a b a a a b
//                       ↑ j=6, needle[6]='b', 不匹配!
//           j = next[j-1] = next[5] = 2
// needle:           a a b a a a b
//                   ↑ j=2, 继续比较 haystack[6] 和 needle[2]
// ============================================================
class Solution2 {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m == 0) return 0;
        
        // --- 第一步：构建 next 数组 ---
        // next[i] 表示 needle[0..i] 的最长相等前后缀长度
        // j 同时充当两个角色：
        //   1. 前缀指针（指向前缀末尾的下一个位置）
        //   2. 当前最长前后缀长度
        vector<int> next(m, 0);
        for (int i = 1, j = 0; i < m; i++) {
            // 关键：不匹配时要循环回退，不是只回退一次
            // 因为回退一次后可能还是不匹配，需要继续回退
            while (j > 0 && needle[i] != needle[j]) {
                j = next[j - 1];
            }
            // 匹配成功，前缀长度增加
            if (needle[i] == needle[j]) {
                j++;
            }
            next[i] = j;
        }
        
        // --- 第二步：在 haystack 中用 KMP 匹配 ---
        // i 扫描 haystack (永不回退)
        // j 扫描 needle (失败时通过 next 跳转)
        for (int i = 0, j = 0; i < n; i++) {
            // 与构建 next 的逻辑完全相同！
            while (j > 0 && haystack[i] != needle[j]) {
                j = next[j - 1];
            }
            if (haystack[i] == needle[j]) {
                j++;
            }
            // needle 完全匹配
            if (j == m) {
                // 起始下标 = 当前位置 - needle长度 + 1
                return i - m + 1;
            }
        }
        return -1;
    }
};

// ============================================================
// 解法3: Rabin-Karp (滚动哈希) — 哈希比较代替逐字符比较
// 时间: O(n+m) 期望  空间: O(1)
//
// 【思路】
// 另一个优化角度：暴力法每次花 O(m) 逐字符比较子串。
// 能不能 O(1) 判断子串是否等于 needle？用哈希！
//
// 把字符串看成 base 进制数，用滑动窗口在 O(1) 内更新哈希值：
//   新哈希 = (旧哈希 - 最高位字符 * base^(m-1)) * base + 新字符
//
// 滚动哈希过程 (haystack="abcde", needle="bcd", base=26):
//   窗口 "abc": hash = a*26^2 + b*26 + c
//   窗口 "bcd": hash = (上面 - a*26^2) * 26 + d
//   窗口 "cde": hash = (上面 - b*26^2) * 26 + e
//
// 注意：哈希碰撞时需要真实比对确认！
// ============================================================
class Solution3 {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m > n) return -1;
        
        long long base = 26, mod = 1e9 + 7;
        
        // 预计算 base^(m-1) % mod，用于移除窗口最左字符
        long long power = 1;
        for (int i = 0; i < m - 1; i++) {
            power = power * base % mod;
        }
        
        // 计算 needle 哈希和 haystack 第一个窗口的哈希
        long long needleHash = 0, windowHash = 0;
        for (int i = 0; i < m; i++) {
            needleHash = (needleHash * base + needle[i]) % mod;
            windowHash = (windowHash * base + haystack[i]) % mod;
        }
        
        for (int i = 0; i <= n - m; i++) {
            // 哈希相等时还要真实比对，防止碰撞导致误判
            if (windowHash == needleHash) {
                if (haystack.substr(i, m) == needle) {
                    return i;
                }
            }
            // 滑动窗口更新哈希值
            if (i < n - m) {
                // 减去最高位，乘 base，加上新字符
                // +mod 防止减法产生负数
                windowHash = ((windowHash - haystack[i] * power % mod + mod) % mod
                              * base + haystack[i + m]) % mod;
            }
        }
        return -1;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 暴力匹配   | KMP        | Rabin-Karp      |
// |----------|-----------|------------|-----------------|
// | 时间     | O(n·m)    | O(n+m)     | O(n+m) 期望     |
// | 空间     | O(1)      | O(m)       | O(1)            |
// | 最坏保证 | 差        | 确定O(n+m) | 碰撞退化O(n·m)  |
// | 代码量   | 5行       | 20行       | 20行            |
// | 面试推荐 | 说思路    | ⭐首选     | 了解思路即可     |
//
// 选择建议:
// - 面试: 暴力法说完思路 → 直接写 KMP
// - 多模式匹配场景: Rabin-Karp 或 Aho-Corasick
// - 实际工程: 大部分语言标准库已有 find/indexOf
//
// 【易错点】
//
// 1. 暴力法边界 unsigned 溢出:
//    ✗ size_t 类型: n-m 当 n < m 时溢出为巨大正数，循环条件永远为真
//    ✓ 用 int 类型，或先 if (m > n) return -1
//
// 2. KMP next 数组回退只用 if 而非 while:
//    ✗ if (j > 0 && needle[i] != needle[j]) j = next[j-1];
//      对 needle = "aaab" 在 i=3 时，j=2 回退到 next[1]=1，但 needle[3]='b' ≠ needle[1]='a'
//      还需要继续回退到 next[0]=0
//    ✓ while (j > 0 && needle[i] != needle[j]) j = next[j-1];
//
// 3. KMP 返回的下标计算错:
//    ✗ return i;  — 这是匹配结束位置
//    ✓ return i - m + 1;  — 匹配起始位置
//
// 4. Rabin-Karp 减法产生负数:
//    ✗ (windowHash - haystack[i] * power) % mod  — 可能为负
//    ✓ (windowHash - haystack[i] * power % mod + mod) % mod  — 加 mod 再取模
//
// 【面试追问 — 递进链】
//
// Q1: "暴力法什么时候会达到 O(n·m) 最坏？"
// → haystack 全是 'a'，needle 是 m-1 个 'a' + 'b'。每个位置都匹配到
//   最后一个字符才失败。
//
// Q2: "KMP 的 next 数组为什么能保证 i 不回退？"
// → 因为已匹配的 haystack[i-j..i-1] = needle[0..j-1]。
//   next[j-1] 给出 needle[0..j-1] 的最长相等前后缀长度 k，
//   所以 haystack 中这段末尾 k 个字符 = needle 前 k 个字符，
//   让 j 跳到 k 就等于 needle 右移了 j-k 位，i 不需要回退。
//
// Q3: "如果要找所有出现位置而不是第一个？"
// → 在 j == m 时不 return，记录 i-m+1 到结果数组，
//   然后 j = next[j-1] 继续匹配。时间仍 O(n+m)。
//
// Q4: "如果有多个 needle 需要同时在 haystack 中查找？"
// → 用 Aho-Corasick 算法：在 Trie 上建失败指针（类似 KMP 的 next），
//   一次遍历 haystack 即可匹配所有模式。
// ============================================================
