/*
 * LeetCode 545: 二叉树的边界 (Boundary of Binary Tree)
 * 
 * 【题目本质】
 * 将二叉树的"外壳"按顺序收集：根 → 左边界(自顶向下) → 叶子(左到右) → 右边界(自底向上)。
 * 核心难点在于精确理解"左边界"和"右边界"的定义，以及保证各部分不重复。
 *
 * 【解法总览】
 * 解法1: 分三部分收集   — O(n) / O(n) — 最直观，面试首选
 * 解法2: 一次DFS + 标记  — O(n) / O(n) — 更优雅，展示思维深度
 */

/*
 * 示例树 (Example 2):
 *
 *              1               边界拼接过程:
 *            /   \             [1]         ← 根
 *           2     3            + [2]       ← 左边界 (2有左孩子4，4是叶子停)
 *          / \   /             + [4,7,8,9,10] ← 叶子 (前序遍历左到右)
 *         4   5 6              + [6,3]     ← 右边界逆序 (3→6→10，10叶子停，[3,6]→[6,3])
 *            / \ / \           = [1,2,4,7,8,9,10,6,3]
 *           7  8 9  10
 *
 * 左边界路径: root->left = 2, 2有左孩子4, 4是叶子 → 停
 *   收集: [2]
 *
 * 叶子(前序DFS): 4, 7, 8, 9, 10
 *   收集: [4, 7, 8, 9, 10]
 *
 * 右边界路径: root->right = 3, 3无右孩子→走左6, 6有右孩子10, 10是叶子 → 停
 *   收集: [3, 6], 逆序 → [6, 3]
 */

// ============================================================
// 解法1: 分三部分收集 — 分别收集左边界、叶子、右边界再拼接
// 时间: O(n)  空间: O(n) (递归栈 + 结果数组)
//
// 【思路】
// 题目定义了边界 = 根 + 左边界 + 叶子 + 逆序右边界，
// 最自然的做法就是分开收集这四部分。
// 
// 关键避免重复的设计:
// - 左边界: 从 root->left 开始，遇到叶子停止 → 不包含叶子
// - 右边界: 从 root->right 开始，遇到叶子停止 → 不包含叶子
// - 叶子: DFS 统一收集所有叶子
// - 根: 非叶子时单独加入（只有一个节点时，根=叶子，直接返回）
// 
// 这样三部分天然互不重叠。
// ============================================================
class Solution1 {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        
        // 特殊情况: 只有根节点（题目说根不算叶子，但只有一个节点时应返回它）
        if (!root->left && !root->right) return {root->val};
        
        vector<int> res;
        
        // Part 1: 根节点
        res.push_back(root->val);
        
        // Part 2: 左边界 (从 root->left 开始，叶子前停止)
        // 
        // 走法: 有左走左，无左走右，遇叶子停
        //
        //    例: root->left = 2
        //        2 有左孩子 4 → 加入 2
        //        4 是叶子 → 停止
        //    结果: [2]
        collectLeftBoundary(root->left, res);
        
        // Part 3: 所有叶子 (前序 DFS，保证左到右顺序)
        collectLeaves(root, res);
        
        // Part 4: 右边界 (从 root->right 开始，叶子前停止，最后逆序)
        //
        // 走法: 有右走右，无右走左，遇叶子停
        //
        //    例: root->right = 3
        //        3 无右孩子，走左 6 → 加入 3
        //        6 有右孩子 10 → 加入 6
        //        10 是叶子 → 停止
        //    收集 [3, 6]，逆序后 [6, 3]
        collectRightBoundary(root->right, res);
        
        return res;
    }
    
