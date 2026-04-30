/*
 * LeetCode 233: 数字 1 的个数 (Number of Digit One)
 *
 * 【题目本质】
 * 统计 0 ~ n 所有整数中，数字 1 总共出现了多少次。
 * 例如 n=13: 1,10,11(两个1),12,13 → 共 6 个 1
 *
 * 【核心思路】
 * 暴力 O(n log n) 在 n=10^9 时不可行。
 * 关键: 不要按"数"统计，而是按"位"统计。
 * 对每一位 (个位、十位、百位...)，独立计算该位上出现 1 的次数。
 *
 * 【三种情况的公式】
 * 设 factor 为当前位的权值 (1, 10, 100, ...)
 *     high = n / (factor * 10)    -- 当前位左边的数
 *     cur  = (n / factor) % 10    -- 当前位的数字
 *     low  = n % factor           -- 当前位右边的数
 *
 * cur == 0: count += high * factor
 * cur == 1: count += high * factor + low + 1
 * cur >= 2: count += (high + 1) * factor
 *
 * 【解法总览】
 * 解法1: 逐位数学分析  — O(log n) / O(1) — 面试首选 ⭐
 * 解法2: 暴力枚举      — O(n log n) / O(1) — 仅用于验证
 * 解法3: 数位 DP       — O(log^2 n) / O(log n) — 通用框架
 */

/*
 * 公式推导可视化 (以 n = 31456 分析百位为例):
 *
 * n = 3 1 [4] 5 6     factor = 100
 *         ^
 *     high=31  cur=4  low=56
 *
 * 我们要数: 0~31456 中百位为 1 的数有多少个?
 * 即形如 XX1YY 的数, 其中 XX=00~??, YY=00~99
 *
 * cur=4 >= 2:
 *   XX = 00~31: 百位可以取1, YY = 00~99 → 32 * 100 = 3200
 *   公式: (high + 1) * factor = (31+1) * 100 = 3200
 *
 * 如果 cur=1 (n=31156):
 *   XX = 00~30: 百位取1, YY = 00~99 → 31 * 100 = 3100
 *   XX = 31:    百位=1=cur, YY 只能 00~56 → 57
 *   公式: high*factor + low+1 = 31*100 + 56+1 = 3157
 *
 * 如果 cur=0 (n=31056):
 *   XX = 00~30: 百位取1, YY = 00~99 → 31 * 100 = 3100
 *   XX = 31:    百位=0 < 1, 不可能取1 → 0
 *   公式: high * factor = 31 * 100 = 3100
 */

// ============================================================
// 解法1: 逐位数学分析 ⭐ 面试首选
// 时间: O(log n) — 只遍历 n 的每一位  空间: O(1)
//
// 【思路】
// 对 n 的每一位 (factor = 1, 10, 100, ...)，把 n 拆成 high/cur/low 三部分，
// 根据 cur 的值（0 / 1 / >=2）套用不同公式，累加各位的贡献。
//
// 【为什么正确】
// 对于 factor 位，我们在数"有多少个 0~n 的整数，其 factor 位恰好是 1"。
// 这等价于计算形如 XXXXX-1-YYYYY 的合法数有多少个。
// 根据 cur 和 1 的大小关系，决定高位和低位的自由度。
// ============================================================
class Solution1 {
public:
    int countDigitOne(int n) {
        long long count = 0;

        // factor: 当前分析的位的权值, 从个位(1)开始
        for (long long factor = 1; factor <= n; factor *= 10) {
            long long high = n / (factor * 10); // 当前位左边
            long long cur = (n / factor) % 10;  // 当前位
            long long low = n % factor;          // 当前位右边

            if (cur == 0) {
                // 当前位为 0: "够不到 1"
                // 高位只能取 0 ~ high-1 (不含 high，因为 high 对应 cur=0)
                // 低位任意: 0 ~ factor-1
                count += high * factor;
            } else if (cur == 1) {
                // 当前位为 1: "刚好踩线"
                // 高位 0 ~ high-1: 低位任意 → high * factor
                // 高位 = high: 当前位=1=cur, 低位只能 0 ~ low → low+1
                count += high * factor + low + 1;
            } else {
                // 当前位 >= 2: "完整覆盖"
                // 高位可取 0 ~ high (含 high，因为 cur>1 可以放心取 1)
                // 低位任意: 0 ~ factor-1
                count += (high + 1) * factor;
            }
        }
        return (int)count;
    }
};

