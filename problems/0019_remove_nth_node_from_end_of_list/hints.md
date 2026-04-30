# 19. 删除链表的倒数第 N 个结点

## 核心思路

这道题的本质是：**如何在单向链表中定位"倒数第 N 个节点"**。单向链表无法从后往前遍历，所以关键问题是把"倒数"转化为"正数"。

## 思维链

1. **读完题第一反应**：链表不知道总长度，而"倒数第 N 个"需要知道总长度才能换算成正数位置 → 最朴素的做法：先遍历一遍数长度 L，倒数第 N 个就是正数第 L-N+1 个，再遍历一次找到它删掉。

2. **暴力解的瓶颈**：需要遍历两次链表（一次数长度，一次定位节点）。题目 Follow-up 问：能不能一趟扫描？

3. **怎么只遍历一次？** 核心观察：倒数第 N 个节点到链表末尾的距离是 N。如果有两个指针，一个先走 N 步，然后两个指针一起走，当先走的指针到达末尾时，后走的指针正好指向倒数第 N 个节点！

4. **具体做法**：快指针先走 N 步 → 快慢一起走直到快指针到末尾 → 慢指针指向的就是要删除的节点。但删除需要知道"前一个节点"，所以让慢指针停在要删除节点的**前一个位置**。

5. **边界处理**：如果要删除的恰好是头节点怎么办？用 **dummy head（虚拟头节点）** 统一处理，避免特判。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 两次遍历 | 先数长度，再定位删除 | O(L) | O(1) | 能说出即可 |
| 双指针（快慢指针） | 快指针先走 N 步，同步走到末尾 | O(L) | O(1) | ⭐ 必须写出 |
| 递归 | 递归到末尾，回溯时计数 | O(L) | O(L) 栈空间 | 加分项 |

## 关键提示

- **提示1**：倒数第 N 个 = 正数第 (L - N + 1) 个。如果你知道 L，问题就变成了正向定位。

- **提示2**：不想数长度？想想两个人在跑道上跑步，一个人先跑 N 米，然后两人同速同时跑，先跑的到终点时，后跑的离终点恰好 N 米。

- **提示3**：删除节点需要操作**前驱节点**的 next 指针。如果要删除的是头节点，谁是它的前驱？→ 用 dummy node。

- **提示4**：画图！链表题不画图很容易搞混指针。

```
  删除倒数第2个（n=2）:
  
  dummy → 1 → 2 → 3 → [4] → 5 → NULL
                   ↑          ↑
                  slow       fast (fast 到 NULL 时 slow 在要删节点的前一个)
  
  slow->next = slow->next->next  即: 3→5，跳过了4
```

- **提示5**：快指针到底走到 NULL 还是走到最后一个节点？这取决于你让慢指针从哪里开始。如果都从 dummy 开始，快指针多走 N+1 步（让 slow 停在被删节点的前一个）。

## 解法详解

### 解法1: 两次遍历 — O(L) / O(1)

**思考过程**: 最直接的想法——不知道长度就先数出来。倒数第 N 个就是正数第 L-N+1 个，第二次遍历走到第 L-N 个节点（即目标的前驱），执行删除。

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // dummy 节点统一处理"删除头节点"的情况
        ListNode dummy(0, head);
        
        // 第一次遍历：数总长度
        int length = 0;
        ListNode* curr = head;
        while (curr) {
            length++;
            curr = curr->next;
        }
        
        // 第二次遍历：走到倒数第 N 个节点的前驱（即正数第 L-N 个）
        // 从 dummy 开始走 L-N 步
        curr = &dummy;
        for (int i = 0; i < length - n; i++) {
            curr = curr->next;
        }
        
        // 删除 curr 的下一个节点
        ListNode* toDelete = curr->next;
        curr->next = toDelete->next;
        delete toDelete;  // 释放内存（面试中提到加分）
        
        return dummy.next;
    }
};
```

**关键点**: 
- 从 dummy 开始走 `L-N` 步到达被删节点的前驱，不是从 head 开始。
- dummy 的作用：当 n == L（删头节点）时，`L - N = 0`，curr 就是 dummy，`dummy.next = head->next`，完美处理。

---

### 解法2: 双指针（快慢指针） — O(L) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1的"瓶颈"是需要遍历两次。双指针利用"固定间距"的技巧，让两个指针保持 N 的间距同步前进，一趟扫描搞定。

```
  快指针先走 n+1 步（让 slow 最终停在被删节点的前驱）:
  
  初始:  dummy → 1 → 2 → 3 → 4 → 5 → NULL
          ↑s
          ↑f
  
  fast走3步(n+1=3):
         dummy → 1 → 2 → 3 → 4 → 5 → NULL
          ↑s               ↑f
  
  同步走直到 fast == NULL:
         dummy → 1 → 2 → 3 → 4 → 5 → NULL
                        ↑s               ↑f
  
  slow->next 就是要删的节点(4)
  slow->next = slow->next->next  →  3→5
