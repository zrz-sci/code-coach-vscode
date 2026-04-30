# 面试题 02.02. 返回倒数第 k 个节点

## 核心思路

经典的**快慢指针（双指针间距法）** 问题。核心思想：让快指针先走 k 步拉开间距，然后快慢指针同步前进，当快指针到达链表末尾时，慢指针恰好停在倒数第 k 个节点。一次遍历、O(1) 空间，是面试中链表题的必备技巧。

## 思维链

1. **读完题第一反应** → 单链表找倒数第 k 个？最直接的想法：先遍历一次算总长度 n，然后再走 n-k 步到目标节点。两次遍历。
2. **能否只遍历一次？** → 如果有两个指针，一个先走 k 步，它就比另一个"领先 k 个位置"。之后两个指针同步走，先走的到末尾时，后走的正好在倒数第 k 个。
3. **为什么有效？** → 快慢指针始终保持 k 步的间距。快指针到 null 时走了 n 步，慢指针走了 n-k 步，正好是倒数第 k 个节点。
4. **边界情况** → 题目保证 k 有效，不需要特殊处理 k > 链表长度的情况。但实际面试中可以讨论。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 两次遍历 | 先算长度再定位 | O(n) | O(1) | 基础解法 |
| 快慢指针 | 快指针先走k步 | O(n) | O(1) | ⭐ 面试首选 |
| 递归 | 利用递归栈回溯计数 | O(n) | O(n) | 可以讨论 |
| 栈 | 全部入栈后弹出k次 | O(n) | O(n) | 不推荐 |

## 关键提示

1. **间距模型**：快慢指针的本质是"固定间距"。快先走 k 步后，fast 和 slow 之间恰好有 k 个节点的间距。之后同步移动，间距不变。当 fast == null 时，slow 就在正确位置。
2. **为什么不是 k-1 步？** 这取决于"倒数第 k 个"的定义。题目中倒数第 1 个 = 最后一个节点。如果 fast 先走 k 步：fast 到 null 时走了 n 步（经过 n 个节点），slow 走了 n-k 步，停在第 n-k+1 个节点 = 倒数第 k 个。正确！
3. **指针图理解**：画图是链表题的核心技巧，下面的指针图展示了整个过程。

## 解法详解

### 解法1: 快慢指针 — O(n) / O(1) ⭐ 面试首选

**思考过程**: 一次遍历找倒数第 k 个，用两个指针保持固定间距。

```
指针图 (链表: 1→2→3→4→5, k=2):

初始状态:
  slow  fast
   ↓     ↓
   1 → 2 → 3 → 4 → 5 → null

Step 1: fast 先走 k=2 步
   slow        fast
    ↓           ↓
    1 → 2 → 3 → 4 → 5 → null
        ↑       ↑
    第0步后  第1步后(i=0)
              第2步后(i=1) → fast 在节点3

Step 2: fast 和 slow 同步前进, 直到 fast == null
   Round 1:
        slow        fast
         ↓           ↓
    1 → 2 → 3 → 4 → 5 → null

   Round 2:
             slow        fast
              ↓           ↓
    1 → 2 → 3 → 4 → 5 → null

   Round 3:
                  slow        fast
                   ↓           ↓
    1 → 2 → 3 → 4 → 5 → null
                   ↑
              fast == null, 停！
              slow 在节点 4 = 倒数第2个 ✓

间距始终保持 k=2:
    slow 在节点4, fast 在 null (原本节点5的下一个)
    4 是倒数第2个 ✓
```

```cpp
class Solution {
public:
    int kthToLast(ListNode* head, int k) {
        ListNode* fast = head;
        ListNode* slow = head;
        
        // Step 1: fast 先走 k 步
        for (int i = 0; i < k; i++) {
            fast = fast->next;
        }
        
        // Step 2: 同步前进直到 fast 到末尾
        while (fast != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }
        
        return slow->val;
    }
};
```

**关键点**:
- 循环条件是 `fast != nullptr`（而不是 `fast->next != nullptr`）。如果用 `fast->next`，slow 会停在倒数第 k+1 个位置，差一步。
- 快指针先走 k 步后，如果 fast 已经是 null，说明 k == 链表长度，倒数第 k 个就是第一个节点，此时 slow 仍在 head，正确。

---

### 解法2: 两次遍历 — O(n) / O(1)

**最直接的思路**: 先数一遍长度，再走 n-k 步。

```
链表: 1→2→3→4→5, k=2

第一遍: 计算长度 n = 5
第二遍: 走 n - k = 5 - 2 = 3 步
        head → 1 → 2 → 3 → 4
                            ↑
                        走了3步，到达节点4 = 倒数第2个 ✓
```

```cpp
class Solution {
public:
    int kthToLast(ListNode* head, int k) {
        // 第一遍: 计算长度
        int n = 0;
        ListNode* curr = head;
        while (curr) {
            n++;
            curr = curr->next;
        }
        
        // 第二遍: 走 n-k 步
        curr = head;
        for (int i = 0; i < n - k; i++) {
            curr = curr->next;
        }
        
        return curr->val;
    }
};
```

**关键点**:
- 需要两次遍历，时间常数比快慢指针大一倍。
- 思路简单直白，适合作为"起步方案"在面试中先说。

---

### 解法3: 递归 — O(n) / O(n)

**不同视角**: 利用递归的"回溯"特性。递归到链表末尾后，回溯时开始计数，当计数到 k 时就是目标节点。

