/*
 * LeetCode 147: 对链表进行插入排序 (Insertion Sort List)
 *
 * 【题目本质】
 * 在链表上实现插入排序。核心操作：维护已排序链表，每次从未排序部分取一个节点，
 * 在已排序链表中找到正确位置并插入。
 *
 * 【解法总览】
 * 解法1: 标准插入排序   — O(n^2) / O(1) — 每次从头找插入位置
 * 解法2: 优化版         — O(n^2) / O(1) — 记录尾部，大值直接追加 ⭐推荐
 */

// ============================================================
// 解法1: 标准链表插入排序
// 时间: O(n^2)  空间: O(1)
//
// 【思路】
// 创建 dummy 节点作为已排序链表的虚拟头。
// 从原链表逐个取出节点，在已排序链表中找到正确位置插入。
//
// 指针操作示意 (head = [4, 2, 1, 3]):
//
// 初始: dummy → null,  curr → [4] → [2] → [1] → [3]
//
// Step 1: 取 curr=[4]
//   prev 从 dummy 开始, prev->next == null, 停止
//   插入: [4]->next = null; dummy->next = [4]
//   结果: dummy → [4]                curr → [2] → [1] → [3]
//
// Step 2: 取 curr=[2]
//   prev 从 dummy 开始, prev->next=[4], 4 >= 2, 停止
//   插入: [2]->next = [4]; dummy->next = [2]
//   结果: dummy → [2] → [4]          curr → [1] → [3]
//
// Step 3: 取 curr=[1]
//   prev 从 dummy 开始, prev->next=[2], 2 >= 1, 停止
//   插入: [1]->next = [2]; dummy->next = [1]
//   结果: dummy → [1] → [2] → [4]    curr → [3]
//
// Step 4: 取 curr=[3]
//   prev 从 dummy 开始:
//     prev->next=[1], 1 < 3 → 前进
//     prev->next=[2], 2 < 3 → 前进
//     prev->next=[4], 4 >= 3, 停止 (prev 指向 [2])
//   插入: [3]->next = [4]; [2]->next = [3]
//   结果: dummy → [1] → [2] → [3] → [4]
//
// 插入操作指针变化详解:
//
//   before: prev → [2] → [4] → null    curr → [3] (已断开)
//
//   curr->next = prev->next:
//           prev → [2] → [4]    curr → [3] → [4]  ← [3]接上[4]
//
//   prev->next = curr:
//           prev → [2] → [3] → [4]               ← [2]指向[3]，完成!
//
// ============================================================
class Solution1 {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode dummy(0);       // 已排序链表的虚拟头
        ListNode* curr = head;   // 遍历原链表

        while (curr) {
            // 关键: 先保存下一个节点! 插入操作会修改 curr->next
            ListNode* next = curr->next;

            // 在已排序链表中找到第一个 val >= curr->val 的节点的前驱
            // 使用 < (而非 <=) 保证稳定排序
            ListNode* prev = &dummy;
            while (prev->next && prev->next->val < curr->val) {
                prev = prev->next;
            }

            // 插入 curr 到 prev 之后
            curr->next = prev->next;  // curr 接上后续节点
            prev->next = curr;        // prev 指向 curr

            curr = next;  // 处理下一个
        }

        return dummy.next;
    }
};

// ============================================================
// 解法2: 优化版 — 记录已排序尾部，大值直接追加 ⭐推荐
// 时间: O(n^2) 最坏, 近乎有序时 O(n)  空间: O(1)
//
// 【优化思路】
// 观察：如果当前节点值 >= 已排序部分的尾部值，它一定排在最后。
// 此时直接追加到尾部，不需要从 dummy 头遍历。
//
// 这个优化对"大部分已排序"的输入效果极好:
//   已排序输入: 每次都直接追加 → O(n)
//   逆序输入: 每次都从头查找 → O(n^2)，和标准版一样
//
// 示意 (head = [-1, 0, 3, 4, 5, 2]):
//
//   取-1: dummy(val=0), 0 <= -1? No → 从头找 → 插在dummy后   tail=[-1]
//   取 0: tail[-1], -1 <= 0? Yes  → 直接追加!                tail=[0]
//   取 3: tail[0],   0 <= 3? Yes  → 直接追加!                tail=[3]
//   取 4: tail[3],   3 <= 4? Yes  → 直接追加!                tail=[4]
//   取 5: tail[4],   4 <= 5? Yes  → 直接追加!                tail=[5]
//   取 2: tail[5],   5 <= 2? No   → 从头找 → 插在0和3之间
//
//   6个节点中只有2个需要从头遍历，大幅减少操作!
//
// ============================================================
class Solution2 {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode dummy(0);
        ListNode* curr = head;
        ListNode* tail = &dummy;  // 已排序链表的尾节点

        while (curr) {
            ListNode* next = curr->next;

            if (tail->val <= curr->val) {
                // 快捷路径: 当前值 >= 已排序尾部 → 直接追加
                tail->next = curr;
                tail = curr;
                tail->next = nullptr;  // 重要! 断开与原链表的连接
            } else {
                // 普通路径: 从头遍历找插入位置
                ListNode* prev = &dummy;
                while (prev->next && prev->next->val < curr->val) {
                    prev = prev->next;
                }
                curr->next = prev->next;
                prev->next = curr;
            }

            curr = next;
        }

        return dummy.next;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 最坏时间  | 最好时间 | 空间  | 面试推荐 |
// |-----------|----------|---------|-------|---------|
// | 标准版     | O(n^2)  | O(n^2)  | O(1)  | ⭐⭐     |
// | 优化版     | O(n^2)  | O(n)    | O(1)  | ⭐⭐⭐   |
//
// 优化版在输入大致有序时接近 O(n)，且代码量增加不多，推荐使用。
//
// ============================================================
// 【易错点】
//
// 1. 忘记保存 next:
//    ✗ 操作 curr 后直接 curr = curr->next
//    ✓ 先 next = curr->next，最后 curr = next
//    插入操作会修改 curr->next，导致原链表遍历断裂
//
// 2. 查找条件 < vs <=:
//    ✗ while (prev->next && prev->next->val <= curr->val) — 不稳定排序
//    ✓ while (prev->next && prev->next->val < curr->val)  — 稳定排序
//    相等元素保持原始相对顺序
//
// 3. 优化版忘记 tail->next = nullptr:
//    追加后 tail 的 next 仍指向原链表中的旧节点
//    不断开会导致已排序链表形成环或混乱
//
// 4. dummy 初始值的边界:
//    dummy(0) 的 val=0，第一个 if (tail->val <= curr->val) 中
//    当 curr->val < 0 时条件不成立，正确走 else 分支
//
// ============================================================
// 【面试追问 Interview Follow-ups】
//
// Q1: 如何做到 O(n log n) 的链表排序?
// A1: 归并排序 (LC 148)。用快慢指针找中点，递归切分，merge 两个有序链表。
//     时间 O(n log n)，空间 O(log n) 递归栈（或自底向上做 O(1) 空间）。
//
// Q2: 输入几乎有序时，插入排序有何优势?
// A2: 如果每个元素距最终位置不超过 k，插入排序是 O(nk)。
//     当 k << log n 时比 O(n log n) 更快。Python 的 Timsort 利用了这个特性。
//
// Q3: 为什么不用快排排链表?
// A3: 快排依赖随机访问来做 partition 和选 pivot。
//     链表随机访问 O(n)，且不能取中间元素做 pivot，容易退化为 O(n^2)。
//     归并排序更适合链表。
// ============================================================
