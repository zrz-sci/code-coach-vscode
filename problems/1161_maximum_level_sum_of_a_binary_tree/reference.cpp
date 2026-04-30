/*
 * LeetCode 1161: 最大层内元素和 (Maximum Level Sum of a Binary Tree)
 *
 * 【题目本质】
 * 按层遍历二叉树，计算每层的节点值之和，返回和最大的那一层的层号（从1开始）。
 * 若有多层和相同，返回最小层号。
 *
 * 【解法总览】
 * 解法1: BFS 层序遍历 — O(n) / O(w) — 最直觉，面试首选
 * 解法2: DFS 递归      — O(n) / O(h) — DFS + 数组记录每层和
 *
 * 【示例树结构】
 *         1           ← 第1层: sum = 1
 *        / \
 *       7   0         ← 第2层: sum = 7+0 = 7  ★ 最大
 *      / \
 *     7  -8           ← 第3层: sum = 7+(-8) = -1
 *
 * BFS 队列变化过程:
 *   初始:   queue = [1]                  → 处理第1层 sum=1
 *   第1轮后: queue = [7, 0]             → 处理第2层 sum=7
 *   第2轮后: queue = [7, -8]            → 处理第3层 sum=-1
 *   第3轮后: queue = []                 → 结束
 *   maxSum=7 在第2层 → 返回 2
 */

// ============================================================
// 解法1: BFS 层序遍历 — 队列逐层处理，每层求和
// 时间: O(n)  空间: O(w)，w 为树的最大宽度
//
// 【思路】
// 题目要"每层的和"，BFS 天然按层处理：
//   - 每轮循环开始时 queue.size() 就是当前层的节点数
//   - 弹出所有当前层节点求和，同时把子节点入队（它们属于下一层）
//   - 每处理完一层就和 maxSum 比较
//
// 为什么 BFS 是首选？因为"层序"和"每层"完美对应，不需要额外转换。
// ============================================================
class Solution1 {
public:
    int maxLevelSum(TreeNode* root) {
        std::queue<TreeNode*> q;
        q.push(root);
        
        // 节点值可为负数，所以 maxSum 必须初始化为极小值
        // 如果初始化为 0，当所有层和都为负时永远不更新
        int maxSum = INT_MIN;
        int resultLevel = 1;
        int currentLevel = 0;
        
        while (!q.empty()) {
            currentLevel++;
            
            // 关键：必须在 for 循环前取 size
            // 因为循环体内会 push 新节点，改变 queue 大小
            int size = q.size();
            int levelSum = 0;
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                levelSum += node->val;
                
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            // 用 > 而不是 >=，保证相同 sum 时保留较小层号
            if (levelSum > maxSum) {
                maxSum = levelSum;
                resultLevel = currentLevel;
            }
        }
        
        return resultLevel;
    }
};

// ============================================================
// 解法2: DFS 递归 — 递归带深度参数，数组记录每层和
// 时间: O(n)  空间: O(h)，h 为树高（递归栈 + 层和数组）
//
// 【思路】
// 不用 BFS 也能统计每层的和：
//   - DFS 递归时把当前深度 depth 传下去
//   - 用 vector<long long> levelSums 按下标存每层的累计和
//   - levelSums[0] = 第1层的和, levelSums[1] = 第2层的和, ...
//   - 遍历完后，在 levelSums 中找最大值的下标
//
// DFS 的遍历顺序（前序/中序/后序）不影响结果，
// 因为我们只是累加每层的值，不关心同层内的访问顺序。
//
// 【DFS 递归过程示意】
//         1  (depth=0)
//        / \
//       7   0  (depth=1)
//      / \
//     7  -8  (depth=2)
//
// dfs(1, 0): levelSums = [1]
// dfs(7, 1): levelSums = [1, 7]
// dfs(7, 2): levelSums = [1, 7, 7]
// dfs(-8,2): levelSums = [1, 7, -1]    ← 7+(-8) = -1
// dfs(0, 1): levelSums = [1, 7, -1]    ← 7+0 = 7
//
// 最终: levelSums = [1, 7, -1]
// 最大值 7 在下标 1 → 层号 = 1+1 = 2
// ============================================================
class Solution2 {
public:
    std::vector<long long> levelSums;
    
