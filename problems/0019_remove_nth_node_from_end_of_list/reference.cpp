/*
 * LeetCode 19: 删除链表的倒数第 N 个结点 (Remove Nth Node From End of List)
 *
 * 【题目本质】
 * 在单向链表中定位"倒数第 N 个节点"并删除。
 * 核心难点：单向链表无法逆向遍历，需要把"倒数"转化为"正数"定位。
 *
 * 【解法总览】
 * 解法1: 两次遍历     — O(L) / O(1) — 最直觉：先数长度，再定位
 * 解法2: 双指针(快慢) — O(L) / O(1) — 一趟扫描，面试首选
 * 解法3: 递归回溯     — O(L) / O(L) — 利用递归"从后往前"特性
 */

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

// ============================================================
// 解法1: 两次遍历 — 先数长度，再正向定位删除
// 时间: O(L)  空间: O(1)
//
// 【思路】
// 最直觉的想法：不知道链表长度就先数出来。
// 倒数第 N 个 = 正数第 (L - N + 1) 个。
// 为了删除，需要找到它的前驱，即正数第 (L - N) 个。
// 用 dummy 节点统一处理删除头节点的边界情况。
//
// 示例: head = [1,2,3,4,5], n = 2
//
//  dummy → 1 → 2 → 3 → 4 → 5 → NULL
//
//  第一遍: 数长度 L = 5
//  倒数第2个 = 正数第4个 = 节点4
//  需要走到前驱(正数第3个 = 节点3): 从 dummy 走 L-N = 3 步
//
//  dummy → 1 → 2 → 3 → 4 → 5 → NULL
//                   ↑curr
//  curr->next = curr->next->next  →  3 → 5
//
//  结果: 1 → 2 → 3 → 5
// ============================================================
class Solution1 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // dummy 节点：当要删的恰好是 head 时，dummy 充当 head 的前驱
        ListNode dummy(0, head);
        
        // 第一次遍历：数出链表总长度
        int length = 0;
        ListNode* curr = head;
        while (curr) {
            length++;
            curr = curr->next;
        }
        
        // 第二次遍历：从 dummy 走 (length - n) 步到达被删节点的前驱
        // 为什么从 dummy 开始？因为当 length == n 时（删头节点），
        // length - n = 0，curr 就是 dummy，dummy->next = head->next 完美处理
        curr = &dummy;
        for (int i = 0; i < length - n; i++) {
            curr = curr->next;
        }
        
        // 删除 curr 的下一个节点
        ListNode* toDelete = curr->next;
        curr->next = toDelete->next;
        delete toDelete;  // 释放内存，面试中提到加分
        
        return dummy.next;
    }
};

// ============================================================
// 解法2: 双指针（快慢指针） — 一趟扫描，面试首选 ⭐
// 时间: O(L)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：需要遍历两次。Follow-up 问能否一趟？
//
// 核心观察：倒数第 N 个节点距离链表末尾（NULL）恰好 N 步。
// 如果两个指针保持固定间距 N，当前面的到达末尾时，
// 后面的就自然指向倒数第 N 个节点。
//
// 但删除需要前驱，所以让间距为 N+1，slow 停在被删节点的前驱。
//
// 具体步骤（以 [1,2,3,4,5], n=2 为例）:
//
//  Step0: fast 和 slow 都在 dummy
//  dummy → 1 → 2 → 3 → 4 → 5 → NULL
//   ↑s
//   ↑f
//
//  Step1: fast 先走 n+1=3 步
//  dummy → 1 → 2 → 3 → 4 → 5 → NULL
//   ↑s               ↑f
//
//  Step2: 同步走，直到 fast == NULL
//  dummy → 1 → 2 → 3 → 4 → 5 → NULL
//                   ↑s               ↑f
//
//  Step3: slow->next 就是要删的节点(4)
//  slow->next = slow->next->next  →  3→5
//
//  结果: 1 → 2 → 3 → 5
//
// 边界情况：删头节点 [1,2], n=2
//  dummy → 1 → 2 → NULL
//   ↑s
//   ↑f
//
//  fast 走 3 步: fast = NULL? 不对，n+1=3 但链表只有 dummy→1→2→NULL
//  实际: dummy → 1 → 2 → NULL
//   ↑s                     ↑f(NULL)
//  
//  fast 已经是 NULL，while 不执行，slow 仍在 dummy
//  slow->next = slow->next->next → dummy→2
//  返回 dummy.next = 2 ✓
// ============================================================
class Solution2 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;
        
        // 快指针先走 n+1 步
        // 为什么是 n+1 而不是 n？
        // 因为我们需要 slow 停在被删节点的"前驱"，不是被删节点本身
        // 间距 n+1 保证当 fast == NULL 时，slow 恰好在前驱位置
        for (int i = 0; i <= n; i++) {
            fast = fast->next;
        }
        
        // 同步前进，直到 fast 到达 NULL（链表末尾之后）
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        
        // 此时 slow 是被删节点的前驱，执行删除
        ListNode* toDelete = slow->next;
        slow->next = toDelete->next;
        delete toDelete;
        
        return dummy.next;
    }
};

