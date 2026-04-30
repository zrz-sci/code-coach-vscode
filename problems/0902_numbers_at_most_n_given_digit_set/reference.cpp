// ============================================================
// LeetCode 902: Numbers At Most N Given Digit Set
// (最大为 N 的数字组合)
// 难度: Hard | 标签: Array, Math, String, Binary Search, Dynamic Programming
//
// 题意: 给定数字集合 digits (1-9 的子集, 可重复使用),
//       问能组成多少个 <= n 的正整数。
// ============================================================


// ============================================================
// 解法1: 数位分析（数学法） ⭐ 推荐
//
// 思路:
//   分两部分:
//   Part 1: 位数比 n 短的数, 全部合法
//           d + d^2 + ... + d^(len-1)
//   Part 2: 位数和 n 相同的数, 逐位分析上界约束
//           对每一位: 小于上界的数字 × 后续位的自由度
//                     等于上界的数字 → 继续约束下一位
//
// 执行过程 (digits=[1,3,5,7], n=100):
//   n = "100", len=3, d=4
//
//   Part 1: 短位数
//     1位: d^1 = 4  (1,3,5,7)
//     2位: d^2 = 16 (11,13,...,77)
//     小计: 20
//
//   Part 2: 等长 (3位), 逐位比较 "100"
//     位0: S[0]='1'
//       '1'<'1'? No. '3'<'1'? No. '5'<'1'? No. '7'<'1'? No.
//       '1'=='1'? Yes → hasSame=true, 继续
//     位1: S[1]='0'
//       所有digit都>'0' → 无贡献, hasSame=false → BREAK
//     Part 2 贡献: 0
//
//   总计: 20
//
// 时间: O(log n * d) | 空间: O(log n)
// ============================================================
class Solution {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string S = to_string(n);
        int len = S.size();
        int d = digits.size();
        int ans = 0;

        // ---- Part 1: 位数比 n 短的数 ----
        // k 位数: 每位 d 种选择, 共 d^k 个
        // 累加 k = 1, 2, ..., len-1
        for (int i = 1; i < len; i++) {
            int power = 1;
            for (int j = 0; j < i; j++) power *= d;
            ans += power; // 加上 d^i
        }

        // ---- Part 2: 位数和 n 相同的数 ----
        // 逐位从高到低分析
        for (int i = 0; i < len; i++) {
            bool hasSame = false;
            for (const string& dig : digits) {
                if (dig[0] < S[i]) {
                    // 这一位放比 S[i] 小的数字
                    // 后续 (len - i - 1) 位每位 d 种选择
                    int power = 1;
                    for (int j = 0; j < len - i - 1; j++) power *= d;
                    ans += power;
                } else if (dig[0] == S[i]) {
                    hasSame = true;
                }
                // dig[0] > S[i]: 不合法, 跳过
            }
            // 当前位没有匹配的数字, 无法继续构造等长的数
            if (!hasSame) return ans;
        }

        // 所有位都恰好匹配 → n 本身可以被组成
        return ans + 1;
    }
};


// ============================================================
// 解法2: 数位 DP（从后往前填表）
//
// 思路:
//   dp[i] = 从第 i 位开始, 且前面已贴着上界时,
//           能组成多少合法的"等长"数
//   dp[len] = 1 (base case: 所有位都填完了)
//
//   转移 (从后往前):
//   for each digit d in digits:
//     if d < S[i]: dp[i] += d^(remaining)  // 后面自由
//     if d == S[i]: dp[i] += dp[i+1]       // 继续受约束
//     if d > S[i]: skip
//
// 时间: O(log n * d) | 空间: O(log n)
// ============================================================
class Solution2 {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string S = to_string(n);
        int len = S.size();
        int d = digits.size();

        // dp[i]: 从位 i 开始, 贴着上界, 能形成多少等长数
        vector<int> dp(len + 1, 0);
        dp[len] = 1;

        for (int i = len - 1; i >= 0; i--) {
            for (const string& dig : digits) {
                if (dig[0] < S[i]) {
                    // 当前位比上界小, 后续位完全自由
                    int power = 1;
                    for (int j = 0; j < len - i - 1; j++) power *= d;
                    dp[i] += power;
                } else if (dig[0] == S[i]) {
                    // 当前位等于上界, 后续继续受约束
                    dp[i] += dp[i + 1];
                }
            }
        }

        // dp[0] = 等长且 <= n 的数
        // 加上所有短位数的数
        int ans = dp[0];
        for (int i = 1; i < len; i++) {
            int power = 1;
            for (int j = 0; j < i; j++) power *= d;
            ans += power;
        }

        return ans;
    }
};


// ============================================================
// 解法3: 预计算 powers 数组优化版
//
// 思路: 预计算 d^0, d^1, ..., d^len 避免重复乘法
//
// 时间: O(log n * d) | 空间: O(log n)
// ============================================================
class Solution3 {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string S = to_string(n);
        int len = S.size();
        int d = digits.size();
        int ans = 0;

        // 预计算 d 的幂次: pw[i] = d^i
        vector<int> pw(len + 1, 1);
        for (int i = 1; i <= len; i++) {
            pw[i] = pw[i - 1] * d;
        }

        // Part 1: 短位数 (1 到 len-1 位)
        for (int i = 1; i < len; i++) {
            ans += pw[i]; // d^i
        }

