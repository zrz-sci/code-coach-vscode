/*
 * LeetCode 142: 环形链表 II (Linked List Cycle II)
 *
 * 【题目本质】
 * 找到链表中环的入口节点。如果无环返回 nullptr。
 * 这是 Floyd 判圈算法的经典应用。
 *
 * 【解法总览】
 * 解法1: 哈希表       — O(n) / O(n) — 最直觉，用 set 记录访问过的节点
 * 解法2: 快慢指针     — O(n) / O(1) — 面试首选，Floyd 算法两阶段
 *
 * 【与 141 题的关系】
 * 141 只需 Phase 1 判断有无环 (slow == fast → return true)
 * 142 在 Phase 1 基础上增加 Phase 2 找环入口
 * 代码区别：141 在相遇时 return true; 142 在相遇时启动第二阶段
 */

// Definition for singly-linked list.
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode(int x) : val(x), next(NULL) {}
// };

// ============================================================
// 解法1: 哈希表 — 记录访问过的节点
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最直觉的想法：遍历链表，把每个节点地址存入哈希表。
// 第一个"已经在哈希表中"的节点就是环入口。
// 如果遍历到 null 都没重复，说明无环。
//
// 为什么存地址不存值？因为不同节点可能有相同的 val，
// 但每个节点在内存中的地址是唯一的。
//
// 图示 (示例1: [3,2,0,-4], pos=1):
//
//   head
//    ↓
//    3 → 2 → 0 → -4
//        ↑         |
//        └─────────┘
//
//   遍历过程:
//   Step1: visit 3   → set = {3}
//   Step2: visit 2   → set = {3, 2}
//   Step3: visit 0   → set = {3, 2, 0}
//   Step4: visit -4  → set = {3, 2, 0, -4}
//   Step5: visit 2   → 已在 set 中! 返回节点 2 (环入口)
// ============================================================
class Solution1 {
public:
    ListNode *detectCycle(ListNode *head) {
        unordered_set<ListNode*> visited;
        ListNode* curr = head;
        while (curr != nullptr) {
            // 如果当前节点的地址已经见过，它就是环入口
            if (visited.count(curr)) {
                return curr;
            }
            visited.insert(curr);
            curr = curr->next;
        }
        // 走到 null，说明无环
        return nullptr;
    }
};

// ============================================================
// 解法2: 快慢指针 (Floyd 判圈算法) — O(1) 空间找环入口
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1 的瓶颈是 O(n) 空间（存所有访问过的节点）。
// Follow-up 要求 O(1) 空间，必须用纯指针技巧。
//
// 分两个阶段：
// Phase 1: 快慢指针判断有无环（和 141 题完全一样）
//   - slow 每次走 1 步，fast 每次走 2 步
//   - 如果 fast 走到 null → 无环
//   - 如果 slow == fast → 有环，记录相遇点
//
// Phase 2: 找环入口（本题的核心）
//   - 一个指针从 head 出发，一个从相遇点出发
//   - 两个都每次走 1 步
//   - 它们相遇的地方就是环入口
//
// 【数学推导 — 为什么 Phase 2 能找到环入口】
//
//   设: a = head 到环入口的距离
//       b = 环入口到相遇点的距离
//       c = 相遇点沿环方向回到入口的距离
//       环长 L = b + c
//
//       head ----a---- entry ----b---- meet
//                        ↑               |
//                        └------c--------┘
//
//   Phase 1 相遇时:
//     慢指针走了: a + b  步
//     快指针走了: a + b + n*L 步 (在环里多转了 n 圈, n ≥ 1)
//
//   快指针速度 = 2 × 慢指针速度:
//     2(a + b) = a + b + n*L
//     a + b = n*L
//     a = n*L - b
//     a = n*(b + c) - b
//     a = (n-1)*(b+c) + c
//
//   含义: 从 head 走 a 步 = 从相遇点走 c 步 + (n-1) 整圈
//         两者最终都到达环入口！
//
// 【图解完整过程 (示例1: [3,2,0,-4], pos=1)】
//
//   链表结构:
//       3 → [2] → 0 → -4
//            ↑          |
//            └──────────┘
//
//   a=1 (3→2), 环=[2,0,-4], 环长=3
//
//   Phase 1:
//   初始: slow=3, fast=3
//   Step1: slow=2,  fast=0    (slow +1, fast +2)
//   Step2: slow=0,  fast=2    (slow +1, fast +2: -4→2)
//   Step3: slow=-4, fast=-4   (slow +1, fast +2: 0→-4)
//          ↑ 相遇! 相遇点 = 节点 -4
//
//   Phase 2:
//   ptr1 从 head(3), ptr2 从相遇点(-4)
//   Step1: ptr1=2, ptr2=2     ← 相遇! 环入口 = 节点 2 ✓
//
//   验证: a=1, b=2 (2→0→-4), c=1 (-4→2), a == c ✓
// ============================================================
class Solution2 {
public:
    ListNode *detectCycle(ListNode *head) {
        // ---- Phase 1: 快慢指针找相遇点 ----
        ListNode* slow = head;
        ListNode* fast = head;
        
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;          // 慢指针走 1 步
            fast = fast->next->next;    // 快指针走 2 步
            
            if (slow == fast) {
                // 有环！slow/fast 就是相遇点
                
                // ---- Phase 2: 从 head 和相遇点同时出发找环入口 ----
                // 为什么这样能找到入口？因为 a = c + (n-1)圈
                // 从 head 走 a 步 和 从相遇点走 a 步 都到达环入口
                ListNode* ptr1 = head;
                ListNode* ptr2 = slow;   // 相遇点
                while (ptr1 != ptr2) {
                    ptr1 = ptr1->next;   // 都是每次走 1 步！
                    ptr2 = ptr2->next;   // 注意：不是快慢指针了
                }
                return ptr1;  // ptr1 == ptr2 就是环入口
            }
        }
        
