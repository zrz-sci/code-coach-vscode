# 199. 二叉树的右视图

## 核心思路

本质上就是求**二叉树每一层的最右边那个节点**。"右视图"只是形象化说法，翻译成算法语言就是：收集每一层的最后一个节点值。

## 思维链

1. **读完题第一反应**：站在右边看，能看到的就是每一层最靠右的节点。那我只要能按层遍历，取每层最后一个就行了——这不就是 BFS 层序遍历吗？

2. **BFS 天然适合**：层序遍历用队列，每层 `size` 个节点，遍历到最后一个就是右视图的那个节点。没有什么"瓶颈"要优化，BFS 本身就是最直觉的解法。

3. **换个角度想 DFS 能不能做？** 如果用 DFS，我需要知道"当前节点在第几层"。只要我优先访问右子树，每层第一个被访问到的就是最右边的节点。用 `result.size() == depth` 来判断是不是该层首次访问。

4. **两种 DFS 方式**：先右后左的前序遍历（根→右→左），或者先左后右但取每层最后一个。前者更简洁，因为每层第一个遇到的就是答案。

5. **对比选择**：BFS 最直觉，面试时先说 BFS；DFS 展示对递归的掌握，可以作为追问的第二种解法。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 层序遍历 | 每层取最后一个节点 | O(n) | O(w)，w为最大宽度 | ⭐ 必须写出 |
| DFS 先右后左 | 先右后左前序遍历，每层首次访问即为答案 | O(n) | O(h)，h为树高 | ⭐ 必须写出 |
| DFS 先左后右 | 正常前序，用覆盖方式取每层最后一个 | O(n) | O(h) | 能说出即可 |

## 关键提示

- **提示1**：如果你能按层遍历整棵树，取每层最后一个元素就是答案。什么遍历方式天然按层处理？

- **提示2**：DFS 也能做。关键问题：怎么知道某个节点是它所在层的"最右边"？想想如果你先访问右子树再访问左子树，每层第一个被你碰到的是哪个？

- **提示3**：DFS 中，`result` 数组的长度天然代表"已经收集了几层的结果"。如果当前深度 `depth == result.size()`，说明这是该层第一个被访问的节点。

- **提示4（易错）**：右视图不等于"只看右子树"！示例2中最底层的5是左子树的后代，但它是该层唯一节点，所以也在右视图中。

```
示例1的树结构:
        1          ← 第0层，右视图看到 1
       / \
      2   3        ← 第1层，右视图看到 3
       \   \
        5   4      ← 第2层，右视图看到 4

示例2的树结构:
        1          ← 第0层，看到 1
       / \
      2   3        ← 第1层，看到 3
     /
    4              ← 第2层，看到 4（注意！不是右子树的后代）
   /
  5                ← 第3层，看到 5（左子树深处的节点）
```

## 解法详解

### 解法1: BFS 层序遍历 — O(n) / O(w) ⭐ 面试首选

**思考过程**：右视图 = 每层最右边的节点。BFS 层序遍历天然按层处理，每层的最后一个节点就是答案。

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
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size(); // 当前层的节点数
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 当前层的最后一个节点就是右视图能看到的
                if (i == size - 1) {
                    result.push_back(node->val);
                }
                
                // 先左后右入队（顺序无所谓，关键是取最后一个）
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return result;
    }
};
```

```
// BFS 执行过程（示例1）:
//
//        1
//       / \
//      2   3
//       \   \
//        5   4
//
// 第0层: 队列 [1]         → size=1, 取最后一个=1  → result=[1]
// 第1层: 队列 [2, 3]      → size=2, 取最后一个=3  → result=[1,3]
// 第2层: 队列 [5, 4]      → size=2, 取最后一个=4  → result=[1,3,4]
```

**关键点**：
- 必须用 `int size = q.size()` 在循环前保存当前层大小，因为循环中会往队列里加入下一层的节点
- 判断 `i == size - 1` 来识别每层最后一个节点

### 解法2: DFS 先右后左 — O(n) / O(h) ⭐ 面试追问

**从解法1的思路出发换角度**：如果我们 DFS 时优先访问右子树，那么每一层第一个被访问到的节点就是最右边的。怎么判断"第一个"？如果 `result.size() == depth`，说明 depth 这一层还没有结果，当前节点就是该层第一个（也是最右的）。

```cpp
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<int>& result) {
        if (!node) return;
        
        // result.size() 代表已收集了几层的结果
        // 如果当前深度 == 已收集层数，说明这是该层第一个被访问的节点
        if (depth == result.size()) {
            result.push_back(node->val);
        }
        
        // 先右后左！确保每层最右边的节点最先被访问
        dfs(node->right, depth + 1, result);
        dfs(node->left,  depth + 1, result);
    }
};
```

```
// DFS 先右后左执行过程（示例2）:
//
//        1
//       / \
//      2   3
//     /
//    4
//   /
//  5
//
// dfs(1, depth=0): result.size()=0 == depth=0 → 收集1, result=[1]
//   dfs(3, depth=1): result.size()=1 == depth=1 → 收集3, result=[1,3]
//     dfs(null): 返回
//     dfs(null): 返回
//   dfs(2, depth=1): result.size()=2 != depth=1 → 跳过（第1层已有3）
//     dfs(null): 返回（2没有右子树）
//     dfs(4, depth=2): result.size()=2 == depth=2 → 收集4, result=[1,3,4]
//       dfs(null): 返回（4没有右子树）
//       dfs(5, depth=3): result.size()=3 == depth=3 → 收集5, result=[1,3,4,5]
//         ...
```

**关键点**：
- `depth == result.size()` 是核心判断，利用了 result 数组长度作为"已收集层数"的天然计数器
- 必须先递归右子树再递归左子树，顺序反了就变成"左视图"了

### 解法3: DFS 先左后右（覆盖法）— O(n) / O(h)

**另一种 DFS 角度**：如果用正常的先左后右 DFS，同一层会先访问左边再访问右边。那我们用 map 或数组，每次都覆盖该层的值，最终留下的就是最右边的。

```cpp
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        dfs(root, 0, result);
        return result;
    }
    
