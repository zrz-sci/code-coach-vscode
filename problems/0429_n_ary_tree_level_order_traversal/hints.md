# 429. N 叉树的层序遍历

## 核心思路

标准 **BFS 层序遍历** -- 用队列逐层处理节点。和二叉树层序遍历 (LeetCode 102) 几乎相同，唯一区别是每个节点的子节点从固定的 `left/right` 变成了 `children` 数组。理解了二叉树 BFS 模板，这道题只需要把 "入队左右孩子" 改成 "遍历 children 数组入队"。

## 思维链

1. **读完题第一反应** -- 层序遍历 = BFS，用队列逐层处理。二叉树层序遍历是面试基础中的基础，这里只是推广到 N 叉树。

2. **和二叉树 BFS 有什么不同？** -- 唯一区别：二叉树每个节点固定有 `left` 和 `right` 两个子节点，N 叉树每个节点有一个 `children` 数组（长度不固定）。所以入队操作从 "if(left) push; if(right) push" 变成 "for(child : children) push"。

3. **BFS 模板的关键：如何区分层？** -- 在每层处理开始前，先记录当前队列的大小 `size`，然后恰好处理 `size` 个节点。这些节点属于同一层，它们的子节点入队后属于下一层。

4. **能不能用 DFS？** -- 可以！DFS 传入层号 `depth`，把节点值放到 `result[depth]` 中。但 BFS 更符合"层序"的直觉，面试首选 BFS。

5. **边界情况** -- 空树 `root == nullptr`，直接返回空列表。

## ASCII 树结构示意

```
示例 1: root = [1, null, 3, 2, 4, null, 5, 6]

            1            ← Level 0: [1]
          / | \
         3  2   4        ← Level 1: [3, 2, 4]
        / \
       5   6             ← Level 2: [5, 6]

输出: [[1], [3, 2, 4], [5, 6]]

示例 2: root = [1, null, 2, 3, 4, 5, null, null, 6, 7, null, 8, null, 9, 10, ...]

                    1                    ← Level 0
              / |    |   \
             2  3    4    5              ← Level 1
               / \   |   / \
              6   7  8  9  10            ← Level 2
                  |     |
                 11    12                ← Level 3 (部分)
                  |
                 14                      ← Level 4

输出: [[1], [2,3,4,5], [6,7,8,9,10], [11,12,13], [14]]
```

## BFS 逐步执行过程

```
以示例 1 为例: root = [1, null, 3, 2, 4, null, 5, 6]

初始: queue = [Node(1)]

--- 第 0 层 ---
size = 1
  弹出 Node(1), level = [1]
  入队 children: [Node(3), Node(2), Node(4)]
→ result = [[1]]
  queue = [Node(3), Node(2), Node(4)]

--- 第 1 层 ---
size = 3
  弹出 Node(3), level = [3]
    入队 children: [Node(5), Node(6)]
  弹出 Node(2), level = [3, 2]
    入队 children: []  (无子节点)
  弹出 Node(4), level = [3, 2, 4]
    入队 children: []  (无子节点)
→ result = [[1], [3, 2, 4]]
  queue = [Node(5), Node(6)]

--- 第 2 层 ---
size = 2
  弹出 Node(5), level = [5]
    入队 children: []
  弹出 Node(6), level = [5, 6]
    入队 children: []
→ result = [[1], [3, 2, 4], [5, 6]]
  queue = []  ← 空，结束

最终输出: [[1], [3, 2, 4], [5, 6]]
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 迭代 | 队列逐层处理 | O(n) | O(n) | 必须写出 |
| DFS 递归 | 递归传层号 | O(n) | O(h) | 能说出即可 |

## 关键提示

1. **空树边界**: `root == nullptr` 时直接返回 `{}`，不要忘记这个检查。

2. **分层的核心技巧**: 每层处理前先记录 `int size = q.size()`，然后 for 循环恰好 `size` 次。这是所有层序遍历题的通用模板。

3. **children 可能包含 nullptr**: 虽然 LeetCode 官方测试数据一般不会有 `null` 在 children 中，但稳妥起见在入队时检查 `if (child) q.push(child)`。

4. **N 叉树序列化格式**: 输入 `[1, null, 3, 2, 4, null, 5, 6]` 中的 `null` 是分隔符，表示一组子节点结束。`1` 的子节点是 `null` 之前的（没有，因为 `null` 紧接在 `1` 后面）... 实际上第一个 `null` 分隔根节点和第一层子节点，后续 `null` 分隔不同父节点的子节点组。不过做题时不需要自己解析，LeetCode 会给你构建好的树。

5. **DFS 做法的要点**: 传入 `depth` 参数，如果 `result.size() == depth` 说明这一层还没创建，先 `push_back` 一个空 vector。然后把当前节点值放进 `result[depth]`，递归处理所有子节点时 `depth + 1`。

## 解法详解

### 解法1: BFS 迭代 -- O(n) / O(n)

**思路**: 标准 BFS 层序遍历模板。队列初始放入根节点，每次处理一层：先记录当前层大小 `size`，弹出 `size` 个节点收集值，同时将它们的所有子节点入队。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> res;
        if (!root) return res;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();     // 当前层的节点数
            vector<int> level;
            for (int i = 0; i < size; i++) {
                Node* node = q.front(); q.pop();
                level.push_back(node->val);
                // 将所有子节点入队（区别于二叉树的 left/right）
                for (Node* child : node->children) {
                    if (child) q.push(child);
                }
            }
            res.push_back(level);
        }
        return res;
    }
};
```

