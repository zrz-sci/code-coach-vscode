// LeetCode 779: K-th Symbol in Grammar
// 难度: Medium | 标签: Bit Manipulation, Recursion, Math
//
// 解法一：Bit Manipulation（最优解）
// 时间 O(log k)  空间 O(1)
//
// 核心发现：答案 = popcount(k - 1) % 2
// 第 n 行第 k 个字符可以看作一棵完全二叉树中从根到叶的路径。
// 每次走右子树翻转一次，走左子树不翻转。
// k-1 的二进制表示中，1 的个数就是走右子树的次数。

class Solution {
public:
    int kthGrammar(int n, int k) {
        // __builtin_popcount 计算整数中 1 的个数
        return __builtin_popcount(k - 1) & 1;
    }
};

// =============================================================
// 解法二：递归法
// 时间 O(n) 或 O(log k)  空间 O(n) 或 O(log k) 栈空间
//
// 第 n 行第 k 个字符由第 n-1 行第 ceil(k/2) 个字符决定：
//   - k 为奇数 -> 与父节点相同
//   - k 为偶数 -> 与父节点取反
// 递归到第 1 行（只有一个 0）结束。

class Solution2 {
public:
    int kthGrammar(int n, int k) {
        // base case: 第 1 行只有 0
        if (n == 1) return 0;

        // 父节点的位置
        int parentK = (k + 1) / 2;
        int parentVal = kthGrammar(n - 1, parentK);

        // k 为奇数：结果和父节点相同
        // k 为偶数：结果和父节点取反
        if (k % 2 == 1) {
            return parentVal;
        } else {
            return 1 - parentVal;
        }
    }
};

// =============================================================
// 解法三：迭代回溯法
// 时间 O(log k)  空间 O(1)
//
// 从位置 k 出发，不断向上回溯到根节点。
// 每次 k 为偶数时计一次翻转，然后 k = ceil(k/2)。
// 最后翻转次数的奇偶性决定答案。

class Solution3 {
public:
    int kthGrammar(int n, int k) {
        int flips = 0;
        while (k > 1) {
            // k 为偶数说明当前节点是右孩子，需要翻转
            if (k % 2 == 0) {
                flips++;
            }
            // 向上移动到父节点
            k = (k + 1) / 2;
        }
        // 根节点是 0，翻转奇数次结果为 1，偶数次结果为 0
        return flips % 2;
    }
};

// =============================================================
// 解法四：递归法（简化版，不用 n）
// 时间 O(log k)  空间 O(log k)
//
// 既然 n 不影响结果，可以直接忽略 n，
// 递归 f(k) = f((k+1)/2) XOR (k 为偶数 ? 1 : 0)

class Solution4 {
public:
    int kthGrammar(int n, int k) {
        return solve(k);
    }

private:
    int solve(int k) {
        if (k == 1) return 0;
        // 父节点的值
        int parent = solve((k + 1) / 2);
        // 偶数位翻转
        return (k % 2 == 0) ? (1 - parent) : parent;
    }
};

// =============================================================
// 解法五：完全二叉树 + 路径分析
// 时间 O(log k)  空间 O(1)
//
// 从另一个角度理解 popcount 解法：
// 将 k-1 写成二进制，从最高位到最低位依次走。
// 0 表示走左子树（值不变），1 表示走右子树（值翻转）。
// 最终值 = XOR(所有走右的步骤) = popcount(k-1) % 2

class Solution5 {
public:
    int kthGrammar(int n, int k) {
        int val = 0;   // 根节点值为 0
        int pos = k - 1; // 转 0-indexed

        // 从最高有效位开始遍历
        // 第 n 行有 2^(n-1) 个元素，所以路径长度是 n-1
        // 但我们可以直接看 pos 的所有位
        for (int bit = 30; bit >= 0; bit--) {
            if (pos & (1 << bit)) {
                val ^= 1; // 走右子树，翻转
            }
        }
        return val;
    }
};
