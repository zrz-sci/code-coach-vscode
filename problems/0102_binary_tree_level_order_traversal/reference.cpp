/*
 * LeetCode 102: 二叉树的层序遍历 (Binary Tree Level Order Traversal)
 * 
 * 【题目本质】
 * 对二叉树做 BFS，按层分组输出每层的节点值。
 * 
 * 【解法总览】
 * 解法1: BFS + 队列      — O(n) / O(n) — 层序遍历最自然的实现，面试首选
 * 解法2: DFS 递归(前序)   — O(n) / O(n) — 利用深度参数分层，展示 DFS 也能做 BFS 的效果
 */

// ============================================================
// 解法1: BFS + 队列 — 逐层处理，用 size 划分层界
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 层序遍历 = BFS 的直接应用。
// 核心技巧：在处理每一层前，先用 size = queue.size() 锁定当前层的节点数。
// 然后只弹出 size 个节点，它们就是当前层的全部节点。
// 这些节点的子节点入队后，自然构成下一层。
//
// 【示例演示】
//        3          ← 第0层
//       / \
//      9   20       ← 第1层
//         / \
//        15   7     ← 第2层
//
// BFS 队列变化:
//   初始: queue=[3]
//   ─── 第0层 ───  size=1
//     弹出3, 入队9和20 → queue=[9,20], level=[3]
//   ─── 第1层 ───  size=2
//     弹出9, 无子节点   → queue=[20]
//     弹出20, 入队15和7 → queue=[15,7], level=[9,20]
//   ─── 第2层 ───  size=2
//     弹出15, 无子节点  → queue=[7]
//     弹出7, 无子节点   → queue=[], level=[15,7]
//   队列空 → 结束
//   输出: [[3],[9,20],[15,7]]
// ============================================================
class Solution1 {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            // 关键：在循环开始前锁定当前层节点数
            // 不能写 i < q.size()，因为循环体中会往队列加元素
            int size = q.size();
            vector<int> level;
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                
                // 子节点入队，它们属于下一层
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(level);
        }
        
        return result;
    }
};

// ============================================================
// 解法2: DFS 递归(前序) — 带深度参数，按深度分层收集
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 换个角度：不用队列，用递归遍历整棵树。
// 只要知道每个节点在第几层（depth），就能把它放到 result[depth] 中。
// 前序遍历（根→左→右）保证同一层的节点按从左到右的顺序被访问。
//
// 为什么 depth == result.size() 就需要新建一层？
// 因为前序遍历时，每一层最先被访问的是最左边的节点。
// 当 depth 等于 result 的长度时，说明还没有任何节点被加入这一层，
// 所以需要创建一个新的空数组。
//
// 【递归过程演示】
//        3  (d=0)
//       / \
//      9   20  (d=1)
//         / \
//        15   7  (d=2)
//
// dfs(3, 0):  result.size()=0, 新建层 → result=[[3]]
// dfs(9, 1):  result.size()=1, 新建层 → result=[[3],[9]]
//   9无左右子节点，返回
// dfs(20,1):  result.size()=2, depth=1 < 2, 不新建 → result=[[3],[9,20]]
// dfs(15,2):  result.size()=2, 新建层 → result=[[3],[9,20],[15]]
// dfs(7, 2):  result.size()=3, depth=2 < 3, 不新建 → result=[[3],[9,20],[15,7]]
// ============================================================
class Solution2 {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<vector<int>>& result) {
        if (!node) return;
        
        // 第一次到达新的一层时，result 中还没有这一层的数组
        // depth 从0开始计数，恰好等于 result.size() 时说明需要新建
        if (depth == (int)result.size()) {
            result.push_back({});
        }
        
        // 将当前节点加入对应层
        result[depth].push_back(node->val);
        
        // 前序：先左后右，保证同层从左到右的顺序
        dfs(node->left, depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | BFS + 队列              | DFS 递归               |
// |------------|-------------------------|------------------------|
// | 核心思想   | 队列逐层处理,size划分层  | 前序遍历+depth参数分组  |
// | 时间       | O(n)                    | O(n)                   |
// | 空间       | O(n) 队列最宽层约n/2    | O(n) 递归栈=树高       |
// | 直觉性     | 层序遍历最自然的思路     | 需要"DFS也能分层"的洞察|
// | 面试推荐   | ⭐ 首选                 | 追问时展示              |
// | 空间偏好   | 宽树(完全二叉树)空间大   | 深树(链状树)空间大      |
//
// 面试建议：先写 BFS 解法（最直觉），如果被追问"能否用 DFS？"再写递归解法。
//
// ============================================================
// 【易错点】
//
// 1. size 变量时机错误:
//    ✗ for (int i = 0; i < q.size(); i++) — q.size() 随子节点入队而变化
//    ✓ int size = q.size(); for (int i = 0; i < size; i++) — 先锁定
//
// 2. 忘记 root 为空的判断:
//    ✗ 直接 q.push(root) → 空指针入队后 q.front() 解引用 crash
//    ✓ if (!root) return result; 开头判空
//
// 3. DFS 解法忘记创建新层数组:
//    ✗ result[depth].push_back(val) → 当 depth >= result.size() 时越界
//    ✓ if (depth == result.size()) result.push_back({});
//
// 4. DFS 用中序/后序导致顺序错乱:
//    ✗ 中序: 左→根→右 → 同层节点不是从左到右的顺序
//    ✓ 前序: 根→左→右 → 保证同层节点严格从左到右
//
// ============================================================
// 【面试追问】
//
// Q1: BFS 队列最大有多少节点？
// → 最宽的那一层。完全二叉树最后一层约 n/2 个节点，所以是 O(n)。
//
// Q2: 能否用 DFS 实现层序遍历？
// → 可以，前序 DFS + depth 参数分组（见解法2）。
//    关键：前序保证同层从左到右。
//
// Q3: 如果要自底向上的层序遍历 (LeetCode 107)？
// → BFS 解法不变，最后 reverse(result.begin(), result.end()) 即可。
//    只多一行代码。
//
// Q4: 如果要锯齿形层序遍历 (LeetCode 103)？
// → BFS 框架不变，加一个 bool leftToRight 标记。
//    奇数层 reverse 当前层数组，或者用 deque 从前端插入。
//    代码改动：加 if (!leftToRight) reverse(level.begin(), level.end());
//
// Q5: 如果只要每层的最后一个节点 (LeetCode 199 右视图)？
// → BFS 框架不变，每层循环中只在 i == size - 1 时收集节点。
//
// ============================================================
// 【相关题型】
//
// - 107. 二叉树的层序遍历 II — 复用本题 BFS，最后 reverse 结果
// - 103. 锯齿形层序遍历 — 复用本题 BFS，奇数层反转
// - 199. 二叉树的右视图 — 复用本题 BFS，每层只取最后一个
// - 637. 二叉树的层平均值 — 复用本题 BFS，每层求均值
// - 515. 在每个树行中找最大值 — 复用本题 BFS，每层取 max
// - 116/117. 填充右侧节点指针 — 复用本题 BFS，同层串联
// ============================================================
