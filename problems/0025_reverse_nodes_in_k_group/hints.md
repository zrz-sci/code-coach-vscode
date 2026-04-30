# 25. K 个一组翻转链表

## 核心思路

本题本质是**分段处理链表**：先数够 k 个节点确认一组，翻转这一组，然后把翻转后的段正确地接回链表。难点在于翻转后首尾指针发生了互换，如何正确地维护段与段之间的连接。

## 思维链

1. **读完题第一反应**：这不就是"翻转链表"的加强版吗？普通翻转链表我会做（206题），这题只是每 k 个翻转一次。
2. **拆解子问题**：每一组的翻转本身就是 206 题的子问题（翻转一段链表）。主要的新挑战是：翻转后这段的头变成尾、尾变成头，怎么和前后段连接？
3. **关键观察**：翻转前，这段的第一个节点是 `groupHead`，最后一个是 `groupTail`；翻转后，`groupTail` 成了新头，`groupHead` 成了新尾。新尾需要连接下一段的结果。
4. **两种思路**：
   - **递归**：先翻转第一组，把 `groupHead`（现在是新尾）接上对剩余链表递归的结果。代码最简洁。
   - **迭代**：用 dummy 节点和 `prevGroupEnd` 指针，逐段翻转并串联。满足 O(1) 空间的 Follow-up。
5. **不足 k 个怎么办？** 先数一下剩余节点够不够 k 个，不够就保持原样直接返回。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归法 | 翻转当前 k 个，递归处理剩余 | O(n) | O(n/k) 栈空间 | 能说出即可 |
| 迭代法 | dummy + 逐段翻转 + 指针串联 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **前置技能**：你必须先会 206. 翻转链表。把"翻转从 a 到 b 的一段链表"作为一个工具函数。

2. **先数再翻**：每次翻转前，先用一个指针走 k 步看看够不够。不够就不翻转。

3. **指针连接是关键** — 画图理解翻转前后的连接关系：
```
翻转前 (k=3):
prevGroupEnd → [1] → [2] → [3] → [4] → [5]
                ↑groupHead    ↑groupTail  ↑nextGroupHead

翻转后:
prevGroupEnd → [3] → [2] → [1] → [4] → [5]
                ↑新头(groupTail)  ↑新尾(groupHead)
连接: prevGroupEnd.next = groupTail (新头)
      groupHead.next = nextGroupHead (下一段)
更新: prevGroupEnd = groupHead (新尾成为下一段的前驱)
```

4. **dummy 节点**：头节点所在的组也会被翻转，head 会变。用 dummy 节点避免特殊处理。

5. **递归思路**：翻转当前 k 个 → 递归处理剩余 → 把当前段的尾接到递归结果上。

## 解法详解

### 解法1: 递归法 — O(n) / O(n/k)

