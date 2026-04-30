# LCR 077. 排序链表 (Sort List)

## 难度: Medium | 标签: Linked List, Two Pointers, Divide and Conquer, Sorting, Merge Sort

**注意**: 本题与主站第 148 题相同。

---

## 题目理解

给定链表的头节点 `head`，将其按**升序**排列并返回排序后的链表。

**进阶要求**: O(n log n) 时间复杂度和**常数级空间**复杂度。

关键点：
- 链表不能随机访问，快速排序的 partition 需要额外技巧
- 归并排序天然适合链表（不需要额外空间存临时数组）
- 自顶向下归并用 O(log n) 递归栈，自底向上归并可做到 O(1) 空间

---

## 思维链

```
1. 读完题 --> 链表排序，要求 O(n log n)
   数组排序有: 快排、归并、堆排
   链表特性: 不能随机访问，但合并操作 O(1) 空间

2. 为什么选归并排序?
   - 归并排序: 分治 + 合并
   - 链表的合并不需要额外数组（直接改指针）
   - 天然适合链表!

3. 归并排序三步:
   a. 找中点: 快慢指针!
   b. 分割: 断开链表
   c. 合并: 合并两个有序链表

4. 找中点 - 快慢指针:
   slow 走一步, fast 走两步
   fast 到末尾时, slow 在中点
   注意: fast 初始化为 head->next (偶数节点时取左半中点)

5. 合并两个有序链表:
   - dummy 头节点简化边界处理
   - 逐个比较, 小的接到 tail 后面
   - 经典题 LC21 的做法

6. 进阶 O(1) 空间:
   自底向上归并: 不递归
   - 先合并长度为 1 的段, 再合并长度为 2 的, 4, 8, ...
   - 直到整个链表有序
```

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 特点 |
|------|------|------|------|------|
| **自顶向下归并** | 递归分治 | O(n log n) | O(log n) | 最直观，推荐先写 |
| **自底向上归并** | 迭代合并 | O(n log n) | O(1) | 进阶，常数空间 |

---

## 解法一: 自顶向下归并排序 -- O(n log n) / O(log n) 【推荐】

### 核心思路

```
sortList(head):
  1. base case: 空链表或单节点 -> 已有序
  2. 找中点: 快慢指针
  3. 断开: slow->next = nullptr
  4. 递归排序左半和右半
  5. 合并两个有序链表
```

### 指针图解: 找中点 + 断开

```
原始链表: 4 -> 2 -> 1 -> 3

Step 1: 快慢指针找中点
  初始: slow = head(4), fast = head->next(2)

  第1步:
    slow -> 2
    fast -> 3 (fast->next = nullptr, 停止)
                  s         f
    4 -> 2 -> 1 -> 3 -> null
         ^              ^
        slow           fast

  中点 = slow = 节点2, mid = slow->next = 节点1

Step 2: 断开
    slow->next = nullptr

    左半: 4 -> 2 -> null
    右半: 1 -> 3 -> null

    4 -> 2 -> null    1 -> 3 -> null
    ^                 ^
    left              right

Step 3: 递归排序
    sortList(4->2) :
      找中点: slow=4, mid=2
      断开: 4->null, 2->null
      排序: 4, 2
      合并: 2->4

    sortList(1->3) :
      找中点: slow=1, mid=3
      断开: 1->null, 3->null
      排序: 1, 3
      合并: 1->3

    左半排序后: 2 -> 4 -> null
    右半排序后: 1 -> 3 -> null

Step 4: 合并两个有序链表
    dummy -> null
    tail = dummy

    比较 2 vs 1: 1 小
    dummy -> 1    tail = 1
                  remaining: 2->4, 3->null

    比较 2 vs 3: 2 小
    dummy -> 1 -> 2    tail = 2
                       remaining: 4, 3->null

    比较 4 vs 3: 3 小
    dummy -> 1 -> 2 -> 3    tail = 3
                            remaining: 4, null

    l2 为空, 接上剩余:
    dummy -> 1 -> 2 -> 3 -> 4 -> null

    返回 dummy.next = 1 -> 2 -> 3 -> 4  ✓
```

### 更复杂的例子: 5个节点

```
原始: -1 -> 5 -> 3 -> 4 -> 0

找中点 (fast = head->next):
  slow=-1, fast=5
  slow=5,  fast=4
  slow=3,  fast=null (停)
  mid = slow->next = 4

  左半: -1 -> 5 -> 3 -> null
  右半: 4 -> 0 -> null

  递归排序左半:
    -1 -> 5 -> 3
    找中点: slow=-1, fast=5; slow=5, fast=null -> mid=3
    左: -1->5->null, 右: 3->null
    排序左: -1->5 -> 找中点 -> -1, 5 -> 合并 -> -1->5
    排序右: 3
    合并: -1->3->5

  递归排序右半:
    4 -> 0
    找中点: slow=4, fast=null -> mid=0
    左: 4, 右: 0
    合并: 0->4

  合并 (-1->3->5) 和 (0->4):
    -1 vs 0: -1
    3 vs 0: 0
    3 vs 4: 3
    5 vs 4: 4
    剩余 5
    结果: -1 -> 0 -> 3 -> 4 -> 5  ✓
```