        // fast 走到了 null，说明链表无环
        return nullptr;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 解法1 哈希表 | 解法2 快慢指针 |
// |----------|-------------|---------------|
// | 时间     | O(n)        | O(n)          |
// | 空间     | O(n)        | O(1) ✅       |
// | 代码难度  | 简单        | 需要数学推导   |
// | 面试场景  | 开局方案    | 优化后的最优解 |
//
// 面试策略: 先说哈希表(30秒)，主动说"可以 O(1) 空间"，
// 然后写快慢指针并能推导出 a = c 的数学关系。
//
// 【易错点】
//
// 1. Phase 2 误用快慢速度
//    ✗ ptr2 = ptr2->next->next;  (Phase 2 不是快慢指针!)
//    ✓ ptr2 = ptr2->next;        (两个都走1步)
//
// 2. 相遇后直接返回相遇点
//    ✗ if (slow == fast) return slow;  (相遇点 ≠ 环入口!)
//    ✓ 相遇后还要执行 Phase 2 才能找到真正的入口
//
// 3. 哈希表存节点值而不是存指针
//    ✗ visited.insert(curr->val);   (不同节点可能值相同)
//    ✓ visited.insert(curr);        (存地址，每个节点唯一)
//
// 4. Phase 1 循环条件不完整
//    ✗ while (fast->next != nullptr)     (fast自身可能为null)
//    ✓ while (fast != nullptr && fast->next != nullptr)
//
// 5. 空链表 / 单节点无环
//    head == nullptr 时，while 条件直接不成立，返回 nullptr ✓
//    单节点无环时同理，fast->next == nullptr，不进循环 ✓
//
// 【面试追问】
//
// Q1: 和 141 环形链表的关系？
//     → 141 只需 Phase 1，相遇 return true / 走到null return false
//       142 在 Phase 1 相遇后增加 Phase 2 找入口
//       代码改动: 141 的 `return true` 变成 Phase 2 的 while 循环
//
// Q2: 为什么快指针速度是2倍不是3倍？
//     → 2倍保证快指针每轮比慢指针多走恰好1步
//       在环内，相对速度为1步/轮，不会"跳过"慢指针
//       如果3倍，相对速度为2步/轮，环长为奇数时才一定能追上
//       2倍是最简单且一定正确的选择
//
// Q3: 如何求环的长度？
//     → Phase 1 相遇后，让一个指针不动，另一个继续走
//       再次回到相遇点时走的步数 = 环长
//       或者在 Phase 1 中记录 slow 进入环后走的步数也行
//
// Q4: LeetCode 287 寻找重复数字和本题什么关系？
//     → 287 把数组 nums 看成"索引→值"的隐式链表
//       nums[i] 就是节点 i 的 next 指针指向 nums[i]
//       重复的数字 = 有两个节点的 next 指向同一个节点 = 环入口
//       直接套用本题的 Floyd 两阶段算法
//
// 【相关题型】
// - 141 环形链表: 本题的 Phase 1，只判断有无环
// - 287 寻找重复数字: 数组版 Floyd 算法，复用完全相同的两阶段模板
// - 160 相交链表: 也是双指针找"汇合点"，推导思路类似
// - 876 链表的中间结点: 快慢指针基础应用，作为热身
// ============================================================
