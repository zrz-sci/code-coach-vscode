# 270. Closest Binary Search Tree Value (最接近的二叉搜索树值)

**难度**: Easy | **标签**: Tree, DFS, Binary Search Tree, Binary Search

## 核心思路

利用 BST 的有序性质，沿搜索路径不断更新最近值。每次比较当前节点与 target，决定向左子树还是右子树走，同时维护全局最优解。

## 思维链

1. **暴力想法**: 中序遍历得到有序数组，线性扫描找最近值 --> O(n)
2. **优化观察**: BST 的搜索路径天然缩小范围，类似二分查找
3. **关键决策**: target < node.val 则向左走（可能更近），否则向右走
4. **边界处理**: 相同距离时返回较小值（题目要求）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| BST迭代搜索 | O(H) | O(1) | ⭐ |
| BST递归搜索 | O(H) | O(H) | |
| 中序遍历 | O(n) | O(n) | |

## 关键提示

```
BST 搜索路径示意:
         4          target = 3.71
        / \
       2   5        4 vs 3.71: |4-3.71|=0.29, 向左
      / \
     1   3          2 vs 3.71: |2-3.71|=1.71, 向右
                    3 vs 3.71: |3-3.71|=0.71
                    
    最终: closest=4 (0.29 < 0.71)
    
搜索决策:
    target < node.val  -->  go LEFT  (左子树可能更近)
    target >= node.val -->  go RIGHT (右子树可能更近)
```

## 解法详解

### 解法一: BST 迭代搜索 (推荐)

```cpp
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        int closest = root->val;
        while (root) {
            // 更新最近值: 距离更小，或距离相同取较小值
            if (abs(root->val - target) < abs(closest - target) ||
                (abs(root->val - target) == abs(closest - target) && root->val < closest)) {
                closest = root->val;
            }
            // BST 性质决定搜索方向
            root = (target < root->val) ? root->left : root->right;
        }
        return closest;
    }
};
```

### 解法二: BST 递归搜索

```cpp
class Solution {
public:
    int closestValue(TreeNode* root, double target) {
        int closest = root->val;
        dfs(root, target, closest);
        return closest;
    }
    
    void dfs(TreeNode* node, double target, int& closest) {
        if (!node) return;
        if (abs(node->val - target) < abs(closest - target) ||
            (abs(node->val - target) == abs(closest - target) && node->val < closest)) {
            closest = node->val;
        }
        if (target < node->val) dfs(node->left, target, closest);
        else dfs(node->right, target, closest);
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `abs(root->val - target) < abs(closest - target)` 仅此条件 | ✓ 还需处理距离相等时取较小值 | 题目要求多个答案返回最小 |
| ✗ `if (target < root->val) root = root->left; else root = root->right;` 漏了等号情况 | ✓ `target < root->val` 即可，等于时左右都行 | target恰好等于节点值时直接返回即可 |
| ✗ 用 `int` 做差值比较 | ✓ 用 `double` 或 `abs()` 配合隐式转换 | val 是 int，target 是 double，差值是 double |

## 面试追问

**Q1: 如果要找 K 个最近的值怎么办?**
--> 用中序遍历 + 双指针，或维护大小为 K 的堆。参见 LeetCode 272。

**Q2: 时间复杂度 O(H) 中 H 最坏是多少?**
--> 退化链表时 H = n，平衡 BST 时 H = log n。

**Q3: 如果有多个等距值，返回所有怎么办?**
--> 维护一个 vector，遇到等距时追加，遇到更近时清空重来。

## 相关题型

- [272. Closest Binary Search Tree Value II](../0272_closest_binary_search_tree_value_ii/) - 找 K 个最近值 (Hard)
- [700. Search in a Binary Search Tree](../0700_search_in_a_binary_search_tree/) - BST 搜索基础
- [701. Insert into a Binary Search Tree](../0701_insert_into_a_binary_search_tree/) - BST 插入
- [230. Kth Smallest Element in a BST](../0230_kth_smallest_element_in_a_bst/) - BST 中序遍历
