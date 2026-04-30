# 708. 循环有序列表的插入

## 核心思路

本质上是在一个**循环有序链表**中找到正确的插入位置。难点不在于插入操作本身，而在于正确处理三种不同的插入场景：中间插入、最大值之后插入（转折点）、以及所有值相同时的兜底插入。

## 思维链

1. **读完题第一反应**：循环链表 + 有序 → 找到第一个 `curr.val <= insertVal <= curr.next.val` 的位置插入就行了。
2. **等等，还有特殊情况**：如果 insertVal 比所有节点都大或都小呢？比如链表 `3→4→1`，要插入 `5` 或 `0`。这时候没有一个位置满足 `curr <= insertVal <= next`。
3. **观察转折点**：循环有序链表存在一个"转折点"，即从最大值跳到最小值的地方（如 `4→1`）。如果 insertVal 大于最大值或小于最小值，都应该插在这个转折点之后。
4. **还有一种兜底情况**：如果所有节点值相同（如 `3→3→3`），永远找不到转折点，遍历一圈回到起点就直接插入。
5. **总结三种情况**：(a) 正常中间位置 (b) 转折点位置 (c) 遍历一圈都没找到 → 任意位置插入。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 一次遍历（分情况讨论） | 遍历链表，分三种情况判断插入位置 | O(n) | O(1) | ⭐ 必须写出 |
| 找最大节点后断开 | 先找到转折点确定逻辑起点，再线性查找 | O(n) | O(1) | 能说出即可 |

## 关键提示

- **提示1**：画出循环链表，标注每个节点的值。想想 insertVal 有哪几种可能的大小关系？
- **提示2**：转折点 `curr > curr.next` 是关键 —— 在循环有序链表中只有最大值到最小值的衔接处会出现这种情况。
- **提示3**：不要忘记"所有值相同"的情况，此时不存在转折点，遍历一圈回到起点就该停了。
- **提示4**：画出指针变化过程，搞清楚循环终止条件 —— 用 `do...while` 或者手动检查是否回到起点。

```
循环有序链表示意图 (head 指向 3):

    ┌──→ 3 ──→ 4 ──→ 1 ──┐
    └─────────────────────┘
              ↑
         转折点: 4 → 1 (最大值 → 最小值)

三种插入场景:
  Case A: insertVal=2  → 插在 1 和 3 之间 (正常中间位置)
  Case B: insertVal=5  → 插在 4 和 1 之间 (大于最大值，插在转折点)
  Case C: insertVal=0  → 插在 4 和 1 之间 (小于最小值，也在转折点)
```

## 解法详解

### 解法1: 一次遍历分情况讨论 — O(n) / O(1) ⭐ 面试首选

**思考过程**：

遍历链表，对于每一对相邻节点 `(curr, next)`，检查 insertVal 是否应该插在它们之间。三种情况满足任一即可插入：

1. **正常位置**：`curr.val <= insertVal <= next.val` — 正好夹在中间。
2. **转折点且 insertVal 最大**：`curr.val > next.val`（到了转折点）且 `insertVal >= curr.val` — 比最大值还大。
3. **转折点且 insertVal 最小**：`curr.val > next.val`（到了转折点）且 `insertVal <= next.val` — 比最小值还小。

如果遍历一圈回到起点都没找到，说明所有值相同，随便插入。

```cpp
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node() {}
    Node(int _val) { val = _val; next = NULL; }
    Node(int _val, Node* _next) { val = _val; next = _next; }
};
*/

class Solution {
public:
    Node* insert(Node* head, int insertVal) {
        Node* newNode = new Node(insertVal);
        
        // Case 0: 空链表 → 自己指向自己
        if (!head) {
            newNode->next = newNode;
            return newNode;
        }
        
        Node* curr = head;
        
        // do...while 保证至少遍历一次，且能检测回到起点
        do {
            Node* next = curr->next;
            
            // Case A: 正常中间位置 — insertVal 夹在 curr 和 next 之间
            if (curr->val <= insertVal && insertVal <= next->val) {
                break;
            }
            
            // Case B/C: 到达转折点（最大→最小），insertVal 是全局最大或最小
            if (curr->val > next->val) {
                if (insertVal >= curr->val || insertVal <= next->val) {
                    break;
                }
            }
            
            curr = next;
        } while (curr != head);
        // 如果遍历一圈都没 break → 所有值相同，curr 停在 head，直接插在 head 后面
        
        // 在 curr 和 curr->next 之间插入
        newNode->next = curr->next;
        curr->next = newNode;
        
        return head;
    }
};
```

**关键点**：
- `do...while` 而非 `while` —— 因为起始时 `curr == head`，如果用 `while(curr != head)` 会直接退出。
- 遍历一圈没找到时不需要特殊处理，因为 `curr` 停回 `head`，直接在 `head` 后面插入即可。

