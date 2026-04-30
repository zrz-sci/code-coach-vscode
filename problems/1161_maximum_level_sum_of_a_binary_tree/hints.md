# 1161. 最大层内元素和

## 核心思路
本质上就是**逐层遍历二叉树，计算每层节点值之和，找到和最大的那一层**。这是一道经典的"层序遍历 + 统计"问题。

## 思维链
1. **读完题第一反应**：需要知道每一层的节点值之和 → 怎么按层访问节点？→ BFS 层序遍历天然按层处理！
2. **BFS 怎么做？**：用队列，每次处理一整层：记录当前队列大小 `size`，弹出 `size` 个节点并求和，同时把下一层的孩子入队。
3. **DFS 能不能做？**：当然可以！DFS 递归时传入当前深度 `depth`，用一个数组 `levelSum[depth]` 累加每层的和。遍历完后找最大值。
4. **比较两种方案**：BFS 更直觉（"层序"本身就是按层），DFS 也可以但需要额外的数据结构记录每层和。两者时间空间都是 O(n)。
5. **最后一步**：遍历所有层的和，找最大值对应的最小层号。

## 解法概览
| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 层序遍历 | 队列逐层处理，每层求和 | O(n) | O(w)，w为最大层宽度 | ⭐ 必须写出 |
| DFS 递归 | 递归带深度参数，数组记录每层和 | O(n) | O(h)，h为树高 | 能说出即可 |

## 关键提示

```
示例树结构:
        1           ← 第1层: sum = 1
       / \
      7   0         ← 第2层: sum = 7+0 = 7  ★ 最大
     / \
    7  -8           ← 第3层: sum = 7+(-8) = -1

BFS 队列变化过程:
  初始:   queue = [1]                → 第1层 sum=1
  第1轮:  弹出1, 入7,0  queue=[7,0] → 第2层 sum=7
  第2轮:  弹出7,0 入7,-8 queue=[7,-8] → 第3层 sum=-1
  第3轮:  弹出7,-8      queue=[]    → 结束
  
  最大 sum=7 在第2层 → 返回 2
```

- **提示1**：BFS 层序遍历时，用 `queue.size()` 可以精确知道当前层有多少节点，从而"一次处理一层"。
- **提示2**：题目说层号从 1 开始（不是 0），注意初始化。
- **提示3**：节点值可以是**负数**，所以 `maxSum` 的初始值应该设为极小值（如 `INT_MIN`），而不是 0。
- **提示4**：如果多层和相同，返回最小层号 → 用 `>` 而不是 `>=` 来更新。

## 解法详解

### 解法1: BFS 层序遍历 — O(n) / O(w) ⭐ 面试首选

**思考过程**：题目要求"每层的和"，BFS 天然按层处理。每处理完一层就记录该层的和，最终找最大和对应的层号。

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
    int maxLevelSum(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);
        
        int maxSum = INT_MIN;  // 节点值可为负，不能初始化为0
        int resultLevel = 1;
        int currentLevel = 0;
        
        while (!q.empty()) {
            currentLevel++;
            int size = q.size();  // 当前层的节点数
            int levelSum = 0;
            
            // 处理当前层的所有节点
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                levelSum += node->val;
                
                // 下一层的节点入队
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            // 严格大于才更新，保证返回最小层号
            if (levelSum > maxSum) {
                maxSum = levelSum;
                resultLevel = currentLevel;
            }
        }
        
        return resultLevel;
    }
};
```

**关键点**：
- `size` 必须在 for 循环之前取，因为循环中会入队新节点改变队列大小。
- 用 `>` 而不是 `>=` 更新，这样同样大的和会保留较小的层号。

---

### 解法2: DFS 递归 — O(n) / O(h)

**思考过程**：如果不用 BFS，DFS 也能统计每层的和。递归时把当前深度传下去，用一个数组（或 vector）按下标记录每层的累计和。

```cpp
class Solution {
public:
    vector<long long> levelSums;  // levelSums[i] = 第(i+1)层的和
    
    void dfs(TreeNode* node, int depth) {
        if (!node) return;
        
        // 如果是第一次到达这个深度，扩展数组
        if (depth >= (int)levelSums.size()) {
            levelSums.push_back(0);
        }
        levelSums[depth] += node->val;
        
        dfs(node->left, depth + 1);
        dfs(node->right, depth + 1);
    }
    
