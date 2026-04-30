/*
 * LeetCode 337: 打家劫舍 III (House Robber III)
 *
 * 【题目本质】
 * 在二叉树上做"选或不选"决策: 父子不能同时选, 求选中节点值之和的最大值。
 * 这是经典的树形 DP 问题。
 *
 * 【系列对比】
 * HR I  (198): 线性数组, dp[i] = max(dp[i-1], dp[i-2] + nums[i])
 * HR II (213): 环形数组, 拆成两次线性 DP
 * HR III(337): 二叉树,   后序遍历, 每个节点返回 (偷, 不偷) 两种状态  ← 本题
 *              从数组的"下标递推"变成了树的"后序遍历收集子节点信息"
 *
 * 【解法总览】
 * 解法1: 暴力递归 (跳孙子)      — O(2^n) / O(h) — 最直觉但超时
 * 解法2: 记忆化搜索             — O(n)   / O(n) — 加缓存消除重复
 * 解法3: 树形 DP (返回 pair)    — O(n)   / O(h) — 最优, 面试首选
 */

// ============================================================
// 解法1: 暴力递归 — 对每个节点考虑"偷"或"不偷", 偷则跳到孙子
// 时间: O(2^n)  空间: O(h) 递归栈
//
// 【思路】
// 最自然的想法:
//   偷 node → node.val + rob(孙子们)
//   不偷 node → rob(子节点们)
// 问题: rob(left) 内部又会递归到孙子, 和外层的 rob(孙子) 重复。
//
// 【树结构 & 重复计算示意】
//         3              rob(3) 会算 rob(2) 和 rob(3右)
//        / \             rob(3) 偷root时还会算 rob(3左孙) rob(1)
//       2   3            而 rob(2) 内部也会算 rob(3左孙)
//        \   \           → 3左孙被重复计算!
//         3   1
// ============================================================
class Solution1 {
public:
    int rob(TreeNode* root) {
        if (!root) return 0;

        // 选择1: 偷当前节点, 子节点不能偷, 跳到孙子
        int robCurr = root->val;
        if (root->left) {
            robCurr += rob(root->left->left) + rob(root->left->right);
        }
        if (root->right) {
            robCurr += rob(root->right->left) + rob(root->right->right);
        }

        // 选择2: 不偷当前节点, 递归子节点
        int skipCurr = rob(root->left) + rob(root->right);

        return max(robCurr, skipCurr);
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 用 HashMap 缓存每个节点的结果
// 时间: O(n)  空间: O(n) HashMap + O(h) 递归栈
//
// 【思路】
// 解法1 的瓶颈: 同一个节点被反复计算 (重叠子问题)。
// 解决: 用 unordered_map<TreeNode*, int> 记录已经算过的节点。
// 每个节点最多计算一次, 所以时间 O(n)。
//
// 这和 HR I 中从暴力递归加 memo 数组是完全一样的套路。
// ============================================================
class Solution2 {
public:
    unordered_map<TreeNode*, int> memo;

    int rob(TreeNode* root) {
        if (!root) return 0;
        // 命中缓存直接返回, 避免重复计算
        if (memo.count(root)) return memo[root];

        int robCurr = root->val;
        if (root->left) {
            robCurr += rob(root->left->left) + rob(root->left->right);
        }
        if (root->right) {
            robCurr += rob(root->right->left) + rob(root->right->right);
        }

        int skipCurr = rob(root->left) + rob(root->right);

        memo[root] = max(robCurr, skipCurr);
        return memo[root];
    }
};

// ============================================================
// 解法3: 树形 DP — 每个节点返回 (偷自己, 不偷自己) 两个状态
// 时间: O(n)  空间: O(h) 仅递归栈
//
// 【思路】
// 核心洞察: 与其"跳孙子", 不如让每个节点直接告诉父节点两种状态:
//   - rob:    偷我时, 我这棵子树能得多少
//   - notRob: 不偷我时, 我这棵子树能得多少
//
// 状态转移:
//   rob_node    = node.val + notRob_left + notRob_right
//                (偷 node, 子不能偷)
//   notRob_node = max(rob_left, notRob_left) + max(rob_right, notRob_right)
//                (不偷 node, 每个子各自取最优 — 注意: 不偷 node 不意味着一定偷子!)
//
// 后序遍历: 先算左右子树的状态, 再合并得到当前节点的状态。
//
// 和 HR I 空间优化的类比:
//   HR I: dp[i] 只依赖 dp[i-1] 和 dp[i-2], 用两个变量滚动
//   HR III: 每个节点只依赖左右子的两个状态, 通过返回值传递, 用完即丢
//
// 【后序遍历过程 (示例1)】
//
//         3                   后序: 叶子先, 根后
//        / \
//       2   3
//        \   \
//         3   1
//
// 节点3(左孙, 叶子): rob=3, notRob=0 → [3, 0]
// 节点1(右孙, 叶子): rob=1, notRob=0 → [1, 0]
// 节点2:
//   left = null → [0, 0]
//   right = 节点3 → [3, 0]
//   rob_2    = 2 + 0 + 0 = 2
//   notRob_2 = max(0,0) + max(3,0) = 3
//   → [2, 3]
// 节点3(右子):
//   left = null → [0, 0]
//   right = 节点1 → [1, 0]
//   rob_3    = 3 + 0 + 0 = 3
//   notRob_3 = max(0,0) + max(1,0) = 1
//   → [3, 1]
// 根节点3:
//   left = 节点2 → [2, 3]
//   right = 节点3右 → [3, 1]
//   rob_root    = 3 + 3 + 1 = 7      (偷root, 取子的notRob)
//   notRob_root = max(2,3) + max(3,1) = 3 + 3 = 6
//   → [7, 6]
// 答案 = max(7, 6) = 7 ✓
//
// 【后序遍历过程 (示例2)】
//
//         3
//        / \
//       4   5
//      / \   \
//     1   3   1
//
// 节点1(叶子): [1, 0]
// 节点3(叶子): [3, 0]
// 节点1(右叶): [1, 0]
// 节点4:
//   left = 节点1 → [1, 0], right = 节点3 → [3, 0]
//   rob_4    = 4 + 0 + 0 = 4
//   notRob_4 = max(1,0) + max(3,0) = 4
//   → [4, 4]
// 节点5:
//   left = null → [0, 0], right = 节点1 → [1, 0]
//   rob_5    = 5 + 0 + 0 = 5
//   notRob_5 = max(0,0) + max(1,0) = 1
//   → [5, 1]
// 根节点3:
//   left = 节点4 → [4, 4], right = 节点5 → [5, 1]
//   rob_root    = 3 + 4 + 1 = 8
//   notRob_root = max(4,4) + max(5,1) = 4 + 5 = 9
//   → [8, 9]
// 答案 = max(8, 9) = 9 ✓
// ============================================================
class Solution3 {
public:
    int rob(TreeNode* root) {
        auto [robRoot, notRobRoot] = dfs(root);
        return max(robRoot, notRobRoot);
    }

private:
    // 返回 {偷当前节点的最大收益, 不偷当前节点的最大收益}
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};

        auto [robLeft, notRobLeft] = dfs(node->left);
        auto [robRight, notRobRight] = dfs(node->right);

        // 偷当前节点: 左右子不能偷
        int robCurr = node->val + notRobLeft + notRobRight;

        // 不偷当前节点: 左右子各自取最优
        // 注意: 不是 robLeft + robRight! 子节点也可以不偷
        int notRobCurr = max(robLeft, notRobLeft) + max(robRight, notRobRight);

        return {robCurr, notRobCurr};
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | 时间   | 空间   | 核心差异                          |
// |-----------|--------|--------|-----------------------------------|
// | 暴力递归  | O(2^n) | O(h)   | 跳孙子 → 重复计算                 |
// | 记忆化    | O(n)   | O(n)   | HashMap 缓存 → 空间开销大         |
// | 树形 DP   | O(n)   | O(h)   | 返回值传状态 → 不需要额外存储     |
//
// 面试首选解法3: 代码最简洁, 空间最优, 体现"树形DP"思维。
//
// 和 House Robber I 的代码对比:
//   HR I:  prev2, prev1 滚动 → dp[i] = max(prev1, prev2 + nums[i])
//   HR III: pair<int,int> dfs(node) → 每个节点"就地"决策, 信息通过返回值向上传递
//   本质一样: 每个位置维护"选/不选"两种状态, 只是数据结构不同。
//
// ============================================================
// 【易错点】
//
// 1. 不偷当前节点时, 错误地强制偷子节点:
//    ✗ notRobCurr = robLeft + robRight
//    ✓ notRobCurr = max(robLeft, notRobLeft) + max(robRight, notRobRight)
//    原因: 不偷 node 不意味着一定偷 left/right。left 的孙子可能更值钱。
//
// 2. 暴力解中直接访问孙子不检查 null:
//    ✗ robCurr += rob(root->left->left);  // root->left 可能是 null!
//    ✓ if (root->left) { robCurr += rob(root->left->left) + ... }
//
// 3. dfs 返回类型搞反:
//    ✗ return {notRobCurr, robCurr};  // 顺序反了
//    ✓ return {robCurr, notRobCurr};  // 和解构赋值的顺序一致
//    建议: 用结构化绑定 auto [rob, notRob] = dfs(node) 让语义更清晰
//
// ============================================================
// 【面试追问】
//
// Q1 (基础): 和 House Robber I 什么关系?
//    → 核心框架一样 (选/不选 DP), 数据结构从数组变成树,
//      递推变成后序遍历, 滚动变量变成 pair 返回值。
//
// Q2 (优化): 为什么解法3比解法2好?
//    → 解法2 用 O(n) HashMap 存所有节点的结果;
//      解法3 通过返回值传递, 空间只有 O(h) 递归栈。
//      类比: HR I 中用滚动变量代替整个 dp 数组。
//
// Q3 (变体): 如果不是二叉树而是 N 叉树怎么办?
//    → 完全一样! dfs 遍历所有 children 而不是只遍历 left/right:
//      for (auto child : node->children) {
//          auto [rc, nrc] = dfs(child);
//          robCurr += nrc; notRobCurr += max(rc, nrc);
//      }
//
// Q4 (进阶): 如果图中有环 (不是树) 怎么办?
//    → 这变成最大权独立集问题, 在一般图上是 NP-hard。
//      树上能 O(n) 解, 正是因为树无环、有唯一的子树分解方式。
// ============================================================
