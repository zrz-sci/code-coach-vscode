# 148. 排序链表

## 核心思路（1句话）
链表排序的最佳选择是归并排序：快慢指针找中点 -> 递归拆分 -> 合并有序子链表；进阶要求 O(1) 空间则用自底向上迭代归并。

## 思维链（4-6步）

1. **第一反应**：排序自然想到快排或归并。但链表不支持随机访问，快排的 partition 需要频繁前后跳跃，效率低且实现复杂。归并排序天然适合链表——只需要顺序访问和指针操作。
2. **拆分问题**：数组归并直接取中点下标，链表没有下标。解决方案：快慢指针（slow 走一步，fast 走两步），当 fast 到末尾时 slow 就在中点。
3. **合并问题**：合并两个有序链表是经典题 LC21，用 dummy 头节点 + 双指针即可。
4. **空间瓶颈**：自顶向下递归的调用栈深度 O(log n)，不满足 Follow-up 的 O(1) 空间要求。
5. **突破——自底向上**：不用递归，从长度为 1 的子链表开始，逐步合并为 2、4、8...直到整个链表有序。完全迭代，O(1) 额外空间。
6. **最终决策**：面试先写自顶向下（好理解好写），Follow-up 再给自底向上展示进阶能力。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 解法1: 自顶向下归并 ⭐面试首选 | O(n log n) | O(log n) 递归栈 | 经典分治，好理解好写 |
| 解法2: 自底向上归并 | O(n log n) | O(1) | 满足 Follow-up 要求 |

## 关键提示

1. **快慢指针找中点的细节**：fast 应初始化为 `head->next` 而非 `head`。这样当链表为偶数长度时，slow 停在前半部分的最后一个节点，方便断开。
   ```
   链表: 1 -> 2 -> 3 -> 4
   fast = head->next:  slow=1, fast=2 -> slow=2, fast=4 -> 停止
   中点 = slow->next = 3, 断开 slow->next = nullptr
   左半: 1->2, 右半: 3->4  ✓ 均匀分割

   fast = head:  slow=1, fast=1 -> slow=2, fast=3 -> slow=3, fast=null -> 停止
   中点 = slow->next = 4, 左半: 1->2->3, 右半: 4  ✗ 不均匀
   ```

2. **指针操作图示（合并过程）**：
   ```
   合并 l1=[1,3] 和 l2=[2,4]:

   dummy -> ?      tail=dummy
   
   比较 l1(1) vs l2(2): 1 < 2, 接 l1
   dummy -> 1 -> ?  tail=1, l1=3
   
   比较 l1(3) vs l2(2): 3 > 2, 接 l2
   dummy -> 1 -> 2 -> ?  tail=2, l2=4
   
   比较 l1(3) vs l2(4): 3 < 4, 接 l1
   dummy -> 1 -> 2 -> 3 -> ?  tail=3, l1=null
   
   l1 为空，接上 l2 剩余
   dummy -> 1 -> 2 -> 3 -> 4
   ```

3. **自底向上的 split 操作图示**：
   ```
   原链表: 4 -> 2 -> 1 -> 3 -> 5 -> 6

   sz=1 轮:
     split(head, 1): 取 [4], 返回剩余 [2->1->3->5->6]
     split(剩余, 1): 取 [2], 返回剩余 [1->3->5->6]
     merge([4], [2]) -> [2->4]
     继续处理剩余...

   sz=2 轮:
     取 [2->4] 和 [1->3], merge -> [1->2->3->4]
     取 [5->6], merge 与 null -> [5->6]

   sz=4 轮:
     取 [1->2->3->4] 和 [5->6], merge -> [1->2->3->4->5->6]
   ```

4. **为什么链表不适合快排**：快排的核心是 partition，需要从两端向中间扫描（或随机访问 pivot）。单链表只能单向遍历，虽然也可以实现（以头节点为 pivot，遍历一遍分成小于和大于两部分），但最坏情况 O(n^2) 且不稳定。归并排序在链表上总是 O(n log n) 且稳定。

5. **dummy 节点的作用**：避免处理"头节点为空"或"头节点改变"的边界情况。合并时 `dummy.next` 最终指向合并后的真正头节点。

## 解法详解

### 解法1: 自顶向下归并排序 — O(n log n)/O(log n) ⭐面试首选

**思考过程**: 分治法最经典的应用。把链表从中点断开，递归排序左右两半，然后合并。和数组归并排序逻辑完全一致，区别只在于"找中点"用快慢指针、"断开"用指针操作。

