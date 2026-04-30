/*
 * LeetCode 206: 反转链表 (Reverse Linked List)
 *
 * 【题目本质】
 * 把单链表中每条边的方向反过来，原来 A→B 变成 B→A。
 * 关键难点：反转当前指针后，不能丢失对后续节点的引用。
 *
 * 【解法总览】
 * 解法1: 迭代（双指针） — O(n) / O(1) — 面试首选
 * 解法2: 递归           — O(n) / O(n) — 面试必考
 * 解法3: 辅助数组       — O(n) / O(n) — 最直觉但不推荐
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
// 解法1: 迭代（双指针）— 三指针逐步反转
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 遍历链表时，用 prev 记住"前一个节点"，把 curr->next
// 从指向后面改成指向 prev。但改了之后就走不到原来的下一个
// 节点了，所以需要 next_temp 先保存。
//
// 每一步做四件事（顺序不能乱）：
//   1. 保存 next_temp = curr->next
//   2. 反转 curr->next = prev
//   3. prev 前进 = curr
//   4. curr 前进 = next_temp
//
// 指针变化过程:
//
//  初始:  prev=NULL   curr=1→2→3→4→5→NULL
//
//  Step1: next_temp=2
//         1→NULL                (反转)
//         prev=1  curr=2→3→4→5→NULL
//
//  Step2: next_temp=3
//         2→1→NULL             (反转)
//         prev=2  curr=3→4→5→NULL
//
//  Step3: next_temp=4
//         3→2→1→NULL           (反转)
//         prev=3  curr=4→5→NULL
//
//  Step4: next_temp=5
//         4→3→2→1→NULL         (反转)
//         prev=4  curr=5→NULL
//
//  Step5: next_temp=NULL
//         5→4→3→2→1→NULL       (反转)
//         prev=5  curr=NULL     → 循环结束，返回 prev
// ============================================================
class Solution1 {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr != nullptr) {
            ListNode* next_temp = curr->next;  // 必须先保存，否则反转后丢失
            curr->next = prev;                  // 核心：反转指针方向
            prev = curr;                        // prev 前进
            curr = next_temp;                   // curr 前进到原来的下一个
        }
        // 循环结束: curr==NULL, prev==原链表最后一个节点 = 新的头
        return prev;
    }
};

// ============================================================
// 解法2: 递归 — 信任子问题已解决，只处理当前层
// 时间: O(n)  空间: O(n) — 递归调用栈深度为 n
//
// 【思路】
// 递归的核心思想：假设 reverseList(head->next) 已经把后面的
// 链表完美反转了，我只需要把 head 接到反转后链表的尾部。
//
// 关键观察：反转后，head->next 指向的节点变成了反转链表的尾节点。
// 所以 head->next->next = head 就能把尾节点指回 head。
// 然后 head->next = nullptr 断开旧连接，防止成环。
//
// 递归过程（以 1→2→3→NULL 为例）:
//
//  调用链: reverseList(1) → reverseList(2) → reverseList(3)
//
//  Base case: head=3, 3->next==NULL → 返回 3
//
//  回溯 head=2:
//    递归返回: 3→NULL
//    当前状态: 2→3→NULL  (head->next 还指着3)
//    操作: head->next->next = head  →  3→2  (3指回2)
//          head->next = nullptr     →  2→NULL (断开2到3的旧连接)
//    结果: 3→2→NULL, 新头=3
//
//  回溯 head=1:
//    递归返回: 3→2→NULL
//    当前状态: 1→2→NULL  (head->next 还指着2)
//    操作: head->next->next = head  →  2→1  (2指回1)
//          head->next = nullptr     →  1→NULL (断开旧连接)
//    结果: 3→2→1→NULL, 新头=3
//
//  最终返回: 3→2→1→NULL ✓
// ============================================================
class Solution2 {
public:
    ListNode* reverseList(ListNode* head) {
        // base case: 空链表或只有一个节点
        // 为什么要检查 head==nullptr? 因为初始输入可能是空链表
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        // 递归反转 head 之后的所有节点
        // newHead 始终是原链表的最后一个节点（反转后的新头）
        ListNode* newHead = reverseList(head->next);

        // head->next 是反转后子链表的尾节点
        // 让它指回 head，完成当前层的反转
        head->next->next = head;

        // 断开 head 的旧连接！不写这行会形成环：
        // 比如 1⇄2 互相指向，永远遍历不完
        head->next = nullptr;

        // newHead 从最深层一路传递回来，始终不变
        return newHead;
    }
};

// ============================================================
// 解法3: 辅助数组 — 存值反转写回
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最直觉的方法：遍历链表把值存到数组，反向遍历数组写回链表。
// 缺点：没有真正改变链表结构，只改了值。如果节点有复杂数据
// （比如多个字段），这种方法不可行。面试中一般只用来展示理解
// 题意，然后马上给出迭代/递归解法。
// ============================================================
class Solution3 {
public:
    ListNode* reverseList(ListNode* head) {
        vector<int> vals;
        ListNode* curr = head;
        // 第一遍：收集所有值
        while (curr) {
            vals.push_back(curr->val);
            curr = curr->next;
        }
        // 第二遍：反向写回
        curr = head;
        for (int i = vals.size() - 1; i >= 0; i--) {
            curr->val = vals[i];
            curr = curr->next;
        }
        return head;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间 | 空间 | 优点           | 缺点              |
// |-----------|------|------|---------------|-------------------|
// | 迭代(双指针)| O(n) | O(1) | 空间最优，稳健   | 需要画图理解指针移动 |
// | 递归       | O(n) | O(n) | 代码简洁优雅     | 栈溢出风险(n很大时) |
// | 辅助数组   | O(n) | O(n) | 最好理解        | 没改变结构，不通用   |
//
// 面试推荐：先写迭代(展示基本功)，再写递归(展示递归思维)。
//
// ============================================================
// 【易错点】
//
// 1. 迭代法：四步操作顺序写反
//    ✗ curr->next = prev; ListNode* next_temp = curr->next;
//      此时 curr->next 已经是 prev，next_temp 拿到的是 prev 不是原来的下一个！
//    ✓ ListNode* next_temp = curr->next; curr->next = prev;
//      必须先保存再反转。
//
// 2. 递归法：忘记 head->next = nullptr
//    ✗ 只写 head->next->next = head;
//      结果：原始头节点1和节点2形成 1⇄2 的环，遍历链表会死循环。
//    ✓ 必须加 head->next = nullptr; 断开旧连接。
//
// 3. 递归 base case 漏掉 head==nullptr
//    ✗ if (head->next == nullptr) return head;
//      输入空链表时 head==nullptr，访问 head->next 导致空指针崩溃。
//    ✓ if (head == nullptr || head->next == nullptr) return head;
//
// 4. 迭代法返回 curr 而不是 prev
//    ✗ return curr;  // 循环结束时 curr==nullptr
//    ✓ return prev;  // prev 指向原链表最后一个节点 = 新头
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: 迭代和递归哪个更好？什么时候选哪个？
//     → 迭代 O(1) 空间，适合生产环境和大数据量。递归代码更简洁，
//       但有栈溢出风险。面试中建议两种都写，展示全面能力。
//
// Q2: 如果只反转第 m 到第 n 个节点呢？（LeetCode 92）
//     → 先走到第 m 个节点的前驱，对 [m,n] 区间用迭代法反转，
//       再把反转后的子链表接回原链表。核心反转操作复用本题。
//
// Q3: 如果每 k 个一组反转呢？（LeetCode 25，Hard）
//     → 外层循环分组，每组用本题的迭代法反转 k 个节点，
//       组间连接好。关键是处理不足 k 个的尾部不反转。
//
// Q4: 如何判断一个链表是否为回文？（LeetCode 234）
//     → 快慢指针找中点 → 反转后半部分(复用本题) → 逐一比较。
//
// ============================================================
// 【相关题型】
//
// - 92. 反转链表 II：复用本题的反转操作，区别是限定 [m,n] 区间，
//   需要额外的前驱/后继指针处理连接。
//
// - 25. K 个一组翻转链表：本题反转作为核心子函数，外层加分组逻辑，
//   代码改动：反转函数加个 count<=k 的限制条件。
//
// - 234. 回文链表：快慢指针找中点 + 本题反转后半部分 + 逐一比较。
//
// - 24. 两两交换链表中的节点：k=2 的特殊情况，可以递归也可以迭代。
// ============================================================
