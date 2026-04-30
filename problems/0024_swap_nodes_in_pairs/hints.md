# 24. 两两交换链表中的节点

## 核心思路

本题本质上是一个**链表指针重排问题**：每两个相邻节点为一组，交换它们的位置，同时保证组与组之间的连接不断裂。核心难点在于交换一对节点时，需要同时处理好**前驱节点**和**后继节点**的指针。

## 思维链

1. **读完题第一反应** → 最朴素的想法是"交换值"，但题目明确要求不能修改节点值，只能改指针。那就只能通过重新连接指针来实现交换。

2. **先想一对怎么交换** → 假设有 `prev → A → B → next_pair`，交换 A 和 B 后应该变成 `prev → B → A → next_pair`。需要改 3 个指针：prev.next、A.next、B.next。

3. **头节点会变怎么办？** → 交换第一对时，头节点从第1个变成第2个。用 **dummy head（虚拟头节点）** 统一处理，避免特判。

4. **怎么处理所有对？** → 两种思路：
   - **迭代**：用一个指针 `prev` 逐对移动，每次交换当前这一对，然后 `prev` 前进两步
   - **递归**：交换当前一对，然后把"剩余链表的交换结果"接上。递归天然处理了子问题

5. **奇数节点怎么办？** → 最后剩一个节点时不交换，直接保留。这是自然的终止条件。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 迭代（dummy head） | 用前驱指针逐对交换 | O(n) | O(1) | ⭐ 必须写出 |
| 递归 | 交换当前对 + 递归处理剩余 | O(n) | O(n) 栈空间 | ⭐ 必须写出 |

## 关键提示

- **提示1**：画出 `prev → A → B → C` 的交换过程，你需要改哪几个指针？顺序是什么？（先想清楚再写代码）

- **提示2**：交换后 `prev` 应该移动到哪里？不是移动一步，而是两步（因为一对已经处理完了）。但交换后 A 和 B 位置变了，`prev` 实际上应该移动到交换后排在后面的那个节点。

- **提示3**：递归思路——假设 `swapPairs(B.next)` 已经返回了后续链表交换好的头节点，你只需要处理当前这一对 A 和 B 的交换。

- **提示4（ASCII 图）**：交换一对的指针变化过程：
```
交换前:
  prev → A → B → (rest)

Step1: 保存 B.next
  prev → A → B → (rest)
              ↑ save this

Step2: B.next = A  (B 指向 A)
  prev → A ← B    (rest)

Step3: A.next = rest  (A 指向后面)
  prev → A → (rest)
         ↑
    B → A → (rest)

Step4: prev.next = B  (前驱指向 B)
  prev → B → A → (rest)

Step5: prev = A  (前进到下一对的前驱)
```

## 解法详解

### 解法1: 迭代（Dummy Head + 逐对交换） — O(n) / O(1) ⭐ 面试首选

**思考过程**：链表交换类问题，迭代是最直接的思路。关键洞察：交换一对节点需要访问它们的**前驱节点**，因为前驱的 next 也要改。使用 dummy head 可以让第一对节点也有前驱，统一处理逻辑。

每次循环处理一对：`prev → first → second → ...`，交换 first 和 second，然后把 prev 移到 first（交换后 first 排在后面）。

```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        // dummy head 统一处理头节点可能变化的情况
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        
        // 每次需要至少两个节点才能交换
        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;       // 要交换的第一个
            ListNode* second = prev->next->next; // 要交换的第二个
            
            // 三步指针重排：顺序很重要，画图理解
            first->next = second->next;  // first 跳过 second，指向后面
            second->next = first;        // second 指向 first（完成翻转）
            prev->next = second;         // 前驱指向新的头（second）
            
            // prev 前进到下一对的前驱位置
            // 交换后 first 在后面，所以 prev = first
            prev = first;
        }
        
        return dummy.next;
    }
};
```

**关键点**：三步指针重排的顺序不能乱。如果先改 `prev->next`，就丢失了对 `first` 的引用。最安全的做法是先保存好 `first` 和 `second` 的引用，然后按 `first->next`、`second->next`、`prev->next` 的顺序修改。

### 解法2: 递归 — O(n) / O(n)

**思考过程**：链表天然适合递归思考。递归的核心：**假设后面的链表已经交换好了**，你只需要处理当前这一对。

