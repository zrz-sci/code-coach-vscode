# 107. 二叉树的层序遍历 II (Binary Tree Level Order Traversal II)

## 核心思路

和 LC 102（标准层序遍历）完全相同，唯一区别是结果要**从底层到顶层**。先正常 BFS 层序遍历收集每层结果，最后反转即可。

## 思维链

1. **第一反应**：和 102 一模一样的 BFS，只是输出顺序反了
2. **暴力想法**：先做标准层序遍历得到 `[[3],[9,20],[15,7]]`，然后 `reverse` 得到 `[[15,7],[9,20],[3]]`
3. **瓶颈在哪？** reverse 是 O(层数)，几乎零开销，没有优化的必要
4. **替代方案**：用 DFS 按深度分组，天然支持任意顺序输出；或用头插法 `push_front` 避免最后 reverse

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS + reverse | 标准层序遍历后反转 | O(n) | O(n) | ⭐ 必须写出 |
| BFS + 头插法 | 每层结果插到 result 开头 | O(n) | O(n) | 能说出即可 |
| DFS + 深度分组 | 递归按深度存入对应层 | O(n) | O(n) | 加分项 |

## 关键提示

1. **核心模式**：BFS 用队列，逐层处理。每层开始前记录 `size = queue.size()`，循环 size 次处理该层所有节点。

2. **树形结构示意**：
```
        3            Level 0: [3]
       / \
      9   20         Level 1: [9, 20]
         / \
        15   7       Level 2: [15, 7]

正序: [[3],[9,20],[15,7]]
反序: [[15,7],[9,20],[3]]  ← 本题要求
```

3. **reverse vs 头插法**：reverse 代码更简洁，头插法（`deque` 或 `insert(0,...)`）避免了最后一步反转，但 vector 头插是 O(k) 的，总体不一定更快。

4. **DFS 思路**：用 `depth` 参数决定放到 result 的哪一层。如果 `depth == result.size()`，说明是新的一层，先 push 一个空 vector。

5. **与 LC 102 的关系**：代码只差最后一行 `reverse(result.begin(), result.end())`。面试时一定要先提到这个关系。

## 解法详解

### 解法1: BFS + reverse — O(n) / O(n) ⭐ 面试首选

**思考过程**: 这就是 LC 102 + 一行 reverse。BFS 天然按层遍历，收集完后翻转就是自底向上。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(level);
        }

        reverse(result.begin(), result.end());
        return result;
    }
};
```

**关键点**: `reverse` 操作是 O(层数)，不是 O(n)，因为只交换 vector 的指针（move semantics），不复制元素。

### 解法2: DFS 深度分组 — O(n) / O(n)

**从解法1变化**: 用递归代替队列，`depth` 参数决定放到哪一层。最后同样 reverse。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        reverse(result.begin(), result.end());
        return result;
    }

    void dfs(TreeNode* node, int depth, vector<vector<int>>& result) {
        if (!node) return;
        if (depth == (int)result.size()) {
            result.push_back({});  // 新的一层
        }
        result[depth].push_back(node->val);
        dfs(node->left, depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};
```

### 解法3: BFS + deque 头插法 — O(n) / O(n)

**思路**: 用 `deque` 代替 `vector` 存结果，每层结果 `push_front`，天然是反序，不需要最后 reverse。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        deque<vector<int>> dq;
        if (!root) return {};

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            vector<int> level;
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            dq.push_front(level);  // 头插，天然反序
        }

        return vector<vector<int>>(dq.begin(), dq.end());
    }
};
```

## 解法对比

| | BFS+reverse | DFS+reverse | BFS+deque头插 |
|--|------------|-------------|--------------|
| 代码量 | 最少 | 中等 | 中等 |
| 直觉 | 最直观 | 需要理解递归深度 | 需要知道 deque |
| 额外操作 | reverse一次 | reverse一次 | 无 |
| **面试推荐** | ⭐ 首选 | 展示递归能力 | 展示 STL 知识 |

## 易错点

1. **忘记判空**：`root == nullptr` 时直接返回空 vector，不要进入 while 循环
   - ✗ 没有 `if (!root) return result;` → 空队列进 while 直接跳过，不报错但不规范
   - ✓ 显式判空，代码意图清晰

2. **DFS 的 depth == result.size() 判断**：必须用 `==` 不是 `>=`
   - ✗ `if (depth >= result.size())` → 如果某层被跳过会创建多余空层
   - ✓ `if (depth == result.size())` → 精确控制层数

3. **vector 头插性能**：`result.insert(result.begin(), level)` 是 O(k) 的，不如 deque
   - ✗ 用 `vector::insert(0, ...)` 头插法
   - ✓ 用 `deque::push_front` 或直接 reverse

4. **reverse 的对象**：是反转 `result`（vector of vectors），不是反转每一层
   - ✗ 对每层的元素做 reverse
   - ✓ 对层的顺序做 reverse

## 面试追问

**Q1: 和 LC 102 的区别是什么？代码改了哪里？**
> 唯一区别是最后加一行 `reverse(result.begin(), result.end())`。其余代码完全相同。面试中先说清楚这个关系再写代码，展示你对题目的理解。

**Q2: reverse 操作的复杂度是多少？会不会成为瓶颈？**
> O(层数)，不是 O(n)。`vector<vector<int>>` 的 reverse 只交换内部指针（move semantics），不复制元素。对于平衡二叉树层数是 O(log n)，完全不是瓶颈。

**Q3: 如果要求不用 reverse，怎么做？**
> 两种方案：(1) 用 deque + push_front，天然反序；(2) DFS 时用 `result.size() - 1 - depth` 作为索引，但需要预知总层数。实际面试中 reverse 方案最简洁。

**Q4: 如果树非常深（百万层），递归 DFS 会有什么问题？**
> 栈溢出。递归深度等于树的高度，极端情况（链状树）是 O(n)。此时应该用 BFS 迭代方案，队列不受调用栈限制。

## 相关题型

- **102. 二叉树的层序遍历** — 本题的正序版本，代码只差一行 reverse。掌握 102 即可秒杀本题。
- **103. 二叉树的锯齿形层序遍历** — 在 102 基础上，奇数层反转。复用 BFS 框架 + 按层号决定是否 reverse。
- **199. 二叉树的右视图** — BFS 框架相同，只取每层最后一个元素。复用 `size = queue.size()` 的分层技巧。
- **637. 二叉树的层平均值** — BFS 框架相同，每层求平均值而非收集所有元素。
