/*
 * LeetCode 314: Binary Tree Vertical Order Traversal
 *
 * 【题目本质】
 * 给每个节点分配列号(根=0, 左-1, 右+1)，按列号从小到大收集节点值。
 * 同一列内：从上到下，同行同列：从左到右 —— 恰好是 BFS 的天然顺序。
 *
 * 【解法总览】
 * 解法1: DFS + 排序           — O(n log n) / O(n) — 最直觉但有坑
 * 解法2: BFS + TreeMap(map)   — O(n log n) / O(n) — 面试首选，简洁正确
 * 解法3: BFS + HashMap + minMax — O(n) / O(n)    — 最优，面试加分
 */


// ============================================================
// 解法1: DFS + 排序
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 最直觉的想法：DFS 遍历每个节点，记录它的 (列号, 行号, 值)，
// 然后按 (列号, 行号) 排序，分组输出。
//
// 问题：DFS 遍历顺序不保证同行同列的节点按"从左到右"排列。
// 需要用 stable_sort 来维持 DFS 中先左后右的相对顺序，
// 但严格来说这在复杂树结构中不一定总是正确。
//
// 示例树:
//         3(col=0,row=0)
//        / \
//     9(-1,1) 20(1,1)
//              / \
//          15(0,2) 7(2,2)
//
// DFS先序收集: (0,0,3), (-1,1,9), (1,1,20), (0,2,15), (2,2,7)
// 排序后: (-1,1,9), (0,0,3), (0,2,15), (1,1,20), (2,2,7)
// 分组: [9], [3,15], [20], [7] ✓
// ============================================================
class Solution1 {
public:
    vector<vector<int>> verticalOrder(TreeNode* root) {
        if (!root) return {};
        
        vector<tuple<int,int,int>> nodes; // (col, row, val)
        
        function<void(TreeNode*, int, int)> dfs = [&](TreeNode* node, int col, int row) {
            if (!node) return;
            nodes.push_back({col, row, node->val});
            dfs(node->left, col - 1, row + 1);
            dfs(node->right, col + 1, row + 1);
        };
        dfs(root, 0, 0);
        
        // stable_sort 维持同(col,row)下DFS的先左后右顺序
        stable_sort(nodes.begin(), nodes.end(), [](auto& a, auto& b) {
            if (get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
            return get<1>(a) < get<1>(b);
        });
        
        vector<vector<int>> result;
        int prevCol = INT_MIN;
        for (auto& [col, row, val] : nodes) {
            if (col != prevCol) {
                result.push_back({});
                prevCol = col;
            }
            result.back().push_back(val);
        }
        return result;
    }
};

// ============================================================
// 解法2: BFS + TreeMap — 面试首选 ⭐
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 核心洞察：题目要求同列内"从上到下、同行从左到右"的顺序，
// 这恰好就是 BFS（层序遍历）的天然遍历顺序！
// 
// BFS 逐层处理，同层中先处理左子树再处理右子树。
// 用 map<int, vector<int>> 按列号自动排序，BFS 中直接 push_back。
//
// BFS 队列变化过程（示例2）:
//         3(col=0)
//        / \
//     9(-1) 8(+1)
//    / \   / \
//  4(-2) 0(0) 1(0) 7(+2)
//
// 队列: (3,0)
//   弹出(3,0) → map[0]=[3], 入队(9,-1),(8,1)
// 队列: (9,-1),(8,1)
//   弹出(9,-1) → map[-1]=[9], 入队(4,-2),(0,0)
//   弹出(8,1)  → map[1]=[8], 入队(1,0),(7,2)
// 队列: (4,-2),(0,0),(1,0),(7,2)
//   弹出(4,-2) → map[-2]=[4]
//   弹出(0,0)  → map[0]=[3,0]   ← 0排在1前面 ✓
//   弹出(1,0)  → map[0]=[3,0,1] ← BFS保证了左到右
//   弹出(7,2)  → map[2]=[7]
//
// 最终: {-2:[4], -1:[9], 0:[3,0,1], 1:[8], 2:[7]}
// ============================================================
class Solution2 {
public:
    vector<vector<int>> verticalOrder(TreeNode* root) {
        if (!root) return {};
        
        // map 自动按列号排序（红黑树，O(log n) 插入）
        map<int, vector<int>> colMap;
        
        // BFS: 队列中存 (节点指针, 列号)
        queue<pair<TreeNode*, int>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            auto [node, col] = q.front();
            q.pop();
            
            // BFS 顺序 push_back，天然保证行序和左右序
            colMap[col].push_back(node->val);
            
            if (node->left)  q.push({node->left, col - 1});
            if (node->right) q.push({node->right, col + 1});
        }
        
