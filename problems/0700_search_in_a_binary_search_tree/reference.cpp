/*
 * LeetCode 700: 二叉搜索树中的搜索 (Search in a Binary Search Tree)
 * 
 * 【题目本质】
 * 利用 BST 的有序性质（左 < 根 < 右），在树上做"二分查找"。
 * 每次比较都能排除一半的搜索空间。
 *
 * 【解法总览】
 * 解法1: 暴力 DFS         — O(n)  / O(n)  — 不利用 BST 性质，遍历整棵树
 * 解法2: 递归 (利用 BST)   — O(h)  / O(h)  — 每次只递归一个方向
 * 解法3: 迭代 (利用 BST)   — O(h)  / O(1)  — 用 while 循环代替递归，最优空间
 *
 * 其中 h 为树高。平衡时 h=O(log n)，退化链表时 h=O(n)。
 */

// ============================================================
// 解法1: 暴力 DFS — 不利用 BST 性质
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 如果我们忘了这是 BST，只把它当普通二叉树——
// 那就只能遍历每个节点，逐一比对。
// 左子树找到就返回，否则去右子树找。
//
// 这个解法的价值在于：展示不利用 BST 性质时需要搜索整棵树。
// 与解法2/3 对比，可以清楚看到 BST 性质的威力。
//
//       4          搜索 val=2:
//      / \         Step1: 4≠2, 搜左子树
//     2   7        Step2: 2==2, 找到! 返回
//    / \           (右子树 7 根本不需要访问，但暴力法不知道)
//   1   3
// ============================================================
class Solution1 {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root) return nullptr;
        if (root->val == val) return root;
        
        // 不利用 BST 性质：左右都要搜
        TreeNode* left = searchBST(root->left, val);
        if (left) return left;               // 左子树找到了就返回
        return searchBST(root->right, val);  // 否则去右子树
    }
};

// ============================================================
// 解法2: 递归 — 利用 BST 性质
// 时间: O(h)  空间: O(h) — 递归调用栈
//
// 【思路】
// BST 性质: 左子树所有值 < 根 < 右子树所有值。
// 
// 这意味着:
// - 如果 val < root->val，val 不可能在右子树中（右子树所有值 > root->val > val）
// - 如果 val > root->val，val 不可能在左子树中（左子树所有值 < root->val < val）
// 
// 所以每一步只需要递归一个方向，就像有序数组上的二分查找。
//
// 递归三要素:
//   返回值: 找到的节点指针（或 nullptr）
//   终止条件: root 为空（没找到）或 root->val == val（找到了）
//   单层逻辑: 比较大小，选择左或右递归
//
// 搜索 val=2 的过程:
//       4          val=2 < 4 → 往左
//      / \
//     2   7        val=2 == 2 → 找到! 返回节点2
//    / \
//   1   3          结果: 以2为根的子树 [2,1,3]
//
// 搜索 val=5 的过程:
//       4          val=5 > 4 → 往右
//      / \
//     2   7        val=5 < 7 → 往左
//    / \
//   1   3          7的左孩子为空 → 返回 nullptr
// ============================================================
class Solution2 {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        // 终止条件: 走到空(没找到) 或 找到目标
        if (!root || root->val == val) return root;
        
        // 利用 BST 性质: 只往一个方向递归
        if (val < root->val) {
            // val 比当前节点小 → 目标只可能在左子树
            return searchBST(root->left, val);
        } else {
            // val 比当前节点大 → 目标只可能在右子树
            return searchBST(root->right, val);
        }
    }
};

// ============================================================
// 解法3: 迭代 — 利用 BST 性质，O(1) 空间
// 时间: O(h)  空间: O(1)
//
// 【思路】
// 解法2 的递归每次只往一个方向走，不需要"回头"。
// 普通二叉树 DFS 需要栈记录回头路（因为左右都要访问），
// 但 BST 搜索每步只走一个方向，所以不需要栈，
// 一个 while 循环就搞定。
//
// 这和有序数组上的二分查找完全类似:
//   数组二分: while (left <= right) { 调整 left 或 right }
//   BST搜索: while (root != null)  { 走左孩子 或 走右孩子 }
//
// 搜索 val=2:
//   root=4 → val<4, root=root->left=2
//   root=2 → val==2, 循环结束, 返回 root
//
// 搜索 val=5:
//   root=4 → val>4, root=root->right=7
//   root=7 → val<7, root=root->left=nullptr
//   root=nullptr → 循环结束, 返回 nullptr
// ============================================================
class Solution3 {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        // 沿着 BST 路径往下走
        while (root && root->val != val) {
            if (val < root->val) {
                root = root->left;   // 目标更小，走左边
            } else {
                root = root->right;  // 目标更大，走右边
            }
        }
        // 循环结束时:
        //   root == nullptr → 没找到
        //   root->val == val → 找到了
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间  | 空间  | 利用BST | 代码复杂度 |
// |-----------|-------|-------|---------|-----------|
// | 暴力 DFS  | O(n)  | O(n)  | ❌      | 简单      |
// | 递归 BST  | O(h)  | O(h)  | ✅      | 最简洁    |
// | 迭代 BST  | O(h)  | O(1)  | ✅      | 简洁      |
//
// - 递归版和迭代版时间复杂度相同，区别仅在空间:
//   递归有 O(h) 的栈开销，迭代只用 O(1)。
// - 面试中推荐先写迭代版（简短+空间最优），
//   再口述递归版展示对递归的理解。
// - 暴力解作为对比提及，说明 BST 性质将 O(n) 优化到 O(h)。
//
// 【易错点】
// 1. 递归终止条件漏掉空指针检查:
//    ✗ if (root->val == val) return root;  ← root 为空时崩溃
//    ✓ if (!root || root->val == val) return root;
//
// 2. 比较方向搞反:
//    ✗ if (val < root->val) return searchBST(root->right, val);
//    ✓ if (val < root->val) return searchBST(root->left, val);
//    val 更小，应该往左（更小的方向）找。
//
// 3. 暴力解中丢失左子树的返回值:
//    ✗ searchBST(root->left, val);         ← 返回值被丢弃
//       return searchBST(root->right, val);
//    ✓ TreeNode* left = searchBST(root->left, val);
//       if (left) return left;
//       return searchBST(root->right, val);
//
// 【面试追问 — 递进链】
//
// Q1: 时间复杂度到底是 O(log n) 还是 O(n)?
// → O(h)，h 是树高。平衡BST时 h=O(log n)，退化链表时 h=O(n)。
//    不能无条件说 O(log n)，必须说明前提。
//
// Q2: 如果有大量查询(比如10^6次)，BST又不平衡怎么办?
// → 方案1: 将BST重建为平衡BST(AVL/红黑树)，保证 h=O(log n)。
//    方案2: 中序遍历得到有序数组，对数组做二分查找。
//    方案3: 用哈希表存所有节点值，O(1)查询（但返回子树需要额外处理）。
//
// Q3: 如果改成插入操作呢? (LeetCode 701)
// → 搜索逻辑完全一样，区别: 搜索走到nullptr就返回null，
//    插入走到nullptr时创建新节点返回。代码改一行:
//    搜索: if (!root) return nullptr;
//    插入: if (!root) return new TreeNode(val);
//
// Q4: 如果改成删除操作呢? (LeetCode 450)
// → 先用搜索逻辑找到目标节点，然后处理三种情况:
//    (1) 叶子节点: 直接删除
//    (2) 只有一个孩子: 用孩子替代自己
//    (3) 两个孩子: 找右子树最小值(后继)替代，再删除后继
// ============================================================