### 代码

```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        // base case: 空链表或单节点
        if (!head || !head->next) return head;

        // Step 1: 快慢指针找中点
        ListNode* slow = head;
        ListNode* fast = head->next;  // 注意: fast 从 head->next 开始!
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // Step 2: 断开
        ListNode* mid = slow->next;
        slow->next = nullptr;

        // Step 3: 递归排序
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);

        // Step 4: 合并
        return merge(left, right);
    }

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

---

## 解法二: 自底向上归并排序 -- O(n log n) / O(1) 【进阶】

### 核心思路

```
不使用递归，从最小的子段开始合并:

step = 1: 每 1 个节点为一组，两两合并
step = 2: 每 2 个节点为一组，两两合并
step = 4: 每 4 个节点为一组，两两合并
...
直到 step >= n

每轮遍历整个链表，将相邻的两段合并。

例: 4 -> 2 -> 1 -> 3

step=1: [4] [2] [1] [3]
  合并 [4]+[2] = [2,4]
  合并 [1]+[3] = [1,3]
  结果: 2 -> 4 -> 1 -> 3

step=2: [2,4] [1,3]
  合并 [2,4]+[1,3] = [1,2,3,4]
  结果: 1 -> 2 -> 3 -> 4
```

### 指针图解: 自底向上合并

```
初始: 4 -> 2 -> 1 -> 3
n = 4

===== step = 1 =====

dummy -> 4 -> 2 -> 1 -> 3
tail = dummy

  切出第1段 (长度1): head1 = 4, 剩余 = 2->1->3
  切出第2段 (长度1): head2 = 2, 剩余 = 1->3
  合并 [4] + [2] = 2->4
  tail 指向合并后的尾部(4)

  dummy -> 2 -> 4 -> ...
                ^
               tail

  切出第3段 (长度1): head1 = 1, 剩余 = 3
  切出第4段 (长度1): head2 = 3, 剩余 = null
  合并 [1] + [3] = 1->3
  tail->next = 1

  dummy -> 2 -> 4 -> 1 -> 3 -> null

===== step = 2 =====

dummy -> 2 -> 4 -> 1 -> 3
tail = dummy

  切出第1段 (长度2): head1 = 2->4, 剩余 = 1->3
  切出第2段 (长度2): head2 = 1->3, 剩余 = null
  合并 [2,4] + [1,3] = 1->2->3->4
  
  dummy -> 1 -> 2 -> 3 -> 4 -> null

step=4 >= n=4, 停止
结果: 1 -> 2 -> 3 -> 4  ✓
```

### 代码

```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // 计算链表长度
        int n = 0;
        for (ListNode* p = head; p; p = p->next) n++;

        ListNode dummy(0);
        dummy.next = head;

        // step 从 1 开始，每次翻倍
        for (int step = 1; step < n; step *= 2) {
            ListNode* tail = &dummy;
            ListNode* cur = dummy.next;

            while (cur) {
                // 切出第一段 (长度 step)
                ListNode* head1 = cur;
                ListNode* prev1 = split(cur, step);
                cur = prev1 ? prev1->next : nullptr;
                if (prev1) prev1->next = nullptr;

                // 切出第二段 (长度 step)
                ListNode* head2 = cur;
                ListNode* prev2 = split(cur, step);
                cur = prev2 ? prev2->next : nullptr;
                if (prev2) prev2->next = nullptr;

                // 合并两段，接到 tail 后面
                auto [mergedHead, mergedTail] = mergeWithTail(head1, head2);
                tail->next = mergedHead;
                tail = mergedTail;
            }
        }
        return dummy.next;
    }

private:
    // 从 head 开始走 n 步，返回第 n 个节点（即断开点前一个）
    ListNode* split(ListNode* head, int n) {
        ListNode* prev = nullptr;
        while (head && n > 0) {
            prev = head;
            head = head->next;
            n--;
        }
        return prev;
    }

    // 合并两个有序链表，返回 {头, 尾}
    pair<ListNode*, ListNode*> mergeWithTail(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1; l1 = l1->next;
            } else {
                tail->next = l2; l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        while (tail->next) tail = tail->next;
        return {dummy.next, tail};
    }
};
```

---

## 关键子操作详解

### 1. 快慢指针找中点

```
为什么 fast 初始化为 head->next 而不是 head?

fast = head:      偶数长度时 slow 停在右半中点
  1 -> 2 -> 3 -> 4
  s=1  s=2  s=3
  f=1  f=3  f=null(停)
  slow = 3 (右半)

