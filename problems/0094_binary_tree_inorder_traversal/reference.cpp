/*
 * LeetCode 94: 二叉树的中序遍历 (Binary Tree Inorder Traversal)
 *
 * 【题目本质】
 * 按"左→根→右"的顺序访问二叉树所有节点，返回访问顺序。
 * 这是二叉树遍历的基础题，也是理解"递归 vs 迭代"的经典入门。
 *
 * 【解法总览】
 * 解法1: 递归          — O(n) / O(h) — 最直觉，三行核心代码
 * 解法2: 迭代（显式栈） — O(n) / O(h) — 面试主要考点（Follow-up）
 * 解法3: Morris 遍历   — O(n) / O(1) — 进阶，O(1)空间但临时修改树
 */

// Definition for a binary tree node. (题目已提供，此处供参考)
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode() : val(0), left(nullptr), right(nullptr) {}
//     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
//     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
// };

// ============================================================
// 解法1: 递归 — 最直觉的方式
// 时间: O(n)  空间: O(h), h为树高，最坏O(n)
//
// 【思路】
// 树的结构天然是递归的：每个子树也是一棵树。
// 中序遍历 = 先遍历左子树 → 访问根 → 再遍历右子树。
// 递归三要素:
//   ① 返回值: void (结果通过引用参数收集)
//   ② 终止条件: 节点为空
//   ③ 单层逻辑: 递归左 → 处理当前 → 递归右
//
//  示例树 [1,null,2,3]:
//       1
//        \
//         2
//        /
//       3
//
//  递归展开:
//  inorder(1)
//    → inorder(null)          // 左子树为空，返回
//    → 访问 1                  // result = [1]
//    → inorder(2)
//        → inorder(3)
//            → inorder(null)  // 左空
//            → 访问 3          // result = [1, 3]
//            → inorder(null)  // 右空
//        → 访问 2              // result = [1, 3, 2]
//        → inorder(null)      // 右空
//  最终: [1, 3, 2] ✓
// ============================================================
class Solution1 {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(TreeNode* node, vector<int>& result) {
        if (!node) return;              // 终止条件：空节点无需处理
        dfs(node->left, result);        // 先处理整个左子树
        result.push_back(node->val);    // 中序位置：左子树处理完才轮到自己
        dfs(node->right, result);       // 最后处理右子树
    }
};


// ============================================================
// 解法2: 迭代（显式栈） — 面试主要考点 ⭐
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 递归的本质是系统帮你维护调用栈。迭代版就是自己维护这个栈。
//
// 核心模式（记住这个口诀）：
//   "一路向左压栈 → 弹出访问 → 转向右子树 → 重复"
//
// 为什么这就是中序？
// - "一路向左到底" = 保证左子树的所有节点先入栈（先处理）
// - "弹出时访问" = 左子树处理完了，轮到当前节点（根）
// - "转右" = 最后处理右子树
//
// 更大的示例 [1,2,3,4,5,null,8,null,null,6,7,9]:
//         1
//        / \
//       2   3
//      / \   \
//     4   5   8
//        / \ /
//       6  7 9
//
//  步骤   栈(底→顶)       curr    动作                    result
//  ──────────────────────────────────────────────────────────
//  init   []              1
//  S1     [1,2,4]         null    一路向左: push 1,2,4
//  S2     [1,2]           null    弹4, 访问4, 右=null      [4]
//  S3     [1]             5       弹2, 访问2, 右=5         [4,2]
//  S4     [1,5,6]         null    一路向左: push 5,6
//  S5     [1,5]           null    弹6, 访问6, 右=null      [4,2,6]
//  S6     [1]             7       弹5, 访问5, 右=7         [4,2,6,5]
//  S7     [1,7]           null    一路向左: push 7
//  S8     [1]             null    弹7, 访问7, 右=null      [4,2,6,5,7]
//  S9     []              3       弹1, 访问1, 右=3         [4,2,6,5,7,1]
//  S10    [3]             null    一路向左: push 3 (3无左)
//  S11    []              8       弹3, 访问3, 右=8         [4,2,6,5,7,1,3]
//  S12    [8,9]           null    一路向左: push 8, 8左=9, push 9
//                                 (注: 9是8的左孩子)
//  S13    [8]             null    弹9, 访问9, 右=null      [4,2,6,5,7,1,3,9]
//  S14    []              null    弹8, 访问8, 右=null      [4,2,6,5,7,1,3,9,8]
//  S15    栈空&curr=null          结束 ✓
// ============================================================
class Solution2 {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* curr = root;

        // 外层循环: curr非空(还有节点待探索) 或 栈非空(还有节点待处理)
        // 易错: 不能只写 !stk.empty()，初始栈空会直接跳过
        while (curr || !stk.empty()) {

            // 阶段1: 从curr出发，一路向左到底，沿途全部压栈
            // 这保证了左子树的节点会先被处理（先入后出，最左的最先弹出）
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }

            // 阶段2: 弹出栈顶 = 当前子树中最左的未访问节点
            curr = stk.top();
            stk.pop();
            result.push_back(curr->val);  // 中序位置: 左子树已处理完，访问当前节点

            // 阶段3: 转向右子树
            // 即使 curr->right 为 null 也没关系:
            // 下一轮外层循环 curr=null，跳过内层while，继续弹栈
            curr = curr->right;
        }

        return result;
    }
};


