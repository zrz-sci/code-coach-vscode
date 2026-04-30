/*
 * LeetCode 987: 二叉树的垂序遍历 (Vertical Order Traversal of a Binary Tree)
 * 
 * 【题目本质】
 * 给树的每个节点分配 (row, col) 坐标，然后按 (col, row, val) 三关键字排序后按列分组输出。
 * 
 * 【解法总览】
 * 解法1: DFS + 全局排序        — O(n log n) / O(n) — 最简洁，面试首选
 * 解法2: BFS + TreeMap 分组排序 — O(n log n) / O(n) — BFS 视角
 * 解法3: DFS + 嵌套有序容器     — O(n log n) / O(n) — 插入即排序
 */

// ============================================================
// 树的坐标分配示意图：
//
// 示例1: root = [3,9,20,null,null,15,7]
//
//          3 (0,0)
//         / \
//    9(1,-1)  20(1,1)
//             / \
//        15(2,0) 7(2,2)
//
// 按列分组:
//   col=-1: [9]
//   col= 0: [3, 15]     ← row 0 在前, row 2 在后
//   col= 1: [20]
//   col= 2: [7]
// 输出: [[9],[3,15],[20],[7]]
//
// 示例2: root = [1,2,3,4,5,6,7]
//
//             1 (0,0)
//            /       \
//       2(1,-1)     3(1,1)
//       /    \      /    \
//  4(2,-2) 5(2,0) 6(2,0) 7(2,2)
//
// col=0 中: 节点1(row=0), 节点5(row=2,val=5), 节点6(row=2,val=6)
// 5 和 6 同行同列 → 按值排序 → 5 在 6 前
// 输出: [[4],[2],[1,5,6],[3],[7]]
// ============================================================

// ============================================================
// 解法1: DFS + 全局排序 — 收集三元组，一次 sort 搞定
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 第一步：DFS 遍历树，给每个节点打上 (col, row) 标签，
//         收集 (col, row, val) 三元组到一个数组。
// 第二步：对数组按 (col, row, val) 排序。
//         tuple 的默认比较就是字典序，完美匹配题目的排序规则：
//         先按列从左到右，再按行从上到下，最后按值从小到大。
// 第三步：线性扫描排序后的数组，col 相同的归到同一组。
//
// 为什么这是最简洁的做法？
// 因为三关键字排序可以用一次 sort 解决，不需要嵌套数据结构。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        vector<tuple<int,int,int>> nodes; // (col, row, val)
        
        // DFS 收集所有节点的坐标和值
        function<void(TreeNode*, int, int)> dfs = [&](TreeNode* node, int row, int col) {
            if (!node) return;
            nodes.push_back({col, row, node->val});
            dfs(node->left,  row + 1, col - 1); // 左孩子: row+1, col-1
            dfs(node->right, row + 1, col + 1); // 右孩子: row+1, col+1
        };
        dfs(root, 0, 0);
        
        // tuple 默认字典序排序: 先 col，再 row，最后 val
        sort(nodes.begin(), nodes.end());
        
        // 按 col 分组输出
        vector<vector<int>> result;
        int prevCol = INT_MIN;
        for (auto& [col, row, val] : nodes) {
            if (col != prevCol) {
                result.push_back({}); // 遇到新列，开新组
                prevCol = col;
            }
            result.back().push_back(val);
        }
        return result;
    }
};

// ============================================================
// 解法2: BFS + TreeMap 分组排序 — 层序遍历视角
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 用 BFS 层序遍历，队列中存 {节点, row, col}。
// 用 map<int, vector<pair<int,int>>> 按列 (col) 分组，
// 每列存储该列所有节点的 (row, val)。
// map 按 key (col) 自动升序排列 → 列从左到右。
// 遍历完后，对每列内部按 (row, val) 排序再提取 val。
//
// BFS vs DFS 的区别：
// BFS 天然按行从上到下访问，但同行内的顺序不保证满足题目要求
//（同行同列要按值排序），所以仍然需要组内排序。
// ============================================================
class Solution2 {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        // key = col（自动排序），value = 该列的 (row, val) 列表
        map<int, vector<pair<int,int>>> colMap;
        
        // BFS: 队列存 {节点指针, row, col}
        queue<tuple<TreeNode*, int, int>> q;
        q.push({root, 0, 0});
        
        while (!q.empty()) {
            auto [node, row, col] = q.front();
            q.pop();
            colMap[col].push_back({row, node->val});
            if (node->left)  q.push({node->left,  row + 1, col - 1});
            if (node->right) q.push({node->right, row + 1, col + 1});
        }
        
        // 按列输出
        vector<vector<int>> result;
        for (auto& [col, vec] : colMap) {
            // 组内按 (row, val) 排序 — pair 默认字典序
            sort(vec.begin(), vec.end());
            vector<int> column;
            for (auto& [row, val] : vec) {
                column.push_back(val);
            }
            result.push_back(column);
        }
        return result;
    }
};