```

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;
        
        // 快指针先走 n+1 步，这样 fast 和 slow 之间间隔 n+1
        // 当 fast 到 NULL 时，slow 在被删节点的前驱
        for (int i = 0; i <= n; i++) {
            fast = fast->next;
        }
        
        // 同步前进直到 fast 到达 NULL
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        
        // 此时 slow 是被删节点的前驱
        ListNode* toDelete = slow->next;
        slow->next = toDelete->next;
        delete toDelete;
        
        return dummy.next;
    }
};
```

**关键点**: 快指针走 `n+1` 步而不是 `n` 步——这是为了让 slow 停在被删节点的**前驱**而非被删节点本身（因为单向链表删除需要前驱）。

---

### 解法3: 递归 — O(L) / O(L)

**不同视角**: 递归天然从末尾"回溯"，回溯时计数就能知道"倒数第几个"。

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        remove(&dummy, n);
        return dummy.next;
    }
    
private:
    // 返回值：当前节点是倒数第几个（从1开始）
    int remove(ListNode* node, int n) {
        if (!node->next) return 0;  // 到达最后一个节点，它后面没有节点
        
        int count = remove(node->next, n) + 1;
        // count 表示 node->next 是倒数第 count 个
        
        if (count == n) {
            // node->next 就是要删的节点，node 是其前驱
            ListNode* toDelete = node->next;
            node->next = toDelete->next;
            delete toDelete;
        }
        
        return count;
    }
};
```

**关键点**: 递归解法空间复杂度 O(L)（递归栈），面试中通常不是首选，但展示了递归思维——从末尾回溯计数。

## 解法对比

| | 两次遍历 | 双指针 ⭐ | 递归 |
|---|---|---|---|
| 遍历次数 | 2 次 | 1 次 | 1 次（递归+回溯） |
| 空间 | O(1) | O(1) | O(L) 栈空间 |
| 代码复杂度 | 简单 | 简单 | 中等 |
| 面试推荐 | 开场暴力 | **首选** | 展示递归思维 |
| 核心技巧 | 正数位置换算 | 快慢指针间距 | 回溯计数 |

**什么时候选哪个？**
- 面试首选解法2（双指针），因为它体现了"一趟扫描"的优化思维，这正是 Follow-up 要求的。
- 解法1可以作为思考起点，说清楚后再优化到解法2。
- 解法3适合递归思维强的候选人展示。

## 易错点

1. **忘记用 dummy 节点，导致删头节点时崩溃**
   - ✗ `ListNode* slow = head;` 当 n 等于链表长度时，要删的是 head，没有前驱节点
   - ✓ `ListNode dummy(0, head); ListNode* slow = &dummy;` dummy 充当 head 的前驱

2. **快指针走 n 步而不是 n+1 步**
   - ✗ `for (int i = 0; i < n; i++) fast = fast->next;` → slow 最终停在被删节点本身，无法执行删除
   - ✓ `for (int i = 0; i <= n; i++) fast = fast->next;` → slow 停在被删节点的前驱

3. **删除后忘记返回 `dummy.next` 而返回 `head`**
   - ✗ `return head;` 如果 head 被删了，head 已经是野指针
   - ✓ `return dummy.next;` dummy.next 始终指向正确的新头节点

4. **内存泄漏（面试加分点）**
   - ✗ `slow->next = slow->next->next;` 被删节点没释放
   - ✓ 先保存 `toDelete = slow->next`，修改指针后 `delete toDelete`

## 面试追问

**Q1: 暴力解怎么做？为什么需要两次遍历？**
→ 单向链表不知道长度，也不能从后往前走，所以必须先数长度再定位。倒数第 N 个 = 正数第 L-N+1 个。

**Q2: 能否一趟扫描？（Follow-up）**
→ 双指针：快指针先走 N+1 步，然后同步走。间距保证 slow 停在被删节点的前驱。关键是 dummy 节点处理头节点被删的情况。

**Q3: 如果链表非常长（比如百万节点），有什么需要注意的？**
→ 递归解法会栈溢出（O(L) 栈空间），必须用迭代解法。双指针和两次遍历都是 O(1) 空间。另外要注意避免 cache miss，单向链表本身对缓存不友好。

**Q4: 如果不给 dummy 节点，怎么处理删除头节点？**
→ 快指针先走 N 步后检查 `fast == NULL`，说明要删的是头节点，直接 `return head->next`。但这是特判逻辑，不如 dummy 优雅。

## 相关题型

- **[203. 移除链表元素](https://leetcode.com/problems/remove-linked-list-elements/)** — 复用 dummy 节点技巧，区别：203 是按值删除，19 是按位置删除
- **[876. 链表的中间结点](https://leetcode.com/problems/middle-of-the-linked-list/)** — 复用快慢指针技巧，区别：876 快指针走两步、慢走一步找中点，19 是快指针先走 N 步找倒数第 N 个
- **[141. 环形链表](https://leetcode.com/problems/linked-list-cycle/)** — 同为快慢指针经典题，区别：141 是快2慢1检测环，19 是先走N步再同步
- **[61. 旋转链表](https://leetcode.com/problems/rotate-list/)** — 同样需要"倒数第 K 个"定位，复用本题的双指针或两次遍历技巧