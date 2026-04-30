/*
 * LeetCode 95: Unique Binary Search Trees II (不同的二叉搜索树 II)
 *
 * 给定整数 n, 生成所有由 1 到 n 构成的结构不同的 BST。
 *
 * 核心: 递归分治——枚举每个值 i 作为根, 递归生成左右子树列表, 笛卡尔积组合。
 *
 * 三种解法:
 *   解法1: 递归分治 (核心解法)
 *   解法2: 递归 + 记忆化 (避免重复子问题)
 *   解法3: DP + 值偏移 (自底向上, 利用结构同构性)
 */




// ===================== 解法1: 递归分治 (核心解法) =====================
//
// build(lo, hi): 生成由值 [lo, hi] 构成的所有 BST 的列表
//
// 关键:
//   - lo > hi 时返回 {nullptr} (不是空列表!)
//     null 代表"空子树是一种合法选择", 参与笛卡尔积
//   - BST 性质由值域分割自动保证: [lo,i-1] < i < [i+1,hi]
//
// n=3 时生成 5 棵树 (卡特兰数 C(3)=5):
//
//   1         1          2         3          3
//    \         \        / \       /          /
//     2         3      1   3     1          2
//      \       /                  \        /
//       3     2                    2      1
//
// 时间: O(n * C(n))  空间: O(n * C(n))   C(n) = 卡特兰数

class Solution1 {
public:
    vector<TreeNode*> generateTrees(int n) {
        if (n == 0) return {};
        return build(1, n);
    }

private:
    vector<TreeNode*> build(int lo, int hi) {
        // Base case: 空范围 → 返回包含 null 的列表
        // 【关键易错点】返回 {} 会导致笛卡尔积循环不执行,
        // 丢失"左/右子树为空"的合法结构
        if (lo > hi) return {nullptr};

        vector<TreeNode*> result;

        // 枚举 i 作为根节点
        for (int i = lo; i <= hi; i++) {
            // 递归: 左子树由 [lo, i-1] 构成 (BST: 全部 < i)
            vector<TreeNode*> leftTrees = build(lo, i - 1);
            // 递归: 右子树由 [i+1, hi] 构成 (BST: 全部 > i)
            vector<TreeNode*> rightTrees = build(i + 1, hi);

            // 笛卡尔积: |leftTrees| * |rightTrees| 种组合
            for (TreeNode* left : leftTrees) {
                for (TreeNode* right : rightTrees) {
                    TreeNode* root = new TreeNode(i, left, right);
                    result.push_back(root);
                }
            }
        }

        return result;
    }
};


// ===================== 解法2: 递归 + 记忆化 =====================
//
// 优化: build(lo, hi) 可能在不同上下文被重复调用
//   例: n=5 时, build(1,2) 会被 根=3 和 根=4→根=3 多次调用
//   用 memo[{lo, hi}] 缓存结果
//
// 注意: 缓存后不同树共享子树指针
//   只读安全; 若需修改节点值, 组合时需深拷贝

class Solution2 {
public:
    vector<TreeNode*> generateTrees(int n) {
        if (n == 0) return {};
        memo.clear();
        return build(1, n);
    }

private:
    map<pair<int,int>, vector<TreeNode*>> memo;

    vector<TreeNode*> build(int lo, int hi) {
        if (lo > hi) return {nullptr};

        auto key = make_pair(lo, hi);
        if (memo.count(key)) return memo[key];

        vector<TreeNode*> result;
        for (int i = lo; i <= hi; i++) {
            auto lefts  = build(lo, i - 1);
            auto rights = build(i + 1, hi);
            for (auto left : lefts) {
                for (auto right : rights) {
                    result.push_back(new TreeNode(i, left, right));
                }
            }
        }

        return memo[key] = result;
    }
};