// ============================================================
// 解法3: Morris 遍历 — O(1) 空间，进阶加分
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 核心问题: 不用栈/递归，遍历完左子树后怎么回到父节点？
// Morris 的天才想法: 利用左子树最右节点（前驱）的空右指针，
// 临时建立一条指向当前节点的链接（线索），形成"回路"。
//
// 规则:
// 对于当前节点 curr:
//   Case A: curr 没有左子树
//     → 直接访问 curr，然后 curr = curr->right
//
//   Case B: curr 有左子树 → 找前驱 (左子树的最右节点)
//     B1: 前驱的右指针 == null (第一次来)
//         → 建线索: predecessor->right = curr
//         → curr = curr->left (进入左子树)
//     B2: 前驱的右指针 == curr (第二次来，说明左子树遍历完了)
//         → 断线索: predecessor->right = null (恢复树)
//         → 访问 curr
//         → curr = curr->right (进入右子树)
//
// 以 [1,null,2,3] 为例:
//       1            
//        \           
//         2          
//        /           
//       3            
//
// Step1: curr=1, 无左子树 → Case A
//        访问1, curr = 1->right = 2      result=[1]
// Step2: curr=2, 有左子树(3), 找前驱=3
//        3->right == null → Case B1
//        建线索: 3->right = 2
//        curr = 2->left = 3
// Step3: curr=3, 无左子树 → Case A
//        访问3, curr = 3->right = 2      result=[1,3]
//        (注: 3->right已被设为2，沿线索回到2)
// Step4: curr=2, 有左子树(3), 找前驱=3
//        3->right == 2 (即curr) → Case B2
//        断线索: 3->right = null
//        访问2, curr = 2->right = null   result=[1,3,2]
// Step5: curr=null → 结束 ✓
//
// 为什么时间是 O(n) 而不是 O(n²)?
// 虽然每个有左子树的节点都要找前驱（向右走），
// 但树中所有"找前驱"走过的边总和 ≤ 2n（每条边最多被走2次），
// 所以总时间仍然是 O(n)。
// ============================================================
class Solution3 {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode* curr = root;

        while (curr) {
            if (!curr->left) {
                // Case A: 没有左子树，直接访问并转右
                result.push_back(curr->val);
                curr = curr->right;
            } else {
                // Case B: 有左子树，找前驱节点
                TreeNode* predecessor = curr->left;
                // 一路向右找到左子树的最右节点
                // 注意: predecessor->right != curr 是为了避免沿着已建线索回到自己
                while (predecessor->right && predecessor->right != curr) {
                    predecessor = predecessor->right;
                }

                if (!predecessor->right) {
                    // Case B1: 第一次来，建立线索，进入左子树
                    predecessor->right = curr;  // 临时线索: 遍历完左子树后能回到curr
                    curr = curr->left;
                } else {
                    // Case B2: 第二次来(左子树已遍历完)，断线索，访问，转右
                    predecessor->right = nullptr;       // 恢复树结构(重要!)
                    result.push_back(curr->val);        // 中序位置访问
                    curr = curr->right;
                }
            }
        }

        return result;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 递归       | 迭代(栈)      | Morris      |
// |-------------|-----------|--------------|-------------|
// | 时间         | O(n)      | O(n)         | O(n)        |
// | 空间         | O(h)递归栈 | O(h)显式栈    | O(1)        |
// | 代码量       | 最少(3行)  | 中等(~15行)  | 较多(~25行) |
// | 修改树结构    | 否        | 否           | 是(临时)     |
// | 理解难度      | 简单      | 中等         | 较难        |
// | 面试推荐      | 热身      | ⭐首选       | 加分项       |
//
// 日常刷题: 递归最快
// 面试场景: 写递归 → 被追问 → 写迭代 → 被追问O(1) → 讲Morris思路
//
// 【易错点】
//
// 1. 迭代版外层 while 条件:
//    ✗ while (!stk.empty())           // 初始栈空，root都没处理
//    ✓ while (curr || !stk.empty())   // curr非空说明还有节点要探索
//
// 2. 迭代版弹出后忘记转右:
//    ✗ curr = stk.top(); stk.pop(); result.push_back(curr->val);
//      // 然后直接进入下一轮循环，curr还指向弹出的节点 → 死循环
//    ✓ 必须加 curr = curr->right; 即使right是null
//
// 3. Morris 找前驱时漏掉 predecessor->right != curr:
//    ✗ while (predecessor->right) predecessor = predecessor->right;
//      // 如果线索已建立，predecessor->right指向curr，会无限循环
//    ✓ while (predecessor->right && predecessor->right != curr)
//
// 4. Morris 忘记断线索:
//    ✗ 只建不断 → 树结构被永久修改
//    ✓ Case B2 必须 predecessor->right = nullptr
//
// 【面试追问】
//
// Q1: 递归空间复杂度是 O(n) 还是 O(h)? 什么时候 h=n?
// → O(h), 树退化为链表(每个节点只有一个孩子)时 h=n
//
// Q2: 迭代版的栈在模拟什么? 为什么一路向左就能保证中序?
// → 模拟递归调用栈。一路向左 = 递归不断进入左子树，
//   弹出 = 递归返回，转右 = 进入右子树。顺序恰好是左→根→右。
//
// Q3: 能否 O(1) 空间? 代价是什么?
// → Morris 遍历, O(1)空间。代价: 临时修改树结构(面试需提到)，
//   且不能在多线程环境使用。如果不允许修改树，最优只能 O(h)。
//
// Q4: 前序和后序的迭代怎么改?
// → 前序: 在入栈时(内层while中)就访问，而不是弹出时访问。
//   后序: 技巧是按"根→右→左"遍历(前序镜像)，最后reverse结果。
//   或者用双标记法/两个栈。
//
// Q5: 这道题的迭代框架还能用在哪?
// → LC 173 BST迭代器: next()就是弹栈+转右，构造时一路向左
//   LC 230 第K小: 中序遍历BST到第K个停下
//   LC 285 中序后继: 找到目标后返回下一个弹出的节点
// ============================================================
