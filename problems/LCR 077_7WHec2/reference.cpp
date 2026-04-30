/*
 * LCR 077: 排序链表 (Sort List) -- 同主站第 148 题
 *
 * 难度: Medium | 标签: Linked List, Two Pointers, Divide and Conquer, Sorting, Merge Sort
 *
 * 【解法一】自顶向下归并排序 -- O(n log n) / O(log n) 递归栈
 *
 * 三步:
 *   1. 快慢指针找中点 (fast 从 head->next 开始, 偶数取左中点)
 *   2. 断开链表 (slow->next = nullptr)
 *   3. 递归排序左右半 + 合并有序链表
 *
 * 指针图示:
 *   原始: 4 -> 2 -> 1 -> 3
 *
 *   找中点:
 *     slow=4, fast=2
 *     slow=2, fast=null(停)
 *     mid = slow->next = 1
 *     断开: 4->2->null | 1->3->null
 *
 *   递归排序:
 *     左半 4->2 => 2->4
 *     右半 1->3 => 1->3
 *
 *   合并: 2->4 + 1->3 => 1->2->3->4  ✓
 */

// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode() : val(0), next(nullptr) {}
//     ListNode(int x) : val(x), next(nullptr) {}
//     ListNode(int x, ListNode *next) : val(x), next(next) {}
// };

class Solution {
public:
    ListNode* sortList(ListNode* head) {
        // ==================== Base Case ====================
        // 空链表或单节点: 天然有序
        if (!head || !head->next) return head;

        // ==================== Step 1: 快慢指针找中点 ====================
        //
        // 为什么 fast = head->next (不是 head)?
        //   偶数长度时:
        //     fast=head     -> slow 停在右半中点 -> 分割不均 -> 可能无限递归!
        //     fast=head->next -> slow 停在左半中点 -> 均匀分割 -> 安全!
        //
        //   例: 1->2->3->4
        //     fast=head->next=2:
        //       slow=1->2, fast=2->4, fast.next=null(停)
        //       slow=2 (左半中点), mid=3
        //       左: 1->2, 右: 3->4 (均匀!)
        //
        ListNode* slow = head;
        ListNode* fast = head->next;  // 关键: 从 head->next 开始!
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // ==================== Step 2: 断开链表 ====================
        //
        // 指针变化:
        //   before: ... -> slow -> mid -> ...
        //   after:  ... -> slow -> null    mid -> ...
        //
        ListNode* mid = slow->next;  // 先取右半起点
        slow->next = nullptr;        // 再断开

        // ==================== Step 3: 递归排序 + 合并 ====================
        ListNode* left = sortList(head);   // 排序左半
        ListNode* right = sortList(mid);   // 排序右半
        return merge(left, right);          // 合并有序链表
    }

private:
    // ==================== 合并两个有序链表 ====================
    //
    // 经典 LC21 做法: dummy 头节点 + 逐个比较
    //
    // 指针图示:
    //   l1: 2 -> 4 -> null
    //   l2: 1 -> 3 -> null
    //
    //   dummy -> null, tail = dummy
    //
    //   Step 1: 1 < 2, dummy->1, l2=3
    //   Step 2: 2 < 3, dummy->1->2, l1=4
    //   Step 3: 3 < 4, dummy->1->2->3, l2=null
    //   Step 4: l2 空, 接上 l1: dummy->1->2->3->4
    //
    //   返回 dummy.next = 1->2->3->4
    //
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);          // 哨兵节点, 简化头节点处理
        ListNode* tail = &dummy;    // tail 始终指向已排好序的尾部

        while (l1 && l2) {
            if (l1->val <= l2->val) {   // <= 保证排序稳定性
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }

        // 将未遍历完的链表直接接到末尾
        tail->next = l1 ? l1 : l2;

        return dummy.next;
    }
};

