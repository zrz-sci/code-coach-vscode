/*
 * 515. Find Largest Value in Each Tree Row
 * 难度: Medium | 标签: Tree, BFS, DFS, Binary Tree
 *
 * 【题目本质】
 * 逐层遍历二叉树，记录每层最大值。BFS 天然按层，DFS 用 depth 标记层。
 *
 * 【解法总览】
 * Solution1: BFS 层序遍历 — O(n) 时间, O(w) 空间 ⭐⭐⭐
 * Solution2: DFS 递归     — O(n) 时间, O(h) 空间 ⭐⭐
 */

// ===================== Solution1: BFS 层序遍历 =====================
class Solution1 {
public:
    vector<int> largestValues(TreeNode* root) {
        vector<int> res;
        if (!root) return res;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            int maxVal = INT_MIN;
            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                maxVal = max(maxVal, node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            res.push_back(maxVal);
        }
        return res;
    }
};

// ===================== Solution2: DFS 递归 =====================
class Solution2 {
public:
    vector<int> res;

    void dfs(TreeNode* node, int depth) {
        if (!node) return;
        if (depth == (int)res.size()) {
            res.push_back(node->val);
        } else {
            res[depth] = max(res[depth], node->val);
        }
        dfs(node->left, depth + 1);
        dfs(node->right, depth + 1);
    }

    vector<int> largestValues(TreeNode* root) {
        dfs(root, 0);
        return res;
    }
};

/*
 * 【解法对比】
 * BFS: 直观按层遍历，空间取决于最大层宽 O(w)
 * DFS: 空间取决于树高 O(h)，对于宽树更优
 *
 * 【易错点】
 * 1. maxVal 初始化必须用 INT_MIN，节点值可以为负
 * 2. 空树 root==nullptr 直接返回空数组
 * 3. BFS 内层循环用 sz 锁定层大小，不能用 q.size()
 *
 * 【面试追问】
 * Q1: BFS vs DFS 空间复杂度谁更优？→ 取决于树形态，宽树用 DFS，深树用 BFS
 * Q2: 找每层最小值？→ max→min, INT_MIN→INT_MAX
 * Q3: 树很深怎么办？→ 迭代 DFS 或直接用 BFS
 */