    void dfs(TreeNode* node, int depth) {
        if (!node) return;
        
        // 第一次到达新深度时，扩展数组
        if (depth >= (int)levelSums.size()) {
            levelSums.push_back(0);
        }
        
        // 累加当前节点值到对应层
        levelSums[depth] += node->val;
        
        dfs(node->left, depth + 1);
        dfs(node->right, depth + 1);
    }
    
    int maxLevelSum(TreeNode* root) {
        levelSums.clear();
        dfs(root, 0);  // depth 从 0 开始，对应第 1 层
        
        int resultLevel = 1;
        long long maxSum = levelSums[0];
        
        for (int i = 1; i < (int)levelSums.size(); i++) {
            if (levelSums[i] > maxSum) {
                maxSum = levelSums[i];
                resultLevel = i + 1;  // 下标 i 对应层号 i+1
            }
        }
        
        return resultLevel;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | BFS 层序遍历        | DFS 递归             |
// |-----------|--------------------|--------------------|
// | 时间       | O(n)              | O(n)               |
// | 空间       | O(w) 最大层宽度     | O(h) 树高           |
// | 代码直觉   | 天然按层，最直接      | 需要额外数组          |
// | 完全二叉树  | w≈n/2 空间较多      | h=logn 空间更少      |
// | 链状树     | w=1 空间极少         | h=n 空间较多         |
// | 面试推荐   | ⭐ 首选             | 追问备选             |
//
// 选择建议：面试优先写 BFS，因为"层序遍历求每层和"完美匹配题意。
// DFS 作为追问时的替代方案展示多角度思维。
//
// ============================================================
// 【易错点】
//
// 1. maxSum 初始化为 0 而不是 INT_MIN
//    ✗ int maxSum = 0;
//      → 所有层和都为负时不会更新，返回错误的初始层号
//    ✓ int maxSum = INT_MIN;
//      → 确保第一层一定能触发更新
//
// 2. BFS 中 size 在循环内动态取值
//    ✗ for (int i = 0; i < q.size(); i++)
//      → 循环体内 push 新节点改变了 q.size()，导致当前层和下一层混在一起
//    ✓ int size = q.size(); for (int i = 0; i < size; i++)
//      → 固定当前层节点数
//
// 3. DFS 中层号 off-by-one
//    ✗ resultLevel = i;  // depth 从0开始，直接用下标作层号
//      → 题目层号从1开始，应该 +1
//    ✓ resultLevel = i + 1;
//
// 4. 用 >= 更新最大值导致返回较大层号
//    ✗ if (levelSum >= maxSum) → 相同 sum 的后面层会覆盖前面层
//    ✓ if (levelSum > maxSum)  → 严格大于才更新，保留最小层号
//
// ============================================================
// 【面试追问】
//
// Q1(基础): 这道题最直接的解法？时间复杂度？
//   → BFS 层序遍历，每个节点入队出队各一次，O(n)。
//
// Q2(多角度): 用 DFS 怎么做？和 BFS 相比空间差异？
//   → DFS 传深度 + 数组记录每层和。空间取决于树形状：
//     完全二叉树 DFS 更省(O(logn) vs O(n/2))，
//     链状树 BFS 更省(O(1) vs O(n))。
//
// Q3(变体): 如果要找第 K 大的层和的层号呢？
//   → BFS/DFS 收集所有层和到数组，然后排序或用堆找第K大。
//     额外 O(L log L) 或 O(L log K)，L 为层数。
//
// Q4(扩展): 如果树非常大无法全部放入内存？
//   → BFS 天然适合流式处理：内存中只需存当前层 + 下一层的节点。
//     可以逐层从外部存储读取。
//
// ============================================================
