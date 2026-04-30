// 解法一：数学贪心（逐步累加）
// 时间 O(sqrt(target))，空间 O(1)
// 先累加 1+2+...+steps 至 sum >= target
// 然后检查 diff = sum - target 的奇偶性来决定是否需要额外步数
class Solution {
public:
    int reachNumber(int target) {
        // 对称性：target 和 -target 答案相同
        long t = abs((long)target);

        long sum = 0;
        int steps = 0;

        // 不断累加直到 sum >= target
        while (sum < t) {
            ++steps;
            sum += steps;
        }

        long diff = sum - t;

        // diff 为偶数：翻转某些步即可到达
        // diff 为奇数：需要额外走步使 diff 变偶数
        if (diff % 2 == 0) {
            return steps;
        }

        // 再走一步
        ++steps;
        sum += steps;
        diff = sum - t;
        if (diff % 2 == 0) {
            return steps;
        }

        // 再走一步（此时 diff 必为偶数）
        ++steps;
        return steps;
    }
};

// 解法二：简洁写法（while 循环判断奇偶）
// 逻辑等价，代码更紧凑
class Solution2 {
public:
    int reachNumber(int target) {
        long t = abs((long)target);
        long sum = 0;
        int steps = 0;

        // 累加直到 sum >= target 且 diff 为偶数
        while (sum < t || (sum - t) % 2 != 0) {
            ++steps;
            sum += steps;
        }

        return steps;
    }
};

// 解法三：二分搜索 + 奇偶调整
// 时间 O(log(target))，空间 O(1)
// 用公式 n*(n+1)/2 >= target 二分找最小 n，然后调整奇偶性
class Solution3 {
public:
    int reachNumber(int target) {
        long t = abs((long)target);

        // 二分搜索最小的 n 使得 n*(n+1)/2 >= target
        long lo = 1, hi = 2 * (long)sqrt((double)t) + 2;
        while (lo < hi) {
            long mid = lo + (hi - lo) / 2;
            if (mid * (mid + 1) / 2 >= t) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }

        int steps = (int)lo;
        long sum = (long)steps * (steps + 1) / 2;
        long diff = sum - t;

        // 调整奇偶性
        while (diff % 2 != 0) {
            ++steps;
            diff += steps;
        }

        return steps;
    }
};

// 解法四：数学公式直接求解
// 使用求根公式找最小 n，然后微调
class Solution4 {
public:
    int reachNumber(int target) {
        long t = abs((long)target);

        // n*(n+1)/2 >= t -> n >= (-1 + sqrt(1+8t)) / 2
        int n = (int)ceil((-1.0 + sqrt(1.0 + 8.0 * t)) / 2.0);

        long sum = (long)n * (n + 1) / 2;
        long diff = sum - t;

        // 如果 diff 为偶数，直接返回
        if (diff % 2 == 0) return n;

        // diff 为奇数，需要再走 1 或 2 步
        // 如果 n+1 为奇数(即 n 为偶数)，走一步 diff 变偶
        // 如果 n+1 为偶数(即 n 为奇数)，走两步
        if ((n + 1) % 2 == 1) {
            return n + 1;
        }
        return n + 2;
    }
};
