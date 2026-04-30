# 563. Binary Tree Tilt - 二叉树的坡度

## 核心思路
后序遍历（自底向上），每个节点返回子树和，同时累加 |左子树和 - 右子树和| 作为坡度。

## 思维链
1. "坡度"定义 = |左子树所有节点值之和 - 右子树所有节点值之和|
2. 要计算每个节点的坡度，必须先知道其左右子树的节点值之和
3. 后序遍历天然先处理子节点 -> 每个节点返回"以自己为根的子树和"
4. 在后序遍历中顺带累加坡度到全局变量

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 后序DFS + 全局累加 | O(n) | O(h) | 最优解，一次遍历 |
| 两次遍历（先算子树和再算坡度） | O(n) | O(n) | 思路直接但冗余 |

## 关键提示
1. 递归函数返回的是**子树和**（不是坡度），坡度通过副作用累加
2. 空节点返回 0
3. 子树和 = 左子树和 + 右子树和 + 当前节点值

## 解法详解

### 解法1: 后序DFS（最优） ⭐
```cpp
class Solution {
    int totalTilt = 0;
    
    int subtreeSum(TreeNode* node) {
        if (!node) return 0;
        int leftSum = subtreeSum(node->left);
        int rightSum = subtreeSum(node->right);
        totalTilt += abs(leftSum - rightSum);  // 累加坡度
        return leftSum + rightSum + node->val;  // 返回子树和
    }
public:
    int findTilt(TreeNode* root) {
        subtreeSum(root);
        return totalTilt;
    }
};
```

## 易错点
- ✗ 递归函数返回坡度 → ✓ 应返回子树和，坡度通过全局变量累加
- ✗ 忘记加上当前节点值 → ✓ 子树和 = left + right + node->val
- ✗ 只算直接子节点差值 → ✓ 是整个子树的值之和的差

## 面试追问

**Q1: 能否不用全局变量？**
可以用引用参数传递累加值，或者返回 pair<子树和, 坡度和>。

**Q2: 如果节点值可能很大导致溢出怎么办？**
使用 long long 类型存储子树和。

**Q3: 迭代方式如何实现？**
使用后序遍历的迭代写法（两个栈或一个栈 + visited 标记），用 hashmap 存每个节点的子树和。

## 相关题型
- [124. Binary Tree Maximum Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum/) - 后序遍历 + 返回值与答案分离
- [543. Diameter of Binary Tree](https://leetcode.com/problems/diameter-of-binary-tree/) - 类似的后序遍历 + 全局变量模式
- [687. Longest Univalue Path](https://leetcode.com/problems/longest-univalue-path/) - 后序遍历统计路径
