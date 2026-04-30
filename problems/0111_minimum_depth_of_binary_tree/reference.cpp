/*
 * LeetCode 111: 二叉树的最小深度 (Minimum Depth of Binary Tree)
 *
 * 【题目本质】
 * 求根节点到最近叶子节点的路径上的节点数。
 * 陷阱：只有一个子树为空的节点不是叶子，不能简单 min(left, right)。
 *
 * 【解法总览】
 * 解法1: DFS 递归 — O(n) / O(h) — 最简洁，面试首选
 * 解法2: BFS 层序 — O(n) / O(w) — 可提前终止，面试首选
 * 解法3: DFS 迭代 — O(n) / O(h) — 栈模拟，展示迭代能力
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

// ============================================================
// 解法1: DFS 递归 — 自底向上求最小深度
// 时间: O(n)  空间: O(h)  h为树高，最坏O(n)
//
// 【思路】
// 和104题"最大深度"的递归框架几乎一样，但有一个关键区别：
// 最大深度可以直接 max(left, right) + 1，因为空子树返回0不影响max。
// 最小深度不行！因为 min(0, x) = 0，但空子树不代表"到叶子的路径"。
//
// 所以必须特判：
// - 左子树为空 → 最小深度在右子树
// - 右子树为空 → 最小深度在左子树
// - 都不为空   → 取两边的 min
//
// 【树的示例图】
//
//  示例1:                      示例2 (链状，本题陷阱):
//       3                           2
//      / \                           \
//     9   20                          3
//        /  \                          \
//       15   7                          4
//                                        \
//  递归过程:                               5
//  minDepth(9) = 1 (叶子)                   \
//  minDepth(15) = 1 (叶子)                   6
//  minDepth(7) = 1 (叶子)
//  minDepth(20) = 1+min(1,1) = 2      节点2: 左空! 不能取min(0, 4)=0
//  minDepth(3) = 1+min(1,2) = 2       必须走右子树: 1 + minDepth(3) = 5
// ============================================================
class Solution1 {
public:
    int minDepth(TreeNode* root) {
        // 空节点返回0
        if (!root) return 0;
        
        int left = minDepth(root->left);
        int right = minDepth(root->right);
        
        // 核心区别：特判单侧子树为空的情况
        // 左子树为空 → 根节点不是叶子，只能走右子树
        if (!root->left)  return 1 + right;
        // 右子树为空 → 根节点不是叶子，只能走左子树
        if (!root->right) return 1 + left;
        
        // 两边都有子树 → 取较小的
        return 1 + min(left, right);
    }
};

// ============================================================
// 解法2: BFS 层序遍历 — 第一个叶子所在层就是答案
// 时间: O(n) 最坏  空间: O(w)  w为树的最大宽度
//
// 【思路】
// DFS 必须遍历完整棵树才能确定全局最小。
// BFS 逐层扫描，有一个天然优势：第一个遇到的叶子节点
// 一定在最浅的那一层，直接返回当前层数即可。
//
// 这正是 BFS 适合"最短路径/最少步数"问题的原因。
//
// 【BFS 过程示例】
//       3
//      / \
//     9   20
//        /  \
//       15   7
//
// Level 1: 队列=[3]      → 3有两个孩子，不是叶子
// Level 2: 队列=[9, 20]  → 9没有孩子，是叶子! → 返回 2
// （无需访问 Level 3）
// ============================================================
class Solution2 {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        
        queue<TreeNode*> q;
        q.push(root);
        int depth = 0;
        
        while (!q.empty()) {
            depth++;
            int size = q.size();
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 叶子节点 = 左右都为空
                // 因为是逐层遍历，第一个叶子就是最浅的
                if (!node->left && !node->right) {
                    return depth;
                }
                
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        
        return depth;
    }
};

// ============================================================
// 解法3: DFS 迭代（栈模拟）— 带剪枝
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 用显式栈模拟 DFS 遍历，每个栈元素记录 {节点, 当前深度}。
// 遇到叶子节点时更新全局 minDep。
// 剪枝：如果当前深度已经 >= minDep，不再入栈其子节点。
//
// 这种方式展示了"用栈模拟递归"的通用技巧。
// ============================================================
class Solution3 {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        
        stack<pair<TreeNode*, int>> stk;
        stk.push({root, 1});
        int minDep = INT_MAX;
        
        while (!stk.empty()) {
            auto [node, depth] = stk.top();
            stk.pop();
            
            // 叶子节点：更新最小深度
            if (!node->left && !node->right) {
                minDep = min(minDep, depth);
                continue;
            }
            
            // 剪枝：当前深度+1 已经 >= 已知最小深度，无需继续
            if (node->right && depth + 1 < minDep) {
                stk.push({node->right, depth + 1});
            }
            if (node->left && depth + 1 < minDep) {
                stk.push({node->left, depth + 1});
            }
        }
        
        return minDep;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 能否提前终止 | 代码复杂度 |
// |------|------|------|-------------|-----------|
// | DFS递归 | O(n) | O(h) | 不能，必须遍历全树 | 最简洁 |
// | BFS层序 | O(n) | O(w) | 能，第一个叶子即停 | 中等 |
// | DFS迭代 | O(n) | O(h) | 有限剪枝 | 较复杂 |
//
// 面试推荐：先写 DFS 递归展示递归思维，再聊 BFS 的优势（提前终止）。
//
// 【与104题"最大深度"的代码差异】
// 104题: return 1 + max(left, right);  // 一行搞定
// 111题: 需要额外两行 if 判断单侧子树为空的情况
//        if (!root->left)  return 1 + right;
//        if (!root->right) return 1 + left;
//        return 1 + min(left, right);
// 原因: max(0, x) = x (正确), 但 min(0, x) = 0 (错误,空子树不是叶子路径)
//
// 【易错点】
// 1. ✗ 直接 return 1 + min(minDepth(left), minDepth(right));
//    → 示例2中根节点左空，min(0,4)=0，返回1，但正确答案是5
//    ✓ 必须特判: if (!root->left) return 1 + right;
//
// 2. ✗ BFS中判断叶子只判断 !node->left (不充分)
//    ✓ 叶子 = !node->left && !node->right (左右都为空)
//
// 3. ✗ 忘记处理 root == nullptr 的边界
//    ✓ 函数开头 if (!root) return 0;
//
// 【面试追问】
// Q1: 和104题(最大深度)的区别？
//     → max不受空子树影响，min会被空子树误导。核心差异是两行if特判。
//
// Q2: DFS遍历整棵树，BFS能提前终止。什么情况下差异最大？
//     → 当树很宽、最浅叶子在左上角时，BFS只访问前几层就返回。
//        DFS即使叶子在第2层也要递归遍历右子树的所有深层节点。
//
// Q3: 如果树极度不平衡(退化为链表)，两种方法哪个更好？
//     → 链表形状下叶子在最深处，BFS也无法提前终止，两者都是O(n)。
//        空间上DFS是O(n)栈深度，BFS每层只有1个节点所以O(1)，BFS更优。
//
// 【相关题型】
// - 104. 最大深度 — 同框架，区别: max不需特判空子树，本题min需要
// - 110. 平衡二叉树 — 复用自底向上求高度，额外判断高度差
// - 102. 层序遍历 — BFS模板完全一致，本题加叶子判断提前返回
// - 559. N叉树最大深度 — 推广到多叉树，遍历children数组
// ============================================================
