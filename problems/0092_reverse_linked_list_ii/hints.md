# 92. 反转链表 II (Reverse Linked List II)

## 难度: Medium
## 标签: Linked List

---

## 核心思路

给定单链表的 head 和两个整数 left、right，反转从位置 left 到位置 right 的链表节点。

**关键洞察**：这道题的核心是「局部反转」——需要精确定位反转段的边界，处理好四个关键节点的指针关系：
1. **prev**: 反转段前一个节点
2. **start/curr**: 反转段的第一个节点（反转后变成最后一个）
3. **end**: 反转段的最后一个节点（反转后变成第一个）
4. **succ**: 反转段后一个节点

使用 **dummy node** 统一处理 left=1 的边界情况。

---

## 思维链（6步推导）

### Step 1: 理解反转需求
- 不是全链表反转，而是区间 [left, right] 的局部反转
- left=1 时头节点会被反转 -> 需要 dummy 节点

### Step 2: 回顾全链表反转
- 三指针法: prev, curr, next
- 循环中: next = curr->next; curr->next = prev; prev = curr; curr = next;
- 能否复用到局部反转？

### Step 3: 方法A - 截断+反转+拼接（分步清晰）
- 定位 start 和 end
- 截断: end->next = nullptr
- 反转截断段
- 重新拼接: prev->next = 新头, 旧头(start)->next = succ

### Step 4: 方法B - 头插法（一次遍历，更优雅）
- 定位到 prev（left 前一个节点）
- curr 始终指向原始 left 位置的节点
- 每次将 curr->next 摘出，插到 prev 后面
- 循环 right-left 次

### Step 5: 画出指针图（面试必画）
```
头插法过程 (left=2, right=4):

原始:  dummy -> 1 -> 2 -> 3 -> 4 -> 5
                ^    ^
               prev  curr

第1轮: 把3摘出插到prev后
       dummy -> 1 -> 3 -> 2 -> 4 -> 5
                ^         ^
               prev      curr (不动!)

第2轮: 把4摘出插到prev后
       dummy -> 1 -> 4 -> 3 -> 2 -> 5
                ^              ^
               prev           curr (不动!)

结果: dummy -> 1 -> 4 -> 3 -> 2 -> 5
```

### Step 6: 验证边界
- left = right: 无需反转，循环 0 次，直接返回
- left = 1: dummy 节点处理，prev = dummy
- 单节点: head = [5], left=1, right=1 -> 返回 [5]

---

## 指针操作详解（头插法核心步骤，逐行分析）

```
每一轮循环中的三步指针操作:

假设当前: prev -> [curr] -> [next] -> [后续...]
         prev -> next 指向 prev.next (即反转段最前面的节点)

步骤1: curr->next = next->next
       把 curr 跳过 next, 连到 next 的后一个
       prev -> [curr] -> [后续...]
                         [next] (断开)

步骤2: next->next = prev->next
       next 连到当前反转段的最前面
       prev -> [curr] -> [后续...]
         [next] -> [prev原来的next] (即反转段最前面)

步骤3: prev->next = next
       prev 连到 next, 完成插入
       prev -> [next] -> [原最前面] -> ... -> [curr] -> [后续...]
```

---

## 解法概览

| 解法 | 时间 | 空间 | 特点 | 推荐度 |
|------|------|------|------|--------|
| Solution1: 头插法 (一次遍历) | O(n) | O(1) | **面试首选**，一次 pass | ⭐⭐⭐⭐⭐ |
| Solution2: 截断+反转+拼接 | O(n) | O(1) | 步骤清晰，容易理解 | ⭐⭐⭐⭐ |
| Solution3: 递归 | O(n) | O(n) | 递归栈开销，不推荐 | ⭐⭐ |

---

## 关键提示

1. **必须用 dummy 节点**：left=1 时头节点被反转，返回值不再是原 head
2. **头插法中 curr 不动**：curr 始终指向原始 left 位置节点，每次移动的是 curr->next
3. **循环次数是 right-left**：反转 k 个节点只需 k-1 次操作
4. **截断法记得恢复**：end->next = nullptr 截断后，start（反转后的尾）要连接 succ

