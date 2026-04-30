/*
 * LeetCode 671: 二叉树中第二小的节点 (Second Minimum Node In a Binary Tree)
 * 难度: Easy
 * 标签: Tree, DFS, Binary Tree
 *
 * 【题目本质】
 * 特殊二叉树中 root.val = min(left.val, right.val)，
 * 找全局第二小的值。root.val 就是最小值，找最小的 > root.val 的值。
 *
 * 【解法总览】
 * 解法一: DFS + 剪枝    O(n) / O(h)
 * 解法二: BFS + 剪枝    O(n) / O(n)
 */


/*
 * ============================================================
 * 解法一: DFS + 剪枝
 * ============================================================
 *
 * 思路:
 *   root.val 就是最小值 minVal。
 *   DFS 遍历，找最小的 > minVal 的值:
 *   - node.val > minVal: 更新候选答案，不继续深入(子节点更大)
 *   - node.val == minVal: 继续深入子节点寻找
 *
 * 示例: [2, 2, 5, null, null, 5, 7]
 *         2
 *        / \
 *       2   5
 *          / \
 *         5   7
 *   minVal = 2
 *   DFS:
 *     node(2): val==minVal, 深入左右
 *       node(2): val==minVal, 叶子, 返回
 *       node(5): val>minVal, ans=min(MAX,5)=5, 剪枝返回
 *   ans = 5
 *
 * 时间: O(n) 最坏, 空间: O(h) 递归栈
 */
class Solution1 {
public:
    int findSecondMinimumValue(TreeNode* root) {
        long long ans = LLONG_MAX;
        int minVal = root->val;
        dfs(root, minVal, ans);
        return ans == LLONG_MAX ? -1 : (int)ans;
    }

private:
    void dfs(TreeNode* node, int minVal, long long& ans) {
        if (!node) return;

        if (node->val > minVal) {
            // 候选答案，且子节点只会 >= node->val，无需深入
            ans = min(ans, (long long)node->val);
            return;
        }

        // node->val == minVal，继续寻找
        dfs(node->left, minVal, ans);
        dfs(node->right, minVal, ans);
    }
};


/*
 * ============================================================
 * 解法二: BFS + 剪枝
 * ============================================================
 *
 * 思路:
 *   层序遍历，遇到 val > minVal 的节点记录候选，不入队其子节点。
 *   val == minVal 的节点继续入队子节点。
 *
 * 时间: O(n), 空间: O(n) 队列
 */
class Solution2 {
public:
    int findSecondMinimumValue(TreeNode* root) {
        long long ans = LLONG_MAX;
        int minVal = root->val;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            if (node->val > minVal) {
                ans = min(ans, (long long)node->val);
                continue; // 不入队子节点 (剪枝)
            }

            // node->val == minVal, 继续探索
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        return ans == LLONG_MAX ? -1 : (int)ans;
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法          | 时间 | 空间  | 特点                    |
 * |--------------|------|------|------------------------|
 * | DFS (解法一)  | O(n) | O(h) | 递归简洁，空间更优        |
 * | BFS (解法二)  | O(n) | O(n) | 迭代，不会栈溢出          |
 *
 * 面试推荐: DFS 更简洁，本题树最多 25 节点，两种都可以
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 初始化溢出:
 *    ✗ int ans = INT_MAX (节点值可以等于 INT_MAX)
 *    ✓ long long ans = LLONG_MAX 或用 -1 标记未找到
 *
 * 2. 缺少剪枝导致错误:
 *    ✗ 不深入 val>minVal 的节点后，漏掉了其子树
 *    ✓ 不需要深入，因为子节点只会 >= 父节点
 *
 * 3. 忘记叶子节点:
 *    ✗ 只检查有子节点的内部节点
 *    ✓ 叶子节点的 val 也可能是候选答案
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 为什么可以剪枝？
 * → 树的性质: parent.val = min(left.val, right.val)。
 *   所以 child.val >= parent.val。如果 parent.val > minVal，
 *   其所有后代都 > minVal 且 >= parent.val，不可能比
 *   parent.val 更小，所以 parent.val 已是该子树能提供的
 *   最小候选。
 *
 * Q2: 所有节点都相同怎么办？
 * → ans 保持 LLONG_MAX，最终返回 -1。
 *   题目保证这种情况返回 -1。
 *
 * Q3: 如果树没有特殊性质，怎么找第二小？
 * → DFS 遍历收集所有值到 set，取第二小。
 *   或维护两个变量 min1/min2 一次遍历完成。
 * ============================================================
 */
