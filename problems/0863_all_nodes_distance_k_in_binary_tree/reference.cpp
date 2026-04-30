/*
 * LeetCode 863: 二叉树中所有距离为 K 的结点
 *
 * 【题目本质】
 * 在二叉树中找所有距离 target 为 k 的节点。
 * 树是特殊的无向图，但只有从父到子的指针。
 * 核心难点：如何让节点能"往上走"到父节点。
 *
 * 【解法总览】
 * 解法1: 建父指针 + BFS — O(n) / O(n) — 最直觉，面试首选
 * 解法2: 纯 DFS 递归      — O(n) / O(n) — 不建图，递归传距离
 */

/*
 *  示例树结构:
 *
 *          3              target = 5, k = 2
 *         / \
 *        5   1            答案: [7, 4, 1]
 *       / \ / \
 *      6  2 0  8          7: target 子树内, 深度2
 *        / \              4: target 子树内, 深度2
 *       7   4             1: 通过父节点3绕过去, 距离2
 *
 *  BFS 从 target=5 出发:
 *  第0层: [5]
 *  第1层: [6, 2, 3]       ← 左子、右子、父节点(通过parent映射)
 *  第2层: [7, 4, 1]       ← 到达 k=2, 收集答案!
 *    (从2展开得7,4)  (从3展开得1, 左子5已visited跳过)
 */

// ============================================================
// 解法1: 建父指针 + BFS — 树转无向图，标准 BFS 找第 k 层
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 树只有从上到下的指针。如果每个节点还能访问父节点，
// 那么从 target 出发就有三个方向（左子、右子、父），
// 问题退化为无向图上 BFS 找第 k 层。
//
// 做法:
// 1. 一次 DFS 遍历，用哈希表记录 child→parent 映射
// 2. 从 target 出发 BFS，扩展三个方向，visited 防回头
// 3. BFS 到第 k 层时，收集该层所有节点值
// ============================================================
class Solution1 {
public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        // Step1: 建立父指针映射
        unordered_map<TreeNode*, TreeNode*> parent;
        function<void(TreeNode*, TreeNode*)> buildParent = [&](TreeNode* node, TreeNode* par) {
            if (!node) return;
            parent[node] = par;
            buildParent(node->left, node);
            buildParent(node->right, node);
        };
        buildParent(root, nullptr);

        // Step2: BFS 从 target 出发
        queue<TreeNode*> q;
        unordered_set<TreeNode*> visited;
        q.push(target);
        visited.insert(target); // 入队时就标记，不是出队时!

        int dist = 0;
        while (!q.empty()) {
            // 到达第 k 层，当前队列中所有节点就是答案
            if (dist == k) {
                vector<int> res;
                while (!q.empty()) {
                    res.push_back(q.front()->val);
                    q.pop();
                }
                return res;
            }

            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* cur = q.front();
                q.pop();
                // 向三个方向扩展: 左子、右子、父节点
                for (TreeNode* next : {cur->left, cur->right, parent[cur]}) {
                    if (next && !visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
            dist++;
        }

        return {}; // k 超出范围，无节点满足
    }
};

// ============================================================
// 解法2: 纯 DFS（递归传距离）— 不建图，利用递归返回值
// 时间: O(n)  空间: O(n)（递归栈 + 结果数组）
//
// 【思路】
// 距离 target 为 k 的节点分两类:
// (A) target 子树内: 从 target 向下, 深度恰好为 k 的节点
// (B) target 子树外: 通过某个祖先 A "绕"到另一侧
//     如果 A 到 target 距离是 d, 则在 A 的另一侧子树找深度 k-d-1 的节点
//     (减1是因为从A到另一侧子树根还要走一步)
//
// 递归函数 dfs(node) 返回:
//   node 到 target 的距离 (如果 target 在 node 子树中)
//   -1 (如果 target 不在 node 子树中)
//
// 图示"绕路"过程:
//
//        A (d=leftDist+1 到 target)
//       / \
//    ...   R   ← 在 R 子树中找深度 k-d-1 的节点
//     |
//   target
//
// ============================================================
class Solution2 {
public:
    vector<int> res;

    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        dfs(root, target, k);
        return res;
    }

