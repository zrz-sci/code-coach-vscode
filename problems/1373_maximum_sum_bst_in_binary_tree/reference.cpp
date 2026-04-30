/*
 * LeetCode 1373: Maximum Sum BST in Binary Tree
 * 二叉搜索子树的最大键值和
 *
 * 难度: Hard
 * 标签: Tree, DFS, BST, Binary Tree
 *
 * 思路: 后序遍历 + 自底向上信息聚合
 * 每个节点返回四元组 {isBST, sum, minVal, maxVal}
 * 自底向上判断当前子树是否为BST，若是则更新全局最大和
 *
 * 时间: O(n) -- 每个节点访问一次
 * 空间: O(h) -- 递归栈深度等于树高
 */

// ======================= 解法: 后序DFS + 信息传递 =======================

class Solution {
    int ans = 0; // 全局答案，初始化为 0（空BST的和为0）

    /*
     * dfs 返回四元组:
     *   isBST  -- 以 node 为根的子树是否是合法 BST
     *   sum    -- 子树所有节点值之和（仅在 isBST 时有意义）
     *   minVal -- 子树中的最小值（用于父节点检查右子树条件）
     *   maxVal -- 子树中的最大值（用于父节点检查左子树条件）
     *
     * 空节点返回 {true, 0, INT_MAX, INT_MIN}:
     *   - true: 空树是合法 BST
     *   - INT_MAX 作为 min: 使得 "node->val > left.max" 对叶节点成立
     *     因为左子空时 left.max = INT_MIN，任何 val > INT_MIN
     *   - INT_MIN 作为 max: 使得 "node->val < right.min" 对叶节点成立
     *     因为右子空时 right.min = INT_MAX，任何 val < INT_MAX
     */
    tuple<bool, int, int, int> dfs(TreeNode* node) {
        // 递归基: 空节点
        if (!node) return {true, 0, INT_MAX, INT_MIN};

        // 后序遍历: 先递归处理左右子树
        auto [lBST, lSum, lMin, lMax] = dfs(node->left);
        auto [rBST, rSum, rMin, rMax] = dfs(node->right);

        /*
         * 判断以 node 为根的子树是否为 BST:
         * 条件1: 左子树是 BST
         * 条件2: 右子树是 BST
         * 条件3: node->val > 左子树最大值 (严格大于)
         * 条件4: node->val < 右子树最小值 (严格小于)
         *
         * 注意: BST 定义是严格不等，不含等号!
         */
        if (lBST && rBST && node->val > lMax && node->val < rMin) {
            // 当前子树是合法 BST
            int sum = lSum + rSum + node->val;

            // 更新全局最大和
            ans = max(ans, sum);

            // 向上返回当前子树的信息
            // min: 整棵子树的最小值 = min(左子树最小值, 当前节点值)
            // max: 整棵子树的最大值 = max(右子树最大值, 当前节点值)
            return {true, sum, min(lMin, node->val), max(rMax, node->val)};
        }

        // 不是 BST: 包含此节点的更大子树也不可能是 BST
        // sum/min/max 不会被使用，返回任意值
        return {false, 0, 0, 0};
    }

public:
    int maxSumBST(TreeNode* root) {
        dfs(root);
        return ans;
    }
};

