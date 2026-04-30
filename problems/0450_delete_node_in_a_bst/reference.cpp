/*
 * LeetCode 450: 删除二叉搜索树中的节点 (Delete Node in a BST)
 *
 * 【题目本质】
 * 在 BST 中找到值为 key 的节点并删除它，保持 BST 性质。
 * 核心难点：被删节点有两个孩子时，用中序后继（或前驱）替代。
 *
 * 【解法总览】
 * 解法1: 递归(返回值重接指针) — O(h) / O(h) — 面试首选，简洁优雅
 * 解法2: 迭代(手动维护父指针) — O(h) / O(1) — 进阶，空间最优
 */

// 树节点定义（题目已提供，此处列出供参考）
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode() : val(0), left(nullptr), right(nullptr) {}
//     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
//     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
// };

// ============================================================
// 解法1: 递归（返回值重接指针）— 面试首选
// 时间: O(h)  空间: O(h) 递归栈，h为树高
//
// 【思路】
// BST 的递归结构天然适合递归处理。
// 核心设计：deleteNode(root, key) 返回"删除 key 后这棵子树的新根"
// 这样父节点只需 root->left = deleteNode(root->left, key) 自动接好指针。
//
// 删除分三种情况：
//
//   原始树:
//        5
//       / \
//      3   6         要删除节点 3
//     / \   \
//    2   4   7
//
//   情况1 - 叶子节点(如删2): 直接返回 nullptr
//        3            3
//       / \    →     / \
//      2   4        X   4
//
//   情况2 - 一个孩子(如删3且3只有左子树2): 返回那个孩子
//        5            5
//       / \    →     / \
//      3   6        2   6
//     /     \            \
//    2       7            7
//
//   情况3 - 两个孩子(删3，有孩子2和4):
//     找右子树最小值(后继=4)，值替换，再递归删后继
//        5            5
//       / \    →     / \
//      3   6        4   6      (用4替换3的值，然后在右子树删4)
//     / \   \      /     \
//    2   4   7    2       7
//
// ============================================================
class Solution1 {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;  // key 不存在于树中
        
        if (key < root->val) {
            // key 比当前节点小，去左子树删，删完重新接上
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            // key 比当前节点大，去右子树删
            root->right = deleteNode(root->right, key);
        } else {
            // 找到了要删除的节点 root->val == key
            
            // 情况1&2合并: 没有左孩子 → 右孩子顶上（叶子时右孩子也是nullptr）
            if (!root->left) return root->right;
            // 情况2: 没有右孩子 → 左孩子顶上
            if (!root->right) return root->left;
            
            // 情况3: 左右孩子都有
            // 找右子树的最小节点（中序后继）：从 root->right 开始一路向左
            TreeNode* successor = root->right;
            while (successor->left) {
                successor = successor->left;
            }
            // 用后继的值覆盖当前节点（相当于"替身"）
            root->val = successor->val;
            // 在右子树中递归删除后继节点
            // 因为后继没有左孩子，删除它会退化为情况1或2，保证不会无限递归
            root->right = deleteNode(root->right, successor->val);
        }
        return root;
    }
};

// ============================================================
// 解法2: 迭代（手动维护父指针）— 空间 O(1)
// 时间: O(h)  空间: O(1)
//
// 【思路】
// 解法1的递归栈深度为 O(h)，进阶要求我们考虑空间优化。
// 迭代版需要手动维护 parent 指针，逻辑分为：
//   Step1: 迭代找到目标节点 curr 及其父节点 parent
//   Step2: 如果 curr 有两个孩子，先找后继做值替换，
//          把问题转化成删除后继（最多一个孩子）
//   Step3: curr 此时最多一个孩子，用 child 替换 curr
//
// 特别注意：删除的可能是根节点（parent == nullptr）
//
//   示例：删除节点3（有两个孩子2和4）
//
//   找后继:
//        5 (parent=nullptr → parent=5)
//       / \
//    →[3]  6       curr=3, parent=5
//     / \   \
//    2   4   7     succ=4, succParent=3
//
//   值替换后:
//        5
//       / \
//      4   6       curr→succ(即原来的4), parent→succParent(即3→现在值为4)
//     / \   \
//    2  [4]  7     现在要删这个4（叶子），parent 是值为4的那个节点
//
//   最终:
//        5
//       / \
//      4   6
//     /     \
//    2       7
// ============================================================
class Solution2 {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        // Step1: 迭代查找目标节点
        TreeNode* parent = nullptr;
        TreeNode* curr = root;
        while (curr && curr->val != key) {
            parent = curr;
            if (key < curr->val) curr = curr->left;
            else curr = curr->right;
        }
        if (!curr) return root;  // key 不存在，树不变
        
