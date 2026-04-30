/*
 * LeetCode 144: 二叉树的前序遍历 (Binary Tree Preorder Traversal)
 *
 * 【题目本质】
 * 按"根→左→右"的顺序访问二叉树所有节点，返回访问序列。
 * 核心考点：递归与迭代两种实现方式，以及 O(1) 空间的 Morris 遍历。
 *
 * 【解法总览】
 * 解法1: 递归 DFS         — O(n) / O(h) — 最直觉，翻译定义即可
 * 解法2: 迭代（显式栈）   — O(n) / O(h) — 面试必考，用栈模拟递归
 * 解法3: Morris 遍历      — O(n) / O(1) — 进阶，利用线索化省空间
 */

// ============================================================
// 解法1: 递归 DFS — 直接翻译前序遍历的递归定义
// 时间: O(n)  空间: O(h)  h为树高，最坏O(n)
//
// 【思路】
// 前序遍历的定义本身就是递归的：
//   1. 访问根节点
//   2. 递归遍历左子树
//   3. 递归遍历右子树
// 直接翻译成代码，递归三要素：
//   - 返回值: void（结果通过引用收集）
//   - 终止条件: 节点为空
//   - 单层逻辑: push_back → 递归左 → 递归右
//
// 示例树及遍历过程:
//          1
//        /   \
//       2     3
//      / \     \
//     4   5     8
//        / \   /
//       6   7 9
//
// 调用链: dfs(1) → push 1 → dfs(2) → push 2 → dfs(4) → push 4
//         → dfs(null) 返回 → dfs(null) 返回 → 回到 dfs(2) → dfs(5)
//         → push 5 → dfs(6) → push 6 → ... → dfs(3) → push 3
//         → dfs(null) → dfs(8) → push 8 → dfs(9) → push 9
//
// 结果: [1, 2, 4, 5, 6, 7, 3, 8, 9]
// ============================================================
class Solution1 {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(TreeNode* node, vector<int>& result) {
        if (!node) return;  // 终止条件: 空节点

        result.push_back(node->val);  // 前序: 先访问根
        dfs(node->left, result);      // 再递归左子树
        dfs(node->right, result);     // 最后递归右子树
        // 如果改成中序: 把 push_back 移到两个 dfs 之间
        // 如果改成后序: 把 push_back 移到两个 dfs 之后
    }
};

// ============================================================
// 解法2: 迭代（显式栈） — 用栈模拟递归调用栈 ⭐ 面试首选
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 递归之所以能工作，是因为系统帮我们维护了调用栈。
// 我们可以自己用一个 stack 模拟同样的过程：
//   1. 根节点入栈
//   2. 循环：弹出栈顶 → 访问 → 先压右孩子再压左孩子
//   3. 为什么先压右再压左？栈是 LIFO，要让左孩子先出栈被处理
//
// 栈状态变化（示例2）:
//          1
//        /   \
//       2     3
//      / \     \
//     4   5     8
//        / \   /
//       6   7 9
//
// 栈(底→顶)    弹出  result
// [1]            -     []
// [3, 2]         1     [1]           ← 弹1, 压右3压左2
// [3, 5, 4]      2     [1,2]         ← 弹2, 压右5压左4
// [3, 5]         4     [1,2,4]       ← 弹4, 无孩子
// [3, 7, 6]      5     [1,2,4,5]     ← 弹5, 压右7压左6
// [3, 7]         6     [1,2,4,5,6]   ← 弹6, 无孩子
// [3]            7     [1,2,4,5,6,7] ← 弹7, 无孩子
// [8]            3     [1,2,4,5,6,7,3]     ← 弹3, 压右8(无左)
// [9]            8     [1,2,4,5,6,7,3,8]   ← 弹8, 压左9(无右)
// []             9     [1,2,4,5,6,7,3,8,9] ← 弹9, 无孩子
// 结束 ✓
// ============================================================
class Solution2 {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) return result;  // 空树直接返回

        stack<TreeNode*> stk;
        stk.push(root);

        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            result.push_back(node->val);  // 弹出时立即访问（前序）

            // 关键: 先压右再压左，保证左子树先被弹出处理
            // 如果搞反了，遍历顺序变成"根→右→左"，不是前序
            if (node->right) stk.push(node->right);
            if (node->left)  stk.push(node->left);
        }

        return result;
    }
};

