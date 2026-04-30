# 61. 旋转链表

## 核心思路

"向右旋转 k 位"本质上就是：**找到链表中某个断点，把后半段移到前面**。关键在于确定断点位置，并处理 k 可能大于链表长度的情况。

## 思维链

1. **读完题第一反应**：每次把最后一个节点移到头部，重复 k 次 → 暴力模拟，每次操作 O(n)，总共 O(n*k)。
2. **暴力解的瓶颈**：k 可以高达 2×10⁹，逐次旋转必然超时。但旋转 n 次（n 为链表长度）等于回到原位，所以实际只需旋转 `k % n` 次。
3. **进一步观察**：向右旋转 k 位，等价于把**倒数第 k 个节点**作为新的头。也就是在**第 n - k 个节点处断开**，后半段接到前面。
4. **实现方式 A（成环法）**：先把链表首尾相连形成环，然后在正确位置断开——只需一次遍历求长度 + 一次走到断点。
5. **实现方式 B（快慢指针）**：快指针先走 k 步，然后快慢一起走到末尾，慢指针就在断点处。

```
// 示例: [1,2,3,4,5], k=2
//
// 原链表:  1 → 2 → 3 → 4 → 5 → NULL
//                     ↑断点
// 断开后:  后半段 4 → 5    前半段 1 → 2 → 3
// 拼接:    4 → 5 → 1 → 2 → 3 → NULL
//
// 断点位置 = n - k%n = 5 - 2 = 第3个节点之后
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力模拟 | 每次把尾节点移到头，重复 k%n 次 | O(n × (k%n)) | O(1) | 能说出即可 |
| 成环断开 | 首尾相连形成环，在正确位置断开 | O(n) | O(1) | ⭐ 必须写出 |
| 快慢指针 | 快指针先走 k 步，找到断点 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **k 可以大于链表长度！** 旋转 n 次回到原位，所以先 `k = k % n`。如果 `k == 0`，直接返回原链表。
2. **"向右旋转 k 位"等价于"在第 n-k 个节点后断开，后半段接到前面"** — 把旋转问题转化为断点问题。
3. **成环法的核心**：先让尾节点指向头节点形成环，然后从尾节点再往前走 `n - k` 步到达新尾巴，断开即可。
4. **空链表和单节点**：`head == NULL` 或只有一个节点时直接返回。
5. 画图！链表题不画图很容易搞混指针：

```
// 成环法过程:
// Step1: 遍历求长度，找到尾节点
//   1 → 2 → 3 → 4 → 5 → NULL    len=5, tail=5
//
// Step2: 尾接头，形成环
//   1 → 2 → 3 → 4 → 5 ─┐
//   ↑                     |
//   └─────────────────────┘
//
// Step3: k=2, 需要从tail再走 n-k = 3 步到达新尾巴
//   tail在5, 走3步: 5→1→2→3  新尾巴=3, 新头=4
//
// Step4: 在3处断开
//   4 → 5 → 1 → 2 → 3 → NULL
```

## 解法详解

### 解法1: 暴力模拟 — O(n × (k%n)) / O(1)

**思考过程**: 最直觉的做法 — 每次旋转一位：找到尾节点和倒数第二个节点，把尾节点移到头部。重复 k 次。k 可能很大，所以先取模。

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        // 先求长度，用于取模
        int n = 0;
        ListNode* cur = head;
        while (cur) {
            n++;
            cur = cur->next;
        }
        k %= n;
        if (k == 0) return head;
        
        // 每次把尾节点移到头部，重复 k 次
        for (int i = 0; i < k; i++) {
            // 找到倒数第二个节点
            ListNode* prev = head;
            while (prev->next->next) {
                prev = prev->next;
            }
            // prev->next 是尾节点，移到头部
            ListNode* tail = prev->next;
            prev->next = nullptr;   // 断开
            tail->next = head;      // 尾接到头前面
            head = tail;            // 更新头
        }
        return head;
    }
};
```

