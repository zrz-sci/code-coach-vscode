/*
 * 【题目本质】
 * 统计二叉树中所有节点值相同的子树个数。
 * 后序遍历自底向上: 一个子树是同值子树 iff
 * 左右子树都是同值子树 且 左右子节点值等于当前值。
 * 叶子节点天然是同值子树。
 *
 * 【解法总览】
 * 解法1 (推荐): 后序遍历 + bool返回值 — O(n) / O(h)
 * 解法2: 后序遍历 + 传父值参数 — O(n) / O(h)
 */

// ===================== 解法1: 后序遍历 + bool返回值 (推荐) =====================
// 思路: 递归返回"当前子树是否同值", 用成员变量累加计数
class Solution1 {
public:
    int count = 0;

    int countUnivalSubtrees(TreeNode* root) {
        isUnival(root);
        return count;
    }

private:
    bool isUnival(TreeNode* node) {
        if (!node) return true;  // 空节点返回true, 不影响父节点判断

        // 后序遍历: 先递归左右 (不能短路求值!)
        bool left = isUnival(node->left);
        bool right = isUnival(node->right);

        if (left && right) {
            // 检查子节点值是否与当前节点相同
            if (node->left && node->left->val != node->val) return false;
            if (node->right && node->right->val != node->val) return false;
            count++;  // 当前子树是同值子树
            return true;
        }
        return false;
    }
};

// ===================== 解法2: 后序遍历 + 传父值参数 =====================
// 思路: 递归时传入父节点值, 简化判断逻辑
class Solution2 {
public:
    int countUnivalSubtrees(TreeNode* root) {
        int count = 0;
        helper(root, 0, count);
        return count;
    }

private:
    // 返回: 当前子树是否为同值子树 且 值等于 parentVal
    bool helper(TreeNode* node, int parentVal, int& count) {
        if (!node) return true;

        // 后序遍历: 两侧都必须递归 (不能短路)
        bool left = helper(node->left, node->val, count);
        bool right = helper(node->right, node->val, count);

        if (!left || !right) return false;

        // 当前子树是同值子树
        count++;
        // 告知父节点: 我的值是否等于你期望的值
        return node->val == parentVal;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | bool返回值 | O(n) | O(h) | 逻辑直观, 判断条件清晰 |
 * | 传父值参数 | O(n) | O(h) | 代码更简洁, 无需空指针检查 |
 *
 * 【易错点】
 * 1. ✗ bool left = isUnival(left) && isUnival(right) (短路求值)
 *    ✓ 分别计算再组合
 *    短路会跳过右子树的递归, 导致计数不完整
 * 2. ✗ 空节点返回 false → ✓ 返回 true
 *    空节点不应阻止父节点成为同值子树
 * 3. ✗ 忘记空指针检查: node->left->val
 *    ✓ 先判断 node->left != nullptr
 * 4. ✗ 先序遍历 → ✓ 后序遍历
 *    必须先知道子树结果才能判断当前节点
 *
 * 【面试追问】
 * Q1: 如何避免全局变量? → 返回 pair<bool,int> 或用引用参数
 * Q2: 最大同值子树大小? → 返回节点数, 非同值返回-1
 * Q3: 时间复杂度下限? → O(n), 每个节点必须访问
 */
