// LeetCode 965: Univalued Binary Tree
// 解法一：DFS 递归 —— 推荐解法
// 时间 O(n)，空间 O(h) 递归栈深度

class Solution {
public:
    bool isUnivalTree(TreeNode* root) {
        if (!root) return true;
        return check(root, root->val);
    }

private:
    bool check(TreeNode* node, int val) {
        // 空节点不影响单值性
        if (!node) return true;
        // 当前节点值与目标值不同，直接返回 false
        if (node->val != val) return false;
        // 短路求值：左子树不满足就不再检查右子树
        return check(node->left, val) && check(node->right, val);
    }
};


// ================================================================
// 解法二：BFS 迭代（层序遍历）
// 时间 O(n)，空间 O(w) 队列宽度
// ================================================================

class Solution2 {
public:
    bool isUnivalTree(TreeNode* root) {
        if (!root) return true;

        int targetVal = root->val;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            // 检查当前节点值
            if (node->val != targetVal) {
                return false;
            }

            // 将非空子节点入队
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        return true;
    }
};


// ================================================================
// 解法三：DFS 迭代（显式栈）
// 时间 O(n)，空间 O(h)
// ================================================================

class Solution3 {
public:
    bool isUnivalTree(TreeNode* root) {
        if (!root) return true;

        int targetVal = root->val;
        stack<TreeNode*> stk;
        stk.push(root);

        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();

            if (node->val != targetVal) {
                return false;
            }

            if (node->right) stk.push(node->right);
            if (node->left) stk.push(node->left);
        }

        return true;
    }
};


// ================================================================
// 解法四：更简洁的递归写法（单函数）
// 时间 O(n)，空间 O(h)
// ================================================================

class Solution4 {
public:
    bool isUnivalTree(TreeNode* root) {
        if (!root) return true;

        // 检查左子节点（如果存在）是否与当前节点值相同
        if (root->left && root->left->val != root->val) {
            return false;
        }
        // 检查右子节点（如果存在）是否与当前节点值相同
        if (root->right && root->right->val != root->val) {
            return false;
        }

        // 递归检查左右子树
        return isUnivalTree(root->left) && isUnivalTree(root->right);
    }
};


// ================================================================
// 解法五：收集所有值到 set（不推荐，仅用于理解）
// 时间 O(n)，空间 O(n)
// ================================================================

class Solution5 {
public:
    bool isUnivalTree(TreeNode* root) {
        unordered_set<int> vals;
        collectValues(root, vals);
        return vals.size() == 1;
    }

private:
    void collectValues(TreeNode* node, unordered_set<int>& vals) {
        if (!node) return;
        vals.insert(node->val);
        // 优化：如果已经有多个不同值，提前终止
        if (vals.size() > 1) return;
        collectValues(node->left, vals);
        collectValues(node->right, vals);
    }
};


// ================================================================
// 解法六：分治法 —— 自底向上判断
// 时间 O(n)，空间 O(h)
// ================================================================

class Solution6 {
public:
    bool isUnivalTree(TreeNode* root) {
        if (!root) return true;
        return isUnival(root).first;
    }

private:
    // 返回 {是否单值, 单值的值}
    pair<bool, int> isUnival(TreeNode* node) {
        if (!node) return {true, -1}; // 空节点，值无意义

        auto [leftUnival, leftVal] = isUnival(node->left);
        if (!leftUnival) return {false, -1};

        auto [rightUnival, rightVal] = isUnival(node->right);
        if (!rightUnival) return {false, -1};

        // 检查左子树值是否与当前节点一致（如果左子树非空）
        if (leftVal != -1 && leftVal != node->val) return {false, -1};
        // 检查右子树值是否与当前节点一致（如果右子树非空）
        if (rightVal != -1 && rightVal != node->val) return {false, -1};

        return {true, node->val};
    }
};
