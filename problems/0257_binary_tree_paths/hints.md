# 257. 二叉树的所有路径

## 核心思路

本质上就是**遍历二叉树的所有从根到叶子的路径**，遇到叶子节点时把当前路径记录下来。这是树的 DFS 的直接应用。

## 思维链

1. **读完题第一反应**：需要找"所有路径"，每条路径从根到叶子。路径是树上从上到下的一条链路，自然想到 DFS（深度优先搜索）——沿着一条路走到底（叶子），记录路径，然后回头走另一条。

2. **怎么记录路径？** DFS 过程中，我需要知道"从根到当前节点经过了哪些节点"。两种方式：
   - 传字符串参数：每递归一层，把当前节点值拼到字符串后面
   - 传路径数组参数：每递归一层，把当前节点加入数组，回溯时弹出

3. **什么时候收集结果？** 当到达叶子节点（左右孩子都为空）时，当前路径就是一条完整路径。

4. **递归 vs 迭代？** 递归最自然，但面试中可能追问迭代写法。迭代可以用栈模拟 DFS，每个栈元素同时保存节点和对应的路径字符串。

5. **BFS 能不能做？** 当然可以，用队列做层序遍历，同样每个元素带上路径信息，遇到叶子就收集。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 递归（传字符串） | 递归传递路径字符串，叶子时收集 | O(n·h) | O(h) 递归栈 | ⭐ 必须写出 |
| DFS 递归（回溯） | 维护路径数组，回溯时弹出 | O(n·h) | O(h) | ⭐ 必须写出 |
| DFS 迭代（栈） | 栈模拟递归，每个元素带路径 | O(n·h) | O(n·h) | 能说出即可 |
| BFS 迭代（队列） | 层序遍历，每个元素带路径 | O(n·h) | O(n·h) | 能说出即可 |

> 注：h 为树高，最坏 O(n)（退化为链），平衡时 O(log n)。时间中的 h 因子来自字符串拷贝/拼接。

## 关键提示

1. **叶子节点的判断**：`!node->left && !node->right`，不是 `node == nullptr`。到 nullptr 时已经"过了"叶子。

2. **箭头 "->" 的拼接时机**：不要在叶子节点后面也加 "->"。推荐在进入子节点前加，或者在非根节点前加。

3. **回溯写法的关键**：push_back 后递归，递归返回后 pop_back。这里"撤销选择"是 pop_back。

4. **示例树的结构**：
```
//       1
//      / \
//     2   3
//      \
//       5
//
// 路径1: 1 → 2 → 5  (叶子是5)
// 路径2: 1 → 3      (叶子是3)
```

5. **字符串传值 vs 传引用**：传值版本天然"回溯"（函数返回后字符串恢复），传引用需要手动回溯。面试时传值写法更简洁不易错。

## 解法详解

### 解法1: DFS 递归（传字符串）— O(n·h) / O(h) ⭐ 面试首选

**思考过程**：最自然的想法——DFS 往下走，把路径信息作为参数传下去。到叶子就收集。字符串按值传递，每层递归都是独立副本，不需要手动回溯。

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
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        if (root) dfs(root, "", result);
        return result;
    }
    
    void dfs(TreeNode* node, string path, vector<string>& result) {
        // 拼接当前节点值（非根节点前面加 "->"）
        if (!path.empty()) path += "->";
        path += to_string(node->val);
        
        // 叶子节点：收集路径
        if (!node->left && !node->right) {
            result.push_back(path);
            return;
        }
        
        // 非叶子：继续往下走（path 是值传递，不需要回溯）
        if (node->left)  dfs(node->left, path, result);
        if (node->right) dfs(node->right, path, result);
    }
};
```

**关键点**：`path` 是按值传递的，每次递归调用都是独立的副本，所以左子树的递归不会影响右子树的 path。这就是"隐式回溯"。

---

### 解法2: DFS 递归（显式回溯）— O(n·h) / O(h) ⭐

**从解法1思考**：解法1 每层都拷贝字符串，有额外开销。如果改用数组传引用 + 手动回溯，可以减少拷贝。这也是回溯模板的标准写法。

```
// 决策树（回溯过程）:
//              [1]
//             /    \
//          [1,2]   [1,3] ← 叶子，收集 "1->3"
//            |         回溯: pop 3
//         [1,2,5] ← 叶子，收集 "1->2->5"
//         回溯: pop 5, pop 2
```

```cpp
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        vector<int> path;
        if (root) backtrack(root, path, result);
        return result;
    }
    
    void backtrack(TreeNode* node, vector<int>& path, vector<string>& result) {
        path.push_back(node->val);   // 做选择
        
        // 叶子节点：把 path 数组转成字符串
        if (!node->left && !node->right) {
            result.push_back(buildPath(path));
        } else {
            if (node->left)  backtrack(node->left, path, result);
            if (node->right) backtrack(node->right, path, result);
        }
        
        path.pop_back();             // 撤销选择（回溯）
    }
    
    string buildPath(const vector<int>& path) {
        string s;
        for (int i = 0; i < path.size(); i++) {
            if (i > 0) s += "->";
            s += to_string(path[i]);
        }
        return s;
    }
};
```

**关键点**：`push_back` 和 `pop_back` 必须配对。回溯的核心就是"恢复现场"。

---

### 解法3: DFS 迭代（栈）— O(n·h) / O(n·h)

**思考过程**：面试追问"不用递归怎么做？"用栈模拟 DFS，每个栈元素保存 `{节点, 当前路径字符串}`。

```cpp
class Solution {
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> result;
        if (!root) return result;
        
