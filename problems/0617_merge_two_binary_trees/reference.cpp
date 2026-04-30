/*
 * LeetCode 617: 合并二叉树 (Merge Two Binary Trees)
 *
 * 【题目本质】
 * 同步遍历两棵树，对应位置节点值相加，某侧为空则直接取另一侧。
 * 核心是"双参数递归"框架的典型应用。
 *
 * 【解法总览】
 * 解法1: 递归(修改原树)    — O(min(m,n)) / O(min(h1,h2)) — 面试首选
 * 解法2: 递归(新建节点)    — O(min(m,n)) / O(min(h1,h2)) — 不修改输入版本
 * 解法3: 迭代BFS(队列)     — O(min(m,n)) / O(min(m,n))   — 展示迭代能力
 *
 * 【示例树的合并过程】
 *
 *    root1:         root2:          merged:
 *       1              2               3        ← 1+2=3
 *      / \            / \             / \
 *     3   2          1   3           4   5      ← 3+1=4, 2+3=5
 *    /              / \   \         / \   \
 *   5              -   4   7       5   4   7    ← 5+0, 0+4, 0+7
 *
 *  递归过程（前序DFS）：
 *  merge(1,2) → val=3, 递归左子树merge(3,1), 递归右子树merge(2,3)
 *    merge(3,1) → val=4, 递归左merge(5,null), 递归右merge(null,4)
 *      merge(5,null) → root2为null, 返回5
 *      merge(null,4) → root1为null, 返回4    ← 整棵子树嫁接!
 *    merge(2,3) → val=5, 递归左merge(null,null), 递归右merge(null,7)
 *      merge(null,null) → 返回null
 *      merge(null,7) → root1为null, 返回7    ← 整棵子树嫁接!
 */


// ============================================================
// 解法1: 递归(修改原树) — 面试首选，代码最简洁
// 时间: O(min(m,n))  空间: O(min(h1,h2)) 递归栈深
//
// 【思路】
// 树的题优先想递归。函数签名 merge(root1, root2) 天然就是"双参数递归"。
// 核心分三种情况：
//   1) root1为空 → 返回root2（把root2的整棵子树嫁接过来）
//   2) root2为空 → 返回root1（保持root1不变）
//   3) 都非空 → 值相加存root1, 递归处理左右子树
//
// 为什么 O(min(m,n))？因为只要某一侧遇到null就停止递归，
// 直接返回另一侧子树，不需要继续遍历。
// ============================================================
class Solution1 {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        // 某一侧为空 → 直接返回另一侧（含两侧都空返回null的情况）
        if (!root1) return root2;
        if (!root2) return root1;
        
        // 两侧都非空：值累加到root1上
        root1->val += root2->val;
        
        // 递归合并左右子树，结果挂到root1上
        root1->left = mergeTrees(root1->left, root2->left);
        root1->right = mergeTrees(root1->right, root2->right);
        
        return root1;
    }
};

// ============================================================
// 解法2: 递归(新建节点) — 不修改输入树
// 时间: O(min(m,n))  空间: O(min(h1,h2)) 递归栈 + O(n) 新节点
//
// 【思路】
// 面试追问"能否不修改输入"时给出此解法。
// 唯一区别：不在root1上累加，而是每次new一个新节点。
//
// 注意：当某一侧为null时直接返回另一侧，严格来说还是共享了
// 原树的节点。如果要完全独立，需要对非null侧做深拷贝。
// 但LeetCode和大多数面试不要求这么严格。
// ============================================================
class Solution2 {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1) return root2;
        if (!root2) return root1;
        
        // 新建节点而非修改root1 ← 与解法1的唯一区别
        TreeNode* merged = new TreeNode(root1->val + root2->val);
        merged->left = mergeTrees(root1->left, root2->left);
        merged->right = mergeTrees(root1->right, root2->right);
        
        return merged;
    }
};

// ============================================================
// 解法3: 迭代BFS(队列) — 用队列同步层序遍历两棵树
// 时间: O(min(m,n))  空间: O(min(m,n)) 队列最多存一层节点
//
// 【思路】
// 递归能做的事，迭代也能做。BFS用队列存放"一对节点"。
// 核心逻辑和递归完全一样，只是用循环代替了递归调用：
//   - 出队一对(n1, n2)，n1.val += n2.val
//   - 左子树：两边都有 → 入队配对；n1没有 → 嫁接n2的
//   - 右子树：同理
//
// 入队保证：只有两边都非空才入队，这样出队时不用判null。
//
// 【BFS遍历过程（以示例1为例）】
// 初始队列: [(1,2)]
// 
// 弹出(1,2): val=3
//   左: (3,1)都非空 → 入队    右: (2,3)都非空 → 入队
//   队列: [(3,1), (2,3)]
//
// 弹出(3,1): val=4
//   左: (5,null) n2空,不处理  右: (null,4) n1空 → 嫁接n2.left=4
//   队列: [(2,3)]
//
// 弹出(2,3): val=5
//   左: (null,null)都空,不处理  右: (null,7) n1空 → 嫁接n2.right=7
//   队列: [] → 结束
// ============================================================
using std::queue;
using std::pair;

