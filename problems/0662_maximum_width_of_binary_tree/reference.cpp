/*
 * LeetCode 662: Maximum Width of Binary Tree (二叉树最大宽度)
 * 
 * 【题目本质】
 * 给二叉树节点分配"完全二叉树编号"，每层宽度 = 最右编号 - 最左编号 + 1，求所有层的最大宽度。
 *
 * 【解法总览】
 * 解法1: BFS + 节点编号 — O(n) / O(n) — 面试首选，直觉清晰
 * 解法2: DFS + 节点编号 — O(n) / O(h) — 空间更优，适合追问
 *
 * 【关键思路：完全二叉树编号】
 * 如果根节点编号为 0:
 *   - 左孩子编号 = 2 * parent_idx
 *   - 右孩子编号 = 2 * parent_idx + 1
 * 
 * 这样每层的宽度 = rightmost_idx - leftmost_idx + 1（含中间的 null）
 *
 * 【树结构示意 (示例1)】
 *          1 (idx=0)
 *         / \
 *        3   2          Level 1: idx=[0,1]  宽度=2
 *       / \   \
 *      5   3   9        Level 2: idx=[0,1,3] 宽度=3-0+1=4
 *     (0) (1) (3)
 *              ↑
 *          位置2是null，但编号算在宽度内
 */


// ============================================================
// 解法1: BFS + 节点编号 — 层序遍历，每层取首尾编号
// 时间: O(n)  空间: O(n)（队列最多存一层的节点）
// 
// 【思路】
// 宽度 = 同层最右 - 最左 + 1，需要知道每层所有节点的"位置编号"。
// BFS 天然按层遍历，给每个节点带上编号，每层处理时取首尾即可。
//
// 为什么需要归一化？
// 树深度可达3000，编号 2^3000 远超任何整型。
// 但宽度只关心同层内的差值，每层把编号减去该层最小编号，
// 差值不变，数值却从0开始，不会溢出。
//
// 【BFS 过程示意 (示例1)】
// Level 0: queue = [(1, idx=0)]
//          width = 0 - 0 + 1 = 1
//          minIdx = 0, 归一化后 idx=0
//          入队: (3, 2*0=0), (2, 2*0+1=1)
//
// Level 1: queue = [(3, 0), (2, 1)]
//          width = 1 - 0 + 1 = 2
//          minIdx = 0, 归一化后 idx={0, 1}
//          入队: (5, 0), (3, 1), (9, 3)
//
// Level 2: queue = [(5, 0), (3, 1), (9, 3)]
//          width = 3 - 0 + 1 = 4  ← 最大宽度!
// ============================================================
class Solution1 {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        
        int maxWidth = 0;
        // pair: <节点, 在完全二叉树中的编号>
        // 用 unsigned long long 防止中间计算溢出
        queue<pair<TreeNode*, unsigned long long>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            int size = q.size();
            // 本层最左节点编号，用于归一化
            unsigned long long minIdx = q.front().second;
            
            unsigned long long first = 0, last = 0;
            for (int i = 0; i < size; i++) {
                auto [node, idx] = q.front();
                q.pop();
                
                // 归一化：减去本层最左编号，防止 2*idx 溢出
                idx -= minIdx;
                
                if (i == 0) first = idx;           // 本层最左
                if (i == size - 1) last = idx;      // 本层最右
                
                // 子节点编号规则：左 = 2*idx, 右 = 2*idx+1
                if (node->left)
                    q.push({node->left, 2 * idx});
                if (node->right)
                    q.push({node->right, 2 * idx + 1});
            }
            
            // 宽度 = 最右编号 - 最左编号 + 1
            maxWidth = max(maxWidth, (int)(last - first + 1));
        }
        
        return maxWidth;
    }
};

