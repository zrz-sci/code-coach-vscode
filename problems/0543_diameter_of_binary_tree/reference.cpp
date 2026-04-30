/*
 * LeetCode 543: 二叉树的直径 (Diameter of Binary Tree)
 *
 * 【题目本质】
 * 二叉树的直径 = 某个节点的「左子树深度 + 右子树深度」的最大值。
 * 本质是在一次深度计算中，顺手统计经过每个节点的最长路径。
 *
 * 【解法总览】
 * 解法1: 暴力法  — O(n²) / O(n) — 每个节点分别求深度
 * 解法2: 一次 DFS（递归） — O(n) / O(n) — ⭐ 面试首选
 * 解法3: 一次 DFS（迭代） — O(n) / O(n) — 后序遍历 + 栈
 *
 *  示例树:
 *       1
 *      / \
 *     2   3
 *    / \
 *   4   5
 *
 *  直径 = 3 (路径: 4→2→1→3 或 5→2→1→3)
 */

// ============================================================
// 解法1: 暴力法 — 每个节点单独求左右深度
// 时间: O(n²)  空间: O(n)
//
// 【思路】
// 最直觉的做法：遍历每个节点，对每个节点分别调用 depth()
// 求左右子树深度，加起来就是"经过该节点的最长路径"。
// 取所有节点中的最大值。
//
// 瓶颈：每个节点都单独调 depth()，子树被反复遍历。
// 链状树最坏 O(n²)。
// ============================================================
class Solution1 {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int result = 0;
        traverse(root, result);
        return result;
    }

private:
    void traverse(TreeNode* node, int& result) {
        if (!node) return;
        // 对当前节点求左右深度 — 这一步每次都要遍历子树
        int left = depth(node->left);
        int right = depth(node->right);
        result = max(result, left + right);
        // 直径可能不经过当前节点，所以继续遍历子树
        traverse(node->left, result);
        traverse(node->right, result);
    }

    // 求以 node 为根的子树深度（边数）
    int depth(TreeNode* node) {
        if (!node) return 0;
        return 1 + max(depth(node->left), depth(node->right));
    }
};

// ============================================================
// 解法2: 一次 DFS（递归）— 求深度的同时更新直径 ⭐ 面试首选
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：对每个节点单独求深度，子树被重复遍历。
// 关键洞察：depth() 递归时已经算出了左右子树深度，
// 何不在那一刻"顺手"更新直径？
//
// 递归函数返回「深度」给父节点用，
// 同时在函数内部更新「直径」这个全局答案。
// 「返回值 ≠ 答案」是这类题的核心模式。
//
//  递归过程（自底向上）:
//       1            depth(4)=0, ans=max(0,0+0)=0
//      / \           depth(5)=0, ans=max(0,0+0)=0
//     2   3          depth(2): L=1,R=1, ans=max(0,1+1)=2, return 2
//    / \             depth(3)=0, ans=max(2,0+0)=2
//   4   5            depth(1): L=2,R=1, ans=max(2,2+1)=3, return 3
//                    最终 ans=3
// ============================================================
class Solution2 {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int ans = 0;
        depth(root, ans);
        return ans;
    }

private:
    // 返回: 以 node 为根的子树深度（边数）
    // 副作用: 更新 ans 为目前见过的最大直径
    int depth(TreeNode* node, int& ans) {
        if (!node) return 0;

        int left = depth(node->left, ans);    // 左子树深度
        int right = depth(node->right, ans);  // 右子树深度

        // 经过当前节点的路径长度 = left + right
        // 用它更新全局答案（直径可能经过任何节点）
        ans = max(ans, left + right);

        // 返回深度给父节点：选较深的一侧 + 1 条边
        return 1 + max(left, right);
    }
};

// ============================================================
// 解法3: 一次 DFS（迭代 — 后序遍历）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 面试追问"不用递归怎么做？"
// 深度是自底向上算的，对应后序遍历。
// 用两个栈实现后序遍历：
//   栈1: 按 根→左→右 遍历
//   栈2: 逆序存储，弹出时就是 左→右→根（后序）
// 用 hashmap 记录每个节点的深度。
//
//  后序处理顺序: 4, 5, 2, 3, 1
//  处理4: depth[4] = 1+max(0,0) = 1, ans = max(0, 0+0) = 0
//     （注意：这里 depth 定义为"到最远叶子的边数+1？"
//      不，我们让 depthMap[null]=0, 
//      depth[叶子] = 1+max(0,0)=1 → 这代表有1条边到null？
//      需要小心定义一致性）
//
//  实际上为简洁，这里 depthMap 存的是"节点高度"
//  (叶子=1, null=0)，但 left+right 对应的是
//  leftChild.height + rightChild.height = 经过当前节点的边数
//  这和递归版是一致的。
// ============================================================
class Solution3 {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        if (!root) return 0;