private:
    void dfs(TreeNode* node, int depth, vector<int>& result) {
        if (!node) return;
        
        // 如果该层还没有记录，先占位
        if (depth == result.size()) {
            result.push_back(node->val);
        } else {
            // 该层已有记录，用当前值覆盖（后来的更靠右）
            result[depth] = node->val;
        }
        
        // 先左后右，最终 result[depth] 留下的是该层最右边的值
        dfs(node->left,  depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};
```

**关键点**：这种方法多做了一些无用的覆盖操作，不如解法2优雅，但帮助理解"先右后左"为什么更好——它避免了多余的覆盖。

## 解法对比

| | BFS 层序遍历 | DFS 先右后左 | DFS 先左后右 |
|---|---|---|---|
| 核心思想 | 每层取最后一个 | 每层取第一个（先访问右侧） | 每层不断覆盖，最后留下的是最右 |
| 空间复杂度 | O(w) 最大宽度 | O(h) 树高 | O(h) 树高 |
| 最坏空间 | 完全二叉树 w≈n/2 | 链状树 h=n | 链状树 h=n |
| 代码直觉性 | ⭐ 最直觉 | 需要理解深度判断 | 多了覆盖逻辑 |
| 面试建议 | 首选 | 追问时展示 | 仅供理解 |

**什么时候选哪个？**
- **面试首选 BFS**：思路清晰，面试官一听就懂
- **追问 DFS**：面试官问"能用DFS做吗？"时展示解法2
- 如果树很宽（接近完全二叉树），DFS 的空间 O(h)=O(log n) 优于 BFS 的 O(w)=O(n)
- 如果树很深（接近链状），BFS 的空间 O(1) 优于 DFS 的 O(h)=O(n)

## 易错点

1. **误以为"只看右子树就行"**
   - ✗ 只递归 `root->right`，遗漏左子树深处的节点
   - ✓ 必须遍历整棵树。示例2中第3层的节点5来自左子树
   ```
   // 这棵树的右视图包含左子树的节点:
   //     1
   //    /
   //   2      ← 右视图是 [1, 2]，2是左子节点！
   ```

2. **BFS 中 size 变量在循环内获取**
   - ✗ `for (int i = 0; i < q.size(); i++)` — q.size() 每次迭代都变
   - ✓ `int size = q.size(); for (int i = 0; i < size; i++)` — 提前保存

3. **DFS 中深度判断写反**
   - ✗ `if (depth < result.size())` — 永远在覆盖已有结果
   - ✓ `if (depth == result.size())` — 只在新层时才加入

4. **DFS 先右后左的顺序写反**
   - ✗ 先 `dfs(node->left, ...)` 再 `dfs(node->right, ...)` — 变成左视图
   - ✓ 先 `dfs(node->right, ...)` 再 `dfs(node->left, ...)` — 确保右边先访问

## 面试追问

**Q1（基础理解）**：这道题和层序遍历有什么关系？
> 右视图就是层序遍历每层的最后一个元素。如果要求左视图，就是每层的第一个元素。

**Q2（DFS 追问）**：能用 DFS 做吗？时间空间和 BFS 比哪个更好？
> DFS 先右后左，用 `depth == result.size()` 判断首次访问。空间上 DFS 是 O(h)，BFS 是 O(w)。对于平衡树 h=log n, w=n/2，DFS 更优；对于链状树 h=n, w=1，BFS 更优。

**Q3（变体）**：如果要求左视图呢？如果要求每一层的第二个节点呢？
> 左视图：BFS 取每层第一个（i==0），或 DFS 先左后右。第二个节点：BFS 取 i==1 的元素（如果该层节点数 >= 2）。

**Q4（进阶变体）**：如果要求二叉树的俯视图（top view）或底视图（bottom view）呢？
> 需要引入"水平距离"(horizontal distance)的概念。对每个节点记录其列号（根为0，左-1，右+1），按列号排序后，俯视图取每列第一个遇到的，底视图取每列最后一个遇到的。

## 相关题型

- **102. 二叉树的层序遍历** — 本题的基础。右视图就是层序遍历结果中每个子数组的最后一个元素。
- **116. 填充每个节点的下一个右侧节点指针** — 同样是按层处理，复用 BFS 层序遍历框架，区别在于层内操作不同（取最后一个 vs 连接 next 指针）。
- **513. 找树左下角的值** — 复用本题 BFS 框架，但改为取最后一层的第一个节点；或复用 DFS 框架，改为先左后右。
- **107. 二叉树的层序遍历 II** — 复用 BFS 层序遍历，最后反转结果数组即可。