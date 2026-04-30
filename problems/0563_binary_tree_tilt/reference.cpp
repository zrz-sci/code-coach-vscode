// 【题目本质】
// 后序遍历二叉树，每个节点的坡度 = |左子树和 - 右子树和|，求所有节点坡度之和。
// 关键在于递归函数返回子树和，同时用副作用累加坡度。

// 【解法总览】
// Solution1: 后序DFS + 全局累加（最优）  O(n) / O(h)
// Solution2: 后序DFS + pair返回（无全局变量）O(n) / O(h)

// ===================== Solution 1 =====================
// 后序DFS + 全局累加
// 递归返回子树和，坡度通过成员变量累加
class Solution1 {
    int totalTilt = 0;

    int subtreeSum(TreeNode* node) {
        if (!node) return 0;
        int leftSum = subtreeSum(node->left);
        int rightSum = subtreeSum(node->right);
        totalTilt += abs(leftSum - rightSum);
        return leftSum + rightSum + node->val;
    }
public:
    int findTilt(TreeNode* root) {
        totalTilt = 0;
        subtreeSum(root);
        return totalTilt;
    }
};

// ===================== Solution 2 =====================
// 后序DFS + pair返回，避免全局变量
// pair.first = 子树和, pair.second = 坡度和
class Solution2 {
    // 返回 {子树和, 坡度累计}
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};
        auto [leftSum, leftTilt] = dfs(node->left);
        auto [rightSum, rightTilt] = dfs(node->right);
        int tilt = abs(leftSum - rightSum);
        int sum = leftSum + rightSum + node->val;
        return {sum, leftTilt + rightTilt + tilt};
    }
public:
    int findTilt(TreeNode* root) {
        return dfs(root).second;
    }
};

// 【解法对比】
// Solution1: 代码最简洁，面试首选
// Solution2: 函数式风格，无副作用，更优雅但略冗长

// 【易错点】
// 1. 递归函数应返回子树和而非坡度
// 2. 子树和要包含当前节点值: left + right + node->val
// 3. 注意重置全局变量（Solution1 多次调用场景）

// 【面试追问】
// Q1: 如何用迭代实现？用后序遍历栈 + hashmap 存子树和
// Q2: 值域很大怎么办？用 long long
// Q3: 类似的"后序遍历+全局累加"模式？LC124/543/687
