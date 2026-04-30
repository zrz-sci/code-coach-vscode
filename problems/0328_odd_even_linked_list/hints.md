# 328. 奇偶链表

## 核心思路

本质上是**链表的分组与拼接问题**：把一条链表按位置（奇数位、偶数位）拆成两条子链表，再把偶数链接到奇数链尾部。关键在于如何**原地**完成拆分，不使用额外空间。

## 思维链

1. **读完题第一反应**：能不能用额外数组？→ 把奇数位的值和偶数位的值分别收集到两个数组里，然后重新写回链表。简单但用了 O(n) 额外空间，题目要求 O(1)。

2. **朴素做法的瓶颈**：额外空间 O(n)。那能不能不创建新节点，直接在原链表上修改指针？

3. **关键观察**：链表的优势就是通过改变 `next` 指针来"移动"节点，根本不需要复制数据。我们只需要两个指针分别串起奇数位和偶数位的节点。

4. **怎么做？**：维护两个指针 `odd` 和 `even`，它们分别在奇数链和偶数链上前进。每一步：`odd->next = even->next`（跳过偶数节点），然后 `even->next = odd->next`（跳过奇数节点）。最后把偶数链接到奇数链尾部。

5. **为什么能 O(1) 空间？**：我们只是修改了已有节点的 `next` 指针，没有创建新节点，只用了几个指针变量。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 额外数组法 | 分别收集奇偶位值，重写链表 | O(n) | O(n) | 能说出即可 |
| 双指针原地重排 | odd/even 两条链交替前进，最后拼接 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

- **提示1**：你需要保存偶数链表的头节点 `evenHead`，因为最后要把它接到奇数链的尾部。如果不保存，遍历结束后就找不到偶数链的起点了。

- **提示2**：循环终止条件应该看 `even` 和 `even->next`。想想为什么不是看 `odd`？因为 `even` 在 `odd` 后面，如果 `even` 为空或 `even->next` 为空，说明没有更多奇数节点可以跳了。

- **提示3**：画图！链表题不画图就写代码是自找麻烦。以下是指针变化过程：

```
原始链表:  1 → 2 → 3 → 4 → 5 → NULL
           ↑   ↑
          odd  even    evenHead = 2

Step 1: odd.next = 3, odd 前进到 3; even.next = 4, even 前进到 4
           1 → 3 → 5    (奇数链)
           2 → 4 → NULL (偶数链)
           
Step 2: odd.next = 5, odd 前进到 5; even.next = NULL, even 前进到 NULL

最终拼接: odd.next = evenHead
           1 → 3 → 5 → 2 → 4 → NULL
```

- **提示4**：注意空链表和只有1-2个节点的边界情况 — 它们不需要任何操作。

## 解法详解

### 解法1: 额外数组法 — O(n) / O(n)

**思考过程**: 最直觉的想法 — 遍历一遍链表，把奇数位的值放一个数组、偶数位的值放另一个数组，然后按顺序写回链表。这不满足 O(1) 空间的要求，但帮助理解题目在问什么。

```cpp
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (!head || !head->next) return head;
        
        vector<int> oddVals, evenVals;
        ListNode* curr = head;
        int idx = 1;
        
        // 分别收集奇数位和偶数位的值
        while (curr) {
            if (idx % 2 == 1)
                oddVals.push_back(curr->val);
            else
                evenVals.push_back(curr->val);
            curr = curr->next;
            idx++;
        }
        
        // 按"先奇后偶"的顺序写回链表
        curr = head;
        for (int v : oddVals) {
            curr->val = v;
            curr = curr->next;
        }
        for (int v : evenVals) {
            curr->val = v;
            curr = curr->next;
        }
        
        return head;
    }
};
```

**关键点**: 这个解法改的是节点的 **值**，不是节点的 **指针**。在面试中，如果面试官说"不允许修改节点值"，这个方法就不行了。而且它用了 O(n) 额外空间，不满足题目要求。

### 解法2: 双指针原地重排 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是额外空间。链表的核心操作就是"改指针"，我们完全可以通过修改 `next` 指针把奇数位节点和偶数位节点各自串成一条链，最后拼接。

**核心思路**：
- `odd` 指针沿奇数位走，`even` 指针沿偶数位走
- 每步让 `odd` 跳过下一个偶数节点，`even` 跳过下一个奇数节点
- 循环结束后 `odd` 停在奇数链尾部，把 `evenHead` 接上去

