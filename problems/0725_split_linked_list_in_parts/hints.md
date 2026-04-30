# 725. Split Linked List in Parts

## 核心思路

先遍历链表获取总长度 n，然后计算每部分的基础长度 partSize = n/k 和多余节点数 extra = n%k。前 extra 个部分长度为 partSize+1，其余为 partSize。最后遍历链表，按计算好的长度逐段切割。

---

## 思维链

1. **理解分割规则**：分成 k 段，每段长度尽量相等，任意两段长度差最多为 1，且前面的段不短于后面的段。
2. **数学计算**：n 个节点分 k 段，每段至少 n/k 个。多出来的 n%k 个节点，分配给前 n%k 段各多一个。
3. **特殊情况**：如果 n < k，前 n 段各一个节点，后 k-n 段为 nullptr。此时 partSize=0, extra=n。
4. **切割操作**：找到每段最后一个节点，保存 next 指针，然后断开。
5. **实现细节**：需要先走 partSize + (i < extra ? 1 : 0) - 1 步到达当前段的尾部，然后执行断开操作。

---

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| 计算长度 + 逐段切割 | 先求 n，再算每段长度，逐段断开 | O(n + k) | O(k) | 首选 |
| 转数组再分割 | 链表转数组，分割后重建 | O(n + k) | O(n) | 浪费空间 |

---

## 关键提示

### 提示 1 — 每段长度怎么算？

如何将 n 个节点"尽量均匀"地分给 k 段？

<details>
<summary>展开</summary>

- 基础长度：partSize = n / k（每段至少这么多）
- 多余节点：extra = n % k
- 分配规则：前 extra 段长度为 partSize + 1，其余段长度为 partSize

例如 n=10, k=3：partSize=3, extra=1。段长度为 [4, 3, 3]。
例如 n=7, k=3：partSize=2, extra=1。段长度为 [3, 2, 2]。
例如 n=2, k=5：partSize=0, extra=2。段长度为 [1, 1, 0, 0, 0]。

</details>

### 提示 2 — 怎么断开链表？

<details>
<summary>展开</summary>

对于每一段：
1. 记录该段头节点 head（即当前 curr 指针）
2. 向前走 curLen - 1 步到达该段尾节点
3. 保存 tail->next 为 nextPart
4. 设 tail->next = nullptr（断开）
5. 将 curr 移到 nextPart

**关键**：走 curLen - 1 步而非 curLen 步。因为 curr 已经在当前段的第一个节点上，再走 curLen-1 步就到了最后一个节点。

</details>

### 提示 3 — n < k 时怎么办？

<details>
<summary>展开</summary>

当 n < k 时，partSize = 0, extra = n。前 n 段每段长度为 0+1=1（各一个节点），后 k-n 段长度为 0（nullptr）。

代码中循环条件 `for (int i = 0; i < k && curr; i++)` 中的 `curr != nullptr` 确保了当节点用完后自动停止。result 初始化为全 nullptr，所以后面的段自动为空。

</details>

### 提示 4 — 循环中的条件判断

<details>
<summary>展开</summary>

```cpp
for (int i = 0; i < k && curr; i++) {
    result[i] = curr;
    int curLen = partSize + (i < extra ? 1 : 0);
    for (int j = 1; j < curLen; j++) {  // 注意从 1 开始
        curr = curr->next;
    }
    ListNode* nextPart = curr->next;
    curr->next = nullptr;
    curr = nextPart;
}
```

- `i < extra` 判断是否需要多分一个节点
- `j` 从 1 开始到 curLen-1，走 curLen-1 步
- 当 curLen == 0 时（n < k 且 i >= extra），内层循环不执行。但此时 curr 已经是 nullptr，外层循环的 `curr` 条件会终止

</details>

---

## 解法详解

### 解法一：计算长度 + 逐段切割（最优）

**算法步骤**：

1. 遍历链表计算总长度 n
2. 计算 partSize = n / k, extra = n % k
3. 初始化 result 数组大小为 k，全部填 nullptr
4. 用指针 curr 从 head 开始遍历：
   - 对于第 i 段（i = 0 到 k-1）：
     - 若 curr 为空则跳出
     - result[i] = curr（记录段头）
     - 当前段长度 curLen = partSize + (i < extra ? 1 : 0)
     - 向前走 curLen - 1 步到达段尾
     - 保存 nextPart = curr->next
     - 断开 curr->next = nullptr
     - curr = nextPart
5. 返回 result

**时间复杂度**：O(n + k)，遍历链表 O(n) + 初始化结果数组 O(k)
**空间复杂度**：O(k)，结果数组（不算输出空间则 O(1)）

### 解法二：链表转数组再分割

先将链表所有值存入数组，然后按段长度分割数组，每段重建链表。代码更简单但浪费 O(n) 空间，且"重建链表"增加了不必要的操作。面试中不推荐。

---

## 易错点

1. **curLen 为 0 时的处理**：当 n < k 且 i >= n 时，curLen=0+0=0。此时不应走步也不应断开（curr 已经是 nullptr）。通过外层循环的 `curr != nullptr` 条件自然规避
2. **走步数搞错**：走 curLen-1 步到达段尾，不是 curLen 步。如果走 curLen 步会走到下一段的第一个节点
3. **忘记断开**：必须设 curr->next = nullptr。不断开会导致前一段的尾部仍指向后一段的头部
4. **result 未初始化为 nullptr**：如果 n < k，后面的段必须是 nullptr。使用 `vector<ListNode*>(k, nullptr)` 初始化
5. **extra 节点分配给后面的段**：题目要求前面的段不短于后面的段，所以 extra 个多余节点分配给前 extra 段。如果分配给后面的段就违反了约束
6. **空链表**：head 为 nullptr 时，n=0，所有段都是 nullptr，直接返回

---

## 面试追问

**Q1: 能否不预先计算链表长度？**

很难。不知道 n 就无法计算 partSize 和 extra。可以"两遍遍历"（第一遍计数，第二遍切割），但无法避免至少一遍完整遍历来获取长度。一种替代方案是用快慢指针找中点然后递归分割，但这比直接计算长度更复杂。

**Q2: 如果 k 很大（远大于 n），时间复杂度如何？**

O(n + k)。遍历链表 O(n)，初始化结果数组 O(k)。当 k >> n 时，主要开销在初始化 k 个 nullptr。空间 O(k) 也可能成为瓶颈。

**Q3: 如果要求"后面的段不短于前面的段"怎么办？**

只需将 extra 的分配反过来：后 extra 段长度为 partSize+1，前面的段长度为 partSize。判断条件改为 `i >= k - extra ? 1 : 0`。

**Q4: 如何验证分割结果的正确性？**

验证三个条件：(1) 所有段长度之和 == n (2) 任意两段长度差 <= 1 (3) 段长度从左到右单调不增。可以在面试中主动写验证函数展示严谨性。

**Q5: 如果链表有环怎么办？**

先用快慢指针检测环并找到入口。然后在环入口处断开，使其变成普通链表，再执行分割算法。或者直接报错——题目保证无环。

---

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 206 | Reverse Linked List | 链表基础操作 |
| 86 | Partition List | 链表分割 |
| 61 | Rotate List | 链表长度计算 + 断开重连 |
| 328 | Odd Even Linked List | 链表节点分组 |
| 143 | Reorder List | 链表拆分 + 合并 |
| 148 | Sort List | 链表分割 + 归并排序 |
