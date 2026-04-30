/*
 * LeetCode 429: N-ary Tree Level Order Traversal (N 叉树的层序遍历)
 *
 * 【题目本质】
 * 给定一棵 N 叉树，返回其节点值的层序遍历（逐层从左到右）。
 * 和二叉树 BFS (LC102) 几乎相同，区别只在子节点从 left/right 变成 children 数组。
 *
 * 【解法总览】
 * 解法1: BFS 迭代  — O(n) / O(n)  — 队列逐层处理 ⭐面试首选
 * 解法2: DFS 递归  — O(n) / O(h)  — 前序遍历传层号
 */

/*
// N 叉树节点定义
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}
    Node(int _val) { val = _val; }
    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

// ============================================================
// 解法1: BFS 迭代 — 队列逐层处理 ⭐面试首选
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 标准 BFS 层序遍历模板:
//   1. 根节点入队
//   2. while 队列非空:
//      a. 记录当前层大小 size
//      b. 弹出 size 个节点，收集它们的值
//      c. 将这些节点的所有子节点入队
//   3. 每完成一层，把该层结果加入 result
//
// 和二叉树 BFS 的唯一区别:
//   二叉树: if(node->left) q.push(node->left);
//           if(node->right) q.push(node->right);
//   N叉树:  for(Node* child : node->children)
//               if(child) q.push(child);
//
// BFS 执行过程 (示例1: [1,null,3,2,4,null,5,6]):
//
//   树结构:
//           1
//         / | \
//        3  2   4
//       / \
//      5   6
//
//   初始: queue = [1]
//
//   第0层: size=1
//     弹出1, level=[1], 入队children: [3,2,4]
//     → result = [[1]], queue = [3,2,4]
//
//   第1层: size=3
//     弹出3, level=[3], 入队children: [5,6]
//     弹出2, level=[3,2], 入队children: []
//     弹出4, level=[3,2,4], 入队children: []
//     → result = [[1],[3,2,4]], queue = [5,6]
//
//   第2层: size=2
//     弹出5, level=[5], 入队children: []
//     弹出6, level=[5,6], 入队children: []
//     → result = [[1],[3,2,4],[5,6]], queue = []
//
//   队列空，返回 [[1],[3,2,4],[5,6]]
// ============================================================
class Solution1 {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> res;
        if (!root) return res;  // 边界: 空树直接返回

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            // 关键: 在内层循环前记录当前层大小
            // 不能写 i < q.size()，因为循环体内会修改队列大小
            int size = q.size();
            vector<int> level;

            for (int i = 0; i < size; i++) {
                Node* node = q.front();
                q.pop();
                level.push_back(node->val);

                // N叉树: 遍历 children 数组（替代二叉树的 left/right）
                for (Node* child : node->children) {
                    if (child) q.push(child);
                }
            }

            res.push_back(level);
        }

        return res;
    }
};

// ============================================================
// 解法2: DFS 递归 — 前序遍历传层号
// 时间: O(n)  空间: O(h) h=树高度
//
// 【思路】
// 用 DFS 做层序遍历的核心: 传入当前节点的深度 depth。
// 将节点值放入 result[depth] 中。
// 由于 DFS 是前序遍历（先访问自己，再递归子节点），
// 同一层的节点会按从左到右的顺序被访问，保证了层序正确性。
//
// 关键点: 当 depth == res.size() 时，说明这一层还没创建，
// 需要先 push_back 一个空 vector。
//
// DFS 执行过程 (示例1):
//
//   dfs(1, depth=0):
//     res.size()==0==depth → res.push_back({})
//     res[0].push_back(1) → res = [[1]]
//     dfs(3, depth=1):
//       res.size()==1==depth → res.push_back({})
//       res[1].push_back(3) → res = [[1],[3]]
//       dfs(5, depth=2):
//         res.size()==2==depth → res.push_back({})
//         res[2].push_back(5) → res = [[1],[3],[5]]
//       dfs(6, depth=2):
//         res[2].push_back(6) → res = [[1],[3],[5,6]]
//     dfs(2, depth=1):
//       res[1].push_back(2) → res = [[1],[3,2],[5,6]]
//     dfs(4, depth=1):
//       res[1].push_back(4) → res = [[1],[3,2,4],[5,6]]
//
//   结果: [[1],[3,2,4],[5,6]] ✓
// ============================================================
class Solution2 {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> res;
        dfs(root, 0, res);
        return res;
    }

    void dfs(Node* node, int depth, vector<vector<int>>& res) {
        if (!node) return;

        // 当前层还没有对应的 vector，先创建
        // 这保证了层按顺序创建（depth 0, 1, 2, ...）
        if ((int)res.size() == depth) {
            res.push_back({});
        }

        // 当前节点的值放入对应层
        res[depth].push_back(node->val);

        // 递归处理所有子节点，深度 +1
        for (Node* child : node->children) {
            dfs(child, depth + 1, res);
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间  | 空间  | 代码复杂度 | 面试推荐 |
// |-----------|-------|-------|-----------|---------|
// | BFS 迭代  | O(n)  | O(w)  | 简单       | ⭐⭐⭐  |
// | DFS 递归  | O(n)  | O(h)  | 简单       | ✓ 备选  |
//
// w = 最宽层宽度, h = 树高度
// 对于平衡 N 叉树: w 可能很大 (N^(h-1)), h = O(logN(n))
// 对于退化链表: w = 1, h = n
//
// 面试策略:
//   1. 层序遍历首选 BFS — 直觉清晰，代码模板化
//   2. 如果面试官追问递归做法，切换到 DFS
//   3. 强调和二叉树 BFS 的区别：children 数组替代 left/right
//
// ============================================================
// 【易错点】
//
// 1. 忘记处理空树:
//    ✗ 不检查 root == nullptr，直接 push 空指针
//    ✓ 开头 if(!root) return {};
//
// 2. BFS 中 size 在循环前取值:
//    ✗ for(int i = 0; i < q.size(); i++) — 循环中 q.size() 会变
//    ✓ int size = q.size(); for(int i = 0; i < size; i++)
//
// 3. DFS 忘记扩展 result:
//    ✗ 直接 res[depth].push_back(val) — 如果 depth 越界会崩溃
//    ✓ 先检查 if(res.size() == depth) res.push_back({})
//
// 4. children 中可能有 nullptr:
//    ✗ 不检查直接 push(child) — 可能入队空指针
//    ✓ if(child) q.push(child) 或 if(child) dfs(child, ...)
// ============================================================

// ============================================================
// 【总结】
//
// ——— 解法对比 ———
// | 解法       | 时间  | 空间      | 适用场景              |
// |-----------|-------|----------|----------------------|
// | BFS 迭代  | O(n)  | O(w)     | 面试首选，层序模板 ⭐   |
// | DFS 递归  | O(n)  | O(h)     | 备选，展示递归思维      |
//
// 本题是二叉树 BFS (LC102) 的直接扩展：left/right → children[]。
// 掌握 BFS 模板后，N 叉树、图的 BFS 都是同一套框架。
//
// ——— 易错点 ———
// 1. 空树不判 → 对 nullptr 调用方法直接崩溃
// 2. BFS 内层循环用 q.size() → 循环体改变队列大小，层边界错乱
// 3. DFS 不扩展 result → depth 越界访问 vector 崩溃
// 4. children 中可能有 nullptr → 入队前必须检查
//
// ——— 面试追问 ———
// Q1: 如果要求逆序层序遍历（从底层到顶层）？
//     → 正常 BFS 收集结果后 reverse(res.begin(), res.end())。
//       或者用栈替代直接 push_back，最后依次弹出。
//
// Q2: 如果要求锯齿形层序（奇数层正序、偶数层逆序）？
//     → BFS 模板不变，用一个 bool 标记当前层方向。
//       偶数层收集完后 reverse 该层，或用 deque 双端插入。
//
// Q3: 如果树的节点数极大（10^7+），BFS 的队列会不会爆内存？
//     → 最坏情况队列大小 = 最宽层宽度 w。完全 N 叉树最底层
//       w = O(N^h)，可能很大。可以改用 DFS（栈深度只有 h）。
//       或者用"双数组交替"代替 queue 减少内存碎片。
// ============================================================