    int maxLevelSum(TreeNode* root) {
        dfs(root, 0);  // 下标0对应第1层
        
        int resultLevel = 1;
        long long maxSum = levelSums[0];
        for (int i = 1; i < (int)levelSums.size(); i++) {
            if (levelSums[i] > maxSum) {
                maxSum = levelSums[i];
                resultLevel = i + 1;  // 下标转层号：+1
            }
        }
        return resultLevel;
    }
};
```

**关键点**：
- `depth` 从 0 开始，层号从 1 开始，最后要 `+1` 转换。
- DFS 的顺序（前序/中序/后序）无所谓，因为我们只是累加，不关心访问顺序。

## 解法对比

| | BFS 层序遍历 | DFS 递归 |
|---|---|---|
| **时间** | O(n) | O(n) |
| **空间** | O(w)，w 最大层宽度 | O(h)，h 树高 + 层和数组 O(h) |
| **直觉** | 天然按层处理，更直接 | 需要额外数组记录每层和 |
| **适用场景** | 宽树（高度小、宽度大时空间略多） | 窄深树（递归栈深但队列小） |
| **面试推荐** | ⭐ 首选，思路清晰 | 追问时展示 |

**选择建议**：面试中优先写 BFS，因为"层序遍历"和"每层求和"完美匹配，代码逻辑清晰。DFS 作为追问的备选方案。

## 易错点

1. **`maxSum` 初始化为 0 而不是 `INT_MIN`**
   - ✗ `int maxSum = 0;` → 如果所有层的和都是负数，永远不会更新，返回错误的初始层号
   - ✓ `int maxSum = INT_MIN;` → 确保第一层一定能更新

2. **BFS 中 `size` 在循环内取值**
   - ✗ `for (int i = 0; i < q.size(); i++)` → 每次循环 `q.size()` 在变化（因为入队了新节点）
   - ✓ `int size = q.size(); for (int i = 0; i < size; i++)` → 固定当前层的节点数

3. **层号 off-by-one**
   - DFS 中 `depth` 从 0 开始，但题目层号从 1 开始 → 返回时要 `+1`
   - BFS 中如果 `currentLevel` 从 0 开始也要注意

4. **用 `>=` 更新最大值**
   - ✗ `if (levelSum >= maxSum)` → 相同和的后面层会覆盖前面层，返回的不是最小层号
   - ✓ `if (levelSum > maxSum)` → 只有严格更大才更新

## 面试追问

**Q1: 这道题最直接的解法是什么？时间复杂度？**
→ BFS 层序遍历，逐层求和。时间 O(n)，每个节点恰好入队出队一次。

**Q2: 如果用 DFS 怎么做？和 BFS 相比有什么优劣？**
→ DFS 递归传深度，用数组记录每层和。空间上：BFS 取决于最大宽度 O(w)，DFS 取决于高度 O(h)。对于完全二叉树 w ≈ n/2 而 h = log n，DFS 更省；对于链状树 h = n 而 w = 1，BFS 更省。

**Q3: 如果不是求最大层和，而是求第 K 大的层和对应的层号呢？**
→ 仍然 BFS/DFS 收集所有层的和，然后对层和数组排序（或用大小为 K 的最小堆），找第 K 大。额外时间 O(L log L) 或 O(L log K)，L 为层数。

**Q4: 如果树特别大，无法完全放入内存怎么办？**
→ 这是一个流式处理问题。BFS 天然适合：每次只需内存中存当前层和下一层的节点。可以用外部存储分层读取。DFS 则需要递归栈深度等于树高。

## 相关题型

- **102. 二叉树的层序遍历** — 完全相同的 BFS 框架，区别是 102 收集每层节点值列表，本题收集每层和
- **104. 二叉树的最大深度** — 复用 BFS 层序遍历框架，区别是只计层数不求和
- **515. 在每个树行中找最大值** — 复用同样的"逐层处理"模式，把 `sum` 改成 `max`
- **637. 二叉树的层平均值** — 复用同样框架，把 `sum` 改成 `sum/count`