# LCR 136. 删除链表的节点

## 核心思路

给定单向链表的头指针和一个要删除的节点值，找到该节点并删除它。这是链表的**基本操作**，考察的是**指针操作**和**边界处理**（特别是删除头节点的情况）。

**核心技巧**：使用 **哨兵节点（dummy node）** 统一处理"删除头节点"和"删除中间/尾部节点"两种情况，避免写 if-else 分支。

## 思维链

1. **读完题第一反应** → 遍历链表，找到值等于 val 的节点，把它跳过（删除）。
2. **删除链表节点需要什么？** → 需要知道要删除节点的**前驱节点**，然后让前驱的 `next` 指向被删节点的 `next`。
3. **棘手情况是什么？** → 如果要删除的是**头节点**，它没有前驱！需要特殊处理：直接返回 `head->next`。
4. **能否统一处理？** → 使用 **哨兵节点**！在 head 前面加一个 dummy 节点，这样 head 也有了前驱，逻辑完全统一。
5. **题目保证节点值互不相同** → 找到就删，不需要处理多个相同值的情况。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哨兵节点 + 遍历 | dummy 统一逻辑 | O(n) | O(1) | 推荐，代码最简洁 |
| 直接处理（分情况） | 头节点特判 | O(n) | O(1) | 可以，但边界多 |
| 递归 | 递归删除 | O(n) | O(n) 栈空间 | 了解即可 |

## 关键提示

1. **哨兵节点**：`ListNode dummy(0); dummy.next = head;` 统一逻辑，避免头节点特判。
2. **节点值互不相同**：题目保证，找到第一个就删除并退出。
3. **找到就 break**：删除后提前退出循环，避免对已删除节点的后续访问。
4. **返回值**：返回 `dummy.next`（不是 `head`），因为 head 可能已被删除。

## 指针操作图解

### 情况1: 删除中间节点

```
删除值为 5 的节点:

初始状态:
  dummy -> [4] -> [5] -> [1] -> [9] -> null
            ^      ^
           prev   cur

找到 cur->val == 5:
  prev->next = cur->next

操作后:
  dummy -> [4] ---------> [1] -> [9] -> null
                  [5] ---^
                 (被跳过，等待 GC)

返回 dummy.next = [4] -> [1] -> [9]
```

### 情况2: 删除头节点

```
删除值为 4 的节点:

初始状态:
  dummy -> [4] -> [5] -> [1] -> [9] -> null
    ^       ^
   prev    cur

找到 cur->val == 4:
  prev->next = cur->next

操作后:
  dummy ---------> [5] -> [1] -> [9] -> null
           [4] ---^
          (被跳过)

返回 dummy.next = [5] -> [1] -> [9]
                  (新的头节点！)

如果没有 dummy，需要特判: if (head->val == val) return head->next;
有了 dummy，头节点和普通节点的处理完全一样。
```

### 情况3: 删除尾节点

```
删除值为 9 的节点:

初始状态:
  dummy -> [4] -> [5] -> [1] -> [9] -> null
                          ^      ^
                         prev   cur

找到 cur->val == 9:
  prev->next = cur->next = null

操作后:
  dummy -> [4] -> [5] -> [1] -> null
                         (新的尾节点！)

返回 dummy.next = [4] -> [5] -> [1]
```

## 解法详解

### 解法1: 哨兵节点 + 遍历 — O(n) / O(1) ⭐ 推荐

**思考过程**: 用 dummy 节点让所有节点（包括 head）都有前驱，统一删除逻辑。

```cpp
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        // 创建哨兵节点，指向 head
        ListNode dummy(0);
        dummy.next = head;
        
        // prev 从 dummy 开始，cur 从 head 开始
        ListNode* prev = &dummy;
        ListNode* cur = head;
        
        while (cur != nullptr) {
            if (cur->val == val) {
                // 找到目标：让前驱跳过当前节点
                prev->next = cur->next;
                break; // 题目保证值唯一，找到即退出
            }
            prev = cur;
            cur = cur->next;
        }
        
        return dummy.next; // 不返回 head，因为 head 可能已被删除
    }
};
```

**关键点**:
- `dummy` 在栈上创建（不是 `new`），函数结束自动销毁，无需手动释放。
- `prev = &dummy` 取地址，不是 `prev = dummy`。
- `break` 找到就退出：题目保证值唯一，不需要继续遍历。
- 返回 `dummy.next` 而非 `head`：head 可能是被删除的节点。

---

### 解法2: 直接处理（头节点特判） — O(n) / O(1)

**不用 dummy 节点，手动处理头节点的情况**:

```
逻辑分支:
  ┌─ val == head->val?
  │   Yes → return head->next (删除头节点)
  │   No  → 遍历找目标，修改前驱的 next
  └─
```