---

## 解法详解

### Solution1: 头插法 - 一次遍历（面试首选 ⭐）

**思考过程**：
- 用 dummy 节点统一 left=1 的边界
- 找到 prev（left 前一个），curr（left 位置）
- 每次把 curr->next 摘出来插到 prev 后面
- curr 本身不动（它最终会成为反转段的最后一个节点）

```cpp
class Solution1 {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        // 走到 left 前一个位置
        for (int i = 1; i < left; i++)
            prev = prev->next;

        // curr 始终指向原始 left 位置 (反转后它会到最后)
        ListNode* curr = prev->next;

        // 头插法: 每次把 curr->next 提到 prev 后面
        for (int i = 0; i < right - left; i++) {
            ListNode* next = curr->next;     // 要移动的节点
            curr->next = next->next;         // curr 跳过 next
            next->next = prev->next;         // next 连到反转段最前面
            prev->next = next;               // prev 连到 next
        }

        return dummy.next;
    }
};
```

**逐步图解 (head=[1,2,3,4,5], left=2, right=4)：**
```
初始:  d -> 1 -> 2 -> 3 -> 4 -> 5
            p    c

i=0:   摘3, 插到prev后
       next = 3
       curr->next = 4  (2跳过3连到4)
       next->next = 2  (3连到prev后的2)
       prev->next = 3  (prev连到3)
       d -> 1 -> 3 -> 2 -> 4 -> 5
            p              c

i=1:   摘4, 插到prev后
       next = 4
       curr->next = 5  (2跳过4连到5)
       next->next = 3  (4连到prev后的3)
       prev->next = 4  (prev连到4)
       d -> 1 -> 4 -> 3 -> 2 -> 5
            p                   c

结果: 1 -> 4 -> 3 -> 2 -> 5
```

---

### Solution2: 截断 + 反转 + 拼接

**思考过程**：
- 把反转段从链表中截出来
- 用标准的全链表反转方法反转
- 再拼接回去

```cpp
class Solution2 {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        // 走到 left 前一个位置
        for (int i = 1; i < left; i++)
            prev = prev->next;

        // 定位反转段的起止
        ListNode* start = prev->next;       // 反转段起始
        ListNode* end = start;
        for (int i = left; i < right; i++)
            end = end->next;                // 反转段结束
        ListNode* succ = end->next;         // 反转段后一个

        // 截断
        end->next = nullptr;

        // 反转截断段, 重新连接
        prev->next = reverse(start);    // prev 连新头(原end)
        start->next = succ;             // start 现在是尾, 连到 succ

        return dummy.next;
    }

    // 标准链表反转
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        while (head) {
            ListNode* n = head->next;
            head->next = prev;
            prev = head;
            head = n;
        }
        return prev;
    }
};
```

**图解：**
```
原始:  d -> 1 -> [2 -> 3 -> 4] -> 5
            p     s         e      succ

截断:  d -> 1    [2 -> 3 -> 4]    5
            p     s         e      succ

反转:  d -> 1    [4 -> 3 -> 2]    5
            p                s     succ

拼接:  d -> 1 -> 4 -> 3 -> 2 -> 5
            p                s
```

---

### Solution3: 递归（了解即可）

**思考过程**：
- 递归地将问题转化为 reverseN（反转前 N 个节点）
- 当 left > 1 时，递归 head->next 并将 left 和 right 各减 1
- 当 left = 1 时，变成反转前 right 个节点的问题

```cpp
class Solution3 {
    ListNode* successor = nullptr;  // 反转段后一个节点

    // 反转前 n 个节点
    ListNode* reverseN(ListNode* head, int n) {
        if (n == 1) {
            successor = head->next;  // 记录后继
            return head;
        }
        ListNode* last = reverseN(head->next, n - 1);
        head->next->next = head;
        head->next = successor;
        return last;
    }

public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if (left == 1) {
            return reverseN(head, right);
        }
        head->next = reverseBetween(head->next, left - 1, right - 1);
        return head;
    }
};
```

