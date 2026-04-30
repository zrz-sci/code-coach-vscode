/*
 * 【题目本质】
 * 偶数除2、奇数+1或-1，求到1的最少操作数。
 * 核心：贪心看二进制末两位决定+1/-1。
 *
 * 【解法总览】
 * Solution1: 位运算贪心      O(log n) / O(1)  ⭐推荐
 * Solution2: 记忆化递归      O(log n) / O(log n)
 */

// ===================== Solution 1: 位运算贪心 =====================
// 偶数：右移
// 奇数：末两位=11且n!=3 -> +1（进位消除连续1）
//       末两位=01或n==3 -> -1
class Solution1 {
public:
    int integerReplacement(int n) {
        int ops = 0;
        long num = n; // 防INT_MAX+1溢出
        while (num > 1) {
            if (num % 2 == 0) {
                num >>= 1;
            } else if (num == 3 || (num & 3) == 1) {
                num--;
            } else {
                num++;
            }
            ops++;
        }
        return ops;
    }
};

// ===================== Solution 2: 记忆化递归 =====================
// 偶数递归n/2，奇数递归min(n+1, n-1)
// memo避免重复计算
class Solution2 {
public:
    unordered_map<long, int> memo;

    int integerReplacement(int n) {
        return dfs((long)n);
    }

    int dfs(long n) {
        if (n == 1) return 0;
        if (memo.count(n)) return memo[n];
        if (n % 2 == 0) {
            return memo[n] = 1 + dfs(n / 2);
        }
        return memo[n] = 1 + min(dfs(n + 1), dfs(n - 1));
    }
};

/*
 * 【解法对比】
 * Solution1 贪心：O(1)空间，无递归栈，面试首选
 * Solution2 递归：代码直观，但需要额外空间，且要处理溢出
 *
 * 【易错点】
 * 1. n=INT_MAX时n+1溢出 -> 必须用long
 * 2. n=3是特例：3->2->1(2步) 优于 3->4->2->1(3步)
 * 3. 贪心判断条件：(num & 3) 看末两位，不是只看最后一位
 * 4. 偶数优先处理（没有选择），奇数才需要贪心
 *
 * 【面试追问】
 * Q: 贪心的正确性如何证明？
 * A: 对于末尾连续k个1：
 *    +1操作将...0111...1变为...1000...0，后续k次右移
 *    -1操作变为...0111...0，后续1次右移再处理剩余的k-1个1
 *    当k>=2时+1总步数更少(k+1 vs 1+处理k-1个1)
 *    当k==1时-1更优(末尾01: -1变00只需1步额外)
 *    n=3(k=2)是边界：+1需3步、-1需2步，因为进位后的1本身就是终点附近
 */
