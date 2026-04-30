/*
 * LeetCode 404: 左叶子之和 (Sum of Left Leaves)
 *
 * 【题目本质】
 * 遍历二叉树，累加所有"左叶子"的值。
 * 关键：一个节点自己不知道自己是不是左孩子，判断必须在父节点处完成。
 *
 * 【解法总览】
 * 解法1: DFS递归(父节点判断)  — O(n) / O(h) — 最简洁，面试首选
 * 解法2: DFS递归(传参标记)    — O(n) / O(h) — 思路更直观
 * 解法3: DFS迭代(栈)         — O(n) / O(h) — 面试追问迭代时用
 * 解法4: BFS层序遍历(队列)    — O(n) / O(w) — BFS 视角
 *
 * 示例树:
 *         3
 *        / \
 *       9   20
 *           / \
 *          15   7
 *
 * 左叶子: 9 (3的左孩子，且是叶子)
 *         15 (20的左孩子，且是叶子)
 * 7 是叶子但是右孩子 → 不算
 * 答案: 9 + 15 = 24
 */


// ============================================================
// 解法1: DFS 递归（父节点判断） — 面试首选
// 时间: O(n)  空间: O(h)  h为树高
//
// 【思路】
// 对于每个节点，检查它的左孩子是否为叶子节点。
// 如果是，累加左孩子的值。
// 然后递归处理左右子树，收集更深层的左叶子之和。
//
// 为什么判断放在父节点？因为一个节点自身无法知道
// 自己是左孩子还是右孩子，只有父节点知道。
//
// 遍历过程:
//         3           ← 检查左孩子9: 是叶子! sum += 9
//        / \
//       9   20        ← 检查左孩子15: 是叶子! sum += 15
//           / \
//          15   7     ← 15和7都是叶子，但从它们自身的角度
//                       递归进去后左右都为null，返回0
// ============================================================
class Solution1 {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        
        int sum = 0;
        
        // 核心判断：当前节点的左孩子存在 且 是叶子节点
        // 为什么要检查 left->left 和 left->right？
        // 因为叶子的定义就是没有任何孩子
        if (root->left && !root->left->left && !root->left->right) {
            sum += root->left->val;
        }
        
        // 继续向下搜索（即使左孩子已经被累加，递归进去也只返回0，不影响正确性）
        sum += sumOfLeftLeaves(root->left);
        sum += sumOfLeftLeaves(root->right);
        
        return sum;
    }
};

// ============================================================
// 解法2: DFS 递归（传参标记）
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 另一个角度：让每个节点"知道"自己是不是左孩子。
// 通过给递归函数加一个 isLeft 参数来传递这个信息。
// 如果一个节点是叶子且 isLeft=true，返回自己的值。
//
// 这个思路在概念上更清晰：每个节点自己决定是否贡献值。
// 但需要一个辅助函数（因为原始函数签名没有 isLeft 参数）。
// ============================================================
class Solution2 {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        // 根节点不是任何人的左孩子，所以传 false
        return dfs(root, false);
    }
    
    int dfs(TreeNode* node, bool isLeft) {
        if (!node) return 0;
        
        // 叶子节点：是左孩子就贡献值，否则贡献0
        if (!node->left && !node->right) {
            return isLeft ? node->val : 0;
        }
        
        // 非叶子节点：递归左右子树
        // 进入左子树时标记 true，进入右子树时标记 false
        return dfs(node->left, true) + dfs(node->right, false);
    }
};

// ============================================================
// 解法3: DFS 迭代（栈）
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 用显式栈模拟递归。每弹出一个节点，就检查它的左孩子
// 是否为叶子——与解法1完全相同的判断逻辑。
//
// 栈的模拟过程（以示例为例）:
// 初始: stack = [3]
// 弹出3: 左孩子9是叶子 → sum=9, 右孩子20入栈 → stack = [20]
// 弹出20: 左孩子15是叶子 → sum=24, 右孩子7入栈 → stack = [7]
// 弹出7: 无左孩子，无右孩子 → stack = []
// 结束: sum = 24
// ============================================================
class Solution3 {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        
        int sum = 0;
        // 使用 vector 当栈也行，这里用标准 stack
        // 但为了与教学一致用 stack
        // （实际上直接用 vector 做栈在 C++ 中更常见）
        
        // 手动包含的头文件省略（按规范不写 #include）
        // std::stack<TreeNode*>
        
        // 这里简单用递归风格的迭代
        struct StackFrame {
            TreeNode* node;
        };
        
        // 简洁写法：直接用函数指针栈
        TreeNode* stk[1001];  // 题目限制最多1000节点
        int top = -1;
        stk[++top] = root;
        
