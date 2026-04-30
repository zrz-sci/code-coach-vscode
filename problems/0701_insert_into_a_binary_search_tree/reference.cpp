/*
 * LeetCode 701: 二叉搜索树中的插入操作
 *
 * 【题目本质】
 * 利用 BST 的左小右大性质，沿路径下行到空位，把新节点挂上去。
 * 新值一定可以作为某个叶子的孩子插入，不需要调整树的结构。
 *
 * 【解法总览】
 * 解法1: 递归 — O(h) / O(h) — 最直觉，面试首选
 * 解法2: 迭代 — O(h) / O(1) — 空间优化，避免递归栈
 *
 * 其中 h 为树高，平衡时 O(log n)，退化时 O(n)。
 */

// ============================================================
// 解法1: 递归 — 利用 BST 性质递归找空位
// 时间: O(h)  空间: O(h) 递归栈
//
// 【思路】
// BST 的递归结构：每个节点把问题分成左子树或右子树。
// val 比当前节点小 → 一定要插到左子树
// val 比当前节点大 → 一定要插到右子树
// 走到空节点 → 就是插入位置，创建新节点返回
//
// 递归三要素:
//   返回值: 插入 val 后，以当前节点为根的子树的根节点
//   终止条件: root == nullptr → 创建新节点返回
//   单层逻辑: 根据 val 和 root->val 的大小关系递归左或右
//
// 示例: 插入 val=5 到 BST
//
//         4               4
//        / \     →       / \
//       2   7           2   7
//      / \             / \ /
//     1   3           1  3 5
//
// 路径: 5>4 → 右子树 → 5<7 → 左子树 → null → 创建节点(5)
//
// 递归调用过程:
//   insertIntoBST(4, 5): 5>4, root->right = insertIntoBST(7, 5)
//     insertIntoBST(7, 5): 5<7, root->left = insertIntoBST(null, 5)
//       insertIntoBST(null, 5): return new TreeNode(5)  ← 终止条件
//     7->left = node(5), return 7
//   4->right = 7, return 4
// ============================================================
class Solution1 {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        // 终止条件：找到空位，创建新节点
        // 这也处理了空树的情况(root本身为null)
        if (!root) return new TreeNode(val);
        
        if (val < root->val)
            // val 比当前节点小，递归插入左子树
            // 返回值赋给 root->left：如果左子树原本为 null，
            // 新节点就被"挂载"上了
            root->left = insertIntoBST(root->left, val);
        else
            // val 比当前节点大，递归插入右子树
            root->right = insertIntoBST(root->right, val);
        
        // 返回当前子树的根节点（本身没有改变）
        return root;
    }
};

// ============================================================
// 解法2: 迭代 — 用循环 + parent 指针找空位
// 时间: O(h)  空间: O(1)
//
// 【思路】
// 递归的本质是沿着一条路径从根走到 null。
// 用 while 循环完全可以模拟：
//   - curr 指针沿路径下行
//   - parent 记录 curr 的父节点
//   - curr 走到 null 时，parent 就是要挂载新节点的位置
//
// 迭代过程 (插入 val=5):
//
//   Step1: curr=4, parent=null   5>4 → curr = 4->right = 7
//   Step2: curr=7, parent=4      5<7 → curr = 7->left = null
//   Step3: curr=null, parent=7   退出循环
//          5 < 7 → parent->left = new TreeNode(5)
//
//         4
//        / \
//       2   7
//      / \ /
//     1  3 5   ← 新节点挂到 7 的左孩子
// ============================================================
class Solution2 {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        // 空树特判：迭代版必须处理，因为没有 parent 可用
        if (!root) return new TreeNode(val);
        
        TreeNode* curr = root;
        TreeNode* parent = nullptr;
        
        // 沿着 BST 路径下行到 null
        while (curr) {
            parent = curr;  // 记录父节点
            if (val < curr->val)
                curr = curr->left;
            else
                curr = curr->right;
        }
        // 此时 curr == null，parent 是插入位置的父节点
        
        // 根据 val 和 parent 的关系，挂到左或右
        if (val < parent->val)
            parent->left = new TreeNode(val);
        else
            parent->right = new TreeNode(val);
        
        return root;  // 根节点不变
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 递归(解法1)       | 迭代(解法2)        |
// |----------|-------------------|--------------------|
// | 时间     | O(h)              | O(h)               |
// | 空间     | O(h) 递归栈       | O(1)               |
// | 代码量   | 5行核心，极简     | 稍多，需 parent    |
// | 空树处理 | 自然是终止条件    | 必须单独特判       |
// | 推荐     | 面试首选          | 追问空间优化时给出 |
//
// 两种解法的核心逻辑完全一致：沿 BST 路径下行到 null，挂载新节点。
// 区别仅在于"用递归调用栈"还是"用显式循环"来走这条路径。
//
// ============================================================
// 【易错点】
//
// 1. 递归版忘记 return root:
//    ✗ 在 if-else 分支中递归完就结束了，没返回 root
//    ✓ 最后必须 return root，让上层递归能接收到"当前子树的根"
//
// 2. 迭代版忘记空树特判:
//    ✗ 直接进 while 循环 → root 为 null 时循环不执行 → parent 为 null
//      → parent->val 直接段错误
//    ✓ 开头 if (!root) return new TreeNode(val);
//
// 3. 递归方向写反:
//    ✗ if (val < root->val) root->right = insertIntoBST(root->right, val);
//       把小的值插到右子树，破坏 BST 性质
//    ✓ val < root->val 对应 root->left
//
// 4. 迭代版挂载方向和下行方向不一致:
//    ✗ while 中用 val < curr->val 往左走，
//       但出来后用 val > parent->val 判断挂右 → 逻辑不匹配
//    ✓ 两处判断条件必须一致：val < xxx → 左
//
// ============================================================
// 【面试追问】
//
// Q1: "这样插入后 BST 还平衡吗？"
//     不一定。如果按有序序列插入(1,2,3,4,5)，树退化为链表，
//     每次操作 O(n)。保持平衡需要 AVL/红黑树的旋转机制。
//
// Q2: "如果要删除一个节点呢？(LeetCode 450)"
//     删除比插入复杂，分三种情况:
//     (1) 叶子: 直接删
//     (2) 只有一个孩子: 孩子替代自己
//     (3) 两个孩子: 找中序后继(右子树最小)替代，再递归删除后继
//     插入只需找空位一种情况，删除要处理三种。
//
// Q3: "如果频繁插入和查询，怎么保证 O(log n)？"
//     使用自平衡 BST: AVL 树(严格平衡，旋转多) 或
//     红黑树(近似平衡，旋转少)。C++ 的 std::set/map 底层就是红黑树。
//     或者使用跳表(Skip List)等概率数据结构。
// ============================================================