**关键点**: 每次旋转都要遍历到倒数第二个节点，内层 O(n)，外层 O(k%n) 次，总 O(n²) 最坏情况。能 AC 但不优雅。

---

### 解法2: 成环断开法 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 暴力法每次只移一个节点，瓶颈在于重复遍历。我们观察到旋转的结果其实就是"在某个位置断开，后半段接到前面"。如果先把链表连成环，只需在正确位置断开一次。

```cpp
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        // 第一步：遍历求长度，同时找到尾节点
        int n = 1;  // head 本身算 1 个
        ListNode* tail = head;
        while (tail->next) {
            n++;
            tail = tail->next;
        }
        
        // k 对 n 取模，旋转 n 的倍数等于没转
        k %= n;
        if (k == 0) return head;  // 不需要旋转
        
        // 第二步：尾接头，形成环
        tail->next = head;
        
        // 第三步：找到新的尾节点 = 从 head 走 (n - k - 1) 步
        // 等价于从 tail 走 (n - k) 步
        // 新尾巴是第 (n-k) 个节点，新头是第 (n-k+1) 个节点
        ListNode* newTail = tail;
        for (int i = 0; i < n - k; i++) {
            newTail = newTail->next;
        }
        
        // 第四步：断开环
        ListNode* newHead = newTail->next;
        newTail->next = nullptr;
        
        return newHead;
    }
};
```

```
// 完整过程演示: [1,2,3,4,5], k=2
//
// Step1: 求长度 n=5, 找到 tail=节点5
//   1 → 2 → 3 → 4 → 5 → NULL
//                         ↑tail
//
// Step2: k = 2%5 = 2, 不为0，继续
//
// Step3: 成环  tail->next = head
//   1 → 2 → 3 → 4 → 5 ─┐
//   ↑                     |
//   └─────────────────────┘
//
// Step4: 从 tail(=5) 走 n-k = 3 步
//   5 →(1步)→ 1 →(2步)→ 2 →(3步)→ 3
//   newTail = 节点3, newHead = 节点4
//
// Step5: 断开  3→NULL
//   结果: 4 → 5 → 1 → 2 → 3 → NULL
```

**关键点**: 成环后只需一次线性遍历找断点。注意 `n-k` 是从 tail 开始走的步数。

---

### 解法3: 快慢指针法 — O(n) / O(1)

**思路**: 不成环，直接用两个指针找断点。快指针先走 k 步，然后快慢一起走，快指针到末尾时慢指针就在断点处。

```cpp
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        // 先求长度（因为 k 可能大于 n）
        int n = 0;
        ListNode* cur = head;
        while (cur) {
            n++;
            cur = cur->next;
        }
        k %= n;
        if (k == 0) return head;
        
        // 快指针先走 k 步
        ListNode* fast = head;
        for (int i = 0; i < k; i++) {
            fast = fast->next;
        }
        
        // 快慢一起走，直到快指针到最后一个节点
        ListNode* slow = head;
        while (fast->next) {
            fast = fast->next;
            slow = slow->next;
        }
        // 此时 slow 是新尾巴，slow->next 是新头
        // fast 是旧尾巴
        
        ListNode* newHead = slow->next;
        slow->next = nullptr;      // 断开
        fast->next = head;          // 旧尾接旧头
        
        return newHead;
    }
};
```

```
// 快慢指针过程: [1,2,3,4,5], k=2
//
// 快指针先走 2 步:
//   1   2   3   4   5
//   ↑slow  ↑fast
//
// 一起走直到 fast 到末尾:
//   1   2   3   4   5
//           ↑slow      ↑fast
//
// slow=3 是新尾巴, slow->next=4 是新头
// fast=5 是旧尾巴, fast->next = head(1)
// 断开: 3→NULL
// 结果: 4 → 5 → 1 → 2 → 3 → NULL
```

**关键点**: 这个方法本质上和解法2相同，都是找到第 n-k 个节点。只是找法不同：解法2成环后从 tail 走，解法3用快慢指针差值定位。

