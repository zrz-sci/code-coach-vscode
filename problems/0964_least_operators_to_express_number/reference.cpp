// LeetCode 964: Least Operators to Express Number
// 解法一：迭代 DP（逐位贪心） —— 推荐解法
// 时间 O(log_x(target))，空间 O(1)

class Solution {
public:
    int leastOpsExpressTarget(int x, int target) {
        // pos: 当前位选正向（不进位）的最小运算符数
        // neg: 当前位选反向（需要向高位进位）的最小运算符数
        int pos = 0, neg = 0;
        int k = 0; // 当前处理的幂次
        int t = target;

        while (t > 0) {
            int r = t % x; // 当前位的系数
            t /= x;

            // cost: 一个 x^k 单元的运算符数
            // k=0 时是 x/x，需要 2 个运算符（除号 + 连接符）
            // k>=1 时需要 k 个运算符（k-1 个乘号 + 连接符）
            int cost = (k == 0) ? 2 : k;

            if (k == 0) {
                // 第一位（最低位），没有上一位的进位问题
                pos = r * cost;            // 正向：用 r 个 x^0
                neg = (x - r) * cost;      // 反向：用 (x-r) 个 x^0，向高位进 1
            } else {
                int newPos, newNeg;

                // 正向：当前位用 r 个 x^k
                // 可以从上一位的正向或反向（进位使 r 变 r+1）转移
                newPos = min(pos + r * cost,          // 上一位不进位
                             neg + (r + 1) * cost);   // 上一位进位，系数变 r+1

                // 反向：当前位用 (x-r) 个 x^k，向高位进 1
                // 从上一位正向转移：花费 (x-r) 个单元
                // 从上一位反向转移：系数变 r+1，反向花费 (x-r-1) 个单元
                newNeg = min(pos + (x - r) * cost,        // 上一位不进位
                             neg + (x - r - 1) * cost);   // 上一位进位

                pos = newPos;
                neg = newNeg;
            }

            k++;
        }

        // 最终答案：
        // pos 是不需要额外进位的情况
        // neg 需要在最高位之上再加一个 x^k 单元来处理进位
        // 减 1 是因为表达式第一项不需要加/减号
        return min(pos, neg + k) - 1;
    }
};


// ================================================================
// 解法二：记忆化搜索
// 时间 O(log_x(target))，空间 O(log_x(target))
// ================================================================

class Solution2 {
public:
    int leastOpsExpressTarget(int x, int target) {
        memo.clear();
        // dp 返回的是运算符数（包含连接符），最终减 1 去掉第一项的连接符
        return dp(0, target, x) - 1;
    }

private:
    // key: (幂次 i, 剩余目标) -> 最小运算符数
    // 用 long long 编码 key 避免 pair hash
    unordered_map<long long, int> memo;

    // 从 x^i 开始，凑出 target 的最小运算符数
    int dp(int i, int target, int x) {
        if (target == 0) return 0;
        if (target == 1) {
            // 需要一个 x^i 单元
            return cost(i);
        }
        if (i >= 40) {
            // 防止溢出，x^40 远超 2*10^8
            return INT_MAX / 2;
        }

        long long key = (long long)i * 300000000LL + target;
        if (memo.count(key)) return memo[key];

        // 计算 x^i
        long long power = 1;
        for (int j = 0; j < i; j++) {
            power *= x;
            if (power > 2e8) {
                // x^i 已经超过 target 上限
                // 只能用反向（从高位减）
                memo[key] = target * cost(i);
                return memo[key];
            }
        }

        int r = (int)(target / power);  // 当前位系数
        int remainder = (int)(target % power);

        // 选择一：正向，用 r 个 x^i，然后递归处理余数
        int res = r * cost(i) + dp(i + 1, (int)(target - r * power), x);

        // 选择二：反向，用 (r+1) 个 x^i（多一个用来减），产生进位
        // 等价于先凑 (r+1)*x^i，再从高位减去多余的部分
        long long overshot = (r + 1) * power - target;
        if (overshot <= 2e8) {
            // 反向：花费 (r+1)*cost(i) 来得到 (r+1)*x^i
            // 然后需要从更高位减去 overshot
            int alt = (r + 1) * cost(i) + dp(i + 1, (int)overshot, x);
            res = min(res, alt);
        }

        memo[key] = res;
        return res;
    }

    // 一个 x^k 单元的运算符花费（包含连接的加/减号）
    int cost(int k) {
        return (k == 0) ? 2 : k;
    }
};


// ================================================================
// 解法三：更简洁的迭代版本
// ================================================================

class Solution3 {
public:
    int leastOpsExpressTarget(int x, int target) {
        // 特殊情况：target 就是 x 本身
        // x 本身不需要任何运算符

        int pos = 0, neg = 0;
        int k = 0;

        while (target > 0) {
            int r = target % x;
            target /= x;

            if (k == 0) {
                // x^0 = x/x, cost = 2 per unit
                pos = r * 2;
                neg = (x - r) * 2;
            } else {
                // x^k, cost = k per unit (k-1 multiplications + 1 connection)
                int p = min(pos + r * k, neg + (r + 1) * k);
                int n = min(pos + (x - r) * k, neg + (x - r - 1) * k);
                pos = p;
                neg = n;
            }
            k++;
        }

        // neg + k 处理最高位进位（需要一个额外的 x^k 单元）
        // 减 1 去掉首项不需要的连接符
        return min(pos, neg + k) - 1;
    }
};
