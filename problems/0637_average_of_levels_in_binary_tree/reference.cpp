/*
 * LeetCode 637: Average of Levels in Binary Tree (二叉树的层平均值)
 *
 * 【题目本质】
 * 逐层遍历二叉树，每层计算节点值的平均值。
 * 经典 BFS 分层模板题 — 几乎所有"逐层处理"的树题都用相同模板。
 *
 * 【解法总览】
 * 解法1: BFS 层序遍历 — O(n) / O(w) — 面试首选
 * 解法2: DFS 递归      — O(n) / O(h) — 替代方案
 */


// TreeNode 定义


// ============================================================
// 解法1: BFS 层序遍历 — 面试首选 ⭐
// 时间: O(n)  空间: O(w)  w = 树的最大宽度
//
// 【思路】
// BFS 天然按层遍历。关键技巧：
// 每轮循环开始时记录 levelSize = q.size()，
// 然后恰好处理 levelSize 个节点。
// 处理完后，队列中剩下的全是下一层的节点。
//
// 【ASCII 图 — BFS 逐层处理过程】
//
//         3            ← Level 0
//        / \
//       9   20         ← Level 1
//          /  \
//        15    7       ← Level 2
//
// 初始: queue = [3]
//
// ┌─ Level 0: levelSize = 1 ──────────────────────┐
// │  处理 3 → sum = 3                              │
// │  入队子节点: 9, 20                              │
// │  avg = 3 / 1 = 3.00                            │
// │  queue = [9, 20]                               │
// └────────────────────────────────────────────────┘
//
// ┌─ Level 1: levelSize = 2 ──────────────────────┐
// │  处理 9  → sum = 9     (9 无子节点, 不入队)     │
// │  处理 20 → sum = 29    (入队 15, 7)            │
// │  avg = 29 / 2 = 14.50                          │
// │  queue = [15, 7]                               │
// └────────────────────────────────────────────────┘
//
// ┌─ Level 2: levelSize = 2 ──────────────────────┐
// │  处理 15 → sum = 15    (叶子, 不入队)           │
// │  处理 7  → sum = 22    (叶子, 不入队)           │
// │  avg = 22 / 2 = 11.00                          │
// │  queue = []  ← 空, BFS 结束                    │
// └────────────────────────────────────────────────┘
//
// 结果: [3.00, 14.50, 11.00]
//
// 【溢出陷阱】
// Node.val 范围 [-2^31, 2^31-1]，最多 10^4 个节点。
// 如果用 int sum 累加: 10000 * 2^31 ≈ 2 * 10^13 → int 溢出!
// 必须用 double 或 long long 累加。
// ============================================================
class Solution1 {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> result;
        if (!root) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            // ✓ 在循环外保存 levelSize
            // ✗ 不能在 for 条件中写 q.size()，因为循环体内会 push 改变大小
            int levelSize = q.size();

            // 用 double 累加，避免 int 溢出
            // (即使用 long long 也可以，但 double 更直接)
            double sum = 0;

            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();

                sum += node->val;

                // 子节点入队 —— 只入队非空节点
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }

            // 当前层处理完毕，计算平均值
            result.push_back(sum / levelSize);
        }

        return result;
    }
};


// ============================================================
// 解法2: DFS 递归 — 替代方案
// 时间: O(n)  空间: O(h)  h = 树高
//
// 【思路】
// DFS 遍历时传入当前节点的层级 depth。
// 用两个数组记录：
//   sums[depth]   = 第 depth 层的节点值之和
//   counts[depth] = 第 depth 层的节点个数
// 最后逐层计算 sums[i] / counts[i]。
//
// 【DFS 过程图示】
//
//       3(d=0)        遍历顺序:
//      / \            3(d=0) → 9(d=1) → 20(d=1) → 15(d=2) → 7(d=2)
//   9(d=1) 20(d=1)
//          / \        处理后:
//      15(d=2) 7(d=2) sums   = [3.0, 29.0, 22.0]
//                     counts = [1,   2,    2   ]
//                     result = [3.0, 14.5, 11.0]
//
// 【空间分析】
// 递归栈深度 = 树高 h。
// 平衡二叉树: h = O(log n)，比 BFS 的 O(n/2) 更优。
// 链状树:     h = O(n)，与 BFS 的 O(1) 相比更差。
// ============================================================
class Solution2 {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> sums;    // 各层节点值之和
        vector<int> counts;     // 各层节点个数

        dfs(root, 0, sums, counts);

