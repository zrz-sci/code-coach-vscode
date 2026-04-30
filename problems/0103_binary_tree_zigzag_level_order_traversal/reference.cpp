/*
 * LeetCode 103: 二叉树的锯齿形层序遍历 (Binary Tree Zigzag Level Order Traversal)
 *
 * 【题目本质】
 * 标准 BFS 层序遍历 + 交替方向控制。
 * 本质：102题（层序遍历）的变体，核心是处理奇数层的反向输出。
 *
 * 【示例树结构】
 *         3           Level 0: 左→右  → [3]
 *        / \
 *       9   20        Level 1: 右→左  → [20, 9]
 *          / \
 *         15   7      Level 2: 左→右  → [15, 7]
 *
 * 结果: [[3], [20,9], [15,7]]
 *
 * 【解法总览】
 * 解法1: BFS + Reverse         — O(n) / O(n) — 最直觉，102题改两行
 * 解法2: BFS + Deque方向控制    — O(n) / O(n) — 面试首选，无额外reverse
 * 解法3: 双栈交替              — O(n) / O(n) — 栈的LIFO天然反向
 * 解法4: DFS 递归              — O(n) / O(n) — 递归角度，展示递归vs迭代
 */


// ============================================================
// 解法1: BFS + Reverse — 标准层序遍历后对奇数层反转
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 如果你会写 102 题（普通层序遍历），这个解法只需要加两行：
// 判断当前层号是否为奇数，如果是就 reverse。
//
// 为什么 reverse 不影响总复杂度？
// 所有层的节点数加起来 = n，所以所有 reverse 加起来也是 O(n)。
//
// 遍历过程：
//         3
//        / \
//       9   20
//          / \
//         15   7
//
// Level 0: queue=[3]     → 弹出3, 子节点[9,20]入队 → level=[3]      (偶数层，不反转)
// Level 1: queue=[9,20]  → 弹出9,20 → level=[9,20] → reverse → [20,9] (奇数层，反转)
// Level 2: queue=[15,7]  → 弹出15,7 → level=[15,7]  (偶数层，不反转)
// ============================================================
class Solution1 {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;  // 易错：忘记判空
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            // 关键：result.size() 是当前层号（因为还没 push_back）
            // 第0层不反转，第1层反转，第2层不反转...
            // 所以 result.size() 为奇数时反转
            if (result.size() % 2 == 1) {
                reverse(level.begin(), level.end());
            }
            
            result.push_back(level);
        }
        return result;
    }
};

// ============================================================
// 解法2: BFS + Deque 方向控制 — 面试首选
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1的 reverse 虽然不影响复杂度，但面试官可能追问"能不能不 reverse"。
//
// 核心观察：BFS 队列里的遍历顺序不需要变（始终先左后右入队），
// 只需要改"收集节点值时的插入方向"：
//   - 偶数层(左→右)：push_back
//   - 奇数层(右→左)：push_front
//
// deque 的 push_front 是 O(1)，比 vector::insert(begin()) 的 O(n) 高效。
//
// 过程示意：
//   Level 0: leftToRight=true
//     弹出3 → deque.push_back(3) → deque=[3]
//
//   Level 1: leftToRight=false
//     弹出9  → deque.push_front(9)  → deque=[9]
//     弹出20 → deque.push_front(20) → deque=[20,9]  ← 20在前面！
//
//   Level 2: leftToRight=true
//     弹出15 → deque.push_back(15) → deque=[15]
//     弹出7  → deque.push_back(7)  → deque=[15,7]
// ============================================================
class Solution2 {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;
        
        while (!q.empty()) {
            int size = q.size();
            deque<int> levelDeque;
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 根据当前方向决定插入位置
                if (leftToRight) {
                    levelDeque.push_back(node->val);   // 正常追加到末尾
                } else {
                    levelDeque.push_front(node->val);  // 插入到头部，实现反向
                }
                
                // 子节点入队顺序永远不变：先左后右
                // 这保证了下一层在队列中的顺序始终是从左到右
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(vector<int>(levelDeque.begin(), levelDeque.end()));
            leftToRight = !leftToRight;  // 方向交替
        }
        return result;
    }
};

