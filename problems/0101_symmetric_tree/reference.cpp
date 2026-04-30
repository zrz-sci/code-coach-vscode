/*
 * LeetCode 101: 对称二叉树 (Symmetric Tree)
 *
 * 【题目本质】
 * 判断一棵二叉树是否轴对称，即左子树和右子树互为镜像。
 * 镜像 = 交叉配对：左的左 对应 右的右，左的右 对应 右的左。
 *
 * 【和 100 题的关系】
 * 100 题: isSame(p->left, q->left) && isSame(p->right, q->right)  — 同侧
 * 101 题: isMirror(p->left, q->right) && isMirror(p->right, q->left) — 交叉
 * 仅此一行区别。
 *
 * 【解法总览】
 * 解法1: 递归 (DFS)       — O(n) / O(h) — 面试首选，最简洁
 * 解法2: 迭代 (BFS/队列)  — O(n) / O(n) — Follow-up 迭代解法
 * 解法3: 迭代 (DFS/栈)    — O(n) / O(n) — 等价于解法2，数据结构不同
 */

// Definition for a binary tree node.
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode() : val(0), left(nullptr), right(nullptr) {}
//     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
//     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
// };

// ============================================================
// 解法1: 递归 (DFS) — 交叉配对递归
// 时间: O(n)  空间: O(h)，h 为树高（最坏 O(n)，平衡 O(log n)）
//
// 【思路】
// "对称" = 左子树和右子树互为镜像。
// 两棵树镜像的递归定义：
//   1. 根值相同
//   2. A 的左子树和 B 的右子树镜像（交叉！）
//   3. A 的右子树和 B 的左子树镜像（交叉！）
//
// 和 100 题（相同的树）对比：
//   100: isSame(p->left, q->left) && isSame(p->right, q->right)
//   101: isMirror(p->left, q->right) && isMirror(p->right, q->left)
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//        同侧配对 → 交叉配对，仅此区别
//
// 【可视化 — 交叉配对过程】
//
//     对称树:                    递归展开:
//         1                      isSymmetric(1)
//        / \                       └── isMirror(2, 2)
//       2   2                           ├── isMirror(3, 3)   ← 左.left vs 右.right
//      / \ / \                          │    ├── isMirror(null,null) → true
//     3  4 4  3                         │    └── isMirror(null,null) → true
//                                       │    → 3==3 ✓ → true
//                                       └── isMirror(4, 4)   ← 左.right vs 右.left
//                                            ├── isMirror(null,null) → true
//                                            └── isMirror(null,null) → true
//                                            → 4==4 ✓ → true
//                                       → 2==2 ✓ && true && true → true
//
//     不对称树:                  递归展开:
//         1                      isSymmetric(1)
//        / \                       └── isMirror(2, 2)
//       2   2                           ├── isMirror(null, null) → true  ← 左.left vs 右.right
//        \   \                          └── isMirror(3, 3)               ← 左.right vs 右.left
//         3   3                              但等等！左.right.left 和 右.left... 
//                                            实际上 isMirror(null, 3) → false ✗
//                                            (左2 无左孩子, 右2 无左孩子)
//                                    结论: 不对称
// ============================================================
class Solution1 {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        return isMirror(root->left, root->right);
    }
    
private:
    bool isMirror(TreeNode* p, TreeNode* q) {
        // 终止条件1: 都是 null — 对称 ✓
        if (!p && !q) return true;
        // 终止条件2: 一个 null 一个非 null — 不对称 ✗
        // 为什么不能合并到上面? 因为都null是true，只有一个null才是false
        if (!p || !q) return false;
        // 终止条件3: 值不等 — 不对称 ✗
        if (p->val != q->val) return false;
        
        // 核心: 交叉配对递归
        // 如果改成 (p->left, q->left) && (p->right, q->right) 就变成了 100 题
        return isMirror(p->left, q->right)    // 左的左 vs 右的右
            && isMirror(p->right, q->left);   // 左的右 vs 右的左
    }
};


// ============================================================
// 解法2: 迭代 (BFS/队列) — 用队列存储待比较的节点对
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 递归本质上是用系统调用栈存储"待比较的节点对"。
// 迭代版就是把这些"节点对"显式存到队列中：
//   - 每次取出一对 (p1, p2) 进行比较
//   - 如果值相同，把它们的孩子交叉入队
//   - 如果值不同或一个为 null，返回 false
//
// 【队列变化过程 (示例1)】
//
//  树:        1
//            / \
//           2   2
//          / \ / \
//         3  4 4  3
//
//  初始队列:     [2, 2]
//  取出(2,2):    val 相同 → 入队 (2.left=3, 2.right=3), (2.right=4, 2.left=4)
//  队列:         [3, 3, 4, 4]
//  取出(3,3):    val 相同 → 孩子都是 null，不入队
//  队列:         [4, 4]
//  取出(4,4):    val 相同 → 孩子都是 null，不入队
//  队列空 → true ✓
// ============================================================
class Solution2 {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        
        queue<TreeNode*> q;
        // 成对入队
        q.push(root->left);
        q.push(root->right);
        
