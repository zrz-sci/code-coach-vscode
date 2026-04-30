/*
 * 270. Closest Binary Search Tree Value
 *
 * 【题目本质】
 * 在 BST 中沿搜索路径找距 target 最近的节点值。
 * 利用 BST 有序性，每次根据 target 与当前节点的大小关系决定方向，
 * 路径上不断更新最优解，类似二分查找。
 *
 * 【解法总览】
 * Solution1: BST 迭代搜索 — O(H) 时间, O(1) 空间 [推荐]
 * Solution2: BST 递归搜索 — O(H) 时间, O(H) 空间
 */

// ===================== Solution 1: BST 迭代搜索 =====================
// 思路: 沿搜索路径迭代，维护全局最近值
// 时间 O(H), 空间 O(1)
class Solution1 {
public:
    int closestValue(TreeNode* root, double target) {
        int closest = root->val;
        while (root) {
            // 更新最近值
            // 距离更小，或距离相等但值更小
            if (abs(root->val - target) < abs(closest - target) ||
                (abs(root->val - target) == abs(closest - target) && root->val < closest)) {
                closest = root->val;
            }
            // BST 搜索方向
            root = (target < root->val) ? root->left : root->right;
        }
        return closest;
    }
};

// ===================== Solution 2: BST 递归搜索 =====================
// 思路: 递归版本，沿 BST 搜索路径，每层更新最近值
// 时间 O(H), 空间 O(H) 递归栈
class Solution2 {
public:
    int closestValue(TreeNode* root, double target) {
        int closest = root->val;
        dfs(root, target, closest);
        return closest;
    }

private:
    void dfs(TreeNode* node, double target, int& closest) {
        if (!node) return;
        // 距离更小，或距离相等取较小值
        double curDist = abs((double)node->val - target);
        double bestDist = abs((double)closest - target);
        if (curDist < bestDist || (curDist == bestDist && node->val < closest)) {
            closest = node->val;
        }
        // 只走有可能更优的分支
        if (target < node->val) {
            dfs(node->left, target, closest);
        } else {
            dfs(node->right, target, closest);
        }
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 迭代 | O(H) | O(1) | 最优，无栈开销 |
 * | Solution2 递归 | O(H) | O(H) | 代码简洁，有递归栈 |
 *
 * H = 树高，平衡BST为 log(n)，退化链表为 n
 *
 * 【易错点】
 * 1. 忘记处理 "距离相等返回较小值" 的要求
 *    ✗ 只比较 abs(val-target) < abs(closest-target)
 *    ✓ 加上 curDist == bestDist && val < closest
 *
 * 2. int 和 double 混合运算精度问题
 *    ✗ abs(root->val - target) 可能丢精度
 *    ✓ 显式转换 abs((double)root->val - target)
 *
 * 3. 搜索方向判断
 *    ✗ target <= node->val 就只向左，漏掉右子树
 *    ✓ target < node->val 向左，target >= node->val 向右
 *
 * 【面试追问】
 * Q1: 找 K 个最近值?
 *     --> 中序遍历 + 滑动窗口/双指针，见 LC 272
 *
 * Q2: BST 不平衡怎么办?
 *     --> 最坏 O(n)，可用 AVL/红黑树保证 O(log n)
 *
 * Q3: 如果 target 恰好是某节点值?
 *     --> 直接返回该值，距离为 0 已是最优
 */
