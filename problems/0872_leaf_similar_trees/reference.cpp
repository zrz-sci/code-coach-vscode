/*
 * LeetCode 872: Leaf-Similar Trees
 * 解法一: 递归 DFS 收集叶子序列
 * 解法二: 迭代 DFS 逐一比较（空间优化）
 *
 * 思路: 分别收集两棵树的叶子节点值序列（从左到右），比较是否相同。
 *       DFS 先左后右的遍历顺序天然保证叶子从左到右被访问。
 *
 * 时间复杂度: O(n1 + n2)
 * 空间复杂度: 解法一 O(n1 + n2), 解法二 O(h1 + h2)
 */

// ===================== 解法一：递归 DFS 收集叶子序列 =====================

class Solution {
public:
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> leaves1, leaves2;
        collectLeaves(root1, leaves1);
        collectLeaves(root2, leaves2);
        return leaves1 == leaves2;
    }

private:
    // 递归收集所有叶子节点的值（从左到右）
    void collectLeaves(TreeNode* node, vector<int>& leaves) {
        if (node == nullptr) return;

        // 判断是否为叶子节点
        if (node->left == nullptr && node->right == nullptr) {
            leaves.push_back(node->val);
            return;
        }

        // 先递归左子树，再递归右子树
        // 这保证叶子按从左到右的顺序被收集
        collectLeaves(node->left, leaves);
        collectLeaves(node->right, leaves);
    }
};

// ===================== 解法二：迭代 DFS + 逐一比较 =====================
// 使用显式栈模拟 DFS，同步比较两棵树的叶子节点
// 优势：1. 空间仅 O(h1 + h2)  2. 可提前终止

class SolutionIterative {
public:
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        stack<TreeNode*> stk1, stk2;
        stk1.push(root1);
        stk2.push(root2);

        while (!stk1.empty() && !stk2.empty()) {
            // 从各自的栈中获取下一个叶子节点
            int leaf1 = nextLeaf(stk1);
            int leaf2 = nextLeaf(stk2);

            // 如果叶子值不同，直接返回 false
            if (leaf1 != leaf2) return false;
        }

        // 两棵树必须同时耗尽叶子节点
        // 检查是否两边都没有剩余的叶子
        return !hasMoreLeaves(stk1) && !hasMoreLeaves(stk2);
    }

private:
    // 从栈中弹出节点，直到找到下一个叶子节点并返回其值
    int nextLeaf(stack<TreeNode*>& stk) {
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();

            // 如果是叶子节点，返回其值
            if (node->left == nullptr && node->right == nullptr) {
                return node->val;
            }

            // 先压右子节点，再压左子节点（栈是 LIFO，左子节点会先被弹出）
            if (node->right != nullptr) stk.push(node->right);
            if (node->left != nullptr) stk.push(node->left);
        }
        return -1; // 不会到这里（题目保证树非空）
    }

    // 检查栈中是否还有叶子节点
    bool hasMoreLeaves(stack<TreeNode*>& stk) {
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            if (node->left == nullptr && node->right == nullptr) return true;
            if (node->right) stk.push(node->right);
            if (node->left) stk.push(node->left);
        }
        return false;
    }
};

// ===================== 解法三：字符串序列化比较 =====================
// 将叶子序列序列化为字符串进行比较

class SolutionStringify {
public:
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        string seq1 = getLeafSequence(root1);
        string seq2 = getLeafSequence(root2);
        return seq1 == seq2;
    }

private:
    string getLeafSequence(TreeNode* node) {
        if (node == nullptr) return "";

        // 叶子节点：返回 "val,"
        if (!node->left && !node->right) {
            return to_string(node->val) + ",";
        }

        // 拼接左右子树的叶子序列
        return getLeafSequence(node->left) + getLeafSequence(node->right);
    }
};

// ===================== 辅助说明 =====================
/*
 * 叶子节点定义:
 *   node->left == nullptr && node->right == nullptr
 *
 * DFS 顺序为什么保证从左到右:
 *   在前序/中序/后序遍历中，只要先处理左子树再处理右子树，
 *   所有叶子节点的访问顺序一定是从左到右的。
 *   这是因为：一个节点的左子树中的所有节点在树的水平位置上
 *   都位于右子树中所有节点的左侧。
 *
 * BFS 为什么不行:
 *   BFS 按层遍历，不同层的叶子节点会按层混合在一起。
 *   例如树：    1
 *            /   \
 *           2     3    <- 叶子 2 在第 2 层
 *               / \
 *              4   5   <- 叶子 4, 5 在第 3 层
 *   DFS 叶子序列: [2, 4, 5]（正确，从左到右）
 *   BFS 叶子出现顺序: 第 2 层发现叶子 2，第 3 层发现叶子 4, 5
 *   虽然这个例子 BFS 顺序恰好对，但一般情况下 BFS 不保证
 *   不同层叶子的左右相对顺序。
 *
 * 注意: 本题节点数 <= 200，值 <= 200，任何合理方法都不会超时。
 */
