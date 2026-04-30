# 82. Remove Duplicates from Sorted List II - 提示与详解

## 难度：Medium | 标签：Linked List, Two Pointers

---

## 一、核心思路

本题的核心是 **Dummy 节点 + 双指针跳过整段重复**。

与 LC 83（保留一个重复元素）不同，本题要求**删除所有出现过重复的元素**。例如 [1,2,3,3,4,4,5] 变为 [1,2,5]，3 和 4 全部删除。

关键难点：
1. **头节点可能被删除**（如 [1,1,2]），需要 dummy 节点
2. 需要记录"最后一个确认不重复的节点"（prev），用于跳过整段重复

**一句话总结**：用 prev 指针连接所有不重复的节点，遇到重复段就整段跳过。

---

## 二、思维链：从题目到解法

### Step 1：理解"删除所有重复" vs "保留一个"

```
LC 83 (保留一个):  [1,2,3,3,4,4,5] → [1,2,3,4,5]
LC 82 (全部删除):  [1,2,3,3,4,4,5] → [1,2,5]

差异：3 出现了两次，83 保留一个 3，82 把所有 3 都删掉
```

### Step 2：为什么需要 Dummy 节点？

```
输入: [1,1,1,2,3]
期望: [2,3]

头节点 1 本身就是重复的，需要被删除。
如果没有 dummy，我们需要特殊处理头节点的变化。
有了 dummy，统一处理所有情况。
```

### Step 3：指针图解 —— 核心算法

```
初始状态:
dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
  ↑
 prev

Step 1: prev->next = [1], curr = [1]
        curr->next = [2], 1 != 2 → 不重复，prev 前进
        
dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
         ↑
        prev

Step 2: prev->next = [2], curr = [2]
        curr->next = [3], 2 != 3 → 不重复，prev 前进
        
dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
                ↑
               prev

Step 3: prev->next = [3], curr = [3]
        curr->next = [3], 3 == 3 → 重复！
        val = 3
        while (curr && curr->val == 3): curr 跳到 [4]
        prev->next = [4]  (跳过了两个 3)
        
dummy → [1] → [2] ──→ [4] → [4] → [5] → null
                ↑
               prev    (跳过了 3→3)

Step 4: prev->next = [4], curr = [4]
        curr->next = [4], 4 == 4 → 重复！
        val = 4
        while (curr && curr->val == 4): curr 跳到 [5]
        prev->next = [5]  (跳过了两个 4)
        
dummy → [1] → [2] ──────→ [5] → null
                ↑
               prev        (跳过了 4→4)

Step 5: prev->next = [5], curr = [5]
        curr->next = null → 不重复，prev 前进
        
dummy → [1] → [2] ──────→ [5] → null
                            ↑
                           prev

prev->next = null → 循环结束

返回 dummy->next = [1] → [2] → [5]
```

### Step 4：处理头节点被删除的情况

```
输入: [1,1,1,2,3]

dummy → [1] → [1] → [1] → [2] → [3] → null
  ↑
 prev

Step 1: prev->next = [1], curr = [1]
        curr->next = [1], 1 == 1 → 重复！
        跳过所有 1 → curr 到 [2]
        prev->next = [2]
        
dummy ──→ [2] → [3] → null
  ↑
 prev

Step 2: prev->next = [2], 不重复, prev 前进
Step 3: prev->next = [3], 不重复, prev 前进

返回 dummy->next = [2] → [3]

注意：原始 head [1] 被跳过了，dummy 节点让这变得自然。
```

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 迭代 + Dummy | prev 指针跳过重复段 | O(n) | O(1) | 面试首选 |
| 递归 | 递归处理子链表 | O(n) | O(n) 栈 | 代码简洁 |
| 计数 + 两次遍历 | 先统计频次再删除 | O(n) | O(n) | 思路直观 |

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：头节点可能被删除，怎么处理？</summary>

创建一个 dummy 节点指向 head。最后返回 dummy->next。这样即使 head 被删除，也能正确返回新的头节点。

</details>

<details>
<summary>提示 2：如何判断一个节点是否是重复的？</summary>

如果 curr->val == curr->next->val，那么 curr 开始的一段都是重复的。需要记录这个值 val，然后一直跳过所有值为 val 的节点。

</details>

<details>
<summary>提示 3：跳过重复段后，prev 指针怎么接上？</summary>

prev->next = curr（curr 已经跳到第一个不等于 val 的节点）。注意 prev 不能前进，因为 curr 可能也是新一段重复的开始。

</details>

<details>
<summary>提示 4：什么时候 prev 才能前进？</summary>

只有当 prev->next 和 prev->next->next 的值不同时（或 prev->next->next 为空），prev 才能安全地前进到 prev->next。

</details>

<details>
<summary>提示 5：递归版本的核心思路是什么？</summary>

