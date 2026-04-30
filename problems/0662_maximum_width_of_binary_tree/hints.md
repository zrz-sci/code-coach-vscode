# 662. 二叉树最大宽度

## 核心思路

本质上是给二叉树的每个节点分配一个"完全二叉树编号"，然后每层的宽度 = 该层最右编号 - 最左编号 + 1。问题从"树的遍历"转化为"编号的管理"。

## 思维链

1. **读完题第一反应**：需要逐层看，每层找到最左和最右的非空节点，中间的 null 也算宽度。→ 自然想到 BFS 层序遍历。

2. **直接 BFS 的问题**：如果把 null 节点也放进队列，遇到"左子树很深、右子树很深但中间全是空"的情况，队列会指数膨胀（一层可能有 2^n 个 null），完全不现实。

3. **突破瓶颈的关键观察**：完全二叉树中，如果父节点编号是 `idx`，左孩子是 `2*idx`，右孩子是 `2*idx+1`。我们不需要真的把 null 节点放进队列，只需要记录每个**实际存在**的节点的编号，宽度就是 `最右编号 - 最左编号 + 1`。

4. **防溢出优化**：树深度最大 3000，编号可达 2^3000。但宽度只在同层内计算，所以每层可以把编号减去该层最左节点的编号做"归一化"，防止溢出。

5. **DFS 也能做**：不一定非得 BFS。用 DFS 记录每层第一次访问时的编号（即最左节点编号），后续同层节点的编号与之相减就是候选宽度。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS + 节点编号 | 层序遍历，每层记录最左最右编号 | O(n) | O(n) | ⭐ 必须写出 |
| DFS + 节点编号 | 前序遍历，记录每层首个节点编号 | O(n) | O(h) | 能说出即可 |

## 关键提示

- **提示1**：想想完全二叉树的数组表示——节点 `i` 的左孩子是 `2i`，右孩子是 `2i+1`。这个编号能帮你算出"含 null 的宽度"。

- **提示2**：你不需要真的存储 null 节点，只需要给每个**实际存在的节点**一个编号。

- **提示3**：编号会指数增长，但每层的宽度保证在 32 位范围内。想想怎么在每层"重置"编号来防溢出？

- **提示4**：BFS 中，每层开始时队列里所有节点的编号，减去该层最小编号，就能归一化。

- **ASCII 示意图**：
```
         1 (idx=0)               每层编号:
        / \                      Level 0: [0]         宽度=1
       3   2                     Level 1: [0, 1]      宽度=2
     (0)  (1)                    Level 2: [0, 1, _, 3] 宽度=4
      / \     \
     5   3     9                 编号规则: 父idx → 左2*idx, 右2*idx+1
    (0) (1)   (3)                宽度 = 最右 - 最左 + 1 = 3 - 0 + 1 = 4
```

## 解法详解

### 解法1: BFS + 节点编号 — O(n) / O(n) ⭐ 面试首选

**思考过程**：
宽度需要知道每层"最左到最右的跨度（含null）"。如果我们给节点编号（按完全二叉树的方式），那么每层的宽度就是 `rightmost_idx - leftmost_idx + 1`。BFS 天然按层遍历，每层处理时取第一个和最后一个节点的编号即可。

为了防止编号溢出，每层开始时把所有编号减去该层最左节点的编号（归一化为从0开始）。因为宽度只关心同层内编号的差值，减去同一个偏移量不影响结果。

```cpp
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
class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        
        int maxWidth = 0;
        // 队列存 <节点指针, 该节点在完全二叉树中的编号>
        queue<pair<TreeNode*, unsigned long long>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            int size = q.size();
            // 本层最左节点的编号，用于归一化防溢出
            unsigned long long minIdx = q.front().second;
            
            unsigned long long first = 0, last = 0;
            for (int i = 0; i < size; i++) {
                auto [node, idx] = q.front();
                q.pop();
                // 归一化：减去本层最左编号
                idx -= minIdx;
                
                if (i == 0) first = idx;       // 本层第一个（最左）
                if (i == size - 1) last = idx;  // 本层最后一个（最右）
                
                if (node->left)
                    q.push({node->left, 2 * idx});
                if (node->right)
                    q.push({node->right, 2 * idx + 1});
            }
            
            maxWidth = max(maxWidth, (int)(last - first + 1));
        }
        
        return maxWidth;
    }
};
```

**关键点**：
- 归一化 `idx -= minIdx` 是防溢出的核心，不做这步在极端情况（树深度大）会溢出。
- 用 `unsigned long long` 存编号更安全，即使归一化后用 `long long` 也够了。

---

### 解法2: DFS + 节点编号 — O(n) / O(h)