        // Part 2: 等长位数, 逐位分析
        for (int i = 0; i < len; i++) {
            bool hasSame = false;
            for (const string& dig : digits) {
                if (dig[0] < S[i]) {
                    ans += pw[len - i - 1]; // 后续位自由: d^(remaining)
                } else if (dig[0] == S[i]) {
                    hasSame = true;
                }
            }
            if (!hasSame) return ans;
        }

        return ans + 1;
    }
};


// ============================================================
// 解法4: 二分查找优化版 (适用于 digits 很大的情况)
//
// 思路:
//   利用 digits 已排序的性质,
//   用 lower_bound 快速找到 < S[i] 的数字个数
//   和是否存在 == S[i] 的数字
//
// 时间: O(log n * log d) | 空间: O(log n)
// ============================================================
class Solution4 {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        string S = to_string(n);
        int len = S.size();
        int d = digits.size();
        int ans = 0;

        // 提取字符数组用于二分
        vector<char> digs;
        for (auto& s : digits) digs.push_back(s[0]);

        // 预计算幂次
        vector<int> pw(len + 1, 1);
        for (int i = 1; i <= len; i++) pw[i] = pw[i - 1] * d;

        // Part 1: 短位数
        for (int i = 1; i < len; i++) ans += pw[i];

        // Part 2: 等长位数
        for (int i = 0; i < len; i++) {
            // lower_bound 找到第一个 >= S[i] 的位置
            auto it = lower_bound(digs.begin(), digs.end(), S[i]);
            int lessCnt = it - digs.begin(); // 严格 < S[i] 的个数
            ans += lessCnt * pw[len - i - 1];

            // 检查是否存在 == S[i]
            if (it == digs.end() || *it != S[i]) {
                return ans; // 无法匹配当前位
            }
            // *it == S[i], 继续下一位
        }

        return ans + 1;
    }
};


/*
 * ============================================================
 * 易错点总结:
 *
 * 1. 忘记 +1: 所有位都匹配时, n 本身也合法, 需要 ans+1
 * 2. 短位数从 i=1 开始: i=0 没意义 (0位数不是正整数)
 * 3. pow() 浮点精度: 不要用 math.h 的 pow, 用整数循环乘
 * 4. digits 是 string 数组: 比较时用 dig[0] 取字符
 * 5. 混淆 < 和 ==: 要严格分开处理, 不能合并为 <=
 *
 * 面试追问:
 * Q: digits 包含 '0'? → 需要处理前导零, 第一位不能选 '0'
 * Q: n 非常大(10^18)? → 算法仍然 O(log n), 但要用 long long
 * Q: 数位 DP 通用模板? → dfs(pos, tight, state) + memo
 * ============================================================
 */


/* ============================================================
 * 📋 总结区块
 * ============================================================
 *
 * 一、解法对比
 * ┌──────────────────┬────────────────┬──────────┬──────────────────────┐
 * │ 解法             │ 时间           │ 空间     │ 说明                 │
 * ├──────────────────┼────────────────┼──────────┼──────────────────────┤
 * │ 1.数位分析(数学) │ O(logN * d)    │ O(logN)  │ 面试首选，最直观     │
 * │ 2.数位DP(填表)   │ O(logN * d)    │ O(logN)  │ 自底向上，DP经典思路 │
 * │ 3.预计算幂次优化 │ O(logN * d)    │ O(logN)  │ 消除重复乘法         │
 * │ 4.二分查找优化   │ O(logN * logd) │ O(logN)  │ digits很大时更快     │
 * └──────────────────┴────────────────┴──────────┴──────────────────────┘
 * 核心思想一致：分"短位数"（全合法）+"等长位数"（逐位约束）两部分。
 * 解法1最直白，解法2用DP思维封装，解法4在d很大时用二分提速。
 *
 * 二、易错点
 * 1. 忘记最终 +1：所有位都恰好匹配时，n 本身也合法，漏掉直接少算1。
 * 2. 短位数从 i=1 开始：0位数不是正整数，从 i=0 开始会多算1。
 * 3. 用 math.h 的 pow()：浮点精度陷阱，必须用整数循环乘。
 * 4. digits 是 string[]：比较时需 dig[0] 取 char，直接比较 string 会出错。
 * 5. Part2 中 hasSame=false 时必须立即 return：
 *    当前位无法匹配上界数字，后续位不可能构造出等长且<=n的数。
 *
 * 三、面试追问
 * Q1: 如果 digits 里包含 '0'，逻辑怎么变？
 *     → 短位数部分：第一位不能选0（有前导零），后续位可以选0。
 *       所以 k 位数的个数变为 (d-1) * d^(k-1)，而非 d^k。
 *       等长位数逻辑不变（最高位不可能是0因为n是正整数）。
 *
 * Q2: n 可以是 10^18 级别怎么办？
 *     → 算法本身是 O(logN * d)，n 再大也只有 ~19 位。
 *       但 power 计算可能溢出 int，需改用 long long。
 *
 * Q3: 数位 DP 有通用模板吗？适用于哪些题？
 *     → 通用模板: dfs(pos, tight, state, ...) + memo
 *       tight 表示当前位是否贴着上界。
 *       适用题型：统计区间[L,R]内满足某性质的数的个数，
 *       如不含某数字、数位和约束、相邻数字约束等。
 *
 * ============================================================ */
