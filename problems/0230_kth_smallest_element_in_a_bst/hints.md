# 230. 二叉搜索树中第 K 小的元素

## 核心思路

BST 的中序遍历天然产生升序序列，因此找第 K 小等价于中序遍历时数到第 K 个节点。关键优化点在于**提前终止**——找到第 K 个后立即停止遍历。

## 思维链

1. **识别 BST 性质**：BST 的中序遍历（左-根-右）产生严格递增序列
2. **建立等价关系**：第 K 小元素 = 中序遍历的第 K 个访问节点
3. **选择遍历方式**：递归中序 vs 迭代中序（栈模拟）
4. **加入计数器**：每访问一个节点 `count++`，当 `count == k` 时记录答案
5. **提前终止**：找到答案后不再继续遍历剩余节点
6. **考虑进阶**：若频繁查询 + 频繁修改，可在节点中维护左子树大小 `leftCount`

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 特点 |
|------|-----------|-----------|--------|------|
| 中序遍历（迭代） | O(H + k) | O(H) | ⭐⭐⭐⭐⭐ | 面试首选，提前终止 |
| 中序遍历（递归） | O(H + k) | O(H) | ⭐⭐⭐⭐ | 代码简洁 |
| 增强BST（进阶） | O(H) | O(n) | ⭐⭐⭐ | 频繁查询场景 |

> H = 树高，平衡树 O(log n)，退化链表 O(n)

## 关键提示

```
BST 中序遍历示意:

        5
       / \
      3   6
     / \
    2   4
   /
  1

中序遍历顺序: 1 -> 2 -> 3 -> 4 -> 5 -> 6
              ^k=1 ^k=2 ^k=3

迭代栈的工作过程 (k=3):
  栈: [5]  -> 一路向左
  栈: [5,3] -> 继续向左
  栈: [5,3,2] -> 继续向左
  栈: [5,3,2,1] -> 到底了
  弹出1, count=1 -> 无右子树
  弹出2, count=2 -> 无右子树
  弹出3, count=3 == k -> 返回3 ✓
```

## 解法详解

### 解法1: 迭代中序遍历（栈模拟）

**核心思想**：用栈模拟中序遍历，每弹出一个节点就计数，到第 K 个即返回。

```cpp
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        
        while (curr || !stk.empty()) {
            // 一路向左，压入所有左节点
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            // 弹出栈顶 = 当前最小未访问节点
            curr = stk.top();
            stk.pop();
            // 计数，达到 k 直接返回
            if (--k == 0) return curr->val;
            // 转向右子树
            curr = curr->right;
        }
        
        return -1; // 不会到达
    }
};
```

**复杂度**：时间 O(H + k)，空间 O(H)

### 解法2: 递归中序遍历

**核心思想**：递归中序遍历，用引用参数传递计数器和结果，找到即停止。

```cpp
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        int result = 0;
        int count = 0;
        inorder(root, k, count, result);
        return result;
    }
    
private:
    void inorder(TreeNode* node, int k, int& count, int& result) {
        if (!node || count >= k) return;
        
        inorder(node->left, k, count, result);
        
        if (++count == k) {
            result = node->val;
            return;
        }
        
        inorder(node->right, k, count, result);
    }
};
```

**复杂度**：时间 O(H + k)，空间 O(H)（递归栈）

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `if (count == k)` 后继续递归 | ✓ 找到后立即 return 终止 | 不提前终止会遍历整棵树 |
| ✗ 中序遍历存入数组再取 `arr[k-1]` | ✓ 遍历到第 k 个就返回 | 数组法浪费 O(n) 空间 |
| ✗ 迭代时忘记 `curr = curr->right` | ✓ 弹出后转向右子树 | 否则右子树永远不会被访问 |
| ✗ k 从 0 开始计数 | ✓ 题目 k 从 1 开始（1-indexed） | 注意 `--k == 0` 的写法 |

## 面试追问

**Q1: 迭代和递归哪个更好？为什么？**
→ 面试中迭代更优：(1) 可以真正提前终止，递归需要额外标志位；(2) 不会栈溢出；(3) 更容易改造为迭代器模式。

**Q2: 如果 BST 经常被修改（插入/删除），如何优化频繁的第 K 小查询？**
→ 在每个节点维护 `leftCount`（左子树节点数）。查询时比较 k 与 leftCount：若 k <= leftCount 往左走；若 k == leftCount + 1 当前节点即答案；否则往右走 k -= leftCount + 1。查询 O(H)，插入/删除时同步更新 leftCount 也是 O(H)。

**Q3: 能否用二分思想？**
→ 可以。先统计左子树节点数 cnt：若 k <= cnt，答案在左子树；若 k == cnt+1，答案为 root；否则在右子树找第 k-cnt-1 小。但统计节点数本身 O(n)，除非节点预存子树大小。

## 相关题型

- [94. 二叉树的中序遍历](https://leetcode.com/problems/binary-tree-inorder-traversal/) - 中序遍历基础
- [173. 二叉搜索树迭代器](https://leetcode.com/problems/binary-search-tree-iterator/) - 栈模拟中序遍历的迭代器
- [270. 最接近的二叉搜索树值](https://leetcode.com/problems/closest-binary-search-tree-value/) - BST 搜索变体
- [671. 二叉树中第二小的节点](https://leetcode.com/problems/second-minimum-node-in-a-set-binary-tree/) - 类似"第K小"
