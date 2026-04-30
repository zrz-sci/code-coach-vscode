# 206. 反转链表

## 核心思路

本质上就是把链表中每个节点的 `next` 指针方向反过来：原来 A→B 变成 B→A。关键在于反转一个节点的指针时，不能丢失对后续节点的引用。

## 思维链

1. **读完题第一反应**：遍历链表，把每个节点的 `next` 指向前一个节点就行了。但单链表只有 `next`，没有 `prev`，怎么知道"前一个节点"是谁？
2. **最朴素的想法**：先把所有节点值存到数组里，反转数组，再构建新链表。可行但浪费空间，而且没有"原地"反转。
3. **突破关键**：我只需要一个额外变量 `prev` 来记住"前一个节点"，这样遍历时就能把当前节点的 `next` 指向 `prev`。但改了 `next` 之后就走不到下一个节点了，所以还需要一个 `next_temp` 先保存下一个节点。
4. **迭代解法成型**：三个指针 `prev`、`curr`、`next_temp`，每步做四件事：保存下一个 → 反转指针 → prev前进 → curr前进。
5. **递归角度**：如果我假设"后面的链表已经反转好了"，我只需要处理当前节点和下一个节点之间的关系。这就是递归的思路——信任递归帮你搞定子问题，你只处理当前层。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 迭代（双指针） | prev/curr 逐步反转每个指针 | O(n) | O(1) | ⭐ 必须写出 |
| 递归 | 递归到尾部，回溯时反转指针 | O(n) | O(n) 栈空间 | ⭐ 必须写出 |
| 辅助数组 | 存值到数组，反转后重建 | O(n) | O(n) | 能说出即可 |

## 关键提示

- **提示1**：画图！链表题不画图等于盲人摸象。把 `1→2→3→4→5→NULL` 画出来，手动模拟反转过程。

- **提示2**：迭代法的核心问题——当你把节点1的 `next` 从2改成 NULL 时，你怎么找到节点2？答：**先存起来**。

- **提示3**：递归法的关键——假设 `reverseList(head->next)` 已经把 2→3→4→5 反转成了 5→4→3→2→NULL，此时 `head`(节点1) 的 `next` 还指着节点2，而节点2的 `next` 是 NULL。你需要让节点2指回节点1，也就是 `head->next->next = head`。

- **提示4**：别忘了把原来的头节点的 `next` 设为 NULL，否则会形成环！

- **ASCII 图解**：
```
原始链表:
  1 → 2 → 3 → 4 → 5 → NULL

反转后:
  NULL ← 1 ← 2 ← 3 ← 4 ← 5
                              ↑ 新的头
```

## 解法详解

### 解法1: 迭代（双指针）— O(n) / O(1) ⭐ 面试首选

**思考过程**：链表反转的本质是把每条边的方向反过来。遍历链表时，我需要知道三个东西：当前节点 `curr`、前一个节点 `prev`（用来反转指针方向）、下一个节点 `next_temp`（因为反转后就走不到下一个了）。

```
// 迭代过程（逐步模拟）:
//
// 初始:     prev=NULL   curr=1→2→3→NULL
//
// Step 1:   next_temp = 2 (先保存)
//           1→NULL (反转: curr->next = prev)
//           prev=1→NULL, curr=2→3→NULL (双双前进)
//
// Step 2:   next_temp = 3
//           2→1→NULL (反转)
//           prev=2→1→NULL, curr=3→NULL
//
// Step 3:   next_temp = NULL
//           3→2→1→NULL (反转)
//           prev=3→2→1→NULL, curr=NULL
//
// curr==NULL, 循环结束, 返回 prev
```

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr != nullptr) {
            ListNode* next_temp = curr->next;  // 先保存下一个，否则反转后丢失
            curr->next = prev;                  // 反转：当前节点指向前一个
            prev = curr;                        // prev 前进一步
            curr = next_temp;                   // curr 前进一步
        }
        // 循环结束时 curr==NULL，prev 指向原链表最后一个节点（即新头）
        return prev;
    }
};
```

**关键点**：四步操作的顺序不能乱！必须先保存 `next`，再反转，再移动 `prev`，最后移动 `curr`。

---

### 解法2: 递归 — O(n) / O(n)  ⭐ 面试必考

**思考过程**：递归的思路是"信任子问题已经解决"。假设 `reverseList(2→3→4→5)` 已经返回了 `5→4→3→2→NULL`，现在我只需要把节点1接到这个反转链表的尾部。而节点1的 `next` 还指着节点2（反转后的尾部），所以让 `head->next->next = head` 就把节点2指回了节点1。

```
// 递归过程图解:
//
// 原始: 1 → 2 → 3 → 4 → 5 → NULL
//
// 递归到底: head=5, 5->next==NULL → 返回5 (base case)
//
// 回溯 head=4:
//   递归已返回: 5→NULL
//   当前: 4→5→NULL
//   操作: head->next->next = head  →  5→4
//         head->next = nullptr     →  4→NULL (断开旧连接)
//   结果: 5→4→NULL, 返回5
//
// 回溯 head=3:
//   递归已返回: 5→4→NULL
//   当前: 3→4→NULL (3的next还指着4)
//   操作: head->next->next = head  →  4→3
//         head->next = nullptr     →  3→NULL
//   结果: 5→4→3→NULL, 返回5
//
// 回溯 head=2: 5→4→3→2→NULL, 返回5
// 回溯 head=1: 5→4→3→2→1→NULL, 返回5
//
// 最终: 5→4→3→2→1→NULL ✓
```

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // base case: 空链表或只有一个节点，直接返回
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        // 递归反转后面的链表，newHead 是反转后的新头（始终是原链表尾节点）
        ListNode* newHead = reverseList(head->next);
        // 此时 head->next 是反转后子链表的尾节点
        // 让尾节点指回 head，完成当前层的反转
        head->next->next = head;
        // 断开 head 的旧连接，防止成环
        head->next = nullptr;
        // newHead 一路传递到最外层，不变
        return newHead;
    }
};
```