fast = head->next: 偶数长度时 slow 停在左半中点
  1 -> 2 -> 3 -> 4
  s=1  s=2
  f=2  f=4  f.next=null(停)
  slow = 2 (左半)

用 fast = head->next:
  左半: 1->2  右半: 3->4  (均匀分割)

用 fast = head:
  左半: 1->2->3  右半: 4  (不均匀, 极端情况导致无限递归!)

结论: fast 一定要从 head->next 开始!
```

### 2. 合并有序链表

```
使用 dummy 节点避免处理头节点的特殊情况:

l1: 1 -> 3 -> 5
l2: 2 -> 4

dummy -> null
tail = dummy

Step 1: 1 < 2, dummy->1, l1=3
Step 2: 3 > 2, dummy->1->2, l2=4
Step 3: 3 < 4, dummy->1->2->3, l1=5
Step 4: 5 > 4, dummy->1->2->3->4, l2=null
Step 5: l2 空, 接上 l1: dummy->1->2->3->4->5

返回 dummy.next
```

---

## 关键提示序列（渐进式）

| # | 提示 | 思路方向 |
|---|------|----------|
| 1 | 链表排序用什么算法？ | 归并排序最适合 |
| 2 | 如何将链表一分为二？ | 快慢指针找中点 |
| 3 | fast 从哪里开始？ | head->next（避免无限递归） |
| 4 | 分割后如何断开？ | slow->next = nullptr |
| 5 | 如何做到 O(1) 空间？ | 自底向上迭代归并 |

---

## 易错点

1. **快慢指针的初始化**
   - `fast = head->next`（不是 head!）
   - 否则偶数长度时分割不均，可能无限递归

2. **断开链表**
   - `slow->next = nullptr` 必须在取 `mid = slow->next` 之后
   - 先取 mid，再断开，顺序不能反

3. **合并时的稳定性**
   - `l1->val <= l2->val` 用 `<=`（不是 `<`）
   - 保证排序稳定性

4. **空链表和单节点**
   - `if (!head || !head->next) return head;`
   - 忘记判断会导致快慢指针出错

5. **自底向上归并的切割**
   - 切出 step 长度的段后，要断开链表
   - 记录剩余部分的起点，否则丢失后半部分

6. **递归栈空间**
   - 自顶向下: O(log n) 栈空间
   - 严格 O(1) 空间需要自底向上

---

## 复杂度分析

### 解法一（自顶向下）
- **时间: O(n log n)** -- 递归 log n 层，每层 O(n)
- **空间: O(log n)** -- 递归栈深度

### 解法二（自底向上）
- **时间: O(n log n)** -- log n 轮合并，每轮 O(n)
- **空间: O(1)** -- 只用常数个指针变量

---

## 排序算法在链表上的适用性

```
+----------+-----------+--------+------+---------+
| 算法     | 时间      | 空间   | 链表 | 原因    |
+----------+-----------+--------+------+---------+
| 归并排序 | O(n logn) | O(1)*  | 最佳 | 顺序访问 |
| 快速排序 | O(n logn) | O(logn)| 可行 | partition需技巧 |
| 插入排序 | O(n^2)    | O(1)   | 可行 | 适合近乎有序 |
| 堆排序   | O(n logn) | O(n)   | 差   | 需要随机访问 |
| 计数/桶  | O(n+k)    | O(k)   | 可行 | 值域有限时 |
+----------+-----------+--------+------+---------+
* 自底向上归并
```

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 21 | Merge Two Sorted Lists | 合并子操作 |
| 148 | Sort List | 本题原版 |
| 876 | Middle of the Linked List | 快慢指针找中点 |
| 23 | Merge k Sorted Lists | 归并排序扩展 |
| 147 | Insertion Sort List | 链表插入排序 |

---

## 面试技巧

1. **先说整体框架**: "归并排序三步: 找中点、分割、合并"
2. **画指针图**: 面试中在白板上画出快慢指针的移动过程
3. **先写自顶向下版本**: 简单清晰，先过面试
4. **被问进阶时再写自底向上**: 展示你理解 O(1) 空间的技巧
5. **提到合并有序链表**: "这是 LC21 的子问题，我已经熟悉了"

---

## 面试追问

**Q1: 能否做到 O(1) 空间?**
> 自底向上归并。step=1,2,4,...逐轮合并，只用常数个指针，无递归栈。

**Q2: 链表能用快排吗? 复杂度如何?**
> 可以。以 head 为 pivot，遍历分成 <pivot 和 >=pivot 两链，递归排序后拼接。平均 O(n log n)，最坏 O(n^2)。缺点：不能随机选 pivot，退化概率更高。

**Q3: 如果链表近乎有序，有更快的方法吗?**
> 链表插入排序 (LC147)。近乎有序时接近 O(n)。或者 "自然归并"：找到已有序的 run 直接合并，对近乎有序的输入趋近 O(n)。
