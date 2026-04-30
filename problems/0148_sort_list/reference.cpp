/*
 * LeetCode 148: 排序链表 (Sort List)
 *
 * 题目本质：链表上的 O(n log n) 排序。归并排序是链表排序的最佳选择——
 *          不需要随机访问，只需要顺序遍历和指针操作。
 *          Follow-up 要求 O(1) 空间，需要自底向上迭代归并。
 *
 * 解法总览：
 *   Solution1 - 自顶向下归并  O(nlogn)/O(logn)  面试首选，经典分治
 *   Solution2 - 自底向上归并  O(nlogn)/O(1)     满足 Follow-up 的 O(1) 空间要求
 *
 * 指针操作图示 (以 [4,2,1,3] 为例):
 *
 *   原始: 4 -> 2 -> 1 -> 3 -> null
 *
 *   找中点(快慢指针): slow=4->2, fast=2->3->null
 *     slow 停在 2, mid = slow->next = 1
 *     断开: 4->2->null  和  1->3->null
 *
 *   递归左半 [4,2]:
 *     找中点: slow=4, mid=2, 断开: [4] 和 [2]
 *     merge([4],[2]) -> 2->4->null
 *
 *   递归右半 [1,3]:
 *     找中点: slow=1, mid=3, 断开: [1] 和 [3]
 *     merge([1],[3]) -> 1->3->null
 *
 *   merge([2,4],[1,3]):
 *     比较 2 vs 1 -> 取 1
 *     比较 2 vs 3 -> 取 2
 *     比较 4 vs 3 -> 取 3
 *     剩余 4
 *     结果: 1->2->3->4->null
 */

// ============================================================
// 解法1: 自顶向下归并排序 — O(n log n)/O(log n) ⭐面试首选
// ============================================================
// 思考过程：
//   链表不支持随机访问 -> 快排不适合 -> 选择归并排序
//   分三步：(1)快慢指针找中点 (2)递归排序两半 (3)合并有序链表
//   空间 O(log n) 来自递归调用栈深度
class Solution1 {
public:
    ListNode* sortList(ListNode* head) {
        // base case: 空链表或单节点
        if (!head || !head->next) return head;

        // 快慢指针找中点
        // 关键: fast 从 head->next 开始，确保偶数长度时 slow 停在左半末尾
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // 断开链表
        ListNode* mid = slow->next;
        slow->next = nullptr;

        // 递归排序两半，然后合并
        return merge(sortList(head), sortList(mid));
    }

private:
    // 合并两个有序链表（同 LC 21）
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (l1 && l2) {
            if (l1->val <= l2->val) {  // <= 保证稳定性
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

// ============================================================
// 解法2: 自底向上归并排序 — O(n log n)/O(1)
// ============================================================
// 思考过程：
//   解法1 的递归栈占 O(log n) 空间，不满足 Follow-up 的 O(1) 要求。
//   改为迭代：从 size=1 的子链表开始，每轮合并相邻的两个 size 子链表，
//   size 翻倍 (1->2->4->8->...) 直到 size >= 链表长度。
//
//   以 [4,2,1,3,5,6] 为例:
//   sz=1: merge([4],[2])=[2,4], merge([1],[3])=[1,3], merge([5],[6])=[5,6]
//   sz=2: merge([2,4],[1,3])=[1,2,3,4], merge([5,6],[])=[5,6]
//   sz=4: merge([1,2,3,4],[5,6])=[1,2,3,4,5,6]
class Solution2 {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // 计算链表长度
        int length = 0;
        for (ListNode* p = head; p; p = p->next) length++;

        ListNode dummy(0, head);

        // 外层: size 从 1 开始翻倍
        for (int sz = 1; sz < length; sz *= 2) {
            ListNode* prev = &dummy;      // 已处理部分的尾巴
            ListNode* curr = dummy.next;  // 当前待处理的起点

            // 内层: 遍历整个链表，每次取两段 sz 长度合并
            while (curr) {
                ListNode* left = curr;
                ListNode* right = split(left, sz);   // 截取 sz 个，返回剩余头
                curr = split(right, sz);             // 截取 sz 个，curr 指向下一对

                // 合并左右两段
                prev->next = merge(left, right);

                // prev 移到合并后链表的末尾
                while (prev->next) prev = prev->next;
            }
        }
        return dummy.next;
    }

private:
    // 从 head 截取 n 个节点，断开，返回剩余链表的头
    // 例: split([1->2->3->4->5], 2) -> 截取 [1->2]，返回 [3->4->5]
    ListNode* split(ListNode* head, int n) {
        for (int i = 1; head && i < n; i++) {
            head = head->next;
        }
        if (!head) return nullptr;
        ListNode* rest = head->next;
        head->next = nullptr;  // 断开
        return rest;
    }

    // 合并两个有序链表
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 解法      | 时间       | 空间     | 特点                          |
 * |-----------|-----------|----------|-------------------------------|
 * | Solution1 | O(nlogn)  | O(logn)  | 自顶向下，经典分治，面试首选  |
 * | Solution2 | O(nlogn)  | O(1)     | 自底向上，满足 Follow-up      |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. 快慢指针: fast 必须从 head->next 开始
 *    ✗ fast=head -> 偶数长度时 slow 走到右半，左半=完整链表，无限递归
 *    ✓ fast=head->next -> slow 停在左半末尾，正确分割
 *
 * 2. 必须断开链表: slow->next = nullptr
 *    ✗ 忘记断开 -> sortList(head) 仍然排序整个链表，无限递归
 *    ✓ 断开后左半 head~slow 和右半 mid~end 是独立链表
 *
 * 3. split 语义: 返回的是"剩余部分的头"，不是截取出来的部分
 *    left = curr; right = split(left, sz); curr = split(right, sz);
 *
 * 4. merge 用 <= 保证稳定性，用 < 会打乱相同值的相对顺序
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 为什么不用快排?
 *   -> 链表无随机访问，快排 partition 效率低，最坏 O(n^2)
 *   -> 归并排序只需顺序访问，始终 O(nlogn) 且稳定
 *
 * Q2: 如何做到 O(1) 空间?
 *   -> Solution2 自底向上迭代归并，只用固定指针变量
 *   -> 从 size=1 开始，每轮合并相邻 size 子链表，size 翻倍
 *
 * Q3: 归并排序是稳定的吗?
 *   -> 是。merge 时相等元素取前半部分的 (l1->val <= l2->val)，
 *      保持了原始相对顺序。这也是链表排序的一个优势。
 */