// ============================================================
// 解法3: 递归回溯 — 利用递归"天然从后往前"的特性
// 时间: O(L)  空间: O(L) — 递归栈空间
//
// 【思路】
// 递归的回溯过程是从末尾往前"走"的。
// 递归到底后，回溯时计数：
//   - 回到第 1 层 → 倒数第 1 个
//   - 回到第 2 层 → 倒数第 2 个
//   - ...
//   - 回到第 N 层 → 就是要删的节点
//
// 为了让调用者能删除当前节点，我们在 node 的前驱层操作：
// 当 node->next 的回溯计数 == N 时，node 就是前驱，执行删除。
//
// 递归过程（[1,2,3,4,5], n=2）:
//
//  remove(dummy) → remove(1) → remove(2) → remove(3) → remove(4) → remove(5)
//                                                                    ↑ 5->next==NULL
//                                                                    返回 0
//                                                          ↑ count=0+1=1
//                                                          不等于 n=2
//                                                          返回 1
//                                               ↑ count=1+1=2 == n
//                                               删除 3->next(即节点4)
//                                               返回 2
//                                    ↑ count=3，返回 3
//                         ↑ count=4，返回 4
//              ↑ count=5，返回 5
// ============================================================
class Solution3 {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        removeHelper(&dummy, n);
        return dummy.next;
    }
    
private:
    // 返回值：node->next 是倒数第几个节点
    int removeHelper(ListNode* node, int n) {
        // 递归终止：到达最后一个节点，它后面没有节点了
        if (!node->next) return 0;
        
        // 递归处理：先深入到末尾
        int count = removeHelper(node->next, n) + 1;
        // count 表示 node->next 是倒数第 count 个
        
        if (count == n) {
            // node->next 就是要删除的节点，node 是其前驱
            ListNode* toDelete = node->next;
            node->next = toDelete->next;
            delete toDelete;
        }
        
        return count;
    }
};

// ============================================================
// 【解法对比】
//
// | 特性       | 解法1(两次遍历) | 解法2(双指针) ⭐ | 解法3(递归)   |
// |------------|----------------|-----------------|--------------|
// | 遍历次数   | 2              | 1               | 1(递归+回溯) |
// | 时间       | O(L)           | O(L)            | O(L)         |
// | 空间       | O(1)           | O(1)            | O(L) 栈空间  |
// | 代码复杂度 | 简单           | 简单            | 中等         |
// | 面试推荐   | 开场思路       | 首选            | 展示递归思维 |
//
// 三种解法时间复杂度相同（都是线性），区别在于：
// - 解法1 需要两次遍历，思路最直接
// - 解法2 一趟扫描，回答了 Follow-up，面试首选
// - 解法3 空间 O(L)，对超长链表可能栈溢出，但展示了递归视角
//
// ============================================================
// 【易错点】
//
// 1. 忘记 dummy 节点:
//    ✗ ListNode* slow = head;
//      当 n == 链表长度（删头节点）时，slow 没有前驱，无法删除
//    ✓ ListNode dummy(0, head); ListNode* slow = &dummy;
//
// 2. 快指针走 n 步而非 n+1 步:
//    ✗ for (int i = 0; i < n; i++) fast = fast->next;
//      slow 最终停在被删节点本身，而非前驱，无法执行单向链表删除
//    ✓ for (int i = 0; i <= n; i++) fast = fast->next;
//      让间距多 1，slow 停在前驱位置
//
// 3. 返回 head 而非 dummy.next:
//    ✗ return head;  // 如果 head 被删了，这是野指针/悬垂指针
//    ✓ return dummy.next;  // dummy.next 始终正确指向新的头节点
//
// 4. 内存泄漏:
//    ✗ slow->next = slow->next->next;  // 被删节点内存泄漏
//    ✓ ListNode* toDelete = slow->next;
//      slow->next = toDelete->next;
//      delete toDelete;  // 面试中主动提到释放内存是加分项
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1(基础理解): "暴力解怎么做？为什么需要两次遍历？"
//    → 单向链表不知道长度，需要先遍历一遍数长度 L。
//      倒数第 N 个 = 正数第 L-N+1 个，再遍历一次定位并删除。
//
// Q2(优化 — Follow-up): "能否只遍历一次？"
//    → 双指针：快指针先走 N+1 步，然后同步走。
//      fast==NULL 时 slow 恰好在被删节点的前驱。
//      用 dummy 节点统一处理删头节点的边界。
//
// Q3(边界): "如果链表只有一个节点且 n=1 呢？"
//    → dummy→1→NULL，fast 走 2 步到 NULL，
//      slow 在 dummy，dummy->next = NULL，返回空链表。
//      dummy 节点保证了这种情况无需特判。
//
// Q4(变体): "如果不用 dummy 节点怎么做？"
//    → fast 先走 N 步，如果此时 fast == NULL，
//      说明要删的是头节点，直接 return head->next。
//      否则同步走，正常删除。
//      这增加了一个特判，不如 dummy 优雅。
//
// Q5(扩展): "如果链表非常长（百万节点），递归解法有什么问题？"
//    → 递归深度等于链表长度 L，栈溢出风险。
//      必须用迭代解法（解法1或解法2），O(1) 空间。
// ============================================================
