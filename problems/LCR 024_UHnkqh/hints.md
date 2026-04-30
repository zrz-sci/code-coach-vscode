# LCR 024. 反转链表

## 核心思路

给定单链表的头节点 head，反转链表并返回新的头节点。这是链表操作的**最基础**也是**最重要**的题目，几乎所有链表高级题都以反转为子操作。两种经典解法：**迭代法**用三个指针逐个翻转，**递归法**先递归到尾部回溯时翻转。

注意：本题与主站第 206 题完全相同。

## 思维链

1. **读完题第一反应**：链表反转 — 把每个节点的 next 指针从"指向下一个"变成"指向上一个"。关键挑战是：改变 next 后会丢失对原来下一个节点的引用。

2. **核心问题**：当我把 A->B 改成 A<-B 时，A 的 next 不再指向 B 了，我怎么继续往后遍历？ → 答案：提前保存 A 的 next（即 B）。

3. **迭代法思路**：维护三个指针 prev, curr, next。每步把 curr->next 从指向 next 改为指向 prev，然后三个指针整体右移。

4. **递归法思路**：先递归到链表末尾，回溯时让"下一个节点的 next"指向自己。即 head->next->next = head，然后 head->next = nullptr。

5. **面试最佳策略**：迭代法写一遍（面试官最常考），然后主动提出递归法并分析递归栈空间。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 迭代（三指针） | prev/curr/next 逐个翻转 | O(n) | O(1) | ⭐ 必须写出 |
| 递归 | 递归到尾部，回溯翻转 | O(n) | O(n) 栈 | ⭐ 必须会说 |
| 头插法 | 每次把头节点移到新链头 | O(n) | O(1) | 能说出即可 |

## 关键提示

1. **迭代的核心四步操作**（背下来）：
   ```
   next = curr->next;   // 1. 暂存下一个
   curr->next = prev;   // 2. 翻转指针
   prev = curr;         // 3. prev 前进
   curr = next;         // 4. curr 前进
   ```

2. **返回值是 prev 不是 curr**：循环结束时 curr == nullptr（越过了尾节点），prev 指向原链表的最后一个节点（新的头节点）。

3. **递归的终止条件**：head == nullptr 或 head->next == nullptr。前者处理空链表，后者处理单节点链表（也是递归到末尾的标志）。

4. **递归的关键操作**：`head->next->next = head` — 让下一个节点的 next 反过来指向自己。然后 `head->next = nullptr` — 断开原来的方向。

5. **空链表和单节点**：两种解法都自然处理了这两种边界情况。

## 解法详解

### 解法1: 迭代（三指针） ⭐

**思考过程**：

以 head = [1, 2, 3, 4, 5] 为例，画指针变化图：

```
初始状态:
prev = nullptr
curr = 1
      prev  curr
       |     |
null   1 --> 2 --> 3 --> 4 --> 5 --> null

Step 1: next = curr->next (= 2)
        curr->next = prev (1->null)
        prev = curr (= 1)
        curr = next (= 2)

null <-- 1    2 --> 3 --> 4 --> 5 --> null
         |    |
        prev curr

Step 2: next = 3
        curr->next = prev (2->1)
        prev = 2, curr = 3

null <-- 1 <-- 2    3 --> 4 --> 5 --> null
               |    |
              prev  curr

Step 3: next = 4
        2->3 变为 3->2
        prev = 3, curr = 4

null <-- 1 <-- 2 <-- 3    4 --> 5 --> null
                     |    |
                    prev  curr

Step 4: next = 5
        prev = 4, curr = 5

null <-- 1 <-- 2 <-- 3 <-- 4    5 --> null
                           |    |
                          prev  curr

Step 5: next = null
        prev = 5, curr = null

null <-- 1 <-- 2 <-- 3 <-- 4 <-- 5    null
                                  |     |
                                 prev  curr

循环结束 (curr == null)，返回 prev = 5
结果: 5 --> 4 --> 3 --> 2 --> 1 --> null
```

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        
        while (curr != nullptr) {
            ListNode* next = curr->next;  // 1. 暂存下一个节点
            curr->next = prev;            // 2. 翻转：当前节点指向前一个
            prev = curr;                  // 3. prev 右移
            curr = next;                  // 4. curr 右移
        }
        
        return prev;  // 新的头节点
    }
};
```

**复杂度分析**：
- 时间：O(n)，遍历链表一次
- 空间：O(1)，只用了三个指针变量

---

### 解法2: 递归

**思考过程**：

递归的核心思想：假设"后面的部分已经反转好了"，我只需要把当前节点"接"到反转好的链表的末尾。

```
原始: 1 --> 2 --> 3 --> 4 --> 5