### 解法2: DFS 递归 -- O(n) / O(h)

**思路**: 前序遍历 N 叉树，传入当前层深度 `depth`。遍历到某个节点时，把它的值放进 `result[depth]`。由于 DFS 是深度优先，同一层的节点会被分散在不同的递归路径中访问，但只要 `depth` 正确，结果和 BFS 一样。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        vector<vector<int>> res;
        dfs(root, 0, res);
        return res;
    }

    void dfs(Node* node, int depth, vector<vector<int>>& res) {
        if (!node) return;
        // 如果当前层还没有对应的 vector，先创建
        if (res.size() == depth) {
            res.push_back({});
        }
        res[depth].push_back(node->val);
        for (Node* child : node->children) {
            dfs(child, depth + 1, res);
        }
    }
};
```

**关键点**: DFS 空间复杂度是 O(h)（递归栈深度 = 树的高度），对于平衡树 h = O(log n)，最坏退化为链表时 h = O(n)。BFS 空间复杂度在最宽层处最大，最坏也是 O(n)。

## 解法对比

| | BFS 迭代 | DFS 递归 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(w) 最宽层宽度 | O(h) 树高度 |
| 代码复杂度 | 简单 | 简单 |
| 面试推荐 | 首选 | 备选 |
| 核心思想 | 队列 + 分层计数 | 前序遍历 + 层号索引 |

## 易错点

1. **忘记处理空树**: 如果 `root == nullptr` 不提前返回，后续 `q.push(root)` 会把空指针入队，然后 `q.front()` 返回 `nullptr`，访问 `nullptr->val` 导致段错误。

2. **size 变量在循环内取值**: 必须在内层 for 循环**之前**记录 `size = q.size()`，不能在 for 条件里写 `i < q.size()`，因为循环体中会往队列里加新元素，`q.size()` 会变化。

3. **DFS 解法忘记扩展 result**: 如果 `depth` 等于 `res.size()`，说明这一层是第一次访问，必须先 `res.push_back({})` 创建空 vector，否则 `res[depth]` 越界。

## 面试追问

**Q1: BFS 和 DFS 做层序遍历有什么区别？什么时候选哪个？**
-> BFS 天然按层处理，代码直觉清晰；DFS 需要额外传入层号来模拟分层。一般层序遍历面试首选 BFS。但如果面试官明确要求递归解法，就用 DFS。

**Q2: 如果树的某一层非常宽（比如完全 N 叉树），BFS 和 DFS 的空间谁更优？**
-> 完全 N 叉树最底层可能有 N^(h-1) 个节点，BFS 队列要存整层。DFS 只需要 O(h) 的栈空间。所以在宽树场景下 DFS 空间更优。

**Q3: 这道题和 LeetCode 102 (二叉树层序遍历) 的关系？**
-> 几乎相同。102 是 N=2 的特例。把 `if(left) push; if(right) push` 改成 `for(child : children) push` 就完成了。

## 相关题型

- **LeetCode 102 二叉树的层序遍历** -- N=2 的特例，完全相同的 BFS 模板
- **LeetCode 107 二叉树的层序遍历 II** -- 相同模板，最后把 result 反转即可
- **LeetCode 199 二叉树的右视图** -- BFS 每层只取最后一个节点
- **LeetCode 559 N 叉树的最大深度** -- 同样用 BFS 或 DFS 遍历 N 叉树，记录最大深度
- **LeetCode 589/590 N 叉树的前序/后序遍历** -- 同系列的 N 叉树 DFS 题