```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        // base case: 空链表或只有一个节点，无需排序
        if (!head || !head->next) return head;

        // 第一步：快慢指针找中点
        // fast 从 head->next 开始，确保偶数长度时 slow 停在左半末尾
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 第二步：从中点断开链表
        ListNode* mid = slow->next;  // 右半部分的头
        slow->next = nullptr;        // 断开左右两半

        // 第三步：递归排序左右两半
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);

        // 第四步：合并两个有序链表
        return merge(left, right);
    }

private:
    // 合并两个有序链表（同 LC 21）
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);           // 哨兵节点，避免处理头节点边界
        ListNode* tail = &dummy;

        while (l1 && l2) {
            if (l1->val <= l2->val) { // <= 保证稳定性
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        // 接上剩余部分
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};
```

**关键点**: 
- `fast = head->next` 而非 `head`，否则偶数长度时分割不均匀，可能导致无限递归
- `slow->next = nullptr` 必须在取出 `mid` 之后再断开
- merge 中用 `<=` 而非 `<`，保证归并排序的稳定性

### 解法2: 自底向上归并排序 — O(n log n)/O(1)

**从解法1优化**: 解法1 的空间瓶颈是递归栈 O(log n)。自底向上方法完全用迭代替代递归：从 size=1 的子链表开始，每轮将相邻的两个 size 长度的子链表合并为 2*size，直到整个链表有序。

```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // 第一步：计算链表长度
        int length = 0;
        for (ListNode* p = head; p; p = p->next) length++;

        ListNode dummy(0, head);  // 哨兵节点

        // 第二步：从 size=1 开始，每轮翻倍
        for (int sz = 1; sz < length; sz *= 2) {
            ListNode* prev = &dummy;       // 已合并部分的尾部
            ListNode* curr = dummy.next;   // 当前待处理的起点

            while (curr) {
                // 取出左半部分（长度 sz）
                ListNode* left = curr;
                ListNode* right = split(left, sz);  // 截取 sz 个，返回剩余头

                // 取出右半部分（长度 sz）
                curr = split(right, sz);  // 截取 sz 个，返回下一轮的起点

                // 合并左右并接到 prev 后面
                prev->next = merge(left, right);

                // prev 移动到合并后链表的末尾
                while (prev->next) prev = prev->next;
            }
        }

        return dummy.next;
    }

private:
    // 从 head 开始截取 n 个节点，断开并返回剩余链表的头
    //
    // 示例: split([1->2->3->4->5], 2)
    //   截取 [1->2], 返回 [3->4->5]
    ListNode* split(ListNode* head, int n) {
        // 走 n-1 步到达第 n 个节点
        for (int i = 1; head && i < n; i++) {
            head = head->next;
        }
        if (!head) return nullptr;  // 不足 n 个节点

        ListNode* rest = head->next;  // 剩余部分的头
        head->next = nullptr;         // 断开
        return rest;
    }

    // 合并两个有序链表（与解法1相同）
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};
```

**关键点**: 
- `split` 函数是核心：截取前 n 个节点，断开，返回剩余部分头指针
- `prev->next = merge(left, right)` 后需要将 prev 走到尾部，为下一对合并做准备
- 外层循环 `sz *= 2`：1, 2, 4, 8, ... 直到 `sz >= length`

## 解法对比

| 维度 | 解法1: 自顶向下 | 解法2: 自底向上 |
|------|----------------|----------------|
| 时间 | O(n log n) | O(n log n) |
| 空间 | O(log n) 递归栈 | O(1) 仅指针变量 |
| 代码复杂度 | 中等，逻辑清晰 | 较高，split 逻辑要仔细 |
| 稳定性 | 稳定 | 稳定 |
| 面试推荐 | 首选，好写好解释 | Follow-up 追问时给出 |
| 本质 | 递归分治 | 迭代模拟分治 |

## 易错点

1. **快慢指针初始化错误导致无限递归**
   ```cpp
   // ✗ 错误：fast 从 head 开始，当链表只有 2 个节点时 slow 走到第 2 个
   //   导致左半 = [1,2]，右半 = []，左半递归又是 [1,2]，无限递归！
   ListNode* slow = head, *fast = head;
   while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }

   // ✓ 正确：fast 从 head->next 开始
   ListNode* slow = head, *fast = head->next;
   while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
   ```
   **原因**：fast 从 head 开始时，对 [1,2] 链表，循环执行一次后 slow=2, fast=null，mid=slow->next=null，右半为空，左半还是完整链表，死循环。