/*
 * ======================== 执行过程示例 ========================
 *
 * Example 1: root = [1,4,3,2,4,2,5,null,null,null,null,null,null,4,6]
 *
 *               1
 *             /   \
 *           4       3
 *          / \     / \
 *         2   4   2   5
 *                    / \
 *                   4   6
 *
 * 后序遍历过程 (叶节点 -> 根):
 *
 * 节点2(左4的左子): {true, 2, 2, 2}
 * 节点4(左4的右子): {true, 4, 4, 4}
 * 节点4(根1的左子):
 *   检查: 4 > lMax=2? YES, 4 < rMin=4? NO (不是严格小于)
 *   返回: {false, 0, 0, 0}
 *
 * 节点2(右3的左子): {true, 2, 2, 2}
 * 节点4(节点5的左子): {true, 4, 4, 4}
 * 节点6(节点5的右子): {true, 6, 6, 6}
 * 节点5:
 *   检查: 5 > 4? YES, 5 < 6? YES --> BST!
 *   sum = 4 + 6 + 5 = 15, ans = max(0, 15) = 15
 *   返回: {true, 15, 4, 6}
 *
 * 节点3:
 *   左: {true, 2, 2, 2}, 右: {true, 15, 4, 6}
 *   检查: 3 > 2? YES, 3 < 4? YES --> BST!
 *   sum = 2 + 15 + 3 = 20, ans = max(15, 20) = 20
 *   返回: {true, 20, 2, 6}
 *
 * 节点1(根):
 *   左: {false, ...} --> 不是 BST
 *   返回: {false, 0, 0, 0}
 *
 * 最终 ans = 20 ✓
 *
 *
 * ======================== 边界情况 ========================
 *
 * 1. 全负数: root = [-4, -2, -5]
 *    所有单节点 BST 的 sum 都为负 -> ans 保持 0
 *    答案 = 0 (空BST)
 *
 * 2. 单节点: root = [5]
 *    dfs(5): left=空, right=空
 *    5 > INT_MIN && 5 < INT_MAX -> true
 *    sum = 5, ans = 5
 *
 * 3. 完整 BST: root = [2, 1, 3]
 *    整棵树就是 BST, sum = 6
 *
 *
 * ======================== 关键点总结 ========================
 *
 * 1. 空节点返回 {true, 0, INT_MAX, INT_MIN}
 *    - min=INT_MAX, max=INT_MIN 是"哨兵值"，使叶节点判断自然成立
 *
 * 2. ans 初始化为 0，不是 INT_MIN
 *    - 题目要求: 如果没有正数和的BST，返回 0
 *
 * 3. BST 条件是严格不等: > 和 <，不是 >= 和 <=
 *
 * 4. 非BST节点: 一旦某子树不是BST，包含它的所有祖先子树也不是BST
 *    后续传递的 sum/min/max 无意义
 *
 * 5. 整数范围: 节点值 [-4*10^4, 4*10^4]，最多 4*10^4 节点
 *    max sum = 4*10^4 * 4*10^4 = 1.6*10^9，int 范围内安全
 *
 *
 * ======================== 变体思考 ========================
 *
 * Q: 如果改成"最大 BST 子树的节点数"而非"键值和"呢？
 * A: 同样的框架，四元组改为 {isBST, nodeCount, min, max}
 *    即 LeetCode 333: Largest BST Subtree
 *
 * Q: 如果 BST 允许等号（<=, >=）呢？
 * A: 将条件改为 node->val >= lMax && node->val <= rMin
 *    但要注意空节点的 min/max 设置
 *
 *
 * ======================== 总结 ========================
 *
 * 【解法对比】
 *   本题只有一种核心解法: 后序 DFS + 自底向上信息聚合
 *   - 后序遍历保证先拿到左右子树信息再判断当前节点
 *   - 四元组 {isBST, sum, min, max} 一次遍历完成所有计算
 *   - 时间 O(n)，空间 O(h)，已是最优
 *   - 如果用"先判 BST 再求和"的两步做法，最坏 O(n^2)（退化链表）
 *
 * 【易错点】
 *   1. 空节点 min/max 设反: 应该 min=INT_MAX, max=INT_MIN（哨兵值）
 *      设反后叶节点的 BST 判断会失败
 *   2. ans 初始化为 INT_MIN 而非 0: 题目要求全负数时返回 0（空 BST）
 *   3. BST 条件用了 >= 或 <=: 本题严格要求 > 和 <，等号不算 BST
 *   4. 非 BST 节点仍然传播 sum/min/max: 一旦 isBST=false，
 *      后续值不被使用，但初学者常纠结于此浪费时间
 *   5. 整数溢出: 节点值 [-4*10^4, 4*10^4]，最多 4*10^4 节点，
 *      最大 sum ~ 1.6*10^9，int 够用但接近边界，面试时提一句更安全
 *
 * 【面试追问】
 *   Q1: 如果要求返回最大 BST 子树的节点数而非键值和，怎么改？
 *       → 四元组改为 {isBST, count, min, max}，合并时 count = lCount + rCount + 1
 *       → 即 LeetCode 333: Largest BST Subtree
 *
 *   Q2: 如果树非常深（百万层），递归栈溢出怎么办？
 *       → 方案1: 将递归改为迭代后序遍历，用显式栈 + 状态机
 *       → 方案2: Morris 遍历（但后序 Morris 较复杂，面试不推荐）
 *       → 实际工程中设置系统栈大小或使用协程
 *
 *   Q3: 如果允许修改一个节点的值使其变成 BST，最大和是多少？
 *       → 这是一个更难的变体，需要对每个非 BST 子树考虑:
 *         哪个节点破坏了 BST 性质，修改它后是否能使子树合法
 *       → 可以在 DFS 中额外记录"差一个违规节点"的状态
 *       → 开放性问题，面试中展示思路即可，不需要完整代码
 */