class Solution3 {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1) return root2;
        if (!root2) return root1;
        
        queue<pair<TreeNode*, TreeNode*>> q;
        q.push({root1, root2});
        
        while (!q.empty()) {
            auto [n1, n2] = q.front();
            q.pop();
            
            // 入队时已保证n1和n2都非空
            n1->val += n2->val;
            
            // 处理左子树
            if (n1->left && n2->left) {
                // 两边都有左子树 → 配对入队，下一轮处理
                q.push({n1->left, n2->left});
            } else if (!n1->left) {
                // n1没左子树，把n2的整棵左子树嫁接过来
                n1->left = n2->left;
            }
            // n2->left为空时，n1->left保持原样，无需操作
            
            // 处理右子树（逻辑完全对称）
            if (n1->right && n2->right) {
                q.push({n1->right, n2->right});
            } else if (!n1->right) {
                n1->right = n2->right;
            }
        }
        
        return root1;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1(递归改原树) | 解法2(递归新建)   | 解法3(迭代BFS)    |
// |-------------|------------------|------------------|------------------|
// | 代码行数     | ~6行核心          | ~7行核心          | ~20行核心         |
// | 修改输入     | 是(改root1)       | 否(近似)          | 是(改root1)       |
// | 空间复杂度   | O(h) 栈深         | O(h) 栈深         | O(n) 队列         |
// | 面试推荐     | ⭐ 首选           | 追问不修改时给出   | 展示迭代能力      |
//
// 面试策略：先写解法1（简洁清晰），被问"不修改输入"给解法2，
// 被问"能否迭代"给解法3。
//
// ============================================================
// 【易错点】
//
// 1. 终止条件只写了 (!root1 && !root2)：
//    ✗  if (!root1 && !root2) return nullptr;
//       root1->val += root2->val;  // 如果只有一个为null → 空指针!
//    ✓  if (!root1) return root2;
//       if (!root2) return root1;
//    为什么：必须分别处理单侧为null的情况，这同时也涵盖了两侧都null的情况
//    （当root1==null时返回root2，如果root2也是null，返回的就是null）
//
// 2. 迭代解法忘记嫁接：
//    ✗  只处理了两边都非空的情况，漏掉 else if (!n1->left) n1->left = n2->left;
//    → root2独有的子树被丢失
//    ✓  三种情况都要处理：两边都有 → 入队；n1没有 → 嫁接n2的；n2没有 → 不管
//
// 3. 迭代解法入队null节点：
//    ✗  不检查直接 q.push({n1->left, n2->left}); → 出队时解引用null
//    ✓  入队前检查两边都非空
//
// ============================================================
// 【面试追问】
//
// Q1: 你的递归解法修改了root1，能否不修改输入？
//     → 解法2：每次new新节点。如果要完全独立于输入，null分支也需深拷贝。
//
// Q2: 递归最坏空间复杂度是多少？什么情况下发生？
//     → O(min(h1,h2))。最坏情况：两棵树都是退化链表（全左偏或全右偏），
//       且长度相同，递归深度=树高=n。
//       注意：不是O(max)而是O(min)，因为较短的那棵树遇到null就返回了。
//
// Q3: 如果不是二叉树而是N叉树，怎么改？
//     → 把left/right改成遍历children数组，按索引i配对：
//       for (int i = 0; i < max(size1, size2); i++)
//         merged->children[i] = merge(child1[i], child2[i])
//       超出一侧范围的视为null。
//
// Q4: 能否用迭代DFS(栈)？
//     → 可以，把解法3的queue换成stack，逻辑完全一样，只是遍历顺序变了
//       （DFS而非BFS），结果相同。
//
// 【相关题型】
// - 100. 相同的树 — 复用同样的双参数递归框架，
//   区别：100是比较(return left==right)，617是合并(return left+right)
// - 101. 对称二叉树 — 双参数递归，
//   区别：100/617同侧配对(left,left)，101交叉配对(left,right)
// - 226. 翻转二叉树 — 单树递归操作，
//   617是双树递归操作，都练习递归三要素
// ============================================================