private:
    bool isLeaf(TreeNode* node) {
        return node && !node->left && !node->right;
    }
    
    void collectLeftBoundary(TreeNode* node, vector<int>& res) {
        TreeNode* cur = node;
        // 遇到叶子停止 — 叶子交给 collectLeaves 收集，避免重复
        while (cur && !isLeaf(cur)) {
            res.push_back(cur->val);
            // 有左走左，无左走右（题目对左边界的精确定义）
            cur = cur->left ? cur->left : cur->right;
        }
    }
    
    void collectLeaves(TreeNode* node, vector<int>& res) {
        if (!node) return;
        if (isLeaf(node)) {
            res.push_back(node->val);
            return;
        }
        // 前序遍历: 先左后右，保证叶子从左到右收集
        collectLeaves(node->left, res);
        collectLeaves(node->right, res);
    }
    
    void collectRightBoundary(TreeNode* node, vector<int>& res) {
        // 先自顶向下收集，再逆序加入结果
        vector<int> tmp;
        TreeNode* cur = node;
        while (cur && !isLeaf(cur)) {
            tmp.push_back(cur->val);
            // 有右走右，无右走左（题目对右边界的精确定义）
            cur = cur->right ? cur->right : cur->left;
        }
        // 逆序加入 — 题目要求右边界是自底向上的
        for (int i = tmp.size() - 1; i >= 0; i--) {
            res.push_back(tmp[i]);
        }
    }
};


// ============================================================
// 解法2: 一次 DFS + 标记 — 前序遍历中根据 flag 分类收集
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1跑了三次遍历（左边界while循环 + 叶子DFS + 右边界while循环），
// 能不能一次前序遍历搞定？
//
// 核心思想: 给每个节点一个 flag 标记它的"角色":
//   flag=0: 根节点
//   flag=1: 左边界节点
//   flag=2: 右边界节点  
//   flag=3: 内部节点（不在边界上）
//
// 前序遍历时:
//   - flag=0 或 flag=1 的非叶子节点 → 加入 left_part
//   - flag=2 的非叶子节点 → 加入 right_part（最后逆序）
//   - 叶子节点（无论 flag）→ 加入 leaves_part
//   - flag=3 的非叶子节点 → 不加入任何部分
//
// flag 如何向下传递？
//
//   当前 flag  | 左孩子 flag          | 右孩子 flag
//   -----------------------------------------------
//   0 (根)     | 1 (左边界)           | 2 (右边界)
//   1 (左边界) | 1 (继承)             | 有左孩子?3:1
//   2 (右边界) | 有右孩子?3:2         | 2 (继承)
//   3 (内部)   | 3                    | 3
//
// 关键: 左边界"有左走左、无左走右"在 flag 传递中体现为:
//   - 左边界节点的左孩子: 继承 flag=1
//   - 左边界节点无左孩子时，右孩子继承 flag=1
//   - 左边界节点有左孩子时，右孩子变成内部节点 flag=3
//
// ============================================================
class Solution2 {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        if (!root->left && !root->right) return {root->val};
        
        vector<int> leftPart, leaves, rightPart;
        
        // 前序遍历，flag=0 表示根节点
        dfs(root, 0, leftPart, leaves, rightPart);
        
        // 拼接: leftPart + leaves + 逆序rightPart
        vector<int> res;
        res.insert(res.end(), leftPart.begin(), leftPart.end());
        res.insert(res.end(), leaves.begin(), leaves.end());
        res.insert(res.end(), rightPart.rbegin(), rightPart.rend());
        return res;
    }
    
