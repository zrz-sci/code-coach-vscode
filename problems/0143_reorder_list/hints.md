# 143. 重排链表

## 核心思路

这道题本质上是：**把链表的后半段反转，然后和前半段交替合并**。目标排列 L0→Ln→L1→Ln-1→... 恰好是"前半段正序 + 后半段逆序"交替穿插的结果。

## 思维链

1. **读完题第一反应**：需要从两端同时往中间取节点，但单链表只能从头往后遍历，无法从尾部往前走。最朴素的做法是把所有节点存到数组里，然后用双指针从两端取节点重新连接。

2. **暴力解的瓶颈**：数组方案可行且好理解，但需要 O(n) 额外空间。面试官一定会追问：能不能 O(1) 空间？

3. **观察目标结构的规律**：以 `1→2→3→4→5` 为例，目标是 `1→5→2→4→3`。拆开看：
   - 前半段：`1→2→3`
   - 后半段（逆序）：`5→4→3`
   - 交替合并：`1→5→2→4→3`

4. **三步法自然浮现**：
   - **找中点**：快慢指针 → 把链表分成前后两半
   - **反转后半段**：经典反转链表
   - **合并两个链表**：交替穿插连接

5. **每一步都是经典链表操作**，组合起来就解决了问题，且空间 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 数组+双指针 | 存入数组，双指针重排 | O(n) | O(n) | 能说出即可 |
| 栈 | 用栈获取后半段逆序 | O(n) | O(n) | 能说出即可 |
| 找中点+反转+合并 | 三步经典操作 | O(n) | O(1) | ⭐ 必须写出 |
| 递归 | 递归到尾部回收节点 | O(n) | O(n) 栈 | 加分项 |

## 关键提示

- **提示1**：如果你能随机访问链表节点（像数组一样），这道题就很简单——用双指针一头一尾交替取。那怎么让链表支持"从尾部取"呢？

- **提示2**：观察目标序列 `L0→Ln→L1→Ln-1→...`，后半段是**逆序**出现的。你知道哪个经典操作能让链表逆序？

- **提示3**：找链表中点用什么技巧？想想快慢指针。

- **提示4**：把三步操作分开实现（找中点、反转、合并），每一步单独写函数，不要混在一起，否则指针关系很容易搞乱。

- **ASCII 示意图**：

```
原始: 1 → 2 → 3 → 4 → 5

Step1 找中点(slow停在3):
      1 → 2 → 3 → 4 → 5
                ↑slow

Step2 断开，反转后半段:
      前半: 1 → 2 → 3
      后半: 4 → 5  →反转→  5 → 4

Step3 交替合并:
      取1, 取5 → 1→5
      取2, 取4 → 1→5→2→4
      取3       → 1→5→2→4→3

结果: 1 → 5 → 2 → 4 → 3
```

## 解法详解

### 解法1: 数组+双指针 — O(n) / O(n)

**思考过程**：单链表不支持随机访问和反向遍历，但数组支持。把所有节点存到数组里，就可以用左右双指针从两端交替取节点，重新连接。

```cpp
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        
        // 把所有节点存入数组，获得随机访问能力
        vector<ListNode*> nodes;
        ListNode* curr = head;
        while (curr) {
            nodes.push_back(curr);
            curr = curr->next;
        }
        
        int left = 0, right = nodes.size() - 1;
        while (left < right) {
            // left节点指向right节点
            nodes[left]->next = nodes[right];
            left++;
            
            // left和right相遇则结束（偶数长度的情况）
            if (left == right) break;
            
            // right节点指向下一个left节点
            nodes[right]->next = nodes[left];
            right--;
        }
        // 最后一个节点的next必须置空，否则形成环！
        nodes[left]->next = nullptr;
    }
};
```

**关键点**：最后 `nodes[left]->next = nullptr` 不能忘，否则链表会有环。

---

### 解法2: 找中点 + 反转后半段 + 合并 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是需要 O(n) 空间存数组。核心需求是"从尾部逆序取节点"，我们可以直接在链表上操作：反转后半段链表，然后和前半段交替合并。

```
// 详细的指针变化过程 (以 1→2→3→4→5 为例):
//
// === Step1: 快慢指针找中点 ===
// 初始:  slow=1, fast=1
// 一步:  slow=2, fast=3
// 两步:  slow=3, fast=5(到尾) → slow停在中点3
//
// === Step2: 断开 + 反转后半段 ===
// 断开: 前半 1→2→3,  后半 4→5
// 反转后半:
//   prev=NULL, curr=4→5
//   → prev=4, curr=5
//   → prev=5→4, curr=NULL  → second = 5→4
//
// === Step3: 交替合并 ===
// first=1→2→3,  second=5→4
//
// Round1: 
//   tmp1=2, tmp2=4
//   1→5, 5→2
//   first=2, second=4
//
// Round2:
//   tmp1=3, tmp2=NULL
//   2→4, 4→3
//   first=3, second=NULL
//
// second为空，结束
// 结果: 1→5→2→4→3  ✓
```

