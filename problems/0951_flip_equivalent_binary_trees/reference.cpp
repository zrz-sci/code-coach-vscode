// LeetCode 951: Flip Equivalent Binary Trees
// 解法一：递归DFS（推荐）
// 时间复杂度: O(n) - 每个节点常数次访问
// 空间复杂度: O(h) - 递归栈深度

class Solution {
public:
    bool flipEquiv(TreeNode* root1, TreeNode* root2) {
        // Base case 1: 两个都为 null，等价
        if (!root1 && !root2) return true;

        // Base case 2: 只有一个为 null，不等价
        if (!root1 || !root2) return false;

        // Base case 3: 值不同，不等价
        if (root1->val != root2->val) return false;

        // 递归检查两种情况：
        // 情况1（不翻转）：左对左，右对右
        // 情况2（翻转）：左对右，右对左
        bool noFlip = flipEquiv(root1->left, root2->left) &&
                      flipEquiv(root1->right, root2->right);

        bool withFlip = flipEquiv(root1->left, root2->right) &&
                        flipEquiv(root1->right, root2->left);

        return noFlip || withFlip;
    }
};


// 解法一的精简写法（一行递归）
// 逻辑完全相同，更紧凑

class Solution1b {
public:
    bool flipEquiv(TreeNode* r1, TreeNode* r2) {
        if (!r1 && !r2) return true;
        if (!r1 || !r2 || r1->val != r2->val) return false;
        return (flipEquiv(r1->left, r2->left) && flipEquiv(r1->right, r2->right)) ||
               (flipEquiv(r1->left, r2->right) && flipEquiv(r1->right, r2->left));
    }
};


// 解法二：规范化后比较
// 思路：对每棵树进行规范化（左子节点值 < 右子节点值），
//       翻转等价的树规范化后完全相同
// 时间复杂度: O(n)
// 空间复杂度: O(h)

class Solution2 {
public:
    // 规范化：确保每个节点的左子值 <= 右子值
    void canonicalize(TreeNode* node) {
        if (!node) return;

        // 递归规范化子树
        canonicalize(node->left);
        canonicalize(node->right);

        // 如果需要交换（左子值 > 右子值，或左子非空但右子为空需要特殊处理）
        // 规则：null 视为最小值（放左边）
        if (node->left && node->right) {
            if (node->left->val > node->right->val) {
                swap(node->left, node->right);
            }
        } else if (node->left && !node->right) {
            // 左子非空，右子空 —— 不需要交换（null 视为 -infinity 放左边）
            // 但为了统一规范，我们定义 null 总在右边
            // 实际上这里的策略要一致，我们定义：非空子节点放左边
            // 所以不需要交换
        } else if (!node->left && node->right) {
            // 右子非空，左子空 —— 把非空移到左边
            swap(node->left, node->right);
        }
    }

    // 标准的相同树判断
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (!p && !q) return true;
        if (!p || !q) return false;
        if (p->val != q->val) return false;
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }

    bool flipEquiv(TreeNode* root1, TreeNode* root2) {
        // 步骤1：分别规范化两棵树
        canonicalize(root1);
        canonicalize(root2);

        // 步骤2：规范化后的树如果翻转等价，应该完全相同
        return isSameTree(root1, root2);
    }
};


// 解法三：迭代BFS方式
// 用队列存储待比较的节点对
// 时间复杂度: O(n)
// 空间复杂度: O(n)

class Solution3 {
public:
    bool flipEquiv(TreeNode* root1, TreeNode* root2) {
        queue<pair<TreeNode*, TreeNode*>> q;
        q.push({root1, root2});

        while (!q.empty()) {
            auto [n1, n2] = q.front();
            q.pop();

            // 两个都空，跳过
            if (!n1 && !n2) continue;

            // 一个空一个非空，或值不同
            if (!n1 || !n2 || n1->val != n2->val) return false;

            // 检查子树对应关系
            // 不翻转：left-left, right-right
            if ((n1->left ? n1->left->val : -1) == (n2->left ? n2->left->val : -1)) {
                q.push({n1->left, n2->left});
                q.push({n1->right, n2->right});
            } else {
                // 翻转：left-right, right-left
                q.push({n1->left, n2->right});
                q.push({n1->right, n2->left});
            }
        }

        return true;
    }
};