### 解法2: 找最大节点后线性插入 — O(n) / O(1)

**思考过程**：

换一种思路 —— 先找到转折点（最大值节点），就能确定链表的"逻辑起点"（最小值节点）。然后从逻辑起点开始线性扫描找插入位置。如果扫完一圈没找到，就插在末尾（转折点之后）。

```cpp
class Solution {
public:
    Node* insert(Node* head, int insertVal) {
        Node* newNode = new Node(insertVal);
        
        if (!head) {
            newNode->next = newNode;
            return newNode;
        }
        
        // Step 1: 找到最大值节点（转折点前一个）
        Node* maxNode = head;
        Node* curr = head->next;
        while (curr != head) {
            // >= 保证找到最后一个最大值节点（处理重复元素）
            if (curr->val >= maxNode->val) {
                maxNode = curr;
            }
            curr = curr->next;
        }
        
        // Step 2: minNode 是逻辑起点
        Node* minNode = maxNode->next;
        
        // Step 3: 如果 insertVal 大于最大值或小于最小值 → 插在转折点
        if (insertVal >= maxNode->val || insertVal <= minNode->val) {
            newNode->next = minNode;
            maxNode->next = newNode;
        } else {
            // Step 4: 从 minNode 开始找第一个 curr.next.val >= insertVal 的位置
            curr = minNode;
            while (curr->next->val < insertVal) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
        
        return head;
    }
};
```

**关键点**：
- 找最大节点时用 `>=` 而不是 `>`，确保在有重复最大值时定位到最后一个，这样 `maxNode->next` 才是真正的最小值起点。

## 解法对比

| | 解法1: 一次遍历 | 解法2: 找最大节点 |
|---|---|---|
| 遍历次数 | 最多一次 | 最多两次（找max + 找位置） |
| 代码简洁度 | 更简洁 | 更直观但代码更长 |
| 思维模式 | 边遍历边判断三种情况 | 先确定逻辑结构再查找 |
| 面试推荐 | ⭐ 首选 | 如果解法1想不清楚可以退而求其次 |

两种解法时间复杂度都是 O(n)、空间 O(1)，面试中解法1更受青睐，因为它一次遍历搞定。

## 易错点

1. **空链表忘记自指**：
   - ✗ `newNode->next = NULL; return newNode;`
   - ✓ `newNode->next = newNode; return newNode;` — 循环链表，单节点必须指向自己。

2. **用 `while(curr != head)` 导致循环体不执行**：
   - ✗ `curr = head; while (curr != head) { ... }` — 初始就等于 head，直接跳过
   - ✓ 用 `do { ... } while (curr != head);` 保证至少执行一次

3. **转折点判断漏了等号**：
   - ✗ `if (insertVal > curr->val || insertVal < next->val)` — 漏掉等于的情况
   - ✓ `if (insertVal >= curr->val || insertVal <= next->val)` — "非递减"意味着允许相等

4. **只检查了 Case A 忘了 Case B/C**：
   - 如果链表是 `[3,4,1]` 插入 `5`，永远找不到 `curr <= 5 <= next` 的位置
   - 必须处理转折点的情况

5. **遍历一圈后忘了兜底插入**：
   - 链表 `[3,3,3]` 插入 `3`，三种 case 都不满足，但 `do...while` 结束后 `curr` 回到 `head`，直接在后面插入即可 —— 解法1 的结构天然处理了这个情况。

## 面试追问

**Q1: 为什么遍历一圈回到起点就可以直接插入？**
> 如果遍历一圈都没有找到合适位置，说明所有节点值相同（否则一定会在转折点或中间位置匹配到）。值全相同的链表，insertVal 插在任何位置都不破坏有序性。

**Q2: 如果链表不是循环的而是普通有序链表呢？代码怎么改？**
> 普通有序链表更简单：(1) insertVal 小于头节点 → 插在最前面；(2) 线性扫描找到第一个 `curr.val <= insertVal < next.val` 的位置；(3) 如果到末尾还没找到 → 插在最后。不需要处理转折点。

**Q3: 如果频繁插入操作，怎么优化？**
> 链表查找是 O(n)，频繁插入可以考虑用**跳表 (Skip List)** 代替，查找和插入都是 O(log n)。或者用平衡 BST（如 `std::set`）来维护有序集合。

## 相关题型

- **[141. 环形链表](https://leetcode.com/problems/linked-list-cycle/)** — 复用循环链表的遍历技巧（快慢指针检测环 vs 本题遍历一圈回到起点）
- **[147. 对链表进行插入排序](https://leetcode.com/problems/insertion-sort-list/)** — 复用"在有序链表中找插入位置"的核心逻辑，区别：147 是普通链表不是循环的
- **[25. K 个一组翻转链表](https://leetcode.com/problems/reverse-nodes-in-k-group/)** — 链表指针操作的进阶练习，锻炼指针重连的能力