```
// 详细指针变化过程（5个节点的情况）:
//
// 初始:
// 1 → 2 → 3 → 4 → 5 → NULL
// ↑   ↑
// odd even          evenHead = node(2)
//
// 第1轮:
//   odd->next = even->next (即 1→3，跳过2)
//   odd = odd->next        (odd 移到 3)
//   even->next = odd->next (即 2→4，跳过3)
//   even = even->next      (even 移到 4)
//
//   奇数链: 1 → 3 → (5)
//   偶数链: 2 → 4 → (5)   ← 注意此时4还指向5
//              ↑       ↑
//            even      odd还没处理
//
// 第2轮:
//   odd->next = even->next (即 3→5，跳过4)
//   odd = odd->next        (odd 移到 5)
//   even->next = odd->next (即 4→NULL)
//   even = even->next      (even = NULL)
//
//   奇数链: 1 → 3 → 5 → NULL
//   偶数链: 2 → 4 → NULL
//
// 拼接: odd->next = evenHead
//   1 → 3 → 5 → 2 → 4 → NULL
```

```cpp
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (!head || !head->next) return head;
        
        ListNode* odd = head;            // 奇数链的当前节点
        ListNode* even = head->next;     // 偶数链的当前节点
        ListNode* evenHead = even;       // 保存偶数链头部，最后要拼接用
        
        // 为什么循环条件是 even && even->next？
        // even 在 odd 后面，如果 even 为空说明链表已遍历完（偶数长度）
        // 如果 even->next 为空说明没有下一个奇数节点了（奇数长度）
        while (even && even->next) {
            odd->next = even->next;     // 奇数节点跳过偶数节点，连到下一个奇数
            odd = odd->next;            // odd 前进
            even->next = odd->next;     // 偶数节点跳过奇数节点，连到下一个偶数
            even = even->next;          // even 前进
        }
        
        // 奇数链尾部 → 偶数链头部
        odd->next = evenHead;
        
        return head;
    }
};
```

**关键点**: 
- 四行操作的顺序不能乱！必须先更新 `odd->next`，再移动 `odd`，然后更新 `even->next`（此时用的是新的 `odd->next`），最后移动 `even`。
- `evenHead` 必须在循环前保存，否则循环中 `even` 会前进，丢失偶数链头。

## 解法对比

| | 额外数组法 | 双指针原地重排 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(n) | O(1) |
| 修改方式 | 修改节点值 | 修改 next 指针 |
| 满足题目要求 | ✗ 空间不满足 | ✓ |
| 面试推荐 | 仅用于说明思路 | ⭐ 首选 |

面试中直接写解法2即可。如果面试官问"还有其他方式吗？"，可以提一下额外数组法作为对比。

## 易错点

- **✗ 循环条件写成 `while (odd && odd->next)`**：这样会在偶数长度的链表上出错，因为 `even` 可能已经是 NULL 了你还试图访问 `even->next`。→ **✓ 应该是 `while (even && even->next)`**，因为 `even` 始终在 `odd` 后面或与其同步，它先到达末尾。

- **✗ 忘记保存 `evenHead`**：循环结束后 `even` 已经走到偶数链尾部（或 NULL），如果没提前保存偶数链头，无法完成拼接。→ **✓ 在循环前 `ListNode* evenHead = even;`**。

- **✗ 四行操作顺序颠倒**：如果先移动 `odd` 再修改 `odd->next`，会指向错误的节点。→ **✓ 先改 next 再移动指针**，具体顺序：`odd->next = ...` → `odd = ...` → `even->next = ...` → `even = ...`。

- **✗ 没处理空链表或单节点**：`head->next` 在 `head` 为 NULL 时会段错误。→ **✓ 开头加 `if (!head || !head->next) return head;`**。

## 面试追问

**Q1: 你的循环条件为什么是 `even && even->next` 而不是 `odd && odd->next`？**

> 因为在每轮循环中，我们先处理 odd 再处理 even。even 的位置始终在 odd 之后（或同一位置），所以 even 会先触及链表尾部。如果链表长度为偶数，循环结束时 even = NULL；如果长度为奇数，循环结束时 even->next = NULL。两种情况都被 `even && even->next` 正确覆盖。

**Q2: 如果题目改成"按值的奇偶"分组（而不是按位置），代码怎么改？**

> 不能再用简单的交替前进了。需要遍历链表，根据 `curr->val % 2` 决定把节点接到奇数链还是偶数链。本质上变成了 LeetCode 86 (Partition List) 的变体 — 用两个 dummy head 分别收集，最后拼接。

**Q3: 如果要求把链表分成三组（位置模3余0、余1、余2），你的代码框架怎么扩展？**

> 同样的思路：维护三个指针和三个链头，遍历时按 `idx % 3` 分配到对应链，最后按顺序拼接三条链。核心框架不变，只是从两组变成三组。

## 相关题型

- **LeetCode 86. 分隔链表 (Partition List)** — 复用"两条链分别收集、最后拼接"的框架，区别：86按值的大小分组，328按位置奇偶分组。
- **LeetCode 206. 反转链表** — 同样是纯指针操作，练习"画指针变化图"的基本功。
- **LeetCode 143. 重排链表 (Reorder List)** — 复用"找中点→拆链→拼接"的思路，比328多一步反转后半部分。
- **LeetCode 725. 分隔链表 (Split Linked List in Parts)** — 链表分组的进阶变体，需要按份数均匀分割。