```cpp
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        // 特判: 要删除的是头节点
        if (head->val == val) {
            return head->next;
        }
        
        // 一般情况: 遍历找到目标节点的前驱
        ListNode* prev = head;
        ListNode* cur = head->next;
        
        while (cur != nullptr) {
            if (cur->val == val) {
                prev->next = cur->next;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        
        return head;
    }
};
```

**关键点**:
- 需要特判 `head->val == val` 的情况。
- `prev` 从 `head` 开始，`cur` 从 `head->next` 开始。
- 如果忘记特判头节点，head 永远不会被删除。

---

### 解法3: 单指针法 — O(n) / O(1)

**只用一个指针，检查 `cur->next` 而不是 `cur`**:

```cpp
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        if (head->val == val) return head->next;
        
        ListNode* cur = head;
        while (cur->next != nullptr && cur->next->val != val) {
            cur = cur->next;
        }
        
        // cur->next 就是要删除的节点（如果存在）
        if (cur->next != nullptr) {
            cur->next = cur->next->next;
        }
        
        return head;
    }
};
```

**关键点**:
- 检查 `cur->next->val` 而不是 `cur->val`，这样 `cur` 自然就是前驱。
- 仍然需要头节点特判。
- 循环条件 `cur->next != nullptr` 防止空指针访问。

---

### 解法4: 递归法 — O(n) / O(n)

**用递归的自然结构来处理链表**:

```
递归思想:
  deleteNode([4]->..., 5)
    = [4] -> deleteNode([5]->..., 5)
                = deleteNode([1]->..., 5)  // 跳过 [5]
                    = [1] -> deleteNode([9]->null, 5)
                                = [9] -> null
```

```cpp
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        if (head == nullptr) return nullptr;
        
        if (head->val == val) {
            return head->next; // 跳过当前节点
        }
        
        head->next = deleteNode(head->next, val);
        return head;
    }
};
```

**关键点**:
- 递归的 base case：`head == nullptr` 返回 nullptr。
- 如果当前节点是目标，返回 `head->next`（跳过）。
- 否则递归处理后续节点，并将结果挂回 `head->next`。
- 缺点：递归深度 O(n)，链表很长时可能栈溢出。

## 面试应对

### 面试官可能追问

**Q: 为什么推荐用 dummy 节点？**

A: 统一了头节点和非头节点的删除逻辑。没有 dummy 时需要 if-else 分支，容易遗漏头节点的处理。dummy 节点是链表题的"万能开头"。

**Q: 如果允许多个节点有相同的值，怎么删除所有？**

A: 不用 `break`，继续遍历：
```cpp
while (cur) {
    if (cur->val == val) {
        prev->next = cur->next;
        cur = cur->next; // 不移动 prev
    } else {
        prev = cur;
        cur = cur->next;
    }
}
```

**Q: 如果给的不是 val 而是指向要删除节点的指针？**

A: 这就是 LeetCode 237（删除链表中的节点）。技巧是把下一个节点的值拷贝过来，然后删除下一个节点。

**Q: 需要 free/delete 被删除的节点吗？**

A: 题目说不需要。实际工程中如果是手动内存管理（C/C++ 裸指针），需要 `delete cur`。如果是智能指针或 GC 语言则不需要。

**Q: 时间能优于 O(n) 吗？**

A: 单链表不能，因为必须找到前驱。如果是双链表，给定指向要删除节点的指针，可以 O(1) 删除。

## 易错点

1. **忘记处理头节点**：不用 dummy 时，必须特判 `head->val == val`。
2. **返回值错误**：应返回 `dummy.next`（可能是新 head），不是旧 `head`。
3. **空指针访问**：遍历时检查 `cur != nullptr`，避免对 null 调用 `->val`。
4. **dummy 的生命周期**：栈上 `ListNode dummy(0)` 函数结束自动销毁，不需要 delete。

## 相关题目

| 题目 | 关联 |
|------|------|
| [237. 删除链表中的节点](https://leetcode.com/problems/delete-node-in-a-linked-list/) | 给的是节点指针而非值 |
| [203. 移除链表元素](https://leetcode.com/problems/remove-linked-list-elements/) | 删除所有值为 val 的节点 |
| [83. 删除排序链表中的重复元素](https://leetcode.com/problems/remove-duplicates-from-sorted-list/) | 删除重复节点 |
| [19. 删除链表的倒数第 N 个结点](https://leetcode.com/problems/remove-nth-node-from-end-of-list/) | 快慢指针定位 |

## Follow-up 思考

1. **如果链表是双向链表？** → 给定要删除的节点指针，可以 O(1) 操作：`node->prev->next = node->next; node->next->prev = node->prev;`
2. **如果要删除倒数第 k 个节点？** → 经典快慢指针，快指针先走 k 步。
3. **如果要删除排序链表的重复元素？** → 遍历时比较相邻元素，相同则跳过。
