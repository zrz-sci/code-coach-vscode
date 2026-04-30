// 783. Minimum Distance Between BST Nodes
// 与 LeetCode 530 完全相同
// 利用 BST 中序遍历有序性，最小差值一定在中序相邻元素之间

// ============================================================
// 解法一: 递归中序遍历 (推荐)
// 时间复杂度: O(n), 空间复杂度: O(h)
// ============================================================
class Solution {
public:
    int minResult = INT_MAX;
    TreeNode* prev = nullptr;

    int minDiffInBST(TreeNode* root) {
        inorder(root);
        return minResult;
    }

    void inorder(TreeNode* node) {
        if (!node) return;

        // 左子树
        inorder(node->left);

        // 处理当前节点: 与前驱节点比较
        if (prev != nullptr) {
            minResult = min(minResult, node->val - prev->val);
        }
        prev = node;

        // 右子树
        inorder(node->right);
    }
};

// ============================================================
// 解法二: 迭代中序遍历 (显式栈)
// 时间复杂度: O(n), 空间复杂度: O(h)
// ============================================================
class Solution_Iterative {
public:
    int minDiffInBST(TreeNode* root) {
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int result = INT_MAX;

        while (curr || !stk.empty()) {
            // 先把所有左子节点压栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }

            // 弹出栈顶(中序当前节点)
            curr = stk.top();
            stk.pop();

            // 计算与前驱的差值
            if (prev) {
                result = min(result, curr->val - prev->val);
            }
            prev = curr;

            // 转向右子树
            curr = curr->right;
        }

        return result;
    }
};

// ============================================================
// 解法三: Morris 中序遍历 (O(1) 空间)
// 时间复杂度: O(n), 空间复杂度: O(1)
// 面试加分项: 不使用递归和栈
// ============================================================
class Solution_Morris {
public:
    int minDiffInBST(TreeNode* root) {
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int result = INT_MAX;

        while (curr) {
            if (!curr->left) {
                // 没有左子树: 访问当前节点, 然后移到右子树
                if (prev) {
                    result = min(result, curr->val - prev->val);
                }
                prev = curr;
                curr = curr->right;
            } else {
                // 有左子树: 找到左子树的最右节点(中序前驱)
                TreeNode* predecessor = curr->left;
                while (predecessor->right && predecessor->right != curr) {
                    predecessor = predecessor->right;
                }

                if (!predecessor->right) {
                    // 建立线索: 前驱的右指针指向当前节点
                    predecessor->right = curr;
                    curr = curr->left;
                } else {
                    // 线索已存在: 说明左子树已遍历完成
                    // 恢复树结构, 访问当前节点
                    predecessor->right = nullptr;
                    if (prev) {
                        result = min(result, curr->val - prev->val);
                    }
                    prev = curr;
                    curr = curr->right;
                }
            }
        }

        return result;
    }
};

// ============================================================
// 解法四: 中序遍历 + 数组 (最简单)
// 时间复杂度: O(n), 空间复杂度: O(n)
// ============================================================
class Solution_Array {
public:
    int minDiffInBST(TreeNode* root) {
        vector<int> vals;
        inorder(root, vals);

        int result = INT_MAX;
        for (int i = 1; i < (int)vals.size(); ++i) {
            result = min(result, vals[i] - vals[i - 1]);
        }
        return result;
    }

    void inorder(TreeNode* node, vector<int>& vals) {
        if (!node) return;
        inorder(node->left, vals);
        vals.push_back(node->val);
        inorder(node->right, vals);
    }
};
