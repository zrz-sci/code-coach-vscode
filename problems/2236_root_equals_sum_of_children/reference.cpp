/*
 * LeetCode 2236: Root Equals Sum of Children (判断根结点是否等于子结点之和)
 * 难度：Easy | 标签：Tree, Binary Tree
 *
 * ============================================================
 * 题目描述：
 * 给定一棵恰好有 3 个节点的二叉树（根、左孩子、右孩子），
 * 判断根节点的值是否等于两个子节点值之和。
 *
 * 约束：
 * - 树只有根节点、左子节点、右子节点（恰好 3 个节点）
 * - -100 <= Node.val <= 100
 * ============================================================
 *
 * 树结构示意（ASCII图）：
 *
 *   示例1: [10, 4, 6]          示例2: [5, 3, 1]
 *
 *         [10]                       [5]
 *        /    \                     /    \
 *      [4]    [6]                [3]    [1]
 *
 *   10 == 4 + 6 => true         5 != 3 + 1 => false
 *
 * ============================================================
 */


// Definition for a binary tree node.

/*
 * ============================================================
 * 解法一：直接比较（最优解）
 * ============================================================
 *
 * 思路：题目保证树恰好 3 个节点，直接比较即可
 *
 * 执行过程（以 [10, 4, 6] 为例）：
 *
 *       [10]
 *      /    \
 *    [4]    [6]
 *
 *    root->val       = 10
 *    root->left->val = 4
 *    root->right->val = 6
 *    10 == 4 + 6 => 10 == 10 => true
 *
 * 时间复杂度：O(1) — 只访问 3 个节点
 * 空间复杂度：O(1) — 无额外空间
 */
class Solution {
public:
    bool checkTree(TreeNode* root) {
        return root->val == root->left->val + root->right->val;
    }
};


/*
 * ============================================================
 * 解法二：防御性编程版本
 * ============================================================
 *
 * 虽然题目保证 3 节点结构，面试中展示代码健壮性是加分项
 *
 * 考虑的边界情况：
 * - root 为空
 * - 左或右子节点为空
 * - 值为负数（-100 <= val <= 100，不影响逻辑）
 */
class Solution_Defensive {
public:
    bool checkTree(TreeNode* root) {
        if (!root) return false;
        if (!root->left || !root->right) return false;
        return root->val == root->left->val + root->right->val;
    }
};


/*
 * ============================================================
 * 解法三：通用递归版本 —— 验证整棵树的 "Children Sum Property"
 * ============================================================
 *
 * 面试追问：如果树不是固定 3 节点，而是任意二叉树，
 * 要验证"每个非叶节点的值 == 直接子节点值之和"？
 *
 * 使用后序遍历（Post-order DFS）：
 * 1. 先递归验证左右子树
 * 2. 再验证当前节点
 *
 * 示例树（满足 Children Sum Property）：
 *
 *           [10]          10 == 4 + 6 ? YES
 *          /    \
 *        [4]    [6]       4 == 1 + 3 ? YES,  6 == 2 + 4 ? YES
 *       /  \    /  \
 *     [1] [3] [2] [4]    叶节点不需要验证
 *
 * 时间复杂度：O(n) — 访问每个节点一次
 * 空间复杂度：O(h) — 递归栈深度，h 为树高
 */
class Solution_General {
public:
    bool checkTree(TreeNode* root) {
        return verify(root);
    }

private:
    bool verify(TreeNode* node) {
        // 空节点或叶节点：自动满足
        if (!node) return true;
        if (!node->left && !node->right) return true;

        // 先验证子树
        if (!verify(node->left)) return false;
        if (!verify(node->right)) return false;

        // 计算子节点值之和
        int childSum = 0;
        if (node->left) childSum += node->left->val;
        if (node->right) childSum += node->right->val;

        // 验证当前节点
        return node->val == childSum;
    }
};


/*
 * ============================================================
 * 解法四：BFS 层序遍历版本（教学目的）
 * ============================================================
 *
 * 用队列进行层序遍历，对每个非叶节点检查条件
 *
 * 遍历过程：
 *
 *    Queue: [root(10)]
 *    弹出 10, 检查: 10 == 4 + 6 ? YES
 *    Queue: [left(4), right(6)]
 *    弹出 4, 叶节点跳过
 *    弹出 6, 叶节点跳过
 *    Queue: empty => return true
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(w)，w 为最大层宽度
 */
class Solution_BFS {
public:
    bool checkTree(TreeNode* root) {
        if (!root) return true;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            // 非叶节点：检查条件
            if (node->left || node->right) {
                int sum = 0;
                if (node->left) {
                    sum += node->left->val;
                    q.push(node->left);
                }
                if (node->right) {
                    sum += node->right->val;
                    q.push(node->right);
                }
                if (node->val != sum) return false;
            }
        }
        return true;
    }
};


/*
 * ============================================================
 * 解法五：Sum Tree 验证（进阶面试追问）
 * ============================================================
 *
 * Sum Tree 定义：每个非叶节点的值 = 左子树所有节点值之和 + 右子树所有节点值之和
 * （注意：是"子树所有节点之和"，不仅仅是直接子节点）
 *
 *        [26]           26 == (10+4+6) + (3+3) = 20 + 6 = 26 ? YES
 *       /    \
 *     [10]   [3]        10 == 4 + 6 = 10 ? YES,  3 == 3 ? YES
 *    /   \      \
 *  [4]   [6]   [3]      叶节点自动满足
 *
 * 后序遍历返回子树总和
 */