    // 返回 node 到 target 的距离; target 不在子树中则返回 -1
    int dfs(TreeNode* node, TreeNode* target, int k) {
        if (!node) return -1;

        // 找到 target 本身
        if (node == target) {
            collect(node, k); // 在 target 子树内找深度为 k 的节点
            return 0;         // target 到自己的距离是 0
        }

        // 先在左子树中找 target
        int leftDist = dfs(node->left, target, k);
        if (leftDist != -1) {
            // target 在左子树中, node 到 target 距离是 leftDist + 1
            int d = leftDist + 1;
            if (d == k) {
                res.push_back(node->val); // node 本身就是答案
            } else {
                // 在右子树中找距离为 k - d - 1 的节点
                // 为什么减1: 从 node 到 node->right 还要走一步
                collect(node->right, k - d - 1);
            }
            return d; // 找到了就返回, 不再搜右子树找 target
        }

        // 再在右子树中找 target
        int rightDist = dfs(node->right, target, k);
        if (rightDist != -1) {
            int d = rightDist + 1;
            if (d == k) {
                res.push_back(node->val);
            } else {
                collect(node->left, k - d - 1);
            }
            return d;
        }

        return -1; // target 不在 node 子树中
    }

    // 从 node 出发, 收集子树中深度恰好为 depth 的所有节点
    void collect(TreeNode* node, int depth) {
        // depth < 0: 说明已经超过需要的距离, 提前终止
        if (!node || depth < 0) return;
        if (depth == 0) {
            res.push_back(node->val);
            return;
        }
        collect(node->left, depth - 1);
        collect(node->right, depth - 1);
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 建图+BFS     | 解法2: 纯DFS        |
// |-------------|--------------------|--------------------|
// | 核心思想     | 树→无向图, BFS k层  | 递归返回值传距离      |
// | 代码量       | 短, 逻辑直白        | 长, 递归分类讨论      |
// | 易出bug      | 低                  | 中高(距离计算易错)    |
// | 面试推荐     | ⭐ 首选             | 加分项              |
//
// 两者时间空间复杂度相同, 都是 O(n)/O(n)。
// 面试中解法1思路更容易讲清楚; 解法2适合展示递归功底。
//
// ============================================================
// 【易错点】
//
// 1. BFS visited 标记时机:
//    ✗ 出队时 visited.insert(cur); → 同一节点被左邻居和右邻居各入队一次
//    ✓ 入队时 visited.insert(next); → 保证每个节点只入队一次
//
// 2. 纯 DFS 中 collect 的 depth < 0 判断:
//    ✗ void collect(TreeNode* node, int depth) {
//         if (!node) return;  // 漏掉 depth<0 的检查
//    ✓ void collect(TreeNode* node, int depth) {
//         if (!node || depth < 0) return;  // k-d-1 可能为负, 必须提前终止
//
// 3. 纯 DFS 中左子树找到 target 后继续搜右子树:
//    ✗ int leftDist = dfs(...); // 找到了但没 return
//       int rightDist = dfs(...); // 又搜一遍, 产生错误结果
//    ✓ if (leftDist != -1) { ... return d; } // 找到后立即返回
//
// 4. parent 映射中访问不存在的 key:
//    ✗ 对 root 节点做 parent[root], 但没有初始化 → 未定义行为
//    ✓ buildParent(root, nullptr) 确保 root 的 parent 是 nullptr,
//       BFS 中用 if (next && ...) 过滤掉 nullptr
//
// ============================================================
// 【面试追问】
//
// Q1(基础): "为什么不能只从 target 往下 DFS?"
// → 往下只能找 target 子树内的节点。距离 k 的节点可能在
//   target 的祖先方向, 需要"往上走", 而树没有父指针。
//
// Q2(优化): "BFS 解法能否不用哈希表存父指针?"
// → 可以, 用解法2的纯 DFS 思路, 递归中利用返回值传递距离,
//   不需要显式的父指针。代价是代码更复杂。
//
// Q3(变体): "如果不是二叉树而是 N 叉树?"
// → 思路完全一样。解法1: BFS 扩展时遍历 children + parent。
//   解法2: dfs 中遍历所有子节点找 target, collect 也遍历所有子节点。
//
// Q4(变体): "如果 target 给的是值而不是指针?"
// → 先做一次 DFS 找到对应的 TreeNode*, 或在建 parent 映射时
//   顺便找到 target 指针, 然后按同样方法解。
// ============================================================
