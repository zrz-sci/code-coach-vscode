/*
 * LeetCode 889: Construct Binary Tree from Preorder and Postorder Traversal
 * (根据前序和后序遍历构造二叉树)
 *
 * 【题目本质】
 * 分治递归：利用前序第二个元素（左子树根）在后序中的位置
 * 来划分左右子树范围，递归构建。
 * 注意：前序+后序的重建结果可能不唯一。
 *
 * 【解法总览】
 * 解法1: 递归 + 线性查找  — O(n^2) / O(n)  — 最直觉
 * 解法2: 递归 + HashMap    — O(n) / O(n)    — 面试首选
 * 解法3: 迭代（栈模拟）   — O(n) / O(n)    — 加分项
 */

// ============================================================
// 解法1: 递归 + 线性查找 — 暴力分治
// 时间: O(n^2) 最坏  空间: O(n) 递归栈
//
// 【思路】
// 前序: [根, [左子树前序], [右子树前序]]
// 后序: [[左子树后序], [右子树后序], 根]
//
// 步骤:
// 1. pre[preS] 是当前子树的根
// 2. pre[preS+1] 是左子树的根（如果左子树存在）
// 3. 在后序中找到 pre[preS+1] 的位置 idx
// 4. 左子树大小 L = idx - postS + 1
// 5. 递归构建左右子树
//
// 范围划分:
//   左子树前序: [preS+1, preS+L]
//   右子树前序: [preS+L+1, preE]
//   左子树后序: [postS, idx]
//   右子树后序: [idx+1, postE-1]
//
// 示例: pre=[1,2,4,5,3,6,7], post=[4,5,2,6,7,3,1]
//
//   根 = pre[0] = 1
//   左子树根 = pre[1] = 2
//   在 post 中找 2 → idx = 2
//   leftSize = 2 - 0 + 1 = 3
//   左子树前序: pre[1..3] = [2,4,5]
//   右子树前序: pre[4..6] = [3,6,7]
//   左子树后序: post[0..2] = [4,5,2]
//   右子树后序: post[3..5] = [6,7,3]
//   递归构建...
// ============================================================
class Solution1 {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        return build(preorder, postorder,
                     0, preorder.size() - 1,
                     0, postorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& pre, vector<int>& post,
                    int preS, int preE, int postS, int postE) {
        // 空子树
        if (preS > preE) return nullptr;

        // 创建根节点
        TreeNode* root = new TreeNode(pre[preS]);

        // 叶子节点，无需继续划分
        if (preS == preE) return root;

        // 前序第二个元素 = 左子树的根
        int leftRootVal = pre[preS + 1];

        // 在后序中线性查找左子树根
        int idx = postS;
        while (post[idx] != leftRootVal) {
            idx++;
        }

        // 左子树节点数
        int leftSize = idx - postS + 1;

        // 递归构建左右子树
        root->left = build(pre, post,
                           preS + 1, preS + leftSize,
                           postS, idx);
        root->right = build(pre, post,
                            preS + leftSize + 1, preE,
                            idx + 1, postE - 1);

        return root;
    }
};

// ============================================================
// 解法2: 递归 + HashMap — O(1) 查找优化（⭐ 面试首选）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 和解法1 完全相同的分治逻辑，区别在于：
// 预处理 postorder 的值→索引映射，
// 将后序中查找左子树根的位置从 O(n) 降到 O(1)。
//
// 总递归调用次数 = n（每个节点处理一次），
// 每次 O(1) 查找 → 总时间 O(n)。
// ============================================================
class Solution2 {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        // 预处理：后序值 → 索引
        unordered_map<int, int> postMap;
        for (int i = 0; i < (int)postorder.size(); i++) {
            postMap[postorder[i]] = i;
        }

        return build(preorder, postorder, postMap,
                     0, preorder.size() - 1,
                     0, postorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& pre, vector<int>& post,
                    unordered_map<int, int>& postMap,
                    int preS, int preE, int postS, int postE) {
        if (preS > preE) return nullptr;

        TreeNode* root = new TreeNode(pre[preS]);
        if (preS == preE) return root;

        // O(1) 查找左子树根在后序中的位置
        int leftRootVal = pre[preS + 1];
        int idx = postMap[leftRootVal];
        int leftSize = idx - postS + 1;

        root->left = build(pre, post, postMap,
                           preS + 1, preS + leftSize,
                           postS, idx);
        root->right = build(pre, post, postMap,
                            preS + leftSize + 1, preE,
                            idx + 1, postE - 1);

        return root;
    }
};