        // map 迭代器按 key 有序，直接提取
        vector<vector<int>> result;
        for (auto& [col, vals] : colMap) {
            result.push_back(vals);
        }
        return result;
    }
};

// ============================================================
// 解法3: BFS + HashMap + minCol/maxCol — 最优 O(n)
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法2中 map 的 O(log n) 插入是不必要的开销。
// 用 unordered_map（O(1) 插入）替代，同时维护 minCol 和 maxCol，
// 最后从 minCol 到 maxCol 线性遍历输出，保证列号有序。
//
// 为什么这样做是对的？
// - 列号是连续的（从最左到最右不会跳过某一列，因为每次只 ±1）
// - 所以从 minCol 到 maxCol 遍历一定能覆盖所有列
// ============================================================
class Solution3 {
public:
    vector<vector<int>> verticalOrder(TreeNode* root) {
        if (!root) return {};
        
        unordered_map<int, vector<int>> colMap;
        int minCol = 0, maxCol = 0; // 追踪列号范围
        
        queue<pair<TreeNode*, int>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            auto [node, col] = q.front();
            q.pop();
            
            colMap[col].push_back(node->val);
            // 动态更新列号边界
            minCol = min(minCol, col);
            maxCol = max(maxCol, col);
            
            if (node->left)  q.push({node->left, col - 1});
            if (node->right) q.push({node->right, col + 1});
        }
        
        // 从最左列到最右列线性遍历，O(列数) = O(n)
        vector<vector<int>> result;
        for (int col = minCol; col <= maxCol; col++) {
            result.push_back(colMap[col]);
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 顺序保证 | 适用场景 |
// |------|------|------|----------|----------|
// | DFS+排序  | O(n log n) | O(n) | 需排序，有坑 | 不推荐 |
// | BFS+map   | O(n log n) | O(n) | BFS天然正确 | 面试首选 |
// | BFS+umap  | O(n)       | O(n) | BFS天然正确 | 追问优化 |
//
// 核心区别：
// - DFS vs BFS：BFS 天然满足"从上到下、同行从左到右"，DFS 不保证
// - map vs unordered_map+minMax：前者 O(log n) 插入但自动排序，
//   后者 O(1) 插入但需要手动遍历 [minCol, maxCol]
//
// 【易错点】
// 1. DFS 同行同列顺序错误：
//    ✗ DFS 中同 (col, row) 的节点按 DFS 序输出
//    ✓ DFS 无法完全保证"从左到右"，应优先用 BFS
//
// 2. 忘记空树检查：
//    ✗ 直接 q.push({root, 0})，root为null时解引用
//    ✓ 开头 if (!root) return {};
//
// 3. unordered_map 遍历顺序不确定：
//    ✗ for (auto& [col, vals] : colMap) result.push_back(vals);
//       → unordered_map 遍历顺序随机，列号不按序输出
//    ✓ for (int col = minCol; col <= maxCol; col++) result.push_back(colMap[col]);
//
// 4. 列号是否连续？
//    列号每次 ±1，从根节点扩展，中间不会跳过列号。
//    所以 [minCol, maxCol] 范围内每个列都一定有节点。
//    （注意：这个性质让解法3成立；如果列号可以跳跃就不能这样遍历）
//
// 【面试追问】
// Q1: 为什么选 BFS 而不是 DFS？
//     → BFS 逐层遍历天然满足"从上到下"和"同行从左到右"的顺序，
//       DFS 需要额外记录行号和排序，且同行同列的顺序可能出错。
//
// Q2: map 的 O(log n) 能优化吗？
//     → 用 unordered_map + minCol/maxCol，最后线性遍历 [min, max]，
//       总体 O(n)。（解法3）
//
// Q3: 如果同行同列要求按值排序呢？（LeetCode 987）
//     → BFS 的天然顺序不够了，需要记录 (col, row, val) 三元组，
//       按 (col, row, val) 排序。可以用 map<int, map<int, multiset<int>>>
//       或者收集后全局排序。复杂度 O(n log n)。
//       关键代码差异：987 需要在同 (col, row) 内 sort by val，
//       本题 314 不需要。
//
// Q4: 树极深（百万层）DFS会栈溢出，怎么办？
//     → BFS 天然用队列迭代，不受递归深度限制。
//       空间只与树的最大宽度成正比。
// ============================================================
