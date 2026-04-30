/*
 * LeetCode 530: 二叉搜索树的最小绝对差 (Minimum Absolute Difference in BST)
 *
 * 【题目本质】
 * BST 中序遍历是有序序列，有序序列的最小差值一定在相邻元素间。
 * 所以本题 = 中序遍历 + 维护 prev 比较相邻节点差值。
 *
 * 【解法总览】
 * 解法1: 中序遍历存数组 — O(n) / O(n) — 最直觉，两步走
 * 解法2: 中序递归 + prev — O(n) / O(h) — 面试首选，边遍历边比较
 * 解法3: 中序迭代 + prev — O(n) / O(h) — 用栈模拟递归，展示功底
 *
 *  注意: 本题与 783 题完全相同
 */

// ============================================================
// 解法1: 中序遍历存数组 — 先收集有序序列，再线性扫描
// 时间: O(n)  空间: O(n)
//
// 【思路】
// BST 中序遍历得到有序数组，最小差值一定在相邻元素之间。
// 这是最直觉的两步法：第一步收集，第二步比较。
// 瓶颈：需要 O(n) 额外空间存整个数组。
//
//       4
//      / \
//     2   6        中序遍历: [1, 2, 3, 4, 6]
//    / \
//   1   3          相邻差值: 1, 1, 1, 2 → 最小 = 1
// ============================================================
class Solution1 {
public:
    int getMinimumDifference(TreeNode* root) {
        vector<int> vals;
        inorder(root, vals);

        int minDiff = INT_MAX;
        for (int i = 1; i < (int)vals.size(); i++) {
            minDiff = min(minDiff, vals[i] - vals[i - 1]);
        }
        return minDiff;
    }

private:
    void inorder(TreeNode* node, vector<int>& vals) {
        if (!node) return;
        inorder(node->left, vals);
        vals.push_back(node->val);
        inorder(node->right, vals);
    }
};

// ============================================================
// 解法2: 中序递归 + prev 指针 — 边遍历边比较，不存数组 ⭐ 面试首选
// 时间: O(n)  空间: O(h)  (h为树高，递归栈开销)
//
// 【思路】
// 解法1的瓶颈是 O(n) 的数组空间。
// 观察：我们只需要"当前节点"和"前一个节点"的差值，不需要整个数组。
// 用一个 prev 指针记住中序遍历中的前一个节点即可。
//
// 【执行过程】
//       4
//      / \
//     2   6
//    / \
//   1   3
//
// 访问1: prev=null → 不比较       → prev=1
// 访问2: prev=1   → diff=2-1=1   → minDiff=1, prev=2
// 访问3: prev=2   → diff=3-2=1   → minDiff=1, prev=3
// 访问4: prev=3   → diff=4-3=1   → minDiff=1, prev=4
// 访问6: prev=4   → diff=6-4=2   → minDiff=1, prev=6
// 返回 1
//
// 【为什么 prev 用引用传递？】
// 递归回溯时，如果 prev 不是引用，子调用中对 prev 的更新会丢失。
// 也可以改用成员变量，但引用传递更函数式，更清晰。
// ============================================================
class Solution2 {
public:
    int getMinimumDifference(TreeNode* root) {
        int minDiff = INT_MAX;
        TreeNode* prev = nullptr;
        inorder(root, prev, minDiff);
        return minDiff;
    }

private:
    void inorder(TreeNode* node, TreeNode*& prev, int& minDiff) {
        if (!node) return;

        // 左子树
        inorder(node->left, prev, minDiff);

        // 处理当前节点：和前驱比较
        if (prev) {
            // 中序遍历保证 node->val >= prev->val，无需取绝对值
            minDiff = min(minDiff, node->val - prev->val);
        }
        prev = node;  // 更新前驱为当前节点

        // 右子树
        inorder(node->right, prev, minDiff);
    }
};

// ============================================================
// 解法2b: 用成员变量代替引用传递（写法变体）
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 和解法2 完全一样的逻辑，只是把 prev 和 minDiff 放成类成员变量。
// 有些人觉得这样更简洁，不容易忘记引用符号 &。
// ============================================================
class Solution2b {
    TreeNode* prev = nullptr;
    int minDiff = INT_MAX;

public:
    int getMinimumDifference(TreeNode* root) {
        prev = nullptr;
        minDiff = INT_MAX;
        inorder(root);
        return minDiff;
    }

private:
    void inorder(TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        if (prev) {
            minDiff = min(minDiff, node->val - prev->val);
        }
        prev = node;
        inorder(node->right);
    }
};