        while (top >= 0) {
            TreeNode* node = stk[top--];
            
            // 与递归版完全相同的判断
            if (node->left) {
                if (!node->left->left && !node->left->right) {
                    // 左孩子是叶子，直接累加，不需要入栈
                    sum += node->left->val;
                } else {
                    // 左孩子不是叶子，入栈继续探索
                    stk[++top] = node->left;
                }
            }
            if (node->right) {
                stk[++top] = node->right;
            }
        }
        
        return sum;
    }
};

// ============================================================
// 解法4: BFS 层序遍历（队列）
// 时间: O(n)  空间: O(w)  w为树的最大宽度
//
// 【思路】
// 用队列做层序遍历，对每个出队的节点做同样的左叶子判断。
// BFS 和 DFS 在本题中没有本质区别，因为我们需要遍历
// 所有节点，只是遍历顺序不同。
//
// BFS 过程:
// Level 0: queue = [3]
//   弹出3 → 左孩子9是叶子, sum=9; 右孩子20入队
// Level 1: queue = [20]  (9是叶子已处理，不入队)
//   弹出20 → 左孩子15是叶子, sum=24; 右孩子7入队
// Level 2: queue = [7]
//   弹出7 → 无子节点
// 结束: sum = 24
// ============================================================
class Solution4 {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        
        int sum = 0;
        // 用数组模拟队列（避免 #include）
        TreeNode* q[1001];
        int front = 0, back = 0;
        q[back++] = root;
        
        while (front < back) {
            TreeNode* node = q[front++];
            
            if (node->left) {
                if (!node->left->left && !node->left->right) {
                    sum += node->left->val;
                } else {
                    q[back++] = node->left;
                }
            }
            if (node->right) {
                q[back++] = node->right;
            }
        }
        
        return sum;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   - 解法1在父节点处判断，不需要额外参数，代码最短
//   - 解法2在子节点处判断（通过传参），概念上更对称
//   - 面试中推荐解法1，更简洁
//
// 递归 vs 迭代 (解法1/2 vs 解法3/4):
//   - 判断逻辑完全一致，只是遍历方式不同
//   - 递归更简洁，迭代用于面试官要求"不用递归"的场景
//   - 迭代版本的空间复杂度本质上和递归相同
//
// DFS vs BFS (解法3 vs 解法4):
//   - 本题中没有本质区别，都是 O(n) 遍历
//   - 如果追问"第k层的左叶子之和"，BFS 更方便
//
// 【易错点】
//
// 1. 把右叶子也累加了：
//    ✗ if (node->right && !node->right->left && !node->right->right)
//        sum += node->right->val;
//    ✓ 右叶子不算，只检查 node->left
//
// 2. 叶子判断不完整：
//    ✗ if (root->left && !root->left->left)  // 只检查了左孩子为空
//    ✓ if (root->left && !root->left->left && !root->left->right)
//       必须左右孩子都为空才是叶子
//
// 3. 根节点是唯一节点时返回了 root->val：
//    ✗ 认为根节点也是"左叶子" → 根节点不是任何节点的左孩子
//    ✓ 解法1自然返回0（root->left为null，不进入if）
//       解法2中 dfs(root, false)，叶子但isLeft=false，返回0
//
// 4. 迭代版中左叶子入栈后被重复处理：
//    不会导致错误答案（叶子入栈后弹出，无子节点，不会累加），
//    但会做无意义的操作。最好直接累加不入栈。
//
// 【面试追问】
//
// Q1 (基础): "如何判断一个节点是不是左叶子？为什么不能在节点自身判断？"
//   → 因为节点自身不携带"我是左孩子还是右孩子"的信息。
//     必须由父节点来判断：node->left 存在且是叶子。
//     或者通过传参数标记（解法2）。
//
// Q2 (变体): "如果改成求右叶子之和呢？"
//   → 把判断条件从 node->left 改成 node->right：
//     if (node->right && !node->right->left && !node->right->right)
//         sum += node->right->val;
//
// Q3 (进阶): "如果要求第k层的左叶子之和？"
//   → 方法1: BFS 层序遍历，用 level 变量追踪层数，只在第k层判断。
//   → 方法2: DFS 递归传入 depth 参数，depth==k 时才累加。
//
// Q4 (深入): "能否不用递归也不用额外数据结构，O(1)空间？"
//   → 理论上可以用 Morris 遍历（利用线索化），O(1)空间遍历整棵树。
//     但在遍历过程中判断左叶子的逻辑会变复杂，面试中一般不要求。
// ============================================================
