/*
 * LCR 143: 子结构判断 (Tree Substructure)
 *
 * 判断树 B 是否为树 A 的子结构。
 * 子结构 != 子树：B 的叶子处 A 可以有更多子节点。
 *
 * 难度：Medium
 * 标签：Tree, Depth-First Search, Binary Tree
 */

/*
 * ASCII 树图示例：
 *
 * 示例1（返回 false）:
 *
 *   A: [1,7,5]        B: [6,1]
 *
 *       1                6
 *      / \              /
 *     7   5            1
 *
 *   遍历 A 每个节点：
 *     起点 1: match(1, 6) → 1 != 6 → false
 *     起点 7: match(7, 6) → 7 != 6 → false
 *     起点 5: match(5, 6) → 5 != 6 → false
 *   → 全部失败 → return false
 *
 *
 * 示例2（返回 true）:
 *
 *   A: [3,6,7,1,8]     B: [6,1]
 *
 *       3                6
 *      / \              /
 *     6   7            1
 *    / \
 *   1   8
 *
 *   遍历 A 每个节点：
 *     起点 3: match(3, 6) → 3 != 6 → false
 *     起点 6: match(6, 6) → 6 == 6 ✓
 *             → match(1, 1) → 1 == 1 ✓
 *               → match(null, null) → B空 → true ✓
 *               → match(null, null) → B空 → true ✓
 *             → match(8, null) → B空 → true ✓  ← 子结构关键：B叶子不要求A也是叶子
 *             → true ✓
 *   → return true
 *
 *
 * 子结构 vs 子树的区别图解:
 *
 *   A:         B:          子结构? → YES    子树? → NO
 *     1          1         B的"2"是叶子,     子树要求A中以2为根
 *    / \        /          A的"2"有子节点    的子树完全等于B
 *   2   3      2           子结构不要求      {2,4,5} != {2}
 *  / \                     B覆盖到A的
 * 4   5                    全部子节点
 *
 */




/* ============================================================
 * 解法1：双重递归 — O(m*n) / O(max(m,n)) ⭐ 面试首选
 *
 * 两个函数的分工：
 *
 * isSubStructure(A, B):  在 A 中找起点
 *   "A 或 B 为空 → false"
 *   "以 A 为起点能匹配 B？或在 A 的左子树找？或在 A 的右子树找？"
 *
 * match(A, B):  从起点逐节点匹配
 *   "B 空了 → true（B 匹配完成）"
 *   "A 空了 → false（A 不够用）"
 *   "值不等 → false"
 *   "递归匹配左右子树"
 *
 *
 * 调用关系图：
 *
 *   isSubStructure(A=3, B=6)
 *   ├── match(3, 6) → false (3 != 6)
 *   ├── isSubStructure(A=6, B=6)          ← 递归到 A 的左子树
 *   │   ├── match(6, 6) → 6==6 ✓
 *   │   │   ├── match(1, 1) → 1==1 ✓
 *   │   │   │   ├── match(null, null) → B空 → true ✓
 *   │   │   │   └── match(null, null) → B空 → true ✓
 *   │   │   │   → true && true = true ✓
 *   │   │   └── match(8, null) → B空 → true ✓
 *   │   │   → true && true = true ✓
 *   │   └── return true (match 成功, 短路)
 *   └── return true (左子树找到, 短路)
 *
 * ============================================================ */

class Solution {
public:
    // 外层递归：遍历 A 的每个节点，尝试作为匹配起点
    bool isSubStructure(TreeNode* A, TreeNode* B) {
        // 空树特判：题目规定空树不是任何树的子结构
        // 所以 A 或 B 为空时直接返回 false
        if (!A || !B) return false;

        // 三个 OR 条件（短路求值）：
        // 1. 以 A 的根为起点，能匹配 B
        // 2. B 是 A->left 的子结构（递归在左子树中找起点）
        // 3. B 是 A->right 的子结构（递归在右子树中找起点）
        return match(A, B)
            || isSubStructure(A->left, B)
            || isSubStructure(A->right, B);
    }

private:
    // 内层递归：从 A 和 B 的当前节点开始逐一比对
    bool match(TreeNode* A, TreeNode* B) {
        // 【关键1】B 遍历完了 → 匹配成功
        // 这是子结构和子树的核心区别！
        // 子树需要：if (!A && !B) return true;
        // 子结构只需要 B 走完就行
        if (!B) return true;

        // 【关键2】A 到底了但 B 还没完 → 匹配失败
        // A 的深度不够，无法完整匹配 B
        if (!A) return false;

        // 【关键3】当前节点值不等 → 匹配失败
        if (A->val != B->val) return false;

        // 值相等，递归匹配左右子树
        // 左右子树都必须匹配（AND 关系）
        return match(A->left, B->left)
            && match(A->right, B->right);
    }
};


/* ============================================================
 * 解法1b：更详细的注释版本（教学用）
 * ============================================================ */

