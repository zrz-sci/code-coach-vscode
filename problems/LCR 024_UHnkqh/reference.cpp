// ============================================================
// LCR 024: 反转链表 (同主站 206. Reverse Linked List)
// 难度: Easy | 标签: Recursion, Linked List
//
// 题意: 给定单链表头节点 head, 反转链表并返回新头节点。
//
// 指针变化图 (head = [1,2,3,4,5]):
//
// 迭代过程:
//   初始: null  1 -> 2 -> 3 -> 4 -> 5 -> null
//          prev curr
//
//   S1:   null <- 1    2 -> 3 -> 4 -> 5 -> null
//                prev curr
//
//   S2:   null <- 1 <- 2    3 -> 4 -> 5 -> null
//                      prev curr
//
//   S3:   null <- 1 <- 2 <- 3    4 -> 5 -> null
//                           prev curr
//
//   S4:   null <- 1 <- 2 <- 3 <- 4    5 -> null
//                                prev curr
//
//   S5:   null <- 1 <- 2 <- 3 <- 4 <- 5    null
//                                     prev  curr
//
//   返回 prev = 5 (新头节点)
// ============================================================


// ============================================================
// 解法1: 迭代（三指针） ⭐ 推荐
//
// 核心四步操作 (每步都不能少):
//   1. next = curr->next     暂存下一个节点
//   2. curr->next = prev     翻转: 当前指向前一个
//   3. prev = curr           prev 右移
//   4. curr = next           curr 右移
//
// 时间: O(n) | 空间: O(1)
// ============================================================
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;  // 反转后的"前一个节点", 初始为 null
        ListNode* curr = head;     // 当前要处理的节点

        while (curr != nullptr) {
            ListNode* next = curr->next;  // Step 1: 暂存下一个
            curr->next = prev;            // Step 2: 翻转指针
            prev = curr;                  // Step 3: prev 前进
            curr = next;                  // Step 4: curr 前进
        }

        return prev;  // prev 是新的头节点
    }
};


// ============================================================
// 解法2: 递归
//
// 递归示意 (head = [1,2,3,4,5]):
//
//   reverseList(1)
//     reverseList(2)
//       reverseList(3)
//         reverseList(4)
//           reverseList(5) → return 5 (base case)
//         // 回溯: 4->next(5)->next = 4, 4->next = null
//         // 状态: null<-4<-5, newHead=5
//       // 回溯: 3->next(4)->next = 3, 3->next = null
//       // 状态: null<-3<-4<-5, newHead=5
//     // 回溯: 2->next(3)->next = 2, 2->next = null
//     // 状态: null<-2<-3<-4<-5, newHead=5
//   // 回溯: 1->next(2)->next = 1, 1->next = null
//   // 状态: null<-1<-2<-3<-4<-5, newHead=5
//   return 5
//
// 关键操作:
//   head->next->next = head  让"下一个"反指自己
//   head->next = nullptr     断开原方向, 防止成环
//
// 时间: O(n) | 空间: O(n) 递归栈
// ============================================================
class Solution2 {
public:
    ListNode* reverseList(ListNode* head) {
        // 终止条件: 空链表或单节点
        if (!head || !head->next) return head;

        // 递归反转后面的部分
        ListNode* newHead = reverseList(head->next);

        // 关键: 让下一个节点反过来指向自己
        head->next->next = head;
        // 断开原来的方向 (最终第一个节点的 next 应为 null)
        head->next = nullptr;

        return newHead;  // 新头节点一路传递回去
    }
};


// ============================================================
// 解法3: 尾递归版
//
// 思路:
//   将迭代法的 prev/curr 作为参数传递
//   每次递归处理一个节点, 不需要回溯
//   某些编译器可以优化为循环, 避免栈溢出
//
// 时间: O(n) | 空间: O(n) 最坏, O(1) 若尾递归优化
// ============================================================
class Solution3 {
public:
    ListNode* reverseList(ListNode* head) {
        return helper(head, nullptr);
    }

private:
    ListNode* helper(ListNode* curr, ListNode* prev) {
        if (!curr) return prev;         // 到达末尾, prev 是新头
        ListNode* next = curr->next;    // 暂存
        curr->next = prev;              // 翻转
        return helper(next, curr);      // 尾递归: 不需要回溯
    }
};


// ============================================================
// 解法4: 头插法
//
// 思路:
//   创建虚拟头节点 dummy
//   遍历原链表, 每次把当前节点插入到 dummy 后面
//   效果: 先遍历到的在后面, 后遍历到的在前面 → 自然反转
//
// 过程:
//   dummy->null,  curr=1->2->3->4->5
//   dummy->1->null,  curr=2->3->4->5
//   dummy->2->1->null,  curr=3->4->5
//   dummy->3->2->1->null,  curr=4->5
//   dummy->4->3->2->1->null,  curr=5
//   dummy->5->4->3->2->1->null
//
// 时间: O(n) | 空间: O(1)
// ============================================================
class Solution4 {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode dummy(0);
        ListNode* curr = head;

