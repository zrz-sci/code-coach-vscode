/*
 * 面试题 01.06: 字符串压缩 (Compress String LCCI)
 *
 * 【题目本质】
 * 遍历字符串统计连续相同字符的个数，构造 "字符+出现次数" 的压缩串。
 * 若压缩后更短则返回压缩串，否则返回原串。
 * 核心考察字符串遍历的边界处理能力。
 *
 * 【解法总览】
 * 解法1: 双指针一次遍历    — O(n) / O(n)   — i指段首，j找段尾 ⭐面试首选
 * 解法2: 单指针+计数器     — O(n) / O(n)   — 逐字符遍历，遇到不同字符时输出
 * 解法3: 预计算长度优化    — O(n) / O(n)   — 先算长度再决定是否构造
 */


// ============================================================
// 解法1: 双指针一次遍历 — i指向段首，j找段尾
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 用两个指针 i 和 j，i 始终指向当前连续段的起点，
// j 向前移动到当前段的终点（第一个和 S[i] 不同的位置）。
// 段长为 j-i，直接拼接 S[i] + to_string(j-i)。
//
// 为什么用双指针？
// 1. 段长 = j-i，不需要额外计数器
// 2. while(i < n) 天然包含最后一段，不会遗漏
// 3. 代码结构对称清晰，不容易出 bug
//
// 示例: S = "aabcccccaaa"
//   i=0, j→2:  段 "aa"    → "a2"
//   i=2, j→3:  段 "b"     → "b1"
//   i=3, j→8:  段 "ccccc" → "c5"
//   i=8, j→11: 段 "aaa"   → "a3"
//   结果: "a2b1c5a3" (长度8 < 11) → 返回压缩串
// ============================================================
class Solution1 {
public:
    string compressString(string S) {
        int n = S.size();
        if (n == 0) return S;  // 边界: 空串直接返回

        string compressed;
        int i = 0;
        while (i < n) {
            int j = i;
            // j 向前跑，直到遇到不同字符或到达末尾
            while (j < n && S[j] == S[i]) {
                j++;
            }
            // S[i..j-1] 是一段连续相同字符
            // 字符: S[i], 长度: j-i
            compressed += S[i];
            compressed += to_string(j - i);  // 注意: 用 to_string，不要用 '0'+count
            i = j;  // 跳到下一段的起点
        }

        // 关键: 用 < 而不是 <=
        // 题目说"压缩后没有变短则返回原串"，等长时也返回原串
        return compressed.size() < S.size() ? compressed : S;
    }
};

// ============================================================
// 解法2: 单指针 + 计数器 — 逐字符遍历
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 维护计数器 count，遍历时比较当前字符和前一个字符。
// 发现不同时输出前一段。
//
// 技巧: 循环用 i <= n（注意 <= 不是 <），
// 当 i == n 时自然触发"输出最后一段"的逻辑，
// 避免循环外的重复代码。
//
// 为什么这种写法容易出 bug？
// 如果循环条件是 i < n，循环结束后最后一段没有输出，
// 需要在循环外补上，容易遗漏。
// 用 i <= n 的技巧虽然消除了遗漏风险，但需要理解:
//   当 i == n 时，S[i] 不会被访问（条件 i == n 先判断为 true）
// ============================================================
class Solution2 {
public:
    string compressString(string S) {
        int n = S.size();
        if (n == 0) return S;

        string compressed;
        int count = 1;

        for (int i = 1; i <= n; i++) {
            // i==n 时触发最后一段的输出
            // 注意: i==n 必须放在 S[i] != S[i-1] 之前（短路求值）
            if (i == n || S[i] != S[i - 1]) {
                compressed += S[i - 1];         // 输出字符
                compressed += to_string(count);  // 输出计数
                count = 1;                       // 重置计数器
            } else {
                count++;
            }
        }

        return compressed.size() < S.size() ? compressed : S;
    }
};