// ============================================================
// 解法2: DFS + 节点编号 — 前序遍历，记录每层首个节点编号
// 时间: O(n)  空间: O(h)（递归栈 + levelFirstIdx 数组）
// 
// 【思路】
// BFS 按层处理很直觉，但 DFS 也可以做。
// 关键观察：前序遍历（根→左→右）中，同一层第一个被访问的节点
// 一定是该层最左的节点。
// 
// 用数组 levelFirstIdx[depth] 记录每层第一次访问时的编号。
// 后续访问同层其他节点时，width = 当前编号 - levelFirstIdx[depth] + 1。
//
// 归一化方式：在传递给子节点时，用归一化后的编号，
// 即 normalizedIdx = idx - levelFirstIdx[depth]。
//
// 【DFS 遍历顺序 (示例1)】
//          1 (depth=0, idx=0)
//         / \
//        3   2
//       / \   \
//      5   3   9
//
// 访问顺序: 1→3→5→3→2→9
// depth=0: 首次, levelFirstIdx[0]=0, width=1
// depth=1: 首次, levelFirstIdx[1]=0, width=1
// depth=2: 首次, levelFirstIdx[2]=0, width=1
// depth=2: 节点3, idx=1, width=1-0+1=2
// depth=1: 节点2, idx=1, width=1-0+1=2
// depth=2: 节点9, idx=3, width=3-0+1=4  ← 最大!
// ============================================================
class Solution2 {
public:
    int widthOfBinaryTree(TreeNode* root) {
        int maxWidth = 0;
        // levelFirstIdx[d] = 深度 d 层第一个被访问节点的编号
        vector<unsigned long long> levelFirstIdx;
        
        dfs(root, 0, 0, levelFirstIdx, maxWidth);
        return maxWidth;
    }
    
private:
    void dfs(TreeNode* node, int depth, unsigned long long idx,
             vector<unsigned long long>& levelFirstIdx, int& maxWidth) {
        if (!node) return;
        
        // 如果是该层第一次访问，记录编号
        // 前序遍历保证第一次访问的是最左节点
        if (depth == (int)levelFirstIdx.size()) {
            levelFirstIdx.push_back(idx);
        }
        
        // 当前宽度 = 当前编号 - 该层最左编号 + 1
        unsigned long long width = idx - levelFirstIdx[depth] + 1;
        maxWidth = max(maxWidth, (int)width);
        
        // 归一化当前编号，防止子节点编号溢出
        // 为什么在这里归一化？因为子节点编号 = 2 * idx 或 2 * idx + 1
        // 如果 idx 不归一化，几十层后就溢出了
        unsigned long long normalizedIdx = idx - levelFirstIdx[depth];
        
        dfs(node->left,  depth + 1, 2 * normalizedIdx,     levelFirstIdx, maxWidth);
        dfs(node->right, depth + 1, 2 * normalizedIdx + 1, levelFirstIdx, maxWidth);
    }
};

// ============================================================
// 【解法对比】
//
// BFS vs DFS:
// - BFS: 更直觉，按层处理天然适合"每层宽度"问题
//   空间 O(n)，取决于最宽层的节点数（最坏情况 n/2）
// - DFS: 空间 O(h)，对于不平衡树（深但不宽）更省空间
//   需要理解"前序遍历首次访问 = 最左节点"这个性质
//
// 面试建议：先写 BFS（5分钟内搞定），追问空间优化时再说 DFS。
//
// ============================================================
// 【易错点】
//
// 1. 不归一化导致溢出:
//    ✗ q.push({node->left, 2 * idx})  // idx 是原始编号，树深60层就溢出
//    ✓ idx -= minIdx; 之后再 q.push({node->left, 2 * idx})
//    原因: 编号呈 2^depth 指数增长，必须每层/每步归一化
//
// 2. 宽度计算差一:
//    ✗ maxWidth = max(maxWidth, (int)(last - first))
//    ✓ maxWidth = max(maxWidth, (int)(last - first + 1))
//    原因: 两个端点之间的"长度"包括两个端点本身
//
// 3. DFS 归一化时机:
//    ✗ dfs(node->left, depth+1, 2*idx)  // idx 是未归一化的
//    ✓ normalizedIdx = idx - levelFirstIdx[depth];
//      dfs(node->left, depth+1, 2*normalizedIdx)
//    原因: 必须在传给子节点之前归一化，否则编号指数膨胀
//
// 4. BFS 中 minIdx 取成全局最小而非本层最小:
//    ✗ 在循环外定义 minIdx = 0
//    ✓ 每层开始时 minIdx = q.front().second
//    原因: 每层的归一化基准不同，必须用本层最左编号
//
// ============================================================
// 【面试追问】
//
// Q1: "如果不编号，直接 BFS 把 null 也入队，为什么不行？"
//   → 极端情况（一条链），第 d 层需要 2^d 个空间存 null，
//     指数膨胀，TLE + MLE。编号法只存实际节点，O(n)。
//
// Q2: "编号溢出怎么办？unsigned long long 够吗？"
//   → 不够，树深 3000 时编号 2^3000。必须归一化：
//     每层把编号减去该层最小值。宽度 = 差值，差值在 int 范围内。
//
// Q3: "DFS 的空间什么时候优于 BFS？"
//   → 树宽但不深时（如完全二叉树 BFS 队列 O(n/2)，DFS 栈 O(logn)）。
//     树深但不宽时两者差不多。极端情况（链状树）DFS O(n) 递归栈。
//
// Q4: "如果追问：返回最宽那一层的深度？"
//   → BFS: 维护 level 计数器，更新 maxWidth 时同时记录 level。
//     DFS: 更新 maxWidth 时记录 depth 参数。
// ============================================================
