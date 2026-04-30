/*
 * LeetCode 814: Binary Tree Pruning
 *
 * 解法一：后序遍历递归（推荐）
 * 时间复杂度: O(n)
 * 空间复杂度: O(h)，h 为树高，最坏 O(n)
 *
 * 思路：
 * 后序遍历：先递归处理左右子树，再决定当前节点是否应被剪掉。
 * 如果 val=0 且左右子树都为空，说明整棵子树不含 1，应剪掉。
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (!root) return nullptr;

        // 后序遍历：先处理子树
        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);

        // 当前节点判断：如果值为0且没有子节点，剪掉
        if (root->val == 0 && !root->left && !root->right) {
            // 注意：实际工程中应 delete root 防止内存泄漏
            return nullptr;
        }

        return root;
    }
};


/*
 * 解法二：后序遍历 + 辅助函数（语义更清晰）
 * 时间复杂度: O(n)
 * 空间复杂度: O(h)
 *
 * 思路：
 * 分离 "子树是否包含1" 和 "剪枝" 两个逻辑。
 * containsOne(node) 判断以 node 为根的子树是否包含1。
 * 在判断过程中同时完成剪枝。
 */

class SolutionContainsOne {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (!containsOne(root)) return nullptr;
        return root;
    }

private:
    // 判断以 node 为根的子树是否包含值 1
    // 同时在过程中剪枝
    bool containsOne(TreeNode* node) {
        if (!node) return false;

        // 后序：先检查左右子树
        bool leftContainsOne = containsOne(node->left);
        bool rightContainsOne = containsOne(node->right);

        // 如果左子树不含1，剪掉
        if (!leftContainsOne) {
            node->left = nullptr;
        }
        // 如果右子树不含1，剪掉
        if (!rightContainsOne) {
            node->right = nullptr;
        }

        // 当前子树包含1：自身为1 或 左子树含1 或 右子树含1
        return node->val == 1 || leftContainsOne || rightContainsOne;
    }
};


/*
 * 解法三：迭代后序遍历（避免递归栈溢出）
 * 时间复杂度: O(n)
 * 空间复杂度: O(n)
 *
 * 思路：
 * 使用显式栈模拟后序遍历，配合父节点映射来修改指针。
 * 后序遍历确保处理节点时其子节点已经被处理过。
 */

class SolutionIterative {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (!root) return nullptr;

        // 使用两个栈实现后序遍历
        // stack1 用于遍历，stack2 存储后序序列
        stack<TreeNode*> stack1, stack2;

        // 记录父节点关系
        unordered_map<TreeNode*, TreeNode*> parent;
        parent[root] = nullptr;

        stack1.push(root);
        while (!stack1.empty()) {
            TreeNode* node = stack1.top();
            stack1.pop();
            stack2.push(node);

            if (node->left) {
                parent[node->left] = node;
                stack1.push(node->left);
            }
            if (node->right) {
                parent[node->right] = node;
                stack1.push(node->right);
            }
        }

        // 按后序顺序处理每个节点
        while (!stack2.empty()) {
            TreeNode* node = stack2.top();
            stack2.pop();

            // 判断是否应该剪掉
            if (node->val == 0 && !node->left && !node->right) {
                TreeNode* par = parent[node];
                if (!par) {
                    // 根节点被剪掉
                    return nullptr;
                }
                // 修改父节点的指针
                if (par->left == node) {
                    par->left = nullptr;
                } else {
                    par->right = nullptr;
                }
            }
        }

        return root;
    }
};


/*
 * 解法四：简洁的一行写法（展示语法技巧）
 * 时间复杂度: O(n)
 * 空间复杂度: O(h)
 *
 * 思路：
 * 利用三元运算符的短路特性，将整个逻辑压缩到极致。
 * 不推荐面试使用（可读性差），但展示了对问题本质的理解。
 */

class SolutionCompact {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (!root) return nullptr;
        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);
        // 等价于：如果节点为0且无子节点则剪掉
        return (root->val || root->left || root->right) ? root : nullptr;
    }
};


/*
 * 解法五：推广到 N 叉树的剪枝（面试拓展）
 *
 * 思路框架：
 * 对于 N 叉树的节点，children 是一个 vector。
 * 后序遍历所有子节点，过滤掉返回 nullptr 的子节点。
 * 最后判断当前节点是否应被剪掉。
 */

/*
class SolutionNary {
public:
    // 假设 NaryNode 有 val 和 vector<NaryNode*> children
    NaryNode* pruneTree(NaryNode* root) {
        if (!root) return nullptr;

        // 递归处理每个子节点
        vector<NaryNode*> prunedChildren;
        for (auto* child : root->children) {
            NaryNode* pruned = pruneTree(child);
            if (pruned) {
                prunedChildren.push_back(pruned);
            }
        }
        root->children = prunedChildren;

        // 如果当前节点值为0且无子节点，剪掉
        if (root->val == 0 && root->children.empty()) {
            return nullptr;
        }

        return root;
    }
};
*/
