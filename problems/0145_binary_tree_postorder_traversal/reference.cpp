/*
 * LeetCode 145: 二叉树的后序遍历 (Binary Tree Postorder Traversal)
 *
 * 【题目本质】
 * 按"左→右→根"的顺序访问二叉树所有节点。
 * 递归版本是基础，迭代版本是面试重点（Follow-up 明确要求）。
 *
 * 【解法总览】
 * 解法1: 递归          — O(n) / O(h) — 最直觉，三行核心代码
 * 解法2: 迭代(反转前序) — O(n) / O(n) — 面试首选，最容易写对
 * 解法3: 迭代(prev指针) — O(n) / O(h) — 真正的后序迭代，面试追问
 * 解法4: Morris后序     — O(n) / O(1) — 加分项，极少要求手写
 *
 * 【树结构示意 — 示例2】
 *
 *           1
 *          / \
 *         2   3
 *        / \    \
 *       4   5    8
 *          / \  /
 *         6   7 9
 *
 * 后序遍历结果: [4, 6, 7, 5, 2, 9, 8, 3, 1]
 * 规律：总是先深入到最左最底，逐层"回收"
 */

// ============================================================
// 解法1: 递归 — 直接按后序定义递归
// 时间: O(n)  空间: O(h) — 递归栈深度等于树高
//
// 【思路】
// 后序遍历的定义本身就是递归的：
//   1. 递归遍历左子树
//   2. 递归遍历右子树
//   3. 访问当前节点
// 递归三要素：
//   返回值: void (结果通过引用收集)
//   终止条件: node == nullptr
//   单层逻辑: 左递归 → 右递归 → 记录值
//
// 【遍历过程 — 示例1: [1,null,2,3]】
//     1
//      \
//       2
//      /
//     3
//
// dfs(1):
//   dfs(null) → 返回
//   dfs(2):
//     dfs(3):
//       dfs(null) → 返回
//       dfs(null) → 返回
//       记录 3  → result = [3]
//     dfs(null) → 返回
//     记录 2  → result = [3, 2]
//   记录 1  → result = [3, 2, 1]  ✓
// ============================================================
class Solution1 {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(TreeNode* node, vector<int>& result) {
        if (!node) return;               // 终止条件：空节点直接返回
        dfs(node->left, result);         // 先递归左子树
        dfs(node->right, result);        // 再递归右子树
        result.push_back(node->val);     // 最后记录当前节点（这就是"后序"）
    }
};

