# 1110. 删点成林 (Delete Nodes And Return Forest)

## 题目理解

给定一棵二叉树（节点值各不相同），和一个待删除节点值的列表 `to_delete`。删除所有指定节点后，原来的一棵树会分裂成一个"森林"（多棵不相交的树）。返回森林中所有树的根节点。

**核心难点**：删除一个节点后，它的子节点变成新的树根 —— 但前提是子节点本身没有被删除。需要一种优雅的遍历方式来处理这种"断开"和"提升"的关系。

---

## ASCII 树图示例

### 示例 1: `root = [1,2,3,4,5,6,7]`, `to_delete = [3, 5]`

```
原始树:
          1
         / \
        2   3        ← 3 要删除
       / \ / \
      4  5 6  7      ← 5 要删除

删除 3 和 5 后:

    树1:     树2:    树3:
      1        6       7
     /
    2
   /
  4

过程拆解:
          1
         / \
        2   X(3)       ← 删除节点3，其子节点6和7"被提升"为新根
       / \   / \
      4  X(5) [6] [7]  ← 删除节点5，其子节点(无)不影响
         (无子节点)

结果: [[1,2,null,4], [6], [7]]
```

### 示例 2: `root = [1,2,4,null,3]`, `to_delete = [3]`

```
原始树:
      1
     / \
    2   4
     \
      3           ← 3 要删除 (叶节点)

删除 3 后:
      1
     / \
    2   4         ← 2 的右子节点被断开
    (right=null)

结果: [[1,2,4]]   ← 只有一棵树
```

### DFS 遍历过程详解

```
以 root=[1,2,3,4,5,6,7], to_delete=[3,5] 为例:

调用 dfs(1, isRoot=true)
  │
  ├─ 1 不在删除列表 且 isRoot=true → 加入结果 res=[1]
  │
  ├─ 递归左: dfs(2, isRoot=false)  (1不被删, 子节点不是新根)
  │   │
  │   ├─ 2 不在删除列表 且 isRoot=false → 不加入
  │   │
  │   ├─ 递归左: dfs(4, isRoot=false) → return 4 (保留)
  │   │
  │   ├─ 递归右: dfs(5, isRoot=false)
  │   │   │  5 在删除列表!
  │   │   │  递归左: dfs(null, isRoot=true) → null
  │   │   │  递归右: dfs(null, isRoot=true) → null
  │   │   └─ return null (删除5, 断开与父节点2的连接)
  │   │
  │   └─ 2->right = null, return 2
  │
  ├─ 递归右: dfs(3, isRoot=false)
  │   │  3 在删除列表!
  │   │
  │   ├─ 递归左: dfs(6, isRoot=true)  ← 父节点3被删, 6是新根!
  │   │   6 不在删除列表 且 isRoot=true → 加入 res=[1, 6]
  │   │   └─ return 6
  │   │
  │   ├─ 递归右: dfs(7, isRoot=true)  ← 父节点3被删, 7是新根!
  │   │   7 不在删除列表 且 isRoot=true → 加入 res=[1, 6, 7]
  │   │   └─ return 7
  │   │
  │   └─ return null (删除3, 断开与父节点1的连接)
  │
  └─ 1->right = null

最终树结构:
  树1(根=1):     树2(根=6):   树3(根=7):
      1              6            7
     /
    2
   /
  4

结果: [1, 6, 7] ✓
```

---

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|---------|
| DFS 后序 + isRoot 标记 | 自顶向下传递"是否为根" | O(n) | O(n) | 最优、最简洁 |
| BFS + 父节点哈希 | 层序遍历，记录父子关系 | O(n) | O(n) | 可选替代 |

---

## 渐进提示

### 提示 1 — 删除节点的两个后果
删除一个节点 X 会导致什么？

<details>
<summary>展开</summary>

1. **向上**：X 与其父节点断开连接（父节点的 left/right 指针变为 null）
2. **向下**：X 的子节点成为新的独立树根（如果子节点存在且不被删除）

这两个操作需要在遍历过程中同时处理。

</details>

### 提示 2 — 为什么用后序遍历？
前序、中序、后序，哪种遍历最适合？

<details>
<summary>展开</summary>

其实这道题可以用**前序的变体** —— 关键不在于访问顺序，而在于信息传递的方向。

我们需要**自顶向下**传递信息（"你的父节点是否被删除了？"），然后**自底向上**返回信息（"我是否被删除了？返回 null 或 self"）。

DFS 的框架：
1. 判断当前节点是否是新根（由父节点传入的 `isRoot` 决定）
2. 递归处理子树（传入"当前节点是否被删"作为子节点的 `isRoot`）
3. 返回当前节点或 null（让父节点断开连接）

</details>

### 提示 3 — isRoot 参数的含义
如何判断一个节点应该被加入结果列表？

<details>
<summary>展开</summary>

一个节点成为森林中某棵树的根，当且仅当：
1. **它自己没有被删除**
2. **它的父节点被删除了（或它就是原始树的根）**

