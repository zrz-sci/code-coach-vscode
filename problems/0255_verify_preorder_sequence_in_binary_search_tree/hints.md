# 255. Verify Preorder Sequence in Binary Search Tree (验证二叉搜索树的前序遍历序列)

**难度**: Medium | **标签**: Stack, Tree, Binary Search Tree, Recursion, Array, Binary Tree, Monotonic Stack

## 核心思路

BST 前序遍历的特点：先访问根节点，再遍历左子树（值都小于根），最后遍历右子树（值都大于根）。利用**单调递减栈**模拟前序遍历：栈维护从根到当前左子树路径上的节点，当遇到比栈顶大的元素时，说明进入了某个节点的右子树，此时弹栈更新下界。

## 思维链

1. **前序遍历特征**: `[根, 左子树...(都<根), 右子树...(都>根)]`
2. **递减序列**: 沿左子树走时，节点值单调递减
3. **转折点**: 当值开始增大，说明进入了某个祖先的右子树
4. **下界维护**: 进入右子树后，所有后续节点必须大于该祖先节点
5. **栈模拟**: 用单调递减栈维护"左侧路径"，弹栈时更新下界 `low`
6. **违规检测**: 如果当前值 <= low，说明序列非法

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 单调递减栈 | O(n) | O(n) | ⭐⭐⭐⭐⭐ |
| 原地栈 (利用数组) | O(n) | O(1) | ⭐⭐⭐⭐ |
| 递归分治 | O(n^2) 最坏 | O(n) | ⭐⭐⭐ |

## 关键提示

- 维护一个变量 `low`（下界），初始化为 `INT_MIN`
- 栈保持单调递减：遇到比栈顶大的元素时，不断弹栈并更新 `low`
- `low` 的含义：当前节点至少应该大于的值（已弹出的最大祖先节点）
- Follow-up 要求 O(1) 空间：可以利用输入数组本身当栈

## 解法详解

### 解法1: 单调递减栈 (最优解)

**思路**: 用栈模拟前序遍历过程。栈中维护从根到当前位置的"左侧祖先路径"。

```cpp
class Solution {
public:
    bool verifyPreorder(vector<int>& preorder) {
        stack<int> stk;
        int low = INT_MIN; // 当前允许的最小值（下界）
        
        for (int val : preorder) {
            // 如果当前值小于下界，说明违反 BST 性质
            if (val < low) return false;
            
            // 弹出所有比当前值小的元素（进入右子树）
            while (!stk.empty() && val > stk.top()) {
                low = stk.top(); // 更新下界
                stk.pop();
            }
            
            // 当前值入栈
            stk.push(val);
        }
        return true;
    }
};
```

**复杂度**: 时间 O(n)，空间 O(n)

### 解法2: 原地栈 (O(1) 空间)

**思路**: 利用输入数组的前部作为栈空间，满足 Follow-up 的常数空间要求。

```cpp
class Solution {
public:
    bool verifyPreorder(vector<int>& preorder) {
        int low = INT_MIN;
        int top = -1; // 栈顶指针
        
        for (int val : preorder) {
            if (val < low) return false;
            
            while (top >= 0 && val > preorder[top]) {
                low = preorder[top];
                top--;
            }
            
            preorder[++top] = val;
        }
        return true;
    }
};
```

**复杂度**: 时间 O(n)，空间 O(1)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `val <= low` 判断 | ✓ `val < low` | 题目说 unique，但 BST 中没有等于的情况 |
| ✗ 不更新 low | ✓ 弹栈时更新 `low = stk.top()` | low 记录进入右子树的分界点 |
| ✗ low 初始化为 0 | ✓ low 初始化为 `INT_MIN` | 节点值可能为负数 |
| ✗ 先 push 再检查 | ✓ 先检查 val < low | 顺序很重要 |

## 面试追问

**Q1**: 为什么用单调递减栈？
→ **A**: BST 前序遍历沿左子树走时值递减，栈模拟这条路径。遇到大值说明转向右子树。

**Q2**: `low` 变量的含义是什么？
→ **A**: 表示"已经离开的最近左子树的根"，进入右子树后所有值必须大于此根。

**Q3**: 如何用递归分治解？
→ **A**: 用上下界递归验证：每个子序列必须在 `(min, max)` 范围内，左子树用 `(min, root)`，右子树用 `(root, max)`。

## 相关题型

- [144. Binary Tree Preorder Traversal](https://leetcode.com/problems/binary-tree-preorder-traversal/) - 前序遍历
- [98. Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/) - 验证 BST
- [331. Verify Preorder Serialization of a Binary Tree](https://leetcode.com/problems/verify-preorder-serialization-of-a-binary-tree/) - 验证前序序列化
