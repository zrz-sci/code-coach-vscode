// 【题目本质】
// 求 N 叉树从根到最远叶节点的路径上的节点数。
// 核心：二叉树 maxDepth 的推广，遍历 children 数组取 max。

// 【解法总览】
// Solution1: DFS 递归 — O(n) 时间，O(h) 空间 ⭐
// Solution2: BFS 层序遍历 — O(n) 时间，O(w) 空间

// ===================== Solution1: DFS 递归 =====================
// 思路：depth(root) = 1 + max(depth(child) for all children)
class Solution1 {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        int depth = 0;
        for (Node* child : root->children) {
            depth = max(depth, maxDepth(child));
        }
        return depth + 1;
    }
};

// ===================== Solution2: BFS 层序遍历 =====================
// 思路：逐层遍历，层数即为最大深度
class Solution2 {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        queue<Node*> q;
        q.push(root);
        int depth = 0;
        while (!q.empty()) {
            depth++;
            int size = q.size();
            for (int i = 0; i < size; i++) {
                Node* node = q.front(); q.pop();
                for (Node* child : node->children) {
                    q.push(child);
                }
            }
        }
        return depth;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 优势 |
// |------|------|------|------|
// | Solution1 DFS | O(n) | O(h) | 代码最简洁 |
// | Solution2 BFS | O(n) | O(w) | 无递归栈风险 |
// h = 树高，w = 最宽层的节点数

// 【易错点】
// 1. root 为空时必须返回 0
// 2. 叶节点 children 为空，depth 初始化 0，返回 0+1=1 ✓
// 3. BFS 必须用 size 变量记录当前层节点数，不能直接用 q.size()

// 【面试追问】
// Q1: 最小深度？→ 叶节点返回1，非叶取 min(children)+1
// Q2: 栈溢出？→ 深度上限 1000 一般安全，不放心用 BFS
// Q3: 二叉树版本区别？→ 只需改 max(left,right) 为 for(child) max(...)