递归下去:
  reverseList(1->2->3->4->5)
    reverseList(2->3->4->5)
      reverseList(3->4->5)
        reverseList(4->5)
          reverseList(5)  → 返回 5 (base case)

回溯:
  处理节点 4:
    4->next->next = 4  即 5->next = 4
    4->next = null
    null <-- 4 <-- 5
    返回 newHead = 5

  处理节点 3:
    3->next->next = 3  即 4->next = 3
    3->next = null
    null <-- 3 <-- 4 <-- 5
    返回 newHead = 5

  处理节点 2:
    2->next->next = 2  即 3->next = 2
    2->next = null
    null <-- 2 <-- 3 <-- 4 <-- 5
    返回 newHead = 5

  处理节点 1:
    1->next->next = 1  即 2->next = 1
    1->next = null
    null <-- 1 <-- 2 <-- 3 <-- 4 <-- 5
    返回 newHead = 5
```

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // 终止条件：空链表或单节点
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        
        // 递归反转后面的部分，newHead 是新的头节点
        ListNode* newHead = reverseList(head->next);
        
        // 关键操作：让下一个节点反过来指向自己
        head->next->next = head;
        // 断开原来的方向，防止形成环
        head->next = nullptr;
        
        // 新头节点一路传递回去
        return newHead;
    }
};
```

**复杂度分析**：
- 时间：O(n)，递归访问每个节点一次
- 空间：O(n)，递归调用栈深度为 n

---

### 解法3: 头插法

**思考过程**：创建一个虚拟头节点 dummy，遍历原链表，每次把当前节点"插入"到 dummy 的后面。

