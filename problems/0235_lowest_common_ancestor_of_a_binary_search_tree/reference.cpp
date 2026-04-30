/*
 * LeetCode 235: 二叉搜索树的最近公共祖先
 * (Lowest Common Ancestor of a Binary Search Tree)
 *
 * 【题目本质】
 * 利用 BST 的有序性，沿树向下走，找到第一个让 p 和 q "分叉"的节点。
 * 分叉 = p 和 q 分别在当前节点的两侧，或当前节点本身就是 p/q。
 *
 * 【解法总览】
 * 解法1: 通用 LCA（后序遍历）       — O(n) / O(n)  — 不利用 BST 性质
 * 解法2: BST 递归（利用有序性）      — O(h) / O(h)  — 利用 BST 导航
 * 解法3: BST 迭代（利用有序性）      — O(h) / O(1)  — 最优，面试首选
 *
 * 【BST 示意图】
 *
 *           6
 *          / \
 *         2   8
 *        / \ / \
 *       0  4 7  9
 *         / \
 *        3   5
 *
 *  示例1: p=2, q=8
 *    cur=6: 2<6 且 8>6 → 分叉! → 返回 6
 *
 *  示例2: p=2, q=4
 *    cur=6: 2<6 且 4<6 → 都在左边 → 往左走
 *    cur=2: 2≤2 且 4>2 → 分叉(2就是p)! → 返回 2
 */

// Definition for a binary tree node.
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
// };

// ============================================================
// 解法1: 通用 LCA（后序遍历，不利用 BST 性质）
// 时间: O(n)  空间: O(n) — 最坏递归深度为 n（退化链表）
//
// 【思路】
// 这是 LeetCode 236（普通二叉树 LCA）的解法。
// 核心想法：后序遍历，对每个节点问——p 和 q 是否分别在我的左右子树中？
// - 如果 root 本身就是 p 或 q，直接返回 root（自己是自己的祖先）
// - 递归在左子树找、右子树找
// - 如果左右都找到了 → root 就是 LCA（分居两侧）
// - 如果只有一侧找到了 → LCA 在那一侧（两个节点在同侧）
//
// 为什么放在这里？面试官可能追问"如果不是 BST 怎么办"，
// 这就是回退方案。
//
// 【遍历过程示意 — p=2, q=8】
//
//           6            ← 最终: left=2, right=8, 都非空 → 返回6
//          / \
//    left=2   right=8    ← 节点2: root==p → 直接返回2
//        / \ / \            节点8: root==q → 直接返回8
//       0  4 7  9
// ============================================================
class Solution1 {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 终止条件：空节点，或者当前节点就是 p 或 q
        // 为什么找到 p/q 就返回？因为如果 p 是 q 的祖先，
        // 我们不需要继续往下找 q，p 本身就是 LCA
        if (!root || root == p || root == q) return root;

        // 后序遍历：先递归左右子树
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // 如果左右子树各找到一个 → p 和 q 分居两侧 → root 是 LCA
        if (left && right) return root;

        // 否则两个都在同一侧，返回非空的那个
        return left ? left : right;
    }
};

// ============================================================
// 解法2: BST 递归 — 利用有序性只走一条路径
// 时间: O(h)  空间: O(h) — 递归栈深度为树高
//
// 【思路】
// 解法1 遍历了整棵树，但 BST 的有序性告诉我们：
//   - p, q 的值都 < root->val → 它们一定都在左子树
//   - p, q 的值都 > root->val → 它们一定都在右子树
//   - 否则（一左一右，或其中一个等于 root）→ root 就是 LCA
//
// 每次只递归一个方向，时间从 O(n) 降到 O(h)。
// 这是尾递归，因为每次函数的最后一步就是递归调用。
//
// 【递归路径示意 — p=2, q=4】
//
//   lowestCommonAncestor(6, 2, 4)
//     2<6 且 4<6 → 都在左边
//     ↓
//   lowestCommonAncestor(2, 2, 4)
//     2==2(不满足都<2), 4>2(不满足都>2) → 分叉! → 返回 2
// ============================================================
class Solution2 {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // p 和 q 都比 root 小 → LCA 在左子树
        if (p->val < root->val && q->val < root->val) {
            return lowestCommonAncestor(root->left, p, q);
        }
        // p 和 q 都比 root 大 → LCA 在右子树
        if (p->val > root->val && q->val > root->val) {
            return lowestCommonAncestor(root->right, p, q);
        }
        // 分叉了：一个在左一个在右，或者 root 就是 p/q
        // 这就是 LCA
        return root;
    }
};