2. **忘记断开链表**
   ```cpp
   // ✗ 错误：只记录了 mid，但没有断开
   ListNode* mid = slow->next;
   // slow->next 还指向 mid，左半部分不是独立的链表！
   ListNode* left = sortList(head);   // 这会排序整个链表，不只是左半

   // ✓ 正确：断开连接
   ListNode* mid = slow->next;
   slow->next = nullptr;              // 关键！断开左右两半
   ListNode* left = sortList(head);
   ```
   **原因**：链表不像数组有天然的边界，必须手动设置 nullptr 来标记子链表的终点。

3. **自底向上的 split 返回值理解错误**
   ```cpp
   // ✗ 错误：以为 split 返回的是截取出来的部分
   ListNode* left = split(curr, sz);  // 实际返回的是剩余部分的头！

   // ✓ 正确：left 是 curr 本身（原地截取），split 返回的是剩余的头
   ListNode* left = curr;
   ListNode* right = split(left, sz);    // 从 left 截取 sz 个，right 是剩余头
   curr = split(right, sz);              // 从 right 截取 sz 个，curr 是下一轮起点
   ```
   **原因**：split 的语义是"从 head 开始取 n 个节点，断开，返回第 n+1 个节点"，被截取的部分就是原来的 head 到第 n 个节点。

4. **merge 中用 `<` 导致不稳定**
   ```cpp
   // ✗ 不稳定：相等时优先取 l2，相同值的相对顺序被打乱
   if (l1->val < l2->val) { tail->next = l1; ... }
   else { tail->next = l2; ... }

   // ✓ 稳定：相等时优先取 l1（来自前半部分），保持原始顺序
   if (l1->val <= l2->val) { tail->next = l1; ... }
   else { tail->next = l2; ... }
   ```

## 面试追问

### Q1: 为什么链表排序选归并而不是快排？
**回答**：
- **随机访问**：快排需要随机访问 pivot 或从两端向中间扫描，链表只能单向顺序访问，实现复杂且效率低。
- **时间复杂度保证**：链表快排最坏 O(n^2)（无法像数组那样随机选 pivot），而归并排序始终 O(n log n)。
- **空间效率**：链表归并不需要额外数组（数组归并需要 O(n) 辅助空间），链表只需改变指针指向。
- **稳定性**：归并排序天然稳定，快排不稳定。
- 反过来，数组排序更适合快排（随机访问 O(1)，cache friendly），链表排序更适合归并排序。

### Q2: 如何实现 O(1) 空间？递归不算 O(1) 吧？
**回答**：
是的，递归调用栈占 O(log n) 空间。要实现真正的 O(1) 空间，需要用自底向上的迭代归并（解法2）：
- 从 size=1 开始，每轮将相邻的两个 size 长度子链表合并
- 外层循环 size 翻倍：1 -> 2 -> 4 -> 8 -> ... 直到 >= n
- 内层循环遍历链表，依次取出和合并相邻的子链表对
- 只使用固定数量的指针变量，所以是 O(1) 空间

关键实现是 split 函数：从链表头部截取指定长度的子链表，断开并返回剩余部分的头。

### Q3: 如果允许 O(n) 空间，有没有更简单的方法？
**回答**：
有。最简单的方法是把链表值复制到数组中，用 `sort()` 排序，再写回链表：
```cpp
ListNode* sortList(ListNode* head) {
    vector<int> vals;
    for (auto p = head; p; p = p->next) vals.push_back(p->val);
    sort(vals.begin(), vals.end());
    int i = 0;
    for (auto p = head; p; p = p->next) p->val = vals[i++];
    return head;
}
```
时间 O(n log n)，空间 O(n)。面试中这不是期望答案，但可以作为暴力解法先提出，再优化。

## 相关题型

| 题目 | 关系 | 复用技巧 |
|------|------|---------|
| [21. 合并两个有序链表](https://leetcode.com/problems/merge-two-sorted-lists/) | 子问题 | merge 函数完全复用，是本题的核心组件 |
| [876. 链表的中间结点](https://leetcode.com/problems/middle-of-the-linked-list/) | 子问题 | 快慢指针找中点，是本题拆分的核心 |
| [23. 合并 K 个升序链表](https://leetcode.com/problems/merge-k-sorted-lists/) | 扩展 | 归并排序的分治思想，分治合并 K 个链表 |
| [147. 对链表进行插入排序](https://leetcode.com/problems/insertion-sort-list/) | 同类型 | O(n^2) 链表排序，对比归并的优势 |
| [912. 排序数组](https://leetcode.com/problems/sort-an-array/) | 同算法 | 归并排序在数组上的实现，对比链表版本差异 |
| [143. 重排链表](https://leetcode.com/problems/reorder-list/) | 技巧复用 | 同样用快慢指针找中点 + 链表操作 |