// ============================================================
// 解法3: 中序迭代（显式栈） + prev 指针
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 用栈手动模拟递归的中序遍历过程。
// 核心模板：一路向左入栈 → 弹出处理 → 转右子树。
// 和解法2的区别仅在于递归改成了循环+栈，逻辑完全一致。
//
// 【迭代过程】
//       4
//      / \
//     2   6
//    / \
//   1   3
//
// Step1: curr=4, 左链入栈: 栈=[4,2,1], curr=null
// Step2: 弹出1, prev=null→不比较, prev=1, curr=null(1无右子)
// Step3: 弹出2, prev=1→diff=1, minDiff=1, prev=2, curr=3(2的右子)
// Step4: 左链入栈: 栈=[4,3], curr=null
// Step5: 弹出3, prev=2→diff=1, minDiff=1, prev=3, curr=null
// Step6: 弹出4, prev=3→diff=1, minDiff=1, prev=4, curr=6(4的右子)
// Step7: 左链入栈: 栈=[6], curr=null
// Step8: 弹出6, prev=4→diff=2, minDiff=1, prev=6, curr=null
// 栈空且curr=null → 结束, 返回1
// ============================================================
class Solution3 {
public:
    int getMinimumDifference(TreeNode* root) {
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int minDiff = INT_MAX;

        while (curr || !stk.empty()) {
            // 一路向左，把整条左链入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }

            // 弹出栈顶 = 当前中序遍历的节点
            curr = stk.top();
            stk.pop();

            // 和前驱比较
            if (prev) {
                minDiff = min(minDiff, curr->val - prev->val);
            }
            prev = curr;

            // 转向右子树
            curr = curr->right;
        }

        return minDiff;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | 1. 存数组 | O(n) | O(n) | 最直觉，两步走 |
// | 2. 递归+prev | O(n) | O(h) | 面试首选，代码简洁 |
// | 2b. 成员变量 | O(n) | O(h) | 解法2的写法变体 |
// | 3. 迭代+prev | O(n) | O(h) | 展示栈模拟递归的功底 |
//
// 面试建议：
// - 先说解法2（递归+prev），逻辑清晰且空间最优
// - 被追问"不用递归怎么做"时，切换到解法3（迭代）
// - 解法1 用来展示思路，不建议作为最终答案
//
// ============================================================
// 【易错点】
//
// 1. prev 忘记用引用传递:
//    ✗ void inorder(TreeNode* node, TreeNode* prev, int& minDiff)
//      → prev 的更新在递归返回后丢失，每次比较的 prev 还是 null
//    ✓ void inorder(TreeNode* node, TreeNode*& prev, int& minDiff)
//      → 引用传递确保 prev 的更新对所有递归层可见
//
// 2. 第一个节点不判断 prev 是否为空:
//    ✗ minDiff = min(minDiff, node->val - prev->val);  // prev=nullptr 崩溃
//    ✓ if (prev) { minDiff = min(minDiff, node->val - prev->val); }
//
// 3. 迭代版 while 条件写错:
//    ✗ while (!stk.empty())  → 初始时栈为空就不进循环
//    ✓ while (curr || !stk.empty())  → curr 非空也要继续
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: "暴力解是什么？为什么不用？"
//   → 枚举所有 n*(n-1)/2 对节点算差值，O(n²)。
//     BST 中序有序，只需比较相邻元素 O(n)，暴力完全浪费了 BST 性质。
//
// Q2: "如果输入不是 BST 而是普通二叉树呢？"
//   → 失去中序有序性质。遍历收集所有值 → 排序 → 比较相邻元素。
//     时间 O(n log n)，空间 O(n)。无法做到 O(n) 时间（除非值域有限可以计数排序）。
//
// Q3: "递归栈空间能否避免？"
//   → 可以用 Morris 中序遍历：利用线索化（叶子节点的右指针临时指向后继），
//     实现不用栈的中序遍历。时间 O(n)，空间 O(1)。
//     遍历完后树结构自动恢复。
//
// Q4: "如果这棵 BST 会频繁插入/删除节点，每次都要查最小差值怎么优化？"
//   → 用平衡 BST（如 std::set）维护节点集合，每次插入/删除时
//     用 lower_bound 找前驱后继，O(log n) 更新最小差值。
//     或用 multiset + 全局 min 堆来维护所有相邻差值。
//
// ============================================================
// 【相关题型】
//
// - 783. 二叉搜索树节点最小距离 — 完全相同的题，代码直接复用
// - 94. 二叉树的中序遍历 — 本题的递归和迭代中序模板都来自这道基础题
// - 98. 验证二叉搜索树 — 同样用中序+prev框架，区别：
//       98 是判断 node->val > prev->val（严格递增），
//       本题是算 node->val - prev->val 的最小值
// - 99. 恢复二叉搜索树 — 中序+prev 的进阶应用，找两个逆序的节点
// - 230. BST中第K小的元素 — 中序遍历+计数，遍历到第K个返回
// ============================================================
