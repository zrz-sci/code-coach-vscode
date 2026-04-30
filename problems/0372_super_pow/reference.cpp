/*
 * 372. Super Pow
 *
 * 【题目本质】
 * 计算 a^b mod 1337，其中 b 是数组表示的超大整数。
 * 核心在于利用模运算分配律，将大指数按位拆解。
 *
 * 【解法总览】
 * Solution1: 迭代法 — 从高位到低位逐位处理指数        O(n) / O(1)
 * Solution2: 递归法 — 从低位到高位递归拆分指数        O(n) / O(n)
 *
 * 【关键公式】
 * a^[d0,d1,...,dn] = (...((a^d0)^10 * a^d1)^10 * ... * a^dn)
 * 即从最高位开始，每处理一位就把之前结果升10次方再乘当前位的幂
 */

// ===================== Solution1: 迭代法（推荐）=====================
// 从高位到低位，result = result^10 * a^b[i]，每步取模
// 时间 O(n)，空间 O(1)
class Solution1 {
    static const int MOD = 1337;

    // 计算 a^k % MOD，k 只在 0~9 范围
    long powmod(long a, int k) {
        a %= MOD;
        long result = 1;
        for (int i = 0; i < k; i++) {
            result = result * a % MOD;
        }
        return result;
    }

public:
    int superPow(int a, vector<int>& b) {
        long result = 1;
        for (int i = 0; i < (int)b.size(); i++) {
            // 之前的结果升10次方，再乘以当前位的贡献
            result = powmod(result, 10) * powmod(a, b[i]) % MOD;
        }
        return (int)result;
    }
};

// ===================== Solution2: 递归法 =====================
// a^[d0,...,dn] = (a^[d0,...,dn-1])^10 * a^dn
// 每次取最后一位递归处理
// 时间 O(n)，空间 O(n) 递归栈
class Solution2 {
    static const int MOD = 1337;

    long powmod(long a, int k) {
        a %= MOD;
        long result = 1;
        for (int i = 0; i < k; i++) {
            result = result * a % MOD;
        }
        return result;
    }

public:
    int superPow(int a, vector<int>& b) {
        if (b.empty()) return 1;
        int last = b.back();
        b.pop_back();
        // 递归：高位部分升10次方 * 当前位的贡献
        return (int)(powmod(superPow(a, b), 10) * powmod(a, last) % MOD);
    }
};

/*
 * 【解法对比】
 * | 解法     | 时间  | 空间  | 优点               | 缺点               |
 * |----------|-------|-------|--------------------|--------------------|
 * | Solution1| O(n)  | O(1)  | 空间最优，无递归开销 | 需要理解从高位迭代   |
 * | Solution2| O(n)  | O(n)  | 递归直观易理解       | 修改了原数组b、栈空间|
 *
 * 【易错点】
 * 1. 中间乘法溢出：每步都要取模，用 long 存中间值
 * 2. powmod 中 a 忘记取模：当 a 很大时 a*result 溢出
 * 3. 递归 base case 遗漏：b 为空时必须返回 1
 * 4. 两部分相乘后忘记取模：powmod(res,10) * powmod(a,d) 也要 % MOD
 * 5. 迭代方向搞反：从高位到低位是 "升10次方"，反过来不等价
 *
 * 【面试追问】
 * Q1: MOD 值 1337 有什么特殊性？
 * A1: 1337 = 7 * 191，两个质数之积。phi(1337) = 6*190 = 1140，
 *     可用欧拉定理将 b mod 1140 后再快速幂，但需处理 gcd(a,1337)!=1 的情况。
 *
 * Q2: 如果 b 不是数组而是字符串，解法有变化吗？
 * A2: 完全相同，只是 b[i] 变成 b[i]-'0'。核心逻辑不变。
 *
 * Q3: 能否用标准快速幂？
 * A3: 可以，但需要先将数组 b 转为对 phi(1337) 取模的整数。
 *     直接对大数组做快速幂不可行，因为指数无法用 int/long 表示。
 */