```cpp
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        
        // Step1: 快慢指针找中点
        // 对于偶数长度 1→2→3→4, slow停在2（前半段的最后一个节点）
        // 对于奇数长度 1→2→3→4→5, slow停在3（中间节点归前半段）
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // Step2: 断开链表，反转后半段
        ListNode* second = slow->next;  // 后半段起点
        slow->next = nullptr;           // 断开！前半段到slow为止
        second = reverseList(second);   // 反转后半段
        
        // Step3: 交替合并前半段和反转后的后半段
        ListNode* first = head;
        while (second) {
            // 保存各自的下一个节点
            ListNode* tmp1 = first->next;
            ListNode* tmp2 = second->next;
            // 穿插连接
            first->next = second;
            second->next = tmp1;
            // 移动指针
            first = tmp1;
            second = tmp2;
        }
    }
    
private:
    // 经典反转链表（迭代版）
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
};
```

**关键点**：
1. `slow->next = nullptr` 必须在取出 `second` 之后才断开
2. 快慢指针的终止条件 `fast->next && fast->next->next` 确保 slow 停在前半段的末尾
3. 合并循环以 `second` 为终止条件，因为后半段长度 ≤ 前半段长度

---

### 解法3: 递归 — O(n) / O(n)

**思考过程**：递归天然能到达链表尾部。递归到底后"回收"尾部节点，和从头开始的节点交替配对。用一个外部指针 `front` 从头开始，递归返回时 `back` 从尾部回来，两者相遇时停止。

```cpp
class Solution {
public:
    void reorderList(ListNode* head) {
        front = head;
        reorder(head);
    }
    
private:
    ListNode* front;  // 从前往后走的指针
    
    void reorder(ListNode* back) {
        if (!back) return;
        
        // 递归到链表尾部
        reorder(back->next);
        
        // 递归返回时，front从前往后，back从后往前
        // 当front已经到达或越过back时，说明配对完成
        if (front == back || front->next == back) {
            back->next = nullptr;  // 收尾：最后一个节点next置空
            return;
        }
        
        // 把back插入到front后面
        // front → A → ... → back  变成  front → back → A → ...
        ListNode* nextFront = front->next;
        front->next = back;
        back->next = nextFront;
        
        // front前进到下一个待配对的位置
        front = nextFront;
    }
};
```

**关键点**：
- 终止条件 `front == back`（奇数长度）和 `front->next == back`（偶数长度）对应两种情况
- 这个解法虽然代码简洁，但递归深度 O(n)，不如解法2的 O(1) 空间

---

## 解法对比

| | 解法1: 数组 | 解法2: 三步法 ⭐ | 解法3: 递归 |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) 数组 | O(1) | O(n) 递归栈 |
| 代码难度 | 简单 | 中等 | 中等 |
| 面试推荐 | 作为思路铺垫 | **首选** | 展示递归功底 |
| 核心技能 | 随机访问 | 快慢指针+反转+合并 | 递归回溯 |

**选择建议**：面试中先说解法1展示思路理解，然后写解法2展示链表操作能力。解法2是三个经典链表操作的组合，面试官最喜欢。

## 易错点

1. **忘记断开链表**
   - ✗ 找到中点后直接反转 `slow->next`，但没有 `slow->next = nullptr`
   - ✓ 必须 `slow->next = nullptr` 断开，否则前半段尾部仍连着后半段，合并时会出环

2. **快慢指针终止条件写错**
   - ✗ `while (fast && fast->next)` — 这样 slow 会停在后半段的第一个节点
   - ✓ `while (fast->next && fast->next->next)` — slow 停在前半段的最后一个节点
   - 区别：偶数长度 `1→2→3→4`，前者 slow=3，后者 slow=2。我们需要后者来正确断开

3. **合并时丢失指针**
   - ✗ `first->next = second; second->next = first->next;` — 此时 `first->next` 已经被改了！
   - ✓ 先用 `tmp1 = first->next` 保存，再操作

4. **数组方案忘记置空末尾**
   - ✗ 合并完不设 `nodes[left]->next = nullptr`
   - ✓ 最后一个节点的 next 必须置空，否则可能形成环

## 面试追问

**Q1: 暴力解的思路？空间可以优化吗？**
→ 数组存所有节点，双指针重排，O(n) 空间。优化方向：不用额外存储，在原链表上操作。

**Q2: O(1) 空间怎么做？分几步？每一步的经典方法是什么？**
→ 三步法：(1) 快慢指针找中点 (2) 反转后半段 (3) 交替合并。每一步都是独立的经典链表操作，面试中分别实现更不容易出错。

**Q3: 如果要求重排后变成 L0→Ln→L1→Ln-1→... 但不能修改指针，只能修改值呢？**
→ 用数组存值，双指针从两端取值覆盖回链表即可。这时候数组方案反而最简洁。

**Q4: 如果是双向链表呢？**
→ 双向链表可以直接用头尾双指针，一个从头往后走，一个从尾往前走，交替连接，不需要找中点和反转。时间 O(n)，空间 O(1)。

## 相关题型

- **206. 反转链表** — 解法2的子步骤，必须先掌握。反转链表是本题的基础组件。
- **876. 链表的中间结点** — 解法2的子步骤，快慢指针找中点。注意本题和876的终止条件略有不同（本题需要停在前半段末尾）。
- **21. 合并两个有序链表** — 解法2的合并步骤是类似的双链表交替连接操作，区别在于本题是"交替取"而不是"比较大小取"。
- **234. 回文链表** — 同样使用"找中点+反转后半段"的技巧，反转后比较两半是否相同。