# 86. 分隔链表

## 核心思路

给定链表和值 x，将所有小于 x 的节点移到大于等于 x 的节点前面，同时保持各自的相对顺序不变。本质是一个**稳定分区**问题。核心技巧：创建两条独立链表（less 和 greater），遍历原链表时按条件分流，最后拼接。这是链表 dummy node 技巧的经典应用。

## 思维链

1. **读完题第一反应**：把小于 x 的节点放前面，大于等于 x 的放后面，还要保持原始相对顺序。这不就是一个"稳定"的 partition 操作吗？（类似快速排序的 partition，但要求稳定）

2. **能不能原地操作？** 链表的原地操作很容易出错（需要处理前驱节点、断链重连等）。不如换个思路：创建两条新链表。

3. **双链表分流法**：
   - 创建 less 链表（存放 < x 的节点）和 greater 链表（存放 >= x 的节点）
   - 遍历原链表，根据节点值分配到对应链表
   - 最后把 less 的尾部接到 greater 的头部

4. **关键细节**：greater 链表的尾部必须设为 nullptr，否则可能形成环（greater 的最后一个节点的 next 可能还指向 less 链表中的某个节点）。

5. **面试最佳策略**：这道题只有一种标准解法（双链表分流），但要把指针图画清楚，把"断尾"这个关键步骤说明白。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双链表分流 | 按 < x 和 >= x 分成两条链，拼接 | O(n) | O(1) | ⭐ 必须写出 |

注意：虽然我们"创建"了两个 dummy 节点，但没有创建任何新的链表节点 — 只是重新组织了原有节点的 next 指针。所以空间复杂度是 O(1)。

## 关键提示

1. **Dummy 节点的作用**：避免处理"链表为空时的第一个节点"的特殊情况。`lessHead` 和 `greaterHead` 都是 dummy 节点，最终返回 `lessHead.next`。

2. **是严格小于 x，不是小于等于**：`if (head->val < x)` 放入 less 链表，`else`（>=x）放入 greater 链表。

3. **必须给 greater 链表断尾**：`greater->next = nullptr`。不然 greater 的最后一个节点的 next 可能还指向 less 链表中的某个节点，形成环。

4. **拼接顺序**：less 尾 → greater 头。`less->next = greaterHead.next`。

5. **空链表和全部节点在同一侧的情况**：dummy 节点自然处理了这些边界。如果所有节点都 >= x，less 链表为空，`lessHead.next == greaterHead.next`，正确。

## 解法详解

### 解法1: 双链表分流 ⭐

**思考过程**：

以 head = [1, 4, 3, 2, 5, 2], x = 3 为例，画完整的指针变化图：

```
初始状态:
lessHead(dummy) → null       greaterHead(dummy) → null
less = &lessHead             greater = &greaterHead
原链表: 1 → 4 → 3 → 2 → 5 → 2 → null
        ^
       head

Step 1: head->val = 1, 1 < 3 → 放入 less
  less->next = head(1)
  less = less->next (= 节点1)
  head = head->next (= 节点4)

  lessHead → 1 → (原next=4, 暂时还连着)
  greaterHead → null

Step 2: head->val = 4, 4 >= 3 → 放入 greater
  greater->next = head(4)
  greater = greater->next (= 节点4)
  head = head->next (= 节点3)

  lessHead → 1
  greaterHead → 4

Step 3: head->val = 3, 3 >= 3 → 放入 greater
  greater->next = head(3)
  greater = greater->next (= 节点3)
  head = head->next (= 节点2)

  lessHead → 1
  greaterHead → 4 → 3

Step 4: head->val = 2, 2 < 3 → 放入 less
  less->next = head(2)
  less = less->next (= 节点2)
  head = head->next (= 节点5)

  lessHead → 1 → 2
  greaterHead → 4 → 3

Step 5: head->val = 5, 5 >= 3 → 放入 greater
  greater->next = head(5)
  greater = greater->next (= 节点5)
  head = head->next (= 节点2')

  lessHead → 1 → 2
  greaterHead → 4 → 3 → 5

Step 6: head->val = 2, 2 < 3 → 放入 less
  less->next = head(2')
  less = less->next (= 节点2')
  head = head->next (= null)

  lessHead → 1 → 2 → 2'
  greaterHead → 4 → 3 → 5

循环结束 (head == null)

关键步骤 - 断尾:
  greater->next = nullptr
  (节点5 的 next 本来还指向节点2', 现在设为 null)

  greaterHead → 4 → 3 → 5 → null

拼接:
  less->next = greaterHead.next
  (节点2' 的 next 指向节点4)

  lessHead → 1 → 2 → 2' → 4 → 3 → 5 → null

返回 lessHead.next = 1
结果: [1, 2, 2, 4, 3, 5]
```

