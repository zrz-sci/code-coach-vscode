/*
 * LeetCode 669: 修剪二叉搜索树 (Trim a Binary Search Tree)
 *
 * 【题目本质】
 * 利用 BST 性质，把值不在 [low, high] 范围内的节点移除，
 * 保持剩余节点的原有结构关系不变。
 * 核心洞察：BST 中如果一个节点太小，它的整个左子树一定更小，全部不要。
 *
 * 【解法总览】
 * 解法1: 递归 — O(n) / O(h) — 面试首选，代码极简
 * 解法2: 迭代 — O(n) / O(1) — 追问空间优化时给出
 */


// ============================================================
// 解法1: 递归 — 利用 BST 性质递归修剪
// 时间: O(n)  空间: O(h)，h 为树高，最坏 O(n)
//
// 【思路】
// trimBST(root, low, high) 返回: 以 root 为根的子树修剪后的新根
//
// 三种情况:
// 1. root->val < low:
//    root 太小，root 的左子树更小（BST性质），全部抛弃
//    答案只可能在右子树中 → 返回 trimBST(root->right, low, high)
//
// 2. root->val > high:
//    root 太大，root 的右子树更大，全部抛弃
//    答案只可能在左子树中 → 返回 trimBST(root->left, low, high)
//
// 3. low <= root->val <= high:
//    root 保留，但左右子树各自可能有越界节点
//    root->left = trimBST(root->left, low, high)
//    root->right = trimBST(root->right, low, high)
//    返回 root
//
// 示例2 递归过程图:
//
//  原始树:                   修剪 [low=1, high=3]:
//       3                         3
//      / \                       /
//     0   4                     2
//      \                       /
//       2                     1
//      /
//     1
//
//  递归调用链:
//  trimBST(3, 1, 3)
//    3 在 [1,3] → 保留
//    3->left = trimBST(0, 1, 3)
//      0 < 1 → 丢弃 0 和其左子树(无)，转到右子树
//      return trimBST(2, 1, 3)
//        2 在 [1,3] → 保留
//        2->left = trimBST(1, 1, 3)
//          1 在 [1,3] → 保留(叶子) → return 1
//        2->right = trimBST(null, 1, 3) → return null
//        return 2 (左孩子=1, 右孩子=null)
//    所以 3->left = 2
//    3->right = trimBST(4, 1, 3)
//      4 > 3 → 丢弃 4 和其右子树(无)，转到左子树
//      return trimBST(null, 1, 3) → return null
//    所以 3->right = null
//  return 3 (左孩子=2, 右孩子=null)
//
//  结果:
//       3
//      /
//     2
//    /
//   1
// ============================================================
class Solution1 {
public:
    TreeNode* trimBST(TreeNode* root, int low, int high) {
        if (!root) return nullptr;

        // 当前节点值太小：它和它的整个左子树都不要
        // 答案只可能在右子树中（右子树值更大，可能有合法的）
        if (root->val < low) {
            return trimBST(root->right, low, high);
        }

        // 当前节点值太大：它和它的整个右子树都不要
        // 答案只可能在左子树中
        if (root->val > high) {
            return trimBST(root->left, low, high);
        }

        // 当前节点在范围内，保留它
        // 递归修剪左右子树，并重新连接
        root->left = trimBST(root->left, low, high);
        root->right = trimBST(root->right, low, high);
        return root;
    }
};