// ===================== 解法3: DP + 值偏移 =====================
//
// 核心观察: build(1,3) 和 build(4,6) 的树结构完全相同,
//           只是节点值偏移了 3
//
// 因此只需计算 dp[len] = build(1, len):
//   build(lo, hi) 等价于 dp[hi-lo+1], 每个节点值 +(lo-1)
//
// 需要深拷贝 + 值偏移, 保证每棵树独立

class Solution3 {
public:
    vector<TreeNode*> generateTrees(int n) {
        if (n == 0) return {};

        // dp[len] = 用值 [1..len] 构建的所有 BST
        vector<vector<TreeNode*>> dp(n + 1);
        dp[0] = {nullptr};  // 空树

        for (int len = 1; len <= n; len++) {
            for (int root = 1; root <= len; root++) {
                // 左子树: [1, root-1] → dp[root-1], 无偏移
                // 右子树: [root+1, len] → dp[len-root], 值偏移 root
                for (TreeNode* left : dp[root - 1]) {
                    for (TreeNode* right : dp[len - root]) {
                        TreeNode* node = new TreeNode(root);
                        node->left  = clone(left, 0);       // 左子树值不变
                        node->right = clone(right, root);    // 右子树值 +root
                        dp[len].push_back(node);
                    }
                }
            }
        }

        return dp[n];
    }

private:
    // 深拷贝树, 每个节点值 +offset
    TreeNode* clone(TreeNode* node, int offset) {
        if (!node) return nullptr;
        TreeNode* copy = new TreeNode(node->val + offset);
        copy->left  = clone(node->left, offset);
        copy->right = clone(node->right, offset);
        return copy;
    }
};


/*
 * ==================== 手动演算 ====================
 *
 * 输入: n = 3
 *
 * build(1, 3):
 *
 *   根=1: lefts = build(1,0) = [null]
 *          rights = build(2,3):
 *            根=2: [null] x [Node(3)] → Node(2, null, 3)
 *            根=3: [Node(2)] x [null] → Node(3, 2, null)
 *          rights = [Node(2,null,3), Node(3,2,null)]
 *          组合:
 *            (null, Node(2,null,3)) → Node(1, null, Node(2,null,3))    树: 1→2→3
 *            (null, Node(3,2,null)) → Node(1, null, Node(3,2,null))    树: 1→3←2
 *
 *   根=2: lefts = [Node(1)], rights = [Node(3)]
 *          → Node(2, Node(1), Node(3))                                 树: 2←1, 2→3
 *
 *   根=3: lefts = build(1,2):
 *            根=1: [null] x [Node(2)] → Node(1, null, 2)
 *            根=2: [Node(1)] x [null] → Node(2, 1, null)
 *          rights = [null]
 *          组合:
 *            (Node(1,null,2), null) → Node(3, Node(1,null,2), null)    树: 3←1→2
 *            (Node(2,1,null), null) → Node(3, Node(2,1,null), null)    树: 3←2←1
 *
 * 输出: 5 棵树 (C(3)=5) ✓
 *
 *
 * 输入: n = 1
 * build(1,1): 根=1, lefts=[null], rights=[null]
 *             → Node(1, null, null)
 * 输出: [[1]] ✓
 *
 *
 * ==================== 卡特兰数参考 ====================
 *
 * n:     0  1  2  3   4   5    6    7     8
 * C(n):  1  1  2  5  14  42  132  429  1430
 *
 * 递推: C(n) = sum_{i=0}^{n-1} C(i) * C(n-1-i)
 * 即: 左子树 i 个节点的方案 * 右子树 (n-1-i) 个节点的方案
 *
 *
 * ==================== 面试追问 ====================
 *
 * Q1: 只需要数量怎么办？→ LC 96，DP 计算卡特兰数
 * Q2: 时间复杂度怎么分析？→ 输出本身 C(n) 棵树 * n 节点 = O(n*C(n))
 * Q3: 为什么不能用回溯？→ 需要保留所有树(不能撤销)，是递归生成+组合
 * Q4: 子树共享安全吗？→ 只读安全；需修改节点值时要深拷贝
 */
