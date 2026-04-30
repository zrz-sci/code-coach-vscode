/*
 * LeetCode 83: 删除排序链表中的重复元素 (Remove Duplicates from Sorted List)
 *
 * 【题目本质】
 * 有序链表 → 重复值一定相邻 → 一次遍历逐个跳过重复节点。
 *
 * 【解法总览】
 * 解法1: 迭代（单指针）— O(n) / O(1) — 面试首选 ⭐
 * 解法2: 递归           — O(n) / O(n) — 展示递归思维
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
// 解法1: 迭代（单指针）— 面试首选 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 链表已排序，重复元素一定相邻。
// 用一个指针 curr 从头走到尾：
//   - curr->val == curr->next->val → 跳过 next（但 curr 不动，因为新 next 可能还重复）
//   - curr->val != curr->next->val → curr 前进
//
// 不需要 dummy 节点，因为头节点永远保留（保留每组第一个）。
//
// 【指针变化过程】
//
// 示例: 1→1→2→3→3→NULL
//
//   Step1: curr=1, next=1, 相同 → 1→2→3→3→NULL  (curr不动)
//          ^
//   Step2: curr=1, next=2, 不同 → curr前进到2
//             ^
//   Step3: curr=2, next=3, 不同 → curr前进到3
//                ^
//   Step4: curr=3, next=3, 相同 → 1→2→3→NULL  (curr不动)
//                   ^
//   Step5: curr=3, next=NULL → 循环结束
//
//   结果: 1→2→3→NULL
// ============================================================
class Solution1 {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* curr = head;
        while (curr && curr->next) {
            if (curr->val == curr->next->val) {
                // 重复：跳过下一个节点
                // 关键：curr 不前进，因为新的 next 可能还是重复的
                // 例如 1→1→1，第一次跳后变成 1→1，需要再跳一次
                ListNode* toDelete = curr->next;
                curr->next = toDelete->next;
                delete toDelete;  // 释放内存，面试中加分
            } else {
                // 不重复：安全前进
                curr = curr->next;
            }
        }
        return head;
    }
};

// ============================================================
// 解法2: 递归
// 时间: O(n)  空间: O(n) — 递归栈深度最坏 O(n)
//
// 【思路】
// 链表天然适合递归："假设后面的子链表已经去重了，
// 我只需要看当前头节点和子链表的头是否值相同"。
//
// 递归三要素：
//   返回值：去重后的子链表头节点
//   终止条件：空链表 或 只有一个节点（不可能重复）
//   单层逻辑：递归处理 head->next，然后判断 head 和新 next 是否重复
//
// 【递归展开过程】
//
// deleteDup(1→1→2→NULL)
//   head->next = deleteDup(1→2→NULL)
//     head->next = deleteDup(2→NULL)
//       head->next = deleteDup(NULL)
//         return NULL                        ← 终止条件
//       2->next = NULL, return 2→NULL        ← 只剩一个节点
//     1->next = 2→NULL, 1≠2 → return 1→2→NULL
//   1->next = 1→2→NULL, 1==1 → return 1→2→NULL  ← 跳过当前1
// 结果: 1→2→NULL
// ============================================================
class Solution2 {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // 终止条件：空链表或单节点
        if (!head || !head->next) return head;

        // 递归：先让后面的子链表去重完毕
        head->next = deleteDuplicates(head->next);

        // 判断当前头节点是否和去重后的下一个节点重复
        // 重复 → 跳过自己，返回 next（相当于删除当前节点）
        // 不重复 → 保留自己
        return (head->val == head->next->val) ? head->next : head;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 迭代          | 递归           |
// |-----------|---------------|---------------|
// | 时间       | O(n)          | O(n)          |
// | 空间       | O(1)          | O(n) 递归栈    |
// | 代码风格   | 命令式，直接   | 声明式，优雅    |
// | 栈溢出风险 | 无            | 链表很长时可能  |
// | 面试推荐   | ⭐ 首选       | 追问时展示      |
//
// 面试建议：先写迭代，被追问"递归行不行"时给递归版。
//
// ============================================================
// 【易错点】
//
// 1. 删除后仍然移动 curr：
//    ✗ curr->next = curr->next->next; curr = curr->next;
//      → 对于连续三个重复 1→1→1，跳过一个后 curr 就到了第三个1，
//        第二个和第三个之间的重复没有处理
//    ✓ 删除后 curr 不动，让 while 循环再次检查新的 curr->next
//
// 2. 循环条件漏判 curr->next：
//    ✗ while (curr) { if (curr->val == curr->next->val) ... }
//      → curr 是最后一个节点时，curr->next 为 NULL，访问 val 会崩溃
//    ✓ while (curr && curr->next)
//
// 3. 不需要 dummy 节点却画蛇添足：
//    本题保留每组重复元素的第一个，头节点一定保留。
//    只有 LeetCode 82（删除所有重复节点，一个不留）才需要 dummy。
//
// 4. 递归版忘记终止条件中检查 head->next：
//    ✗ if (!head) return head;  // 漏了单节点情况
//       后面 head->next 可能为 NULL，访问 head->next->val 崩溃
//    ✓ if (!head || !head->next) return head;
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: 需要 dummy 节点吗？
//     不需要。保留每组第一个节点 → 头节点一定保留。
//     对比第 82 题，要删除所有重复节点才需要 dummy。
//
// Q2: 改成"删除所有出现过重复的节点"（LeetCode 82）怎么做？
//     需要 dummy + prev 指针。发现一组重复时，prev->next 直接跳过整组。
//     关键区别：本题 curr 自身保留只跳 next；82 题整组都跳。
//
// Q3: 如果链表无序怎么去重？
//     方法一：归并排序 O(n log n) 后用本题方法。
//     方法二：哈希表记录已出现值，一次遍历 O(n) 时间 O(n) 空间。
//
// Q4: delete 被跳过的节点在实际工程中重要吗？
//     重要。不 delete 会内存泄漏。面试中主动提出并处理是加分项。
//
// ============================================================
// 【相关题型】
//
// - 82. 删除排序链表中的重复元素 II
//     复用"有序→重复相邻"的核心观察，但需要 dummy + prev，
//     且发现重复时 while 跳过整组，代码从 `curr->next = curr->next->next`
//     变成 `prev->next = curr`（跳过一整段）。
//
// - 26. 删除有序数组中的重复项
//     数组版的同一思路：快慢指针，慢指针标记已去重部分末尾，
//     快指针扫描，值不同时写入慢指针位置。
//     复用"有序→重复相邻→一次遍历"的模式。
//
// - 203. 移除链表元素
//     同样的链表节点删除操作，但判断条件从"和下一个相同"变成"等于目标值"。
//     需要 dummy（头节点本身可能被删）。
// ============================================================