// ============================================================
// 解法2: 迭代 — 三步走，O(1) 空间
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 递归版的三种情况对应迭代版的三步:
//
// Step1: 找到一个在 [low, high] 内的根节点
//   - 如果当前 root 太小，root = root->right (BST: 右边更大)
//   - 如果当前 root 太大，root = root->left  (BST: 左边更小)
//   - 循环直到 root 在范围内 或 root 为空
//
// Step2: 修剪左子树 (处理 val < low 的节点)
//   - root 已经 >= low，但左子树中可能有 < low 的
//   - BST性质: 左子树中如果某节点 < low，它的左子树更小，全扔
//   - 用它的右子树(值更大，可能合法)替换它
//
//   图示:
//     node             node
//      |                |
//     left(太小)  →   left->right(可能合法)
//      / \
//    更小 right
//   (全扔)
//
// Step3: 修剪右子树 (处理 val > high 的节点)
//   - 对称处理: 太大的节点用它的左子树替换
//
//   图示:
//     node             node
//      |                |
//     right(太大) →   right->left(可能合法)
//      / \
//    left  更大
//         (全扔)
// ============================================================
class Solution2 {
public:
    TreeNode* trimBST(TreeNode* root, int low, int high) {
        // Step1: 找到值在 [low, high] 内的新根
        while (root && (root->val < low || root->val > high)) {
            if (root->val < low) {
                root = root->right;   // 太小，往右跳（右边更大）
            } else {
                root = root->left;    // 太大，往左跳（左边更小）
            }
        }
        if (!root) return nullptr;

        // Step2: 从 root 出发，修剪左子树中 val < low 的节点
        // 此时 root->val >= low 已确认
        // 左子树中越界只可能是"太小"（因为左子树 <= root <= high）
        TreeNode* node = root;
        while (node->left) {
            if (node->left->val < low) {
                // 左孩子太小 → 它的左子树更小，全扔
                // 用左孩子的右子树替代（右子树值更大，可能合法）
                // 注意: 替换后不移动 node！新接上来的节点可能还是太小
                node->left = node->left->right;
            } else {
                // 左孩子合法，继续往左深入检查更深的节点
                node = node->left;
            }
        }

        // Step3: 从 root 出发，修剪右子树中 val > high 的节点
        // 此时 root->val <= high 已确认
        // 右子树中越界只可能是"太大"
        node = root;
        while (node->right) {
            if (node->right->val > high) {
                // 右孩子太大 → 它的右子树更大，全扔
                // 用右孩子的左子树替代（左子树值更小，可能合法）
                node->right = node->right->left;
            } else {
                // 右孩子合法，继续往右深入检查
                node = node->right;
            }
        }

        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 递归 (Solution1)      | 迭代 (Solution2)      |
// |------------|----------------------|----------------------|
// | 代码量     | ~10行，极简           | ~25行，三段逻辑       |
// | 空间       | O(h) 递归栈           | O(1)                 |
// | 可读性     | 高，自然递归           | 中，需理解三步逻辑    |
// | 面试推荐   | ⭐ 首选               | 追问O(1)空间时给出    |
//
// 面试建议: 先写递归，3分钟搞定。面试官追问空间时再给迭代版。
//
// 【易错点】
//
// 1. ✗ root->val < low 时返回 nullptr
//      → 这把右子树中的合法节点全丢了！
//    ✓ root->val < low 时返回 trimBST(root->right, low, high)
//      → 右子树中可能有在 [low, high] 范围内的节点
//
// 2. ✗ 递归时忘记 root->left = trimBST(root->left, ...)
//      只写了 trimBST(root->left, ...) 而没有赋值回来
//    ✓ 必须 root->left = trimBST(root->left, low, high)
//      → 修剪后的子树要重新连到父节点上
//
// 3. ✗ 迭代版 Step2 中替换后立即 node = node->left
//      新接上来的 node->left 可能仍然 < low，会遗漏
//    ✓ 替换后不移动 node，继续在 while 中检查新的 node->left
//
// 4. ✗ 迭代版 Step1 找到合法根后忘记判空
//      如果整棵树的值都不在范围内，root 会变成 nullptr
//    ✓ Step1 循环结束后加 if (!root) return nullptr;
//
// 【面试追问】
//
// Q1: 为什么 root->val < low 时可以直接丢弃整个左子树？
// A1: BST 性质保证左子树所有值 < root->val < low，
//     全部不在 [low, high] 范围内，一定要删除。
//     但右子树值 > root->val，可能有 >= low 的合法节点。
//
// Q2: 能否 O(1) 空间完成？
// A2: 可以，用迭代版 (Solution2)。三步走：
//     找合法根 → 修剪左子树越界节点 → 修剪右子树越界节点。
//     每一步都利用 BST 性质做指针替换，不需要额外空间。
//
// Q3: 如果是普通二叉树（不是 BST），怎么修剪？
// A3: 不能利用"跳过整棵子树"的优化。需要遍历每个节点，
//     合法节点保留并递归修剪子树，不合法节点的处理更复杂——
//     它可能左右子树都有合法节点，需要合并两棵子树。
//     时间仍是 O(n)，但逻辑复杂得多。
//
// Q4: 与 LeetCode 450 "删除BST中的节点" 区别？
// A4: 450 删除单个节点，难点在"被删节点有两个孩子"时需要找
//     中序后继来替代。本题虽然删多个节点，但因为 BST 的连续
//     范围性质，越界节点一定和它的某侧子树一起越界，所以反而
//     只需用另一侧子树替换，不需要找后继，代码更简单。
// ============================================================
