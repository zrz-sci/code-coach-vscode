/*
 * LeetCode 971: Flip Binary Tree To Match Preorder Traversal
 *
 * [题目本质]
 * 给定二叉树和目标前序遍历 voyage，通过最少的翻转（交换左右子树）使前序遍历匹配 voyage。
 * 核心：DFS 模拟前序遍历，在每个节点判断是否需要翻转。
 *
 * [解法总览]
 * 解法1: DFS 递归（全局索引）  — O(n) / O(n) — 面试首选 ⭐
 * 解法2: 迭代栈模拟前序遍历    — O(n) / O(n) — 面试官要求迭代时
 */

// ============================================================
// 解法1: DFS 递归 — 全局索引 idx 跟踪 voyage 位置
// 时间: O(n)  空间: O(n) 递归栈
//
// [思路]
// 前序遍历：根 → 左 → 右。翻转某节点后变为：根 → 右 → 左。
//
// 用全局索引 idx 表示当前在 voyage 中期望匹配的位置。
// 每访问一个节点，先验证 node->val == voyage[idx]，然后 idx++。
//
// 关键决策：访问完当前节点后，下一个应该是左子节点。
// 如果 left->val != voyage[idx]，必须翻转当前节点（先递归右子树）。
// 如果翻转后右子节点也不匹配，则不可能。
//
// 遍历过程 (tree=[1,2,3], voyage=[1,3,2]):
//   idx=0: node=1, voyage[0]=1 ✓, idx→1
//     left=2, voyage[1]=3, 2≠3 → 翻转节点1, flipped=[1]
//     先右(3): idx=1, node=3, voyage[1]=3 ✓, idx→2
//     后左(2): idx=2, node=2, voyage[2]=2 ✓, idx→3
//   结果: [1]
//
// 不可能的情况 (tree=[1,2], voyage=[2,1]):
//   idx=0: node=1, voyage[0]=2, 1≠2 → impossible!
//   结果: [-1]
// ============================================================
class Solution1 {
public:
    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        vector<int> flipped;
        int idx = 0;
        bool possible = true;

        dfs(root, voyage, idx, flipped, possible);

        if (!possible) return {-1};
        return flipped;
    }

private:
    void dfs(TreeNode* node, vector<int>& voyage, int& idx,
             vector<int>& flipped, bool& possible) {
        // base case: 空节点或已确定不可能
        if (!node || !possible) return;

        // 当前节点值必须等于 voyage[idx]
        // 翻转只能交换左右子树，不能改变节点值
        if (node->val != voyage[idx]) {
            possible = false;
            return;
        }
        idx++; // 消费当前位置

        // 判断是否需要翻转：
        // 前序遍历中，访问完根后下一个应该是左子节点
        // 如果左子存在但其值 ≠ voyage[idx]，需要翻转（先访问右子）
        if (node->left && node->left->val != voyage[idx]) {
            flipped.push_back(node->val); // 记录翻转的节点
            // 翻转后的顺序：先右后左
            dfs(node->right, voyage, idx, flipped, possible);
            dfs(node->left, voyage, idx, flipped, possible);
        } else {
            // 正常前序顺序：先左后右
            dfs(node->left, voyage, idx, flipped, possible);
            dfs(node->right, voyage, idx, flipped, possible);
        }
    }
};

// ============================================================
// 解法1 变体: 用成员变量替代参数传递
// 时间: O(n)  空间: O(n)
//
// [思路]
// 将 idx, flipped, possible 作为成员变量，简化 DFS 签名。
// 逻辑完全相同，只是代码更简洁。
// ============================================================
class Solution1b {
public:
    int idx = 0;
    vector<int> flipped;
    bool possible = true;

    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        dfs(root, voyage);
        return possible ? flipped : vector<int>{-1};
    }

    void dfs(TreeNode* node, vector<int>& voyage) {
        if (!node || !possible) return;

        if (node->val != voyage[idx++]) {
            possible = false;
            return;
        }

        // 需要翻转的情况
        if (node->left && node->left->val != voyage[idx]) {
            flipped.push_back(node->val);
            dfs(node->right, voyage); // 先右
            dfs(node->left, voyage);  // 后左
        } else {
            dfs(node->left, voyage);  // 先左
            dfs(node->right, voyage); // 后右
        }
    }
};

