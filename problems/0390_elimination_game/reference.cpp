/*
 * 【题目本质】
 * 交替从左右消除序列，求最后剩余元素。
 * 核心：每轮数量减半、间距翻倍，追踪head位置即可。
 *
 * 【解法总览】
 * Solution1: 迭代追踪head  O(log n) / O(1) ⭐推荐
 * Solution2: 递归对称性     O(log n) / O(log n)
 */

// ===================== Solution 1: 迭代追踪head =====================
// 思路：维护 head(当前首元素)、step(间距)、remaining(剩余个数)、left(方向)
// 从左消除或从右消除且奇数个 -> head += step
class Solution1 {
public:
    int lastRemaining(int n) {
        int head = 1, step = 1, remaining = n;
        bool left = true;
        while (remaining > 1) {
            if (left || remaining % 2 == 1) {
                head += step;
            }
            remaining /= 2;
            step *= 2;
            left = !left;
        }
        return head;
    }
};

// ===================== Solution 2: 递归对称性 =====================
// 思路：从左消除一轮后得[2,4,...,2*(n/2)]
//       等价于 2 * (从右消除[1..n/2])
//       从右消除 f'(m) = m + 1 - f(m) (对称性)
//       合并：f(n) = 2 * (n/2 + 1 - f(n/2))
class Solution2 {
public:
    int lastRemaining(int n) {
        if (n == 1) return 1;
        return 2 * (n / 2 + 1 - lastRemaining(n / 2));
    }
};

/*
 * 【解法对比】
 * Solution1 迭代：空间O(1)，无栈溢出风险，面试首选
 * Solution2 递归：代码极简，但递归深度O(log n)
 *
 * 【易错点】
 * 1. 从右消除时，仅奇数个元素才移动head
 * 2. step和remaining的更新顺序不能颠倒
 * 3. 递归中 n/2 是整数除法，奇偶都适用
 *
 * 【面试追问】
 * Q: 如何证明迭代解法的正确性？
 * A: 每轮消除等价于在等差数列中隔一个取一个。
 *    从左取时第一个必被删，head跳到第二个(+step)。
 *    从右取时，偶数个元素最左保留，奇数个最左被删。
 *    归纳法可证每一步head正确指向剩余序列首元素。
 */