**关键点**：`head->next->next = head` 这一行是精髓。`head->next` 是旧的下一个节点（现在是反转链表的尾节点），让它的 `next` 指回 `head`，就完成了反转。之后必须把 `head->next = nullptr`，否则形成环。

---

### 解法3: 辅助数组 — O(n) / O(n)

**思考过程**：最直觉的方法——遍历链表把值存到数组里，反转数组，再用反转后的值逐个赋回链表节点。这个方法简单但浪费空间，面试中一般只用来说明"我理解了题意"，然后马上给出更优解。

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // 存所有值
        vector<int> vals;
        ListNode* curr = head;
        while (curr) {
            vals.push_back(curr->val);
            curr = curr->next;
        }
        // 反转后写回
        curr = head;
        for (int i = vals.size() - 1; i >= 0; i--) {
            curr->val = vals[i];
            curr = curr->next;
        }
        return head;
    }
};
```

**关键点**：这个方法没有真正改变链表结构，只是改了值。如果节点携带复杂数据（不只是 int），这种方法不适用。

## 解法对比

| | 迭代（双指针） | 递归 | 辅助数组 |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | **O(1)** ✓ | O(n) 递归栈 | O(n) 数组 |
| 代码量 | 5行核心 | 4行核心 | 10行左右 |
| 理解难度 | 低（画图即懂） | 中（需要理解递归信任） | 最低 |
| 面试推荐 | **首选** | **必须也会** | 仅用于破冰 |

**选择建议**：
- 面试中**先写迭代**（O(1) 空间，面试官最满意）
- 面试官追问递归时**再写递归**（展示递归思维能力）
- 如果链表很长（5000节点），递归可能栈溢出，迭代更稳健

## 易错点

1. **迭代法忘记先保存 next**
   - ✗ `curr->next = prev; curr = curr->next;` — 此时 `curr->next` 已经是 `prev` 了，走不到原来的下一个节点！
   - ✓ `ListNode* next_temp = curr->next; curr->next = prev; ... curr = next_temp;`

2. **递归法忘记断开旧连接，形成环**
   - ✗ 只写了 `head->next->next = head;` 而没有 `head->next = nullptr;`
   - 结果：节点1和节点2互相指向，`1 ⇄ 2`，形成死循环
   - ✓ 必须加 `head->next = nullptr;`

3. **递归 base case 漏掉空链表**
   - ✗ `if (head->next == nullptr) return head;` — 当 `head==NULL` 时访问 `head->next` 空指针崩溃
   - ✓ `if (head == nullptr || head->next == nullptr) return head;`

4. **迭代法返回值搞错**
   - ✗ `return curr;` — 循环结束时 curr 是 NULL
   - ✓ `return prev;` — prev 指向原链表最后一个节点，即新头

## 面试追问

**Q1: 迭代和递归的空间复杂度区别是什么？为什么？**
> 迭代 O(1)，只用了 prev/curr/next_temp 三个指针。递归 O(n)，因为递归调用栈深度等于链表长度 n。对于非常长的链表，递归可能导致栈溢出。

**Q2: 如果只反转链表的一部分（比如第 m 到第 n 个节点）怎么办？**
> 这就是 [LeetCode 92: 反转链表 II](https://leetcode.com/problems/reverse-linked-list-ii/)。核心思路：先走到第 m 个节点，对 m 到 n 的部分用本题的迭代法反转，然后把反转后的子链表接回原链表。需要额外记录反转区间的前驱和后继节点。

**Q3: 如果每 k 个节点一组进行反转呢？**
> 这就是 [LeetCode 25: K 个一组翻转链表](https://leetcode.com/problems/reverse-nodes-in-k-group/)（Hard）。核心：先检查剩余节点是否够 k 个，够的话对这 k 个用本题的迭代法反转，然后递归/迭代处理剩余部分。本题的反转操作是那道题的核心子函数。

**Q4: 能否不用额外变量（不用 prev），用"头插法"反转？**
> 可以。维护一个 dummy 节点，每次把当前节点摘下来插到 dummy 后面。本质上和双指针法一样，只是视角不同。

## 相关题型

- **[92. 反转链表 II](https://leetcode.com/problems/reverse-linked-list-ii/)** — 直接复用本题的反转操作，区别是只反转 [m, n] 区间，需要额外处理区间前后的连接
- **[25. K 个一组翻转链表](https://leetcode.com/problems/reverse-nodes-in-k-group/)** — 本题的反转作为子函数，外层加"每 k 个一组"的分组逻辑
- **[234. 回文链表](https://leetcode.com/problems/palindrome-linked-list/)** — 复用本题反转后半部分链表，然后和前半部分逐一比较
- **[24. 两两交换链表中的节点](https://leetcode.com/problems/swap-nodes-in-pairs/)** — k=2 的特殊情况，可以视为 25 题的简化版