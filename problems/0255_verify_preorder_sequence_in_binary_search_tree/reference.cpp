/*
 * 【题目本质】
 * 验证一个序列是否是某棵 BST 的前序遍历 —— 单调递减栈 + 下界追踪
 * BST 前序: [根, 左子树(都<根), 右子树(都>根)]
 *
 * 【解法总览】
 * 解法1: 单调递减栈        O(n) / O(n)  ⭐推荐
 * 解法2: 原地栈(O(1)空间)  O(n) / O(1)  Follow-up 最优
 */

// ===================== 解法1: 单调递减栈 =====================
// 核心思想:
//   沿左子树走时，值单调递减 → 用递减栈维护
//   遇到比栈顶大的值 → 进入某祖先的右子树 → 弹栈更新下界 low
//   后续所有值必须 > low，否则违反 BST 性质
//
// 示例 [5,2,1,3,6]:
//   5 → 栈[5], low=-INF
//   2 → 栈[5,2], low=-INF (左子树)
//   1 → 栈[5,2,1], low=-INF (左子树)
//   3 → 弹1(low=1), 弹2(low=2), 栈[5,3] (进入2的右子树)
//   6 → 弹3(low=3), 弹5(low=5), 栈[6] (进入5的右子树)
class Solution1 {
public:
    bool verifyPreorder(vector<int>& preorder) {
        stack<int> stk;
        int low = INT_MIN; // 当前允许的最小值

        for (int val : preorder) {
            // 违反下界约束 → 非法序列
            if (val < low) return false;

            // 弹出所有小于 val 的栈元素（离开左子树，进入右子树）
            while (!stk.empty() && val > stk.top()) {
                low = stk.top(); // 更新下界为弹出的祖先值
                stk.pop();
            }

            stk.push(val); // 当前值入栈
        }
        return true;
    }
};

// ===================== 解法2: 原地栈 (常数空间) =====================
// 思路: 利用 preorder 数组的前部作为栈，用 top 指针模拟栈操作
// 满足 Follow-up 要求: O(1) 额外空间
class Solution2 {
public:
    bool verifyPreorder(vector<int>& preorder) {
        int low = INT_MIN;
        int top = -1; // 栈顶指针，-1 表示栈空

        for (int val : preorder) {
            if (val < low) return false;

            // 弹栈：preorder[0..top] 当做栈
            while (top >= 0 && val > preorder[top]) {
                low = preorder[top];
                top--;
            }

            // 入栈
            preorder[++top] = val;
        }
        return true;
    }
};

/*
 * 【解法对比】
 * ┌────────────────┬──────┬──────┬────────────────────────┐
 * │ 解法           │ 时间 │ 空间 │ 特点                   │
 * ├────────────────┼──────┼──────┼────────────────────────┤
 * │ 单调递减栈     │ O(n) │ O(n) │ 经典做法，面试首选     │
 * │ 原地栈         │ O(n) │ O(1) │ Follow-up，修改输入    │
 * └────────────────┴──────┴──────┴────────────────────────┘
 *
 * 【易错点】
 * 1. low 初始化为 INT_MIN，不是 0（节点值可能 >= 1）
 * 2. 弹栈时先更新 low 再 pop，low 始终取最后弹出的值
 * 3. 检查 val < low 要在弹栈之前（先验证合法性）
 * 4. 原地栈会修改输入数组，面试时需告知面试官
 *
 * 【面试追问】
 * Q1: 这个单调栈为什么是递减的？
 *     → BST 前序沿左子树走是递减的，栈模拟这条路径
 * Q2: low 的物理意义？
 *     → 最近一个"已离开其左子树"的祖先节点值
 * Q3: 后序遍历怎么验证？
 *     → 逆序处理，用单调递增栈，维护上界 high
 */
