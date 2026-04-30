/*
 * LeetCode 82: Remove Duplicates from Sorted List II (删除排序链表中的重复元素 II)
 * 难度：Medium | 标签：Linked List, Two Pointers
 *
 * ============================================================
 * 题目描述：
 * 给定已排序链表的头节点 head，删除所有有重复数字的节点，
 * 只保留原始链表中没有重复出现的数字。返回排序后的链表。
 *
 * 约束：
 * - 节点数在 [0, 300] 范围内
 * - -100 <= Node.val <= 100
 * - 链表已按升序排列
 *
 * 关键区别（vs LC 83）：
 * LC 83: [1,2,3,3,4,4,5] → [1,2,3,4,5]  (保留一个重复元素)
 * LC 82: [1,2,3,3,4,4,5] → [1,2,5]       (删除所有重复元素)
 * ============================================================
 *
 * 指针图解：
 *
 * 输入: dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
 *         ↑
 *        prev
 *
 * Step1: 1!=2, prev前进
 *        dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
 *                 ↑
 *                prev
 *
 * Step2: 2!=3, prev前进
 *        dummy → [1] → [2] → [3] → [3] → [4] → [4] → [5] → null
 *                        ↑
 *                       prev
 *
 * Step3: 3==3! 跳过所有3
 *        dummy → [1] → [2] ──→ [4] → [4] → [5] → null
 *                        ↑
 *                       prev (不前进！)
 *
 * Step4: 4==4! 跳过所有4
 *        dummy → [1] → [2] ────────→ [5] → null
 *                        ↑
 *                       prev (不前进！)
 *
 * Step5: 5→null, 不重复, prev前进
 *        dummy → [1] → [2] ────────→ [5] → null
 *                                     ↑
 *                                    prev
 *
 * 结果: [1] → [2] → [5]
 *
 * ============================================================
 */

// Definition for singly-linked list.


/*
 * ============================================================
 * 解法一：迭代 + Dummy 节点（面试首选）
 * ============================================================
 *
 * 核心思路：
 * 1. dummy 节点处理头节点可能被删除的情况
 * 2. prev 指向最后一个确认不重复的节点
 * 3. 发现重复段时，跳过整段，prev->next 直接指向段后
 * 4. 关键：跳过重复后 prev 不前进（新节点可能也是重复的开始）
 *
 * 时间复杂度：O(n) — 每个节点最多被访问两次
 * 空间复杂度：O(1) — 只用了固定数量的指针
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // dummy 节点：值随意，next 指向 head
        ListNode dummy(0, head);
        ListNode* prev = &dummy;  // prev 指向最后一个确认不重复的节点

        while (prev->next) {
            ListNode* curr = prev->next;

            if (curr->next && curr->val == curr->next->val) {
                // ===== 发现重复 =====
                int val = curr->val;

                // 跳过所有值为 val 的节点
                while (curr && curr->val == val) {
                    curr = curr->next;
                }
                // curr 现在指向第一个值 != val 的节点（或 null）
                prev->next = curr;

                // 重要：prev 不前进！
                // 因为 curr 可能是新一段重复的开始
                // 例如: [2,3,3,4,4,5] 中跳过 3,3 后 curr=[4]
                // 但 [4,4] 也是重复的，prev 需要能跳过它

            } else {
                // ===== 不重复，安全前进 =====
                prev = prev->next;
            }
        }

        return dummy.next;
    }
};


/*
 * ============================================================
 * 解法二：递归
 * ============================================================
 *
 * 核心思路（决策树）：
 *
 *   head->val == head->next->val?
 *        |                 |
 *       YES               NO
 *        |                 |
 *   跳过所有相同值      保留 head
 *   递归处理剩余       head->next = 递归(head->next)
 *   return 递归结果    return head
 *
 * 递归展开（输入 [1,2,3,3,4,4,5]）：
 *
 *   f([1,2,3,3,4,4,5])
 *     1!=2 → 保留1, 1->next = f([2,3,3,4,4,5])
 *       2!=3 → 保留2, 2->next = f([3,3,4,4,5])
 *         3==3 → 跳过3,3 → f([4,4,5])
 *           4==4 → 跳过4,4 → f([5])
 *             单节点 → return [5]
 *       2->next = [5]
 *     1->next = [2]→[5]
 *   return [1]→[2]→[5]
 *
 * 时间复杂度：O(n) — 每个节点处理一次
 * 空间复杂度：O(n) — 递归栈深度
 */
class Solution_Recursive {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // 基准情况：空链表或单节点
        if (!head || !head->next) return head;

        if (head->val == head->next->val) {
            // 当前值重复，跳过所有相同值的节点
            int val = head->val;
            while (head && head->val == val) {
                head = head->next;
            }
            // 递归处理剩余部分（head 可能为 null）
            return deleteDuplicates(head);
        } else {
            // 当前值不重复，保留 head
            head->next = deleteDuplicates(head->next);
            return head;
        }
    }
};


/*
 * ============================================================
 * 解法三：双指针变体（显式 fast/slow 命名）
 * ============================================================
 *
 * 与解法一逻辑相同，用 slow/fast 命名更直观：
 * - slow: 最后一个确认不重复的节点（= prev）
 * - fast: 向前探索的指针（= curr）
 */
class Solution_TwoPointer {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(0, head);
        ListNode* slow = &dummy;

        while (slow->next) {
            ListNode* fast = slow->next;

            // fast 向前探索相同值的段
            while (fast->next && fast->val == fast->next->val) {
                fast = fast->next;
            }

            if (slow->next == fast) {
                // slow->next 和 fast 是同一个节点 → 没有重复
                slow = slow->next;
            } else {
                // slow->next 到 fast 是一段重复 → 跳过
                slow->next = fast->next;
            }
        }