        while (!q.empty()) {
            // 成对取出 — 必须取两个
            TreeNode* p1 = q.front(); q.pop();
            TreeNode* p2 = q.front(); q.pop();
            
            // 都是 null: 这一对没问题，看下一对
            if (!p1 && !p2) continue;
            // 一个 null 或值不等: 不对称
            if (!p1 || !p2) return false;
            if (p1->val != p2->val) return false;
            
            // 交叉配对入队 — 这是和"相同的树"迭代版的唯一区别
            q.push(p1->left);    // 左的左
            q.push(p2->right);   // 右的右  → 这一对应该值相同
            q.push(p1->right);   // 左的右
            q.push(p2->left);    // 右的左  → 这一对应该值相同
        }
        
        return true;
    }
};


// ============================================================
// 解法3: 迭代 (DFS/栈) — 和解法2逻辑完全一样，只是用栈替换队列
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 队列 → 栈，BFS → DFS，其余代码一模一样。
// 唯一注意: 栈是 LIFO，取出顺序和入栈顺序相反，
// 但由于我们总是成对操作，不影响正确性。
// ============================================================
class Solution3 {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) return true;
        
        stack<TreeNode*> stk;
        stk.push(root->left);
        stk.push(root->right);
        
        while (!stk.empty()) {
            // 栈是 LIFO，所以后入的先出
            TreeNode* p2 = stk.top(); stk.pop();
            TreeNode* p1 = stk.top(); stk.pop();
            
            if (!p1 && !p2) continue;
            if (!p1 || !p2) return false;
            if (p1->val != p2->val) return false;
            
            // 交叉配对入栈
            stk.push(p1->left);
            stk.push(p2->right);
            stk.push(p1->right);
            stk.push(p2->left);
        }
        
        return true;
    }
};


// ============================================================
// 【解法对比】
//
// | 解法       | 时间  | 空间   | 代码量 | 适用场景         |
// |-----------|-------|--------|--------|-----------------|
// | 递归 DFS   | O(n) | O(h)   | 最少   | 面试首选         |
// | 迭代 BFS   | O(n) | O(n)   | 中等   | Follow-up 迭代   |
// | 迭代 DFS   | O(n) | O(n)   | 中等   | 和 BFS 等价      |
//
// 递归空间 O(h) 在平衡树下是 O(log n)，优于迭代的 O(n)。
// 面试时先写递归，被追问再给迭代。
//
// 【易错点】
//
// 1. 递归终止条件的顺序不能反：
//    ✗ if (!p || !q) return false;  // 两个都null也会返回false!
//    ✓ 必须先判断 (!p && !q) return true; 再判断 (!p || !q) return false;
//
// 2. 交叉配对写成同侧配对：
//    ✗ isMirror(p->left, q->left) — 这是判断"相同的树"
//    ✓ isMirror(p->left, q->right) — 这才是判断"镜像"
//
// 3. 迭代版忘记处理 (null, null) 对：
//    ✗ 直接访问 p1->val 导致空指针异常
//    ✓ if (!p1 && !p2) continue; 跳过这对，而不是报错
//
// 4. 迭代版入队不成对：
//    ✗ 只入了 p1->left 和 p1->right，忘了交叉
//    ✓ 必须 (p1->left, p2->right) 和 (p1->right, p2->left) 两对都入队
//
// 【面试追问 — 递进链】
//
// Q1: 递归的空间复杂度是多少？最坏情况？
// → O(h)。最坏退化成链表，h=n，空间 O(n)。平衡树 O(log n)。
//
// Q2: 能否用迭代实现？（Follow-up）
// → 队列/栈存储节点对，交叉配对入队/入栈，见解法2/3。
//
// Q3: 和 100 题（相同的树）代码上具体哪里不同？
// → 仅递归调用的参数配对方式不同：
//    100: (left,left) + (right,right)  — 同侧
//    101: (left,right) + (right,left)  — 交叉
//
// Q4: 如果要把一棵不对称的树"变对称"，最少修改几个节点？
// → 递归比较左右子树，遇到不匹配的位置计数。
//    可以在 isMirror 框架上改造: 不 return false，而是计数差异。
//
// 【相关题型】
//
// - 100. 相同的树
//   复用: 完全相同的递归框架
//   区别: 同侧配对(left,left) → 交叉配对(left,right)，改一行
//
// - 226. 翻转二叉树
//   关系: 翻转后和原树互为镜像。理论上可以先翻转右子树再用100题判相同
//   但直接用101的方法更优（不修改原树）
//
// - 572. 另一棵树的子树
//   复用: "判断两棵树是否相同"作为子函数
//   区别: 572 在主树每个节点调用 isSame，101 只在根节点调用 isMirror
// ============================================================
