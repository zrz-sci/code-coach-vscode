/*
 * LeetCode 98: 验证二叉搜索树 (Validate Binary Search Tree)
 *
 * 【题目本质】
 * 判断一棵二叉树是否满足 BST 的全局约束：左子树所有值 < 根 < 右子树所有值。
 * 注意不是只比较直接孩子，而是整个子树。
 *
 * 【解法总览】
 * 解法1: 上下界递归   — O(n) / O(h) — 最直觉，面试首选
 * 解法2: 中序遍历递归 — O(n) / O(h) — 利用 BST 中序=严格递增
 * 解法3: 中序遍历迭代 — O(n) / O(h) — 栈模拟，不用递归
 *
 * 示例树:
 *
 *  有效BST:           无效BST:
 *       2                  5
 *      / \                / \
 *     1   3              1   4       ← 4 < 5，右孩子比根小
 *                           / \
 *                          3   6     ← 3 在5的右子树中但 3 < 5
 */


// ============================================================
// 解法1: 上下界递归 — 传递合法范围 (lower, upper)
// 时间: O(n)  空间: O(h)  h为树高
//
// 【思路】
// 每个节点都有一个"合法的取值范围"。
// 根节点: (-∞, +∞)
// 往左走: 上界收紧为父节点值 → (lower, parent->val)
// 往右走: 下界收紧为父节点值 → (parent->val, upper)
// 只要某个节点不在它的合法范围内，就不是BST。
//
// 为什么这样做是对的？
// 因为BST的定义是"左子树所有节点 < 根"，而不仅是左孩子 < 根。
// 通过传递范围，我们把这个"所有节点"的全局约束，
// 转化为每个节点的局部判断 O(1)。
//
// 范围传递过程 (以 [5,1,4,null,null,3,6] 为例):
//         5 (-∞, +∞)         ✓ -∞ < 5 < +∞
//        / \
//   1(-∞,5)  4(5,+∞)        ✓ 1∈(-∞,5); ✗ 4∉(5,+∞) → false!
//            / \
//       3(5,4) 6(4,+∞)      (已经不会执行到这里)
// ============================================================
class Solution1 {
public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LONG_MIN, LONG_MAX);
    }

private:
    bool validate(TreeNode* node, long long lower, long long upper) {
        if (!node) return true;  // 空节点不违反任何约束

        // 当前节点值必须严格在开区间 (lower, upper) 内
        // 用 long long 避免节点值恰好等于 INT_MIN/INT_MAX 时的边界问题
        if (node->val <= lower || node->val >= upper)
            return false;

        // 左子树: 上界收紧为 node->val (左子树所有值必须 < node->val)
        // 右子树: 下界收紧为 node->val (右子树所有值必须 > node->val)
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }
};

// ============================================================
// 解法2: 中序遍历（递归）— BST 中序遍历 = 严格递增
// 时间: O(n)  空间: O(h)
//
// 【思路】
// BST 有一个核心性质：中序遍历（左-根-右）的结果是严格递增序列。
// 反过来，如果中序遍历不是严格递增的，就不是 BST。
//
// 所以我们做一次中序遍历，用 prev 记录上一个访问的节点，
// 每次访问新节点时检查 cur->val > prev->val 是否成立。
//
// 为什么用 TreeNode* 而不是 int 做 prev？
// 因为节点值范围是 [INT_MIN, INT_MAX]，
// 如果用 int prev = INT_MIN，第一个节点值恰好是 INT_MIN 就会误判。
// 用指针的话，nullptr 天然表示"还没有前驱"，第一次比较直接跳过。
//
// 中序遍历过程 (以有效 BST [2,1,3] 为例):
//       2
//      / \
//     1   3
//
//  Step1: inorder(2) → inorder(1) → inorder(null)返回true
//  Step2: 访问1, prev=null → 跳过比较, prev=1
//  Step3: inorder(null)返回true, 回到2
//  Step4: 访问2, prev=1, 2>1 ✓, prev=2
//  Step5: inorder(3) → inorder(null)返回true
//  Step6: 访问3, prev=2, 3>2 ✓, prev=3
//  Step7: inorder(null)返回true → 全部通过 → true
// ============================================================
class Solution2 {
public:
    TreeNode* prev = nullptr;

    bool isValidBST(TreeNode* root) {
        return inorder(root);
    }

private:
    bool inorder(TreeNode* node) {
        if (!node) return true;

        // 先递归左子树（中序遍历的"左"）
        if (!inorder(node->left)) return false;

        // 处理当前节点（中序遍历的"根"）
        // 检查当前值是否严格大于前一个值
        if (prev && node->val <= prev->val)
            return false;
        prev = node;  // 更新 prev 为当前节点

        // 再递归右子树（中序遍历的"右"）
        return inorder(node->right);
    }
};

