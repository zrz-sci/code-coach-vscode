# 237. 删除链表中的节点

## 核心思路

无法访问前驱节点，所以不能用传统的"修改前驱的 next 指针"的方式。**核心技巧：把下一个节点的值复制过来，然后删除下一个节点**。本质是"用值覆盖"代替"真正删除当前节点"。

## 思维链

1. **分析约束**：只给了待删除节点的指针，没有 head 指针，无法找到前驱节点
2. **传统删除需要前驱**：`prev->next = node->next`，但这里没有 prev
3. **转换思路**：既然删不了当前节点，就把"下一个节点"的信息搬过来
4. **操作步骤**：`node->val = node->next->val`（值覆盖） + `node->next = node->next->next`（跳过下一个节点）
5. **验证可行性**：题目保证待删除节点不是尾节点，所以 `node->next` 一定存在

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 特点 |
|------|-----------|-----------|--------|------|
| 值覆盖 + 删除后继 | O(1) | O(1) | ⭐⭐⭐⭐⭐ | 唯一解法，脑筋急转弯 |

## 关键提示

```
传统删除 vs 本题删除:

传统(有前驱):
  prev -> [node] -> next -> ...
  prev ---------> next -> ...    (prev->next = node->next)

本题(无前驱):
  ??? -> [node:5] -> [next:1] -> [tail:9] -> NULL
  
  Step 1: 复制 next 的值到 node
  ??? -> [node:1] -> [next:1] -> [tail:9] -> NULL
                      ^^^^^^^^
  Step 2: 跳过 next 节点
  ??? -> [node:1] ---------> [tail:9] -> NULL

  效果: 从外部看，值为5的节点"消失"了
  
  具体示例: head = [4,5,1,9], 删除 node=5
  Before: 4 -> [5] -> 1 -> 9 -> NULL
  Copy:   4 -> [1] -> 1 -> 9 -> NULL
  Skip:   4 -> [1] -------> 9 -> NULL
  Result: 4 -> 1 -> 9  ✓
```

## 解法详解

### 解法1: 值覆盖 + 删除后继节点

**核心思想**：无法删除当前节点就"冒名顶替"——把下一个节点的值复制到当前节点，然后删除下一个节点。

```cpp
class Solution {
public:
    void deleteNode(ListNode* node) {
        // 将下一个节点的值复制到当前节点
        node->val = node->next->val;
        // 跳过下一个节点（等效于删除下一个节点）
        node->next = node->next->next;
    }
};
```

**复杂度**：时间 O(1)，空间 O(1)

### 解法2: 思考扩展——如果需要释放内存

```cpp
class Solution {
public:
    void deleteNode(ListNode* node) {
        ListNode* toDelete = node->next;
        node->val = toDelete->val;
        node->next = toDelete->next;
        delete toDelete; // 释放被跳过的节点内存
    }
};
```

**复杂度**：时间 O(1)，空间 O(1)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 试图找到 head 或 prev | ✓ 直接值覆盖 + 删除后继 | 没有 head 指针，找不到前驱 |
| ✗ `node = node->next` | ✓ `node->val = node->next->val` | 修改局部指针变量不影响链表结构 |
| ✗ 忘记更新 next 指针 | ✓ `node->next = node->next->next` | 不跳过后继节点会导致重复值 |
| ✗ 对尾节点使用此方法 | ✓ 题目保证不是尾节点 | 尾节点的 next 为 NULL，会段错误 |

## 面试追问

**Q1: 这种"值覆盖"的方法有什么局限性？**
→ (1) 不能处理尾节点；(2) 如果节点中有其他外部引用（比如其他指针指向被删除的 next 节点），这些引用会失效；(3) 如果节点存储的数据很大（如大对象），复制开销大。

**Q2: 如果要删除倒数第 K 个节点呢？**
→ 用快慢指针。快指针先走 K 步，然后快慢同时走，快指针到尾时慢指针指向倒数第 K 个的前驱。这需要 head 指针。（LC 19）

**Q3: 这道题的实际工程意义是什么？**
→ 在某些场景中（如实时系统），节点对象不可移动（因为有外部引用），这时"值覆盖"方案不可行。面试中要能讨论这种 trade-off。

## 相关题型

- [203. 移除链表元素](https://leetcode.com/problems/remove-linked-list-elements/) - 传统链表删除
- [19. 删除链表的倒数第 N 个结点](https://leetcode.com/problems/remove-nth-node-from-end-of-list/) - 快慢指针删除
- [83. 删除排序链表中的重复元素](https://leetcode.com/problems/remove-duplicates-from-sorted-list/) - 链表去重
- [82. 删除排序链表中的重复元素 II](https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/) - 进阶去重
