// ============================================================
// 623. Add One Row to Tree (在二叉树中增加一行)
// 难度: Medium | 标签: Tree, DFS, BFS, Binary Tree
// ============================================================
// 题意: 在二叉树的指定深度 depth 处插入一行值为 val 的新节点
//       原左子树挂到新左节点的左边, 原右子树挂到新右节点的右边
// ============================================================

// ============================================================
// 解法一: DFS 递归 (推荐)
// ============================================================
// 思路: 递归深入到 depth-1 层, 在每个节点下方插入新节点行
// 时间: O(n) | 空间: O(h), h 为树高
// ============================================================

class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        // depth == 1: 创建新根, 原树挂在左子树
        if (depth == 1) {
            TreeNode* newRoot = new TreeNode(val);
            newRoot->left = root;
            return newRoot;
        }
        dfs(root, val, depth, 1);
        return root;
    }

private:
    void dfs(TreeNode* node, int val, int targetDepth, int curDepth) {
        if (!node) return;

        if (curDepth == targetDepth - 1) {
            // 创建新节点并插入
            TreeNode* newLeft = new TreeNode(val);
            TreeNode* newRight = new TreeNode(val);
            // 保留原有子树关系
            newLeft->left = node->left;
            newRight->right = node->right;
            // 重新连接
            node->left = newLeft;
            node->right = newRight;
            return; // 插入完成, 无需继续深入
        }

        dfs(node->left, val, targetDepth, curDepth + 1);
        dfs(node->right, val, targetDepth, curDepth + 1);
    }
};

// ============================================================
// 解法二: BFS 层序遍历
// ============================================================
// 思路: 逐层遍历到第 depth-1 层, 对该层所有节点执行插入
// 时间: O(n) | 空间: O(w), w 为最宽层节点数
// ============================================================

/*
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        if (depth == 1) {
            TreeNode* newRoot = new TreeNode(val);
            newRoot->left = root;
            return newRoot;
        }

        queue<TreeNode*> q;
        q.push(root);
        int curDepth = 1;

        // 逐层推进到 depth-1 层
        while (curDepth < depth - 1) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            curDepth++;
        }

        // 对 depth-1 层的每个节点插入新行
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            TreeNode* newLeft  = new TreeNode(val);
            TreeNode* newRight = new TreeNode(val);
            newLeft->left   = node->left;
            newRight->right = node->right;
            node->left  = newLeft;
            node->right = newRight;
        }

        return root;
    }
};
*/

// ============================================================
// 关键要点:
// 1. depth==1 必须特殊处理: 新节点成为根, 原树挂在 left
// 2. 插入方向: 原左子树 -> 新左节点的 left; 原右子树 -> 新右节点的 right
// 3. DFS 在 curDepth == depth-1 时操作, 操作后不再继续递归
// 4. BFS 遍历到 depth-1 层后, 队列中恰好是所有需要操作的节点
// ============================================================
