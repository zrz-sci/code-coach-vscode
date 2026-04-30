/*
 * LeetCode 257: 二叉树的所有路径 (Binary Tree Paths)
 *
 * 【题目本质】
 * 遍历二叉树，收集所有从根到叶子的路径，用 "val1->val2->..." 格式输出。
 * 本质是树的 DFS 遍历 + 路径记录。
 *
 * 【解法总览】
 * 解法1: DFS 递归（传字符串）     — O(n·h) / O(h)   — 最简洁，面试首选
 * 解法2: DFS 递归（显式回溯）     — O(n·h) / O(h)   — 回溯模板标准写法
 * 解法3: DFS 迭代（栈）           — O(n·h) / O(n·h) — 追问"不用递归"
 * 解法4: BFS 迭代（队列）         — O(n·h) / O(n·h) — 展示 BFS 也能做
 */

// 示例树:
//       1
//      / \
//     2   3
//      \
//       5
//
// DFS 遍历过程（前序）:
// Step1: 访问1, path="1"
// Step2: 访问2, path="1->2"
// Step3: 访问5, path="1->2->5", 叶子! → 收集
// Step4: 回到1, 访问3, path="1->3", 叶子! → 收集
// 结果: ["1->2->5", "1->3"]

// ============================================================
// 解法1: DFS 递归（传字符串）— 隐式回溯
// 时间: O(n·h)  空间: O(h) 递归栈深度
//
// 【思路】
// 最自然的想法：DFS 遍历树，把"到当前节点的路径"作为参数传下去。
// 字符串按值传递，每层递归拿到的是独立副本，不需要手动回溯。
// 到叶子节点时，path 就是完整路径，直接收集。
//
// 为什么按值传递就不需要回溯？
// 因为 C++ 值传递会拷贝，左子树调用时修改的 path 不影响右子树调用时的 path。
// 函数返回后，调用者的 path 还是原来的值——这就是"隐式回溯"。
// ============================================================
class Solution1 {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        if (root) dfs(root, "", result);
        return result;
    }
    
    // path 按值传递 → 隐式回溯
    void dfs(TreeNode* node, string path, vector<string>& result) {
        // 拼接当前节点（先判断是否需要加箭头）
        if (!path.empty()) path += "->";
        path += to_string(node->val);
        
        // 叶子节点：收集完整路径
        if (!node->left && !node->right) {
            result.push_back(path);
            return;
        }
        
        // 非叶子：递归进入子树
        // 因为 path 是值传递，左子树的调用不影响右子树
        if (node->left)  dfs(node->left, path, result);
        if (node->right) dfs(node->right, path, result);
    }
};

// ============================================================
// 解法2: DFS 递归（显式回溯）— 标准回溯模板
// 时间: O(n·h)  空间: O(h) 递归栈 + 路径数组
//
// 【思路】
// 用 vector<int> 维护当前路径，按引用传递。
// 每进入一个节点：push_back（做选择）
// 递归返回后：pop_back（撤销选择/回溯）
// 到叶子时把路径数组转成字符串收集。
//
// 这就是经典回溯三步：
// 1. 做选择 → path.push_back(node->val)
// 2. 递归探索
// 3. 撤销选择 → path.pop_back()
//
// 决策树:
//              [1]           ← push 1
//             /    \
//          [1,2]   [1,3]    ← push 2 / push 3
//            |       ↑ 叶子，收集 "1->3"，pop 3
//         [1,2,5]
//           ↑ 叶子，收集 "1->2->5"，pop 5
//         回到 [1,2]，pop 2
//         回到 [1]，进入右子树
// ============================================================
class Solution2 {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        vector<int> path;
        if (root) backtrack(root, path, result);
        return result;
    }
    
    void backtrack(TreeNode* node, vector<int>& path, vector<string>& result) {
        path.push_back(node->val);       // 做选择
        
        if (!node->left && !node->right) {
            // 叶子：将 path 数组转为 "v1->v2->v3" 格式
            result.push_back(buildPath(path));
        } else {
            // 非叶子：继续递归
            if (node->left)  backtrack(node->left, path, result);
            if (node->right) backtrack(node->right, path, result);
        }
        
        path.pop_back();                 // 撤销选择（回溯！）
    }
    
    string buildPath(const vector<int>& path) {
        string s;
        for (int i = 0; i < (int)path.size(); i++) {
            if (i > 0) s += "->";
            s += to_string(path[i]);
        }
        return s;
    }
};

// ============================================================
// 解法3: DFS 迭代（栈模拟递归）
// 时间: O(n·h)  空间: O(n·h) 栈中每个元素存一份路径字符串
//
// 【思路】
// 面试追问"不用递归怎么做"时的解法。
// 用栈模拟 DFS，每个栈元素是 {节点, 从根到该节点的路径字符串}。
// 弹出栈顶时，如果是叶子就收集；否则把子节点连同更新后的路径一起入栈。
//
// 栈的变化过程 (以示例树为例):
// 初始:  stack = [{1, "1"}]
// Pop 1: 非叶, push {3,"1->3"}, {2,"1->2"}
// Pop 2: 非叶, push {5,"1->2->5"}
// Pop 5: 叶子! 收集 "1->2->5"
// Pop 3: 叶子! 收集 "1->3"
// ============================================================
class Solution3 {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        if (!root) return result;
        
