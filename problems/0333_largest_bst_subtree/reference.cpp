/*
 * 【题目本质】
 * 在二叉树中找到节点数最多的 BST 子树。
 * 核心：后序遍历自底向上收集子树信息（是否BST/大小/值域），一次 O(n) 遍历。
 *
 * 【解法总览】
 * Solution1: 后序遍历 + 信息汇总 — O(n) 时间, O(h) 空间 ⭐推荐
 * Solution2: 暴力逐节点验证 BST — O(n^2) 时间, O(h) 空间
 */

// ===== Solution1: 后序遍历 + 信息汇总 =====
// 思路：定义 Info{isBST, size, minVal, maxVal}，自底向上合并
// 空节点：isBST=true, size=0, min=INT_MAX, max=INT_MIN
class Solution1 {
public:
    int largestBSTSubtree(TreeNode* root) {
        int ans = 0;
        dfs(root, ans);
        return ans;
    }

private:
    struct Info {
        bool isBST;
        int size;
        int minVal, maxVal;
    };

    Info dfs(TreeNode* node, int& ans) {
        if (!node) return {true, 0, INT_MAX, INT_MIN};

        auto left = dfs(node->left, ans);
        auto right = dfs(node->right, ans);

        // 当前节点能构成 BST 的条件：
        // 1. 左右子树都是 BST
        // 2. 左子树最大值 < 当前值 < 右子树最小值
        if (left.isBST && right.isBST &&
            left.maxVal < node->val && node->val < right.minVal) {
            int size = left.size + right.size + 1;
            ans = max(ans, size);
            return {true, size,
                    min(left.minVal, node->val),   // 整棵子树的最小值
                    max(right.maxVal, node->val)};  // 整棵子树的最大值
        }

        // 不是 BST，向上传递 false
        return {false, 0, 0, 0};
    }
};

// ===== Solution2: 暴力验证每个子树 =====
// 思路：对每个节点调用 isValidBST，若合法则统计节点数
// 缺点：重复遍历导致 O(n^2)
class Solution2 {
public:
    int largestBSTSubtree(TreeNode* root) {
        if (!root) return 0;
        if (isValidBST(root, LONG_MIN, LONG_MAX))
            return countNodes(root);
        return max(largestBSTSubtree(root->left),
                   largestBSTSubtree(root->right));
    }

private:
    bool isValidBST(TreeNode* node, long lo, long hi) {
        if (!node) return true;
        if (node->val <= lo || node->val >= hi) return false;
        return isValidBST(node->left, lo, node->val) &&
               isValidBST(node->right, node->val, hi);
    }

    int countNodes(TreeNode* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};

/*
 * 【解法对比】
 * Solution1: O(n) 时间, O(h) 空间 — 每个节点只访问一次，后序合并信息
 * Solution2: O(n^2) 时间, O(h) 空间 — 每个节点可能触发完整子树验证
 *
 * 【易错点】
 * 1. 空节点 min/max 设置：min=INT_MAX, max=INT_MIN（保证父节点比较总成立）
 * 2. BST 严格不等：left.maxVal < node->val（不是 <=）
 * 3. 非 BST 子树的信息无需向上传递，直接返回 {false, 0, 0, 0}
 * 4. 更新 ans 要在合并成功时立即执行
 *
 * 【面试追问】
 * Q1: 为什么空节点 min=INT_MAX, max=INT_MIN？
 *     → 保证 left.maxVal(INT_MIN) < node->val 和 node->val < right.minVal(INT_MAX) 恒成立
 * Q2: 如何返回最大 BST 子树的根节点？
 *     → 在更新 ans 时同时记录 node 指针
 * Q3: 与 1373 题（最大和BST）有何异同？
 *     → 结构完全相同，只是 ans 从记录 size 改为记录 sum
 */
