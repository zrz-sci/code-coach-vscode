/*
 * LeetCode 160: 相交链表 (Intersection of Two Linked Lists)
 *
 * 【题目本质】
 * 两条可能不等长的链表，找到它们的第一个公共节点（指针相同，不是值相同）。
 * 核心难点：如何消除两条链表的长度差，让两个指针"同步"到达交点。
 *
 * 【解法总览】
 * 解法1: 暴力嵌套      — O(m×n) / O(1)   — 最直觉，对照理解题意
 * 解法2: 哈希表        — O(m+n) / O(m)   — 空间换时间
 * 解法3: 双指针拼接法   — O(m+n) / O(1)   — 最优，面试首选
 * 解法4: 差值对齐法     — O(m+n) / O(1)   — 直觉版 O(1) 空间
 *
 * 链表结构示意:
 *
 *   A:  a1 → a2 ↘
 *                 c1 → c2 → c3 → null    (c1 是交点)
 *   B:  b1 → b2 → b3 ↗
 *
 *   A 独有部分长度 = a, B 独有部分长度 = b, 公共部分长度 = c
 *   A 总长 = a+c, B 总长 = b+c
 */

// Definition for singly-linked list.
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode(int x) : val(x), next(NULL) {}
// };

// ============================================================
// 解法1: 暴力嵌套 — 对 A 的每个节点遍历 B 查找
// 时间: O(m×n)  空间: O(1)
//
// 【思路】
// 最朴素的想法：拿 A 中的每个节点，去 B 中一个个比对。
// 第一个指针地址相同的节点就是交点。
// 瓶颈：对 A 的每个节点都要遍历整个 B，查找代价太高。
// ============================================================
class Solution1 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        for (ListNode* a = headA; a != nullptr; a = a->next) {
            for (ListNode* b = headB; b != nullptr; b = b->next) {
                // 比较的是指针（内存地址），不是值！
                if (a == b) return a;
            }
        }
        return nullptr;
    }
};

// ============================================================
// 解法2: 哈希表 — 存 A 所有节点，遍历 B 查找
// 时间: O(m+n)  空间: O(m)
//
// 【思路】
// 解法1 的瓶颈在查找：对 A 的每个节点，要 O(n) 遍历 B。
// 用 HashSet 把 A 的所有节点存起来，查找就变成 O(1)。
// 代价是 O(m) 额外空间。
// ============================================================
class Solution2 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        unordered_set<ListNode*> visited;

        // 第一遍：把 A 的所有节点指针存入 set
        for (ListNode* a = headA; a != nullptr; a = a->next) {
            visited.insert(a);
        }

        // 第二遍：遍历 B，第一个出现在 set 中的就是交点
        for (ListNode* b = headB; b != nullptr; b = b->next) {
            if (visited.count(b)) return b;
        }

        return nullptr;
    }
};

// ============================================================
// 解法3: 双指针拼接法 — A→B 与 B→A 等长同步走 ⭐ 面试首选
// 时间: O(m+n)  空间: O(1)
//
// 【思路】
// 解法2 用了 O(m) 空间。能否 O(1) 空间？
//
// 关键数学观察:
//   A 独有长度 = a, B 独有长度 = b, 公共长度 = c
//   a + c + b == b + c + a  (交换律!)
//
// 所以让:
//   pA: 走完 A 再走 B → 路径长度 = (a+c) + b
//   pB: 走完 B 再走 A → 路径长度 = (b+c) + a
// 两者相等，它们一定同时到达交点 c1!
//
// 指针路径示意:
//
//   pA: a1 → a2 → c1 → c2 → c3 → (null→跳转) → b1 → b2 → b3 → [c1] ← 相遇!
//                                                                  ^
//   pB: b1 → b2 → b3 → c1 → c2 → c3 → (null→跳转) → a1 → a2 → [c1] ← 相遇!
//
//   pA 走了: a + c + b 步到达 c1
//   pB 走了: b + c + a 步到达 c1
//   步数相等 ✓
//
// 不相交的情况 (c=0):
//   pA: a1→...→null → b1→...→null   共 a+b 步到 null
//   pB: b1→...→null → a1→...→null   共 b+a 步到 null
//   同时到 null，null==null 退出循环，返回 null ✓
// ============================================================
class Solution3 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode* pA = headA;
        ListNode* pB = headB;

        // 当 pA == pB 时退出: 要么同时到达交点，要么同时到达 null
        while (pA != pB) {
            // 关键: 判断 pA 是否为 null，而不是 pA->next
            // 走到 null 才跳转，确保不相交时两指针能同时到 null
            pA = (pA != nullptr) ? pA->next : headB;
            pB = (pB != nullptr) ? pB->next : headA;
        }

        return pA;  // 交点 or nullptr
    }
};

