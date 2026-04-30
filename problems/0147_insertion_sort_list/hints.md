# 147. 对链表进行插入排序 (Insertion Sort List)

## 难度: Medium | 标签: Linked List, Sorting

---

## 核心思路

将数组插入排序的思想迁移到链表上：维护一个"已排序链表"（用 dummy 头），从原链表中逐个取节点，在已排序链表中找到正确位置插入。链表的优势是插入 O(1)（不需要像数组那样移动元素），劣势是查找插入位置只能从头遍历 O(n)。

## 思维链

1. **读完题第一反应**：数组的插入排序很熟悉，链表的区别在于不能随机访问，但插入不需要移动数据。

2. **链表 vs 数组插入排序的核心区别**：
   - 数组：找到插入位置后需要后移所有元素 → O(n) 移动
   - 链表：找到插入位置后只需修改 2-3 个指针 → O(1) 插入
   - 但找插入位置都是 O(n)（链表不支持二分查找）

3. **实现策略**：
   - 创建一个 dummy 节点作为已排序链表的虚拟头
   - 遍历原链表的每个节点 curr
   - 在已排序链表中找到**第一个 val >= curr->val** 的节点的前驱 prev
   - 将 curr 插入到 prev 之后

4. **优化思考**：如果当前节点值大于已排序部分的尾节点值，直接追加到尾部，避免从头遍历。对近乎有序的链表效果显著。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 标准插入排序 | 每次从头找插入位置 | O(n^2) | O(1) | ⭐⭐ 基础版 |
| 优化版（尾部快捷路径） | 大于尾部直接追加 | O(n^2) 最坏, 近乎有序 O(n) | O(1) | ⭐⭐⭐ 推荐 |

## 关键提示

1. **Dummy 节点是链表排序的标配**：避免处理头节点变化的特殊情况。最后返回 `dummy.next`。

2. **先保存 next 再操作 curr**：`curr->next = prev->next; prev->next = curr;` 这两步会断开 curr 与原链表的连接，必须提前保存 `next = curr->next`。

3. **查找插入位置的条件**：`while (prev->next && prev->next->val < curr->val)` —— 注意是 `<` 不是 `<=`，使用 `<` 可以保证**稳定排序**（相同值保持原始相对顺序）。

4. **时间复杂度 O(n^2) 是插入排序的本质限制**：面试官通常会追问 O(n log n) 排序 → 指向 LC 148 归并排序。

## 解法详解

### 解法1: 标准链表插入排序 — O(n^2) / O(1)

**思考过程**：忠实实现插入排序算法。逐个取未排序节点，在已排序链表中线性查找正确位置并插入。

```
指针操作示意 (head = [4,2,1,3]):

初始状态:
  dummy → null                    (已排序链表为空)
  curr = 4 → 2 → 1 → 3 → null   (原链表)

第1步: 取 curr=4, 在已排序链表中找位置
  dummy → null, prev 停在 dummy (next==null)
  插入: dummy → 4 → null
  原链表: curr = 2 → 1 → 3 → null

第2步: 取 curr=2, 在已排序链表中找位置
  dummy → 4, prev 停在 dummy (4 >= 2)
  插入: dummy → 2 → 4 → null          ← 2 插到 4 前面
  原链表: curr = 1 → 3 → null

第3步: 取 curr=1, 在已排序链表中找位置
  dummy → 2 → 4, prev 停在 dummy (2 >= 1)
  插入: dummy → 1 → 2 → 4 → null      ← 1 插到最前面
  原链表: curr = 3 → null

第4步: 取 curr=3, 在已排序链表中找位置
  dummy → 1 → 2 → 4, prev 停在 2 (4 >= 3)
  插入: dummy → 1 → 2 → 3 → 4 → null  ← 3 插到 2 和 4 之间
  
返回 dummy.next = 1 → 2 → 3 → 4
```

```
插入操作的指针变化详解 (第4步, 插入 curr=3):

插入前:
  prev → [2] → [4] → null
  curr → [3] → (已断开)

Step A: curr->next = prev->next
  curr → [3] → [4] → null     ← 3 接上 4
  prev → [2] → [4] → null     ← prev 还指向 4

Step B: prev->next = curr
  prev → [2] → [3] → [4] → null  ← 2 指向 3，完成!
```

```cpp
class Solution1 {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode dummy(0);       // 已排序链表的虚拟头
        ListNode* curr = head;   // 当前待插入的节点
        
        while (curr) {
            // 第一步: 保存下一个节点（因为后面会断开 curr 的 next）
            ListNode* next = curr->next;
            
            // 第二步: 在已排序链表中找插入位置
            // 找到第一个 val >= curr->val 的节点的前驱
            ListNode* prev = &dummy;
            while (prev->next && prev->next->val < curr->val) {
                prev = prev->next;
            }
            
            // 第三步: 插入 curr 到 prev 后面
            curr->next = prev->next;  // curr 接上后面的节点
            prev->next = curr;        // prev 指向 curr
            
            // 第四步: 继续处理下一个节点
            curr = next;
        }
        
        return dummy.next;
    }
};
```

---

### 解法2: 优化版 — 记录已排序尾部 — O(n^2) / O(1) ⭐推荐

**优化思路**：如果当前节点的值大于等于已排序部分的尾部值，说明它应该排在最后，直接追加到尾部即可，不需要从头遍历。这个优化在输入近乎有序时效果显著（退化为 O(n)）。

