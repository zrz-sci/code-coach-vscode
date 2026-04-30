/*
 * LeetCode 110: 平衡二叉树 (Balanced Binary Tree)
 *
 * 【题目本质】
 * 判断一棵二叉树中，每个节点的左右子树高度差是否都不超过 1。
 * 核心在于如何高效获取每个节点的子树高度信息。
 *
 * 【解法总览】
 * 解法1: 自顶向下（暴力） — O(n log n)~O(n²) / O(n) — 最直觉
 * 解法2: 自底向上（最优） — O(n) / O(n) — 面试首选 ⭐
 */

// ============================================================
// 解法1: 自顶向下（暴力）
// 时间: O(n log n) 平衡树 / O(n²) 倾斜树    空间: O(n) 递归栈
//
// 【思路】
// 最直觉的翻译：对每个节点分别算左右子树高度，比较差值，
// 然后递归验证左右子树也满足条件。
//
// 瓶颈：height() 对同一个节点被多层 isBalanced() 反复调用。
//
//         3               isBalanced(3): height(9)遍历左子树, height(20)遍历右子树
//        / \              isBalanced(9): height(NULL)+height(NULL)
//       9   20            isBalanced(20): height(15)+height(7)
//           / \           → 节点15和7在 isBalanced(3) 时算了一遍，
//          15   7            在 isBalanced(20) 时又算了一遍
// ============================================================
class Solution1 {
public:
    // 求以 root 为根的树的高度（即 LeetCode 104 的解法）
    int height(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(height(root->left), height(root->right));
    }
    
    bool isBalanced(TreeNode* root) {
        if (!root) return true;
        
        // 当前节点的左右子树高度差 ≤ 1
        int leftH = height(root->left);
        int rightH = height(root->right);
        if (abs(leftH - rightH) > 1) return false;
        
        // 递归检查：左右子树各自也必须平衡
        return isBalanced(root->left) && isBalanced(root->right);
    }
};


// ============================================================
// 解法2: 自底向上（最优）⭐ 面试首选
// 时间: O(n)    空间: O(n) 递归栈
//
// 【思路】
// 解法1的瓶颈：height() 和 isBalanced() 是分开的，导致重复遍历。
// 优化：把两者合二为一。
//
// 设计一个函数 checkHeight(node)：
//   - 如果 node 的子树平衡 → 返回真实高度 (>= 0)
//   - 如果 node 的子树不平衡 → 返回 -1 (作为标记)
//
// 用后序遍历（先左右再自己），保证处理当前节点时
// 左右子树的信息已经算好，不需要重复计算。
//
// 执行过程（示例1）:
//         3
//        / \
//       9   20
//           / \
//          15   7
//
// checkHeight(9):
//   left=0, right=0, |0-0|=0 ≤ 1 → return 1
// checkHeight(15):
//   left=0, right=0, |0-0|=0 ≤ 1 → return 1
// checkHeight(7):
//   left=0, right=0, |0-0|=0 ≤ 1 → return 1
// checkHeight(20):
//   left=1, right=1, |1-1|=0 ≤ 1 → return 2
// checkHeight(3):
//   left=1, right=2, |1-2|=1 ≤ 1 → return 3
// 3 != -1 → true
//
// 执行过程（示例2 不平衡）:
//         1
//        / \
//       2   2
//      / \
//     3   3
//    / \
//   4   4
//
// checkHeight(4左)=1, checkHeight(4右)=1
// checkHeight(3左): left=1,right=1 → return 2
// checkHeight(3右): return 1
// checkHeight(2左): left=2,right=1, |2-1|=1 → return 3
// checkHeight(2右): return 1
// checkHeight(1): left=3,right=1, |3-1|=2 > 1 → return -1 !!!
// -1 == -1 → false
// ============================================================
class Solution2 {
public:
    // 返回值: >= 0 表示子树高度（平衡）; -1 表示子树不平衡
    int checkHeight(TreeNode* root) {
        if (!root) return 0;  // 空节点高度为 0，且平衡
        
        // 后序遍历：先算左子树
        int leftH = checkHeight(root->left);
        if (leftH == -1) return -1;   // 提前终止：左子树已不平衡，无需继续
        
        // 再算右子树
        int rightH = checkHeight(root->right);
        if (rightH == -1) return -1;  // 提前终止：右子树已不平衡
        
        // 当前节点：判断左右高度差
        if (abs(leftH - rightH) > 1) return -1;  // 当前节点不平衡
        
        // 当前子树平衡，返回高度
        return 1 + max(leftH, rightH);
    }
    
    bool isBalanced(TreeNode* root) {
        return checkHeight(root) != -1;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度       | 解法1: 自顶向下      | 解法2: 自底向上      |
// |------------|----------------------|----------------------|
// | 遍历方向   | 根→叶, 每个节点重新算 | 叶→根, 信息一次汇聚  |
// | 时间       | O(n log n)~O(n²)     | O(n)                 |
// | 空间       | O(n)                 | O(n)                 |
// | 核心技巧   | height+isBalanced分离 | 用-1标记合二为一      |
// | 面试推荐   | 说思路即可           | 必须写出 ⭐           |
//
// 为什么解法2更优？
// 解法1中 height() 在外层 isBalanced() 的每一层递归都被重新调用，
// 导致底层节点被反复访问。解法2用后序遍历，每个节点只访问一次，
// 高度信息从叶子向根逐层传递，不存在重复计算。
//
// ============================================================
// 【易错点】
//
// 1. ✗ 只检查根节点的高度差:
//      return abs(height(left) - height(right)) <= 1;
//    ✓ 必须递归检查每个节点都满足, 不仅仅是根。
//
// 2. ✗ checkHeight 中不提前终止:
//      int leftH = checkHeight(root->left);
//      int rightH = checkHeight(root->right);  // leftH==-1 也继续
//    ✓ 应在 leftH==-1 时立即 return -1, 避免无意义递归。
//    (不影响正确性但影响效率, 面试官可能追问)
//
// 3. ✗ 空树返回 false:
//    ✓ 空树是平衡的, 应返回 true。
//
// 4. ✗ 空节点的 height 返回 -1 (用边计数的习惯):
//    ✓ 这里 -1 是"不平衡"标记, 空节点应返回 0, 避免歧义。
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力解的时间复杂度为什么不是 O(n)?
// A1: height() 对同一节点被多层 isBalanced() 反复调用。
//     倾斜树最坏: O(n) + O(n-1) + ... + O(1) = O(n²)。
//     平衡树: 每层 O(n), 共 log n 层 → O(n log n)。
//
// Q2: 能否用迭代实现?
// A2: 可以。用后序遍历的迭代版(双栈或标记法) +
//     unordered_map<TreeNode*, int> 存每个节点高度。
//     后序保证处理当前节点时子节点高度已知。代码较复杂。
//
// Q3: 如果要返回"最大不平衡度"(所有节点高度差的最大值), 怎么改?
// A3: 不用 -1 提前终止, 维护全局 maxDiff,
//     每次 maxDiff = max(maxDiff, abs(leftH - rightH)),
//     函数始终返回真实高度, 最后返回 maxDiff。
//
// 【相关题型】
// - 104. 二叉树的最大深度 — 本题的 height() 就是 104 的解法
// - 543. 二叉树的直径 — 复用"自底向上算高度"框架,
//   区别: 543 维护 leftH+rightH 最大值, 而非判断差≤1
// - 100. 相同的树 — 同样的递归判断框架练习
// ============================================================