// ============================================================
// 解法3: DFS + 嵌套有序容器 — 插入时即排序
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 用三层嵌套有序容器: map<col, map<row, multiset<val>>>
// - 外层 map: col 自动升序 → 列从左到右
// - 中层 map: row 自动升序 → 同列内行从上到下
// - 内层 multiset: val 自动升序 → 同行同列按值排序
//
// 这样 DFS 插入时就已经排好序了，最后直接三层遍历输出。
//
// 为什么用 multiset 不是 set？
// 因为同行同列可能有两个值相同的节点，set 会去重丢失节点。
//
// 优点: 逻辑清晰，排序规则体现在数据结构选择上
// 缺点: 三层嵌套可读性稍差，常数较大
// ============================================================
class Solution3 {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        // col → row → 所有 val（multiset 排序且不去重）
        map<int, map<int, multiset<int>>> grid;
        
        function<void(TreeNode*, int, int)> dfs = [&](TreeNode* node, int row, int col) {
            if (!node) return;
            grid[col][row].insert(node->val); // 插入即排序
            dfs(node->left,  row + 1, col - 1);
            dfs(node->right, row + 1, col + 1);
        };
        dfs(root, 0, 0);
        
        // 三层遍历，每层都已有序
        vector<vector<int>> result;
        for (auto& [col, rowMap] : grid) {
            vector<int> column;
            for (auto& [row, vals] : rowMap) {
                for (int v : vals) {
                    column.push_back(v);
                }
            }
            result.push_back(column);
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: DFS+全局排序   | 解法2: BFS+TreeMap     | 解法3: DFS+嵌套容器   |
// |-------------|----------------------|------------------------|----------------------|
// | 核心思路     | 收集三元组, 一次sort  | BFS + map 分组 + sort  | 三层有序容器插入即排序 |
// | 代码量       | 最少 (~20行)         | 中等 (~25行)           | 中等 (~25行)         |
// | 常数开销     | 最小(一个vector)     | 中等(map+queue)        | 较大(嵌套map+multiset)|
// | 面试推荐     | ⭐ 首选              | 备选                   | 展示STL功底           |
// | 适用场景     | 通用                 | 需要逐层处理时自然     | 不想手动排序时         |
//
// 面试建议: 首选解法1，代码最简洁，排序逻辑最直观。
// 如果面试官问"不用全局排序能不能做"，给出解法3。
//
// 【易错点】
//
// 1. ✗ tuple 顺序写成 (row, col, val)
//    ✓ 必须是 (col, row, val)
//    原因: 第一分组关键字是列（从左到右），不是行。
//    如果写反，同一行的不同列节点会被错误地排在一起。
//
// 2. ✗ 在解法3中用 set 而不是 multiset
//    ✓ 必须用 multiset
//    原因: 同 (row, col) 位置可能有两个值相同的节点，
//    set 会去重导致丢失节点。
//    例: 如果 (2,0) 位置有两个值为5的节点，set 只保留一个。
//
// 3. ✗ 和 LeetCode 314 混淆排序规则
//    ✓ 987: 同行同列按 val 从小到大
//       314: 同行同列按 BFS 从左到右的遍历顺序
//    如果照搬 314 代码，示例2和3会出错（5和6的顺序可能不对）。
//
// 4. ✗ 用数组下标存 col，但 col 可能为负
//    ✓ 用 map 或者收集后排序，不要预分配固定大小数组
//    例: 如果树完全左偏，col 最小可到 -999。
//
// 5. ✗ 分组时没处理好第一列
//    ✓ prevCol 初始化为 INT_MIN（不能是0，因为第一列的col可能是负数）
//    如果初始化为0，当第一个节点的col恰好是0时不会新建组。
//
// 【面试追问 — 递进链】
//
// Q1 (基础理解):
//   "这道题的排序规则是什么？能否口头描述清楚？"
//   → 三关键字: col 升序 → row 升序 → val 升序。
//     先按列从左到右分组，同列按行从上到下，同行同列按值从小到大。
//
// Q2 (对比):
//   "和 314 题有什么区别？代码需要改哪里？"
//   → 314 同行同列按遍历顺序，987 按值排序。
//     如果用全局排序法，314 需要把三元组改成 (col, row, 遍历序号)
//     而不是 (col, row, val)，即第三关键字不同。
//
// Q3 (优化):
//   "如果树有 10^6 个节点，你的做法瓶颈在哪？能优化吗？"
//   → 瓶颈在 O(n log n) 排序，这是下界（因为需要值排序）。
//     可以优化常数: 用解法1（单 vector + sort）比解法3（嵌套 map）快。
//     如果值域有限（val ≤ 1000），同行同列的组内可以用计数排序。
//
// Q4 (变体):
//   "如果要求从下到上、从右到左遍历呢？"
//   → 把三元组改成 (-col, -row, val) 排序，
//     或者最后 reverse 每一列 + reverse 整体结果。
// ============================================================
