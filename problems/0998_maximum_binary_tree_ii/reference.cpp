/*
 * LeetCode 998: 最大二叉树 II (Maximum Binary Tree II)
 *
 * 【题目本质】
 * 在已有的最大二叉树末尾追加一个值 val，利用"追加到末尾"的性质
 * 只需沿右子树链找到插入位置。
 *
 * 【解法总览】
 * 解法1: 递归（右子树插入）   — O(n) / O(n)  — 面试首选 ⭐
 * 解法2: 迭代（右子树链遍历） — O(n) / O(1)  — 空间优化
 */

// ============================================================
// 解法1: 递归（右子树插入） — 面试首选 ⭐
// 时间: O(n)  空间: O(n) 递归栈
//
// 【思路】
// 为什么 val 只可能在右子树链上？
//   最大二叉树构造: 数组最大值做根，左半做左子树，右半做右子树。
//   val 追加在数组末尾 → 它在所有现有元素的右侧。
//   从根开始，val 总是落入"右半部分"，递归地只进入右子树。
//
// 三种情况:
//   1. root == null → 直接返回新节点
//   2. val > root->val → val 成为新根，原树变成 val 的左子树
//      （原树所有元素在数组中都在 val 左边）
//   3. val < root->val → 递归插入到 root 的右子树
//
// 示例: 原树对应 a=[2,1,5,3], 插入 val=4
//
//        5           5
//       / \   →     / \
//      2   3       2   4    ← val=4 > 3
//       \           \  /
//        1           1 3    ← 3 变成 4 的左子树
//
//   root=5: 5>4 → 递归右子树
//   root=3: 3<4 → 创建节点4, 4.left=3, 返回4
//   回到5: 5.right = 4
//
// 示例: 插入 val=5 到 [1,4,2,3] 的树
//
//      4                5
//     / \     →        /
//    1   3            4       ← val=5 > 4, 整棵树变成5的左子树
//       /            / \
//      2            1   3
//                      /
//                     2
// ============================================================
class Solution1 {
public:
    TreeNode* insertIntoMaxTree(TreeNode* root, int val) {
        // 基础情况: 走到空节点，直接返回新节点
        if (root == nullptr) {
            return new TreeNode(val);
        }

        // val 比当前节点大: val 成为新根
        if (val > root->val) {
            TreeNode* node = new TreeNode(val);
            node->left = root; // 原子树的所有值在 val 左边
            return node;
        }

        // val 比当前节点小: 继续在右子树中找位置
        // 关键: 必须将递归结果赋值回 root->right
        root->right = insertIntoMaxTree(root->right, val);
        return root;
    }
};

// ============================================================
// 解法2: 迭代（右子树链遍历）
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 显式沿右子树链遍历，找到第一个 curr->right->val < val 的位置。
// 将新节点插入 curr 和 curr->right 之间。
//
// 需要特殊处理 val > root->val 的情况（根节点改变）。
//
// 循环过程:
//   nums = [2,1,5,3], val = 4
//
//   root=5: root.val=5 > val=4 → 进入循环
//   curr=5: curr.right=3, 3.val=3 < val=4 → 找到位置
//   插入: node.left = 3 (原右子树), curr.right = node
//
//          5
//         / \
//        2   4
//         \  /
//          1 3
// ============================================================
class Solution2 {
public:
    TreeNode* insertIntoMaxTree(TreeNode* root, int val) {
        TreeNode* node = new TreeNode(val);

        // 特殊情况: val 是最大值，或者空树
        if (root == nullptr || val > root->val) {
            node->left = root;
            return node;
        }

        // 沿右子树链寻找插入位置
        // 条件: 当前右孩子存在且值 > val，继续往下
        TreeNode* curr = root;
        while (curr->right != nullptr && curr->right->val > val) {
            curr = curr->right;
        }

        // 此时 curr->right == null 或 curr->right->val < val
        // 在 curr 和 curr->right 之间插入 node
        node->left = curr->right;  // 原右子树变成 node 的左子树
        curr->right = node;        // node 成为 curr 的新右孩子

        return root; // 根节点不变
    }
};

// ============================================================
// 解法1 的简洁写法（合并两个base case）
// ============================================================
class Solution3 {
public:
    TreeNode* insertIntoMaxTree(TreeNode* root, int val) {
        // null 或 val > root->val: val 成为新（子）根
        if (!root || val > root->val) {
            TreeNode* node = new TreeNode(val);
            node->left = root; // null 或原子树
            return node;
        }
        // 递归到右子树
        root->right = insertIntoMaxTree(root->right, val);
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 代码量 | 推荐场景 |
// |------|------|------|--------|----------|
// | 递归 | O(n) | O(n) | 很少 | ⭐面试首选 |
// | 迭代 | O(n) | O(1) | 中等 | 追问空间 |
// | 简洁版 | O(n) | O(n) | 最少 | 白板写 |
//
// 【易错点】
//
// 1. 把原子树挂到右子树
//    ✗ node->right = root; → 原元素在 val 右边？不对！
//    ✓ node->left = root;  → 原元素在数组中都在 val 左边
//
// 2. 递归没赋值回 root->right
//    ✗ insertIntoMaxTree(root->right, val); → 新节点没挂上
//    ✓ root->right = insertIntoMaxTree(root->right, val);
//
// 3. 迭代版没处理 val > root->val
//    ✗ 直接进循环 → 永远不会让 root 改变
//    ✓ 单独处理: node->left = root; return node;
//
// 4. 迭代循环条件写反
//    ✗ while (curr->right->val < val) → 找到第一个比 val 小的就停
//       但如果 curr->right 为 null 会空指针
//    ✓ while (curr->right != nullptr && curr->right->val > val)
//       先检查非空，再比较值
//
// 【面试追问】
//
// Q1: 最坏时间复杂度？
// → O(n)，当原数组严格递增（如 [1,2,3,4]）时树是右链，
//    val 需要遍历整条链。
//
// Q2: 如果 val 插入数组开头？
// → 对称处理: 沿左子树链找插入位置，原子树变成 node->right。
//
// Q3: 如果要删除最后一个元素而不是添加？
// → 找到最大二叉树中最右下角的节点（即原数组最后一个元素），
//    将其删除。如果它有左子树，左子树顶替它的位置。
//
// Q4: 和最大二叉树 (LC 654) 的关系？
// → LC 654 是从头构造，O(n) 或 O(n log n)。
//    本题利用"追加到末尾"只需 O(n) 一次插入。
//    如果反复追加 n 个元素，等价于构造整棵最大二叉树。
// ============================================================
