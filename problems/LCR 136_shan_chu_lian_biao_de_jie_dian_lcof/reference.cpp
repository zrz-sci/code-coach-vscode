/*
 * LCR 136: 删除链表的节点
 *
 * 难度: Easy
 * 标签: Linked List
 *
 * 核心思想: 遍历链表找到目标值，修改前驱的 next 指针跳过目标节点
 *           使用哨兵节点(dummy)统一处理删除头节点的特殊情况
 */

// 注意: ListNode 定义在 solution.cpp 中已提供
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode() : val(0), next(nullptr) {}
//     ListNode(int x) : val(x), next(nullptr) {}
//     ListNode(int x, ListNode *next) : val(x), next(next) {}
// };

// ============================================================
// 解法1: 哨兵节点 + 双指针遍历  ⭐ 面试推荐
// 时间: O(n)  空间: O(1)
//
// 核心: dummy 节点让 head 也有前驱，统一删除逻辑
//
// 指针图 — 删除中间节点 (val=5):
//
//   dummy -> [4] -> [5] -> [1] -> [9] -> null
//             ^      ^
//            prev   cur    cur->val == 5!
//
//   prev->next = cur->next:
//
//   dummy -> [4] ---------> [1] -> [9] -> null
//
//   返回 dummy.next = [4]->[1]->[9]
//
// 指针图 — 删除头节点 (val=4):
//
//   dummy -> [4] -> [5] -> [1] -> [9] -> null
//     ^       ^
//    prev    cur    cur->val == 4!
//
//   prev->next = cur->next:
//
//   dummy ---------> [5] -> [1] -> [9] -> null
//
//   返回 dummy.next = [5]->[1]->[9]  (新头节点)
//
// 指针图 — 删除尾节点 (val=9):
//
//   dummy -> [4] -> [5] -> [1] -> [9] -> null
//                            ^      ^
//                           prev   cur   cur->val == 9!
//
//   prev->next = cur->next = null:
//
//   dummy -> [4] -> [5] -> [1] -> null
//
//   返回 dummy.next = [4]->[5]->[1]
// ============================================================
class Solution_Dummy {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        // 创建哨兵节点，指向原始 head
        ListNode dummy(0);
        dummy.next = head;

        // prev 从 dummy 开始, cur 从 head 开始
        ListNode* prev = &dummy;
        ListNode* cur = head;

        while (cur != nullptr) {
            if (cur->val == val) {
                // 找到目标: 前驱跳过当前节点
                prev->next = cur->next;
                break; // 题目保证值唯一
            }
            // 向后移动两个指针
            prev = cur;
            cur = cur->next;
        }

        // 返回 dummy.next 而非 head (head 可能已被删)
        return dummy.next;
    }
};

// ============================================================
// 解法2: 直接处理（头节点特判）
// 时间: O(n)  空间: O(1)
//
// 不用 dummy，手动处理头节点情况
//
// 逻辑:
//   ┌─ head->val == val ?
//   │   Yes -> return head->next
//   │   No  -> 遍历找前驱，修改 next
//   └─
// ============================================================
class Solution_Direct {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        // 特判: 删除头节点
        if (head->val == val) {
            return head->next;
        }

        // 一般情况: 遍历找到目标节点的前驱
        ListNode* prev = head;
        ListNode* cur = head->next;

        while (cur != nullptr) {
            if (cur->val == val) {
                prev->next = cur->next;
                break;
            }
            prev = cur;
            cur = cur->next;
        }

        return head; // head 没变
    }
};

// ============================================================
// 解法3: 单指针法（检查 next 的值）
// 时间: O(n)  空间: O(1)
//
// 只用一个指针，检查 cur->next->val 而非 cur->val
// 这样 cur 自然就是要删除节点的前驱
// ============================================================
class Solution_SinglePtr {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        // 特判: 删除头节点
        if (head->val == val) return head->next;

        ListNode* cur = head;

        // 找到 cur->next->val == val 的节点
        while (cur->next != nullptr && cur->next->val != val) {
            cur = cur->next;
        }

        // cur->next 是要删除的节点（如果找到了）
        if (cur->next != nullptr) {
            cur->next = cur->next->next;
        }

        return head;
    }
};

// ============================================================
// 解法4: 递归法
// 时间: O(n)  空间: O(n) 递归栈
//
// 递归处理链表:
//   deleteNode([4]->..., 5)
//     = [4] -> deleteNode([5]->..., 5)
//                = [1] -> [9] -> null  // 跳过 [5]
//
// 优点: 代码极简
// 缺点: 递归深度 O(n)，长链表可能栈溢出
// ============================================================
class Solution_Recursive {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        // base case: 空链表
        if (head == nullptr) return nullptr;