// ============================================================
// 解法2b: BFS + 下标控制（不用 deque 的变体）
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 有些面试官可能不希望用 deque。可以预分配 vector，
// 用下标控制写入位置：
//   leftToRight: 第i个元素写到 level[i]
//   rightToLeft: 第i个元素写到 level[size-1-i]
// ============================================================
class Solution2b {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        bool leftToRight = true;
        
        while (!q.empty()) {
            int size = q.size();
            vector<int> level(size);  // 预分配固定大小
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 关键：通过下标控制方向
                int idx = leftToRight ? i : (size - 1 - i);
                level[idx] = node->val;
                
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(level);
            leftToRight = !leftToRight;
        }
        return result;
    }
};

// ============================================================
// 解法3: 双栈交替 — 利用栈的 LIFO 天然实现反向
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 用两个栈 s1 和 s2 交替工作：
//   - s1 负责"左→右"的层：弹出节点时先左后右压入 s2
//   - s2 负责"右→左"的层：弹出节点时先右后左压入 s1
//
// 为什么这样能 work？因为栈是后进先出：
//   s1 弹出顺序 = 左→右
//   压入s2时先左后右 → s2弹出就是右→左 ✓
//   s2 弹出顺序 = 右→左
//   压入s1时先右后左 → s1弹出就是左→右 ✓
//
// 详细过程：
//         3
//        / \
//       9   20
//          / \
//         15   7
//
// 初始: s1=[3], s2=[]
//
// 处理 s1 (左→右层):
//   弹出 3 → 输出 3
//     压入 s2: 先左(9)后右(20) → s2=[9,20]
//   level=[3]
//
// 处理 s2 (右→左层):
//   弹出 20 → 输出 20
//     压入 s1: 先右(7)后左(15) → s1=[7,15]
//   弹出 9 → 输出 9
//     (无子节点)
//   level=[20,9]
//
// 处理 s1 (左→右层):
//   弹出 15 → 输出 15
//   弹出 7  → 输出 7
//   level=[15,7]
// ============================================================
class Solution3 {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        stack<TreeNode*> s1, s2;
        s1.push(root);
        
        while (!s1.empty() || !s2.empty()) {
            vector<int> level;
            
            if (!s1.empty()) {
                // 当前层：左→右（从 s1 弹出）
                while (!s1.empty()) {
                    TreeNode* node = s1.top(); s1.pop();
                    level.push_back(node->val);
                    // 先左后右压入 s2 → s2 弹出时变成右→左
                    if (node->left)  s2.push(node->left);
                    if (node->right) s2.push(node->right);
                }
            } else {
                // 当前层：右→左（从 s2 弹出）
                while (!s2.empty()) {
                    TreeNode* node = s2.top(); s2.pop();
                    level.push_back(node->val);
                    // 先右后左压入 s1 → s1 弹出时变成左→右
                    if (node->right) s1.push(node->right);
                    if (node->left)  s1.push(node->left);
                }
            }
            
            result.push_back(level);
        }
        return result;
    }
};

// ============================================================
// 解法4: DFS 递归 — 递归按层收集，方向控制
// 时间: O(n)  空间: O(n)（递归栈深度 O(h)，结果空间 O(n)）
//
// 【思路】
// 和 BFS 不同的角度：用 DFS 前序遍历整棵树，
// 每个节点带着自己的层号 depth。
// 根据 depth 的奇偶决定往 result[depth] 的前面还是后面插。
//
// 注意：vector::insert(begin(), val) 是 O(k) 操作，
// 如果追求效率可以先 push_back，最后对奇数层 reverse。
//
// DFS 遍历顺序（前序: 根→左→右）:
//         3
//        / \
//       9   20
//          / \
//         15   7
//
// 访问顺序: 3(depth=0) → 9(depth=1) → 20(depth=1) → 15(depth=2) → 7(depth=2)
//
// depth=0 (偶数层): push_back(3)           → [3]
// depth=1 (奇数层): insert(begin,9)        → [9]
//                   insert(begin,20)       → [20,9]   ← 20插到9前面
// depth=2 (偶数层): push_back(15)          → [15]
//                   push_back(7)           → [15,7]
// ============================================================
class Solution4 {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<vector<int>>& result) {
        if (!node) return;
        