**为什么必须断尾？** 看上面的例子，Step 5 后节点 5 的 next 仍然指向原链表中的节点 2'。如果不断尾，节点 5 → 节点 2' → (less 链表中) → ... → 节点 2' 形成环！

```cpp
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        // 创建两个 dummy 头节点
        ListNode lessHead(0), greaterHead(0);
        // less/greater 指针分别追踪两条链的尾部
        ListNode* less = &lessHead;
        ListNode* greater = &greaterHead;
        
        // 遍历原链表，按值分流
        while (head != nullptr) {
            if (head->val < x) {
                less->next = head;      // 追加到 less 链
                less = less->next;       // less 尾指针前进
            } else {
                greater->next = head;   // 追加到 greater 链
                greater = greater->next; // greater 尾指针前进
            }
            head = head->next;          // 移到下一个节点
        }
        
        // 关键！greater 链尾部必须断开，防止成环
        greater->next = nullptr;
        
        // 拼接：less 链尾 → greater 链头
        less->next = greaterHead.next;
        
        return lessHead.next;
    }
};
```

**复杂度分析**：
- 时间：O(n)，遍历链表一次
- 空间：O(1)，只创建了两个 dummy 节点（栈上分配），没有创建新的链表节点

---

### 变体写法: 使用 new 分配 dummy（需要注意内存）

```cpp
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        // 注意: 用 new 分配的 dummy 需要手动释放
        ListNode* lessHead = new ListNode(0);
        ListNode* greaterHead = new ListNode(0);
        ListNode* less = lessHead;
        ListNode* greater = greaterHead;
        
        while (head) {
            if (head->val < x) {
                less->next = head;
                less = less->next;
            } else {
                greater->next = head;
                greater = greater->next;
            }
            head = head->next;
        }
        
        greater->next = nullptr;
        less->next = greaterHead->next;
        
        ListNode* result = lessHead->next;
        // 释放 dummy 节点
        delete lessHead;
        delete greaterHead;
        return result;
    }
};
```

**推荐用栈上分配**（`ListNode lessHead(0)`）而非 `new`，这样自动管理内存，不会泄漏。

---

### 思考: 如果不用 dummy 节点呢？

不用 dummy 需要处理很多边界情况：

```cpp
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode *lessHead = nullptr, *lessTail = nullptr;
        ListNode *greaterHead = nullptr, *greaterTail = nullptr;
        
        while (head) {
            ListNode* next = head->next;
            head->next = nullptr; // 断开当前节点
            
            if (head->val < x) {
                if (!lessHead) {
                    lessHead = lessTail = head;
                } else {
                    lessTail->next = head;
                    lessTail = head;
                }
            } else {
                if (!greaterHead) {
                    greaterHead = greaterTail = head;
                } else {
                    greaterTail->next = head;
                    greaterTail = head;
                }
            }
            
            head = next;
        }
        
        if (!lessHead) return greaterHead;
        lessTail->next = greaterHead;
        return lessHead;
    }
};
```

这比用 dummy 节点复杂得多 — 这就是为什么 dummy 节点是链表题的标准技巧。

## 解法对比

| 维度 | 双链表 + Dummy | 双链表无 Dummy | 原地移动 |
|------|---------------|---------------|---------|
| 代码简洁度 | 最简洁 | 较复杂 | 很复杂 |
| 边界处理 | Dummy 自动处理 | 需手动判空 | 需处理前驱 |
| 空间 | O(1) | O(1) | O(1) |
| 面试推荐 | 首选 | 不推荐 | 不推荐 |
| 是否保持稳定 | 是 | 是 | 取决于实现 |

## 易错点

1. **忘记给 greater 链断尾 — 最常见的 bug！**
   ```cpp
   // 错误：没有 greater->next = nullptr
   less->next = greaterHead.next;
   return lessHead.next;
   // 后果: greater 链最后一个节点的 next 可能还指向 less 链中的某个节点
   //       形成环, 导致无限循环或错误输出
   ```

2. **判断条件用错：<= 而非 <**
   ```cpp
   // 错误：题目要求"严格小于 x"放左边
   if (head->val <= x) { ... }  // 应该是 < x
   // 等于 x 的节点应该放在 greater 链中
   ```

