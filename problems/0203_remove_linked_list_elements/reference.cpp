/*
 * LeetCode 203: 移除链表元素 (Remove Linked List Elements)
 *
 * 【题目本质】
 * 遍历链表，删除所有值等于 val 的节点。
 * 核心难点不在算法，而在于头节点可能被删、指针操作不能出错。
 *
 * 【解法总览】
 * 解法1: 迭代（不用 dummy）  — O(n) / O(1) — 分两段处理
 * 解法2: 迭代 + 虚拟头节点   — O(n) / O(1) — 面试首选 ⭐
 * 解法3: 递归               — O(n) / O(n) — 代码最简洁
 */

// Definition for singly-linked list.
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode() : val(0), next(nullptr) {}
//     ListNode(int x) : val(x), next(nullptr) {}
//     ListNode(int x, ListNode *next) : val(x), next(next) {}
// };

// ============================================================
// 解法1: 迭代（不用 dummy）— 分两段处理头部和其余
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 最直觉的做法：
// 第一步：头节点可能是 val，先用 while 跳过所有头部的 val 节点
// 第二步：遍历剩余链表，用 curr 指针检查 curr->next，等于 val 就删
//
// 为什么分两步？因为删除头节点 vs 删除中间节点的操作不同：
// - 删头节点：直接 head = head->next
// - 删中间节点：prev->next = curr->next
//
// 示意 (val=6):
//   输入:  1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//                  ↑删             ↑删
//   输出:  1 → 2 → 3 → 4 → 5 → NULL
//
// 特殊情况 (val=7):
//   输入:  7 → 7 → 7 → 7 → NULL
//          ↑   ↑   ↑   ↑ 全部删除
//   输出:  NULL
// ============================================================
class Solution1 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 第一步：跳过头部所有等于 val 的节点
        // 为什么用 while 不用 if？因为可能连续多个头节点都是 val
        while (head != nullptr && head->val == val) {
            ListNode* toDelete = head;
            head = head->next;
            delete toDelete;
        }
        
        // 此时 head 要么为 nullptr（全删了），要么 head->val != val
        ListNode* curr = head;
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->next->val == val) {
                // 删除 curr->next: 让 curr 跳过它
                ListNode* toDelete = curr->next;
                curr->next = curr->next->next;
                delete toDelete;
                // 关键：curr 不移动！新的 curr->next 可能也是 val
            } else {
                // 当前 curr->next 保留，curr 前进
                curr = curr->next;
            }
        }
        return head;
    }
};

// ============================================================
// 解法2: 迭代 + 虚拟头节点 (Dummy Head) — 面试首选 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1 的麻烦在于：删头节点和删中间节点的代码不同，要分两段。
// 优化思路：加一个 dummy 节点 → dummy->next = head
// 这样原始头节点也变成了"中间节点"（dummy 是它的前驱），
// 所有节点的删除逻辑完全统一：prev->next = prev->next->next
//
// 示意 (val=6):
//
//   dummy → 1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//    ↑prev
//
//   Step1: prev->next=1, 1≠6, prev前进
//   dummy → 1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//            ↑prev
//
//   Step2: prev->next=2, 2≠6, prev前进
//   dummy → 1 → 2 → 6 → 3 → 4 → 5 → 6 → NULL
//                 ↑prev
//
//   Step3: prev->next=6, 6==6! 删除: prev->next = 6->next = 3
//   dummy → 1 → 2 ──→ 3 → 4 → 5 → 6 → NULL
//                 ↑prev  (prev不动!)
//
//   Step4: prev->next=3, 3≠6, prev前进
//   ... 以此类推 ...
//
//   最终: dummy → 1 → 2 → 3 → 4 → 5 → NULL
//   返回 dummy->next = 1
//
// 全部删除的情况 (val=7):
//   dummy → 7 → 7 → 7 → NULL
//    ↑prev
//   连续删除3次, dummy->next = NULL
//   返回 NULL ✓
// ============================================================
class Solution2 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 创建虚拟头节点，next 指向 head
        ListNode* dummy = new ListNode(0, head);
        ListNode* prev = dummy;
        
        // 统一逻辑：始终检查 prev->next
        while (prev->next != nullptr) {
            if (prev->next->val == val) {
                // 删除 prev->next
                ListNode* toDelete = prev->next;
                prev->next = prev->next->next;
                delete toDelete;
                // prev 不移动 — 因为新的 prev->next 可能也需要删除
            } else {
                // 保留，prev 前进
                prev = prev->next;
            }
        }
        
        // 从 dummy->next 取出真正的头节点
        ListNode* newHead = dummy->next;
        delete dummy; // 释放虚拟节点的内存
        return newHead;
    }
};