        // 如果当前节点就是目标 -> 跳过它
        if (head->val == val) {
            return head->next;
        }

        // 否则递归处理后续，挂回当前节点的 next
        head->next = deleteNode(head->next, val);
        return head;
    }
};

// ============================================================
// 解法对比分析
// ============================================================
/*
 * | 解法         | 时间 | 空间 | 头节点处理  | 代码量  | 面试推荐 |
 * |-------------|------|------|------------|---------|---------|
 * | 哨兵+双指针  | O(n) | O(1) | 自动统一   | 适中    | ⭐ 首选 |
 * | 直接处理     | O(n) | O(1) | 需要特判   | 适中    | 可以    |
 * | 单指针       | O(n) | O(1) | 需要特判   | 最短    | 简洁    |
 * | 递归         | O(n) | O(n) | 自动处理   | 最短    | 了解    |
 *
 * 面试建议:
 * - 优先写解法1（哨兵节点），展示链表处理的标准技巧
 * - 提到 dummy 节点可以消除头节点特判（加分）
 * - 分析边界: 空链表、单节点、头节点、尾节点
 *
 * 哨兵节点的核心价值:
 *
 *   不用 dummy:                   用 dummy:
 *   ┌── head 是目标？──┐          dummy -> head -> ... -> null
 *   │ Yes: return head->next │    统一处理，无需分支
 *   │ No:  遍历+删除        │
 *   └──────────────────────┘
 *
 *   多了一个分支 = 多了一个 bug 的机会
 */

// ============================================================
// 面试 Follow-up
// ============================================================
/*
 * Q1: 如果有多个节点值相同，要全部删除？
 * A1: 不用 break，继续遍历:
 *     if (cur->val == val) {
 *         prev->next = cur->next;
 *         cur = cur->next; // 不移动 prev
 *     } else {
 *         prev = cur; cur = cur->next;
 *     }
 *
 * Q2: 如果给的是节点指针而非值？(LC 237)
 * A2: 把下一个节点的值拷贝到当前，然后删除下一个:
 *     node->val = node->next->val;
 *     node->next = node->next->next;
 *
 * Q3: 如果是双向链表？
 * A3: O(1) 删除: node->prev->next = node->next;
 *                  node->next->prev = node->prev;
 *
 * Q4: 需要释放被删除节点的内存吗？
 * A4: 本题不需要。实际工程中裸指针要 delete，
 *     智能指针/GC语言自动处理。
 *
 * Q5: 能否不遍历，O(1) 删除？
 * A5: 单链表给定值: 不能，必须找到前驱。
 *     单链表给定指针(非尾节点): LC 237 的覆盖法 O(1)。
 *     双链表给定指针: O(1)。
 */

// ============================================================
// 使用 Solution_Dummy 作为默认解法提交
// ============================================================
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        ListNode* cur = head;
        while (cur) {
            if (cur->val == val) {
                prev->next = cur->next;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        return dummy.next;
    }
};

// ============================================================
// 总结
// ============================================================
/*
 * -------- 解法对比一览 --------
 *
 * | 解法         | 时间 | 空间 | 头节点处理 | 面试推荐 |
 * |-------------|------|------|-----------|---------|
 * | 哨兵+双指针  | O(n) | O(1) | 自动统一  | ⭐首选  |
 * | 直接处理     | O(n) | O(1) | 需要特判  | 可以     |
 * | 单指针       | O(n) | O(1) | 需要特判  | 简洁     |
 * | 递归         | O(n) | O(n) | 自动处理  | 了解     |
 *
 * -------- 易错点 --------
 *
 * 1. 忘记处理删除头节点 -- 不用 dummy 时必须特判 head->val==val
 * 2. 返回值写成 return head 而非 return dummy.next
 *    -- head 可能已被删除，dummy.next 才是真正的新头
 * 3. break 时机 -- 题目保证值唯一，找到后要 break 避免野指针
 * 4. 内存泄漏 -- 面试中提一句"工程上需 delete 被删节点"是加分项
 * 5. 空链表判断 -- 虽然题目保证非空，但健壮代码应判 head==nullptr
 *
 * -------- 面试追问链 --------
 *
 * Q1: 如果有多个相同值节点要全部删除? (LC 203)
 *     -> 不用 break，继续遍历; 注意删除后 prev 不动，只移 cur
 *
 * Q2: 如果给的是节点指针而非值? (LC 237)
 *     -> 覆盖法: node->val = node->next->val;
 *        node->next = node->next->next; O(1) 完成
 *
 * Q3: 如果是双向链表，能否 O(1) 删除?
 *     -> 可以: node->prev->next = node->next;
 *        node->next->prev = node->prev;
 *        无需遍历找前驱，这正是双链表的核心优势
 */
