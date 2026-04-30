# 515. 在每个树行中找最大值 (Find Largest Value in Each Tree Row)

**难度**: Medium | **标签**: Tree, Depth-First Search, Breadth-First Search, Binary Tree

## 核心思路

逐层遍历二叉树，记录每层的最大值。BFS 天然按层处理，DFS 则通过记录深度来对应层。

## 思维链

1. "每一层的最大值" → 需要按层分组处理节点
2. BFS 用队列逐层遍历，每层取 max → 最直观
3. DFS 用 depth 参数标记当前层，`res[depth] = max(res[depth], node->val)` → 同样 O(n)
4. 注意空树返回空数组，节点值可能为负数（初始化用 INT_MIN）

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| BFS 层序遍历 | O(n) | O(w) w=最大层宽 | ⭐⭐⭐ |
| DFS 递归 | O(n) | O(h) h=树高 | ⭐⭐ |

## 关键提示

1. BFS: 每层开始前用 `queue.size()` 锁定当前层节点数
2. DFS: `depth == res.size()` 时说明首次到达该层，push_back 初始值
3. 节点值范围 [-2^31, 2^31-1]，初始化最大值必须用 `INT_MIN` 或 `LLONG_MIN`

## 解法详解

### 解法1: BFS 层序遍历

```cpp
class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        vector<int> res;
        if (!root) return res;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            int maxVal = INT_MIN;
            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                maxVal = max(maxVal, node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            res.push_back(maxVal);
        }
        return res;
    }
};
```

### 解法2: DFS 递归

```cpp
class Solution {
public:
    vector<int> res;
    void dfs(TreeNode* node, int depth) {
        if (!node) return;
        if (depth == res.size()) {
            res.push_back(node->val);
        } else {
            res[depth] = max(res[depth], node->val);
        }
        dfs(node->left, depth + 1);
        dfs(node->right, depth + 1);
    }
    vector<int> largestValues(TreeNode* root) {
        dfs(root, 0);
        return res;
    }
};
```

## 易错点

| 错误 | 正确 |
|------|------|
| ✗ 初始化 `maxVal = 0` | ✓ 初始化 `maxVal = INT_MIN`（节点值可能为负） |
| ✗ 忘记判断 `root == nullptr` | ✓ 空树直接返回空数组 |
| ✗ BFS 内层循环用 `q.size()` 作条件 | ✓ 先用变量 `sz` 保存当前层大小 |

## 面试追问

- **Q1**: BFS 和 DFS 哪个更适合这题？
  → BFS 更直观，天然按层处理；DFS 在树极宽时空间更优
- **Q2**: 如果要找每层的最小值呢？
  → 同样逻辑，把 `max` 换成 `min`，初始化用 `INT_MAX`
- **Q3**: 如果树非常深（百万级），DFS 会有什么问题？
  → 递归栈溢出风险，需要改用迭代 DFS 或 BFS

## 相关题型

- [102. 二叉树的层序遍历](https://leetcode.com/problems/binary-tree-level-order-traversal/)
- [199. 二叉树的右视图](https://leetcode.com/problems/binary-tree-right-side-view/)
- [637. 二叉树的层平均值](https://leetcode.com/problems/average-of-levels-in-binary-tree/)