**从 BFS 到 DFS 的转变**：
BFS 天然按层处理，DFS 也可以做——关键是用一个数组记录每层"第一次访问"的节点编号。由于 DFS 前序遍历时，同层的第一个被访问的节点一定是最左节点，后续同层节点的编号与之相减即可得候选宽度。

```cpp
class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        int maxWidth = 0;
        // levelFirstIdx[depth] = 该层第一个被访问节点的编号
        vector<unsigned long long> levelFirstIdx;
        
        // 前序 DFS: node, depth, idx
        function<void(TreeNode*, int, unsigned long long)> dfs = 
            [&](TreeNode* node, int depth, unsigned long long idx) {
            if (!node) return;
            
            // 如果是该层第一次访问，记录编号
            if (depth == (int)levelFirstIdx.size()) {
                levelFirstIdx.push_back(idx);
            }
            
            // 当前节点与该层最左节点的编号差 + 1 = 候选宽度
            unsigned long long width = idx - levelFirstIdx[depth] + 1;
            maxWidth = max(maxWidth, (int)width);
            
            // 递归左右子树，编号规则不变
            // 归一化：用 idx - levelFirstIdx[depth] 防溢出
            unsigned long long normalizedIdx = idx - levelFirstIdx[depth];
            dfs(node->left, depth + 1, 2 * normalizedIdx);
            dfs(node->right, depth + 1, 2 * normalizedIdx + 1);
        };
        
        dfs(root, 0, 0);
        return maxWidth;
    }
};
```

**关键点**：
- `levelFirstIdx` 相当于 BFS 中每层的 `minIdx`，利用前序遍历"左先于右"的特性，第一次到达新深度的一定是最左节点。
- DFS 的空间复杂度是 O(h)（递归栈 + levelFirstIdx 数组），在树很宽但不深时比 BFS 更省空间。

## 解法对比

| | BFS + 编号 | DFS + 编号 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(n)（队列最宽层） | O(h)（递归栈+数组） |
| 直觉性 | ⭐ 更直觉（层序天然按层） | 需要理解"首次访问=最左" |
| 适用场景 | 面试首选，清晰不出错 | 树很宽但不深时更省空间 |
| 实现难度 | 简单 | 归一化时机需要想清楚 |

**面试建议**：优先写 BFS，清晰直观，不容易写错。DFS 作为 follow-up 讨论。

## 易错点

1. **不做归一化导致溢出**
   - ✗ 直接用 `idx = 2 * parentIdx + 1`，树深 60 层编号就超 `long long`
   - ✓ 每层/每次递归时 `idx -= minIdx`，只保留相对偏移

2. **宽度计算差一**
   - ✗ `width = last - first`（少算了端点本身）
   - ✓ `width = last - first + 1`

3. **DFS 中归一化时机错误**
   - ✗ 先递归再归一化 → 子节点编号已经基于未归一化的父编号算过了
   - ✓ 在传给子节点之前就要归一化当前编号

4. **BFS 中 `minIdx` 取错**
   - ✗ 用上一层的 `minIdx` → 每层的偏移量不同
   - ✓ 每层开始时取 `q.front().second` 作为本层的 `minIdx`

## 面试追问

**Q1: 为什么不能把 null 也放进队列来计算宽度？**
→ 因为每层节点数是 2^depth，极端情况（如一条向右的链）会导致队列指数膨胀。编号法只存实际节点，空间 O(n)。

**Q2: 编号为什么会溢出？怎么防？**
→ 完全二叉树第 d 层的编号范围是 [2^d, 2^(d+1)-1]，树深 3000 时编号天文数字。解决：每层归一化，让编号从 0 开始。因为宽度 = 同层最右 - 最左 + 1，减去同一偏移不影响差值。

**Q3: 如果要返回最大宽度所在的层数呢？**
→ 在更新 maxWidth 时同时记录当前层深度即可，BFS 中维护一个 `level` 计数器。

**Q4: 能否用 O(1) 额外空间（不算递归栈）？**
→ 不太行。至少需要 O(每层节点数) 来存编号（BFS），或 O(h) 来存每层首节点编号（DFS）。这是本质需求。

## 相关题型

- **102. 二叉树的层序遍历** — 复用 BFS 层序遍历模板，本题在此基础上增加了"编号"这一维信息
- **111. 二叉树的最小深度** — 同样是 BFS 层序遍历的应用，找到第一个叶子节点的层
- **1161. 最大层内元素之和** — 同样的 BFS 逐层处理框架，本题算宽度，1161 算元素和
- **199. 二叉树的右视图** — BFS 每层取最后一个节点，vs 本题每层取首尾编号