```
优化效果对比 (head = [-1,0,3,4,5,2]):

标准版: 每次都从 dummy 开始找
  取-1: 从dummy找 → 插入到dummy后
  取 0: 从dummy找 → 插入到-1后
  取 3: 从dummy找 → 遍历到尾部 → 插入
  取 4: 从dummy找 → 遍历到尾部 → 插入  ← 每次都白白遍历
  取 5: 从dummy找 → 遍历到尾部 → 插入
  取 2: 从dummy找 → 插入到0和3之间

优化版: 记录尾部，大值直接追加
  取-1: tail为dummy, -1>0? → 从dummy找 → 插入, tail=-1
  取 0: 0 >= -1 → 直接追加! tail=0            ← 省了遍历
  取 3: 3 >= 0  → 直接追加! tail=3             ← 省了遍历
  取 4: 4 >= 3  → 直接追加! tail=4             ← 省了遍历
  取 5: 5 >= 4  → 直接追加! tail=5             ← 省了遍历
  取 2: 2 < 5   → 从dummy找 → 插入到0和3之间
```

```cpp
class Solution2 {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode dummy(0);
        ListNode* curr = head;
        ListNode* tail = &dummy;  // 已排序链表的尾节点
        
        while (curr) {
            ListNode* next = curr->next;
            
            if (tail->val <= curr->val) {
                // 优化: 当前值 >= 尾部值，直接追加到尾部
                // 对于已经大致有序的输入，这个分支几乎每次都走
                tail->next = curr;
                tail = curr;
                tail->next = nullptr;
            } else {
                // 需要从头遍历找插入位置
                ListNode* prev = &dummy;
                while (prev->next && prev->next->val < curr->val) {
                    prev = prev->next;
                }
                curr->next = prev->next;
                prev->next = curr;
            }
            
            curr = next;
        }
        
        return dummy.next;
    }
};
```

**为什么优化有效**：
- 最好情况（输入已排序）：每次都直接追加，O(n)
- 最坏情况（输入逆序）：每次都从头查找，仍然 O(n^2)
- 平均情况：减少了"明显属于尾部"的遍历开销

## 解法对比

| | 标准版 | 优化版 |
|---|---|---|
| 时间最坏 | O(n^2) | O(n^2) |
| 时间最好 | O(n^2) | O(n) |
| 空间 | O(1) | O(1) |
| 代码复杂度 | 简单 | 稍复杂 |
| 面试推荐度 | ⭐⭐ | ⭐⭐⭐ |
| 核心优势 | 简洁 | 近乎有序时高效 |

## 易错点

1. **忘记保存 next 导致链表断裂**
   - 错: 直接 `curr->next = prev->next; prev->next = curr; curr = curr->next;`
   - 对: 先 `ListNode* next = curr->next;`，最后 `curr = next;`
   - curr 的 next 在插入时会被修改，必须提前保存

2. **查找条件写成 `<=` 导致不稳定排序**
   - 错: `while (prev->next && prev->next->val <= curr->val)`
   - 对: `while (prev->next && prev->next->val < curr->val)`
   - 用 `<` 保证相等元素保持原始顺序（稳定排序）

3. **优化版忘记设置 `tail->next = nullptr`**
   - 追加到尾部后，tail 的 next 可能还指向原链表中的旧节点
   - 必须显式设置 `tail->next = nullptr` 断开旧连接

4. **dummy 初始值的影响**
   - `ListNode dummy(0)` 的 val=0 在优化版中参与比较
   - 当 curr->val < 0 时，`tail->val <= curr->val` 为 false（tail=dummy, val=0 > 负数）
   - 此时走 else 分支从头查找，是正确的

## 面试追问

**Q1: 时间复杂度能优化到 O(n log n) 吗？**
→ 插入排序本质是 O(n^2)，无法优化。但链表可以用归并排序做到 O(n log n)，参考 LC 148 Sort List。归并排序对链表特别友好：不需要额外数组，merge 只需修改指针。

**Q2: 链表插入排序 vs 数组插入排序，哪个更好？**
→ 各有优劣。链表插入 O(1) 但查找 O(n)；数组可以二分查找 O(log n) 但移动元素 O(n)。总时间都是 O(n^2)。链表的优势在于不需要移动数据，在节点很大（携带大量数据）时更高效。

**Q3: 如果输入几乎有序，插入排序反而是好选择？**
→ 是的。对于"几乎有序"的输入（每个元素距离最终位置不超过 k），插入排序是 O(nk)。当 k 很小时比 O(n log n) 的归并/快排更快。这也是为什么 Python 的 Timsort 在小片段内使用插入排序。

**Q4: 为什么不能用快排对链表排序？**
→ 可以，但不推荐。快排需要随机访问来做 partition，链表的随机访问是 O(n)。虽然也可以实现链表快排，但 pivot 选择不方便（不能取中间元素），容易退化为 O(n^2)。归并排序对链表更自然。

## 相关题型

- [148. Sort List](../0148_sort_list/) — O(n log n) 链表归并排序，面试高频
- [21. Merge Two Sorted Lists](../0021_merge_two_sorted_lists/) — 归并排序的基础操作
- [708. Insert into a Sorted Circular Linked List](../0708_insert_into_a_sorted_circular_linked_list/) — 类似的"在有序链表中找插入位置"