        int ans = 0;
        unordered_map<TreeNode*, int> depthMap;
        depthMap[nullptr] = 0;  // null 节点深度为 0

        // 后序遍历：栈1遍历，栈2存后序序列
        stack<TreeNode*> stk1, stk2;
        stk1.push(root);
        while (!stk1.empty()) {
            TreeNode* node = stk1.top(); stk1.pop();
            stk2.push(node);
            // 先左后右入栈1，则栈2弹出时先右后左，
            // 最终栈2弹出顺序: 左→右→根（后序）
            if (node->left)  stk1.push(node->left);
            if (node->right) stk1.push(node->right);
        }

        // 按后序顺序处理，保证子节点先于父节点处理
        while (!stk2.empty()) {
            TreeNode* node = stk2.top(); stk2.pop();
            int left  = depthMap[node->left];   // 左子树深度
            int right = depthMap[node->right];  // 右子树深度
            // 经过当前节点的路径长度
            ans = max(ans, left + right);
            // 当前节点的深度
            depthMap[node] = 1 + max(left, right);
        }

        return ans;
    }
};

// ============================================================
// 补充: 不用全局变量的递归版本（返回 pair）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 面试追问"能否不用引用/全局变量？"
// 让递归函数返回 pair<depth, diameter>，
// 把两个信息都通过返回值传递。
// ============================================================
class Solution4 {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        return dfs(root).second;
    }

private:
    // 返回 {子树深度, 子树内最大直径}
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};

        auto [ld, ldia] = dfs(node->left);
        auto [rd, rdia] = dfs(node->right);

        int depth = 1 + max(ld, rd);
        // 直径取三者最大：左子树内的、右子树内的、经过当前节点的
        int diameter = max({ld + rd, ldia, rdia});

        return {depth, diameter};
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间   | 空间  | 特点                          |
// |-----------|--------|-------|-------------------------------|
// | 暴力       | O(n²) | O(n) | 思路直接，但有重复计算          |
// | 递归 DFS   | O(n)  | O(n) | ⭐ 最简洁，面试首选             |
// | 迭代 DFS   | O(n)  | O(n) | 后序遍历模拟，代码较长          |
// | pair 返回  | O(n)  | O(n) | 无全局变量，函数式风格          |
//
// 核心区别：暴力法把"求深度"和"求直径"分两步，
//          优化法合二为一，在一次 DFS 中同时完成。
//
// ============================================================
// 【易错点】
//
// 1. ✗ 只看根节点: return depth(root->left) + depth(root->right)
//    ✓ 必须对每个节点检查，直径可能不经过根
//
// 2. ✗ 递归函数返回 left + right（直径）给父节点
//    ✓ 返回 1 + max(left, right)（深度）给父节点
//    → 返回值是「深度」，更新的 ans 才是「直径」
//
// 3. ✗ depth(null) 返回 -1 但忘记调整后续计算
//    ✓ 如果 depth(null)=0，则 depth(叶子)=1（代表1条边）
//    → 保持定义一致即可，本题用 depth(null)=0 最自然
//
// 4. ✗ 忘记在递归中写 ans = max(ans, left + right)
//    ✓ 这一行是整个算法的灵魂，漏掉就只算了深度没算直径
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: 直径一定经过根节点吗？
// → 不一定。必须检查每个节点。
//
// Q2: 递归函数返回什么？和答案什么关系？
// → 返回深度（给父节点），答案在递归过程中通过全局变量维护。
//   「返回值 ≠ 答案」是这类题的核心模式。
//
// Q3: 不用递归怎么做？
// → 后序遍历（迭代），用 hashmap 存每个节点深度（见 Solution3）。
//
// Q4: 不用全局变量怎么做？
// → 返回 pair<depth, diameter>（见 Solution4）。
//
// Q5: 如果要返回直径路径上的节点列表（而不只是长度）？
// → 每个节点返回「到最远叶子的路径节点列表」，
//   更新最大直径时拼接左右路径即可。
//
// 【相关题型】
// - 124. 二叉树中的最大路径和 — 完全相同的框架，
//   区别：求节点值之和而非边数，且需要 max(0, child) 剪枝负值
// - 104. 二叉树的最大深度 — 本题的子问题，543 多了一步更新直径
// - 687. 最长同值路径 — 同框架，加条件：子节点值 == 当前值才延伸
// - 110. 平衡二叉树 — 同样在求深度时"顺手"判断平衡性
// ============================================================
