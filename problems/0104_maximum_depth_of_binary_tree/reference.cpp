/*
 * LeetCode 104: Maximum Depth of Binary Tree
 *
 * 【题目本质】
 * 求从根节点到最远叶子节点的路径上的节点数。
 * 即 depth(root) = max(depth(left), depth(right)) + 1。
 *
 * 【解法总览】
 * 解法1: DFS 递归（后序遍历）     — O(n) / O(h) — 面试首选，最简洁
 * 解法2: BFS 层序遍历            — O(n) / O(w) — 直觉清晰，数层数
 * 解法3: DFS 迭代（显式栈）       — O(n) / O(h) — 练习栈模拟递归
 *
 * 其中 n=节点数, h=树高(最坏n), w=最大宽度(最坏n/2)
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
// 解法1: DFS 递归（后序遍历，自底向上）
// 时间: O(n)  空间: O(h) — h为树高，即递归调用栈深度
//
// 【思路】
// 树是递归结构，一棵树的深度可以用子树的深度来表示：
//   depth(root) = max(depth(left), depth(right)) + 1
// 这是后序遍历：先递归处理左右子树拿到深度，再合并。
//
// 【递归三要素】
// 1. 返回值: int — 以当前节点为根的子树的最大深度
// 2. 终止条件: root == nullptr → 返回 0（空树深度为0）
// 3. 单层逻辑: max(左深度, 右深度) + 1
//
// 【递归过程可视化】
//       3
//      / \
//     9   20
//        /  \
//       15   7
//
// maxDepth(3):
//   ├─ maxDepth(9):
//   │    ├─ maxDepth(NULL) → 0
//   │    └─ maxDepth(NULL) → 0
//   │    → max(0, 0) + 1 = 1
//   └─ maxDepth(20):
//        ├─ maxDepth(15) → max(0,0)+1 = 1
//        └─ maxDepth(7)  → max(0,0)+1 = 1
//        → max(1, 1) + 1 = 2
//   → max(1, 2) + 1 = 3 ✓
// ============================================================
class Solution1 {
public:
    int maxDepth(TreeNode* root) {
        // 终止条件：空节点深度为 0
        if (!root) return 0;
        
        // 后序遍历：先算左右子树深度
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);
        
        // 当前树深度 = 较深的子树 + 自身这一层
        return max(leftDepth, rightDepth) + 1;
    }
};

// ============================================================
// 解法2: BFS 层序遍历
// 时间: O(n)  空间: O(w) — w为树的最大宽度，完全二叉树约 n/2
//
// 【思路】
// 最大深度 = 树有多少层。
// BFS 天然按层遍历——用队列逐层处理，每处理完一层 depth++。
// 当队列为空时，depth 就是最大深度。
//
// 【BFS 逐层处理过程】
//       3
//      / \
//     9   20
//        /  \
//       15   7
//
// 队列: [3]           → 处理第1层 → depth=1, 队列变为[9,20]
// 队列: [9,20]        → 处理第2层 → depth=2, 队列变为[15,7]
// 队列: [15,7]        → 处理第3层 → depth=3, 队列变为[]
// 队列为空 → 返回 3
// ============================================================
class Solution2 {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        
        queue<TreeNode*> q;
        q.push(root);
        int depth = 0;
        
        while (!q.empty()) {
            // 关键: 用 levelSize 锁定当前层的节点数
            // 这样内层 for 只处理当前层，不会混入下一层
            int levelSize = q.size();
            
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                // 将子节点（下一层）加入队列
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            // 一整层处理完毕，深度+1
            depth++;
        }
        
        return depth;
    }
};

// ============================================================
// 解法3: DFS 迭代（显式栈）
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 解法1 的递归本质上是系统帮你维护了一个调用栈。
// 我们可以自己用显式栈来模拟，栈中存 (节点, 该节点的深度)。
// 遍历过程中维护一个全局最大深度。
//
// 【为什么需要存深度？】
// 递归版中，深度信息隐含在调用层数中。
// 迭代版没有这个隐含信息，所以必须显式记录每个节点的深度。
//
// 【迭代过程】
//       3
//      / \
//     9   20
//        /  \
//       15   7
//
// 栈: [(3,1)]                         maxD=0
// 弹出(3,1), maxD=1, 压入(20,2),(9,2)  栈:[(20,2),(9,2)]
// 弹出(9,2), maxD=2, 无子节点           栈:[(20,2)]
// 弹出(20,2), maxD=2, 压入(7,3),(15,3)  栈:[(7,3),(15,3)]
// 弹出(15,3), maxD=3                   栈:[(7,3)]
// 弹出(7,3), maxD=3                    栈:[]
// 返回 3 ✓
// ============================================================
class Solution3 {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        
        // 栈中存储 (节点指针, 该节点所在的深度)
        stack<pair<TreeNode*, int>> stk;
        stk.push({root, 1});
        int maxD = 0;
        
        while (!stk.empty()) {
            auto [node, depth] = stk.top();
            stk.pop();
            
            // 每次弹出都尝试更新最大深度
            maxD = max(maxD, depth);
            
            // 先压右再压左 → 左边先被弹出（前序遍历顺序）
            // 实际上对本题来说压入顺序不影响结果，因为只求最大值
            if (node->right) stk.push({node->right, depth + 1});
            if (node->left)  stk.push({node->left, depth + 1});
        }
        
        return maxD;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1 DFS递归    | 解法2 BFS层序    | 解法3 DFS迭代    |
// |-----------|-----------------|-----------------|-----------------|
// | 代码量     | 极少(4行核心)    | 中等(~12行)      | 中等(~10行)      |
// | 思维难度   | 最低             | 稍高(分层技巧)   | 中等(栈+配对)     |
// | 空间       | O(h) 调用栈      | O(w) 队列        | O(h) 显式栈      |
// | 栈溢出风险 | 有(极深树)       | 无               | 无               |
// | 适用场景   | 面试首选         | 需要按层处理时   | 练习迭代思维      |
//
// 面试推荐：先写解法1展示递归基本功（30秒），再主动提出解法2展示多角度。
//
// 【易错点】
//
// 1. 空树未处理:
//    ✗ 直接 return max(maxDepth(root->left), ...) — root为空时崩溃
//    ✓ 第一行 if (!root) return 0;
//
// 2. BFS 不分层:
//    ✗ while (!q.empty()) { q.pop(); depth++; } — depth变成总节点数
//    ✓ 用 levelSize = q.size() 配合内层 for 循环分层处理
//
// 3. DFS迭代忘记携带深度:
//    ✗ stack<TreeNode*> 只存节点 — 无法得知深度
//    ✓ stack<pair<TreeNode*, int>> 存 (节点, 深度)
//
// 4. 最小深度(111题)不能简单改 max 为 min:
//    ✗ return min(maxDepth(left), maxDepth(right)) + 1;
//      当左子树空时 minDepth=0, 结果错误地返回1
//    ✓ 需要判断：如果某侧子树空，只取另一侧
//
// 【面试追问（递进链）】
//
// Q1: "时间空间复杂度？最坏情况？"
//     → O(n) 时间，O(h) 空间。最坏 h=n（退化链表），最好 h=logn（完全平衡）
//
// Q2: "如果树极深可能栈溢出，怎么办？"
//     → 改用 BFS（解法2）或 DFS 迭代（解法3），显式栈在堆上分配不受调用栈限制
//
// Q3: "改成求最小深度（111题），代码改哪里？"
//     → 递归版：不能直接 max→min，需处理单侧子树为空的情况
//       BFS版：第一个遇到的叶子节点（左右都空）的层数就是答案，可提前返回
//
// Q4: "改成 N 叉树（559题），代码改哪里？"
//     → 递归: 遍历 children 取 max 替代 max(left, right)
//       BFS: 遍历 children 入队替代 left/right 入队
//
// 【相关题型】
// - 111. 最小深度 — 复用递归/BFS框架，区别：递归需特判单侧空子树，BFS可提前终止
// - 559. N叉树最大深度 — 复用递归思路，max(left,right)改为遍历children取max
// - 110. 平衡二叉树 — 复用本题递归求深度，额外判断 |leftDepth-rightDepth| <= 1
// - 102. 层序遍历 — 复用解法2的BFS分层模板，区别：收集每层值而非只计数
// ============================================================