// ============================================================
// 解法3: 迭代（栈模拟） — 用栈模拟构建过程
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 用栈维护"从根到当前位置的路径"。
// 按前序遍历顺序创建节点并入栈。
// 用后序遍历的指针判断栈顶节点的子树是否已全部构建完毕：
//   如果 stk.top()->val == postorder[postIdx]，
//   说明栈顶节点的整棵子树已处理完，弹出并移动 postIdx。
//
// 新节点挂到哪里？
//   弹栈结束后，看栈顶节点（当前的"待构建父节点"）：
//   - 如果 left 为空 → 挂为左孩子
//   - 否则 → 挂为右孩子
//
// 关键洞察: 后序遍历是"左右根"，
//   当栈顶值出现在后序当前位置时，意味着该节点的子树全部完成。
//
// 示例: pre=[1,2,4,5,3,6,7], post=[4,5,2,6,7,3,1]
//
//   i=0: 创建 1，入栈 [1]
//   i=1: stk.top()=1, post[0]=4 ≠ 1 → 创建 2，挂为 1.left，入栈 [1,2]
//   i=2: stk.top()=2, post[0]=4 ≠ 2 → 创建 4，挂为 2.left，入栈 [1,2,4]
//   i=3: stk.top()=4, post[0]=4 == 4 → 弹出 4, postIdx=1
//        stk.top()=2, post[1]=5 ≠ 2 → 创建 5，挂为 2.right，入栈 [1,2,5]
//   i=4: stk.top()=5, post[1]=5 == 5 → 弹出 5, postIdx=2
//        stk.top()=2, post[2]=2 == 2 → 弹出 2, postIdx=3
//        stk.top()=1, post[3]=6 ≠ 1 → 创建 3，挂为 1.right，入栈 [1,3]
//   ... 以此类推
// ============================================================
class Solution3 {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        if (preorder.empty()) return nullptr;

        TreeNode* root = new TreeNode(preorder[0]);
        stack<TreeNode*> stk;
        stk.push(root);
        int postIdx = 0;

        for (int i = 1; i < (int)preorder.size(); i++) {
            TreeNode* node = new TreeNode(preorder[i]);

            // 弹出已完成子树的节点
            while (stk.top()->val == postorder[postIdx]) {
                stk.pop();
                postIdx++;
            }

            // 挂到栈顶节点的空子节点位置
            TreeNode* parent = stk.top();
            if (parent->left == nullptr) {
                parent->left = node;
            } else {
                parent->right = node;
            }

            stk.push(node);
        }

        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间       | 空间  | 特点                    |
// |---------------|-----------|------|------------------------|
// | 递归+线性查找   | O(n^2)    | O(n) | 最直觉，展示分治框架     |
// | 递归+HashMap   | O(n)      | O(n) | ⭐面试首选，查找优化     |
// | 迭代(栈)       | O(n)      | O(n) | 无递归，但思路较难解释   |
//
// 面试建议:
//   先画出前序/后序的结构图 → 解释如何划分左右子树
//   → 写递归+HashMap → 提到可以用栈迭代优化
//
// 【易错点】
// 1. 忘记叶子节点判断:
//    ✗ 不判断 preS==preE 直接访问 pre[preS+1] → 越界
//    ✓ preS==preE 时立刻返回叶子节点
//
// 2. leftSize 少算一个:
//    ✗ leftSize = idx - postS → 少了左子树根自身
//    ✓ leftSize = idx - postS + 1
//
// 3. 右子树后序范围多了根:
//    ✗ 右子树后序: [idx+1, postE] → postE 是根节点
//    ✓ 右子树后序: [idx+1, postE-1]
//
// 4. 套用 LC 105 模板:
//    ✗ 在后序中找根（后序最后一个就是根，不需要"找"）
//    ✓ 在后序中找的是"左子树根"（前序第二个元素），不是当前根
//
// 【面试追问 Interview Follow-ups】
// Q1: 为什么前序+后序不能唯一确定二叉树？
// A1: 当节点只有一个孩子时，无法区分左右。例如 pre=[1,2], post=[2,1]
//     → 2 可以是 1 的左孩子或右孩子。中序遍历才能明确左右分界。
//
// Q2: 复杂度能否优于 O(n)？
// A2: 不能。必须创建 n 个节点，下界就是 O(n)。
//
// Q3: 和 LC 105/106 的关键区别？
// A3: 105/106 在中序中找根来划分左右，本题在后序中找左子树根
//     来划分大小。中序提供左右分界，后序/前序提供根的位置。
// ============================================================