// ============================================================
// 解法2: 迭代（反转前序）— 面试首选，最容易写对
// 时间: O(n)  空间: O(n) — 栈最坏存 n 个节点 + 反转
//
// 【思路】
// 直接迭代实现后序很复杂（根要最后处理，但最先遇到）。
// 换个角度：
//   后序:     左 → 右 → 根
//   反转后序: 根 → 右 → 左  ← 这是"修改版前序"！
//
// 前序迭代（根→左→右）只需要：
//   pop根, push右, push左 → 出栈顺序 = 根→左→右
//
// 修改版前序（根→右→左）：
//   pop根, push左, push右 → 出栈顺序 = 根→右→左
//
// 最后 reverse 结果，就得到后序（左→右→根）。
//
// 【和144题(前序)的对比】
// 前序: push右 → push左 → 不反转 → 根左右
// 后序: push左 → push右 → 反转   → 左右根
// 区别就两处：入栈顺序交换 + 最后反转
//
// 【遍历过程 — 示例1: [1,null,2,3]】
//     1
//      \
//       2
//      /
//     3
//
// 操作               栈(底→顶)    result
// push(1)           [1]          []
// pop(1), push左无   []           [1]
//   push右(2)       [2]          [1]
// pop(2), push左(3) [3]          [1,2]
//   push右无        [3]          [1,2]
// pop(3)            []           [1,2,3]
//
// reverse: [3,2,1] ← 后序 ✓
// ============================================================
class Solution2 {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) return result;

        stack<TreeNode*> stk;
        stk.push(root);

        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            result.push_back(node->val);   // 收集顺序: 根→右→左

            // 先压左后压右 → 出栈时右先出
            // 所以收集顺序是 根→右→左
            if (node->left)  stk.push(node->left);   // 左先压 → 后出
            if (node->right) stk.push(node->right);  // 右后压 → 先出
        }

        // 关键一步：反转 "根→右→左" 得到 "左→右→根" = 后序
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 解法3: 迭代（prev 指针）— 真正的后序迭代
// 时间: O(n)  空间: O(h) — 栈深度最大为树高
//
// 【思路】
// 核心难点：对于栈顶节点，怎么判断它的右子树是否已经处理完？
//
// 方案：维护 prev 指针，指向上一个被"处理"（加入result）的节点。
// 对于栈顶 curr:
//   - 如果 curr 没有右孩子 → 右子树不存在，可以处理 curr
//   - 如果 curr->right == prev → 右孩子刚被处理，右子树已完成，可以处理 curr
//   - 否则 → 右子树还没处理，转向 curr->right
//
// 流程和中序遍历类似：一路向左压栈，但弹出时多了一个判断。
//
// 【和94题(中序)的对比】
// 中序: 弹出时直接处理，然后转右子树
// 后序: 弹出时要检查右子树是否完成，完成才处理
// 区别就在弹出时多了一个 if 判断
//
// 【详细过程 — 示例2: 部分展示】
//           1
//          / \
//         2   3
//        / \    \
//       4   5    8
//          / \  /
//         6   7 9
//
// Step  栈(底→顶)     curr  prev  动作            result
// 1     [1,2,4]       4     null  一路向左          -
// 2     [1,2]         4     null  4无右→处理4       [4]
// 3     [1,2]         null  4     top=2,右=5≠prev   -
// 4     [1,2,5,6]     6     4     curr=5,一路向左   -
// 5     [1,2,5]       6     4     6无右→处理6       [4,6]
// 6     [1,2,5]       null  6     top=5,右=7≠prev   -
// 7     [1,2,5,7]     7     6     curr=7,无左       -
// 8     [1,2,5]       7     6     7无右→处理7       [4,6,7]
// 9     [1,2]         null  7     top=5,右=7==prev   -
//       → 处理5                                     [4,6,7,5]
// 10    [1]           null  5     top=2,右=5==prev  [4,6,7,5,2]
// ...   → 处理右子树3/8/9，最后处理1
// 最终: [4,6,7,5,2,9,8,3,1] ✓
// ============================================================
class Solution3 {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) return result;

        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;  // 上一个被加入 result 的节点

        while (curr || !stk.empty()) {
            // 阶段1: 一路向左，所有左节点入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }

            // 阶段2: 看栈顶节点（不要急着 pop）
            curr = stk.top();

            if (!curr->right || curr->right == prev) {
                // 可以处理当前节点:
                //   没有右孩子 — 不需要等
                //   右孩子 == prev — 右子树已处理完
                result.push_back(curr->val);
                stk.pop();
                prev = curr;     // 标记"我刚被处理了"
                curr = nullptr;  // 关键！不要再向左走，回去看栈顶
                                 // 如果不置空，下一轮 while(curr) 又会向左压栈，死循环
            } else {
                // 右子树还没处理 → 转向右子树
                curr = curr->right;
            }
        }

        return result;
    }
};

// ============================================================
// 解法4: Morris 后序遍历 — O(1) 空间
// 时间: O(n)  空间: O(1) — 不用栈，利用线索化
//
// 【思路】
// Morris 遍历的核心：利用叶子节点空闲的 right 指针，
// 临时建立"线索"指回祖先，遍历完后恢复。
//
// 后序的 Morris 是最复杂的：
// 1. 创建虚拟根节点 dummy，dummy->left = root
// 2. 对 dummy 做类似中序 Morris 的遍历
// 3. 断开线索时，反向输出从 curr->left 到前驱的右边界路径
//
// 这个解法面试中极少要求手写，能说清原理即可。
// ============================================================
class Solution4 {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode dummy(0);
        dummy.left = root;
        TreeNode* curr = &dummy;

        while (curr) {
            if (!curr->left) {
                // 没有左子树，直接向右
                curr = curr->right;
            } else {
                // 找到左子树的最右节点（中序前驱）
                TreeNode* pred = curr->left;
                while (pred->right && pred->right != curr) {
                    pred = pred->right;
                }

                if (!pred->right) {
                    // 第一次到达：建立线索
                    pred->right = curr;
                    curr = curr->left;
                } else {
                    // 第二次到达：断开线索
                    // 反向输出从 curr->left 到 pred 路径上的节点值
                    addReversePath(curr->left, pred, result);
                    pred->right = nullptr;  // 恢复树结构
                    curr = curr->right;
                }
            }
        }
        return result;
    }

