/*
 * LeetCode 199: 二叉树的右视图 (Binary Tree Right Side View)
 *
 * 【题目本质】
 * 求二叉树每一层的最右边节点。"右视图"是形象化说法，
 * 算法上就是：按层收集每层最后一个（或DFS中每层首次从右侧访问到的）节点。
 *
 * 【解法总览】
 * 解法1: BFS 层序遍历 — O(n) / O(w)  — 最直觉，面试首选
 * 解法2: DFS 先右后左 — O(n) / O(h)  — 递归方式，面试追问
 * 解法3: DFS 先左后右(覆盖法) — O(n) / O(h) — 帮助理解
 *
 *        示例1的树:             示例2的树:
 *            1                      1
 *           / \                    / \
 *          2   3                  2   3
 *           \   \                /
 *            5   4              4
 *                              /
 *  右视图: [1,3,4]            5
 *                         右视图: [1,3,4,5]
 *
 *  注意: 右视图不等于"只看右子树"！
 *  示例2中第2、3层的可见节点 4 和 5 都来自左子树。
 */

// ============================================================
// 解法1: BFS 层序遍历 — 每层取最后一个节点
// 时间: O(n)  空间: O(w)，w 为树的最大宽度
//
// 【思路】
// "右视图 = 每层最后一个节点" → 层序遍历天然按层处理。
// 用队列做 BFS，每层用 size 控制循环次数，
// 当 i == size-1 时就是该层最右的节点，收入结果。
//
// BFS 执行过程 (示例1):
//
//     队列状态              操作                    result
//     [1]                  层大小=1                 
//       弹出1(i=0=size-1)  → 收集1                 [1]
//       入队: 2, 3
//     [2, 3]               层大小=2
//       弹出2(i=0)         → 跳过
//       弹出3(i=1=size-1)  → 收集3                 [1, 3]
//       入队: 5, 4
//     [5, 4]               层大小=2
//       弹出5(i=0)         → 跳过
//       弹出4(i=1=size-1)  → 收集4                 [1, 3, 4]
//     []                   结束
// ============================================================
class Solution1 {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            // 必须在 for 循环之前保存当前层大小
            // 因为循环体内会 push 下一层节点导致 q.size() 变化
            int size = q.size();
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 每层最后一个节点就是右视图能看到的
                if (i == size - 1) {
                    result.push_back(node->val);
                }
                
                // 入队顺序: 先左后右（保证右边在后面，被最后处理）
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return result;
    }
};

// ============================================================
// 解法2: DFS 先右后左 — 每层第一个访问到的就是最右节点
// 时间: O(n)  空间: O(h)，h 为树高（递归栈深度）
//
// 【思路】
// 换个角度：如果 DFS 时优先走右子树，那么同一层中
// 右边的节点一定先被访问。怎么判断"该层还没收集过"？
// 
// 巧妙利用 result.size()：它恰好等于"已收集了几层的结果"。
// 当 depth == result.size() 时，说明 depth 这一层还没有结果，
// 当前节点就是该层最先被访问到的（因为先右后左，所以是最右的）。
//
// DFS 执行过程 (示例2):
//
//     调用                        depth  result.size()  动作
//     dfs(1, 0)                   0      0              收集1  → [1]
//       dfs(3, 1)                 1      1              收集3  → [1,3]
//         dfs(null, 2)            —      —              返回
//         dfs(null, 2)            —      —              返回
//       dfs(2, 1)                 1      2              跳过（1≠2）
//         dfs(null, 2)            —      —              返回
//         dfs(4, 2)               2      2              收集4  → [1,3,4]
//           dfs(null, 3)          —      —              返回
//           dfs(5, 3)             3      3              收集5  → [1,3,4,5]
// ============================================================
class Solution2 {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<int>& result) {
        if (!node) return;
        
        // result.size() == 已收集的层数
        // depth == result.size() 意味着该层首次被访问
        if (depth == static_cast<int>(result.size())) {
            result.push_back(node->val);
        }
        