// ============================================================
// 解法2: 迭代栈模拟前序遍历
// 时间: O(n)  空间: O(n)
//
// [思路]
// 用显式栈模拟前序遍历。标准前序用栈时先压右再压左（出栈先左后右）。
// 需要翻转时改为先压左再压右（出栈先右后左）。
//
// 判断是否需要翻转：
// 当节点有右子且 right->val == voyage[idx] 时，说明右子应该先访问
// （因为左子不匹配下一个位置），需要翻转。
//
// 注意：只有当节点同时有左右子时，翻转才有意义。
// 如果只有左子或只有右子，访问顺序是唯一的。
//
// 示例 (tree=[1,2,3], voyage=[1,3,2]):
//   栈: [1]
//   弹出1, voyage[0]=1 ✓, idx→1
//     right=3, voyage[1]=3 → 需要翻转! flipped=[1]
//     压入顺序: left=2, right=3 (出栈先3后2)
//   栈: [2, 3]
//   弹出3, voyage[1]=3 ✓, idx→2
//   栈: [2]
//   弹出2, voyage[2]=2 ✓, idx→3
//   栈: []
//   结果: [1]
// ============================================================
class Solution2 {
public:
    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        if (!root) return {};

        vector<int> flipped;
        stack<TreeNode*> stk;
        stk.push(root);
        int idx = 0;

        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();

            if (!node) continue;

            // 当前节点必须匹配
            if (node->val != voyage[idx++]) return {-1};

            // 判断是否需要翻转
            // 如果右子存在且 right->val == voyage[idx]（下一个期望值）
            // 说明应该先访问右子 → 翻转
            if (node->right && node->right->val == voyage[idx]) {
                // 需要翻转（前提是左子也存在，否则没有"翻转"的概念）
                if (node->left) flipped.push_back(node->val);
                // 先压左（后访问），再压右（先访问）
                stk.push(node->left);
                stk.push(node->right);
            } else {
                // 正常顺序：先压右（后访问），再压左（先访问）
                stk.push(node->right);
                stk.push(node->left);
            }
        }

        return flipped;
    }
};

// ============================================================
// [解法对比]
//
// | 解法 | 时间 | 空间 | 代码复杂度 | 推荐场景 |
// |------|------|------|------------|----------|
// | DFS 递归 | O(n) | O(n) | 简单 | ⭐ 面试首选 |
// | 成员变量版 | O(n) | O(n) | 最简洁 | 快速实现 |
// | 迭代栈 | O(n) | O(n) | 中等 | 面试官要求非递归 |
//
// [易错点]
//
// 1. idx 值传递导致左右子树不共享进度
//    ✗ void dfs(TreeNode* node, vector<int>& v, int idx, ...) // 值传递
//    ✓ void dfs(TreeNode* node, vector<int>& v, int& idx, ...) // 引用传递
//    → 左子树消费了 idx 后，右子树必须从更新后的 idx 继续
//
// 2. 翻转判断条件写反
//    ✗ if (node->left && node->left->val == voyage[idx]) // 相等时翻转
//    ✓ if (node->left && node->left->val != voyage[idx]) // 不相等才需要翻转
//    → 左子匹配就正常递归，不匹配才翻转
//
// 3. 迭代版中只有左子或只有右子时错误翻转
//    只有同时存在左右子时翻转才有意义
//    if (node->left) flipped.push_back(node->val); // 有左子才算翻转
//
// 4. 忘记 idx++ 在检查左子之前
//    当前节点消费 voyage[idx] 后 idx 才指向下一个位置
//    先 idx++ 再检查 node->left->val != voyage[idx]
//
// [面试追问]
//
// Q1: 为什么贪心是最优的？
// → 在每个节点，是否翻转是由 voyage 唯一确定的。不存在"翻转或不翻转都行"的情况。
//
// Q2: 空间能优化到 O(h) 吗？
// → 递归栈深度就是 O(h)，h 是树高。flipped 数组最坏 O(n)，但这是输出的一部分。
//
// Q3: 如果 voyage 的长度 != 节点数怎么办？
// → 题目保证 n == voyage.length，不需要处理。但防御性编程可以检查 idx < n。
// ============================================================
