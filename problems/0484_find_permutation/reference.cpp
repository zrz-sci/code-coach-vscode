/*
 * 484. Find Permutation
 * 难度: Medium | 标签: Stack, Greedy, Array, String
 *
 * 【题目本质】
 * 给定 'I'(增) / 'D'(减) 序列，构造满足该模式的字典序最小排列 [1..n]。
 * 贪心：先升序排列，再将每段连续 D 对应的子数组反转。
 *
 * 【解法总览】
 * Solution1: 升序初始化 + 反转D段   O(n)  O(1)额外  ⭐ 推荐
 * Solution2: 栈模拟               O(n)  O(n)      通用经典
 * Solution3: 双指针原地构造        O(n)  O(1)额外   一次遍历
 */

// ===================== Solution1: 升序 + 反转D段 =====================
// 思路: [1,2,...,n] 本身字典序最小, 只在 D 段做最小范围反转
class Solution1 {
public:
    vector<int> findPermutation(string s) {
        int n = s.size() + 1;
        vector<int> res(n);

        // Step1: 初始化升序排列
        for (int i = 0; i < n; i++) {
            res[i] = i + 1;
        }

        // Step2: 找到每段连续 'D', 反转对应区间
        int i = 0;
        while (i < (int)s.size()) {
            if (s[i] == 'D') {
                int j = i;
                // 找到连续 D 段的末尾
                while (j < (int)s.size() && s[j] == 'D') j++;
                // s[i..j-1] 全是 D, 需要 res[i] > res[i+1] > ... > res[j]
                // 反转 res[i..j] (共 j-i+1 个元素)
                reverse(res.begin() + i, res.begin() + j + 1);
                i = j;
            } else {
                i++;
            }
        }

        return res;
    }
};

// ===================== Solution2: 栈模拟 =====================
// 思路: 按 1,2,...,n 入栈, 遇到 'I' 或到末尾时弹出所有
// 栈的 LIFO 特性自然产生 D 段的递减效果
class Solution2 {
public:
    vector<int> findPermutation(string s) {
        int n = s.size() + 1;
        vector<int> res;
        res.reserve(n);
        stack<int> stk;

        for (int i = 1; i <= n; i++) {
            stk.push(i);

            // 到末尾 或 遇到 'I' -> 弹出所有元素
            if (i == n || s[i - 1] == 'I') {
                while (!stk.empty()) {
                    res.push_back(stk.top());
                    stk.pop();
                }
            }
        }

        return res;
    }
};

// ===================== Solution3: 双指针原地构造 =====================
// 思路: 用一个计数器, 遇到 I 时直接放, 遇到 D 时记录段长后倒序填充
class Solution3 {
public:
    vector<int> findPermutation(string s) {
        int n = s.size() + 1;
        vector<int> res(n);
        int num = 1; // 当前要放的最小数

        int i = 0;
        while (i < n) {
            if (i == (int)s.size() || s[i] == 'I') {
                // 放当前数字
                res[i] = num++;
                i++;
            } else {
                // 找到连续 D 段的末尾
                int j = i;
                while (j < (int)s.size() && s[j] == 'D') j++;
                // 从 i 到 j (共 j-i+1 个位置) 倒序填充
                for (int k = j; k >= i; k--) {
                    res[k] = num++;
                }
                i = j + 1;
            }
        }

        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 反转 | O(n) | O(1)额外 | 最直观, 面试易讲解 |
 * | Solution2 栈 | O(n) | O(n) | 通用模式, 易推广 |
 * | Solution3 原地 | O(n) | O(1)额外 | 一次遍历, 最高效 |
 *
 * 【易错点】
 * 1. 排列长度 n = s.size() + 1, 不是 s.size()
 * 2. 反转区间: D段 s[i..j-1] 对应位置 res[i..j], 右边界是 j 不是 j-1
 * 3. 栈解法中 i==n 时也要弹出: 字符串末尾可能是 D 段
 * 4. 数的范围从 1 开始, 不是从 0 开始
 * 5. 连续 D 段找完后 i 要跳到 j, 不要 i++
 *
 * 【面试追问】
 * Q1: 如何证明反转D段后排列一定合法?
 *     反转前 res[i..j] 升序, 反转后降序, 满足 D 约束。
 *     I 位置不受影响, 因为反转区间只覆盖 D 段。
 * Q2: 能否处理包含 '=' (相等) 的情况?
 *     排列中每个数唯一, 不可能有相等关系。若允许重复数字则需不同建模。
 * Q3: 如果同时要求字典序第 k 小的排列呢?
 *     可用康托展开或逐位确定法, 但复杂度会增加到 O(n^2) 或需要树状数组。
 */
