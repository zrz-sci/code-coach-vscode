/*
 * LeetCode 107: Binary Tree Level Order Traversal II (二叉树的层序遍历 II)
 *
 * 【题目本质】
 * 和 LC 102 完全相同的 BFS 层序遍历，只是输出顺序从底层到顶层。
 * 核心：BFS 收集每层 → reverse 结果。
 *
 * 【解法总览】
 * Solution1: BFS + reverse — O(n) / O(n) — ⭐ 面试首选
 * Solution2: DFS 深度分组 + reverse — O(n) / O(n)
 * Solution3: BFS + deque 头插法 — O(n) / O(n)
 *
 * 【示例树】
 *         3            Level 0: [3]
 *        / \
 *       9   20         Level 1: [9, 20]
 *          / \
 *         15   7       Level 2: [15, 7]
 *
 *   正序: [[3],[9,20],[15,7]]
 *   反序: [[15,7],[9,20],[3]]  ← 本题要求
 */

// ============================================================
// Solution1: BFS + reverse
// 时间 O(n)  空间 O(n)
//
// 【思路】
// 标准 BFS 层序遍历（和 LC 102 一模一样），收集完所有层后
// 一次 reverse 得到自底向上的顺序。
// reverse 只交换 vector 指针（move semantics），O(层数) 不是 O(n)。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size(); // 当前层的节点数
            vector<int> level;
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(level);
        }

        // 唯一和 LC 102 不同的地方
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// Solution2: DFS 深度分组
// 时间 O(n)  空间 O(n)
//
// 【思路】
// 用递归遍历树，depth 参数决定当前节点放到 result 的哪一层。
// depth == result.size() 时说明遇到了新的一层，push 一个空 vector。
// 最后同样 reverse。
//
// 和 Solution1 的区别：用递归栈代替显式队列，
// 适合展示递归思维，但深度大时有栈溢出风险。
// ============================================================
class Solution2 {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        reverse(result.begin(), result.end());
        return result;
    }

private:
    void dfs(TreeNode* node, int depth, vector<vector<int>>& result) {
        if (!node) return;
        // 遇到新层，先开辟空间
        if (depth == (int)result.size()) {
            result.push_back({});
        }
        result[depth].push_back(node->val);
        dfs(node->left, depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};

// ============================================================
// Solution3: BFS + deque 头插法
// 时间 O(n)  空间 O(n)
//
// 【思路】
// 用 deque 代替 vector 存结果，每层 push_front，
// 天然是反序，不需要最后 reverse。
// 代价：deque 内存布局不如 vector 紧凑，常数略大。
// ============================================================
class Solution3 {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        deque<vector<int>> dq;
        if (!root) return {};

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            dq.push_front(level); // 头插，天然反序
        }

        return vector<vector<int>>(dq.begin(), dq.end());
    }
};

// ============================================================
// 【解法对比】
// | 解法       | 额外操作    | 代码量 | 面试推荐 |
// |-----------|-----------|-------|---------|
// | Solution1 | reverse   | 最少   | ⭐ 首选  |
// | Solution2 | reverse   | 中等   | 展示递归  |
// | Solution3 | push_front| 中等   | 展示STL  |
//
// 【易错点】
// 1. 忘记判空：root == nullptr 时返回 {}，不要进入 while
//    ✗ 没有 if(!root) return → 不报错但不规范
//    ✓ 显式判空
//
// 2. DFS 中 depth == result.size() 必须用 == 不是 >=
//    ✗ depth >= result.size() → 层被跳过时创建多余空层
//    ✓ depth == result.size() → 精确控制
//
// 3. reverse 对象是层的顺序，不是层内元素
//    ✗ 对每层元素 reverse → 变成 [7,15] 而非 [15,7]
//    ✓ 对 result（vector of vectors）做 reverse
//
// 4. vector 头插 O(k)，不如 deque::push_front O(1)
//    ✗ result.insert(result.begin(), level)
//    ✓ 用 deque 或直接 reverse
//
// 【举一反三】
// - 102. 层序遍历：去掉 reverse 即可，代码只差一行
// - 103. 锯齿形层序：奇数层 reverse，复用 BFS 框架
// - 199. 右视图：BFS 取每层最后一个，复用 size 分层技巧
// - 637. 层平均值：BFS 每层求 sum/size
//
// 【面试追问 Interview Follow-ups】
// Q1: 和 LC 102 的区别？
// A1: 只多一行 reverse。面试先说清这个关系再写代码。
//
// Q2: reverse 复杂度？会成瓶颈吗？
// A2: O(层数)，move semantics 只交换指针，平衡树 O(log n)，
//     完全不是瓶颈。
//
// Q3: 不用 reverse 怎么做？
// A3: deque + push_front 天然反序；或 DFS 预知深度后
//     用 depth 倒序索引。实际面试 reverse 最简洁。
//
// Q4: 树很深时 DFS 的问题？
// A4: 栈溢出。链状树递归深度 O(n)。用 BFS 迭代更安全。
// ============================================================