// ============================================================
// 解法3: 预计算长度 — 先算长度再决定是否构造
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 优化思路: 如果压缩后注定不会更短，就别浪费时间构造了。
// 第一遍: 遍历统计压缩后的总长度
// 第二遍: 如果确实更短，才构造压缩串
//
// 什么时候这个优化有价值？
// 当原串中字符多样性高（如 "abcdef"），压缩后必然更长。
// 预计算可以避免不必要的 string 拼接和内存分配。
//
// 计算长度的关键:
//   每一段 = 1(字符) + digits(count) 位
//   digits(count) = to_string(j-i).size()
//   例如: 段长 5 → "5" → 1位; 段长 100 → "100" → 3位
// ============================================================
class Solution3 {
public:
    string compressString(string S) {
        int n = S.size();
        if (n == 0) return S;

        // 第一遍: 计算压缩后的长度
        int compressedLen = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && S[j] == S[i]) j++;
            // 1个字符 + 数字位数
            int segLen = j - i;
            compressedLen += 1;  // 字符本身
            // 计算数字位数: 1→1位, 10→2位, 100→3位...
            if (segLen < 10) compressedLen += 1;
            else if (segLen < 100) compressedLen += 2;
            else if (segLen < 1000) compressedLen += 3;
            else if (segLen < 10000) compressedLen += 4;
            else compressedLen += 5;  // 最多50000
            i = j;
        }

        // 提前判断: 压缩后不会更短，直接返回原串
        if (compressedLen >= n) return S;

        // 第二遍: 构造压缩串
        string compressed;
        compressed.reserve(compressedLen);  // 预分配精确空间，避免扩容
        i = 0;
        while (i < n) {
            int j = i;
            while (j < n && S[j] == S[i]) j++;
            compressed += S[i];
            compressed += to_string(j - i);
            i = j;
        }

        return compressed;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间  | 空间  | 代码复杂度 | 面试推荐 |
// |-------------|-------|-------|-----------|---------|
// | 双指针       | O(n) | O(n) | 简单       | ⭐⭐⭐   |
// | 单指针+计数器 | O(n) | O(n) | 简单       | ⭐⭐     |
// | 预计算长度    | O(n) | O(n) | 中等       | 加分项   |
//
// 演进关系:
//   解法1 ↔ 解法2: 同一思路的不同写法（双指针 vs 计数器）
//   解法1 → 解法3: 增加预计算优化，避免不必要的字符串构造
//
// 面试策略:
//   1. 直接写解法1（双指针），代码最简洁清晰
//   2. 面试官问"能不能优化"？提预计算长度（解法3）
//   3. 面试官问"还有什么变体"？引到 LC 443 原地压缩
//
// ============================================================
// 【易错点】
//
// 1. 忘记处理最后一段（单指针写法）:
//    ✗ for(i=1; i<n; i++) { if(S[i]!=S[i-1]) output... }
//      循环结束后最后一段 "aaa" 没有输出
//    ✓ 用 i<=n 的技巧，或者循环后补一次 output
//
// 2. 长度比较用 <= 而不是 <:
//    ✗ return compressed.size() <= S.size() ? compressed : S;
//      等长时应该返回原串，不是压缩串
//    ✓ return compressed.size() < S.size() ? compressed : S;
//
// 3. 计数用 char 而不是 string:
//    ✗ compressed += (char)('0' + count);
//      当 count >= 10 时溢出，变成不可见字符
//    ✓ compressed += to_string(count);
//
// 4. 空串越界:
//    ✗ char curr = S[0];  // S 为空时 UB
//    ✓ if (S.empty()) return S; 放在最前面
//
// 5. int vs size_t 比较警告:
//    ✗ compressed.size() < S.size()  // 两个 size_t 比较没问题
//    但如果写成 compressed.length() < (int)S.size() 要注意符号
//
// ============================================================
// 【面试追问 Interview Follow-ups】
//
// Q1: 时间和空间复杂度？
// A1: 时间 O(n) 一次遍历，空间 O(n) 构造压缩串。
//     最坏情况压缩串长度 2n（每个字符不同时 "a1b1c1..."）。
//
// Q2: 如果字符串非常长（1GB），怎么处理？
// A2: 流式处理（streaming）。不需要加载整个字符串到内存。
//     一个字符一个字符读入，维护当前字符和计数器，
//     输出也是流式的。这是大数据场景的经典模式。
//
// Q3: 如果要支持解压怎么做？
// A3: 遍历压缩串，交替读字符和后面的数字（可能多位），
//     把字符重复相应次数。注意多位数字的解析。
//     参考 LC 394 Decode String。
//
// Q4: 字符串含数字时格式有歧义怎么办？
// A4: 引入分隔符（"a:1:b:2"）或固定宽度计数（"a001b002"），
//     或转义序列。考察编码设计能力。
//
// Q5: 和 LC 443 String Compression 的区别？
// A5: LC 443 要求原地压缩（O(1)空间），且计数为1时不写数字。
//     本题允许新建字符串，且计数为1也要写。
//     LC 443 更难，需要双指针+原地写入+读写指针分离。
//
// 相关题: 443 String Compression, 38 Count and Say,
//         394 Decode String, 271 Encode and Decode Strings
// ============================================================
