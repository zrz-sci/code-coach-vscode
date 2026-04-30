/*
 * 483. Smallest Good Base
 * 难度: Hard | 标签: Math, Binary Search
 *
 * 【题目本质】
 * 找最小的 k >= 2 使得 n 的 k 进制表示全为 1。
 * 即求最小 k 满足 n = 1 + k + k^2 + ... + k^(m-1) = (k^m - 1)/(k - 1)。
 * 关键洞察：m 越大 k 越小，从最大 m 开始枚举，二分/验证 k。
 *
 * 【解法总览】
 * Solution1: 枚举位数m + 二分搜索k    O(log^2 n)  O(1)  ⭐ 推荐
 * Solution2: 枚举位数m + 直接计算k验证  O(log^2 n)  O(1)  数学简洁
 */

// ===================== Solution1: 枚举m + 二分k =====================
class Solution1 {
public:
    string smallestGoodBase(string n) {
        long long num = stoll(n);

        // m: n 在 k 进制下的位数 (全1时)
        // k >= 2 => n >= 2^m - 1 => m <= log2(n+1)
        // n <= 10^18 => m <= 60
        for (int m = 60; m >= 2; m--) {
            // 二分搜索 k
            // k 的上界: k^(m-1) <= n => k <= n^(1/(m-1))
            long long lo = 2;
            long long hi = (long long)pow((double)num, 1.0 / (m - 1)) + 1;

            while (lo <= hi) {
                long long mid = lo + (hi - lo) / 2;

                // 计算等比求和 sum = 1 + mid + mid^2 + ... + mid^(m-1)
                long long sum = 0, cur = 1;
                bool overflow = false;
                for (int i = 0; i < m; i++) {
                    sum += cur;
                    if (sum > num) { overflow = true; break; }
                    if (i < m - 1) {
                        // 溢出检查
                        if (cur > num / mid) { overflow = true; break; }
                        cur *= mid;
                    }
                }

                if (overflow || sum > num) {
                    hi = mid - 1;
                } else if (sum < num) {
                    lo = mid + 1;
                } else {
                    return to_string(mid);
                }
            }
        }
        // 兜底: m=2 时 n = 1 + k => k = n-1 (总是成立)
        return to_string(num - 1);
    }
};

// ===================== Solution2: 枚举m + 直接计算k =====================
class Solution2 {
public:
    string smallestGoodBase(string n) {
        long long num = stoll(n);

        // 从最大位数开始枚举
        for (int m = 60; m >= 2; m--) {
            // 直接用 pow 计算 k 的候选值
            long long k = (long long)pow((double)num, 1.0 / (m - 1));
            if (k < 2) continue;

            // 验证: 1 + k + k^2 + ... + k^(m-1) == num ?
            long long sum = 0, cur = 1;
            bool valid = true;
            for (int i = 0; i < m; i++) {
                sum += cur;
                if (sum > num) { valid = false; break; }
                if (i < m - 1) {
                    if (cur > num / k) { valid = false; break; }
                    cur *= k;
                }
            }
            if (valid && sum == num) {
                return to_string(k);
            }

            // 浮点精度问题: 也尝试 k+1
            k++;
            sum = 0; cur = 1; valid = true;
            for (int i = 0; i < m; i++) {
                sum += cur;
                if (sum > num) { valid = false; break; }
                if (i < m - 1) {
                    if (cur > num / k) { valid = false; break; }
                    cur *= k;
                }
            }
            if (valid && sum == num) {
                return to_string(k);
            }
        }
        return to_string(num - 1);
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 二分 | O(log^2 n) | O(1) | 精确无浮点误差风险 |
 * | Solution2 直接计算 | O(log^2 n) | O(1) | 代码更简洁 |
 *
 * 【易错点】
 * 1. 必须用 long long: n 最大 10^18
 * 2. 溢出防护: cur *= mid 前检查 cur > num / mid
 * 3. 浮点精度: pow() 算根可能偏差 +-1, 需要验证邻近值
 * 4. m 从大到小枚举: 保证找到的 k 最小
 * 5. 兜底情况: 所有 m 都不行时返回 num-1 (对应 n = 1 + (n-1))
 *
 * 【面试追问】
 * Q1: 为什么 m 从大到小枚举?
 *     m 越大 k 越小。等比求和 n = 1+k+...+k^(m-1) 中，m 和 k 负相关。
 *     从最大 m 开始找到的第一个合法 k 即为最小。
 * Q2: 如果 n 非常大(比如 10^100), 怎么办?
 *     需要大数运算。但本题 n <= 10^18 用 long long 足够。
 * Q3: 这个思路能否推广到 "每位不全是1而是其他数字d" 的情况?
 *     可以, 此时 n = d*(k^m-1)/(k-1), 需要额外检查 n 是否被 d 整除。
 */
