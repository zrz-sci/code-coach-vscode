/*
 * LeetCode 105: 从前序与中序遍历序列构造二叉树
 * (Construct Binary Tree from Preorder and Inorder Traversal)
 *
 * 题目本质：利用前序确定根、中序确定左右子树范围的分治构建。
 *          HashMap 加速中序查找是关键优化。
 *
 * 解法总览：
 *   Solution1 - 递归+HashMap(preIdx)   O(n)/O(n)    面试首选
 *   Solution2 - 递归+线性查找(下标)    O(n^2)/O(n)  无优化基础版
 *   Solution3 - 迭代(栈模拟)           O(n)/O(n)    追问不用递归时给出
 *
 * ASCII 树图:
 *   preorder = [3, 9, 20, 15, 7]
 *   inorder  = [9, 3, 15, 20, 7]
 *
 *   inorder 中找到 root=3 的位置 (index=1):
 *     左子树 inorder[0..0] = [9]        -> leftSize = 1
 *     右子树 inorder[2..4] = [15,20,7]
 *
 *   preorder 中:
 *     root = pre[0] = 3
 *     左子树 pre[1..1] = [9]
 *     右子树 pre[2..4] = [20,15,7]
 *
 *   构建结果:
 *          3
 *         / \
 *        9   20
 *           / \
 *         15   7
 */

// ============================================================
// 解法1: 递归 + HashMap + 全局 preIdx — O(n)/O(n) ⭐面试首选
// ============================================================
// 思考过程：
//   前序第一个是根 -> 在中序用 HashMap O(1) 查找根位置 ->
//   分出左右子树范围 -> 递归构建。
//   用全局 preIdx 自动按前序顺序取根，避免复杂的下标计算。
//   关键约束：必须先建左子树再建右子树（与前序"根左右"一致）。
class Solution1 {
public:
    unordered_map<int, int> inMap;  // 中序: 值 -> 下标
    int preIdx = 0;                 // 前序指针，每取一个根就自增

    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        // 预处理中序数组，建立 HashMap
        for (int i = 0; i < (int)inorder.size(); i++) {
            inMap[inorder[i]] = i;
        }
        return build(preorder, 0, (int)inorder.size() - 1);
    }

    TreeNode* build(vector<int>& preorder, int inLeft, int inRight) {
        if (inLeft > inRight) return nullptr;

        // 前序的下一个元素就是当前子树的根
        int rootVal = preorder[preIdx++];
        TreeNode* root = new TreeNode(rootVal);

        // 在中序中定位根
        int inIdx = inMap[rootVal];

        // 先左后右！顺序不能反（preIdx 按前序递增）
        root->left = build(preorder, inLeft, inIdx - 1);
        root->right = build(preorder, inIdx + 1, inRight);

        return root;
    }
};

// ============================================================
// 解法2: 递归 + 线性查找 + 显式下标范围 — O(n^2)/O(n)
// ============================================================
// 思考过程：
//   不用 HashMap，直接线性查找根在中序中的位置。
//   需要同时维护前序和中序的下标范围（6 个参数）。
//   每层 O(n) 查找，最坏 O(n^2)（退化为链状树时）。
class Solution2 {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return build(preorder, 0, (int)preorder.size() - 1,
                     inorder, 0, (int)inorder.size() - 1);
    }

    TreeNode* build(vector<int>& pre, int pL, int pR,
                    vector<int>& in, int iL, int iR) {
        if (pL > pR) return nullptr;

        // 前序第一个是根
        TreeNode* root = new TreeNode(pre[pL]);

        // 线性查找根在中序中的位置
        int idx = iL;
        while (in[idx] != pre[pL]) idx++;

        int leftSize = idx - iL;  // 左子树节点数

        // 左子树: pre[pL+1 .. pL+leftSize],  in[iL .. idx-1]
        root->left = build(pre, pL + 1, pL + leftSize,
                           in, iL, idx - 1);
        // 右子树: pre[pL+leftSize+1 .. pR],  in[idx+1 .. iR]
        root->right = build(pre, pL + leftSize + 1, pR,
                            in, idx + 1, iR);
        return root;
    }
};

// ============================================================
// 解法3: 迭代（栈模拟） — O(n)/O(n)
// ============================================================
// 思考过程：
//   用栈维护从根到当前节点的路径。
//   遍历前序数组，每个新节点要么是栈顶的左孩子，
//   要么是某个祖先的右孩子（通过与中序对比来判断）。
//   当栈顶值 == inorder[inIdx] 时，说明左子树结束，需回溯。
class Solution3 {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty()) return nullptr;

        TreeNode* root = new TreeNode(preorder[0]);
        stack<TreeNode*> stk;
        stk.push(root);
        int inIdx = 0;

        for (int i = 1; i < (int)preorder.size(); i++) {
            TreeNode* node = new TreeNode(preorder[i]);

            if (stk.top()->val != inorder[inIdx]) {
                // 栈顶不是中序下一个 -> 新节点是栈顶的左孩子
                stk.top()->left = node;
            } else {
                // 回溯：弹出所有已完成左子树的祖先
                TreeNode* parent = nullptr;
                while (!stk.empty() && stk.top()->val == inorder[inIdx]) {
                    parent = stk.top();
                    stk.pop();
                    inIdx++;
                }
                // 新节点是最后弹出的节点的右孩子
                parent->right = node;
            }
            stk.push(node);
        }
        return root;
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 解法      | 时间   | 空间  | 特点                          |
 * |-----------|--------|-------|-------------------------------|
 * | Solution1 | O(n)   | O(n)  | HashMap+preIdx, 面试首选      |
 * | Solution2 | O(n^2) | O(n)  | 线性查找, 6参数, 不推荐       |
 * | Solution3 | O(n)   | O(n)  | 迭代栈模拟, 追问时给出        |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. 先右后左导致 preIdx 错位:
 *    ✗ root->right = build(...); root->left = build(...);
 *    ✓ root->left = build(...); root->right = build(...);
 *    前序是 "根左右"，preIdx 必须先消费左子树再消费右子树
 *
 * 2. Solution2 右子树的前序起点算错:
 *    ✗ pre[pL + leftSize, pR]       -> 少跳了根节点
 *    ✓ pre[pL + leftSize + 1, pR]   -> +1 跳过根节点 pL
 *
 * 3. preIdx 不能作为局部变量/参数传递:
 *    ✗ build(preorder, preIdx+1, ...) -> 左子树消费几个 preIdx 未知
 *    ✓ 成员变量 preIdx，所有递归层共享
 *
 * 4. HashMap 依赖"无重复值"前提:
 *    有重复值时 HashMap 会覆盖，导致构建错误
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 后序+中序构建? (LC 106)
 *   -> 后序最后一个是根，从后往前取，先建右子树再建左子树
 *
 * Q2: 前序+后序能唯一确定吗? (LC 889)
 *   -> 不能。无法区分只有左孩子 vs 只有右孩子的情况
 *
 * Q3: 时间复杂度为什么是 O(n)?
 *   -> 每个节点恰好创建一次 + HashMap 查找一次 = O(n)
 *   -> 等价于 preIdx 从 0 到 n-1，n 次 O(1) 操作
 */