如果 head->val == head->next->val：跳过所有相同值的节点，然后递归处理剩余部分。
如果 head->val != head->next->val：保留 head，递归处理 head->next。

</details>

---

## 五、解法详解

### 解法一：迭代 + Dummy 节点（面试首选）

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // Step 1: 创建 dummy 节点，处理头节点被删除的情况
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        // Step 2: 遍历链表
        while (prev->next) {
            ListNode* curr = prev->next;

            // 检查是否有重复
            if (curr->next && curr->val == curr->next->val) {
                // 记录重复值
                int val = curr->val;
                // 跳过所有值为 val 的节点
                while (curr && curr->val == val) {
                    curr = curr->next;
                }
                // prev->next 直接接到第一个不等于 val 的节点
                prev->next = curr;
                // 注意：prev 不前进！因为 curr 可能也是重复的开始
            } else {
                // 不重复，prev 安全前进
                prev = prev->next;
            }
        }

        return dummy.next;
    }
};
```

**完整指针追踪**：

```
输入: [1,2,3,3,4,4,5]

迭代1: prev=dummy, curr=[1], next=[2], 1!=2 → prev=[1]
迭代2: prev=[1],   curr=[2], next=[3], 2!=3 → prev=[2]
迭代3: prev=[2],   curr=[3], next=[3], 3==3 → 跳过3,3 → prev→next=[4]
迭代4: prev=[2],   curr=[4], next=[4], 4==4 → 跳过4,4 → prev→next=[5]
迭代5: prev=[2],   curr=[5], next=null       → prev=[5]
迭代6: prev=[5],   prev→next=null → 退出

结果: dummy→[1]→[2]→[5]→null
```

### 解法二：递归

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // 基准情况：空链表或单节点
        if (!head || !head->next) return head;

        if (head->val == head->next->val) {
            // 当前值是重复的，跳过所有相同值的节点
            int val = head->val;
            while (head && head->val == val) {
                head = head->next;
            }
            // head 现在指向第一个不同值的节点（或 null）
            return deleteDuplicates(head);  // 递归处理剩余
        } else {
            // 当前值不重复，保留 head，递归处理后续
            head->next = deleteDuplicates(head->next);
            return head;
        }
    }
};
```

**递归展开**：

```
deleteDuplicates([1,2,3,3,4,4,5])
  1 != 2 → 保留 1
  1->next = deleteDuplicates([2,3,3,4,4,5])
              2 != 3 → 保留 2
              2->next = deleteDuplicates([3,3,4,4,5])
                          3 == 3 → 跳过 3,3 → head=[4,4,5]
                          return deleteDuplicates([4,4,5])
                                   4 == 4 → 跳过 4,4 → head=[5]
                                   return deleteDuplicates([5])
                                            单节点 → return [5]
              2->next = [5]
  1->next = [2]→[5]
return [1]→[2]→[5]
```

### 解法三：使用 set 标记重复值（两次遍历）

```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // 第一次遍历：找出所有重复的值
        unordered_set<int> duplicates;
        unordered_map<int, int> count;
        ListNode* curr = head;
        while (curr) {
            count[curr->val]++;
            if (count[curr->val] > 1) {
                duplicates.insert(curr->val);
            }
            curr = curr->next;
        }

        // 第二次遍历：删除所有重复值的节点
        ListNode dummy(0, head);
        ListNode* prev = &dummy;
        curr = head;
        while (curr) {
            if (duplicates.count(curr->val)) {
                prev->next = curr->next;  // 跳过
            } else {
                prev = curr;  // 保留
            }
            curr = curr->next;
        }

        return dummy.next;
    }
};
```

---

## 六、Dummy 节点技巧详解

```
为什么链表题经常需要 Dummy 节点？

场景1：头节点可能被删除
  输入: [1,1,2]
  没有 dummy: 需要特殊判断 head 是否应该被删除
  有了 dummy: dummy->next 就是新头，统一处理

场景2：需要 prev 指针
  prev 初始化为谁？如果初始化为 head，就没法删除 head
  有了 dummy: prev 初始化为 &dummy，可以删除 head

Dummy 节点模板：
  ListNode dummy(0, head);   // 创建 dummy，指向 head
  // ... 处理 ...
  return dummy.next;          // 返回真正的头

栈上分配 vs 堆上分配：
  ListNode dummy(0, head);           // 栈上，自动释放
  ListNode* dummy = new ListNode(0, head);  // 堆上，需手动 delete
  面试中用栈上分配更安全。
```

---

## 七、解法对比

| 维度 | 迭代 + Dummy | 递归 | 两次遍历 |
|------|-------------|------|---------|
| 时间复杂度 | O(n) | O(n) | O(n) |
| 空间复杂度 | O(1) | O(n) 栈 | O(n) set |
| 代码量 | 中等 | 简洁 | 较多 |
| 面试推荐 | ⭐⭐⭐ | ⭐⭐ | ⭐ |
| 能处理无序链表 | 否 | 否 | 是 |

