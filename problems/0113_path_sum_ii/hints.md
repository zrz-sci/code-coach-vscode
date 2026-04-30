# 113. 路径总和 II (Path Sum II)

## 难度: Medium | 标签: Tree, Depth-First Search, Backtracking, Binary Tree

---

## 核心思路

找出二叉树中所有**从根到叶子**的路径，使得路径上节点值之和等于 `targetSum`。这是一个经典的**DFS + 回溯**问题：沿着每条路径向下走，维护当前路径和剩余目标值，到达叶子时检查是否满足条件。

## 题目示例 ASCII 图

```
示例1: root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22

              5
             / \
            4   8
           /   / \
          11  13   4
         / \      / \
        7   2    5   1

  路径1: 5 → 4 → 11 → 2  (5+4+11+2 = 22) ✓
  路径2: 5 → 8 → 4 → 5   (5+8+4+5 = 22) ✓
  路径3: 5 → 4 → 11 → 7  (5+4+11+7 = 27) ✗
  路径4: 5 → 8 → 13       (5+8+13 = 26)   ✗
  路径5: 5 → 8 → 4 → 1   (5+8+4+1 = 18)  ✗

  输出: [[5,4,11,2], [5,8,4,5]]
```

```
示例2: root = [1,2,3], targetSum = 5

      1
     / \
    2   3

  路径1: 1 → 2  (1+2 = 3) ✗
  路径2: 1 → 3  (1+3 = 4) ✗

  输出: []  (没有满足条件的路径)
```

## 思维链

1. **读完题第一反应** → 需要遍历所有从根到叶子的路径，检查路径和。自然想到 DFS。
2. **路径记录** → 用一个 `vector<int> path` 记录当前路径上的节点值。进入一个节点时 `push_back`，离开时 `pop_back`（回溯）。
3. **何时收集答案？** → 到达**叶子节点**（左右孩子都为空）时，检查剩余 `remain` 是否等于当前节点值。如果是，把当前 path 加入结果。
4. **剩余值传递** → 每次递归时传递 `remain - node->val`，这样到叶子时只需要检查 `remain == node->val`。
5. **为什么不能提前剪枝？** → 节点值可能为负！即使中间路径和已经超过 target，后面可能有负数把它拉回来。所以必须走到叶子才能判断。
6. **迭代 vs 递归** → 递归更简洁，迭代需要用栈模拟，每个栈帧要存节点、路径副本和剩余值，空间开销更大。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 回溯（递归） | 维护 path，叶子处检查 | O(n * H) | O(H) | ⭐ 面试首选 |
| DFS 迭代（栈） | 栈存 {节点, 路径, 剩余} | O(n * H) | O(n * H) | 追问"不用递归" |
| BFS（队列） | 层序遍历，记录路径 | O(n * H) | O(n * H) | 少见，不推荐 |

> H = 树的高度。时间中的 O(n * H) 是因为每找到一条路径需要 O(H) 拷贝。

## 关键提示

1. **叶子节点判断**：`!node->left && !node->right`，必须左右子节点都为空才算叶子。只有一个子节点为空不算叶子（常见错误）。
2. **回溯的 pop_back**：在递归返回前必须 `path.pop_back()`，恢复状态。如果忘记了，path 会越来越长，结果全部错误。
3. **不能提前剪枝**：因为节点值可能为负数（题目约束 -1000 <= Node.val <= 1000），所以 `remain < 0` 不能作为剪枝条件。
4. **空树处理**：`root == nullptr` 时直接返回空结果。
5. **答案拷贝开销**：每次 `res.push_back(path)` 会拷贝整个 path 向量，所以总时间最坏是 O(n * H)（所有叶子都满足条件时）。

## DFS 回溯过程可视化

```
targetSum = 22

              5              path=[5], remain=17
             / \
            4   8            左: path=[5,4], remain=13
           /   / \           右: path=[5,8], remain=9
          11  13   4
         / \      / \
        7   2    5   1

DFS 执行过程:
  → 进入 5:  path=[5], remain=22-5=17
    → 进入 4:  path=[5,4], remain=17-4=13
      → 进入 11: path=[5,4,11], remain=13-11=2
        → 进入 7:  path=[5,4,11,7], remain=2-7=-5
          叶子! remain≠0 ✗
          pop_back → path=[5,4,11]
        → 进入 2:  path=[5,4,11,2], remain=2-2=0
          叶子! remain==0 ✓ → 收集 [5,4,11,2]
          pop_back → path=[5,4,11]
        pop_back → path=[5,4]
      pop_back → path=[5]
    → 进入 8:  path=[5,8], remain=17-8=9
      → 进入 13: path=[5,8,13], remain=9-13=-4
        叶子! remain≠0 ✗
        pop_back → path=[5,8]
      → 进入 4:  path=[5,8,4], remain=9-4=5
        → 进入 5: path=[5,8,4,5], remain=5-5=0
          叶子! remain==0 ✓ → 收集 [5,8,4,5]
          pop_back → path=[5,8,4]
        → 进入 1: path=[5,8,4,1], remain=5-1=4
          叶子! remain≠0 ✗
          pop_back → path=[5,8,4]
        pop_back → path=[5,8]
      pop_back → path=[5]
    pop_back → path=[]

  结果: [[5,4,11,2], [5,8,4,5]]
```

