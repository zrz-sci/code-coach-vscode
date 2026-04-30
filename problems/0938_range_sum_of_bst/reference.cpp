/*
 * LeetCode 938: 二叉搜索树的范围和 (Range Sum of BST)
 *
 * 【题目本质】
 * 在 BST 中找出所有值在 [low, high] 范围内的节点，求它们的值之和。
 * 核心：利用 BST 的有序性进行剪枝，跳过不可能包含目标值的子树。
 *
 * 【解法总览】
 * 解法1: 暴力 DFS（不利用 BST）   — O(n) / O(h) — 最直觉
 * 解法2: BST 剪枝递归             — O(n)最坏 / O(h) — 面试首选
 * 解法3: BST 剪枝迭代（栈）       — O(n)最坏 / O(h) — 追问时给出
 */

// ============================================================
// 解法1: 暴力 DFS — 遍历所有节点，逐个判断
// 时间: O(n)  空间: O(h)  h为树高
//
// 【思路】
// 把这道题当普通二叉树来做：遍历每个节点，值在范围内就加。
// 完全没有利用 BST 性质，所有节点都会被访问一遍。
// 这是起点，用来对比剪枝版的优势。
//
//           10          low=7, high=15
//          /  \
//         5    15       
//        / \     \
//       3   7    18
//
// 访问顺序: 10→5→3→7→15→18 (全部6个节点)
// 判断: 10✓ 5✗ 3✗ 7✓ 15✓ 18✗ → 10+7+15=32
// ============================================================
class Solution1 {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        
        int sum = 0;
        // 当前节点在范围内就累加
        if (root->val >= low && root->val <= high) {
            sum += root->val;
        }
        // 无论如何都遍历左右子树 — 这就是"暴力"的体现
        sum += rangeSumBST(root->left, low, high);
        sum += rangeSumBST(root->right, low, high);
        
        return sum;
    }
};

// ============================================================
// 解法2: BST 剪枝递归 — 利用 BST 有序性跳过无用子树 ⭐ 面试首选
// 时间: O(n) 最坏  空间: O(h)
//
// 【思路】
// 解法1的瓶颈：遍历了所有节点，包括不可能在范围内的。
// BST 性质：左子树所有值 < 根 < 右子树所有值。
// 因此：
//   - root->val < low  → root 和它的整个左子树都 < low，全部跳过
//   - root->val > high → root 和它的整个右子树都 > high，全部跳过
//   - low ≤ root->val ≤ high → 累加当前值，左右都要搜
//
// 剪枝过程:
//           10          [low=7, high=15]
//          /  \
//         5    15       
//        / \     \
//       3   7    18
//
// 递归(10): 7 ≤ 10 ≤ 15 → 累加10, 搜左+右
//   递归(5):  5 < 7 → 跳过左子树(3不访问), 只搜右
//     递归(7):  7 ≤ 7 ≤ 15 → 累加7, 左null+右null
//   递归(15): 7 ≤ 15 ≤ 15 → 累加15, 搜左(null)+右
//     递归(18): 18 > 15 → 跳过右子树, 搜左(null)
//
// 总共访问: 10, 5, 7, 15, 18 = 5个节点 (跳过了3!)
// ============================================================
class Solution2 {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        
        // 当前值太小 → 整个左子树更小，全部不要，只搜右边
        if (root->val < low) {
            return rangeSumBST(root->right, low, high);
        }
        // 当前值太大 → 整个右子树更大，全部不要，只搜左边
        if (root->val > high) {
            return rangeSumBST(root->left, low, high);
        }
        // 当前值在 [low, high] → 当前值 + 左子树贡献 + 右子树贡献
        return root->val
             + rangeSumBST(root->left, low, high)
             + rangeSumBST(root->right, low, high);
    }
};