        stack<pair<TreeNode*, string>> stk;
        stk.push({root, to_string(root->val)});
        
        while (!stk.empty()) {
            auto [node, path] = stk.top();
            stk.pop();
            
            // 叶子节点：收集路径
            if (!node->left && !node->right) {
                result.push_back(path);
                continue;
            }
            
            // 先压右再压左 → 弹出顺序是先左后右（模拟前序遍历）
            if (node->right) {
                stk.push({node->right, path + "->" + to_string(node->right->val)});
            }
            if (node->left) {
                stk.push({node->left, path + "->" + to_string(node->left->val)});
            }
        }
        return result;
    }
};

// ============================================================
// 解法4: BFS 迭代（队列）
// 时间: O(n·h)  空间: O(n·h) 队列中每个元素存一份路径字符串
//
// 【思路】
// 和解法3 几乎相同，只是把栈换成队列，遍历顺序从 DFS 变成 BFS。
// 层序遍历，每个队列元素是 {节点, 路径}。
// 遇到叶子就收集，否则子节点入队。
//
// 队列变化过程:
// 初始:  queue = [{1, "1"}]
// Dequeue 1: 非叶, enqueue {2,"1->2"}, {3,"1->3"}
// Dequeue 2: 非叶, enqueue {5,"1->2->5"}
// Dequeue 3: 叶子! 收集 "1->3"
// Dequeue 5: 叶子! 收集 "1->2->5"
// ============================================================
class Solution4 {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        if (!root) return result;
        
        queue<pair<TreeNode*, string>> q;
        q.push({root, to_string(root->val)});
        
        while (!q.empty()) {
            auto [node, path] = q.front();
            q.pop();
            
            if (!node->left && !node->right) {
                result.push_back(path);
                continue;
            }
            
            if (node->left) {
                q.push({node->left, path + "->" + to_string(node->left->val)});
            }
            if (node->right) {
                q.push({node->right, path + "->" + to_string(node->right->val)});
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 回溯方式 | 空间 | 代码复杂度 | 适用场景 |
// |------|---------|------|-----------|---------|
// | 解法1 传字符串   | 隐式(值传递) | O(h) 递归栈      | 最简洁 | 面试首选 |
// | 解法2 显式回溯   | 显式(pop)   | O(h) 递归栈      | 中等   | 展示回溯模板 |
// | 解法3 迭代栈     | 不需要      | O(n·h) 栈存路径  | 中等   | 追问不用递归 |
// | 解法4 BFS       | 不需要      | O(n·h) 队列存路径 | 中等   | 展示 BFS 也能做 |
//
// 解法1 vs 解法2:
//   解法1 利用值传递自动回溯，代码更短；
//   解法2 是标准回溯模板，更适合推广到 Path Sum II 等需要数组路径的题。
//
// 解法3 vs 解法4:
//   逻辑几乎相同，只是数据结构不同（栈 vs 队列），遍历顺序不同（DFS vs BFS）。
//
// ============================================================
// 【易错点】
//
// 1. ✗ 用 nullptr 作为叶子判断:
//      if (!node) { result.push_back(path); }
//    → 叶子节点的两个空孩子各触发一次，导致同一条路径被收集两次。
//    ✓ 正确: if (!node->left && !node->right) { result.push_back(path); }
//
// 2. ✗ 根节点前多了箭头:
//      path += "->" + to_string(node->val);  // 根节点会产生 "->1"
//    ✓ 正确: 先判断 path 是否为空，空则不加箭头。
//
// 3. ✗ 回溯版漏掉 pop_back():
//      path.push_back(node->val);
//      backtrack(left); backtrack(right);
//      // 忘了 pop_back → path 会一直增长
//    ✓ 正确: push_back 和 pop_back 必须配对。
//
// 4. ✗ 迭代版入栈时用当前节点的值而不是子节点的值:
//      stk.push({node->left, path + "->" + to_string(node->val)});
//    ✓ 正确: 应该拼接 node->left->val（子节点的值）。
//
// ============================================================
// 【面试追问】
//
// Q1: "时间复杂度为什么不是 O(n)？"
//    → 每个节点访问一次是 O(n)，但到叶子时要拷贝/构建路径字符串，
//      路径长度最长为 h（树高），叶子数最多 n/2，
//      所以总时间 O(n + 叶子数 × h) = O(n·h)。
//      平衡树: O(n·log n)，退化链表: O(n²)。
//
// Q2: "字符串按值传递有拷贝开销，怎么优化？"
//    → 解法2 用 vector<int> 传引用 + 回溯，减少字符串拷贝。
//      或者用 string 传引用 + 手动 erase 回溯（但 erase 也是 O(k)）。
//      在这道题的数据规模（n≤100）下，优化意义不大。
//
// Q3: "如果要求所有路径的和（不是打印路径）？"
//    → 传一个 int 参数 currentSum，每层加 node->val，叶子时累加到总和。
//      参考 129. 求根节点到叶节点数字之和。
//
// Q4: "如果要求路径和等于某个 target 的所有路径？"
//    → 在叶子处加判断 currentSum == target 再收集。
//      就是 113. Path Sum II，直接复用解法2 的回溯框架。
// ============================================================
