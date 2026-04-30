/*
 * 【题目本质】
 * 在 BST 中找给定节点 p 的中序后继，即"大于 p.val 的最小节点"。
 * 核心思想：利用 BST 的有序性质进行二分搜索。
 *
 * 【解法总览】
 * Solution1: 迭代二分搜索 — 利用 BST 性质，O(h) 时间 O(1) 空间 ⭐推荐
 * Solution2: 中序遍历     — 遍历找 p 的下一个节点，O(n) 时间 O(h) 空间
 */

// ===================== Solution1: 迭代二分搜索 =====================
// 思路：从根开始，利用 BST 性质二分搜索
//       val > p->val: 当前节点可能是后继 -> 记录，向左找更小的候选
//       val <= p->val: 当前节点不可能是后继 -> 向右找更大的
// 时间：O(h)，h 为树高
// 空间：O(1)
class Solution1 {
public:
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        TreeNode* successor = nullptr;
        while (root) {
            if (root->val > p->val) {
                // root 比 p 大 -> 可能是后继
                // 但左子树可能有更小的(仍然>p)的候选
                successor = root;
                root = root->left;
            } else {
                // root <= p -> 后继一定在右子树
                root = root->right;
            }
        }
        return successor;
    }
};

// ===================== Solution2: 中序遍历(迭代) =====================
// 思路：用栈模拟中序遍历，遇到 p 后返回下一个访问的节点
// 时间：O(n) 最坏情况
// 空间：O(h) 栈空间
class Solution2 {
public:
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        bool foundP = false;

        while (cur || !stk.empty()) {
            // 先走到最左
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top();
            stk.pop();

            // 如果上一个访问的是 p，当前就是后继
            if (foundP) return cur;

            // 标记找到 p
            if (cur == p) foundP = true;

            cur = cur->right;
        }
        return nullptr;  // p 是最大节点，无后继
    }
};

// ===================== 补充: 找中序前驱(Predecessor) =====================
// 对称操作：找"小于 p.val 的最大节点"
class SolutionPredecessor {
public:
    TreeNode* inorderPredecessor(TreeNode* root, TreeNode* p) {
        TreeNode* predecessor = nullptr;
        while (root) {
            if (root->val < p->val) {
                // root 比 p 小 -> 可能是前驱
                predecessor = root;
                root = root->right;  // 向右找更大的
            } else {
                // root >= p -> 前驱一定在左子树
                root = root->left;
            }
        }
        return predecessor;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 适用场景 |
 * |------|------|------|----------|
 * | 迭代二分搜索 | O(h) | O(1) | BST，面试首选 |
 * | 中序遍历     | O(n) | O(h) | 普通二叉树也适用 |
 *
 * 【易错点】
 * 1. 等号方向: val > p->val 向左，val <= p->val 向右
 *    -> 等于 p 时应继续向右（后继必须严格大于）
 * 2. 不要只找第一个大于 p 的就返回
 *    -> 需要继续向左搜索，找"大于 p 的最小值"
 * 3. p 是最大节点时返回 nullptr
 *    -> 迭代到 root==nullptr 自然返回初始值 nullptr
 * 4. 不要与"右子树最左节点"的分类讨论混淆
 *    -> 迭代法从根开始统一处理，无需分情况
 *
 * 【面试追问】
 * Q1: 普通二叉树如何找后继? -> 只能中序遍历 O(n)
 * Q2: 有parent指针? -> 两种情况分别处理 (LC 510)
 * Q3: 找前驱? -> 对称操作: val < p.val 记录+向右，val >= p.val 向左
 */