class Solution_Verbose {
public:
    bool isSubStructure(TreeNode* A, TreeNode* B) {
        // ======== 特殊情况 ========
        // 根据题意，空树不是任何树的子结构
        if (A == nullptr || B == nullptr) {
            return false;
        }

        // ======== 尝试以 A 为起点匹配 ========
        if (match(A, B)) {
            return true;  // 找到了！
        }

        // ======== 在 A 的左子树中继续寻找起点 ========
        if (isSubStructure(A->left, B)) {
            return true;  // 左子树里找到了！
        }

        // ======== 在 A 的右子树中继续寻找起点 ========
        if (isSubStructure(A->right, B)) {
            return true;  // 右子树里找到了！
        }

        // ======== A 的所有节点都试过了，都不行 ========
        return false;
    }

private:
    // 从 nodeA 和 nodeB 开始，逐节点比较
    bool match(TreeNode* nodeA, TreeNode* nodeB) {
        // B 走完了 → 全部匹配成功
        if (nodeB == nullptr) {
            return true;
        }

        // A 走完了但 B 还有节点 → 匹配失败（A 不够深）
        if (nodeA == nullptr) {
            return false;
        }

        // 当前节点值不同 → 匹配失败
        if (nodeA->val != nodeB->val) {
            return false;
        }

        // 当前节点值相同，继续递归比较左右子树
        bool leftMatch = match(nodeA->left, nodeB->left);
        bool rightMatch = match(nodeA->right, nodeB->right);

        // 左右都匹配才算成功
        return leftMatch && rightMatch;
    }
};


/* ============================================================
 * 复杂度分析
 *
 * 设 A 有 m 个节点，B 有 n 个节点
 *
 * 时间复杂度：O(m * n)
 *   - 外层 isSubStructure 遍历 A 的每个节点：O(m) 次调用
 *   - 每次调用 match 最多比较 B 的所有节点：O(n)
 *   - 最坏情况：A 是一条链且所有节点值都等于 B 的根值
 *     每个节点都会触发完整的 match → O(m * n)
 *   - 最好情况：A 的根就匹配成功 → O(n)
 *
 * 空间复杂度：O(max(m, n))
 *   - isSubStructure 的递归深度最大为 A 的高度 → O(m) 最坏（链状）
 *   - match 的递归深度最大为 B 的高度 → O(n) 最坏
 *   - 两者嵌套的最大递归深度为 max(m的高度, n的高度)
 *   - 平衡树情况下为 O(log m + log n)
 *
 * ============================================================
 *
 * 边界情况测试：
 *
 * 1. B 为空 → return false（题目规定空树不是子结构）
 * 2. A 为空 → return false
 * 3. A 和 B 完全相同 → return true
 * 4. B 是 A 的叶子节点 → return true
 * 5. B 比 A 大 → return false（A 不够深/宽）
 * 6. A 和 B 值全相同但结构不同 → 取决于具体结构
 *
 * 示例测试：
 *   A=[1,2,3,4], B=[3]     → true  （B 的单节点在 A 中找到）
 *   A=[-1,3,2,0], B=[]      → false （B 为空）
 *   A=[1,2], B=[1,2]        → true  （完全匹配）
 *   A=[1,2], B=[1,null,2]   → false （B 要求右子节点=2，A 只有左子节点）
 *
 * ============================================================ */


/* ============================================================
 * 📋 总结区块
 * ============================================================
 *
 * 一、解法对比
 * ┌────────────┬──────────┬───────────────┬───────────────────────┐
 * │ 解法       │ 时间     │ 空间          │ 说明                  │
 * ├────────────┼──────────┼───────────────┼───────────────────────┤
 * │ 双重递归   │ O(m*n)   │ O(max(m,n))   │ 面试首选，思路清晰    │
 * │ Verbose版  │ O(m*n)   │ O(max(m,n))   │ 教学用，逻辑完全等价  │
 * └────────────┴──────────┴───────────────┴───────────────────────┘
 * 本题核心是"两层递归的分工"：外层找起点，内层做匹配。
 * 没有更优时间复杂度的通用解法（除非对特殊结构做序列化+KMP）。
 *
 * 二、易错点
 * 1. 子结构 vs 子树：
 *    子结构: if (!B) return true;       // B走完即成功
 *    子树:   if (!A && !B) return true;  // 必须同时走完
 *    这是本题最核心的区分点。
 *
 * 2. 空树判定：题目规定空树不是任何树的子结构，
 *    所以 isSubStructure 一开始必须判 !A || !B → false。
 *    但 match 中 !B → true 是"匹配完成"的意思，二者语义不同。
 *
 * 3. 两个递归函数返回值混淆：
 *    isSubStructure 用 OR（任一起点匹配即可）
 *    match 用 AND（左右子树都必须匹配）
 *
 * 4. 忘记短路求值的优势：
 *    match(A,B) || isSubStructure(A->left,B) || ...
 *    一旦 match 成功就不再递归左右子树。
 *
 * 三、面试追问
 * Q1: 子结构和子树有什么区别？代码上怎么改？
 *     → 子结构: if(!B) return true
 *     → 子树:   if(!A && !B) return true; if(!A || !B) return false;
 *       即要求 A 和 B 的结构完全一致（LC 572）
 *
 * Q2: 时间复杂度能优化吗？
 *     → 对一般二叉树无法优化。若树结构特殊（如BST），
 *       可以利用有序性剪枝。序列化+KMP可达O(m+n)，
 *       但实现复杂且需处理序列化歧义问题。
 *
 * Q3: 如果不只是判断子结构，还要返回所有匹配的起点节点呢？
 *     → 去掉短路求值，遍历所有节点，对每个节点调用 match，
 *       匹配成功的收集到结果列表中。时间仍为 O(m*n)。
 *
 * ============================================================ */
