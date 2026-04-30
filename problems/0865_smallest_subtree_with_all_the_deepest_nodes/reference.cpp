/*
 * LeetCode 865: 具有所有最深节点的最小子树
 * (同 LeetCode 1123: 最深叶节点的最近公共祖先)
 * 
 * 【题目本质】
 * 找到所有最深叶节点的最近公共祖先(LCA)。
 * 关键洞察：后序遍历时比较左右子树深度，
 * 深度相等 → 当前节点是LCA；不等 → 答案在更深的那一侧。
 *
 * 【解法总览】
 * 解法1: 两次遍历(求深度+找LCA)   — O(n) / O(n)
 * 解法2: 一次DFS返回pair(深度,节点) — O(n) / O(h) ⭐ 面试首选
 * 解法3: BFS逐层淘汰              — O(n) / O(n)
 */

// Definition for a binary tree node (provided by LeetCode)
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode() : val(0), left(nullptr), right(nullptr) {}
//     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
//     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
// };

// ============================================================
// 解法1: 两次遍历 — 先求深度，再找 LCA
// 时间: O(n)  空间: O(n) — 哈希表存每个节点深度
//
// 【思路】
// 最朴素的想法：
// 第一遍 DFS 求出每个节点的深度，找到最大深度 maxD。
// 第二遍 DFS 类似经典 LCA：从下往上，
//   如果当前节点深度 == maxD，返回自己（它是目标节点之一）
//   否则看左右子树的返回值：两边都非空 → 当前节点是 LCA
//
// 示例树:
//          3  (depth 0)
//         / \
//        5   1  (depth 1)
//       / \ / \
//      6  2 0  8  (depth 2)
//        / \
//       7   4  (depth 3) ← 最深
//
// 第二遍 DFS:
//   节点7: depth==3, 返回7
//   节点4: depth==3, 返回4
//   节点2: left=7, right=4, 两边都非空 → 返回节点2 (LCA)
//   节点6: depth!=3, 无子树返回 → 返回nullptr
//   节点5: left=nullptr, right=节点2 → 返回节点2
//   节点1子树: 没有depth==3的节点 → 返回nullptr
//   节点3: left=节点2, right=nullptr → 返回节点2 ✓
// ============================================================
class Solution1 {
public:
    unordered_map<TreeNode*, int> depth;
    int maxDepth = 0;
    
    void computeDepth(TreeNode* node, int d) {
        if (!node) return;
        depth[node] = d;
        maxDepth = max(maxDepth, d);
        computeDepth(node->left, d + 1);
        computeDepth(node->right, d + 1);
    }
    
    TreeNode* findLCA(TreeNode* node) {
        if (!node) return nullptr;
        // 当前节点是最深节点之一 → 返回自己
        if (depth[node] == maxDepth) return node;
        
        TreeNode* left = findLCA(node->left);
        TreeNode* right = findLCA(node->right);
        
        // 左右都找到了最深节点 → 当前节点是它们的 LCA
        if (left && right) return node;
        // 只有一边有 → 答案在那一边
        return left ? left : right;
    }
    
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        computeDepth(root, 0);
        return findLCA(root);
    }
};

// ============================================================
// 解法2: 一次 DFS 返回 pair — ⭐ 面试首选
// 时间: O(n)  空间: O(h) — h为树高，只需递归栈
//
// 【思路】
// 解法1 需要两次遍历 + 哈希表，能否合二为一？
// 
// 核心观察：后序遍历时，我们已经知道左右子树的深度了。
// 设计递归返回 pair<int, TreeNode*>:
//   - int: 当前子树的最大深度
//   - TreeNode*: 当前子树中包含所有最深节点的最小子树根
//
// 决策逻辑（这是本题的灵魂）：
//   leftDepth > rightDepth → 最深节点全在左边 → 返回左边的答案
//   leftDepth < rightDepth → 最深节点全在右边 → 返回右边的答案
//   leftDepth == rightDepth → 两边都有最深节点 → 当前节点是 LCA
//
// 逐步推演:
//          3
//         / \
//        5   1
//       / \ / \
//      6  2 0  8
//        / \
//       7   4
//
// dfs(7) = {1, 7}    dfs(4) = {1, 4}
// dfs(2): left={1,7}, right={1,4} → 深度相等! → {2, 节点2}
// dfs(6) = {1, 6}
// dfs(5): left={1,6}, right={2,节点2} → 右更深 → {3, 节点2}
// dfs(0) = {1, 0}    dfs(8) = {1, 8}
// dfs(1): left={1,0}, right={1,8} → 相等 → {2, 节点1}
// dfs(3): left={3,节点2}, right={2,节点1} → 左更深 → {4, 节点2} ✓
// ============================================================
class Solution2 {
public:
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        return dfs(root).second;
    }
    