class Solution_SumTree {
public:
    bool isSumTree(TreeNode* root) {
        return subtreeSum(root) != -1;
    }

private:
    // 返回以 node 为根的子树总和
    // 如果不是 Sum Tree，返回 -1
    // 注意：实际实现中用 optional<int> 更安全
    long long subtreeSum(TreeNode* node) {
        if (!node) return 0;

        // 叶节点直接返回自身值
        if (!node->left && !node->right) {
            return node->val;
        }

        long long leftSum = subtreeSum(node->left);
        if (leftSum == -1) return -1;

        long long rightSum = subtreeSum(node->right);
        if (rightSum == -1) return -1;

        // 验证：node->val 应该等于左右子树所有节点之和
        if (node->val != leftSum + rightSum) {
            return -1;
        }

        // 返回包含 node 自身的子树总和
        return node->val + leftSum + rightSum;
    }
};


/*
 * ============================================================
 * 复杂度总结
 * ============================================================
 *
 * | 解法         | 时间     | 空间    | 说明                |
 * |-------------|---------|---------|---------------------|
 * | 直接比较      | O(1)   | O(1)   | 本题最优             |
 * | 防御版        | O(1)   | O(1)   | 加 NULL 检查         |
 * | 通用递归      | O(n)   | O(h)   | 验证所有节点          |
 * | BFS          | O(n)   | O(w)   | 层序遍历版本          |
 * | Sum Tree     | O(n)   | O(h)   | 面试进阶追问          |
 *
 * ============================================================
 * 面试追问链
 * ============================================================
 *
 * Q1: 如果树不是 3 节点，而是任意二叉树？
 *     -> 用 Solution_General（后序遍历验证每个非叶节点）
 *
 * Q2: 如果要验证 Sum Tree（子树总和，不仅是直接子节点）？
 *     -> 用 Solution_SumTree（后序遍历返回子树和）
 *
 * Q3: 如果要修改树使其满足 Children Sum Property？
 *     -> 自顶向下：如果 parent < children_sum，增大 parent
 *        如果 parent > children_sum，增大一个 child
 *        递归处理后，回溯更新 parent = left + right
 *
 * Q4: 如何处理 N-ary tree（多叉树）？
 *     -> 将 left + right 改为遍历所有 children 求和
 *
 * Q5: 如果值是浮点数，如何处理精度？
 *     -> 用 abs(root->val - sum) < epsilon 替代 ==
 *
 * ============================================================
 * 相关题目
 * ============================================================
 *
 * 100. Same Tree               —— 树的基础比较
 * 104. Maximum Depth            —— 树的递归入门
 * 112. Path Sum                 —— 树的值比较 + 递归
 * 226. Invert Binary Tree       —— 树的结构操作
 * 404. Sum of Left Leaves       —— 树的值累加
 * 508. Most Frequent Subtree Sum —— 子树和统计
 * 617. Merge Two Binary Trees   —— 树的值操作
 */


/* ============================================================
 * 📋 总结区块
 * ============================================================
 *
 * 一、解法对比
 * ┌──────────────────┬──────┬──────┬────────────────────────────┐
 * │ 解法             │ 时间 │ 空间 │ 说明                       │
 * ├──────────────────┼──────┼──────┼────────────────────────────┤
 * │ 1.直接比较       │ O(1) │ O(1) │ 本题最优解                 │
 * │ 2.防御性编程     │ O(1) │ O(1) │ 加 NULL 检查，面试加分     │
 * │ 3.通用递归(DFS)  │ O(n) │ O(h) │ Children Sum 推广          │
 * │ 4.BFS层序遍历    │ O(n) │ O(w) │ 教学目的，展示BFS思维      │
 * │ 5.Sum Tree验证   │ O(n) │ O(h) │ 进阶追问：子树总和性质     │
 * └──────────────────┴──────┴──────┴────────────────────────────┘
 * 本题本身是 Easy 入门题，一行代码即可。价值在于面试追问的延伸：
 * 从固定3节点 → 任意二叉树 → Sum Tree → N叉树 → 浮点精度。
 *
 * 二、易错点
 * 1. 过度工程化：本题保证恰好3个节点，直接比较即可。
 *    面试中先给最简解，再主动提"如果不保证3节点我会这样写"。
 *
 * 2. 通用版忘记叶节点判断：
 *    叶节点没有子节点，不需要验证 Children Sum Property。
 *    如果不特判叶节点，childSum=0 != node->val 会误判。
 *
 * 3. Sum Tree 中返回值语义混淆：
 *    subtreeSum 返回"子树总和"而非"节点值"。
 *    用 -1 表示非法状态时要注意节点值本身可能为负数。
 *    更安全的做法是用 optional<int> 或 pair<bool, int>。
 *
 * 三、面试追问
 * Q1: 如果树不是固定3节点，而是任意二叉树，如何验证 Children Sum？
 *     → 后序遍历（Solution_General）：先验证左右子树，
 *       再检查当前节点值是否等于左子节点+右子节点。
 *       叶节点自动满足。时间 O(n)，空间 O(h)。
 *
 * Q2: 如果要验证 Sum Tree（节点值 = 左子树所有节点之和 + 右子树所有节点之和）？
 *     → 后序遍历返回子树总和（Solution_SumTree）。
 *       每个非叶节点检查 val == leftSum + rightSum。
 *       返回 val + leftSum + rightSum 给父节点使用。
 *
 * Q3: 如果要修改树使其满足 Children Sum Property？
 *     → 两遍遍历：第一遍自顶向下，若 parent < childSum 则增大 parent，
 *       若 parent > childSum 则增大某个child（保证值只增不减）。
 *       第二遍自底向上回溯，令 parent = left->val + right->val。
 *
 * ============================================================ */