// ============================================================
// 解法3: BST 迭代 — O(h) / O(1) ⭐ 面试首选
//
// 【思路】
// 解法2 是尾递归（每次只递归一个方向，且递归调用是最后一步），
// 尾递归可以直接改写为 while 循环，省去递归栈开销。
//
// 为什么迭代更好？
// 1. 空间从 O(h) 降到 O(1)
// 2. 避免极端情况下（退化链表，h=n）的栈溢出风险
// 3. 代码同样简洁
//
// 【迭代过程示意 — p=2, q=8】
//
//   cur = 6
//   Round1: p=2 < 6, q=8 > 6 → 不满足"都在左"也不满足"都在右"
//           → 分叉! → 返回 cur=6
//
// 【迭代过程示意 — p=2, q=4】
//
//   cur = 6
//   Round1: p=2 < 6 且 q=4 < 6 → 都在左边 → cur = cur->left = 2
//   Round2: p=2 == 2 (不满足 <2), q=4 > 2 (不满足"都>2" 因为 p==2)
//           → 分叉! → 返回 cur=2
//
// 【迭代过程示意 — p=3, q=5】
//
//   cur = 6
//   Round1: 3<6 且 5<6 → 都在左边 → cur = 2
//   Round2: 3>2 且 5>2 → 都在右边 → cur = 4
//   Round3: 3<4 且 5>4 → 分叉! → 返回 cur=4
// ============================================================
class Solution3 {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        TreeNode* cur = root;
        while (cur) {
            if (p->val < cur->val && q->val < cur->val) {
                // 两个节点都在左子树，往左走
                cur = cur->left;
            } else if (p->val > cur->val && q->val > cur->val) {
                // 两个节点都在右子树，往右走
                cur = cur->right;
            } else {
                // 分叉了：
                //   情况A: p 在左, q 在右 (或反过来)
                //   情况B: cur 就是 p, q 在 cur 的子树中 (或反过来)
                // 无论哪种情况，cur 都是最近公共祖先
                return cur;
            }
        }
        // 题目保证 p、q 都存在于 BST 中，不会走到这里
        return nullptr;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 利用BST | 适用场景 |
// |------|------|------|---------|----------|
// | 解法1 通用LCA | O(n) | O(n) | 否 | 任何二叉树(236题) |
// | 解法2 BST递归 | O(h) | O(h) | 是 | BST，代码清晰 |
// | 解法3 BST迭代 | O(h) | O(1) | 是 | BST，面试首选 |
//
// 解法1 vs 解法2/3:
//   解法1 把它当普通二叉树，需要遍历整棵树
//   解法2/3 利用 BST 有序性，每次只走一个方向，像二分查找一样
//
// 解法2 vs 解法3:
//   逻辑完全相同，解法3 把尾递归改成 while 循环
//   解法3 空间从 O(h) 降到 O(1)，是最优解
//
// ============================================================
// 【易错点】
//
// 1. 用 <= 和 >= 而不是 < 和 >:
//    ✗ if (p->val <= cur->val && q->val <= cur->val) cur = cur->left;
//      → 当 p->val == cur->val 时错误地往左走了，但 cur 本身就是 LCA
//    ✓ if (p->val < cur->val && q->val < cur->val) cur = cur->left;
//      → 等于的情况落入 else 分支，正确返回 cur
//
// 2. 通用LCA(解法1) 终止条件漏掉 root==p || root==q:
//    ✗ if (!root) return root;
//      → 当 p 是 q 的祖先时，不会在 p 处停下来，继续递归后返回错误结果
//    ✓ if (!root || root == p || root == q) return root;
//
// 3. 迭代解法中 while 条件写成 while(cur != nullptr) 是正确的，
//    但如果写成 while(true) 也没问题（因为题目保证一定有答案）。
//    关键是不要在循环外忘记 return nullptr 导致编译警告。
//
// ============================================================
// 【面试追问】
//
// Q1: 你的解法利用了 BST 的什么性质？如果是普通二叉树怎么办？
//   → BST 有序性让我们判断 p、q 在 root 的哪一侧，只走一条路径 O(h)。
//     普通二叉树必须后序遍历，O(n) 时间，就是解法1 / LeetCode 236。
//
// Q2: 如果 p 或 q 不一定存在于树中，你的代码需要怎么改？
//   → 解法3 不能直接用了（它假设 p、q 一定存在）。
//     需要先做两次 BST 查找验证 p、q 都存在，再找 LCA。
//     或者用解法1 的变体：不提前返回，用 flag 记录是否真正找到了两个节点。
//     对应 LeetCode 1644（Premium）。
//
// Q3: 如果树非常大，需要频繁查询不同的 (p,q) 对，怎么优化？
//   → 对于 BST：每次查询 O(h)，平衡 BST 就是 O(log n)，通常足够。
//     对于一般二叉树：可以用 Euler Tour + Sparse Table 预处理 O(n)，
//     之后每次查询 O(1)。或者用 Binary Lifting（倍增法），
//     预处理 O(n log n)，每次查询 O(log n)。
//
// ============================================================
// 【相关题型】
//
// 236. 二叉树的最近公共祖先
//   → 本题的"通用版"，不能用 BST 性质，必须用后序遍历（本题解法1）
//   → 区别：236 无法判断 p、q 在哪一侧，必须左右都搜索
//
// 700. 二叉搜索树中的搜索
//   → 复用同样的"BST 导航"（小了往左，大了往右），但只找一个目标
//   → 本题相当于同时"导航" p 和 q，找到分叉点
//
// 1644. 二叉树的最近公共祖先 II（Premium）
//   → 变体：p 或 q 可能不存在，需要额外验证
//
// 1650. 二叉树的最近公共祖先 III（Premium）
//   → 每个节点有 parent 指针，转化为"两个链表求交点"（类似 160 题）
// ============================================================
