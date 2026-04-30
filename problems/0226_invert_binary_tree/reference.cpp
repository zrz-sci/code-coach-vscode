/*
 * LeetCode 226: 翻转二叉树 (Invert Binary Tree)
 *
 * 【题目本质】
 * 对每个节点交换其左右子树，使整棵树变成镜像。
 * 本质是一次完整的树遍历 + 每个节点做 swap 操作。
 *
 * 【解法总览】
 * 解法1: 递归 DFS (前序) — O(n) / O(h) — 最简洁，面试首选
 * 解法2: 递归 DFS (后序) — O(n) / O(h) — 另一种递归视角
 * 解法3: 迭代 DFS (栈)  — O(n) / O(n) — 不用递归，显式栈
 * 解法4: 迭代 BFS (队列) — O(n) / O(n) — 层序遍历版本
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
// 解法1: 递归 DFS (前序) — 先交换，再递归子树
// 时间: O(n)  空间: O(h)，h为树高
//
// 【思路】
// 翻转一棵树 = 交换根的左右子树 + 递归翻转左子树 + 递归翻转右子树
// 这是前序遍历的思路：先处理当前节点，再递归处理子树。
//
// 递归三要素:
//   终止条件: root == nullptr → 返回 nullptr
//   单层逻辑: swap(root->left, root->right)
//   返回值:   返回 root（当前子树已翻转完毕的根）
//
//  翻转前:              翻转后:
//       4                    4
//      / \                  / \
//     2   7                7   2
//    / \ / \              / \ / \
//   1  3 6  9            9  6 3  1
//
// 前序递归过程:
//   invertTree(4): swap(2,7) → 递归左(7) → 递归右(2)
//     invertTree(7): swap(6,9) → 递归左(9) → 递归右(6)
//       invertTree(9): swap(null,null) → 返回9
//       invertTree(6): swap(null,null) → 返回6
//     invertTree(2): swap(1,3) → 递归左(3) → 递归右(1)
//       invertTree(3): swap(null,null) → 返回3
//       invertTree(1): swap(null,null) → 返回1
// ============================================================
class Solution1 {
public:
    TreeNode* invertTree(TreeNode* root) {
        // 终止条件：空节点无需翻转
        if (!root) return nullptr;
        
        // 前序：先交换当前节点的左右子树
        swap(root->left, root->right);
        
        // 再递归翻转左右子树
        invertTree(root->left);
        invertTree(root->right);
        
        return root;
    }
};

// ============================================================
// 解法2: 递归 DFS (后序) — 先递归子树，再交换
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 和解法1的区别：先把左右子树各自翻转好，最后再交换它们。
// 就像先把两个零件各自加工好，再拼到一起。
// 
// 为什么前序和后序都对？
// 因为每个节点只需要"交换一次左右指针"，无论是先交换再递归、
// 还是先递归再交换，最终效果都是所有节点的左右指针被交换了恰好一次。
//
// ⚠️ 中序为什么容易错：
//   invertTree(root->left);       // 翻转左子树
//   swap(root->left, root->right); // 交换
//   invertTree(root->right);      // ✗ 此时 right 是原来已翻转过的 left！
// 这会导致原左子树被翻转两次（等于没翻），原右子树从未被翻转。
// ============================================================
class Solution2 {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        
        // 后序：先递归翻转左右子树
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);
        
        // 最后交换
        root->left = right;
        root->right = left;
        
        return root;
    }
};

// ============================================================
// 解法3: 迭代 DFS (栈) — 用栈模拟递归
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 递归用的是系统调用栈，我们用显式栈来代替。
// 核心不变：遍历每个节点 + 交换左右子树。
// 用栈进行前序遍历：弹出节点 → 交换 → 子节点入栈。
//
// 栈模拟过程:
//   stack = [4]
//   Pop 4, swap(2↔7), push 7,2  → stack = [7, 2]
//   Pop 2, swap(1↔3), push 3,1  → stack = [7, 3, 1]
//   Pop 1, 叶子无子节点          → stack = [7, 3]
//   Pop 3, 叶子无子节点          → stack = [7]
//   Pop 7, swap(6↔9), push 9,6  → stack = [9, 6]
//   Pop 6, 叶子                  → stack = [9]
//   Pop 9, 叶子                  → stack = []  → 完成
// ============================================================
class Solution3 {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        
        stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            
            // 交换当前节点的左右子树
            swap(node->left, node->right);
            
            // 非空子节点入栈（入栈顺序无所谓，只要每个节点被处理一次）
            if (node->left)  stk.push(node->left);
            if (node->right) stk.push(node->right);
        }
        
        return root;
    }
};

// ============================================================
// 解法4: 迭代 BFS (队列) — 层序遍历
// 时间: O(n)  空间: O(n)
//
// 【思路】
// BFS 按层遍历，对每个出队的节点交换左右子树。
// 和栈版本的唯一区别：数据结构从 LIFO(栈) 换成 FIFO(队列)。
// 核心操作完全相同：遍历 + swap。
//
// BFS 过程:
//   Level 0: queue = [4]
//     处理4: swap(2↔7), enqueue 7,2
//   Level 1: queue = [7, 2]
//     处理7: swap(6↔9), enqueue 9,6
//     处理2: swap(1↔3), enqueue 3,1
//   Level 2: queue = [9, 6, 3, 1]
//     全是叶子，无子节点入队
//   完成
// ============================================================
class Solution4 {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            
            // 交换当前节点的左右子树
            swap(node->left, node->right);
            
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间  | 空间  | 特点                     |
// |-------------|-------|-------|--------------------------|
// | 递归前序     | O(n)  | O(h)  | 最简洁，5行核心代码        |
// | 递归后序     | O(n)  | O(h)  | 先处理子树再合并，更函数式  |
// | 迭代DFS(栈)  | O(n)  | O(n)  | 避免递归栈溢出             |
// | 迭代BFS(队列)| O(n)  | O(n)  | 层序处理，便于扩展层级逻辑  |
//
// 面试建议：先写递归前序（解法1），被追问"不用递归"时给解法3或4。
// 四种解法的核心都是同一个操作：遍历每个节点 + swap(left, right)。
//
// 【易错点】
// 1. ✗ 终止条件写成叶子节点判断:
//      if (!root->left && !root->right) return root;
//    ✓ 应该判断空指针:
//      if (!root) return nullptr;
//    因为当 root 为 nullptr 时，访问 root->left 会崩溃。
//
// 2. ✗ 中序递归导致一边翻转两次:
//      invertTree(root->left);
//      swap(root->left, root->right);
//      invertTree(root->right);  // right 已经是翻转过的原 left！
//    ✓ 直接用前序或后序避免此问题。
//
// 3. ✗ 迭代版忘记判断子节点非空就入栈:
//      stk.push(node->left);   // 如果 left 是 null，后续弹出会解引用空指针
//    ✓ 加判断:
//      if (node->left) stk.push(node->left);
//
// 【面试追问】
// Q1: 递归用前序还是后序？中序行不行？
//     → 前序后序都行。中序交换后左右身份变了，容易导致一边被翻转两次。
//
// Q2: 如果树有上亿个节点且非常深，递归会怎样？怎么解决？
//     → 递归深度等于树高，极端情况（链状树）会栈溢出。
//       用迭代版（栈/队列在堆上分配）解决。
//
// Q3: 怎么验证翻转是否正确？
//     → 翻转两次应该回到原树。或者检查翻转后的中序遍历是否是原中序遍历的逆序。
//
// Q4: 如果要判断一棵树是否是另一棵树的镜像（对称树），怎么做？
//     → LeetCode 101. 先翻转一棵再用 LeetCode 100 判等，
//       或直接交叉递归比较 (left.left, right.right) 和 (left.right, right.left)。
// ============================================================