private:
    // 返回 {子树最大深度, 包含所有最深节点的最小子树根}
    pair<int, TreeNode*> dfs(TreeNode* node) {
        if (!node) return {0, nullptr};
        
        auto [leftDepth, leftNode] = dfs(node->left);
        auto [rightDepth, rightNode] = dfs(node->right);
        
        if (leftDepth > rightDepth) {
            // 所有最深节点在左子树 → 答案来自左边
            return {leftDepth + 1, leftNode};
        } else if (leftDepth < rightDepth) {
            // 所有最深节点在右子树 → 答案来自右边
            return {rightDepth + 1, rightNode};
        } else {
            // 左右一样深 → 最深节点分布两侧 → 当前节点是LCA
            return {leftDepth + 1, node};
        }
    }
};

// ============================================================
// 解法3: BFS 逐层淘汰 — 迭代思路
// 时间: O(n)  空间: O(n) — BFS 队列 + 父节点哈希表
//
// 【思路】
// 完全不同的角度——从底层往上"收缩"：
// 1. BFS 遍历拿到最深层的所有节点
// 2. 把这些节点替换为它们的父节点，去重
// 3. 重复步骤2直到只剩一个节点，就是答案
//
// 示例推演:
//   最深层: {7, 4}
//   替换为父节点: {2}     ← 去重后只有一个 → 返回节点2 ✓
//
// 另一个例子（假设最深层是 {6, 7, 4}):
//   替换为父节点: {5, 2}  ← 6的父是5, 7和4的父是2
//   再替换: {3}           ← 5和2的父都是3 → 返回节点3
// ============================================================
class Solution3 {
public:
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        // 记录每个节点的父节点
        unordered_map<TreeNode*, TreeNode*> parent;
        queue<TreeNode*> q;
        q.push(root);
        parent[root] = nullptr;
        
        vector<TreeNode*> deepest;
        
        // BFS 层序遍历，结束后 deepest 存的是最后一层
        while (!q.empty()) {
            deepest.clear();
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front(); q.pop();
                deepest.push_back(node);
                if (node->left) {
                    parent[node->left] = node;
                    q.push(node->left);
                }
                if (node->right) {
                    parent[node->right] = node;
                    q.push(node->right);
                }
            }
        }
        
        // 逐层向上收缩：每个节点替换为父节点，去重
        while (deepest.size() > 1) {
            unordered_set<TreeNode*> nextLevel;
            for (TreeNode* node : deepest) {
                // 替换为父节点（去重由 set 保证）
                nextLevel.insert(parent[node]);
            }
            deepest.assign(nextLevel.begin(), nextLevel.end());
        }
        
        return deepest[0];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 遍历次数 | 时间 | 空间     | 代码量 | 推荐度 |
// |------|---------|------|----------|--------|--------|
// | 1    | 2次DFS  | O(n) | O(n)     | 中等   | 理解用 |
// | 2    | 1次DFS  | O(n) | O(h)     | 最少   | ⭐首选 |
// | 3    | BFS+收缩| O(n) | O(n)     | 最多   | 展示用 |
//
// 解法2 是最优雅的：一次后序遍历，pair 同时携带深度和答案，
// 空间只需递归栈 O(h)，代码最简洁。面试中首推。
//
// 【易错点】
//
// 1. 递归返回深度时忘记 +1：
//    ✗ return {leftDepth, leftNode};        // 深度没有随层级累加
//    ✓ return {leftDepth + 1, leftNode};    // 每上一层 +1
//
// 2. 空节点返回深度不一致导致 off-by-one：
//    ✗ if (!node) return {-1, nullptr};     // 后续 +1 后叶子深度为0，对比逻辑变复杂
//    ✓ if (!node) return {0, nullptr};      // 叶子深度为1，简单统一
//
// 3. 左右深度相等时返回了子节点而不是当前节点：
//    ✗ if (ld == rd) return {ld+1, leftNode};  // 应该返回当前节点作为LCA
//    ✓ if (ld == rd) return {ld+1, node};      // 两边都有最深节点 → 自己是LCA
//
// 4. 解法3中 BFS 收缩时没去重：
//    如果两个最深节点有相同父节点，不去重会导致多算一个，
//    收缩过程不会正确终止。必须用 set。
//
// 【面试追问 — 递进链】
//
// Q1: 这道题和 LeetCode 236 (LCA) 的关系？
//     → 本题 = "找所有最深叶子的 LCA"。经典 LCA 是给定两个节点，
//       本题目标节点由"最大深度"隐式决定。解法2的框架和236完全一致：
//       后序遍历，左右汇报，两边都有就返回自己。
//
// Q2: 能否不用递归，纯迭代实现解法2？
//     → 可以用后序迭代(两个栈法)模拟，但需要额外 map 存每个节点
//       的子树深度和答案，代码复杂很多。面试中说出思路即可，
//       实际写解法3的BFS方案更自然。
//
// Q3: 如果树非常深（百万级），递归栈溢出怎么办？
//     → 方案1: 转成迭代（解法3的BFS天然不用递归栈）
//       方案2: 如果是极端退化的链状树，可以先求最大深度，
//              再从根走到最深叶，路径上找分叉点即为答案。
//       方案3: 增大栈空间（工程手段，非算法面试答案）
// ============================================================