递归三要素：
- **返回值**：交换好的这一对的新头节点（即 second）
- **终止条件**：没有节点或只剩一个节点，无法交换，直接返回
- **单层逻辑**：交换当前两个节点，把 first.next 接到递归返回的结果上

```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        // 终止条件：0个或1个节点，无法交换
        if (!head || !head->next) return head;
        
        ListNode* first = head;
        ListNode* second = head->next;
        
        // 递归处理后面的链表（信任递归：后面的已经交换好了）
        first->next = swapPairs(second->next);
        
        // 交换当前这一对
        second->next = first;
        
        // second 是交换后的新头
        return second;
    }
};
```

**关键点**：递归的空间复杂度是 O(n/2) = O(n)，因为每两个节点一层递归。如果面试官追问空间优化，需要切换到迭代解法。

递归展开可视化（以 1→2→3→4 为例）：
```
swapPairs(1→2→3→4)
  first=1, second=2
  first->next = swapPairs(3→4)
                  first=3, second=4
                  first->next = swapPairs(NULL) → return NULL
                  3->next = NULL
                  4->next = 3
                  return 4  → 即 4→3
  1->next = 4→3  → 即 1→4→3
  2->next = 1    → 即 2→1→4→3
  return 2       → 最终: 2→1→4→3 ✓
```

## 解法对比

| | 迭代（Dummy Head） | 递归 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(1) ✅ | O(n) 递归栈 |
| 代码量 | 稍多（需要管理 prev） | 简洁优雅 |
| 可读性 | 需要画图才能理解指针变化 | 逻辑清晰，但要理解递归跳跃 |
| 面试推荐 | 展示链表操作功底 | 展示递归思维 |

**建议**：面试时两种都能写。先说递归（思路清晰），面试官追问空间优化时给迭代。

## 易错点

1. **✗ 循环条件写成 `while (prev->next)`**
   → ✓ 应该是 `while (prev->next && prev->next->next)`
   → 为什么：只有一个节点时不能交换，少了第二个检查会空指针

2. **✗ 指针重排顺序错误，先改 `prev->next = second`**
   ```cpp
   prev->next = second;        // ✗ 此时 first 还能通过 prev->next 找到吗？
   first->next = second->next; // 这里的 first 虽然已保存，但逻辑容易混乱
   ```
   → ✓ 先改 first 和 second 内部的指针，最后改 prev 的指针
   → 原则：先改"远离入口"的指针，最后改"入口"的指针

3. **✗ 迭代中 prev 前进一步 `prev = prev->next`**
   → ✓ 应该是 `prev = first`（交换后 first 在后面的位置）
   → 为什么：交换后 prev→second→first→...，下一对的前驱是 first

4. **✗ 递归中忘记终止条件 `!head->next`**
   → 只写 `if (!head) return head;`，当链表长度为奇数时，`head->next` 为 NULL，访问 `head->next->next` 会崩溃

## 面试追问

**Q1（基础理解）**：交换一对节点需要改几个指针？
> 3个：prev->next、first->next、second->next。画图是最好的理解方式。

**Q2（空间优化）**：递归解法的空间复杂度能优化吗？
> 可以，递归栈深度是 O(n/2)。切换到迭代解法可以做到 O(1) 空间。

**Q3（变体：K个一组反转）**：如果不是两两交换，而是每 K 个一组反转呢？（LeetCode 25）
> 本题是 K=2 的特例。K个一组需要：(1) 先检查剩余节点是否够 K 个；(2) 反转 K 个节点的子链表；(3) 递归或迭代处理后续。反转子链表的操作从"交换两个"泛化为"反转K个"。

**Q4（变体：交换相邻但不连续的对）**：如果要交换第1和第3个、第2和第4个呢？
> 这就不是相邻交换了，需要更通用的链表操作：定位到指定位置，然后断开重接。

## 相关题型

- **25. K 个一组翻转链表** — 本题是 K=2 的特例。K组翻转需要将"交换一对"泛化为"反转K个节点的子链表"，核心的指针重排思路完全一致
- **206. 反转链表** — 反转整个链表是本题的基础操作。本题可以看作"分段反转，每段长度为2"
- **92. 反转链表 II** — 反转链表的指定区间，同样需要 dummy head + 前驱指针的技巧
- **1721. 交换链表中的节点** — 交换第 k 个和倒数第 k 个节点，需要先定位再交换，定位用快慢指针