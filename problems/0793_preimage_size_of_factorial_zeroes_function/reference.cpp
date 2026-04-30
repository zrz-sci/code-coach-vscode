// 793. Preimage Size of Factorial Zeroes Function
// 难度: Hard
// 标签: Math, Binary Search

// ============================================================
// 解法一：双二分查找 (推荐面试首选)
// 时间 O(log^2 K), 空间 O(1)
// ============================================================
// 思路: 分别找到满足 f(x) >= k 的最小 x (leftBound)
//       和满足 f(x) >= k+1 的最小 x (rightBound)
//       答案 = rightBound - leftBound (一定是 0 或 5)
class Solution {
public:
    int preimageSizeFZF(int k) {
        // 找到满足 trailingZeroes(x) >= k 的最小 x
        long long left = lowerBound(k);
        // 找到满足 trailingZeroes(x) >= k+1 的最小 x
        long long right = lowerBound(k + 1);
        // 差值就是满足 f(x) == k 的 x 的个数
        return (int)(right - left);
    }

private:
    // 计算 x! 末尾零的个数
    // 末尾零由因子 10 = 2*5 产生, 因子 2 总是足够,
    // 所以只需数因子 5 的个数
    long long trailingZeroes(long long x) {
        long long count = 0;
        while (x > 0) {
            x /= 5;
            count += x;
        }
        return count;
    }

    // 找满足 trailingZeroes(x) >= target 的最小 x
    long long lowerBound(int target) {
        long long lo = 0, hi = 5LL * (target + 1);

        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (trailingZeroes(mid) < target) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }

        return lo;
    }
};

// ============================================================
// 解法二：单次二分 + 判断
// 时间 O(log^2 K), 空间 O(1)
// ============================================================
// 思路: 二分找任意一个 x 使得 f(x) == k
//       如果找到, 答案为 5; 否则为 0
class Solution2 {
public:
    int preimageSizeFZF(int k) {
        long long lo = 0, hi = 5LL * (k + 1);

        while (lo <= hi) {
            long long mid = lo + (hi - lo) / 2;
            long long zeros = trailingZeroes(mid);

            if (zeros == k) {
                return 5;  // 找到了, 答案恒为 5
            } else if (zeros < k) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        return 0;  // k 是被跳过的值
    }

private:
    long long trailingZeroes(long long x) {
        long long count = 0;
        while (x > 0) {
            x /= 5;
            count += x;
        }
        return count;
    }
};

// ============================================================
// 解法三：数学推导 (5进制分析)
// 时间 O(log K), 空间 O(1)
// ============================================================
// 观察: f(x) 跳过的值恰好是那些在"5进制"表示中
// 会产生进位的值。可以直接判断 k 是否在跳跃间隙中。
//
// 原理: 对于连续5个数 [5m, 5m+4], 它们的 f 值相同。
// f(5m) - f(5m-1) 取决于 m 中 5 的因子个数:
//   - 如果 m 不是5的倍数: f 增加 1
//   - 如果 m 是5的倍数但不是25的倍数: f 增加 2 (跳过1个值)
//   - 如果 m 是25的倍数但不是125的倍数: f 增加 3 (跳过2个值)
//   - ...
//
// 要判断 k 是否被跳过, 可以对 k 做"5进制分解":
// 不断将 k 除以 5 取余, 如果某一步余数 == 4,
// 则说明 k 处于一个"跳跃间隙"中, 答案为 0;
// 否则答案为 5。
class Solution3 {
public:
    int preimageSizeFZF(int k) {
        long long val = k;
        while (val > 0) {
            if (val % 5 == 4) return 0;
            val /= 5;
        }
        return 5;
    }
};

// ============================================================
// 辅助说明: 为什么答案只可能是 0 或 5
// ============================================================
//
// 设 x = 5q + r, 其中 0 <= r <= 4
// 则 f(x) = f(5q+r)
//
// 对于 r = 0,1,2,3,4 (即 5q, 5q+1, 5q+2, 5q+3, 5q+4):
// 这 5 个数的阶乘末尾零个数完全相同, 因为 5q+1 到 5q+4
// 都不是 5 的倍数, 不会贡献新的因子 5。
//
// 因此 f 值相同的连续 x 恰好有 5 个 (一个"平台")。
//
// 从一个平台到下一个平台, f 值至少增加 1:
// - 增加 1: 标准情况 (5q 只是 5 的 1 次倍数)
// - 增加 2: 5q 是 25 的倍数 (跳过 1 个值)
// - 增加 3: 5q 是 125 的倍数 (跳过 2 个值)
// - ...
//
// 所以对于任何 k, 要么恰好有 5 个 x 使得 f(x)=k,
// 要么 k 被跳过, 0 个 x。
//
// 例子:
//   k=0: x=0,1,2,3,4   -> 5个
//   k=1: x=5,6,7,8,9   -> 5个
//   k=4: x=20,21,22,23,24 -> 5个
//   k=5: 被跳过 (f(24)=4, f(25)=6) -> 0个
//   k=6: x=25,26,27,28,29 -> 5个