        while (curr) {
            ListNode* next = curr->next;  // 暂存
            curr->next = dummy.next;      // 当前节点指向 dummy 后面
            dummy.next = curr;            // dummy->当前节点
            curr = next;                  // 继续下一个
        }

        return dummy.next;
    }
};


// ============================================================
// 解法5: 使用栈 (辅助理解, 不推荐面试使用)
//
// 思路:
//   把所有节点压栈, 再依次弹出拼接
//   空间 O(n), 仅用于理解"反转"的本质
//
// 时间: O(n) | 空间: O(n)
// ============================================================
class Solution5 {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head) return nullptr;

        stack<ListNode*> stk;
        while (head) {
            stk.push(head);
            head = head->next;
        }

        ListNode* newHead = stk.top();
        ListNode* curr = newHead;
        stk.pop();

        while (!stk.empty()) {
            curr->next = stk.top();
            stk.pop();
            curr = curr->next;
        }
        curr->next = nullptr;  // 断尾

        return newHead;
    }
};


/*
 * ============================================================
 * 易错点总结:
 *
 * 1. 返回 curr 而非 prev:
 *    循环结束时 curr == nullptr, prev 才是新头节点
 *
 * 2. 递归中忘记 head->next = nullptr:
 *    原链表首节点变为新链尾, 其 next 必须为 null
 *    否则与第二个节点形成双向环
 *
 * 3. 递归中搞混操作对象:
 *    head->next->next = head (对!)  让下一个反指自己
 *    head->next = head (错!)        自己指向自己, 形成自环
 *
 * 4. next 在翻转之后赋值:
 *    curr->next = prev 之后, curr->next 已变
 *    再取 curr->next 就取到 prev 了!
 *    必须先保存 next = curr->next, 再翻转
 *
 * 5. 忘记处理空链表:
 *    head == nullptr 时不能解引用 head->next
 *
 * 面试追问:
 * Q: 迭代 vs 递归? → 迭代 O(1)空间, 递归 O(n)栈
 * Q: 反转部分链表? → LC 92, 需要找连接点
 * Q: K组反转? → LC 25, 每K个一组调用反转
 * Q: 双向链表? → 更简单, 交换 prev/next 即可
 * ============================================================
 */


/* ============================================================
 * 📋 总结区块
 * ============================================================
 *
 * 一、解法对比
 * ┌──────────────┬──────┬────────────────┬──────────────────────────┐
 * │ 解法         │ 时间 │ 空间           │ 说明                     │
 * ├──────────────┼──────┼────────────────┼──────────────────────────┤
 * │ 1.迭代三指针 │ O(n) │ O(1)           │ 面试首选，空间最优       │
 * │ 2.递归       │ O(n) │ O(n) 递归栈    │ 代码简洁但栈溢出风险     │
 * │ 3.尾递归     │ O(n) │ O(n)/O(1)优化  │ 理论可被编译器优化       │
 * │ 4.头插法     │ O(n) │ O(1)           │ 思路新颖，面试区分度高   │
 * │ 5.栈辅助     │ O(n) │ O(n)           │ 仅用于理解，不推荐面试用 │
 * └──────────────┴──────┴────────────────┴──────────────────────────┘
 * 面试中写迭代法即可拿满分；递归法用于展示递归思维但要能分析栈空间。
 * 头插法是亮点解法，如果面试官问"还有别的方法吗"可以给出。
 *
 * 二、易错点
 * 1. 返回 curr 而非 prev：
 *    循环结束时 curr==nullptr，prev 才指向新头。
 *    这是最高频的低级错误。
 *
 * 2. 忘记先暂存 next：
 *    执行 curr->next = prev 后，原来的 curr->next 已丢失。
 *    必须在翻转前 ListNode* next = curr->next。
 *    顺序错误直接导致链表断裂、死循环。
 *
 * 3. 递归版忘记断尾 head->next = nullptr：
 *    原链表首节点变成新链尾，如果不置 null，
 *    它和第二个节点形成双向指针 → 死循环。
 *
 * 4. 递归版混淆操作对象：
 *    正确: head->next->next = head  (让下一个反指自己)
 *    错误: head->next = head        (自环)
 *
 * 三、面试追问
 * Q1: 迭代 vs 递归各自的优缺点？
 *     → 迭代: O(1)空间，适合超长链表；代码略长但无栈溢出风险。
 *     → 递归: 代码简洁优雅；但 O(n) 栈空间，10^5 节点可能栈溢出。
 *       面试中推荐先写迭代，被问到再补递归。
 *
 * Q2: 如何反转链表的第 m 到第 n 个节点？(LC 92)
 *     → 找到第 m-1 个节点作为连接点(connection)，
 *       对 [m, n] 段做普通反转，最后重新连接前后两段。
 *       关键是记住 connection 和 tail（反转段的原头=新尾）。
 *
 * Q3: K 个一组反转链表？(LC 25)
 *     → 先检查剩余节点是否 >= K，不足则不反转。
 *       对每组 K 个节点调用普通反转，然后连接各组。
 *       可用递归或迭代实现，迭代需维护 groupPrev 指针。
 *
 * ============================================================ */