        return dummy.next;
    }
};


/*
 * ============================================================
 * 解法四：使用 unordered_map（无序链表也可用）
 * ============================================================
 *
 * 两次遍历：
 * 1. 第一遍统计每个值出现的频次
 * 2. 第二遍删除频次 > 1 的节点
 *
 * 优势：也能处理无序链表
 * 劣势：O(n) 额外空间
 *
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
class Solution_Map {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        // 第一遍：统计频次
        unordered_map<int, int> freq;
        ListNode* curr = head;
        while (curr) {
            freq[curr->val]++;
            curr = curr->next;
        }

        // 第二遍：删除频次 > 1 的节点
        ListNode dummy(0, head);
        ListNode* prev = &dummy;
        curr = head;
        while (curr) {
            if (freq[curr->val] > 1) {
                prev->next = curr->next;  // 跳过
            } else {
                prev = curr;              // 保留
            }
            curr = curr->next;
        }

        return dummy.next;
    }
};


/*
 * ============================================================
 * 常见错误示范
 * ============================================================
 */

/* 错误1: 忘记 dummy 节点
 *
 * ListNode* prev = head;  // 如果 head 本身重复，无法删除
 * 输入 [1,1,2] → 期望 [2] → 得到 [1,2] (错误)
 */

/* 错误2: 跳过重复时未检查 null
 *
 * while (curr->val == val) curr = curr->next;
 * // 如果链表以重复值结尾 [1,2,3,3]，curr 变为 null
 * // 此时 curr->val 会导致空指针异常
 * // 正确: while (curr && curr->val == val)
 */

/* 错误3: 跳过重复后 prev 前进
 *
 * prev->next = curr;
 * prev = prev->next;  // 错！curr 可能也是重复的开始
 * // 输入 [1,2,2,3,3,4] → 可能跳不过 [3,3]
 */

/* 错误4: 混淆 LC82 和 LC83
 *
 * LC83 (保留一个): if (curr->val == curr->next->val) curr->next = curr->next->next;
 * LC82 (全删): 需要 prev 指针，跳过整段
 */


/*
 * ============================================================
 * 复杂度对比
 * ============================================================
 *
 * | 解法         | 时间   | 空间   | 面试推荐 |
 * |-------------|--------|--------|---------|
 * | 迭代+Dummy  | O(n)   | O(1)   | ⭐⭐⭐  |
 * | 递归         | O(n)   | O(n)   | ⭐⭐    |
 * | 双指针变体   | O(n)   | O(1)   | ⭐⭐⭐  |
 * | HashMap     | O(n)   | O(n)   | ⭐     |
 *
 * ============================================================
 * 面试追问
 * ============================================================
 *
 * Q1: 如果保留一个重复元素？ → LC 83，简单得多
 * Q2: 如果链表无序？ → 用 HashMap 统计频次
 * Q3: 能否不用 dummy 节点？ → 可以但需特殊处理头节点
 * Q4: 如果要保留恰好出现 k 次的元素？ → 先统计频次再删除
 * Q5: 递归深度最坏多少？ → O(n)，所有不重复时
 *
 * ============================================================
 * 相关题目
 * ============================================================
 *
 *  83. Remove Duplicates from Sorted List     —— 保留一个
 * 203. Remove Linked List Elements            —— 删除指定值
 *  19. Remove Nth Node From End of List       —— dummy技巧
 * 206. Reverse Linked List                    —— 链表基础
 *  26. Remove Duplicates from Sorted Array    —— 数组版(保留1个)
 *  80. Remove Duplicates from Sorted Array II —— 数组版(保留2个)
 *
 * ============================================================
 * 总结：解法对比 & 面试要点
 * ============================================================
 *
 * 【解法选择决策树】
 *
 *   链表有序？
 *    ├─ YES → 迭代+Dummy O(n), O(1)  ← 面试首选
 *    │         核心: prev指针 + 跳过重复段后不前进
 *    │         递归版 O(n), O(n) ← 代码最简洁但栈空间大
 *    └─ NO  → HashMap两遍 O(n), O(n) ← 无序链表唯一选择
 *
 * 【核心易错点】
 * 1. 必须用 Dummy 节点 — head 本身可能被删除([1,1,2]→[2])
 * 2. 跳过重复段后 prev 不前进 — 新段可能也是重复的开头
 *    这是本题与 LC83 最本质的区别，也是最容易写错的地方
 * 3. while 跳过重复时必须检查 null — 链表可能以重复值结尾
 *    错: while(curr->val==val) / 对: while(curr && curr->val==val)
 * 4. 混淆 LC82 和 LC83 — 82是"全删"需要prev，83是"留一个"只需curr
 *
 * 【面试追问链 Q1 → Q2 → Q3】
 *
 * Q1: 能否不用 Dummy 节点？
 *   → 可以，但必须单独处理头节点重复的情况（while循环跳过头部重复段），
 *     代码会变复杂且容易出错。Dummy 是链表题的通用技巧，面试中推荐直接用。
 *
 * Q2: 如果链表无序，还能 O(1) 空间解决吗？
 *   → 不能。无序时必须先知道每个值的出现次数才能决定删不删，
 *     至少需要 O(n) 空间做 HashMap 统计。
 *     或者先排序 O(nlogn)，但链表归并排序后还是需要再做一遍删除。
 *
 * Q3: 如果要保留恰好出现 k 次的元素，怎么改？
 *   → HashMap方案：第一遍统计频次，第二遍只保留 freq[val]==k 的节点。
 *     对于有序链表也可以一遍扫描：记录当前值的出现次数 cnt，
 *     走完一段后如果 cnt==k 就保留，否则跳过。
 */