private:
    // 反向输出从 from 到 to 的右边界（通过 right 指针连接的链）
    void addReversePath(TreeNode* from, TreeNode* to, vector<int>& result) {
        reversePath(from, to);           // 反转链
        TreeNode* node = to;
        while (true) {
            result.push_back(node->val);
            if (node == from) break;
            node = node->right;
        }
        reversePath(to, from);           // 恢复原链
    }

    // 反转从 from 到 to 的 right 指针链
    void reversePath(TreeNode* from, TreeNode* to) {
        if (from == to) return;
        TreeNode* prev = from;
        TreeNode* curr = from->right;
        while (prev != to) {
            TreeNode* next = curr->right;
            curr->right = prev;
            prev = curr;
            curr = next;
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间 | 空间 | 真后序? | 代码难度 | 面试推荐 |
// |---------------|------|------|---------|----------|----------|
// | 递归          | O(n) | O(h) |   ✓    |  ★☆☆☆  | 暖场    |
// | 反转前序      | O(n) | O(n) |   ✗    |  ★★☆☆  | 首选    |
// | prev 指针     | O(n) | O(h) |   ✓    |  ★★★☆  | 追问    |
// | Morris        | O(n) | O(1) |   ✓    |  ★★★★  | 加分项  |
//
// 解法2(反转前序) vs 解法3(prev指针):
//   - 反转前序简单好写，但不是真正的后序（全部收集完才反转）
//   - prev 指针是真后序，可以边遍历边处理，适合 stream 场景
//   - 面试先给解法2，追问时给解法3
//
// 三种迭代遍历的统一视角：
//   前序: pop时处理, push右→push左
//   中序: 一路向左压栈, pop时处理, 转右
//   后序: 一路向左压栈, peek时判断右子树是否完成, 完成才pop处理
//
// ============================================================
// 【易错点】
//
// 1. 解法2中入栈顺序:
//    ✗ push右→push左 → 出栈:左先右后 → 收集:根左右 → 反转:右左根 ≠ 后序
//    ✓ push左→push右 → 出栈:右先左后 → 收集:根右左 → 反转:左右根 = 后序
//
// 2. 解法3中忘记 curr = nullptr:
//    ✗ 处理完节点后 curr 仍指向该节点
//      → 下一轮 while(curr) 又向左压栈 → 死循环/重复
//    ✓ prev = curr; curr = nullptr;
//      → 下一轮跳过 while(curr)，直接看栈顶
//
// 3. 解法3中先 pop 再判断:
//    ✗ node = stk.top(); stk.pop();
//      if (需要转右) → node 已经 pop 出来了，没法回去
//    ✓ 先 top() 看一眼，确认可以处理后才 pop()
//
// 4. 递归中 push_back 位置:
//    ✗ push_back 放在递归前 → 变成前序
//    ✗ push_back 放在两次递归之间 → 变成中序
//    ✓ push_back 放在两次递归之后 → 后序
//
// ============================================================
// 【面试追问】
//
// Q1(基础): 为什么后序遍历的迭代比前序和中序都难？
// → 因为后序要求根节点最后处理，但遍历时根最先被遇到且入栈。
//   前序：遇到就处理；中序：弹出时处理；后序：弹出时还不确定能否处理
//   （要等右子树完成），这个"等待"增加了实现复杂度。
//
// Q2(优化): 你的反转法需要 O(n) 空间存结果才能反转，能否避免？
// → 用 prev 指针法（解法3），实现真正的后序迭代，
//   栈空间只需 O(h)，不需要反转。
//
// Q3(进阶): 能否 O(1) 空间完成？
// → Morris 后序遍历。利用叶节点空闲的 right 指针建立线索，
//   断开线索时反向输出右边界路径。代码较复杂但确实 O(1) 空间。
//
// Q4(变体): 如果要求后序遍历的第 K 个元素，怎么提前终止？
// → 不能用反转法（需要全部收集才能反转），必须用 prev 指针法
//   或递归。维护一个计数器，每次 push_back 时 count++，
//   当 count == k 时立刻返回（递归中可以用全局标志提前终止）。
// ============================================================