```cpp
class Solution {
    int count = 0;
    int result = 0;
public:
    int kthToLast(ListNode* head, int k) {
        helper(head, k);
        return result;
    }
    
    void helper(ListNode* node, int k) {
        if (!node) return;
        helper(node->next, k);  // 先递归到末尾
        count++;                 // 回溯时计数
        if (count == k) {
            result = node->val;  // 找到目标
        }
    }
};
```

**关键点**:
- 递归栈深度为 O(n)，空间复杂度 O(n)，不如快慢指针。
- 但这种"递归 + 回溯计数"的思路在链表题中很常用，值得掌握。

---

### 解法4: 栈 — O(n) / O(n)

**辅助数据结构**: 把所有节点值压入栈，然后弹出 k 次，最后弹出的就是倒数第 k 个。

```cpp
class Solution {
public:
    int kthToLast(ListNode* head, int k) {
        stack<int> stk;
        while (head) {
            stk.push(head->val);
            head = head->next;
        }
        for (int i = 1; i < k; i++) {
            stk.pop();
        }
        return stk.top();
    }
};
```

**关键点**:
- 简单但空间 O(n)，面试中不推荐作为最终方案。
- 但可以作为思路说明"栈的 LIFO 特性天然适合处理'倒数'问题"。

## 解法对比

|  | 快慢指针 | 两次遍历 | 递归 | 栈 |
|--|---------|---------|------|-----|
| 时间 | O(n) | O(n) | O(n) | O(n) |
| 空间 | **O(1)** | **O(1)** | O(n) | O(n) |
| 遍历次数 | **1次** | 2次 | 1次 | 1次 + k次弹出 |
| 面试评价 | ⭐ 最优 | 基础过关 | 展示递归功底 | 不推荐 |

**面试选择**: 先说两次遍历的思路（表明你能想到暴力解），然后优化到快慢指针（一次遍历）。

## 易错点

1. **快指针先走 k-1 步而非 k 步**：
   - ✗ `for (int i = 0; i < k-1; i++) fast = fast->next;` → slow 会停在倒数第 k+1 个位置
   - ✓ `for (int i = 0; i < k; i++) fast = fast->next;`

2. **循环终止条件写错**：
   - ✗ `while (fast->next)` → slow 停在倒数第 k+1 个位置
   - ✓ `while (fast)` → slow 停在倒数第 k 个位置
   - 具体来说：如果用 `fast->next`，当 fast 在最后一个节点时就停了，fast 比预期少走一步，slow 也少走一步。

3. **两次遍历时走步数算错**：
   - ✗ `for (int i = 0; i < n - k + 1; i++)` → 多走一步
   - ✓ `for (int i = 0; i < n - k; i++)` → 从 head 出发走 n-k 步

4. **没有处理 k > 链表长度的情况**（防御性编程）：
   - 题目保证 k 有效，但面试中如果不保证，需要在 fast 先走 k 步时检查 `if (!fast) return -1;`

5. **返回值混淆——返回节点还是值**：
   - 本题要求返回 `int`（节点值），而不是 `ListNode*`（节点指针）
   - LeetCode 19 题要求的是删除节点（需要返回 ListNode*），注意区分

## 面试追问

**Q1: 如果要返回倒数第 k 个节点的指针而非值，代码怎么改？**
> 把返回值类型改为 `ListNode*`，`return slow;` 即可。这就变成了 LeetCode 19（删除倒数第 N 个节点）的子问题。

**Q2: 如果要删除倒数第 k 个节点呢？（LeetCode 19）**
> 需要找到倒数第 k+1 个节点（即目标节点的前一个）。技巧：使用 dummy head 节点。让 fast 从 dummy 开始先走 k+1 步（或等价地，从 head 走 k 步，但 slow 从 dummy 开始），这样 slow 停在目标节点的前一个位置，执行 `slow->next = slow->next->next` 删除。

**Q3: 如果 k 可能无效（k ≤ 0 或 k > 链表长度），怎么处理？**
> 在 fast 先走 k 步的循环中加入检查：如果 fast 提前变成 nullptr 且 i < k，说明 k > 链表长度，返回错误标志。k ≤ 0 在函数入口直接判断。

**Q4: 如果链表有环呢？**
> 如果链表有环，快慢指针方法会无限循环。需要先用快慢指针检测环（Floyd判环），确认无环后再找倒数第 k 个。如果有环，"倒数第 k 个"本身就没有意义（链表没有末尾）。

**Q5: 能否不用额外变量，纯递归返回结果？**
> 可以用引用参数传递计数器：`int helper(ListNode* node, int& k)`，递归到末尾后回溯时 k--，当 k == 0 时返回当前节点值。这样避免了成员变量。

## 相关题型

- **[19. 删除链表的倒数第 N 个节点](../0019_remove_nth_node_from_end_of_list/)** — 本题的直接扩展：找到倒数第 N+1 个节点后执行删除。增加 dummy head 处理边界（删除头节点的情况）。快慢指针间距从 k 变为 k+1。
- **[876. 链表的中间节点](../0876_middle_of_the_linked_list/)** — 同样是快慢指针，但间距策略不同：fast 每次走 2 步、slow 每次走 1 步。fast 到末尾时 slow 在中间。
- **[141. 环形链表](../0141_linked_list_cycle/)** — 快慢指针的另一个经典应用：快走 2 步慢走 1 步，如果相遇则有环。
- **[61. 旋转链表](../0061_rotate_list/)** — 需要找到倒数第 k%n 个节点作为新的头，复用本题的快慢指针技巧。