        // 栈里同时存节点和到达该节点的路径
        stack<pair<TreeNode*, string>> stk;
        stk.push({root, to_string(root->val)});
        
        while (!stk.empty()) {
            auto [node, path] = stk.top();
            stk.pop();
            
            // 叶子节点：收集
            if (!node->left && !node->right) {
                result.push_back(path);
                continue;
            }
            
            // 非叶子：子节点入栈，路径拼接
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
```

---

### 解法4: BFS 迭代（队列）— O(n·h) / O(n·h)

**思考过程**：把栈换成队列就变成了 BFS。逻辑几乎相同，只是遍历顺序从深度优先变成广度优先。

```cpp
class Solution {
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
```

## 解法对比

| | 解法1 传字符串 | 解法2 回溯 | 解法3 迭代栈 | 解法4 BFS |
|---|---|---|---|---|
| 代码量 | 最少 | 中等 | 中等 | 中等 |
| 回溯方式 | 隐式(值传递) | 显式(pop_back) | 不需要(每个栈元素独立) | 不需要 |
| 空间特点 | 递归栈 O(h) | 递归栈 O(h) | 栈存路径 O(n·h) | 队列存路径 O(n·h) |
| 面试推荐 | ⭐ 首选 | ⭐ 展示回溯 | 追问时写 | 了解即可 |

**面试策略**：先写解法1（简洁），被问"能不能用回溯模板"写解法2，被问"不用递归"写解法3。

## 易错点

1. **✗ 把 nullptr 当叶子判断**
   ```cpp
   // 错误：进入了空节点才判断
   void dfs(TreeNode* node, ...) {
       if (!node) { result.push_back(path); return; }  // ✗ 会在叶子的左右各收集一次！
   }
   ```
   **✓ 正确：在非空节点检查是否叶子**
   ```cpp
   if (!node->left && !node->right) { result.push_back(path); return; }
   ```

2. **✗ 箭头多加或少加**
   ```cpp
   path += "->" + to_string(node->val);  // ✗ 根节点前面也会有 "->"
   ```
   **✓ 先判断 path 是否为空，或者在进入子节点时加箭头。**

3. **✗ 回溯版忘记 pop_back**
   ```cpp
   path.push_back(node->val);
   if (node->left)  backtrack(node->left, path, result);
   if (node->right) backtrack(node->right, path, result);
   // ✗ 忘了 path.pop_back()，路径越来越长
   ```

## 面试追问

**Q1: 时间复杂度怎么分析？为什么不是简单的 O(n)？**
> 每个节点访问一次是 O(n)，但每到叶子节点需要拷贝/拼接整条路径，路径长度最多 O(h)。叶子数最多 O(n/2)，所以总时间 O(n·h)。最坏（退化链表）是 O(n²)，平衡树是 O(n·log n)。

**Q2: 解法1 字符串按值传递和按引用传递有什么区别？**
> 按值传递：每次递归自动创建副本，不需要手动回溯，代码简洁但有拷贝开销。按引用传递：需要手动 append + erase 回溯，性能略好但容易写错。面试中按值传递更稳妥。

**Q3: 如果不是"根到叶子"，而是"根到任意节点"呢？**
> 去掉叶子判断条件，每经过一个节点都把当前路径收集。或者如果要求路径和等于某个值（112. Path Sum），在叶子处判断和是否满足条件。

**Q4: 如果要返回的不是字符串而是节点值数组 `vector<vector<int>>`？**
> 用解法2 的回溯写法更自然。到叶子时直接 `result.push_back(path)` 拷贝当前数组即可。实际上 113. Path Sum II 就是这个变体。

## 相关题型

- **112. 路径总和 (Path Sum)** — 复用同样的根到叶 DFS 框架，区别：不需要记录路径，只需判断和是否等于 target。
- **113. 路径总和 II (Path Sum II)** — 复用解法2 的回溯框架，区别：收集条件从"叶子"变成"叶子且路径和 == target"，返回 `vector<vector<int>>` 而不是字符串。
- **129. 求根节点到叶节点数字之和** — 复用传参数 DFS 框架，区别：传的不是字符串而是数字 `num * 10 + node->val`。
- **988. 从叶结点开始的最小字符串** — 类似但从叶子到根方向构建字符串，需要反向拼接。