        // 第一次到达新层，创建空 vector
        if (depth >= (int)result.size()) {
            result.push_back({});
        }
        
        // 偶数层正常追加，奇数层头部插入
        if (depth % 2 == 0) {
            result[depth].push_back(node->val);
        } else {
            // O(k) 插入，不如 deque 高效，但逻辑清晰
            result[depth].insert(result[depth].begin(), node->val);
        }
        
        // 先左后右递归，保证同层节点的 DFS 访问顺序是从左到右
        dfs(node->left,  depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 核心思想         | 代码量 | 额外操作       | 面试推荐 |
// |------------|-----------------|--------|---------------|---------|
// | BFS+Reverse| 标准BFS后处理    | 最少   | reverse O(k)/层| 先说     |
// | BFS+Deque  | BFS中控制方向    | 适中   | 无             | ⭐主力写 |
// | 双栈交替    | 栈LIFO天然反向   | 稍多   | 无             | 追问时说 |
// | DFS递归    | 递归按层收集      | 适中   | insert O(k)/层 | 展示能力 |
//
// 面试策略：
//   1. 先用30秒说"可以标准BFS+reverse"，展示你理解题意
//   2. 然后写 BFS+Deque（解法2或2b），展示优化能力
//   3. 如果面试官问"还有其他方式"，说双栈或DFS
//
// 【易错点】
//
// 1. 层号判断搞反：
//    ✗ result.size() % 2 == 0 时 reverse
//      → 第0层是偶数，不需要反转！第0层左→右是正常顺序
//    ✓ result.size() % 2 == 1 时 reverse
//      → 第1、3、5层（奇数层）需要反转
//
// 2. 双栈解法中子节点压栈顺序搞反：
//    ✗ s1弹出(左→右)时，子节点先右后左压入s2
//      → s2弹出变成左→右，没有实现锯齿！
//    ✓ s1弹出(左→右)时，子节点先左后右压入s2
//      → s2弹出变成右→左 ✓
//
// 3. 忘记 root 判空：
//    ✗ 直接 q.push(root) → root=nullptr 时崩溃
//    ✓ 开头 if (!root) return result;
//
// 4. DFS 解法中 depth 和 result.size() 比较时的类型问题：
//    ✗ depth >= result.size() → depth是int, size()是size_t(unsigned)
//      当 depth=0, result 为空时没问题，但加上 (int) 转换更安全
//    ✓ depth >= (int)result.size()
//
// 【面试追问 — 递进链】
//
// Q1: 这和 102 题有什么区别？你改了哪里？
//     → BFS 框架完全相同，只在收集值时加了方向控制。
//       具体改动：加一个 bool leftToRight，偶数层 push_back，奇数层 push_front。
//
// Q2: reverse 和 deque 方案哪个更好？
//     → 渐进复杂度相同。reverse 方案更简洁不易出错；
//       deque 方案避免了不必要的数据移动，常数更优。
//       面试中先说 reverse，再优化到 deque。
//
// Q3: 如果要求 O(1) 额外空间（不算输出），能做到吗？
//     → 不能。BFS 需要队列存储当前层节点，DFS 需要递归栈。
//       最少也需要 O(w) 空间（w 是树的最大宽度）。
//
// Q4: 如果改成"每隔两层反转一次"或者"前3层正常，后面锯齿"，你怎么改？
//     → 只需修改方向判断条件：把 `depth % 2` 改成对应的逻辑即可。
//       框架完全不变，体现了"收集逻辑"和"遍历逻辑"分离的设计优势。
// ============================================================
