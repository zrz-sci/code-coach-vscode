// ========================================
// 655. Print Binary Tree
// ========================================
// 解法一：两次 DFS（求高度 + 填充矩阵）
// 时间 O(n + m*n)  空间 O(m*n)
// ========================================
class Solution {
public:
    vector<vector<string>> printTree(TreeNode* root) {
        // 第一步：求树的高度
        int height = getHeight(root);

        // 第二步：计算矩阵尺寸
        int rows = height + 1;
        int cols = (1 << (height + 1)) - 1;  // 2^(height+1) - 1

        // 第三步：初始化矩阵（全部空字符串）
        vector<vector<string>> res(rows, vector<string>(cols, ""));

        // 第四步：递归填充节点位置
        fill(root, 0, (cols - 1) / 2, height, res);

        return res;
    }

private:
    // 求树的高度（单节点返回 0，空树返回 -1）
    int getHeight(TreeNode* node) {
        if (!node) return -1;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // 将 node 的值填入 res[row][col]
    // 左孩子列 = col - 2^(height - row - 1)
    // 右孩子列 = col + 2^(height - row - 1)
    void fill(TreeNode* node, int row, int col,
              int height, vector<vector<string>>& res) {
        if (!node) return;

        res[row][col] = to_string(node->val);

        // 计算子节点的列偏移量
        int offset = 1 << (height - row - 1);

        fill(node->left, row + 1, col - offset, height, res);
        fill(node->right, row + 1, col + offset, height, res);
    }
};

// ========================================
// 解法二：BFS 求高度 + DFS 填充
// 时间 O(n + m*n)  空间 O(m*n)
// ========================================
class Solution2 {
public:
    vector<vector<string>> printTree(TreeNode* root) {
        // BFS 求树高度
        int height = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            bool hasNext = false;
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                if (node->left) {
                    q.push(node->left);
                    hasNext = true;
                }
                if (node->right) {
                    q.push(node->right);
                    hasNext = true;
                }
            }
            if (hasNext) height++;
        }

        int rows = height + 1;
        int cols = (1 << (height + 1)) - 1;
        vector<vector<string>> res(rows, vector<string>(cols, ""));

        // DFS 填充
        fill(root, 0, (cols - 1) / 2, height, res);
        return res;
    }

private:
    void fill(TreeNode* node, int row, int col,
              int height, vector<vector<string>>& res) {
        if (!node) return;
        res[row][col] = to_string(node->val);
        int offset = 1 << (height - row - 1);
        fill(node->left, row + 1, col - offset, height, res);
        fill(node->right, row + 1, col + offset, height, res);
    }
};

// ========================================
// 解法三：BFS 层序填充（全 BFS 版本）
// 时间 O(n + m*n)  空间 O(m*n)
// ========================================
// 用队列存 (node, row, col) 三元组进行层序填充
class Solution3 {
public:
    vector<vector<string>> printTree(TreeNode* root) {
        int height = getHeight(root);
        int rows = height + 1;
        int cols = (1 << (height + 1)) - 1;
        vector<vector<string>> res(rows, vector<string>(cols, ""));

        // BFS 填充，队列中存储 (节点, 行, 列)
        queue<tuple<TreeNode*, int, int>> q;
        q.push({root, 0, (cols - 1) / 2});

        while (!q.empty()) {
            auto [node, row, col] = q.front();
            q.pop();

            if (!node) continue;

            res[row][col] = to_string(node->val);

            int offset = 1 << (height - row - 1);
            q.push({node->left, row + 1, col - offset});
            q.push({node->right, row + 1, col + offset});
        }

        return res;
    }

private:
    int getHeight(TreeNode* node) {
        if (!node) return -1;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }
};

// ========================================
// 解法四：递归简化写法
// 时间 O(n + m*n)  空间 O(m*n)
// ========================================
// 使用成员变量减少参数传递，代码更简洁
class Solution4 {
    int h;
    vector<vector<string>> res;

public:
    vector<vector<string>> printTree(TreeNode* root) {
        h = getHeight(root);
        int cols = (1 << (h + 1)) - 1;
        res.assign(h + 1, vector<string>(cols, ""));
        dfs(root, 0, (cols - 1) / 2);
        return res;
    }

private:
    int getHeight(TreeNode* node) {
        if (!node) return -1;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    void dfs(TreeNode* node, int row, int col) {
        if (!node) return;
        res[row][col] = to_string(node->val);
        // 位运算计算偏移量：2^(h - row - 1)
        int off = 1 << (h - row - 1);
        dfs(node->left, row + 1, col - off);
        dfs(node->right, row + 1, col + off);
    }
};