/*
 * ==================== 解法二: 自底向上归并 -- O(n log n) / O(1) ====================
 *
 * 不使用递归, 真正的常数空间!
 *
 * 思路:
 *   step = 1, 2, 4, 8, ...
 *   每轮将链表切成长度为 step 的段, 两两合并
 *
 * 示例: 4 -> 2 -> 1 -> 3
 *
 *   step=1: [4][2] -> [2,4], [1][3] -> [1,3]
 *           结果: 2->4->1->3
 *
 *   step=2: [2,4][1,3] -> [1,2,3,4]
 *           结果: 1->2->3->4  ✓

class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // Step 1: 计算链表长度
        int n = 0;
        for (ListNode* p = head; p; p = p->next) n++;

        ListNode dummy(0);
        dummy.next = head;

        // Step 2: step 从 1 开始翻倍
        for (int step = 1; step < n; step *= 2) {
            ListNode* tail = &dummy;    // 上一段合并结果的尾
            ListNode* cur = dummy.next; // 当前未处理的起点

            while (cur) {
                // 切出第一段 (长度 step)
                ListNode* head1 = cur;
                for (int i = 1; i < step && cur->next; i++)
                    cur = cur->next;
                ListNode* head2 = cur->next;
                cur->next = nullptr;  // 断开第一段

                // 切出第二段 (长度 step)
                cur = head2;
                for (int i = 1; i < step && cur && cur->next; i++)
                    cur = cur->next;
                ListNode* next = nullptr;
                if (cur) {
                    next = cur->next;
                    cur->next = nullptr;  // 断开第二段
                }

                // 合并两段, 接到 tail 后面
                tail->next = merge(head1, head2);

                // tail 移到合并结果的尾部
                while (tail->next) tail = tail->next;

                cur = next;  // 处理下一对
            }
        }
        return dummy.next;
    }

private:
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1; l1 = l1->next;
            } else {
                tail->next = l2; l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};
*/

/*
 * ==================== 复杂度分析 ====================
 *
 * 自顶向下:
 *   时间: O(n log n) -- 递归 log n 层, 每层遍历 n 个节点
 *   空间: O(log n)   -- 递归栈深度
 *
 * 自底向上:
 *   时间: O(n log n) -- log n 轮合并, 每轮遍历 n 个节点
 *   空间: O(1)       -- 只用常数个指针变量
 *
 * ==================== 关键易错点 ====================
 *
 * 1. 快慢指针初始化:
 *    fast = head->next (不是 head!)
 *    否则偶数长度时 slow 停在右半, 左半只有 1 个节点
 *    两个节点 [1,2] 时: head=1, slow=2, 左半=1->2 (没断开!) -> 无限递归!
 *
 * 2. 先取 mid 再断开:
 *    ListNode* mid = slow->next;  // 先取
 *    slow->next = nullptr;        // 再断
 *    顺序反了 mid 就丢了!
 *
 * 3. 合并的稳定性:
 *    l1->val <= l2->val 用 <= (不是 <)
 *    等值时优先取 l1, 保证稳定排序
 *
 * 4. 空链表/单节点:
 *    if (!head || !head->next) return head;
 *    忘记判断 -> 快慢指针 fast = head->next 空指针崩溃
 *
 * 5. 自底向上的段切割:
 *    切出 step 个节点后必须断开 (cur->next = nullptr)
 *    否则合并时会访问到后面的节点, 导致链表混乱
 *
 * ==================== 排序算法对比 (链表) ====================
 *
 * 归并排序: 最适合链表 (顺序访问, 合并不需额外空间)
 * 快速排序: 可行但 partition 需技巧, 随机 pivot 困难
 * 插入排序: O(n^2), 适合近乎有序的短链表
 * 堆排序:   需要随机访问, 链表上效率差
 */

// ============================================================
// 【总结】
//
// ——— 解法对比 ———
// | 解法              | 时间       | 空间      | 适用场景              |
// |------------------|-----------|-----------|----------------------|
// | 自顶向下归并       | O(n logn) | O(logn)   | 面试首选，递归直觉清晰  |
// | 自底向上归并       | O(n logn) | O(1)      | 追问O(1)空间时使用     |
//
// 链表排序 = 归并排序（天然适配顺序访问 + 合并零额外空间）。
// 快排对链表不友好（partition 需要随机 pivot，链表做不到 O(1)）。
//
// ——— 易错点 ———
// 1. fast 起点: fast=head→偶数长度无限递归; fast=head->next→安全
// 2. 断链顺序: 先取 mid=slow->next，再 slow->next=nullptr; 反了mid丢失
// 3. 合并稳定性: l1->val <= l2->val 用 <=，等值优先取 l1 保持稳定
// 4. base case: 必须判断 !head || !head->next，否则空指针崩溃
// 5. 自底向上切段后必须断开 next，否则合并越界
//
// ——— 面试追问 ———
// Q1: 能否做到 O(1) 空间？
//     → 自底向上归并。step=1,2,4,...逐轮合并，只用常数个指针。
//       （即解法二，代码更长但空间严格 O(1)）
//
// Q2: 链表能用快排吗？复杂度如何？
//     → 可以。以 head 为 pivot，遍历分成 <pivot 和 >=pivot 两链，
//       递归排序后拼接。平均 O(n logn)，最坏 O(n^2)。
//       缺点：不能随机选 pivot，退化概率更高。
//
// Q3: 如果链表近乎有序，有更快的方法吗？
//     → 链表插入排序 (LC147)。近乎有序时接近 O(n)。
//       或者 "自然归并"：找到已有序的 run 直接合并，
//       对近乎有序的输入趋近 O(n)。
// ============================================================