## 解法详解

### 解法1: DFS 回溯（递归） — O(n * H) / O(H) ⭐ 面试首选

**思考过程**: 每进入一个节点就加入路径，到叶子检查条件，返回时弹出。

```cpp
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        vector<int> path;
        dfs(root, targetSum, path, res);
        return res;
    }
    void dfs(TreeNode* node, int remain, vector<int>& path, vector<vector<int>>& res) {
        if (!node) return;
        path.push_back(node->val);
        if (!node->left && !node->right && remain == node->val)
            res.push_back(path);
        dfs(node->left, remain - node->val, path, res);
        dfs(node->right, remain - node->val, path, res);
        path.pop_back(); // 回溯
    }
};
```

**关键点**:
- `remain == node->val`（而不是 `remain == 0`）：因为我们是在减去当前节点值之前检查的。等价于 `remain - node->val == 0`。
- 即使到了叶子也不 return，因为 `path.pop_back()` 必须执行。不过叶子的左右子节点都是 null，递归会在下一层直接返回。

---

### 解法2: DFS 迭代 — O(n * H) / O(n * H)

```cpp
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        vector<vector<int>> res;
        if (!root) return res;
        stack<tuple<TreeNode*, vector<int>, int>> stk;
        stk.push({root, {root->val}, targetSum - root->val});
        while (!stk.empty()) {
            auto [node, path, remain] = stk.top(); stk.pop();
            if (!node->left && !node->right && remain == 0)
                res.push_back(path);
            for (auto child : {node->right, node->left}) {
                if (child) {
                    auto p = path; // 拷贝路径
                    p.push_back(child->val);
                    stk.push({child, p, remain - child->val});
                }
            }
        }
        return res;
    }
};
```

---

## 易错点

1. **必须到叶子节点才能判断**：`!left && !right` 缺一不可。如果只判断 `remain == 0` 而不管是否是叶子，会把非叶子节点的路径误收集。
2. **回溯时 pop_back 不能忘**：忘了 pop_back 是最常见的 bug，会导致路径越来越长。
3. **节点值可能为负**：不能用 `remain < 0` 提前剪枝。
4. **迭代解法的路径拷贝**：栈中存的是路径的副本（而不是引用），每次入栈都要拷贝，空间开销大。这是迭代解法比递归慢的原因。

## 面试追问递进链

1. **"只判断是否存在满足条件的路径？"** → [112. Path Sum](../0112_path_sum/)，不需要记录路径，递归更简单。
2. **"路径不一定从根开始？"** → [437. Path Sum III](../0437_path_sum_iii/)，用前缀和 + 哈希表。
3. **"不是树而是图？"** → 需要 visited 集合避免环。
4. **"能否用 BFS？"** → 可以，但要在队列中存路径副本，效率不如 DFS 回溯。
5. **"时间复杂度为什么是 O(n * H)？"** → 遍历 n 个节点是 O(n)，但每找到一个答案要拷贝长度为 H 的路径。最坏情况所有叶子都满足条件，叶子数可达 n/2，每条路径长 H。

## 复杂度分析

| 解法 | 时间 | 空间 | 说明 |
|------|------|------|------|
| DFS 回溯 | O(n * H) | O(H) | H=树高，递归栈+path 共 O(H)，不含结果空间 |
| 迭代 DFS | O(n * H) | O(n * H) | 栈中每个帧都存路径副本 |

## 相关题型

- [112. Path Sum](../0112_path_sum/) — 只判断是否存在路径
- [437. Path Sum III](../0437_path_sum_iii/) — 任意起点路径，前缀和
- [129. Sum Root to Leaf Numbers](../0129_sum_root_to_leaf_numbers/) — 根到叶路径组成的数
- [257. Binary Tree Paths](../0257_binary_tree_paths/) — 返回所有根到叶路径
- [124. Binary Tree Maximum Path Sum](../0124_binary_tree_maximum_path_sum/) — 最大路径和（任意路径）