        // 关键：先右后左！保证每层最右边的节点最先被访问
        // 如果改成先左后右，就变成了"左视图"
        dfs(node->right, depth + 1, result);
        dfs(node->left,  depth + 1, result);
    }
};

// ============================================================
// 解法3: DFS 先左后右（覆盖法）
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 如果用正常的先左后右 DFS，同一层先访问左边再右边。
// 每次访问都覆盖 result[depth] 的值，最终留下的就是最右的。
// 
// 与解法2对比：解法2只在每层第一次时 push_back，不做多余操作；
// 解法3每次都覆盖，多了无用赋值。但这种方式帮助理解：
// "先右后左"的优势就是避免了多余覆盖。
// ============================================================
class Solution3 {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<int>& result) {
        if (!node) return;
        
        if (depth == static_cast<int>(result.size())) {
            // 该层还没有记录，新增
            result.push_back(node->val);
        } else {
            // 该层已有记录，覆盖为更靠右的值
            result[depth] = node->val;
        }
        
        // 先左后右：同一层后访问的（右边）会覆盖先访问的（左边）
        dfs(node->left,  depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间  | 空间   | 思路               | 面试    |
// |--------------|-------|--------|--------------------|---------|
// | BFS 层序遍历  | O(n)  | O(w)   | 每层取最后一个       | ⭐ 首选 |
// | DFS 先右后左  | O(n)  | O(h)   | 每层取首次访问的     | ⭐ 追问 |
// | DFS 先左后右  | O(n)  | O(h)   | 覆盖，留最后一个     | 理解用  |
//
// 空间对比：
// - 完全二叉树: h=log n, w≈n/2 → DFS 更优
// - 链状树:     h=n,     w=1   → BFS 更优
// - 一般情况:   两者都是 O(n) 最坏
//
// 【易错点】
//
// 1. 只递归右子树就返回 — 错！
//    ✗ 只走 root->right，遇到 null 就停
//    ✓ 必须遍历整棵树；左子树深处的节点也可能在右视图中
//    反例: 树 [1,2,null]，右子树为空，但右视图是 [1,2]
//
// 2. BFS 中 size 在循环条件里动态取
//    ✗ for (int i = 0; i < q.size(); i++) — 每次 q.size() 都在变
//    ✓ int size = q.size(); for (int i = 0; i < size; i++)
//
// 3. DFS 先右后左的递归顺序写反
//    ✗ dfs(node->left, ...); dfs(node->right, ...);  → 这是左视图
//    ✓ dfs(node->right, ...); dfs(node->left, ...);  → 右视图
//
// 4. depth 和 result.size() 比较时的类型问题
//    depth 是 int，result.size() 是 size_t (unsigned)
//    当 depth 为负数时（不会发生但好习惯）可能出问题
//    建议显式转换: depth == static_cast<int>(result.size())
//
// 【面试追问】
//
// Q1: 这道题和层序遍历的关系？
//     右视图 = 层序遍历每层最后一个元素。
//     左视图 = 层序遍历每层第一个元素。
//     本质上都是"按层处理"的变体。
//
// Q2: BFS 和 DFS 哪个空间更优？
//     取决于树的形状。完全二叉树 DFS 更优(O(log n) vs O(n/2))；
//     链状树 BFS 更优(O(1) vs O(n))。面试中可以两种都写。
//
// Q3: 如果改成左视图怎么做？
//     BFS: 取每层第一个（i==0）。
//     DFS: 先左后右，depth==result.size() 时收集（解法2改递归顺序）。
//
// Q4: 如果要求"二叉树的俯视图(top view)"或"底视图(bottom view)"？
//     引入水平距离(horizontal distance)，根为0，左-1，右+1。
//     BFS 逐层遍历，对每个节点记录其列号。
//     俯视图: 每列第一次出现的值; 底视图: 每列最后一次出现的值。
//     用 map<int, int> 按列号记录即可。
// ============================================================