// ============================================================
// 解法3: 递归 — 链表天然的递归结构
// 时间: O(n)  空间: O(n) — 递归栈深度为链表长度
//
// 【思路】
// 链表是递归定义的: 一个节点 + 剩余链表。
// 递归思考方式:
//   "假设 removeElements(head->next, val) 已经帮你把后面全处理好了，
//    你只需要决定：当前 head 留不留？"
//
// 递归三要素:
//   返回值: 处理好的子链表的头节点
//   终止条件: head == nullptr, 返回 nullptr
//   单层逻辑: 
//     1) 递归处理 head->next
//     2) 如果 head->val == val, 跳过自己, 返回 head->next
//        否则, 保留自己, 返回 head
//
// 递归展开过程 (head = [1,2,6,3], val = 6):
//
//   remove(1→2→6→3→N, 6)
//   │ 1->next = remove(2→6→3→N, 6)
//   │ │ 2->next = remove(6→3→N, 6)
//   │ │ │ 6->next = remove(3→N, 6)
//   │ │ │ │ 3->next = remove(N, 6)
//   │ │ │ │ │ return NULL              ← base case
//   │ │ │ │ 3->next = NULL
//   │ │ │ │ 3≠6, return 3→NULL
//   │ │ │ 6->next = 3→NULL
//   │ │ │ 6==6, return 3→NULL          ← 跳过6!
//   │ │ 2->next = 3→NULL
//   │ │ 2≠6, return 2→3→NULL
//   │ 1->next = 2→3→NULL
//   │ 1≠6, return 1→2→3→NULL           ← 最终结果
// ============================================================
class Solution3 {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // 终止条件：空链表无需处理
        if (head == nullptr) return nullptr;
        
        // 递归：先把后面的链表处理好
        head->next = removeElements(head->next, val);
        
        // 决定当前节点的去留
        // 如果当前值等于 val，跳过自己（返回已处理好的下一个节点）
        // 否则保留自己
        if (head->val == val) {
            ListNode* next = head->next;
            delete head; // 释放被跳过的节点
            return next;
        }
        return head;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1: 不用dummy | 解法2: dummy ⭐ | 解法3: 递归   |
// |-----------|-----------------|----------------|-------------|
// | 时间复杂度 | O(n)            | O(n)           | O(n)        |
// | 空间复杂度 | O(1)            | O(1)           | O(n) 栈空间  |
// | 代码简洁度 | 差 (分两段)      | 好 (统一逻辑)    | 最好 (3行核心)|
// | 面试推荐   | 不推荐           | ⭐首选          | 追问时展示    |
//
// 核心区别:
// - 解法1 vs 解法2: dummy 节点消除了头节点的特殊性
// - 解法2 vs 解法3: 迭代 O(1) 空间 vs 递归 O(n) 空间
//   面试中先写解法2，被问"能递归吗？"再给解法3
//
// ============================================================
// 【易错点】
//
// 1. 删除节点后移动了 prev:
//    ✗ prev->next = prev->next->next; prev = prev->next;
//    ✓ prev->next = prev->next->next; (prev 不动!)
//    原因: 新的 prev->next 可能也等于 val，需要继续检查
//
// 2. 返回 head 而不是 dummy->next:
//    ✗ return head;   // head 可能已经被删了!
//    ✓ return dummy->next;
//
// 3. 解法1 中头部处理用 if 而不是 while:
//    ✗ if (head && head->val == val) head = head->next;
//       → 只跳过一个, [7,7,7] 会漏删
//    ✓ while (head && head->val == val) head = head->next;
//
// 4. 递归终止条件写错:
//    ✗ if (head->next == nullptr) ...  // head 本身可能是 nullptr!
//    ✓ if (head == nullptr) return nullptr;
//
// 5. 忘记释放内存:
//    LeetCode 不检查, 但面试中提到 delete 被删节点 + delete dummy
//    展示工程素养, 是加分项
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1 (基础): 为什么要用 dummy 节点？不用行不行？
//    → 不用也行(解法1), 但删头节点需要单独处理。
//      dummy 让所有删除操作统一为 prev->next = prev->next->next
//
// Q2 (递归 vs 迭代): 递归解法有什么缺点？什么时候不能用？
//    → 栈空间 O(n)。链表极长时(10^6+)可能栈溢出。
//      面试中提到这点说明你理解递归的本质代价。
//
// Q3 (变体): 如果不是删除等于 val 的, 而是只保留第一次出现的(去重)?
//    → 类似 LeetCode 83。用 set 记录已出现的值,
//      框架完全相同, 只改判断条件。
//
// Q4 (工程素养): 你的代码有没有内存泄漏?
//    → 没有 delete 就有泄漏。应该 delete 被删节点和 dummy。
//      实际项目中通常用智能指针(unique_ptr)管理。
//
// ============================================================
// 【相关题型】
//
// - LeetCode 83: 删除排序链表中的重复元素
//   → 复用 dummy + prev 框架, 判断条件改为 curr->val == curr->next->val
//
// - LeetCode 82: 删除排序链表中的重复元素 II
//   → 同样的 dummy 框架, 但要把所有重复节点全删(不留一个), 需要计数
//
// - LeetCode 237: 删除链表中的节点
//   → 只给要删的节点(无 prev), 用"复制下一个值+删下一个节点"的技巧
//
// - LeetCode 206: 反转链表
//   → 同为链表基础操作, 复用迭代(指针翻转)/递归双解法的思维模式
// ============================================================
