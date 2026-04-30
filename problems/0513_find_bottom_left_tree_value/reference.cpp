/*
 * LeetCode 513: 找树左下角的值 (Find Bottom Left Tree Value)
 *
 * 【题目本质】
 * 找二叉树最深一层的最左边节点的值。
 * 拆成两个维度：(1) 最深层 → 需要知道深度 (2) 最左边 → 需要控制访问顺序
 *
 * 【解法总览】
 * 解法1: BFS 层序遍历（从左到右） — O(n) / O(n) — 最直觉，面试首选
 * 解法2: BFS 从右到左遍历        — O(n) / O(n) — 技巧性强，代码最简洁
 * 解法3: DFS 先序遍历            — O(n) / O(h) — 空间更优，面试追问DFS时用
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

// ============================================================
// 解法1: BFS 层序遍历（从左到右）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// "最底层最左边" → 天然联想到 BFS 层序遍历。
// BFS 逐层处理，每层从左到右。我们只需记录每层第一个节点的值。
// 遍历完所有层后，最后一次记录的就是最底层最左边的值。
//
// 示例2:
//          1
//         / \
//        2   3
//       /   / \
//      4   5   6
//         /
//        7
//
//  BFS 逐层:
//    Level 0: [1]       → first = 1
//    Level 1: [2, 3]    → first = 2
//    Level 2: [4, 5, 6] → first = 4
//    Level 3: [7]       → first = 7  ← 最底层，答案!
// ============================================================
class Solution1 {
public:
    int findBottomLeftValue(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);
        int result = root->val;
        
        while (!q.empty()) {
            // 必须先存 size，因为循环中 q.size() 会变
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 每层第一个节点就是该层最左边的
                if (i == 0) {
                    result = node->val;
                }
                
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        // 循环结束时 result 保存的是最后一层的第一个节点值
        return result;
    }
};

// ============================================================
// 解法2: BFS 从右到左遍历
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1需要分层 + 判断 i==0，代码略有冗余。
// 换个角度：如果入队时先右后左，那么：
//   - 同一层中，右边的先出队，左边的后出队
//   - 更深层的比浅层后出队
//   所以整棵树最后一个出队的节点 = 最深层的最左边节点！
//
// 不需要分层逻辑，代码更简洁。
//
// 示例2 入队/出队过程:
//   push(1) → [1]
//   pop 1, push 3,2 → [3,2]
//   pop 3, push 6,5 → [2,6,5]
//   pop 2, push 4   → [6,5,4]
//   pop 6            → [5,4]
//   pop 5, push 7   → [4,7]
//   pop 4            → [7]
//   pop 7            → []   ← 最后出队的是7，答案!
// ============================================================
class Solution2 {
public:
    int findBottomLeftValue(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);
        TreeNode* node = nullptr;
        
        while (!q.empty()) {
            node = q.front();
            q.pop();
            
            // 关键：先右后左入队
            // 同层中左节点后入队 → 后出队
            // 最终最后出队的就是最底层最左边
            if (node->right) q.push(node->right);
            if (node->left)  q.push(node->left);
        }
        
        // node 是最后一个出队的，即最底层最左边
        return node->val;
    }
};

// ============================================================
// 解法3: DFS 先序遍历
// 时间: O(n)  空间: O(h)，h为树高
//
// 【思路】
// 不想用 BFS？DFS 也行。
// 关键观察：如果先递归左子树再递归右子树（先序/中序/后序均可，
// 重要的是左在右前面），那么同一层中最左边的节点一定最先被访问。
//
// 所以维护 maxDepth，当 depth > maxDepth 时更新答案。
// ">"保证只在首次到达更深层时更新 → 一定是该层最左边的节点。
//
// 示例2:
//          1 (d=0)
//         / \
//        2   3 (d=1)
//       /   / \
//      4   5   6 (d=2)
//         /
//        7 (d=3)
//
//  DFS 遍历: 1→2→4→3→5→7→6
//    节点1: d=0 > max=-1 → result=1, max=0
//    节点2: d=1 > max=0  → result=2, max=1
//    节点4: d=2 > max=1  → result=4, max=2
//    节点3: d=1 ≤ max=2  → 跳过
//    节点5: d=2 ≤ max=2  → 跳过
//    节点7: d=3 > max=2  → result=7, max=3  ✓
//    节点6: d=2 ≤ max=3  → 跳过
//    答案 = 7
//
// 空间优势：递归栈深度 O(h)，平衡树 O(log n)，优于 BFS 的 O(n)。
// ============================================================
class Solution3 {
public:
    int maxDepth = -1;
    int result = 0;
    
    int findBottomLeftValue(TreeNode* root) {
        dfs(root, 0);
        return result;
    }
    
    void dfs(TreeNode* node, int depth) {
        if (!node) return;
        
        // 首次到达更深层 → 该层最左边的节点（因为先递归左子树）
        // 注意是 > 不是 >=，>= 会让同层右边的节点覆盖左边的
        if (depth > maxDepth) {
            maxDepth = depth;
            result = node->val;
        }
        
        // 先左后右：保证同深度中左边先被访问
        dfs(node->left, depth + 1);
        dfs(node->right, depth + 1);
    }
};

// ============================================================
// 解法3变体: DFS 不用全局变量（用引用参数）
// 有些面试官不喜欢全局变量，改用引用参数传递
// ============================================================
class Solution3b {
public:
    int findBottomLeftValue(TreeNode* root) {
        int result = root->val;
        int maxDepth = -1;
        dfs(root, 0, maxDepth, result);
        return result;
    }
    
    void dfs(TreeNode* node, int depth, int& maxDepth, int& result) {
        if (!node) return;
        
        if (depth > maxDepth) {
            maxDepth = depth;
            result = node->val;
        }
        
        dfs(node->left, depth + 1, maxDepth, result);
        dfs(node->right, depth + 1, maxDepth, result);
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | BFS从左到右    | BFS从右到左    | DFS             |
// |-----------|---------------|---------------|-----------------|
// | 时间       | O(n)          | O(n)          | O(n)            |
// | 空间       | O(n)          | O(n)          | O(h), h=树高    |
// | 代码复杂度  | 中（需分层）    | 最简洁         | 中（递归+状态）  |
// | 直觉性     | 最直觉         | 需理解逆序技巧  | 需理解先左后右    |
// | 面试推荐   | ⭐首选          | 加分项         | ⭐追问DFS时用    |
//
// BFS 从左到右最好理解、最好解释，面试首选。
// DFS 在树退化时空间更优（平衡树 O(log n) vs BFS 的 O(n)），
// 但最坏（链状树）也是 O(n)。
//
// ============================================================
// 【易错点】
//
// 1. DFS 条件用 >= 而非 >
//    ✗ if (depth >= maxDepth) → 同层右边的节点会覆盖左边的
//    ✓ if (depth > maxDepth)  → 只在首次到达更深层时更新
//
// 2. DFS 递归顺序搞反
//    ✗ dfs(right, ...); dfs(left, ...); → 同深度中右边先访问，得到最右边
//    ✓ dfs(left, ...); dfs(right, ...); → 左边先访问，首次更深一定最左
//
// 3. BFS 分层循环中用 q.size() 做条件
//    ✗ for (int i = 0; i < q.size(); i++) → q.size() 每次循环都在变
//    ✓ int size = q.size(); for (int i = 0; i < size; i++)
//
// 4. BFS 从右到左解法中先push左后push右
//    ✗ push(left); push(right); → 变成从左到右，最后出队的是最底层最右边
//    ✓ push(right); push(left); → 最后出队的才是最底层最左边
//
// ============================================================
// 【面试追问】
//
// Q1: 最直觉的做法？为什么 BFS 适合这道题？
// → BFS 天然按层遍历，"最底层最左边"直接对应"最后一层第一个节点"。
//    时间 O(n)，空间 O(n)（队列最大存一层宽度）。
//
// Q2: 能不能用 DFS？空间能不能比 BFS 更优？
// → 可以。DFS 先左后右递归，带深度参数，首次到达更深层时更新答案。
//    空间 O(h)，平衡树时 O(log n) < O(n)。
//
// Q3: 如果改成找最底层最右边呢？代码改哪里？
// → BFS: 记录每层最后一个(i==size-1)，或从左到右入队取最后出队的。
//    DFS: 先递归右子树，或保持先左后右但条件改为 >=。
//
// Q4: 如果要返回最底层所有节点（不只是最左边）怎么办？
// → BFS: 记录最后一层的完整列表。
//    DFS: 记录 maxDepth，收集所有 depth==maxDepth 的节点。
//
// ============================================================
// 【相关题型】
//
// - 199. 二叉树的右视图
//   复用 BFS 层序框架，区别：199记录每层最后一个，513记录每层第一个。
//   或用 DFS 先右后左的顺序。
//
// - 102. 二叉树的层序遍历
//   BFS 分层模板的基础版，513 是在此基础上只提取特定信息。
//
// - 104. 二叉树的最大深度
//   复用 DFS 带深度递归框架，区别：104只求深度值，513在到达最大深度时记录节点值。
//
// - 111. 二叉树的最小深度
//   复用 BFS 逐层框架，首次遇到叶子即返回。与513方向相反（最浅 vs 最深）。
// ============================================================
