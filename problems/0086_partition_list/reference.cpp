// ============================================================
// LeetCode 86: Partition List (分隔链表)
// 难度: Medium | 标签: Linked List, Two Pointers
//
// 题意: 给定链表 head 和值 x, 将所有 < x 的节点移到 >= x 节点前面,
//       保持各自的相对顺序不变。
//
// 指针变化图 (head = [1,4,3,2,5,2], x = 3):
//
//   原始: 1 -> 4 -> 3 -> 2 -> 5 -> 2 -> null
//
//   分流过程:
//     1 < 3 → less:    [1]           greater: []
//     4 >= 3 → less:   [1]           greater: [4]
//     3 >= 3 → less:   [1]           greater: [4, 3]
//     2 < 3 → less:    [1, 2]        greater: [4, 3]
//     5 >= 3 → less:   [1, 2]        greater: [4, 3, 5]
//     2 < 3 → less:    [1, 2, 2]     greater: [4, 3, 5]
//
//   断尾: greater 尾 -> null   (5->null, 原来 5->2 断开!)
//   拼接: less 尾 -> greater 头 (2->4)
//
//   结果: 1 -> 2 -> 2 -> 4 -> 3 -> 5 -> null
// ============================================================


// ============================================================
// 解法1: 双链表分流 + Dummy 节点 ⭐ 推荐
//
// 核心三步:
//   1. 遍历原链表, < x 的接到 less 链, >= x 的接到 greater 链
//   2. greater 链断尾: greater->next = nullptr (防止成环!)
//   3. 拼接: less 尾 → greater 头
//
// 为什么必须断尾?
//   greater 链最后一个节点的 next 可能还指向 less 链中的某个节点
//   例如上例中节点 5 的 next 原来指向节点 2 (已在 less 链中)
//   不断尾就会: ...5 -> 2 -> ... -> 2 -> 4 -> 3 -> 5 -> 2 (环!)
//
// 时间: O(n) | 空间: O(1) (只创建了两个 dummy, 没有新链表节点)
// ============================================================
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        // 两个 dummy 头节点 (栈上分配, 自动管理内存)
        ListNode lessHead(0), greaterHead(0);
        // 两个尾指针, 用于追加节点
        ListNode* less = &lessHead;
        ListNode* greater = &greaterHead;

        // 遍历原链表, 按值分流
        while (head != nullptr) {
            if (head->val < x) {
                // < x 的节点追加到 less 链
                less->next = head;
                less = less->next;
            } else {
                // >= x 的节点追加到 greater 链
                greater->next = head;
                greater = greater->next;
            }
            head = head->next;
        }

        // 关键! greater 链尾部必须断开, 防止成环
        greater->next = nullptr;

        // 拼接: less 链尾 → greater 链头
        less->next = greaterHead.next;

        // 返回 less 链的真正头节点
        return lessHead.next;
    }
};


// ============================================================
// 解法2: 不使用 Dummy 节点的版本
//
// 思路:
//   手动维护两条链的头尾指针
//   需要处理"链为空时第一个节点"的特殊情况
//   代码更长, 更容易出错 — 不推荐
//
// 目的: 对比展示 dummy 节点的价值
//
// 时间: O(n) | 空间: O(1)
// ============================================================
class Solution2 {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode* lessHead = nullptr;
        ListNode* lessTail = nullptr;
        ListNode* greaterHead = nullptr;
        ListNode* greaterTail = nullptr;

        while (head) {
            ListNode* next = head->next;
            head->next = nullptr; // 断开当前节点

            if (head->val < x) {
                if (!lessHead) {
                    lessHead = lessTail = head;
                } else {
                    lessTail->next = head;
                    lessTail = head;
                }
            } else {
                if (!greaterHead) {
                    greaterHead = greaterTail = head;
                } else {
                    greaterTail->next = head;
                    greaterTail = head;
                }
            }

            head = next;
        }

        // 拼接 (注意 less 链可能为空)
        if (!lessHead) return greaterHead;
        lessTail->next = greaterHead;
        return lessHead;
    }
};


// ============================================================
// 解法3: 三路分区 (扩展: < x, == x, > x)
//
// 思路:
//   创建三条链: less, equal, greater
//   遍历分流后拼接: less → equal → greater
//   这是"荷兰国旗问题"在链表上的实现
//
// 适用: 面试官追问"如果等于 x 的节点也要单独分组"
//
// 时间: O(n) | 空间: O(1)
// ============================================================
class Solution3 {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode lessD(0), eqD(0), greatD(0);
        ListNode* l = &lessD;
        ListNode* e = &eqD;
        ListNode* g = &greatD;

        while (head) {
            if (head->val < x) {
                l->next = head;
                l = l->next;
            } else if (head->val == x) {
                e->next = head;
                e = e->next;
            } else {
                g->next = head;
                g = g->next;
            }
            head = head->next;
        }

        // 断尾
        g->next = nullptr;

        // 拼接: less → equal → greater
        e->next = greatD.next;
        l->next = eqD.next;

        return lessD.next;
    }
};


// ============================================================
// 解法4: 使用 new 分配 Dummy 的版本
//
// 注意: 需要手动 delete 防止内存泄漏
// 面试中不推荐, 但某些代码风格要求指针而非栈对象
//
// 时间: O(n) | 空间: O(1) (忽略 dummy 的 O(1))
// ============================================================
class Solution4 {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode* lessHead = new ListNode(0);
        ListNode* greaterHead = new ListNode(0);
        ListNode* less = lessHead;
        ListNode* greater = greaterHead;

        while (head) {
            if (head->val < x) {
                less->next = head;
                less = less->next;
            } else {
                greater->next = head;
                greater = greater->next;
            }
            head = head->next;
        }

        greater->next = nullptr;
        less->next = greaterHead->next;

        ListNode* result = lessHead->next;

        // 释放 dummy 节点, 防止内存泄漏
        delete lessHead;
        delete greaterHead;

        return result;
    }
};


/*
 * ============================================================
 * 易错点总结:
 *
 * 1. 忘记断尾 (最常见的 BUG):
 *    greater->next = nullptr; 这一行不能少
 *    没有它, greater 链尾可能指回 less 链, 形成环
 *    面试中画图时一定要展示这个细节
 *
 * 2. 判断条件写成 <= 而非 <:
 *    题目要求: 严格小于 x 的放左边, >= x 的放右边
 *    if (head->val <= x) 是错的, 等于 x 的应该放 greater
 *
 * 3. 拼接方向搞反:
 *    应该是 less->next = greaterHead.next (less 在前)
 *    写成 greater->next = lessHead.next 就反了
 *
 * 4. 返回值搞错:
 *    返回 lessHead.next (跳过 dummy)
 *    不是返回 lessHead (会多出一个 val=0 的 dummy)
 *    也不是返回 less (less 指向 less 链的尾部)
 *
 * 5. 忘记考虑空链表:
 *    head=null 时 while 不进入
 *    greater->next = nullptr (greaterHead.next 本来就是 null)
 *    less->next = greaterHead.next = null
 *    返回 lessHead.next = null  ✓ 自然正确
 *
 * 面试追问:
 * Q: "能否原地?" → 已是 O(1) 空间, dummy 只是辅助变量
 * Q: "保持稳定?" → 此方法天然保持相对顺序
 * Q: "三路分区?" → 加一条 equal 链, 见解法3
 * Q: "和快排的 partition 区别?" → 快排不要求稳定, 此题要求
 * Q: "dummy 节点的作用?" → 避免处理空链表/首节点的特殊逻辑
 * ============================================================
 */