```
初始: dummy -> null, curr = 1->2->3->4->5

Step 1: 取出 1, 插到 dummy 后面
  dummy -> 1 -> null
  curr = 2->3->4->5

Step 2: 取出 2, 插到 dummy 后面
  dummy -> 2 -> 1 -> null
  curr = 3->4->5

Step 3: 取出 3
  dummy -> 3 -> 2 -> 1 -> null

Step 4: 取出 4
  dummy -> 4 -> 3 -> 2 -> 1 -> null

Step 5: 取出 5
  dummy -> 5 -> 4 -> 3 -> 2 -> 1 -> null

返回 dummy->next = 5
```

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode dummy(0);  // 虚拟头节点
        ListNode* curr = head;
        
        while (curr != nullptr) {
            ListNode* next = curr->next;  // 暂存
            curr->next = dummy.next;      // 当前节点指向 dummy 后面的节点
            dummy.next = curr;            // dummy 的 next 指向当前节点
            curr = next;                  // 继续下一个
        }
        
        return dummy.next;
    }
};
```

## 解法对比

| 维度 | 迭代（三指针） | 递归 | 头插法 |
|------|---------------|------|--------|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(1) | O(n) 栈空间 | O(1) |
| 代码简洁度 | 最简洁 | 简洁但需理解 | 中等 |
| 可读性 | 高 | 中 | 高 |
| 面试推荐 | 首选 | 必须会 | 了解 |
| 适合扩展到反转部分链表 | 适合 | 较难改动 | 适合 |

## 易错点

1. **返回 curr 而非 prev**
   ```cpp
   // 错误：循环结束时 curr == nullptr！
   while (curr) {
       // ... 翻转 ...
   }
   return curr;  // 返回的是 nullptr！应该返回 prev
   ```

2. **递归中忘记断开原方向**
   ```cpp
   // 错误：没有 head->next = nullptr
   head->next->next = head;
   // head->next = nullptr;  // 少了这一步！
   // 后果：原链表第一个节点的 next 仍然指向第二个节点
   //       形成了 1 <-> 2 的环！
   ```

3. **递归中搞混 head 和 head->next**
   ```cpp
   // 错误：不是 head->next = head, 而是 head->next->next = head
   head->next = head;       // 错！这让自己指向自己，形成自环
   head->next->next = head; // 对！让下一个节点反指自己
   ```

4. **忘记处理空链表**
   ```cpp
   // 如果 head == nullptr，直接解引用 head->next 会 crash
   // 迭代法自然处理（while 不进入）
   // 递归法需要 if (!head) return head;
   ```

5. **next 变量声明位置不对**
   ```cpp
   // 错误：next 在循环外声明但循环内赋值，逻辑上没问题
   // 但如果手误把赋值放在了翻转之后...
   ListNode* next;
   while (curr) {
       curr->next = prev;    // 翻转了！
       next = curr->next;    // 此时 curr->next 已经变成 prev 了！next 取错了！
       // 正确顺序：先保存 next，再翻转
   }
   ```

## 面试追问

### 追问链1: 从基础到进阶

**Q1: 迭代法中，能不能只用两个指针？**

可以，把 next 的保存放在移动时做：
```cpp
while (curr) {
    ListNode* tmp = curr->next; // 仍然需要一个临时变量
    curr->next = prev;
    prev = curr;
    curr = tmp;
}
```
本质上还是三个变量（prev, curr, tmp），只是 tmp 是局部的。真正只用两个指针无法完成反转（因为修改 curr->next 后会丢失对下一个节点的引用）。

**Q2: 如果要反转链表的一部分（第 m 到第 n 个节点），怎么做？**

这是 [92. 反转链表 II](../0092_reverse_linked_list_ii/hints.md) 的问题。需要：
1. 找到第 m-1 个节点（连接点）
2. 反转第 m 到第 n 个节点
3. 重新连接两端

**Q3: 如果是双向链表呢？**

更简单：只需要交换每个节点的 prev 和 next 指针，不需要额外保存。

### 追问链2: 递归深入

**Q4: 递归的空间复杂度 O(n)，链表很长时会栈溢出，怎么办？**

1. 改用迭代法（推荐）
2. 使用尾递归优化（但 C++ 不保证尾递归优化）
3. 手动模拟栈：用显式栈来模拟递归过程

**Q5: 能把递归法改成尾递归吗？**

```cpp
ListNode* reverseHelper(ListNode* curr, ListNode* prev) {
    if (!curr) return prev;
    ListNode* next = curr->next;
    curr->next = prev;
    return reverseHelper(next, curr); // 尾递归
}
ListNode* reverseList(ListNode* head) {
    return reverseHelper(head, nullptr);
}
```
这个尾递归版本在某些编译器优化下可以避免栈增长。

### 追问链3: 应用场景

**Q6: 反转链表在哪些题目中作为子操作出现？**

- [25. K 个一组翻转链表](../0025_reverse_nodes_in_k_group/hints.md)
- [143. 重排链表](../0143_reorder_list/hints.md)（反转后半部分 + 交错合并）
- [234. 回文链表](../0234_palindrome_linked_list/hints.md)（反转后半部分 + 比较）
- [92. 反转链表 II](../0092_reverse_linked_list_ii/hints.md)

**Q7: 能否不改变原链表，返回一个新的反转链表？**

可以，创建新节点，用头插法构建：
```cpp
ListNode* reverseNew(ListNode* head) {
    ListNode* newHead = nullptr;
    while (head) {
        newHead = new ListNode(head->val, newHead);
        head = head->next;
    }
    return newHead;
}
```
时间 O(n)，空间 O(n)。

## 相关题型

| 题目 | 关系 |
|------|------|
| [92. 反转链表 II](../0092_reverse_linked_list_ii/hints.md) | 进阶：反转部分链表 |
| [25. K个一组翻转链表](../0025_reverse_nodes_in_k_group/hints.md) | 进阶：分段反转 |
| [234. 回文链表](../0234_palindrome_linked_list/hints.md) | 应用：反转后半段比较 |
| [143. 重排链表](../0143_reorder_list/hints.md) | 应用：反转后半段交错 |
| [206. 反转链表](../0206_reverse_linked_list/hints.md) | 同题：主站版本 |
| [86. 分隔链表](../0086_partition_list/hints.md) | 同类：链表指针操作 |