// ============================================================
// 解法3: BST 剪枝迭代（显式栈） — 用栈模拟递归
// 时间: O(n) 最坏  空间: O(h)
//
// 【思路】
// 面试追问"能否不用递归"时给出。
// 核心剪枝逻辑和解法2完全一致，只是用显式栈代替递归调用栈。
// 好处：不会因为树极深而栈溢出。
//
// 栈的模拟过程 (示例1):
// 初始: stk=[10]
// Pop 10: 7≤10≤15 → sum+=10, push(5), push(15)  stk=[5,15]
// Pop 15: 7≤15≤15 → sum+=15, push(null), push(18) stk=[5,null,18]
// Pop 18: 18>15   → push(null)  stk=[5,null,null]
// Pop null: skip   stk=[5,null]
// Pop null: skip   stk=[5]
// Pop 5:  5<7     → push(7)    stk=[7]
// Pop 7:  7≤7≤15  → sum+=7, push(null), push(null) stk=[null,null]
// Pop null: skip   Pop null: skip → 栈空，结束
// sum = 10 + 15 + 7 = 32 ✓
// ============================================================
class Solution3 {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        
        int sum = 0;
        stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            
            // push 进来的可能是 null（叶子节点的孩子），必须跳过
            if (!node) continue;
            
            if (node->val < low) {
                // 当前值太小，只搜右子树（值更大的方向）
                stk.push(node->right);
            } else if (node->val > high) {
                // 当前值太大，只搜左子树（值更小的方向）
                stk.push(node->left);
            } else {
                // 在范围内：累加 + 左右都搜
                sum += node->val;
                stk.push(node->left);
                stk.push(node->right);
            }
        }
        
        return sum;
    }
};

// ============================================================
// 【解法对比】
//
// | 特性           | 暴力 DFS    | BST剪枝递归  | BST剪枝迭代 |
// |----------------|-------------|-------------|-------------|
// | 利用 BST 性质  | ✗           | ✓           | ✓           |
// | 平均访问节点数 | n           | << n        | << n        |
// | 实现复杂度     | 最简单      | 简单        | 中等        |
// | 栈溢出风险     | 树极深时有  | 树极深时有  | 无(显式栈)  |
// | 面试推荐       | 作为起点    | ⭐首选      | 追问时给出  |
//
// 解法1 vs 解法2: 唯一区别是"是否根据 BST 性质决定搜索方向"
//   解法1 无论如何都搜左右子树
//   解法2 根据 val 与 low/high 的关系跳过不需要的子树
// 解法2 vs 解法3: 逻辑完全一致，只是递归 vs 迭代的实现差异
//
// ============================================================
// 【易错点】
//
// 1. 剪枝方向写反:
//    ✗: if (root->val < low) return rangeSumBST(root->left, low, high);
//    ✓: if (root->val < low) return rangeSumBST(root->right, low, high);
//    原因: val < low，需要往更大的方向搜，即右子树
//
// 2. 迭代版不检查 null:
//    ✗: TreeNode* node = stk.top(); stk.pop(); sum += node->val; // 空指针!
//    ✓: if (!node) continue; // push进来的可能是null
//
// 3. 范围判断用 < 而不是 <=:
//    ✗: if (root->val < low || root->val > high) // 漏掉了等于边界的情况
//    解法2的写法天然正确: < low 和 > high 的分支不累加，
//    else 分支包含了 == low 和 == high 的情况
//
// ============================================================
// 【面试追问】
//
// Q1(基础): 如果不是 BST 而是普通二叉树，怎么做？
//    → 只能遍历所有节点逐个判断，O(n)，无法剪枝。
//
// Q2(复杂度): 剪枝后最好和最坏时间复杂度？
//    → 最好 O(log n): [low,high] 很窄时只沿一条路径走到底
//      最坏 O(n): [low,high] 覆盖整棵树时退化为全遍历
//
// Q3(变体): 如果需要多次查询不同的 [low, high]？
//    → 预处理: 中序遍历 BST 得有序数组 + 前缀和
//      每次查询: 二分找 low 和 high 的位置，O(log n)
//      预处理 O(n)，每次查询 O(log n)
//
// Q4(变体): 如果要求范围内节点的个数而不是和？
//    → 把 sum += root->val 改成 count += 1，剪枝逻辑不变
// ============================================================
