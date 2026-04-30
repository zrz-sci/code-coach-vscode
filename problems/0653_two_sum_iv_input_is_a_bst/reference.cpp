// ========================================
// 653. Two Sum IV - Input is a BST
// ========================================
// 解法一：DFS + HashSet
// 时间 O(n)  空间 O(n)
// ========================================
class Solution {
public:
    bool findTarget(TreeNode* root, int k) {
        unordered_set<int> seen;
        return dfs(root, k, seen);
    }

private:
    bool dfs(TreeNode* node, int k, unordered_set<int>& seen) {
        if (!node) return false;

        // 检查互补值是否已在集合中
        if (seen.count(k - node->val)) return true;

        // 将当前值加入集合
        seen.insert(node->val);

        // 递归检查左右子树
        return dfs(node->left, k, seen) ||
               dfs(node->right, k, seen);
    }
};

// ========================================
// 解法二：中序遍历 + 双指针
// 时间 O(n)  空间 O(n)
// ========================================
// 利用 BST 中序遍历有序的性质，转化为有序数组上的 Two Sum
class Solution2 {
public:
    bool findTarget(TreeNode* root, int k) {
        // 步骤一：中序遍历获得有序数组
        vector<int> sorted;
        inorder(root, sorted);

        // 步骤二：双指针查找
        int left = 0, right = (int)sorted.size() - 1;
        while (left < right) {
            int sum = sorted[left] + sorted[right];
            if (sum == k) return true;
            if (sum < k) left++;
            else right--;
        }
        return false;
    }

private:
    void inorder(TreeNode* node, vector<int>& result) {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->val);
        inorder(node->right, result);
    }
};

// ========================================
// 解法三：BST 迭代器双指针（空间最优）
// 时间 O(n)  空间 O(h) 其中 h 为树高
// ========================================
// 用两个栈分别模拟正向（从小到大）和反向（从大到小）
// 的中序遍历迭代器，实现 O(h) 空间的双指针
class Solution3 {
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;

        // 正向迭代器：从最小值开始
        stack<TreeNode*> leftStack;
        // 反向迭代器：从最大值开始
        stack<TreeNode*> rightStack;

        // 初始化正向栈：一路向左压栈
        pushAllLeft(root, leftStack);
        // 初始化反向栈：一路向右压栈
        pushAllRight(root, rightStack);

        // 获取初始的最小值和最大值节点
        TreeNode* leftNode = leftStack.top();
        TreeNode* rightNode = rightStack.top();

        while (leftNode != rightNode) {
            int sum = leftNode->val + rightNode->val;

            if (sum == k) return true;

            if (sum < k) {
                // 需要更大的左值 -> 正向迭代器前进
                leftNode = nextLeft(leftStack);
            } else {
                // 需要更小的右值 -> 反向迭代器前进
                rightNode = nextRight(rightStack);
            }
        }
        return false;
    }

private:
    // 将节点及其所有左后代压入栈
    void pushAllLeft(TreeNode* node, stack<TreeNode*>& stk) {
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }

    // 将节点及其所有右后代压入栈
    void pushAllRight(TreeNode* node, stack<TreeNode*>& stk) {
        while (node) {
            stk.push(node);
            node = node->right;
        }
    }

    // 正向迭代器：获取下一个更大的节点
    TreeNode* nextLeft(stack<TreeNode*>& stk) {
        TreeNode* node = stk.top();
        stk.pop();
        // 当前节点弹出后，将其右子树一路向左压栈
        pushAllLeft(node->right, stk);
        return stk.empty() ? nullptr : stk.top();
    }

    // 反向迭代器：获取下一个更小的节点
    TreeNode* nextRight(stack<TreeNode*>& stk) {
        TreeNode* node = stk.top();
        stk.pop();
        // 当前节点弹出后，将其左子树一路向右压栈
        pushAllRight(node->left, stk);
        return stk.empty() ? nullptr : stk.top();
    }
};

// ========================================
// 解法四：BFS + HashSet（迭代版本）
// 时间 O(n)  空间 O(n)
// ========================================
// 与解法一思路相同，用 BFS（队列）代替 DFS（递归）
class Solution4 {
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;

        unordered_set<int> seen;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            if (seen.count(k - node->val)) return true;
            seen.insert(node->val);

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return false;
    }
};
