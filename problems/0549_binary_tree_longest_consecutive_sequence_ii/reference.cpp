/*
 * LeetCode 549: 二叉树最长连续序列 II
 *              (Binary Tree Longest Consecutive Sequence II)
 *
 * 【题目本质】
 * 在二叉树中找最长的连续路径（值差为1），路径可递增或递减，
 * 可经过"子-父-子"拐点。
 * 每个节点维护 (inc, dec) 两个方向信息，合并更新全局答案。
 *
 * 【解法总览】
 * 解法1: DFS 返回 (inc, dec) — O(n) / O(n) — ⭐ 面试首选
 * 解法2: DFS 返回 pair（不用全局变量）— O(n) / O(n)
 *
 *  示例:
 *       2         inc=2(2→3), dec=2(2→1)
 *      / \        最长路径 = 2+2-1 = 3 (即 1→2→3)
 *     1   3
 */

// ============================================================
// 解法1: DFS 返回 (inc, dec) + 全局变量 ⭐ 面试首选
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 后序遍历，每个节点计算:
//   inc = 以该节点为顶端，向下的最长递增长度
//   dec = 以该节点为顶端，向下的最长递减长度
//
// 如果 left.val == node.val + 1 → inc 可从左子递增延伸
// 如果 left.val == node.val - 1 → dec 可从左子递减延伸
// 右子同理。
//
// 全局答案 = max(inc + dec - 1)，在每个节点处更新。
// -1 是因为 node 在 inc 和 dec 中各被计一次。
//
//  与 543 直径完全相同的模式：
//  - 返回值 (inc, dec) 给父节点用
//  - 全局答案在每个节点内部更新
//  - 返回值 ≠ 答案
// ============================================================
class Solution1 {
public:
    int longestConsecutive(TreeNode* root) {
        int ans = 0;
        dfs(root, ans);
        return ans;
    }

private:
    // 返回 {inc, dec}
    pair<int, int> dfs(TreeNode* node, int& ans) {
        if (!node) return {0, 0};

        int inc = 1, dec = 1;  // 至少包含自身

        auto [li, ld] = dfs(node->left, ans);
        auto [ri, rd] = dfs(node->right, ans);

        if (node->left) {
            if (node->left->val == node->val + 1)
                inc = max(inc, li + 1);  // 左子递增延伸
            if (node->left->val == node->val - 1)
                dec = max(dec, ld + 1);  // 左子递减延伸
        }

        if (node->right) {
            if (node->right->val == node->val + 1)
                inc = max(inc, ri + 1);  // 右子递增延伸
            if (node->right->val == node->val - 1)
                dec = max(dec, rd + 1);  // 右子递减延伸
        }

        // 合并: 经过当前节点的最长路径
        ans = max(ans, inc + dec - 1);

        return {inc, dec};
    }
};

// ============================================================
// 解法2: DFS 返回三元组（不用全局变量）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 返回 {inc, dec, maxLen}，maxLen 是子树内的最长路径。
// 这样不需要引用参数或全局变量。
// ============================================================
class Solution2 {
public:
    int longestConsecutive(TreeNode* root) {
        auto [inc, dec, ans] = dfs(root);
        return ans;
    }

private:
    // 返回 {inc, dec, maxLen}
    // inc: 以 node 为端的最长递增
    // dec: 以 node 为端的最长递减
    // maxLen: 整个子树内的最长连续路径
    tuple<int, int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0, 0};

        int inc = 1, dec = 1;

        auto [li, ld, lmax] = dfs(node->left);
        auto [ri, rd, rmax] = dfs(node->right);

        if (node->left) {
            if (node->left->val == node->val + 1)
                inc = max(inc, li + 1);
            if (node->left->val == node->val - 1)
                dec = max(dec, ld + 1);
        }

        if (node->right) {
            if (node->right->val == node->val + 1)
                inc = max(inc, ri + 1);
            if (node->right->val == node->val - 1)
                dec = max(dec, rd + 1);
        }

        int curLen = inc + dec - 1;
        int maxLen = max({curLen, lmax, rmax});

        return {inc, dec, maxLen};
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间  | 空间  | 特点                       |
// |---------------|-------|-------|---------------------------|
// | 全局变量版     | O(n) | O(n) | ⭐ 代码简洁，面试首选       |
// | 三元组返回版   | O(n) | O(n) | 无全局变量，函数式风格      |
//
// 核心模式：「返回值给父节点用，全局答案在节点内部更新」
// 和 543 直径、124 最大路径和完全相同。
//
// ============================================================
// 【易错点】
//
// 1. ✗ 只考虑单方向（只维护 inc 或 dec）
//    ✓ 必须同时维护两个方向，路径可经过"子-父-子"拐点
//
// 2. ✗ 合并时 ans = inc + dec（漏掉 -1）
//    ✓ ans = inc + dec - 1，node 在两侧各被计一次
//
// 3. ✗ 方向搞反: child.val == node.val + 1 更新 dec
//    ✓ child.val == node.val + 1 → 从 node 到 child 是递增
//    → 更新 inc（递增方向）
//
// 4. ✗ 忘记检查 node->left/right 是否为 null
//    ✓ 先判空再比较值，否则访问空指针
//
// ============================================================
// 【面试追问】
//
// Q1: 和 298 题的区别？
// → 298 只要从上到下的递增路径，只需一个方向。
//   549 允许递增/递减且可经过拐点，需要 (inc, dec) 双向。
//
// Q2: 为什么 inc + dec - 1 是正确的？
// → inc 是从 node 往一侧延伸的递增长度，
//   dec 是从 node 往另一侧延伸的递减长度。
//   合起来形成完整路径，node 被计两次所以 -1。
//
// Q3: 路径一定经过某个"拐点"吗？
// → 不一定。纯递增或纯递减路径也是合法的。
//   此时 inc 或 dec 为 1，ans = max(inc, dec)。
//
// 【相关题型】
// - 298. 二叉树最长连续序列 — 简化版（单方向）
// - 543. 二叉树的直径 — 同模式
// - 124. 二叉树中的最大路径和 — 同模式，求值之和
// - 687. 最长同值路径 — 同模式，条件改为值相等
// ============================================================