---

## 八、易错点

### Bug 1：忘记用 Dummy 节点

```cpp
// 错误：直接用 head 作为起点
ListNode* prev = head;
// 如果 head 本身就是重复的 [1,1,2]，无法删除 head
```

### Bug 2：跳过重复时未检查 null

```cpp
// 错误：while 循环没有检查 curr 是否为 null
int val = curr->val;
while (curr->val == val) {  // curr 可能是 nullptr！
    curr = curr->next;
}
// 正确：
while (curr && curr->val == val) {
    curr = curr->next;
}
```

### Bug 3：跳过重复后 prev 错误地前进

```cpp
// 错误：跳过重复后立即 prev = prev->next
if (curr->next && curr->val == curr->next->val) {
    // ... 跳过重复段 ...
    prev->next = curr;
    prev = prev->next;  // 错！curr 可能也是新一段重复的开始！
}
```

**图解 Bug 3**：
```
输入: [1,2,2,3,3,4]

假设跳过 2,2 后 prev->next = [3]，然后 prev 前进到 [3]
但 [3] 也是重复的！prev 已经前进了就无法跳过 [3,3]

正确做法：跳过重复后 prev 不前进，下一轮循环再检查
```

### Bug 4：混淆 LC 82 和 LC 83

```
LC 83（保留一个）: 简单得多
  if (curr->val == curr->next->val) curr->next = curr->next->next;
  
LC 82（全部删除）: 需要 prev 指针跳过整段
  只有确认不重复的节点才能被 prev 连接
```

### Bug 5：递归版本忘记跳过所有重复值

```cpp
// 错误：只跳过一个重复节点
if (head->val == head->next->val) {
    return deleteDuplicates(head->next);  // 只跳了一个！
}
// 正确：跳过所有值为 val 的节点
int val = head->val;
while (head && head->val == val) head = head->next;
return deleteDuplicates(head);
```

---

## 九、面试追问（递进链）

### Q1：LC 82 vs LC 83 的区别？

| | LC 82 | LC 83 |
|---|-------|-------|
| 题目 | 删除所有重复的节点 | 保留一个重复的节点 |
| [1,1,2] | [2] | [1,2] |
| 需要 dummy | 是 | 否 |
| 复杂度 | O(n)/O(1) | O(n)/O(1) |

### Q2：如果链表无序，如何去重（删除所有出现 > 1 次的元素）？

```cpp
// 第一遍统计频次，第二遍删除
unordered_map<int, int> freq;
ListNode* curr = head;
while (curr) { freq[curr->val]++; curr = curr->next; }

ListNode dummy(0, head);
ListNode* prev = &dummy;
curr = head;
while (curr) {
    if (freq[curr->val] > 1) prev->next = curr->next;
    else prev = curr;
    curr = curr->next;
}
return dummy.next;
```

### Q3：能否原地完成，不使用额外空间？

对于有序链表：本题的迭代解法就是 O(1) 空间。
对于无序链表：不用额外空间需要 O(n^2) 时间（对每个节点检查是否有重复）。

### Q4：如果要保留出现恰好 k 次的元素？

需要先遍历统计频次，然后第二遍删除出现次数不等于 k 的节点。

### Q5：如何用单次遍历解决？能否避免回头检查？

当前解法已经是单次遍历 O(n)。prev 指针保证了我们不需要回头：
- prev 始终指向最后一个确认不重复的节点
- 遇到重复段时 prev->next 直接跳到段尾

### Q6：递归解法的栈深度是多少？最坏情况？

最坏情况：所有元素都不重复，递归深度为 n（如 [1,2,3,...,n]）。
可能导致栈溢出。面试中应提到这个问题，推荐迭代解法。

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 83 | Remove Duplicates from Sorted List | 保留一个重复元素 |
| 203 | Remove Linked List Elements | 删除指定值的节点 |
| 206 | Reverse Linked List | 链表基础操作 |
| 19 | Remove Nth Node From End | dummy 节点技巧 |
| 237 | Delete Node in a Linked List | 链表删除变体 |
| 26 | Remove Duplicates from Sorted Array | 数组版去重（保留一个） |
| 80 | Remove Duplicates from Sorted Array II | 数组版去重（保留两个） |

---

## 十一、总结

```
dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
  ↑
 prev
 
关键决策树：
  
  prev->next 存在?
     |
     是 → curr = prev->next
           |
           curr->next 存在且 curr->val == curr->next->val?
           |                    |
           是(重复)             否(不重复)
           |                    |
     记录val,跳过整段        prev前进
     prev->next = 跳后位置   prev = prev->next
     (prev不前进!)
```

**记忆口诀**：
- 哨兵开路保头节点
- prev 锚定不重复处
- 重复整段一起跳
- 跳完 prev 不能动（还要检查新节点是否重复）
