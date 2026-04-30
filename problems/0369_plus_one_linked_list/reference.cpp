// ============================================================
// 369. Plus One Linked List
// ============================================================
// 标签: Linked List, Math
// 难度: Medium
//
// 【题目本质】
//   单链表表示整数（高位在头），加1后返回。
//   核心难点: 进位从低位向高位传播，但单链表只能正向遍历。
//   关键: 找到最右的非9节点 -> 那是进位停止的地方。
//
// 【解法总览】
//   解法1: 找最右非9节点 -- 哨兵+单次遍历, O(1)空间
//   解法2: 递归 -- 回溯处理进位, O(n)栈空间
// ============================================================


// ============================================================
// 解法1: 找最右非9节点 + 哨兵 (推荐)
//
// 算法:
//   1. 创建 dummy(0) -> head
//   2. 遍历链表，notNine 记录最右的非9节点
//   3. notNine->val++
//   4. notNine 之后所有节点 val = 0
//   5. 返回: dummy->val != 0 ? dummy : dummy->next
//
// 核心洞察:
//   加1只影响从尾部开始的连续9链 + 紧挨其前的非9节点。
//   例: 1->2->9->9 +1 = 1->3->0->0
//       notNine指向2, 2+1=3, 后面9全变0
//
// 过程可视化:
//   输入: 9 -> 9 -> 9
//   dummy(0) -> 9 -> 9 -> 9
//   notNine = dummy (全是9, 从未更新)
//   dummy->val++ = 1, 后续: 0->0->0
//   结果: 1 -> 0 -> 0 -> 0
//
//   输入: 1 -> 2 -> 3
//   dummy(0) -> 1 -> 2 -> 3
//   notNine -> 3 (最右非9)
//   3->val++ = 4, 后续无节点
//   结果: 1 -> 2 -> 4
//
// 时间: O(n)
// 空间: O(1)
// ============================================================
class Solution {
public:
    ListNode* plusOne(ListNode* head) {
        // 哨兵节点处理可能的新头
        ListNode* dummy = new ListNode(0);
        dummy->next = head;

        // 找最右的非9节点
        ListNode* notNine = dummy;
        ListNode* curr = head;
        while (curr) {
            if (curr->val != 9) {
                notNine = curr;
            }
            curr = curr->next;
        }

        // 进位操作
        notNine->val++;                // 非9节点 +1

        // notNine 之后所有节点(都是9)置0
        curr = notNine->next;
        while (curr) {
            curr->val = 0;
            curr = curr->next;
        }

        // 如果 dummy 被进位 -> 全是9的情况
        return dummy->val != 0 ? dummy : dummy->next;
    }
};


// ============================================================
// 解法2: 递归法
//
// 思路:
//   递归到链表尾部，回溯时处理进位。
//   carry 从尾部传回头部。
//
// 递归函数 addOne(node):
//   if node == nullptr: return 1  (加1的进位)
//   carry = addOne(node->next)    (递归处理后续)
//   sum = node->val + carry
//   node->val = sum % 10
//   return sum / 10               (进位传给前一个节点)
//
// 过程可视化 (1->2->9):
//   addOne(1): carry = addOne(2)
//     addOne(2): carry = addOne(9)
//       addOne(9): carry = addOne(null) = 1
//         sum = 9+1=10, val=0, return 1
//       sum = 2+1=3, val=3, return 0
//     sum = 1+0=1, val=1, return 0
//   最终 carry=0, 无需新头
//   结果: 1 -> 3 -> 0
//
// 时间: O(n)
// 空间: O(n) -- 递归栈
// ============================================================
class Solution2 {
public:
    ListNode* plusOne(ListNode* head) {
        int carry = addOne(head);
        if (carry) {
            // 还有进位: 创建新头节点
            ListNode* newHead = new ListNode(1);
            newHead->next = head;
            return newHead;
        }
        return head;
    }

private:
    int addOne(ListNode* node) {
        if (!node) return 1;  // 加1的初始进位

        int carry = addOne(node->next);
        int sum = node->val + carry;
        node->val = sum % 10;
        return sum / 10;
    }
};


// ============================================================
// 【解法对比】
//   解法1 (最右非9): O(n) 时间, O(1) 空间 -- 最优, 面试首选
//   解法2 (递归):    O(n) 时间, O(n) 空间 -- 简洁但费栈空间
//
// 变体: 反转链表法
//   反转 -> 从头加1处理进位 -> 再反转
//   O(n) 时间, O(1) 空间, 但需要3次遍历
//   适用于加任意数(不只是加1)
//
// 【易错点】
//   1. 忘记 dummy 节点: 全是9时需要新头，没有 dummy 要特判。
//   2. 找最左非9而非最右非9: 进位从右传播，要找最右非9。
//   3. 忘记将 notNine 后面的节点置0: 9+进位=10, 保留0。
//   4. 递归法忘记处理最终进位: 如果 head 返回的 carry=1，
//      需要创建新节点。
//   5. 返回值搞错: 不是永远返回 head，全9时要返回 dummy。
//
// 【面试追问】
//   Q1: 能否用栈代替递归?
//       → 可以。将节点压栈，然后逐个弹出处理进位。
//         时间 O(n), 空间 O(n), 本质和递归一样。
//
//   Q2: 加 k 而非加 1 怎么做?
//       → "最右非9"技巧不再适用。需要反转链表或用栈，
//         逐位处理 digit + k%10 + carry, k /= 10。
//
//   Q3: 如果是双向链表呢?
//       → 直接从尾部开始向前遍历处理进位即可，O(n)时间O(1)空间，
//         无需任何技巧。单链表的难点在于只能正向遍历。
// ============================================================
