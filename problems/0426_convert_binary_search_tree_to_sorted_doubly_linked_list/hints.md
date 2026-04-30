# 426. Convert Binary Search Tree to Sorted Doubly Linked List - 将二叉搜索树转化为排序的双向链表

## 核心思路
中序遍历 BST 得到有序序列，遍历过程中用一个 `prev` 指针将节点依次链接成双向链表，最后首尾相连形成循环。

## 思维链
1. BST 的中序遍历 = 有序序列 → 自然对应排序链表
2. 维护 `prev` 指针，每访问一个节点：`prev->right = cur`, `cur->left = prev`
3. 记录 `head`（第一个访问的节点，即最小值）
4. 遍历结束后，`head->left = last`, `last->right = head` 形成循环

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1 | 递归中序遍历 ⭐ | O(n) | O(h) | 首选 |
| 解法2 | 迭代中序遍历(栈) | O(n) | O(h) | 面试加分 |

## 关键提示
1. `left` 指针复用为链表的 `prev`（前驱），`right` 指针复用为 `next`（后继）
2. 必须是**原地**转换，不能创建新节点
3. 空树需要特判，直接返回 nullptr
4. 循环链表的关键：最后将 head 和 tail 互相链接

## 解法详解

### 解法1: 递归中序遍历
```cpp
class Solution {
    Node* prev = nullptr;
    Node* head = nullptr;
    
    void inorder(Node* cur) {
        if (!cur) return;
        inorder(cur->left);
        
        if (prev) {
            prev->right = cur;
            cur->left = prev;
        } else {
            head = cur; // 最左节点 = 链表头
        }
        prev = cur;
        
        inorder(cur->right);
    }
    
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        inorder(root);
        // 首尾相连形成循环
        head->left = prev;
        prev->right = head;
        return head;
    }
};
```

### 解法2: 迭代中序遍历
```cpp
class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        if (!root) return nullptr;
        
        stack<Node*> stk;
        Node* cur = root;
        Node* prev = nullptr;
        Node* head = nullptr;
        
        while (cur || !stk.empty()) {
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top(); stk.pop();
            
            if (prev) {
                prev->right = cur;
                cur->left = prev;
            } else {
                head = cur;
            }
            prev = cur;
            cur = cur->right;
        }
        
        head->left = prev;
        prev->right = head;
        return head;
    }
};
```

## 易错点
| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 忘记处理空树 `root == nullptr` | ✓ 开头特判空树直接返回 nullptr |
| ✗ 忘记最后首尾相连（不是普通双向链表） | ✓ `head->left = prev; prev->right = head;` |
| ✗ 递归中使用局部变量而非成员变量 | ✓ `prev`/`head` 需要在整个遍历过程中共享 |
| ✗ 中序遍历时修改了 left 导致遍历出错 | ✓ 先递归 left 子树再修改 left 指针，不影响遍历 |

## 面试追问
**Q1: 为什么中序遍历时修改 left 指针不会影响遍历？**
> 因为我们先递归处理完整个左子树后，才修改当前节点的 left。此时左子树已经完全处理完毕，修改 left 不会影响已完成的递归。

**Q2: 能否用 Morris 遍历做到 O(1) 空间？**
> 理论上可以，但 Morris 遍历会临时修改树结构（建立线索），而本题最终也要修改指针，两者会冲突，实现非常复杂，不推荐。

**Q3: 如果输入不是 BST 而是普通二叉树，如何修改？**
> 如果仍要求有序链表，需要先中序遍历收集所有值，排序后构建链表。如果不要求有序，可以用任意遍历顺序链接。

## 相关题型
- [94. Binary Tree Inorder Traversal](https://leetcode.com/problems/binary-tree-inorder-traversal/) - 中序遍历基础
- [897. Increasing Order Search Tree](https://leetcode.com/problems/increasing-order-search-tree/) - BST 转单向链表
- [114. Flatten Binary Tree to Linked List](https://leetcode.com/problems/flatten-binary-tree-to-linked-list/) - 前序展开为链表
