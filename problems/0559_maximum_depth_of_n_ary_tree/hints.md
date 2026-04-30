# 559. Maximum Depth of N-ary Tree - N叉树的最大深度

## 核心思路
递归或迭代遍历 N 叉树，找到从根到最远叶节点的路径长度。

## 思维链
1. 与二叉树最大深度有何区别？ → 子节点不止两个，需要遍历 children 数组
2. 递归关系？ → `depth(root) = 1 + max(depth(child) for child in children)`
3. 迭代方法？ → BFS 按层遍历，层数即深度

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ DFS 递归 | O(n) | O(h) | 代码最简洁 |
| BFS 层序遍历 | O(n) | O(w) | 直观统计层数 |

## 关键提示
1. 空节点返回 0
2. 叶节点（无子节点）返回 1
3. 对所有子节点取 max 深度再 +1

## 解法详解

### 解法1: DFS 递归 ⭐
```cpp
class Solution {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        int depth = 0;
        for (Node* child : root->children) {
            depth = max(depth, maxDepth(child));
        }
        return depth + 1;
    }
};
```

### 解法2: BFS 层序遍历
```cpp
class Solution {
public:
    int maxDepth(Node* root) {
        if (!root) return 0;
        queue<Node*> q;
        q.push(root);
        int depth = 0;
        while (!q.empty()) {
            depth++;
            int size = q.size();
            for (int i = 0; i < size; i++) {
                Node* node = q.front(); q.pop();
                for (Node* child : node->children) {
                    q.push(child);
                }
            }
        }
        return depth;
    }
};
```

## 易错点
- ✗ 忘记判空 root → ✓ `if (!root) return 0;`
- ✗ 初始 depth = 1 而非 0 → ✓ 初始 0，最后 +1 包含当前节点
- ✗ BFS 中忘记按层处理（没有内层 for 循环）→ ✓ 必须用 size 控制每层

## 面试追问

**Q1: 如果要求最小深度呢？**
DFS 中对叶节点返回 1，非叶节点取 min(children) + 1。注意空子节点列表的处理。

**Q2: 递归栈溢出怎么办？**
树深度可达 1000，一般不会溢出。若担心，用 BFS 迭代方法。

**Q3: 与二叉树版本的代码区别？**
将 `max(left, right)` 替换为 `for(child : children) max(...)` 即可。

## 相关题型
- [104. Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree/) - 二叉树版本
- [111. Minimum Depth of Binary Tree](https://leetcode.com/problems/minimum-depth-of-binary-tree/) - 最小深度
- [429. N-ary Tree Level Order Traversal](https://leetcode.com/problems/n-ary-tree-level-order-traversal/) - N叉树层序