// ============================================================
// 解法3: Morris 遍历 — 利用线索化实现 O(1) 空间
// 时间: O(n)  空间: O(1)  不需要栈，不需要递归
//
// 【思路】
// 递归和栈都需要 O(h) 空间来"记住回去的路"。
// Morris 的核心洞察: 叶子节点的 right 指针是 null，白白浪费了。
// 我们可以临时把它指向需要"回溯"的祖先节点，遍历完后恢复。
//
// 算法:
//   对于当前节点 curr:
//   - 若无左子树: 访问 curr，移到 curr->right
//   - 若有左子树: 找左子树的最右节点(predecessor)
//     - 若 predecessor->right == null:
//       建立线索 predecessor->right = curr
//       访问 curr（前序: 第一次到就访问!）
//       移到 curr->left
//     - 若 predecessor->right == curr:
//       左子树已遍历完，断开线索 predecessor->right = null
//       移到 curr->right
//
// 前序 vs 中序 Morris 的唯一区别:
//   前序: 建立线索时(第一次到达)就访问
//   中序: 断开线索时(第二次到达)才访问
//
// 过程示意:
//      1
//     / \
//    2   3
//   / \
//  4   5
//
// curr=1: 找左子树(2)最右节点=5, 5->right=null
//   → 建线索5->right=1, 访问1, curr=2
// curr=2: 找左子树(4)最右节点=4, 4->right=null
//   → 建线索4->right=2, 访问2, curr=4
// curr=4: 无左子树 → 访问4, curr=4->right=2(线索)
// curr=2: 找左子树(4)最右节点=4, 4->right=2(是自己!)
//   → 断开线索4->right=null, curr=5
// curr=5: 无左子树 → 访问5, curr=5->right=1(线索)
// curr=1: 找左子树(2)最右节点=5, 5->right=1(是自己!)
//   → 断开线索5->right=null, curr=3
// curr=3: 无左子树 → 访问3, curr=null → 结束
//
// 结果: [1, 2, 4, 5, 3] ✓
// ============================================================
class Solution3 {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode* curr = root;

        while (curr) {
            if (!curr->left) {
                // 无左子树: 直接访问当前节点，向右移动
                result.push_back(curr->val);
                curr = curr->right;
            } else {
                // 找左子树的最右节点 (中序前驱)
                TreeNode* predecessor = curr->left;
                // 注意: predecessor->right != curr 防止沿着线索无限循环
                while (predecessor->right && predecessor->right != curr) {
                    predecessor = predecessor->right;
                }

                if (!predecessor->right) {
                    // 第一次到达 curr: 建立线索，访问当前节点，进入左子树
                    predecessor->right = curr;
                    result.push_back(curr->val);  // 前序关键: 第一次到就访问!
                    curr = curr->left;
                } else {
                    // 第二次到达 curr: 左子树已遍历完，断开线索恢复树结构
                    predecessor->right = nullptr;
                    curr = curr->right;
                }
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间  | 空间  | 修改树 | 代码复杂度 | 面试场景        |
// |----------|-------|-------|--------|------------|-----------------|
// | 递归     | O(n)  | O(h)  | 否     | 最简单     | 先写展示理解    |
// | 迭代栈   | O(n)  | O(h)  | 否     | 中等       | 面试官期望看到  |
// | Morris   | O(n)  | O(1)  | 临时   | 较难       | 追问O(1)空间时  |
//
// 递归 vs 迭代: 本质上做同一件事，递归用系统栈，迭代用显式栈。
//   迭代版的优势是不会栈溢出（超深的树），且更体现编码能力。
// Morris: 利用空指针建立临时线索实现回溯，遍历完后树结构恢复原样。
//   代价是代码更复杂，且遍历过程中树被临时修改（并发不安全）。
//
// 【易错点】
// 1. 迭代版压栈顺序:
//    ✗ 先压 left 再压 right → 出栈顺序变成 根→右→左
//    ✓ 先压 right 再压 left → 出栈顺序才是 根→左→右
//
// 2. 迭代版忘记判空:
//    ✗ if (!root) return result; 漏掉 → 空指针入栈导致崩溃
//    ✓ 在入栈前或循环开始前检查 null
//
// 3. Morris 前序的访问时机:
//    ✗ 在断开线索时访问（那是中序遍历!）
//    ✓ 在建立线索时（第一次到达）就访问
//
// 4. Morris 找前驱时死循环:
//    ✗ while (predecessor->right) → 线索建立后沿着线索无限走
//    ✓ while (predecessor->right && predecessor->right != curr)
//
// 【面试追问】
// Q1(基础): 递归的空间复杂度是多少？最坏情况？
//   → O(h)，最坏是链状树 h=n 即 O(n)，平衡树 O(log n)
//
// Q2(Follow-up): 用迭代实现？
//   → 解法2，用显式栈，先压右再压左。关键是利用栈 LIFO 特性。
//
// Q3(进阶): 能否 O(1) 空间？
//   → Morris 遍历。利用叶子空指针建立线索。前序在建线索时访问。
//
// Q4(变体): 如何快速把前序迭代改成后序迭代？
//   → 前序"根左右" → 改压栈顺序变成"根右左"(先压左再压右)
//     → 对结果 reverse 得到"左右根" = 后序
//
// Q5(深入): Morris 遍历的时间真的是 O(n) 吗？看起来有嵌套循环。
//   → 是的。虽然有 while 找前驱，但每条边最多被访问 2 次
//     （一次找前驱，一次断开线索），总操作 O(n)。
// ============================================================
