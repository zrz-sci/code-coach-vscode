# 298. Binary Tree Longest Consecutive Sequence (二叉树最长连续序列)

**难度**: Medium | **标签**: Tree, Depth-First Search, Binary Tree

## 核心思路

DFS 遍历二叉树，自顶向下传递当前连续序列长度。如果子节点值 == 父节点值 + 1，则长度加 1；否则重新从 1 开始。全局记录最大长度。

## 思维链

1. "连续序列"定义：父到子方向，值递增 1
2. 不能从子到父（只能自顶向下）
3. 路径可以从任意节点开始 -> 每个节点都可能是新连续序列的起点
4. DFS 时传入父节点值和当前连续长度
5. 子节点值 == 父节点值 + 1 -> 延续序列；否则重置为 1

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| DFS 自顶向下 | 传递连续长度 | O(n) | O(h) | ⭐ |
| BFS 层序 | 队列存 {node, len} | O(n) | O(w) | |
| DFS 自底向上 | 返回以当前节点结尾的长度 | O(n) | O(h) | |

其中 n = 节点数，h = 树高，w = 最大宽度

## 关键提示

```
示例1:
        1
         \
          3
         / \
        2   4
             \
              5

DFS遍历过程 (node, parentVal, curLen -> maxLen):
  1: parent=INT_MIN, len=1 -> max=1
    3: parent=1, 3!=1+1(3!=2)? 错! 3==1+1? 不! 3!=2
       等等... 1+1=2, 3!=2, 所以重置 len=1 -> max=1
    
  再看: 1的值是1, 子节点3的值是3, 3 != 1+1=2, 所以不连续

实际连续路径: 3->4->5 (长度3)
  3: len=1
    4: 4==3+1 ✓ len=2
      5: 5==4+1 ✓ len=3 -> max=3

示例2:
        2
         \
          3
         /
        2
       /
      1

  2->3: 3==2+1 ✓ len=2 -> max=2
  3->2: 2!=3+1  len=1
  2->1: 1!=2+1  len=1
  答案: 2 (路径 2->3)

注意: 3->2->1 虽然递减，但本题要求递增！
```

## 解法详解

### 解法一：DFS 自顶向下（推荐）

```cpp
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        int maxLen = 0;
        dfs(root, nullptr, 0, maxLen);
        return maxLen;
    }
    
private:
    void dfs(TreeNode* node, TreeNode* parent, int curLen, int& maxLen) {
        if (!node) return;
        
        // 判断是否延续连续序列
        if (parent && node->val == parent->val + 1) {
            curLen++;
        } else {
            curLen = 1;
        }
        
        maxLen = max(maxLen, curLen);
        
        dfs(node->left, node, curLen, maxLen);
        dfs(node->right, node, curLen, maxLen);
    }
};
```

### 解法二：DFS 自底向上

```cpp
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        int maxLen = 0;
        dfs(root, maxLen);
        return maxLen;
    }
    
private:
    int dfs(TreeNode* node, int& maxLen) {
        if (!node) return 0;
        
        int leftLen = dfs(node->left, maxLen);
        int rightLen = dfs(node->right, maxLen);
        
        int curLen = 1;
        if (node->left && node->left->val == node->val + 1) {
            curLen = max(curLen, leftLen + 1);
        }
        if (node->right && node->right->val == node->val + 1) {
            curLen = max(curLen, rightLen + 1);
        }
        
        maxLen = max(maxLen, curLen);
        return curLen;
    }
};
```

### 解法三：BFS

```cpp
class Solution {
public:
    int longestConsecutive(TreeNode* root) {
        if (!root) return 0;
        
        int maxLen = 1;
        // {node, current consecutive length}
        queue<pair<TreeNode*, int>> q;
        q.push({root, 1});
        
        while (!q.empty()) {
            auto [node, len] = q.front();
            q.pop();
            
            for (TreeNode* child : {node->left, node->right}) {
                if (!child) continue;
                int newLen = (child->val == node->val + 1) ? len + 1 : 1;
                maxLen = max(maxLen, newLen);
                q.push({child, newLen});
            }
        }
        
        return maxLen;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `node->val == parent->val - 1` | ✓ `node->val == parent->val + 1` | 题目要求递增，不是递减 |
| ✗ 不处理 parent 为空的情况 | ✓ `if (parent && ...)` | 根节点没有父节点 |
| ✗ 初始长度设为 0 | ✓ 初始长度设为 1 | 单个节点也算长度 1 的序列 |
| ✗ 只检查左子树或右子树 | ✓ 两个子树都要检查 | 最长路径可能在右子树 |

## 面试追问

**Q1: 如果允许递减序列或双向连续怎么办？**
> 这就是 LC 549 (Binary Tree Longest Consecutive Sequence II)，需要同时维护递增和递减两个长度，在当前节点尝试拼接左递增+右递减（或反过来）。

**Q2: 如果路径可以穿过根节点（从一个子树到另一个子树）呢？**
> 同上 LC 549 的思路，自底向上返回两个方向的长度，在父节点处合并。

**Q3: 空间复杂度怎么优化？**
> DFS 的空间是 O(h)，对平衡树是 O(log n)，最坏（链表）是 O(n)。Morris 遍历可以做到 O(1) 空间，但实现复杂且不值得。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 549 | Binary Tree Longest Consecutive Sequence II | 进阶版，允许双向 |
| 124 | Binary Tree Maximum Path Sum | 自底向上 + 全局最优 |
| 543 | Diameter of Binary Tree | 类似的 DFS 模式 |
| 687 | Longest Univalue Path | 自底向上求路径长度 |
| 128 | Longest Consecutive Sequence | 数组版连续序列 |
