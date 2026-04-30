// ============================================================
// LeetCode 92. Reverse Linked List II
// 难度: Medium | 标签: Linked List
// ============================================================

// ------------------------------------------------------------
// 解法1: 头插法 - 一次遍历 (面试首选)
// 时间 O(n), 空间 O(1)
//
// 思考过程:
//   1. dummy 节点统一 left=1 的边界情况
//   2. 找到 prev (left 前一个节点)
//   3. curr 始终指向原始 left 位置 (反转后它到末尾, 不移动)
//   4. 每次把 curr->next 摘出, 插到 prev 后面
//   5. 循环 right-left 次
//
// 指针图 (head=[1,2,3,4,5], left=2, right=4):
//
//   原始:  d -> 1 -> 2 -> 3 -> 4 -> 5
//               p    c
//
//   i=0:   d -> 1 -> 3 -> 2 -> 4 -> 5   (摘3插到prev后)
//               p              c
//
//   i=1:   d -> 1 -> 4 -> 3 -> 2 -> 5   (摘4插到prev后)
//               p                   c
//
//   结果:  1 -> 4 -> 3 -> 2 -> 5
// ------------------------------------------------------------
class Solution1 {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head);     // dummy 节点, 统一边界
        ListNode* prev = &dummy;

        // 定位: 走到 left 前一个位置
        for (int i = 1; i < left; i++)
            prev = prev->next;

        // curr 始终指向原始 left 位置节点
        // 反转完成后, curr 自然到了反转段的末尾
        ListNode* curr = prev->next;

        // 头插法: 每次把 curr->next 摘出, 插到 prev 后面
        for (int i = 0; i < right - left; i++) {
            ListNode* next = curr->next;     // Step1: 取出要移动的节点
            curr->next = next->next;         // Step2: curr 跳过 next
            next->next = prev->next;         // Step3: next 连到反转段头部
            prev->next = next;               // Step4: prev 连到 next
        }

        return dummy.next;
    }
};

// ------------------------------------------------------------
// 解法2: 截断 + 反转 + 拼接 (步骤更清晰)
// 时间 O(n), 空间 O(1)
//
// 思考过程:
//   1. 定位 start(反转段起始) 和 end(反转段结束)
//   2. 截断: end->next = nullptr, 形成独立子链表
//   3. 用标准全链表反转处理子链表
//   4. 拼接: prev->next = 新头(原end), 原start->next = succ
//
// 指针图:
//   原始:  d -> 1 -> [2 -> 3 -> 4] -> 5
//               p     s         e      succ
//
//   截断:  d -> 1    [2 -> 3 -> 4]    5
//
//   反转:  d -> 1    [4 -> 3 -> 2]    5
//
//   拼接:  d -> 1 -> 4 -> 3 -> 2 -> 5
// ------------------------------------------------------------
class Solution2 {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        // 定位 prev
        for (int i = 1; i < left; i++)
            prev = prev->next;

        // 定位 start 和 end
        ListNode* start = prev->next;
        ListNode* end = start;
        for (int i = left; i < right; i++)
            end = end->next;
        ListNode* succ = end->next;  // 反转段后一个节点

        // 截断: 形成独立子链表
        end->next = nullptr;

        // 反转子链表, 拼接回去
        prev->next = reverse(start);    // prev -> 新头(原end)
        start->next = succ;             // start 现在是尾 -> succ

        return dummy.next;
    }

private:
    // 标准全链表反转
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        while (head) {
            ListNode* n = head->next;
            head->next = prev;
            prev = head;
            head = n;
        }
        return prev;
    }
};

// ------------------------------------------------------------
// 解法3: 递归 (了解即可, 不推荐面试使用)
// 时间 O(n), 空间 O(n) 递归栈
//
// 思考过程:
//   1. 当 left > 1, 递归 head->next, left-1, right-1
//      相当于把问题"右移"一步, 直到 left = 1
//   2. 当 left = 1, 变成"反转前 right 个节点"的问题
//   3. reverseN(head, n): 反转前 n 个节点, 返回新头
//      用 successor 记录第 n+1 个节点(反转段的后继)
// ------------------------------------------------------------
class Solution3 {
    ListNode* successor = nullptr;  // 后继节点

    // 反转链表前 n 个节点
    ListNode* reverseN(ListNode* head, int n) {
        if (n == 1) {
            successor = head->next;  // 记录后继
            return head;
        }
        ListNode* last = reverseN(head->next, n - 1);
        head->next->next = head;     // 反转指向
        head->next = successor;      // 连接后继
        return last;
    }

public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if (left == 1) {
            return reverseN(head, right);
        }
        // left > 1: 递归缩小问题范围
        head->next = reverseBetween(head->next, left - 1, right - 1);
        return head;
    }
};

/*
 * ============================================================
 * 解法对比:
 * ============================================================
 * | 解法         | 时间  | 空间  | 遍历次数 | 面试推荐 |
 * |-------------|-------|-------|---------|---------|
 * | 头插法       | O(n)  | O(1)  | 一次    | 首选    |
 * | 截断+拼接    | O(n)  | O(1)  | 两次    | 替代    |
 * | 递归         | O(n)  | O(n)  | 一次    | 不推荐   |
 *
 * 面试策略:
 *   1. 先画图讲清头插法的指针变化
 *   2. curr 不动是关键 (它自动被"推"到末尾)
 *   3. 提及 dummy 节点处理 left=1 的边界
 *
 * ============================================================
 * 易错点:
 * ============================================================
 * 1. 没用 dummy 节点 -> left=1 时头节点被反转, 返回值错误
 *    修复: ListNode dummy(0, head); return dummy.next;
 *
 * 2. 头插法中移动了 curr
 *    错误: curr = curr->next 在循环末尾
 *    curr 始终指向原始 left 位置, 不移动!
 *
 * 3. 循环次数是 right-left (不是 right-left+1)
 *    反转 k 个节点只需 k-1 次操作
 *
 * 4. 截断法忘记 end->next = nullptr
 *    导致 reverse() 不知道在哪停, 反转整个后续链表
 *
 * 5. 头插法指针赋值顺序:
 *    必须先 curr->next = next->next (断开)
 *    再 next->next = prev->next (连新位置)
 *    最后 prev->next = next (完成插入)
 *    顺序错了会丢失节点引用
 *
 * ============================================================
 * 面试追问:
 * ============================================================
 * Q1: 一次遍历? -> Solution1 头插法
 *
 * Q2: 每 k 个反转? -> LC 25, 复用区间反转逻辑
 *
 * Q3: 给定节点值而非位置?
 *     -> 先遍历找位置, 注意值可能重复
 *
 * Q4: 链表有环怎么办?
 *     -> 反转前需用快慢指针检测环, 有环则反转会死循环
 */
