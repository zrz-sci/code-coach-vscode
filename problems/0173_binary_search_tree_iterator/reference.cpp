/*
 * 【题目本质】
 * 将 BST 的递归中序遍历转化为迭代器模式（可暂停/恢复的遍历）。
 * 核心是用栈模拟递归调用栈，实现 O(h) 空间的受控遍历。
 *
 * 【解法总览】
 * 解法1: 栈模拟中序遍历 — O(h) 空间，均摊 O(1) 时间（推荐）
 * 解法2: 预存数组 — O(n) 空间，O(1) 时间（简单但不满足 follow-up）
 */

// ===================== 解法1: 栈模拟中序遍历 =====================
// 思路：用栈保存"从当前节点到最左叶子的路径"
// 栈顶永远是下一个要返回的最小值（中序遍历的下一个元素）
// next() 弹出栈顶后，将其右子树的左链压栈（处理右子树的中序遍历）
class Solution1 {
    stack<TreeNode*> stk;

    // 核心辅助：将 node 及其所有左子节点压栈
    // 模拟递归中序遍历"一路走到最左"的过程
    void pushAllLeft(TreeNode* node) {
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }

public:
    Solution1(TreeNode* root) {
        // 初始化：将根到最左叶子的路径压栈
        pushAllLeft(root);
    }

    int next() {
        // 栈顶是当前最小的未访问节点
        TreeNode* top = stk.top();
        stk.pop();
        // 中序：左-根-右，根已处理，现在处理右子树
        // 右子树同样需要"先走到最左"
        pushAllLeft(top->right);
        return top->val;
    }

    bool hasNext() {
        return !stk.empty();
    }
};

// ===================== 解法2: 预存数组 =====================
// 思路：构造时完整中序遍历，结果存数组，用下标索引访问
// 简单直接，但空间 O(n) 不满足 follow-up 的 O(h) 要求
class Solution2 {
    vector<int> inorder;
    int idx;

    void dfs(TreeNode* node) {
        if (!node) return;
        dfs(node->left);
        inorder.push_back(node->val);
        dfs(node->right);
    }

public:
    Solution2(TreeNode* root) : idx(0) {
        dfs(root);
    }

    int next() {
        return inorder[idx++];
    }

    bool hasNext() {
        return idx < (int)inorder.size();
    }
};

/*
 * 【解法对比】
 * | 解法 | next() 时间 | hasNext() 时间 | 空间 |
 * |------|------------|---------------|------|
 * | 栈模拟 | 均摊 O(1) | O(1) | O(h) |
 * | 预存数组 | O(1) | O(1) | O(n) |
 *
 * 【易错点】
 * 1. pushAllLeft 中必须先 push(node) 再 node=node->left
 *    否则当前节点不会被压栈
 * 2. next() 弹出后必须处理右子树，否则遍历不完整
 *    正确: pop → pushAllLeft(top->right) → return top->val
 * 3. 均摊 O(1) 的理解：不是每次 O(1)，而是 n 次调用总共 O(n)
 *    因为每个节点恰好入栈一次出栈一次
 *
 * 【面试追问】
 * Q1: 为什么栈空间是 O(h) 不是 O(n)？
 *     → 栈中最多存储一条从根到叶子的路径，长度为树高 h
 * Q2: 平衡 BST 的 h 是多少？→ O(log n)，最坏退化链表 O(n)
 * Q3: 如何实现反向迭代器（从大到小）？
 *     → 改为 pushAllRight，next 时处理左子树
 */
