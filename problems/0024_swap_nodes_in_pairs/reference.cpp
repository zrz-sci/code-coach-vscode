/*
 * LeetCode 24: Swap Nodes in Pairs (两两交换链表中的节点)
 *
 * 【题目本质】
 * 链表指针重排问题：每两个相邻节点为一组，交换它们的位置。
 * 核心在于正确处理三个指针的修改顺序，以及用 dummy head 统一头节点的处理。
 *
 * 【解法总览】
 * 解法1: 迭代 (Dummy Head) — O(n) / O(1) — 面试首选，空间最优
 * 解法2: 递归           — O(n) / O(n) — 代码最简洁，递归思维经典
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
// 解法1: 迭代 (Dummy Head + 逐对交换)
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 链表交换需要访问前驱节点（因为前驱的 next 也要改）。
// 用 dummy head 让第一对也有前驱，统一所有情况。
// 每轮循环：prev → first → second → rest
// 交换后：  prev → second → first → rest
// 然后 prev 移动到 first（它现在在后面）。
//
// 【指针变化图】
//
// 交换前:
//   prev → first(A) → second(B) → rest(C...)
//
// Step1: first->next = second->next   (A 跳过 B，指向 C)
//   prev → A → C...
//          B → C...    (B.next 还没改，仍指向原来的 C)
//   等等，B.next 还是指向 C 吗？不，A 原来指向 B，现在 A 指向 C
//   B 仍然指向原来的... 不对，让我重新画：
//
//   初始: prev → A → B → C
//   Step1: A->next = C        (A 跳过 B)
//          prev → A → C       B → C (B.next 未改，但 B 是局部变量已保存)
//   Step2: B->next = A        (B 指向 A)
//          B → A → C
//   Step3: prev->next = B     (前驱接上 B)
//          prev → B → A → C   ✓
//   Step4: prev = A           (A 是下一对的前驱)
//
// 示例完整过程 [1,2,3,4]:
//
//   初始: dummy → 1 → 2 → 3 → 4
//         prev
//
//   第1轮: first=1, second=2
//     1->next = 3       dummy → 1 → 3 → 4, 2 → 3(旧,但已保存second)
//     2->next = 1       2 → 1 → 3 → 4
//     prev->next = 2    dummy → 2 → 1 → 3 → 4
//     prev = 1(first)   dummy → 2 → 1 → 3 → 4
//                                     prev
//
//   第2轮: first=3, second=4
//     3->next = NULL    dummy → 2 → 1 → 3 → NULL, 4 → NULL(旧)
//     4->next = 3       4 → 3 → NULL
//     prev->next = 4    dummy → 2 → 1 → 4 → 3 → NULL
//     prev = 3(first)
//
//   循环结束（prev->next = NULL，不够两个节点）
//   返回 dummy.next = 2
//   结果: 2 → 1 → 4 → 3  ✓
// ============================================================
class Solution1 {
public:
    ListNode* swapPairs(ListNode* head) {
        // dummy head: 统一处理头节点变化，避免特判
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        
        // 至少需要两个节点才能交换
        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;
            ListNode* second = prev->next->next;
            
            // 三步指针重排（顺序关键：先改内部的，最后改入口的）
            // 为什么这个顺序？因为先改 prev->next 会丢失 first 的引用路径
            // 虽然 first 已保存在局部变量中，但按"从远到近"修改更不容易出错
            first->next = second->next;   // A 指向 C (跳过 B)
            second->next = first;         // B 指向 A (完成反转)
            prev->next = second;          // 前驱指向 B (接入主链)
            
            // prev 前进到下一对的前驱
            // 交换后顺序是 second → first → (下一对)
            // 所以 first 就是下一对的前驱
            prev = first;
        }
        
        return dummy.next;
    }
};


// ============================================================
// 解法2: 递归
// 时间: O(n)  空间: O(n) — 递归栈深度 n/2
//
// 【思路】
// 递归的核心：信任子问题已经解决。
// 假设 swapPairs(third) 已经返回了第3个节点之后所有节点交换好的链表头，
// 那我只需要处理当前这一对 (first, second) 的交换：
//   - first->next 指向递归结果
//   - second->next 指向 first
//   - 返回 second（新的头）
//
// 递归三要素:
//   返回值: 交换好的子链表的头节点
//   终止条件: 0个或1个节点，直接返回
//   单层逻辑: 交换当前对，接上递归结果
//
// 递归展开过程 [1,2,3,4]:
//
//   swapPairs(1→2→3→4)
//   │ first=1, second=2
//   │ 1->next = swapPairs(3→4)
//   │           │ first=3, second=4
//   │           │ 3->next = swapPairs(NULL) → return NULL
//   │           │ 4->next = 3
//   │           │ return 4       → 子链表: 4→3→NULL
//   │ 1->next = 4→3→NULL        → 1→4→3
//   │ 2->next = 1               → 2→1→4→3
//   │ return 2                  → 最终: 2→1→4→3  ✓
//
// 奇数长度 [1,2,3]:
//   swapPairs(1→2→3)
//   │ first=1, second=2
//   │ 1->next = swapPairs(3)
//   │           │ head=3, head->next=NULL → 终止, return 3
//   │ 1->next = 3               → 1→3
//   │ 2->next = 1               → 2→1→3
//   │ return 2                  → 最终: 2→1→3  ✓
// ============================================================
class Solution2 {
public:
    ListNode* swapPairs(ListNode* head) {
        // 终止条件：0个或1个节点，无法组成一对
        // 为什么要检查 !head->next？
        // 奇数长度链表最后剩1个节点，不需要交换
        if (!head || !head->next) return head;
        
        ListNode* first = head;
        ListNode* second = head->next;
        
        // 递归处理第三个节点开始的剩余链表
        // 信任递归：返回值是后续链表已经交换好的头节点
        first->next = swapPairs(second->next);
        
        // 交换当前一对：second 排前面，first 排后面
        second->next = first;
        
        // 返回新的头节点（second）
        return second;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度     | 迭代 (Solution1)      | 递归 (Solution2)       |
// |----------|----------------------|------------------------|
// | 时间     | O(n)                 | O(n)                   |
// | 空间     | O(1) ✅ 最优          | O(n/2) 递归栈          |
// | 代码行数 | ~15行                | ~8行                   |
// | 思维方式 | 逐步模拟指针操作      | 子问题分解，信任递归    |
// | 面试建议 | 展示链表操作功底      | 展示递归思维能力        |
//
// 面试策略：先给递归解（简洁，容易说清楚逻辑），
// 面试官追问空间优化时切迭代解。两种都能写是加分项。
//
// ============================================================
// 【易错点】
//
// 1. 循环条件不完整
//    ✗ while (prev->next)
//    ✓ while (prev->next && prev->next->next)
//    原因: 只剩1个节点时不能交换，缺少第二个检查会导致
//          second = prev->next->next 访问空指针
//
// 2. 指针修改顺序导致链表断裂
//    ✗ prev->next = second;        // 先改入口
//      first->next = second->next; // 此时逻辑上虽然 first 已保存，
//      second->next = first;       // 但思维容易混乱
//    ✓ first->next = second->next; // 从"远端"开始改
//      second->next = first;       // 改中间
//      prev->next = second;        // 最后改入口
//    原则: 修改指针时，从远离链表入口的方向开始改，最后改入口
//
// 3. 迭代中 prev 前进位置错误
//    ✗ prev = prev->next;          // 只前进了1步
//    ✗ prev = second;              // second 现在在前面，前进0步(原地)
//    ✓ prev = first;               // first 交换后在后面，是下一对的前驱
//    画图：交换后 prev→B→A→(next pair)，下一对的前驱是 A 即 first
//
// 4. 递归终止条件不完整
//    ✗ if (!head) return head;     // 漏掉了单节点情况
//    当 head->next == NULL 时，second = head->next 为 NULL，
//    后续 second->next 会崩溃
//    ✓ if (!head || !head->next) return head;
//
// ============================================================
// 【面试追问】
//
// Q1 (基础): 交换一对节点需要改几个指针？为什么需要 dummy head？
//    → 3个指针: prev->next, first->next, second->next。
//    → dummy head 让第一对也有前驱节点，不需要特判头节点变化。
//
// Q2 (优化): 递归解法能否优化空间？
//    → 递归栈深度 O(n/2)，无法在递归框架下优化。
//    → 切换到迭代解法可以做到 O(1) 空间。
//
// Q3 (泛化): 如果改成每 K 个一组翻转呢？(LeetCode 25)
//    → 本题是 K=2 的特例。泛化到 K 个需要：
//      (1) 先数 K 个节点，不够则不翻转
//      (2) 翻转这 K 个节点的子链表（用 206 反转链表的技巧）
//      (3) 接上递归/迭代处理后续的结果
//    → 核心的"前驱指针 + 子链表反转 + 接入主链"思路完全复用本题。
//
// Q4 (变体): 如果链表很长（百万级），递归还能用吗？
//    → 不能，会栈溢出。必须用迭代。
//    → 这也是为什么面试中迭代解法被认为更"成熟"的原因。
// ============================================================