private:
    bool isLeaf(TreeNode* node) {
        return node && !node->left && !node->right;
    }
    
    void dfs(TreeNode* node, int flag,
             vector<int>& leftPart, vector<int>& leaves, vector<int>& rightPart) {
        if (!node) return;
        
        // 叶子节点: 无论 flag 是什么，都收集到 leaves
        if (isLeaf(node)) {
            leaves.push_back(node->val);
            return;
        }
        
        // 非叶子节点: 根据 flag 决定放入哪个部分
        if (flag == 0 || flag == 1) {
            // 根或左边界 → 加入 leftPart（根也放这里，保证出现在最前面）
            leftPart.push_back(node->val);
        } else if (flag == 2) {
            // 右边界 → 加入 rightPart（最后逆序输出）
            rightPart.push_back(node->val);
        }
        // flag == 3: 内部节点，不加入任何部分
        
        // 计算左右孩子的 flag
        //
        // 左孩子的 flag:
        //   当前是根(0): 左孩子→左边界(1)
        //   当前是左边界(1): 左孩子继承→左边界(1)
        //   当前是右边界(2): 有右孩子→左孩子变内部(3); 无右孩子→左孩子继承右边界(2)
        //   当前是内部(3): 左孩子→内部(3)
        int leftFlag, rightFlag;
        
        if (flag == 0) {
            leftFlag = 1;   // 根的左孩子是左边界
            rightFlag = 2;  // 根的右孩子是右边界
        } else if (flag == 1) {
            leftFlag = 1;   // 左边界的左孩子继承
            // 左边界节点有左孩子 → 右孩子变内部; 无左孩子 → 右孩子继承左边界
            rightFlag = node->left ? 3 : 1;
        } else if (flag == 2) {
            // 右边界节点有右孩子 → 左孩子变内部; 无右孩子 → 左孩子继承右边界
            leftFlag = node->right ? 3 : 2;
            rightFlag = 2;  // 右边界的右孩子继承
        } else {
            leftFlag = 3;   // 内部的孩子还是内部
            rightFlag = 3;
        }
        
        // 前序: 先左后右
        dfs(node->left, leftFlag, leftPart, leaves, rightPart);
        dfs(node->right, rightFlag, leftPart, leaves, rightPart);
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 分三部分    | 解法2: 一次DFS+标记  |
// |-------------|-------------------|---------------------|
// | 遍历次数     | 3次              | 1次                 |
// | 代码直观性   | ⭐非常清晰        | 较复杂，flag传递易错 |
// | 面试推荐     | ⭐首选            | 展示深度时用        |
// | 出错概率     | 低               | 中等               |
//
// 两种解法时间空间复杂度相同，都是 O(n)。
// 面试中推荐解法1，因为逻辑清晰、不容易出错。
// 如果面试官追问"能否一次遍历搞定？"，再展示解法2。
//
// ============================================================
// 【易错点】
//
// 1. 根节点是叶子时重复收集:
//    ✗ 先 push root，再 addLeaves(root) 中再 push → 重复
//    ✓ 先检查 root 是否叶子，只有一个节点直接返回 {root->val}
//
// 2. 左边界没在叶子前停止:
//    ✗ while (cur) { res.push_back(cur->val); ... }
//    ✓ while (cur && !isLeaf(cur)) { ... }
//    少了 !isLeaf(cur) 导致叶子被左边界和 addLeaves 都收集了
//
// 3. 左边界从 root 开始而不是 root->left:
//    ✗ TreeNode* cur = root;  → 根被重复收集
//    ✓ TreeNode* cur = root->left;  → 左边界从根的左子节点开始
//
// 4. 右边界忘记逆序:
//    ✗ 直接把右边界顺序拼接 → 输出顺序错误
//    ✓ 收集后 reverse，或从后往前加入结果
//
// 5. 解法2中 flag 传递规则搞反:
//    ✗ 左边界节点有左孩子时，右孩子继承左边界 → 错
//    ✓ 左边界节点有左孩子时，右孩子变内部节点(flag=3)
//       因为"有左走左"，右孩子不在左边界路径上
//
// ============================================================
// 【面试追问】
//
// Q1: 如何保证左边界、叶子、右边界不重复？
//   → 左/右边界在遇到叶子前停止收集，叶子由 DFS 统一收集。
//     三部分的收集范围天然不重叠。
//
// Q2: 树退化成链表（如只有右子树）怎么办？
//   → 仍然正确。例如 1→2→3→4:
//     左边界为空(root无左孩子)，叶子=[4]，右边界=[2,3]逆序=[3,2]
//     结果: [1,4,3,2]
//     递归深度 O(n)，如果担心栈溢出可将叶子收集改为迭代栈。
//
// Q3: 能否纯迭代实现（不用递归）？
//   → 可以。左边界和右边界本身就是 while 循环（已是迭代）。
//     叶子收集用栈模拟前序遍历: 入栈先右后左，出栈时检查是否叶子。
//
// Q4: 和二叉树的"左视图""右视图"(LC199)有什么区别？
//   → 左视图是BFS每层最左节点。左边界是"有左走左、无左走右"的路径。
//     例如: 根有左孩子A，A无左孩子但有右孩子B。
//     左视图中A是某层最左，B不一定是下层最左。
//     但左边界中A和B都在路径上(A无左→走右到B)。
// ============================================================