        // Step2: 如果有两个孩子，找后继做值替换
        if (curr->left && curr->right) {
            TreeNode* succParent = curr;   // 注意初始化为 curr 而非 nullptr！
            TreeNode* succ = curr->right;
            while (succ->left) {
                succParent = succ;
                succ = succ->left;
            }
            curr->val = succ->val;         // 值替换
            // 问题转化：删除 succ（它最多有右孩子）
            curr = succ;
            parent = succParent;
        }
        
        // Step3: curr 现在最多一个孩子
        TreeNode* child = curr->left ? curr->left : curr->right;
        
        if (!parent) {
            // 删除的是根节点
            return child;
        }
        // 用 child 替换 curr
        if (parent->left == curr) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        
        return root;
    }
};

// ============================================================
// 补充解法: 递归（不用值替换，纯指针操作）
// 时间: O(h)  空间: O(h)
//
// 【思路】
// 有些面试官不喜欢值替换（因为实际系统中节点可能有外部引用）。
// 纯指针操作的方案：两个孩子时，将左子树接到右子树最小节点的左边，
// 然后返回右子树根。
//
//   删除3（有孩子2和4）:
//   
//        5              5
//       / \            / \
//      3   6    →     4   6       返回右子树根4
//     / \   \        /     \      把左子树2接到4的最左（4本身就是最左）
//    2   4   7      2       7
//
//   为什么正确？2 < 4（因为2在3的左子树，4在3的右子树的最左），
//   所以2接到4左边仍满足BST性质。
// ============================================================
class Solution3 {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;
        
        if (key < root->val) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            root->right = deleteNode(root->right, key);
        } else {
            // 找到目标节点
            if (!root->left) return root->right;
            if (!root->right) return root->left;
            
            // 两个孩子：把左子树挂到右子树最小节点的左边
            TreeNode* rightMin = root->right;
            while (rightMin->left) {
                rightMin = rightMin->left;
            }
            rightMin->left = root->left;  // 左子树接过去
            
            // 返回右子树根作为新的子树根
            return root->right;
        }
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1(递归值替换) | 解法2(迭代)    | 解法3(递归指针操作) |
// |-----------|-----------------|---------------|-------------------|
// | 时间       | O(h)            | O(h)          | O(h)              |
// | 空间       | O(h) 递归栈      | O(1)          | O(h) 递归栈        |
// | 代码复杂度  | 简洁            | 复杂           | 简洁               |
// | 是否改值    | 是(改val)       | 是(改val)      | 否(纯指针)          |
// | 面试推荐   | ⭐ 首选          | 追问空间优化时   | 追问不改值时        |
//
// 选择建议：
// - 面试默认写解法1，最简洁不容易出错
// - 面试官问"能否O(1)空间"→ 给解法2
// - 面试官问"如果节点有外部引用不能改值"→ 给解法3
//
// 【易错点】
// 1. 递归版忘记 if(!root) return nullptr → key不存在时访问空指针崩溃
//
// 2. 情况3中用 successor 的值替换后，忘记在右子树中递归删除 successor
//    → 树中出现重复值，违反BST性质
//
// 3. 迭代版中 succParent 初始化为 nullptr 而不是 curr
//    → 当 curr->right 没有左子树时，succ = curr->right，
//      其父节点应该是 curr，初始化为 nullptr 会导致空指针
//
// 4. 解法3中把左子树接到右子树最小节点时，忘记处理 rightMin 的查找
//    → 一定要 while(rightMin->left) 走到最左，否则接错位置破坏BST性质
//
// 5. 迭代版忘记处理删除根节点的情况（parent == nullptr）
//    → 返回了原来的 root 但 root 已经被删了
//
// 【面试追问】
// Q1: 删除BST节点分几种情况？
//     → 3种：叶子直接删；一个孩子让孩子顶上；两个孩子找后继替代
//
// Q2: 能否O(1)空间？
//     → 可以，用迭代(解法2)，手动维护parent指针
//
// Q3: 如果节点有外部指针引用，不能改val怎么办？
//     → 用纯指针操作(解法3)，把左子树接到右子树最小节点的左边
//
// Q4: 频繁删除会有什么问题？
//     → 普通BST可能退化为链表(h=n)，需要自平衡BST(AVL/红黑树)保证h=O(logn)
//
// Q5: 前驱替代 vs 后继替代有区别吗？
//     → 功能等价，但总用一种可能导致不平衡，可以交替使用
// ============================================================