// ============================================================
// 解法3: 中序遍历（迭代栈）— 用栈模拟递归
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 和解法2完全一样的逻辑，但用显式栈代替系统调用栈。
// 好处：不用担心深度递归导致栈溢出。
//
// 迭代中序遍历模板:
// 1. 从当前节点一路向左，全部入栈
// 2. 弹出栈顶（它就是下一个要访问的节点）
// 3. 处理该节点（检查是否递增）
// 4. 转向右子树，重复 1
//
// 迭代过程 (以 [2,1,3] 为例):
//  初始: stk=[], cur=2
//  Round1: 一路向左 → stk=[2,1], cur=null
//          弹出1 → 访问1, prev=null(跳过), prev→1, cur=null(无右子)
//  Round2: stk=[2], cur=null
//          弹出2 → 访问2, 2>1✓, prev→2, cur=3
//  Round3: 一路向左 → stk=[3], cur=null
//          弹出3 → 访问3, 3>2✓, prev→3, cur=null(无右子)
//  Round4: stk=[], cur=null → 循环结束 → true
// ============================================================
class Solution3 {
public:
    bool isValidBST(TreeNode* root) {
        std::stack<TreeNode*> stk;
        TreeNode* cur = root;
        TreeNode* prev = nullptr;

        while (cur || !stk.empty()) {
            // 一路向左，全部入栈
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }

            // 弹出栈顶 = 当前最小的未访问节点
            cur = stk.top();
            stk.pop();

            // 检查是否严格递增
            if (prev && cur->val <= prev->val)
                return false;
            prev = cur;

            // 转向右子树（右子树的左链会在下一轮被入栈）
            cur = cur->right;
        }

        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 核心思想              | 时间 | 空间 | 适合场景     |
// |---------------|----------------------|------|------|-------------|
// | 上下界递归    | 传递合法值域范围      | O(n) | O(h) | 面试首选    |
// | 中序遍历递归  | BST中序=严格递增      | O(n) | O(h) | 展示BST理解 |
// | 中序遍历迭代  | 栈模拟中序+递增检查   | O(n) | O(h) | 追问不用递归 |
//
// 三种方法时间空间完全一样。
// - 上下界法最直觉，容易解释，代码简洁，面试优先用这个。
// - 中序遍历法展示你知道 BST 的核心性质（中序有序），加分。
// - 迭代版在面试官追问"不用递归"时使用。
//
// ============================================================
// 【易错点】
//
// 1. 只比较直接孩子:
//    ✗ if (node->left && node->left->val >= node->val) return false;
//    → 这只检查了左孩子，右子树的 [5,1,4,null,null,3,6] 中
//      3在5的右子树中但 3<5，上面的代码无法检测到。
//    ✓ 必须用上下界传递或中序遍历来确保全局约束。
//
// 2. 边界值用 int 导致错误:
//    ✗ validate(root, INT_MIN, INT_MAX)
//      → 当节点值恰好是 INT_MIN 时，INT_MIN <= INT_MIN 为 true，误判为非法。
//    ✓ validate(root, LONG_MIN, LONG_MAX)
//    ✓ 或改用 TreeNode* / optional 表示无穷。
//
// 3. 中序遍历 prev 初始化为 INT_MIN:
//    ✗ int prev = INT_MIN;  → 节点值为 INT_MIN 时 cur->val <= prev 误判
//    ✓ TreeNode* prev = nullptr; 然后 if (prev && ...) 跳过第一次比较。
//
// 4. 忘记 BST 是严格不等式:
//    ✗ if (node->val < lower || node->val > upper)  → 允许了相等
//    ✓ if (node->val <= lower || node->val >= upper)  → BST 不允许重复值
//
// 5. 中序遍历递归中 prev 没用成员变量/引用:
//    ✗ 在函数内定义局部 prev → 每次递归调用都是新的 prev，不会更新
//    ✓ 用类成员变量，或通过引用参数传递 prev。
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么不能只比较每个节点和它的左右孩子？
//     → BST 是全局约束。[5,1,4,null,null,3,6] 中 4 是 5 的右孩子，
//       4 的左孩子 3 虽然 < 4（局部合法），但 3 < 5（全局不合法）。
//       只比较直接孩子无法发现这种跨层违规。
//
// Q2: 节点值可以是 INT_MIN/INT_MAX，边界怎么处理？
//     → 方案A: 用 long long (LONG_MIN, LONG_MAX) 做边界
//     → 方案B: 传 TreeNode* 指针做边界，nullptr 代表无穷
//     → 方案C: 用中序遍历 + TreeNode* prev，天然避免边界问题
//
// Q3: 如果不允许递归怎么办？
//     → 解法3，用显式栈模拟中序遍历。空间仍为 O(h)，但不会栈溢出。
//
// Q4: 如果 BST 中有两个节点被错误交换了，怎么恢复？（LeetCode 99）
//     → 在中序遍历中，正常应严格递增。如果有两个节点被交换，
//       会出现 1 或 2 处"逆序"。记录第一个逆序的 prev 和
//       最后一个逆序的 cur，交换它们的值即可恢复。
//       直接复用本题解法2/3 的框架，只需额外记录两个错位节点。
//
// ============================================================
// 【相关题型】
//
// - LeetCode 94 二叉树的中序遍历
//   → 本题解法2/3 直接复用中序遍历模板，区别是遍历中做判断而非收集结果
//
// - LeetCode 99 恢复二叉搜索树
//   → 复用中序遍历 + prev 框架，改为记录两个错位节点并交换值
//   → 代码差异: 本题发现逆序直接 return false，99题改为记录节点最后交换
//
// - LeetCode 700 二叉搜索树中的搜索
//   → 复用解法1的"比根小往左，比根大往右"的 BST 搜索逻辑
//
// - LeetCode 230 BST中第K小的元素
//   → 复用中序遍历框架，遍历到第 K 个时返回
//
// - LeetCode 501 BST中的众数
//   → 复用中序遍历 + prev 框架，额外维护频率计数
// ============================================================
