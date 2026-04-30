# 203. 移除链表元素

## 核心思路

本质上就是**遍历链表，跳过所有值等于 val 的节点**。难点不在算法本身，而在于链表删除操作的指针处理——尤其是头节点可能被删除的情况。

## 思维链

1. **读完题第一反应**：遍历链表，遇到 `val` 就删掉。链表删除 = 让前一个节点的 `next` 跳过当前节点。
2. **发现麻烦之处**：如果要删的是头节点呢？头节点没有"前一个节点"，需要特殊处理。
3. **怎么消除特殊情况？**：加一个虚拟头节点（dummy），让所有节点（包括原始头节点）都有"前一个节点"，统一处理逻辑。
4. **另一个视角**：链表天然是递归结构——处理当前节点 + 递归处理剩余链表。能不能用递归？可以！递归地处理子链表，返回处理后的头节点。
5. **对比选择**：迭代 + dummy 是面试首选（O(1) 空间），递归代码最简洁但有 O(n) 栈空间开销。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 迭代（不用 dummy） | 单独处理头节点 + 遍历删除 | O(n) | O(1) | 能说出即可 |
| 迭代 + 虚拟头节点 | dummy 统一逻辑，遍历删除 | O(n) | O(1) | ⭐ 必须写出 |
| 递归 | 递归处理子链表，当前节点决定保留或跳过 | O(n) | O(n) | 加分项 |

## 关键提示

- **提示1**：删除链表节点的核心操作是 `prev->next = curr->next`。想想谁是 `prev`？
- **提示2**：如果头节点就是要删的值（如 `[7,7,7,7], val=7`），你的代码能正确处理吗？试试加一个假的头节点。
- **提示3**：递归思考——假设 `removeElements(head->next, val)` 已经帮你把后面都处理好了，你只需要决定当前 `head` 保留还是跳过。
- **提示4**：遍历时不要急着移动指针。只有当"没删除"时才移动 `prev`，"删除了"时 `prev` 不动。

```
// 删除 val=6 的过程 (使用 dummy):
//
// dummy → 1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//   ↑prev  ↑curr
//
// Step1: curr=1, val≠6, prev移到1
// dummy → 1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//          ↑prev ↑curr
//
// Step2: curr=2, val≠6, prev移到2
// dummy → 1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//               ↑prev ↑curr
//
// Step3: curr=6, val==6! prev->next = curr->next, 删除6
// dummy → 1 → 2 → 3 → 4 → 5 → 6 → NULL
//               ↑prev ↑curr
//
// Step4-5: curr=3,4,5 都不等于6, prev逐步前进
// dummy → 1 → 2 → 3 → 4 → 5 → 6 → NULL
//                            ↑prev ↑curr
//
// Step6: curr=6, val==6! prev->next = NULL
// dummy → 1 → 2 → 3 → 4 → 5 → NULL
//
// 返回 dummy->next = 1
```

## 解法详解

### 解法1: 迭代（不用 dummy）— O(n) / O(1)

**思考过程**：最直接的想法——先处理头部连续等于 val 的节点（while 循环跳过），然后遍历剩余链表删除。

```cpp
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 第一步：处理头节点等于 val 的情况（可能连续多个）
        while (head != nullptr && head->val == val) {
            ListNode* toDelete = head;
            head = head->next;
            delete toDelete; // 释放内存（LeetCode 不强制，但好习惯）
        }
        
        // 第二步：遍历剩余链表
        ListNode* curr = head;
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->next->val == val) {
                // 删除 curr->next
                ListNode* toDelete = curr->next;
                curr->next = curr->next->next;
                delete toDelete;
                // 注意：curr 不动！因为新的 curr->next 可能也是 val
            } else {
                curr = curr->next;
            }
        }
        return head;
    }
};
```

**关键点**：头部需要单独用 while 循环处理，逻辑分两段，容易遗漏边界。这就是为什么我们更推荐解法2。

---

### 解法2: 迭代 + 虚拟头节点（Dummy Head）— O(n) / O(1) ⭐ 面试首选

**从解法1优化**：解法1的麻烦在于头节点没有 `prev`，需要特殊处理。加一个 dummy 节点指向 head，所有节点的删除逻辑就统一了。

```cpp
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // dummy 节点：值随便填，next 指向 head
        ListNode* dummy = new ListNode(0, head);
        ListNode* prev = dummy;
        
        while (prev->next != nullptr) {
            if (prev->next->val == val) {
                // 删除 prev->next
                ListNode* toDelete = prev->next;
                prev->next = prev->next->next;
                delete toDelete;
                // prev 不移动，继续检查新的 prev->next
            } else {
                prev = prev->next;
            }
        }
        
        ListNode* newHead = dummy->next;
        delete dummy; // 释放 dummy
        return newHead;
    }
};
```