        // 计算各层平均值
        vector<double> result;
        for (int i = 0; i < (int)sums.size(); i++) {
            result.push_back(sums[i] / counts[i]);
        }
        return result;
    }

    void dfs(TreeNode* node, int depth,
             vector<double>& sums, vector<int>& counts) {
        if (!node) return;

        // 第一次到达这一层 → 初始化
        // depth == sums.size() 是因为 DFS 先深入左子树，
        // 每次到达新层级时 sums 的大小恰好等于当前层级
        if (depth == (int)sums.size()) {
            sums.push_back(0);
            counts.push_back(0);
        }

        // 累加当前节点的值到对应层级
        sums[depth] += node->val;
        counts[depth]++;

        // 递归左右子树，层级 +1
        dfs(node->left,  depth + 1, sums, counts);
        dfs(node->right, depth + 1, sums, counts);
    }
};


// ============================================================
// 【解法对比】
//
// | 解法       | 时间  | 空间         | 优势            | 劣势           |
// |-----------|-------|-------------|----------------|---------------|
// | BFS 层序  | O(n)  | O(w) 宽度   | 直觉匹配"逐层"  | 宽树空间大      |
// | DFS 递归  | O(n)  | O(h) 树高   | 深树空间小      | 需辅助数组      |
//
// 完全二叉树: BFS O(n/2), DFS O(log n) → DFS 更优
// 链状树:     BFS O(1),   DFS O(n)    → BFS 更优
// 一般情况:   差别不大，BFS 代码更简洁直觉
//
// ============================================================
// 【BFS 分层模板 — 通用版】
//
// 几乎所有"逐层处理"的树题都可以用这个模板:
//
// queue<TreeNode*> q;
// q.push(root);
// while (!q.empty()) {
//     int levelSize = q.size();        // ← 关键：先保存当前层大小
//     for (int i = 0; i < levelSize; i++) {
//         TreeNode* node = q.front();
//         q.pop();
//         // ... 对 node 做本层操作 ...
//         if (node->left)  q.push(node->left);
//         if (node->right) q.push(node->right);
//     }
//     // ... 本层处理完毕，做汇总操作 ...
// }
//
// 变种:
// - 层平均值(本题):   sum += val, push_back(sum / levelSize)
// - 层最大值(515):    maxVal = max(maxVal, val)
// - 右视图(199):      只取每层最后一个节点 (i == levelSize - 1)
// - 锯齿形(103):     奇数层翻转当前层列表
//
// ============================================================
// 【易错点】
//
// 1. sum 用 int 导致溢出:
//    ✗ int sum = 0;  → 10000 * 2^31 溢出
//    ✓ double sum = 0; 或 long long sum = 0;
//
// 2. for 条件中直接写 q.size():
//    ✗ for (int i = 0; i < q.size(); i++) { q.push(...); }
//      → q.size() 每次迭代重新计算，入队后变大，循环次数不对
//    ✓ int levelSize = q.size(); for (...levelSize...)
//
// 3. DFS 中 depth 判断不精确:
//    ✗ if (depth >= sums.size()) → 可能跳过中间层
//    ✓ if (depth == sums.size()) → 恰好到达新层时初始化
//
// 4. 忘记检查子节点为 null:
//    ✗ q.push(node->left); → 空指针入队，下一轮取出会崩溃
//    ✓ if (node->left) q.push(node->left);
//
// ============================================================
// 【面试追问】
//
// Q1: BFS vs DFS 空间哪个更优？
//     取决于树形状。完全二叉树→DFS优 O(logn)。
//     链状树→BFS优 O(1)。
//
// Q2: 怎么改成求每层最大值/最小值？
//     把 sum += val 改成 maxVal = max(maxVal, val) 即可。
//     就是 LC 515 的做法。
//
// Q3: 如果节点值可能是浮点数呢？
//     sum 的精度问题更突出。可以用 Kahan 求和算法减少浮点误差。
//
// ============================================================
// 【相关题型】
//
// - 102. 二叉树的层序遍历:
//   基础版，返回每层节点列表。本题模板的直接来源。
//
// - 515. 在每个树行中找最大值:
//   把"平均值"改成"最大值"，模板几乎一样。
//
// - 199. 二叉树的右视图:
//   每层只取最后一个节点 (i == levelSize - 1)。
//
// - 103. 二叉树的锯齿形层序遍历:
//   BFS 分层 + 奇偶层翻转。
//
// - 116. 填充每个节点的下一个右侧节点指针:
//   BFS 分层，同层节点用 next 指针串联。
// ============================================================
