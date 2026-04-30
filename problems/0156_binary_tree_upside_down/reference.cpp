/*
 * LeetCode 156: Binary Tree Upside Down (上下翻转二叉树)
 *
 * 【题目本质】
 * 题目保证树是"左子链+右叶子"的特殊结构，翻转操作本质上就是
 * 沿着左子链做一次"链表反转"，同时把每个节点的右兄弟重新挂到新位置。
 *
 * 【解法总览】
 * 解法1: 递归（自底向上） — O(n) / O(h) — 最直观，面试首选
 * 解法2: 迭代（链表反转） — O(n) / O(1) — 空间最优，面试追问
 */

/*
 * 树的特殊结构（题目保证）:
 *
 *       1               沿左子链展开像一条链表:
 *      / \              1(right=3) → 2(right=5) → 4
 *     2   3
 *    / \
 *   4   5
 *
 * 翻转后:
 *       4
 *      / \
 *     5   2
 *        / \
 *       3   1
 *
 * 规则: 对每个节点 root（有左孩子 left 和右孩子 right）:
 *   left->left  = right   (原右兄弟变成左孩子的左孩子)
 *   left->right = root    (原父节点变成左孩子的右孩子)
 *   root->left  = NULL    (断开，避免环)
 *   root->right = NULL    (断开，避免环)
 */

// ============================================================
// 解法1: 递归（自底向上）
// 时间: O(n)  空间: O(h) 递归栈，h 为左子链长度
//
// 【思路】
// 先递归到最左下角（新根），回溯时逐层重连指针。
// 这和递归反转链表的思路完全一样：
//   反转链表: head->next->next = head; head->next = nullptr;
//   本题:     left->right = root; left->left = root->right;
//             root->left = root->right = nullptr;
//
// 递归回溯过程:
//
//  递归下沉: root=1 → root=2 → root=4 (base case, return 4)
//
//  回溯 root=2, left=4:
//    4->left = 2->right = 5    (原右兄弟5成为4的左孩子)
//    4->right = 2              (原父2成为4的右孩子)
//    2->left = 2->right = NULL (断开)
//    此时:     4
//            / \
//           5   2
//
//  回溯 root=1, left=2:
//    2->left = 1->right = 3    (原右兄弟3成为2的左孩子)
//    2->right = 1              (原父1成为2的右孩子)
//    1->left = 1->right = NULL (断开)
//    最终:     4
//            / \
//           5   2
//              / \
//             3   1
// ============================================================
class Solution1 {
public:
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        // 空节点或叶子节点（最左下角）就是新根
        if (!root || !root->left) return root;

        // 递归到最左下角，拿到新根
        TreeNode* newRoot = upsideDownBinaryTree(root->left);

        // 回溯时重连指针:
        // root->left 是当前节点的左孩子，翻转后它会成为 root 的"父节点"
        root->left->left = root->right;   // 原右兄弟 → 左孩子的左孩子
        root->left->right = root;          // 原父节点 → 左孩子的右孩子

        // 必须断开，否则形成环（比如节点2指向1，1还指向2）
        root->left = nullptr;
        root->right = nullptr;

        return newRoot;  // 新根始终是最左下角的节点，一路传递回来
    }
};

// ============================================================
// 解法2: 迭代（链表反转类比）
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的递归栈空间是O(h)。既然结构类似链表，那么就像迭代反转
// 链表一样，用 prev/curr/next 推进即可。
//
// 区别：链表反转只需 prev 和 curr，而这里还需保存 prevRight
// （上一个节点的右孩子，因为它要变成当前节点的左孩子）。
//
// 变量对应关系（与链表反转类比）:
//   链表反转:  prev ← curr → next
//   本题:      prev(+prevRight) ← curr(+currRight) → next(curr->left)
//
// 迭代过程:
//
// 初始: prev=NULL, prevRight=NULL, curr=1
//
// Round1 curr=1:
//   next=2, currRight=3
//   1->left=NULL(prevRight), 1->right=NULL(prev)
//   prev=1, prevRight=3, curr=2
//
// Round2 curr=2:
//   next=4, currRight=5
//   2->left=3(prevRight), 2->right=1(prev)
//   prev=2, prevRight=5, curr=4
//       2
//      / \
//     3   1
//
// Round3 curr=4:
//   next=NULL, currRight=NULL
//   4->left=5(prevRight), 4->right=2(prev)
//   prev=4, prevRight=NULL, curr=NULL → 退出
//       4
//      / \
//     5   2
//        / \
//       3   1
// ============================================================
class Solution2 {
public:
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        TreeNode* curr = root;
        TreeNode* prev = nullptr;       // 翻转后 curr 的右孩子
        TreeNode* prevRight = nullptr;  // 翻转后 curr 的左孩子

        while (curr) {
            // 先保存，因为下面要覆盖这些指针
            TreeNode* next = curr->left;       // 左子链的下一个节点
            TreeNode* currRight = curr->right; // 当前右孩子

            // 重连指针（翻转规则）
            curr->left = prevRight;  // 上一个节点的右孩子 → 当前的左孩子
            curr->right = prev;      // 上一个节点 → 当前的右孩子

            // 推进所有指针
            prev = curr;
            prevRight = currRight;
            curr = next;
        }

        // 循环结束时 curr=NULL，prev 指向最后处理的节点（最左下角）= 新根
        return prev;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 解法1 递归        | 解法2 迭代        |
// |----------|-------------------|-------------------|
// | 时间     | O(n)              | O(n)              |
// | 空间     | O(h) 递归栈       | O(1)              |
// | 可读性   | 高，递归自然      | 中，需类比链表反转 |
// | 面试推荐 | ⭐ 先写           | 追问O(1)空间时给出 |
//
// 两种解法的核心操作完全相同（重连3个指针+断开2个指针），
// 只是遍历方式不同（递归 vs 迭代）。
//
// 【易错点】
//
// 1. 递归版忘记断开 root->left 和 root->right
//    ✗ 只写了 root->left->left = root->right; root->left->right = root;
//    ✓ 必须加 root->left = nullptr; root->right = nullptr;
//    原因: 不断开会形成环，比如 2->right=1 但 1->left 还指向 2
//
// 2. 递归终止条件只检查 !root，忘了检查 !root->left
//    ✗ if (!root) return root;
//    ✓ if (!root || !root->left) return root;
//    原因: 没有左孩子的节点就是最左下角，是新根，直接返回
//
// 3. 迭代版先改指针再保存
//    ✗ curr->left = prevRight; TreeNode* next = curr->left; // next 已经被覆盖!
//    ✓ 先 TreeNode* next = curr->left; 再 curr->left = prevRight;
//    原因: 修改指针后原始信息丢失，必须先保存
//
// 【面试追问】
//
// Q1(基础理解): 这棵树有什么特殊结构？为什么普通二叉树不能这样翻转？
//    → 右节点都是叶子且都有左兄弟，树退化成"左子链+右叶子"。
//      普通二叉树的右子树可能有复杂结构，无法简单重连。
//
// Q2(空间优化): 递归O(h)空间能否优化到O(1)？
//    → 可以，用迭代（解法2），完全类比链表反转的迭代版。
//
// Q3(变体): 如果去掉"右节点是叶子"的限制，还能做吗？
//    → 不能直接用同样方法。右子树有结构时，翻转后的挂载位置
//      不再是简单的一对一，需要重新定义规则或采用其他方法。
// ============================================================