**关键点**：
- dummy 让我们永远通过 `prev->next` 来判断和删除，逻辑完全统一
- 删除后 `prev` 不动，因为新的 `prev->next` 可能还需要删除

---

### 解法3: 递归 — O(n) / O(n)

**不同视角**：链表是天然的递归结构。「删除链表中值为 val 的节点」= 「递归处理子链表」+「决定当前节点保留还是删掉」。

```cpp
class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 递归终止条件：空链表
        if (head == nullptr) return nullptr;
        
        // 递归：先把后面的链表处理好
        head->next = removeElements(head->next, val);
        
        // 当前节点：保留还是跳过？
        // 如果 head->val == val，返回 head->next（跳过自己）
        // 否则返回 head（保留自己）
        return head->val == val ? head->next : head;
    }
};
```

**关键点**：
- 递归三要素：
  - **返回值**：处理好的子链表头节点
  - **终止条件**：`head == nullptr`
  - **单层逻辑**：先递归处理后面，再决定自己留不留
- 代码极其简洁，但栈空间 O(n)，链表很长时可能栈溢出

```
// 递归展开过程 (head=[1,2,6,3], val=6):
//
// removeElements(1→2→6→3→NULL, 6)
//   1->next = removeElements(2→6→3→NULL, 6)
//     2->next = removeElements(6→3→NULL, 6)
//       6->next = removeElements(3→NULL, 6)
//         3->next = removeElements(NULL, 6)
//           return NULL                          ← 终止
//         3->next = NULL, 3≠6, return 3→NULL
//       6->next = 3→NULL, 6==6, return 3→NULL    ← 跳过6！
//     2->next = 3→NULL, 2≠6, return 2→3→NULL
//   1->next = 2→3→NULL, 1≠6, return 1→2→3→NULL
// 
// 结果: 1→2→3→NULL ✓
```

## 解法对比

|  | 解法1: 不用dummy | 解法2: dummy ⭐ | 解法3: 递归 |
|--|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(1) | O(1) | O(n) 栈空间 |
| 代码复杂度 | 分两段处理，易出错 | 统一逻辑，简洁清晰 | 最简洁，3行核心 |
| 适用场景 | 不推荐 | **面试首选** | 展示递归思维 |

**面试建议**：先写解法2（dummy），如果面试官问"能用递归吗？"再给解法3。

## 易错点

- ✗ 删除节点后移动了 prev：
  ```cpp
  if (prev->next->val == val) {
      prev->next = prev->next->next;
      prev = prev->next; // ❌ 错！新的 prev->next 可能也是 val
  }
  ```
  ✓ 删除后 prev 不动，下一轮继续检查新的 `prev->next`

- ✗ 忘记处理全部是 val 的情况（如 `[7,7,7], val=7`）：
  解法1 中如果只用 `if` 而不是 `while` 跳过头部，会漏删。解法2 的 dummy 天然避免了这个问题。

- ✗ 返回 `head` 而不是 `dummy->next`：
  ```cpp
  return head; // ❌ head 可能已经被删掉了
  ```
  ✓ 必须返回 `dummy->next`

- ✗ 递归解法终止条件写成 `head->next == nullptr`：
  ```cpp
  if (head->next == nullptr) ... // ❌ 没处理 head 本身为 nullptr 的情况
  ```
  ✓ 终止条件应该是 `head == nullptr`

## 面试追问

**Q1（基础理解）**：为什么需要虚拟头节点？不用行不行？
> 不用也行（解法1），但删除头节点需要单独处理。dummy 让所有删除操作统一为 `prev->next = prev->next->next`，代码更简洁、不易出错。

**Q2（递归 vs 迭代）**：递归解法的缺点是什么？
> 栈空间 O(n)。如果链表有 10^4 个节点一般没问题，但如果链表很长（10^6+），可能导致栈溢出。面试中提到这点是加分项。

**Q3（变体追问）**：如果不是删除等于 val 的节点，而是只保留第一次出现的（去重），怎么改？
> 类似 LeetCode 83（删除排序链表中的重复元素）。用哈希集合记录见过的值，遇到重复就删除，框架和本题一样。

**Q4（内存管理）**：你的代码有没有内存泄漏？
> 如果不 `delete` 被删除的节点就有泄漏。LeetCode 平台不检查，但面试中提到 `delete` 展示工程素养。

## 相关题型

- **LeetCode 83: 删除排序链表中的重复元素** — 复用 dummy + prev 遍历框架，区别：判断条件从 `val==target` 变成 `curr->val == curr->next->val`
- **LeetCode 82: 删除排序链表中的重复元素 II** — 同样的 dummy 框架，但要删除所有重复的节点（不是留一个），需要额外计数
- **LeetCode 237: 删除链表中的节点** — 不给 head 只给要删的节点，巧妙地用"复制值+删下一个"绕过没有 prev 的问题
- **LeetCode 206: 反转链表** — 同样是链表基础操作，复用迭代/递归双解法的思维模式