**思考过程**：链表天然适合递归。每次处理 k 个节点：先检查够不够 k 个，不够直接返回 head；够的话翻转这 k 个，然后把翻转后的尾节点（原来的 head）接上对剩余部分递归的结果。

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
    ListNode* reverseKGroup(ListNode* head, int k) {
        // Step 1: 检查剩余节点是否够 k 个
        ListNode* check = head;
        for (int i = 0; i < k; i++) {
            if (!check) return head;  // 不足 k 个，保持原样
            check = check->next;
        }
        // 此时 check 指向第 k+1 个节点（下一组的起点）
        
        // Step 2: 翻转从 head 开始的 k 个节点
        // 标准翻转链表，翻转 k 次
        ListNode* prev = nullptr;
        ListNode* curr = head;
        for (int i = 0; i < k; i++) {
            ListNode* nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }
        // 翻转后: prev 是新头，head 是新尾，curr == check 是下一组起点
        
        // Step 3: 原来的 head 现在是新尾，接上递归处理后续的结果
        head->next = reverseKGroup(curr, k);
        
        // prev 是翻转后这段的新头
        return prev;
    }
};
```

**关键点**：
- 翻转 k 次后，`prev` 是新头，原来的 `head` 变成新尾。
- `curr` 恰好指向下一组的起点，传给递归。
- 递归的 base case 是节点不足 k 个。

### 解法2: 迭代法 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**：递归的栈空间是 O(n/k)，面试 Follow-up 要求 O(1) 空间。改用迭代：用 dummy 节点和 `prevGroupEnd` 指针，逐段翻转并手动串联。

```
// 完整的迭代过程 (k=3, 链表 1→2→3→4→5):
//
// 初始:
// dummy → 1 → 2 → 3 → 4 → 5 → NULL
//   ↑prevGroupEnd
//
// === 第1轮: 翻转 [1,2,3] ===
// 数 k 步: check 走到 4，够 k 个
// 翻转前: groupHead=1, groupTail(走k步找到)=3, nextGroupHead=4
// 翻转: 1←2←3   4→5→NULL
//       prev=3, curr=4
// 连接: prevGroupEnd(dummy).next = 3(新头)
//        1(groupHead,新尾).next = 4(nextGroupHead)
// 结果: dummy → 3 → 2 → 1 → 4 → 5 → NULL
// 更新: prevGroupEnd = 1
//
// === 第2轮: 翻转 [4,5] ===
// 数 k 步: check 从4出发只走到NULL(2步<3)，不够 k 个 → 结束
//
// 最终: dummy → 3 → 2 → 1 → 4 → 5 → NULL
//       返回 dummy.next = 3
```

```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // dummy 节点处理头部翻转的情况
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prevGroupEnd = &dummy;
        
        while (true) {
            // Step 1: 检查从 prevGroupEnd 之后是否有 k 个节点
            ListNode* check = prevGroupEnd;
            for (int i = 0; i < k; i++) {
                check = check->next;
                if (!check) return dummy.next;  // 不足 k 个，结束
            }
            // check 现在指向当前组的最后一个节点（groupTail）
            
            // Step 2: 记录关键指针
            ListNode* groupHead = prevGroupEnd->next;    // 翻转前的头（翻转后的尾）
            ListNode* nextGroupHead = check->next;       // 下一组的起点
            
            // Step 3: 翻转从 groupHead 开始的 k 个节点
            ListNode* prev = nullptr;
            ListNode* curr = groupHead;
            for (int i = 0; i < k; i++) {
                ListNode* nxt = curr->next;
                curr->next = prev;
                prev = curr;
                curr = nxt;
            }
            // prev 是翻转后的新头（即原来的 groupTail/check）
            
            // Step 4: 把翻转后的段接回链表
            prevGroupEnd->next = prev;              // 前一段的尾 → 新头
            groupHead->next = nextGroupHead;        // 新尾 → 下一段的头
            
            // Step 5: 更新 prevGroupEnd 为当前组翻转后的尾（原来的 groupHead）
            prevGroupEnd = groupHead;
        }
    }
};
```

**关键点**：
- `prevGroupEnd` 始终是"上一组翻转后的最后一个节点"，也是当前组前面的连接点。
- 翻转后 `prev`（新头）和 `groupHead`（新尾）的角色互换了，连接时不要搞反。
- 用 `check` 走 k 步既能判断够不够，又能定位 groupTail。

## 解法对比

| | 递归法 | 迭代法 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(n/k) 递归栈 | O(1) ✅ |
| 代码量 | 较短，逻辑清晰 | 较长，需要仔细维护指针 |
| 面试 | 可以先写递归展示思路 | Follow-up 必须写出 |
| 适用 | 理解和表达思路 | 满足 O(1) 空间要求 |

**建议**：面试时先口述递归思路表明理解，然后直接写迭代版本。

## 易错点

1. **检查节点数量时走错位置**：
   - ✗ 从 `head` 开始检查，但实际第二轮时 head 已经不是当前组的起点了
   - ✓ 始终从 `prevGroupEnd` 后面开始检查

2. **翻转后连接搞反**：
   - ✗ `prevGroupEnd->next = groupHead`（接到了新尾上）
   - ✓ `prevGroupEnd->next = prev`（prev 才是翻转后的新头）

3. **忘记更新 prevGroupEnd**：
   - ✗ 翻转第一组后 prevGroupEnd 还是 dummy，第二组连接错误
   - ✓ 每轮结束 `prevGroupEnd = groupHead`（翻转后 groupHead 变成了尾）

4. **翻转时循环次数**：
   - ✗ `for (int i = 0; i < k-1; i++)` 少翻转一次
   - ✓ `for (int i = 0; i < k; i++)` 翻转 k 次，每次改变一个 next 指针

5. **递归版忘了检查不足 k 个的 base case**：
   - ✗ 不检查直接翻转，最后不满 k 个的也被翻转了
   - ✓ 先走 k 步检查，不够直接 `return head`

## 面试追问

**Q1（基础理解）**：翻转链表（206题）你会做吗？时间空间复杂度？
> 用三指针 prev/curr/nxt 逐个翻转，O(n) 时间 O(1) 空间。本题的"翻转 k 个节点"就是这个子问题。

**Q2（本题核心）**：翻转后怎么和前后段正确连接？
> 翻转前记录 groupHead 和 nextGroupHead。翻转后 groupHead 变成尾，prev 变成头。连接：`prevGroupEnd->next = prev`，`groupHead->next = nextGroupHead`。

**Q3（Follow-up O(1) 空间）**：递归解法空间不是 O(1)，怎么改？
> 改成迭代：用 while 循环代替递归，用 prevGroupEnd 指针手动管理段间连接，空间降为 O(1)。

**Q4（变体）**：如果要求不足 k 个的最后一段也翻转呢？
> 去掉检查够不够 k 个的逻辑，对最后一段也执行翻转即可。或者在循环中把翻转次数改为 `min(k, remaining)`。

**Q5（变体）**：如果 k 个一组，奇数组翻转、偶数组不翻转呢？
> 加一个 groupIndex 计数器，翻转前判断 `groupIndex % 2 == 0` 才翻转，否则直接移动 prevGroupEnd 跳过这 k 个。

## 相关题型

- **206. 翻转链表** — 本题的核心子问题，必须先掌握三指针翻转技巧
- **24. 两两交换链表节点** — 本题 k=2 的特例，可以用完全相同的模板，只需把 k 设为 2
- **92. 反转链表 II** — 翻转链表的指定区间 [left, right]，复用"翻转一段 + 前后连接"的技巧，区别是只翻转一次且位置由参数指定
- **143. 重排链表** — 同样需要"找中点 + 翻转后半段 + 合并"的链表操作组合，练习链表多步操作的串联