/*
 * LeetCode 1028: 从先序遍历还原二叉树 (Recover a Tree From Preorder Traversal)
 *
 * 【题目本质】
 * 给定先序遍历的序列化字符串（破折号数量=深度，后跟节点值），还原二叉树。
 * 核心是：利用深度信息确定每个节点的父节点及左/右位置。
 *
 * 【解法总览】
 * 解法1: 迭代（栈）— O(n) / O(n) — 显式用栈维护祖先路径 ⭐面试首选
 * 解法2: 递归（DFS）— O(n) / O(n) — 利用递归调用栈隐式维护路径
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
// 解法1: 迭代（栈）— 显式维护从根到当前节点的祖先路径
// 时间: O(n)  空间: O(n)  （n 为字符串长度，节点数 ≤ n）
//
// 【思路】
// 先序遍历是"根→左→右"。按顺序扫描字符串，每次解析出 (depth, value)。
// 栈中始终维护"从根到当前正在构建的节点"的完整路径。
// 栈的大小 == 栈顶节点的深度 + 1。
//
// 当新节点深度为 d 时：
//   - 弹栈直到栈大小 == d（此时栈顶是深度 d-1 的父节点）
//   - 如果父节点左孩子为空 → 挂左
//   - 否则 → 挂右
//   - 将新节点入栈
//
// 【示例树 "1-2--3--4-5--6--7"】
//
//         1            解析过程:
//        / \           (0,1): push 1          栈: [1]
//       2   5          (1,2): 1.left=2, push  栈: [1,2]
//      / \ / \         (2,3): 2.left=3, push  栈: [1,2,3]
//     3  4 6  7        (2,4): pop3, 2.right=4 栈: [1,2,4]
//                      (1,5): pop4,pop2, 1.right=5 栈: [1,5]
//                      (2,6): 5.left=6, push  栈: [1,5,6]
//                      (2,7): pop6, 5.right=7 栈: [1,5,7]
// ============================================================
class Solution1 {
public:
    TreeNode* recoverFromPreorder(string traversal) {
        stack<TreeNode*> stk;
        int i = 0, n = traversal.size();
        TreeNode* root = nullptr;
        
        while (i < n) {
            // 第一步：数连续破折号 → 得到当前节点的深度
            int depth = 0;
            while (i < n && traversal[i] == '-') {
                depth++;
                i++;
            }
            
            // 第二步：读连续数字 → 得到节点值（可能是多位数，值可达 10^9）
            int val = 0;
            while (i < n && traversal[i] != '-') {
                val = val * 10 + (traversal[i] - '0');
                i++;
            }
            
            TreeNode* node = new TreeNode(val);
            
            // 记录根节点（深度为 0 的节点只出现一次）
            if (depth == 0) {
                root = node;
            }
            
            // 弹栈：把深度 >= depth 的节点弹出
            // 弹完后栈大小 == depth，栈顶就是深度 depth-1 的父节点
            while ((int)stk.size() > depth) {
                stk.pop();
            }
            
            // 将新节点挂到父节点上
            // 题目保证：如果只有一个孩子，一定是左孩子
            // 所以优先挂左，左非空才挂右
            if (!stk.empty()) {
                if (stk.top()->left == nullptr) {
                    stk.top()->left = node;
                } else {
                    stk.top()->right = node;
                }
            }
            
            // 新节点入栈，成为"当前路径"的最深节点
            stk.push(node);
        }
        
        return root;
    }
};


// ============================================================
// 解法2: 递归（DFS）— 利用先序遍历的递归结构自然还原
// 时间: O(n)  空间: O(h)  递归栈深度 = 树高 h
//
// 【思路】
// 先序遍历 = 根节点 + 左子树的先序遍历 + 右子树的先序遍历
// 这天然是一个递归定义。
//
// 递归函数 dfs(expectedDepth) 的语义：
//   "如果当前位置的节点深度 == expectedDepth，则消费它并递归构建子树；
//    否则返回 null（不消费任何字符）。"
//
// 核心技巧："偷看"机制
//   先用临时变量数破折号，如果深度不匹配就不移动全局指针 pos。
//   只有匹配了才正式消费字符串。
//
// 递归调用过程（示例1）:
//   dfs(0): 解析(0,1) → 创建节点1
//     dfs(1): 解析(1,2) → 创建节点2
//       dfs(2): 解析(2,3) → 创建节点3
//         dfs(3): 偷看深度2≠3 → 返回null
//         dfs(3): 偷看深度2≠3 → 返回null
//       → 3 的左右都是 null，返回节点3
//       dfs(2): 解析(2,4) → 创建节点4
//         dfs(3): 偷看深度1≠3 → 返回null
//         dfs(3): 偷看深度1≠3 → 返回null
//       → 返回节点4
//     → 2.left=3, 2.right=4，返回节点2
//     dfs(1): 解析(1,5) → 创建节点5
//       dfs(2): 解析(2,6) → 创建节点6
//         ...同理...
//       dfs(2): 解析(2,7) → 创建节点7
//     → 5.left=6, 5.right=7，返回节点5
//   → 1.left=2, 1.right=5，返回节点1（根）
// ============================================================
class Solution2 {
public:
    int pos = 0;  // 全局扫描指针，标记当前解析到字符串的哪个位置
    
    TreeNode* recoverFromPreorder(string traversal) {
        pos = 0;
        return dfs(traversal, 0);
    }
    
    TreeNode* dfs(const string& s, int expectedDepth) {
        // "偷看"：用临时变量数破折号，不动 pos
        int depth = 0;
        int tempPos = pos;
        while (tempPos < (int)s.size() && s[tempPos] == '-') {
            depth++;
            tempPos++;
        }
        
        // 深度不匹配 → 这个节点不属于当前期望的子树，返回 null
        // 关键：pos 没有移动，字符串没有被消费
        if (depth != expectedDepth) {
            return nullptr;
        }
        
        // 深度匹配，正式消费：移动 pos 跳过破折号
        pos = tempPos;
        
        // 读取节点值（注意多位数）
        int val = 0;
        while (pos < (int)s.size() && s[pos] != '-') {
            val = val * 10 + (s[pos] - '0');
            pos++;
        }
        
        TreeNode* node = new TreeNode(val);
        
        // 先序遍历递归：先左后右
        // 子节点的期望深度是当前深度 + 1
        node->left = dfs(s, expectedDepth + 1);
        node->right = dfs(s, expectedDepth + 1);
        
        return node;
    }
};


// ============================================================
// 解法2b: 递归（DFS）— 不用成员变量，通过引用传递 pos
// 和解法2完全相同的逻辑，但 pos 通过引用传递，更符合函数式风格
// ============================================================
class Solution2b {
public:
    TreeNode* recoverFromPreorder(string traversal) {
        int pos = 0;
        return dfs(traversal, 0, pos);
    }
    
    TreeNode* dfs(const string& s, int expectedDepth, int& pos) {
        // 偷看深度
        int depth = 0;
        int tempPos = pos;
        while (tempPos < (int)s.size() && s[tempPos] == '-') {
            depth++;
            tempPos++;
        }
        
        if (depth != expectedDepth) {
            return nullptr;
        }
        
        pos = tempPos;
        
        int val = 0;
        while (pos < (int)s.size() && s[pos] != '-') {
            val = val * 10 + (s[pos] - '0');
            pos++;
        }
        
        TreeNode* node = new TreeNode(val);
        node->left = dfs(s, expectedDepth + 1, pos);
        node->right = dfs(s, expectedDepth + 1, pos);
        return node;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 迭代（栈）      | 递归（DFS）        |
// |-------------|----------------|-------------------|
// | 核心数据结构 | 显式栈          | 隐式递归调用栈      |
// | 代码量       | 略多            | 更简洁             |
// | 可读性       | 栈操作直观       | 递归逻辑优雅       |
// | 调试难度     | 较容易（可打印栈）| 需要理解递归展开    |
// | 面试推荐     | ⭐ 容易解释      | ⭐ 写起来快         |
//
// 两者本质相同：都是按先序顺序扫描，用深度信息确定父子关系。
// 栈版本在"弹栈回退"时更直观；递归版本在"自动匹配深度"时更优雅。
//
// ============================================================
// 【易错点】
//
// 1. 节点值是多位数（值可达 10^9）：
//    ✗ val = s[i] - '0';  // 只读了一位
//    ✓ while (pos < n && s[pos] != '-') val = val*10 + (s[pos++]-'0');
//
// 2. 递归版本直接移动 pos 而不是先"偷看"：
//    ✗ 直接数破折号移动 pos，发现不匹配后无法回退
//    ✓ 用 tempPos 偷看，匹配了才 pos = tempPos
//
// 3. 迭代版本中 stk.size() 返回 size_t (无符号整数)：
//    ✗ stk.size() > depth  // 当 depth 为负时（虽然此题不会）比较出错
//    ✓ (int)stk.size() > depth  // 显式转换更安全
//
// 4. 迭代版本返回根节点：
//    ✗ return stk.top();  // 栈顶是最后处理的叶子，不是根
//    ✓ 在 depth==0 时记录 root，或者最后弹栈到底
//
// 5. 忘记处理只有根节点的情况（字符串 "1"，没有任何破折号）：
//    → 解析得到 depth=0, val=1，两种解法都能正确处理
//
// ============================================================
// 【面试追问】
//
// Q1: 字符串解析有更简洁的写法吗？
//     → 可以用 sscanf 或 stoi + find，但手动解析更高效且面试中更常见。
//        关键就是两个 while 循环：一个数'-'，一个读数字。
//
// Q2: 如果改成后序遍历的序列化，怎么还原？
//     → 后序是"左→右→根"，需要从字符串末尾开始往前解析（变成"根→右→左"），
//        然后用类似的栈方法，但要先挂右再挂左。
//
// Q3: 如果这是 N 叉树（每个节点可以有任意多个孩子），怎么改？
//     → 栈版本：不再是"左空挂左否则挂右"，而是 children.push_back(node)。
//        递归版本：while 循环调用 dfs(expectedDepth+1) 直到返回 null。
//
// Q4: 时间复杂度能比 O(n) 更好吗？
//     → 不能，因为必须读取整个字符串的每个字符，O(n) 已经是下界。
// ============================================================
