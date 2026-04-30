/*
 * 298. Binary Tree Longest Consecutive Sequence
 *
 * 【题目本质】
 * 在二叉树中找最长的父->子递增1连续路径。
 * 经典 DFS 自顶向下传递状态的模式。
 *
 * 【解法总览】
 * Solution1: DFS 自顶向下  O(n) / O(h)  ⭐推荐
 * Solution2: DFS 自底向上  O(n) / O(h)
 * Solution3: BFS 层序      O(n) / O(w)
 */

// ===================== Solution1: DFS 自顶向下 =====================
// 思路：传入父节点和当前连续长度，子节点值==父值+1则延续，否则重置。
class Solution1 {
public:
    int longestConsecutive(TreeNode* root) {
        int maxLen = 0;
        dfs(root, nullptr, 0, maxLen);
        return maxLen;
    }

private:
    void dfs(TreeNode* node, TreeNode* parent, int curLen, int& maxLen) {
        if (!node) return;

        if (parent && node->val == parent->val + 1) {
            curLen++;
        } else {
            curLen = 1;
        }

        maxLen = max(maxLen, curLen);
        dfs(node->left, node, curLen, maxLen);
        dfs(node->right, node, curLen, maxLen);
    }
};

// ===================== Solution2: DFS 自底向上 =====================
// 思路：返回以当前节点结尾的最长连续长度，父节点判断是否与子节点衔接。
class Solution2 {
public:
    int longestConsecutive(TreeNode* root) {
        int maxLen = 0;
        dfs(root, maxLen);
        return maxLen;
    }

private:
    // 返回：以 node 为起点向下的最长连续长度
    int dfs(TreeNode* node, int& maxLen) {
        if (!node) return 0;

        int leftLen = dfs(node->left, maxLen);
        int rightLen = dfs(node->right, maxLen);

        int curLen = 1;
        if (node->left && node->left->val == node->val + 1) {
            curLen = max(curLen, leftLen + 1);
        }
        if (node->right && node->right->val == node->val + 1) {
            curLen = max(curLen, rightLen + 1);
        }

        maxLen = max(maxLen, curLen);
        return curLen;
    }
};

// ===================== Solution3: BFS =====================
// 思路：层序遍历，队列存 {节点, 当前连续长度}。
class Solution3 {
public:
    int longestConsecutive(TreeNode* root) {
        if (!root) return 0;

        int maxLen = 1;
        queue<pair<TreeNode*, int>> q;
        q.push({root, 1});

        while (!q.empty()) {
            auto [node, len] = q.front();
            q.pop();

            for (TreeNode* child : {node->left, node->right}) {
                if (!child) continue;
                int newLen = (child->val == node->val + 1) ? len + 1 : 1;
                maxLen = max(maxLen, newLen);
                q.push({child, newLen});
            }
        }

        return maxLen;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 自顶向下 | O(n) | O(h) | 最直观，面试首选 |
 * | Solution2 自底向上 | O(n) | O(h) | 为549进阶版做铺垫 |
 * | Solution3 BFS | O(n) | O(w) | 迭代实现，无递归栈 |
 *
 * 【易错点】
 * 1. 方向错误：要求父->子递增(val+1)，不是递减
 * 2. 初始长度=1：单个节点本身就是长度1的序列
 * 3. 自底向上时：返回值是向下延伸长度，不是全局最长
 * 4. 空节点处理：返回0，不是1
 *
 * 【面试追问】
 * Q1: 如果允许递增+递减(549)？-> 自底向上返回(inc, dec)，合并检查
 * Q2: 如果路径可以从子到父？-> 变成无向图DFS，每个节点标记visited
 * Q3: 空间优化？-> Morris遍历O(1)空间但破坏树结构，通常不推荐
 */