---

## 解法对比

| | 暴力模拟 | 成环断开 | 快慢指针 |
|---|---|---|---|
| 时间 | O(n × (k%n)) | O(n) | O(n) |
| 空间 | O(1) | O(1) | O(1) |
| 遍历次数 | 1 + k%n 次 | 2 次（求长度 + 找断点） | 2 次（求长度 + 快慢走） |
| 代码简洁度 | 简单但低效 | 最简洁优雅 | 稍长但直观 |
| 面试推荐 | 提思路即可 | ⭐ 首选 | ⭐ 同样好 |

**成环法 vs 快慢指针**：思路本质相同（都在找第 n-k 个节点），成环法代码更短，快慢指针法更直觉。面试中说任一种都可以。

## 易错点

1. **忘记对 k 取模**
   - ✗ 直接用 k，当 k=2×10⁹ 且 n=3 时循环或走步数爆炸
   - ✓ `k %= n;` 先取模，旋转 n 次等于不转

2. **取模后忘记检查 k==0**
   - ✗ k%n==0 时继续操作，成环后断开位置回到原点但已经改了指针
   - ✓ `if (k == 0) return head;` 提前返回

3. **求长度时 n 的初始值**
   - ✗ 成环法中 `n = 0` 然后 `while (tail)` — tail 会走到 NULL，丢失尾节点
   - ✓ `n = 1` 然后 `while (tail->next)` — 循环结束 tail 正好在最后一个节点

4. **断开位置差一错误**
   - ✗ 从 tail 走 `n - k - 1` 步到新尾巴（少走一步，断错位置）
   - ✓ 从 tail 走 `n - k` 步到新尾巴。可以用示例验证：n=5, k=2, 从tail走3步: 5→1→2→3，节点3确实是新尾巴

5. **空链表未处理**
   - ✗ 直接访问 `head->next` 导致空指针
   - ✓ 开头 `if (!head || !head->next || k == 0) return head;`

## 面试追问

**Q1: 这道题的暴力解是什么？瓶颈在哪？**
> 每次把尾节点移到头部，重复 k 次。瓶颈：每次都要 O(n) 找尾节点，总 O(n·k)。但 k 取模后最多 O(n²)。

**Q2: 能否一趟遍历解决？**
> 严格一趟不行（需要知道长度才能取模）。但可以做到"两趟"：第一趟求长度并找到尾节点，第二趟走到断点位置。不需要额外空间。

**Q3: 如果改成"向左旋转 k 位"呢？**
> 向左旋转 k 位 = 向右旋转 n-k 位。代码几乎不变，只需把 `k %= n` 后改成 `k = n - k`，然后同样的逻辑。

**Q4: 如果是双向链表，有什么变化？**
> 双向链表可以从尾部直接反向走到断点，不需要先求长度再正向走。但仍需求长度来取模。整体复杂度不变，代码略简。

**Q5: 如果是数组而不是链表呢？**
> 数组旋转用"三次翻转法"：先整体翻转，再翻转前 k 个，再翻转后 n-k 个。O(n) 时间 O(1) 空间。参见 [189. 轮转数组]。

## 相关题型

- **[189. 轮转数组](https://leetcode.cn/problems/rotate-array/)** — 同样的旋转操作，但在数组上。数组用"三次翻转法"，链表用"成环断开法"，对比学习两种数据结构的旋转技巧。
- **[19. 删除链表的倒数第 N 个结点](https://leetcode.cn/problems/remove-nth-node-from-end-of-list/)** — 复用"快指针先走 k 步"的快慢指针技巧，找倒数第 N 个节点。
- **[25. K 个一组翻转链表](https://leetcode.cn/problems/reverse-nodes-in-k-group/)** — 更复杂的链表分段操作，需要对"找断点 + 重新连接"更熟练。
- **[143. 重排链表](https://leetcode.cn/problems/reorder-list/)** — 同样需要找链表中点 + 断开 + 重新拼接，综合运用链表基本操作。