// ============================================================
// 解法2: 暴力枚举 — 仅用于小规模验证
// 时间: O(n * log10(n))  空间: O(1)
//
// 遍历 1~n 的每个数，数每个数中 1 的个数。
// n = 10^9 时约 10^10 次操作，远超时间限制。
// 但适合做 n <= 10^5 的对拍验证。
// ============================================================
class Solution2 {
public:
    int countDigitOne(int n) {
        int count = 0;
        for (int i = 1; i <= n; i++) {
            int num = i;
            while (num > 0) {
                if (num % 10 == 1) count++;
                num /= 10;
            }
        }
        return count;
    }
};

// ============================================================
// 解法3: 数位 DP — 通用框架
// 时间: O(10 * log(n) * log(n))  空间: O(log(n))
//
// 【思路】
// 数位 DP 是处理"统计 0~n 中满足某数位条件的数"的通用方法。
// 状态: dp(pos, cnt, tight)
// - pos: 当前处理第几位 (从最高位开始)
// - cnt: 已经出现的 1 的个数
// - tight: 是否受上界约束 (即前面的位是否都取到了 n 的对应位)
//
// 转移: 枚举当前位取 0~limit (tight ? digits[pos] : 9)
//
// 【和解法1的对比】
// - 解法1 是针对"统计某个数字出现次数"的特化公式，O(log n) 时间，O(1) 空间
// - 数位 DP 是通用框架，可以处理更复杂的约束（如"1 和 2 都不出现"等）
// - 面试中优先写解法1，追问通用方法再说数位 DP
// ============================================================
class Solution3 {
    int digits[10];   // n 的各位数字 (高位在前)
    int len;           // 位数
    int memo[10][10][2]; // memo[pos][cnt][tight]

    // 返回从第 pos 位开始，已有 cnt 个 1，是否受限，能产生的总 1 个数
    int dp(int pos, int cnt, bool tight) {
        if (pos == len) return cnt; // 所有位处理完，返回 1 的个数

        if (memo[pos][cnt][tight] != -1)
            return memo[pos][cnt][tight];

        int limit = tight ? digits[pos] : 9;
        int result = 0;
        for (int d = 0; d <= limit; d++) {
            result += dp(pos + 1,
                        cnt + (d == 1 ? 1 : 0),
                        tight && (d == limit));
        }
        return memo[pos][cnt][tight] = result;
    }

public:
    int countDigitOne(int n) {
        if (n <= 0) return 0;

        // 提取 n 的各位数字 (高位在前)
        len = 0;
        int tmp = n;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        reverse(digits, digits + len);

        memset(memo, -1, sizeof(memo));
        return dp(0, 0, true);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 适用场景 |
// |------|------|------|---------|
// | 1.逐位数学 ⭐ | O(log n) | O(1) | 面试首选 |
// | 2.暴力枚举 | O(n log n) | O(1) | 验证用 |
// | 3.数位 DP | O(log^2 n) | O(log n) | 复杂数位条件 |
//
// 【验证对拍】
// 用解法2验证解法1: n=13 → 6; n=100 → 21; n=1000 → 301
//
// 【易错点】
// 1. 整数溢出:
//    ✗ for (int factor = 1; ...)  // factor*10 可能溢出 int
//    ✓ for (long long factor = 1; ...)
//    n 可达 10^9, factor 最大 10^9, factor*10 = 10^10 超 int 范围。
//
// 2. cur==1 时忘记 +1:
//    ✗ count += high * factor + low;     // 漏了 low=0 的情况
//    ✓ count += high * factor + low + 1; // 低位 0~low 共 low+1 个
//
// 3. 混淆 cur==0 和 cur>=2 的 high 范围:
//    cur==0: 高位 0 ~ high-1 (不含 high) → high * factor
//    cur>=2: 高位 0 ~ high   (含 high)   → (high+1) * factor
//    直觉: cur>=2 时当前位"富余"，可以让高位多取一个值。
//
// 4. 数位 DP 中遗漏 tight 状态:
//    如果不区分 tight/non-tight, 会多算超过 n 的数。
//    tight=true 时当前位上限是 digits[pos], tight=false 时上限是 9。
//
// 【面试追问】
// Q1: 推广到统计数字 d (0~9)?
//    → 把 cur 和 d 比较即可。d=0 需特殊处理（高位从 1 开始，无前导零）。
//
// Q2: 如果要统计 [a, b] 区间内的 1 的个数?
//    → countDigitOne(b) - countDigitOne(a - 1)，利用前缀和思想。
//
// Q3: 数位 DP 的通用框架?
//    → dp(pos, state, tight): pos 从高到低填位, state 记录需要的信息,
//      tight 表示是否受上界约束。memo 缓存 (pos, state, tight)。
//
// Q4: 能否一行公式统一三种情况?
//    → count += (high + (cur>1?1:0)) * factor + (cur==1 ? low+1 : 0);
//      但分开写更清晰，面试中推荐分开。
// ============================================================