---

## 解法对比

| 维度 | Solution1 头插法 | Solution2 截断拼接 | Solution3 递归 |
|------|-----------------|-------------------|---------------|
| 时间复杂度 | O(n) | O(n) | O(n) |
| 空间复杂度 | O(1) | O(1) | O(n) 栈 |
| 遍历次数 | 一次 | 两次(定位+反转) | 一次(递归) |
| 代码难度 | 中等 | 容易理解 | 较难理解 |
| 面试推荐 | **首选** | 作为替代方案 | 不推荐 |

**面试策略**：先画图讲清 Solution1 头插法的过程，指针变化要一步步讲清楚。

---

## 易错点（具体 Bug 示例）

### Bug 1: 没用 dummy 节点，left=1 时头节点丢失
```cpp
// 错误: 直接用 head 做起点
ListNode* prev = head;
for (int i = 1; i < left; i++) prev = prev->next;
// 当 left=1 时, prev = head, 但我们需要 head 之前的节点!
// 修复: ListNode dummy(0, head); ListNode* prev = &dummy;
```

### Bug 2: 头插法中移动了 curr 的位置
```cpp
// 错误: 每轮循环后 curr = curr->next
for (int i = 0; i < right - left; i++) {
    ListNode* next = curr->next;
    curr->next = next->next;
    next->next = prev->next;
    prev->next = next;
    curr = curr->next;  // 错! curr 不应该移动
}
// curr 始终指向原始 left 位置, 它会自动被"推"到反转段末尾
```

### Bug 3: 循环次数搞错
```cpp
// 错误: 循环 right-left+1 次 (多了一次)
for (int i = 0; i <= right - left; i++)  // 应该是 < 不是 <=
// 反转 k 个节点只需 k-1 次操作
// 比如 [2,3,4] 三个节点, 只需 2 次头插
```

### Bug 4: 截断法忘记设 end->next = nullptr
```cpp
// end->next = nullptr 缺失导致 reverse 函数不知道在哪里停止
// 会把整个后续链表都反转了
```

### Bug 5: 截断法拼接时搞错 start 和 end
```cpp
// 反转后 start 变成了尾节点, end 变成了头节点
prev->next = reverse(start);  // 新头是原 end
start->next = succ;           // start 现在是尾, 连后面
// 如果写成 end->next = succ 就错了 (end 现在在中间)
```

---

## 面试追问递进链

### Q1: "能否一次遍历完成？"
**答**：用头插法（Solution1），只需一次遍历，O(n) 时间 O(1) 空间。定位到 prev 后，每次把 curr->next 摘出来插到 prev 后面。

### Q2: "如果要反转每 k 个一组呢？"
**答**：LeetCode 25. Reverse Nodes in k-Group。可以复用本题的区间反转逻辑，每 k 个节点为一组调用。最后不足 k 个不反转。

### Q3: "如果给定的不是位置而是节点值呢？"
**答**：需要先遍历找到 left 和 right 对应的位置，再执行反转。注意值可能重复。

### Q4: "如何判断链表是否有环？和反转有什么关系？"
**答**：Floyd 快慢指针判断环。反转本身不涉及环检测，但如果输入链表有环，反转会进入死循环。面试时要提到这个前提条件。

---

## 相关题型

| 题目 | 关系 | 复用说明 |
|------|------|----------|
| [206. Reverse Linked List](../0206_reverse_linked_list/) | 全反转 | 本题的子问题，Solution2 直接调用 |
| [25. Reverse Nodes in k-Group](../0025_reverse_nodes_in_k_group/) | 每k个反转 | 多次调用本题的区间反转逻辑 |
| [24. Swap Nodes in Pairs](../0024_swap_nodes_in_pairs/) | 两两交换 | k=2 的特殊情况 |
| [234. Palindrome Linked List](../0234_palindrome_linked_list/) | 回文链表 | 反转后半段再比较 |
| [143. Reorder List](../0143_reorder_list/) | 重排链表 | 找中点 + 反转后半段 + 合并 |