用 `isRoot` 参数传递条件2：
- 原始调用 `dfs(root, true)`（原始根一定是"根候选"）
- 如果当前节点被删除，递归调用子节点时传 `isRoot = true`
- 如果当前节点保留，递归调用子节点时传 `isRoot = false`

然后在 DFS 开头检查：
```
if (isRoot && !shouldDelete) → 加入结果
```

</details>

### 提示 4 — 返回值的作用
DFS 函数返回什么？

<details>
<summary>展开</summary>

返回值用于**断开父子连接**：
- 如果当前节点被删除 → 返回 `nullptr`
- 如果当前节点保留 → 返回自身

父节点用返回值更新自己的子指针：
```cpp
node->left = dfs(node->left, del);   // 如果左子被删，自动变 null
node->right = dfs(node->right, del);
return del ? nullptr : node;
```

这比手动维护父节点指针简洁得多。

</details>

### 提示 5 — 完整代码骨架

<details>
<summary>展开</summary>

```cpp
class Solution {
    set<int> deleteSet;
    vector<TreeNode*> result;

    TreeNode* dfs(TreeNode* node, bool isRoot) {
        if (!node) return nullptr;

        bool shouldDelete = deleteSet.count(node->val);

        // 如果是根且不删除，收集为森林中的一棵树
        if (isRoot && !shouldDelete) {
            result.push_back(node);
        }

        // 递归子树：如果当前被删，子节点成为新根候选
        node->left  = dfs(node->left,  shouldDelete);
        node->right = dfs(node->right, shouldDelete);

        // 返回：删除则返回null断开连接，否则返回自身
        return shouldDelete ? nullptr : node;
    }
};
```

</details>

---

## 关键思维模式

### 1. "自顶向下传递 + 自底向上返回"
- **向下传递**：`isRoot` 告诉子节点"你是否应该作为新根"
- **向上返回**：返回值告诉父节点"我是否还存在"
- 这种双向信息传递是树问题中非常常见的模式

### 2. "返回值断开连接"模式
用 DFS 的返回值来修改树的结构（断开/重连指针），比维护父节点信息更简洁。类似的题目：
- 删除 BST 中的节点（返回新的子树根）
- 修剪 BST（返回修剪后的子树根）

### 3. "HashSet 预处理"
将 `to_delete` 转为 `unordered_set`，将查找从 O(m) 降为 O(1)。对于树中每个节点都要查询一次，这个优化是必要的。

---

## 复杂度分析

**时间复杂度**：O(n + m)
- 构建 HashSet：O(m)，m = to_delete 的长度
- DFS 遍历每个节点一次：O(n)
- 每个节点的操作（查 HashSet、指针赋值）：O(1)

**空间复杂度**：O(n + m)
- HashSet：O(m)
- DFS 递归栈：O(h)，h 是树高，最坏 O(n)
- 结果数组：O(n) 最坏情况

---

## 常见错误

1. **忘记处理根节点**：原始根也可能被删除，必须对根调用 `dfs(root, true)`
2. **先删除再递归**：如果先删除当前节点再递归子树，会丢失子树信息。必须先递归（或同时处理）
3. **没有断开连接**：删除节点后忘记将父节点的指针设为 null，导致结果树中仍保留已删除的子树
4. **isRoot 传递错误**：当前节点被删时，子节点的 isRoot 应为 true，反之为 false

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 450 | Delete Node in a BST | 树中删除节点，返回值修改结构 |
| 669 | Trim a Binary Search Tree | 返回值修改树结构 |
| 814 | Binary Tree Pruning | 后序遍历 + 返回值剪枝 |
| 865 | Smallest Subtree with all the Deepest Nodes | 后序遍历 + 信息上传 |

---

## 进阶思考

### 如果要删除后恢复原树？
可以在删除前先序列化（或深拷贝）树。也可以记录所有被修改的指针（parent, left/right, old_value），实现一个 undo 操作。

### 如果 to_delete 可以动态更新？
当前方案是一次性批量删除。如果需要支持增量删除，可以考虑用 Link-Cut Tree 或 Euler Tour Tree 等高级数据结构维护森林。

### BFS 替代方案
也可以用 BFS 层序遍历，在出队时判断是否删除，手动断开子节点连接并将子节点作为新根。但代码没有 DFS 版本简洁。

---

## 面试追问

**Q1: 如果节点值不唯一（可能重复），方案需要怎么改？**
> 题目保证节点值唯一（1 <= val <= 1000, n <= 1000）。若值重复，`to_delete` 需改为删除特定节点指针（而非值），用 `unordered_set<TreeNode*>` 替代 `unordered_set<int>`。

**Q2: 如果要求返回结果按特定顺序（如原树的层序），怎么处理？**
> 当前解法收集顺序取决于遍历顺序。可以对 result 按根节点在原树中的深度排序，或在 BFS 解法中用层序天然保证。

**Q3: 如果删除操作是动态的（边删边加新节点），如何设计数据结构？**
> 可以用"带父指针的树 + lazy 删除标记"维护森林。每次删除 O(子节点数) 更新。若需频繁查询"某节点属于哪棵树"，可用并查集维护连通分量。