// ============================================================
// 解法4: 差值对齐法 — 先计算长度差，长的先走差值步
// 时间: O(m+n)  空间: O(1)
//
// 【思路】
// 另一个消除长度差的直觉方法:
// 1) 分别计算 A、B 长度
// 2) 长的链表先走 |lenA-lenB| 步
// 3) 然后同步走，第一个相同节点就是交点
//
// 示意图:
//   A:  a1 → a2 → c1 → c2 → c3           (长度 5, a=2, c=3)
//   B:  b1 → b2 → b3 → c1 → c2 → c3      (长度 6, b=3, c=3)
//
//   B 比 A 长 1，B 先走 1 步:
//   A:  a1 → a2 → c1 → c2 → c3
//       ↑ pA
//   B:       b2 → b3 → c1 → c2 → c3
//            ↑ pB (已经先走了1步)
//
//   同步走:
//   Step1: pA=a2, pB=b3  (不等)
//   Step2: pA=c1, pB=c1  (相等! 返回 c1)
// ============================================================
class Solution4 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // 第一步: 计算两条链表长度
        int lenA = 0, lenB = 0;
        for (ListNode* p = headA; p; p = p->next) lenA++;
        for (ListNode* p = headB; p; p = p->next) lenB++;

        ListNode* pA = headA;
        ListNode* pB = headB;

        // 第二步: 长的先走差值步，实现对齐
        // 用两个 while 避免计算负数差值
        while (lenA > lenB) { pA = pA->next; lenA--; }
        while (lenB > lenA) { pB = pB->next; lenB--; }

        // 第三步: 同步走，直到相遇或同时到 null
        while (pA != pB) {
            pA = pA->next;
            pB = pB->next;
        }

        return pA;  // 交点 or nullptr
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间     | 空间  | 特点               |
// |--------------|---------|------|--------------------|
// | 暴力嵌套      | O(m×n)  | O(1) | 只用于理解题意       |
// | 哈希表        | O(m+n)  | O(m) | 直觉，用空间换时间    |
// | 双指针拼接法   | O(m+n)  | O(1) | 最优雅，代码最简洁    |
// | 差值对齐法     | O(m+n)  | O(1) | 最直觉的O(1)空间方案  |
//
// 双指针拼接 vs 差值对齐:
//   本质都是"消除长度差"。拼接法用数学巧妙实现，差值法分三步显式对齐。
//   面试推荐双指针拼接法（代码短，印象深刻），但差值对齐法更容易讲清楚。
//
// ============================================================
// 【易错点】
//
// 1. 比较值而非指针:
//    ✗ if (a->val == b->val) return a;   // 值相等不代表是同一节点!
//    ✓ if (a == b) return a;              // 必须比较指针(内存地址)
//
// 2. 双指针法跳转时机:
//    ✗ pA = (pA->next) ? pA->next : headB;  // pA 永远不会为 null
//      → 不相交时死循环! 因为两指针永远不可能同时为 null
//    ✓ pA = (pA) ? pA->next : headB;         // pA 走到 null 才跳转
//      → 不相交时，两指针各走 m+n 步后同时为 null，退出循环
//
// 3. 差值对齐法的长度差处理:
//    ✗ int diff = lenA - lenB;  // 如果 lenA < lenB，diff 为负数
//      for (int i = 0; i < diff; i++) pA = pA->next;  // 不执行
//      // 漏掉了 lenB > lenA 的情况!
//    ✓ 用两个独立的 while 分别处理两个方向
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 你能用暴力法做吗？瓶颈是什么？
//     → 双循环 O(m×n)，瓶颈在"查找"操作是 O(n)。
//
// Q2: 如何降到 O(m+n)？
//     → 哈希表把查找降到 O(1)，但需要 O(m) 空间。
//
// Q3: Follow-up: 能否同时做到 O(m+n) 时间和 O(1) 空间？
//     → 双指针拼接法或差值对齐法。
//
// Q4: 为什么双指针拼接法一定不会死循环？
//     → 不相交: 两指针各走 a+b 步同时到 null。
//       相交: 两指针各走 a+b+c 步同时到交点。
//       最多 2(m+n) 步一定结束。
//
// Q5: 如果链表可能有环怎么办？
//     → 先用快慢指针检测环 (LC 141/142)。
//       若一个有环一个无环: 不可能相交。
//       若都有环: 找各自入环点，若同一入环点则类似本题处理，
//       若不同入环点则沿环走一圈看能否遇到另一个入环点。
// ============================================================