3. **拼接顺序搞反**
   ```cpp
   // 错误：greater 接 less
   greater->next = lessHead.next;  // 应该是 less->next = greaterHead.next
   return greaterHead.next;         // 应该返回 lessHead.next
   ```

4. **Dummy 节点的 .next vs ->next 搞混**
   ```cpp
   // 栈上分配: ListNode lessHead(0);
   // 用 . 访问: lessHead.next
   // 指针: ListNode* less = &lessHead;
   // 用 -> 访问: less->next
   // 最终返回: lessHead.next (不是 less->next, 虽然值相同但语义不同)
   ```

5. **空链表没有特殊处理**
   ```cpp
   // 实际上 dummy 节点自然处理了空链表:
   // head == nullptr 时, while 不进入
   // greater->next = nullptr (greaterHead.next 已经是 nullptr)
   // less->next = greaterHead.next (lessHead.next = nullptr)
   // 返回 lessHead.next = nullptr ✓
   ```

## 面试追问

### 追问链1: 本题深入

**Q1: 为什么不能原地操作（像数组的 partition 那样）？**

数组可以用双指针从两端向中间交换元素。但链表是单向的，没有"从尾部往前"的能力。原地操作链表 partition 需要维护前驱指针，代码复杂度大幅增加，而且容易出错。

双链表分流法的空间复杂度已经是 O(1)（只用了两个 dummy 节点），所以没必要追求"更原地"的实现。

**Q2: 如果要求三路分区（< x, == x, > x）怎么做？**

创建三条链表：less, equal, greater。遍历分流，最后 less → equal → greater 拼接。这就是"荷兰国旗问题"在链表上的实现。

```cpp
ListNode* threeWayPartition(ListNode* head, int x) {
    ListNode lessD(0), eqD(0), greatD(0);
    ListNode *l = &lessD, *e = &eqD, *g = &greatD;
    while (head) {
        if (head->val < x) { l->next = head; l = l->next; }
        else if (head->val == x) { e->next = head; e = e->next; }
        else { g->next = head; g = g->next; }
        head = head->next;
    }
    g->next = nullptr;
    e->next = greatD.next;
    l->next = eqD.next;
    return lessD.next;
}
```

**Q3: 这道题和快速排序的 partition 有什么关系？**

快速排序的 partition 是选一个 pivot，把数组分成 < pivot 和 >= pivot 两部分。本题本质上就是在链表上做 partition，区别在于：
- 快排的 partition 不要求稳定（相对顺序可以变）
- 本题要求稳定（保持原始相对顺序）

### 追问链2: 链表技巧

**Q4: Dummy 节点在哪些链表题中有用？**

几乎所有链表题都可以用 dummy 节点：
- 合并链表（LC 21）
- 删除节点（LC 203, 83）
- 分隔链表（本题）
- 排序链表（LC 148）

关键好处：避免处理"头节点可能改变"的特殊情况。

**Q5: 如何判断链表是否有环？ 断尾为什么重要？**

如果忘记断尾，greater 链最后一个节点的 next 可能指回 less 链中的某个节点，形成环。可以用 Floyd 判圈法检测，但更好的做法是一开始就不要产生环 — 所以断尾是必须的。

### 追问链3: 扩展

**Q6: 如果链表值是复杂对象，不是简单整数呢？**

只需要改变比较逻辑即可。分流的框架完全不变。

**Q7: 能否用 std::partition 处理链表？**

`std::partition` 是为随机访问迭代器设计的，不适用于链表。但 `std::list` 有自己的 `sort` 和 `splice` 操作，可以实现类似功能。

## 相关题型

| 题目 | 关系 |
|------|------|
| [21. 合并两个有序链表](../0021_merge_two_sorted_lists/hints.md) | 同类：双链表操作 + dummy 节点 |
| [328. 奇偶链表](../0328_odd_even_linked_list/hints.md) | 同类：按规则分成两条链再拼接 |
| [148. 排序链表](../0148_sort_list/hints.md) | 进阶：链表排序（归并排序用到分隔） |
| [LCR 024. 反转链表](../LCR%20024_UHnkqh/hints.md) | 基础：链表指针操作 |
| [206. 反转链表](../0206_reverse_linked_list/hints.md) | 基础：链表指针操作 |
| [143. 重排链表](../0143_reorder_list/hints.md) | 综合：找中点 + 反转 + 合并 |
| [75. 颜色分类](../0075_sort_colors/hints.md) | 同思想：三路 partition（数组版） |
