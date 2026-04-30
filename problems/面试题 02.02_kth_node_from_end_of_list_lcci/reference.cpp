/*
 * 面试题 02.02: 返回倒数第 k 个节点
 * 难度: Easy | 标签: Linked List, Two Pointers
 *
 * 题目: 找出单向链表中倒数第 k 个节点，返回该节点的值。
 *       给定的 k 保证是有效的。
 *
 * 核心模式: 快慢指针（固定间距法）
 */

// ============================================================
// 解法1: 快慢指针 — O(n) / O(1) ⭐ 面试首选
// ============================================================
// 思路: 让 fast 先走 k 步拉开间距，然后 fast 和 slow 同步前进。
//       当 fast 到达 null 时，slow 恰好在倒数第 k 个节点。
//
// 指针图 (链表: 1→2→3→4→5, k=2):
//
// 初始:     s,f
//            ↓
//            1 → 2 → 3 → 4 → 5 → null
//
// fast走2步: s           f
//            ↓           ↓
//            1 → 2 → 3 → 4 → 5 → null
//
// 同步走:
//   Round1:      s           f
//                ↓           ↓
//            1 → 2 → 3 → 4 → 5 → null
//
//   Round2:           s           f
//                     ↓           ↓
//            1 → 2 → 3 → 4 → 5 → null
//
//   Round3:                s           f
//                          ↓           ↓
//            1 → 2 → 3 → 4 → 5 → null
//                          ↑
//                    f==null, slow在4 = 倒数第2个 ✓

class Solution {
public:
    int kthToLast(ListNode* head, int k) {
        ListNode* fast = head;
        ListNode* slow = head;

        // Step 1: fast 先走 k 步，拉开间距
        for (int i = 0; i < k; i++) {
            fast = fast->next;
        }

        // Step 2: 同步前进，直到 fast 到达 null
        while (fast != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        // slow 现在指向倒数第 k 个节点
        return slow->val;
    }
};

/*
 * 为什么是 k 步而不是 k-1 步？
 *
 * 假设链表长度为 n，fast 先走 k 步后:
 *   fast 在第 k+1 个节点 (0-indexed: 位置k)
 *   fast 距离 null 还有 n-k 步
 *
 * 同步走 n-k 步后:
 *   fast 到达 null (走完了)
 *   slow 从 head 走了 n-k 步, 在第 n-k+1 个节点 = 倒数第 k 个 ✓
 *
 * 如果只走 k-1 步:
 *   fast 距离 null 还有 n-k+1 步
 *   slow 最终走 n-k+1 步, 在第 n-k+2 个节点 = 倒数第 k-1 个 ✗
 */


// ============================================================
// 解法2: 两次遍历 — O(n) / O(1)
// ============================================================
// 思路: 第一遍计算链表长度 n，第二遍走 n-k 步到目标节点。
//
// 链表: 1→2→3→4→5, k=2
// 第一遍: n = 5
// 第二遍: 走 5-2=3 步 → head→1→2→3→4 (节点4 = 倒数第2个)

/*
class Solution {
public:
    int kthToLast(ListNode* head, int k) {
        // 第一遍: 计算长度
        int n = 0;
        ListNode* curr = head;
        while (curr) {
            n++;
            curr = curr->next;
        }

        // 第二遍: 走 n-k 步到目标
        curr = head;
        for (int i = 0; i < n - k; i++) {
            curr = curr->next;
        }

        return curr->val;
    }
};
*/


// ============================================================
// 解法3: 递归 (回溯计数) — O(n) / O(n)
// ============================================================
// 思路: 递归到末尾，回溯时计数，数到 k 时就是目标节点。
// 空间 O(n) 来自递归栈，不如快慢指针。

/*
class Solution {
    int count = 0;
    int result = 0;
public:
    int kthToLast(ListNode* head, int k) {
        helper(head, k);
        return result;
    }

    void helper(ListNode* node, int k) {
        if (!node) return;
        helper(node->next, k);   // 递归到末尾
        count++;                  // 回溯计数: 1=最后一个, 2=倒数第二...
        if (count == k) {
            result = node->val;   // 找到倒数第 k 个
        }
    }
};
*/

/*
 * 递归执行过程 (链表: 1→2→3→4→5, k=2):
 *
 * 递归展开:
 * helper(1) → helper(2) → helper(3) → helper(4) → helper(5) → helper(null)
 *                                                                 ↓ return
 * 回溯:                                                count=1 (节点5)
 *                                               count=2 (节点4) ← k==2, result=4 ✓
 *                                        count=3 (节点3)
 *                                 count=4 (节点2)
 *                          count=5 (节点1)
 */


// ============================================================
// 解法4: 栈 — O(n) / O(n)
// ============================================================
// 思路: 全部压栈，弹出 k 次，最后弹出的就是倒数第 k 个。

/*
class Solution {
public:
    int kthToLast(ListNode* head, int k) {
        stack<int> stk;
        while (head) {
            stk.push(head->val);
            head = head->next;
        }
        // 弹出 k-1 次 (丢弃倒数第1到第k-1个)
        for (int i = 1; i < k; i++) {
            stk.pop();
        }
        // 栈顶就是倒数第 k 个
        return stk.top();
    }
};
*/


// ============================================================
// 扩展: 删除倒数第 k 个节点 (LeetCode 19 的核心)
// ============================================================
// 需要找到倒数第 k+1 个节点 (目标的前一个)
// 技巧: 使用 dummy head，让 slow 从 dummy 开始

/*
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;

    // fast 先走 n+1 步 (比本题多走1步)
    for (int i = 0; i <= n; i++) {
        fast = fast->next;
    }

    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    // slow 在目标节点的前一个, 执行删除
    ListNode* toDelete = slow->next;
    slow->next = slow->next->next;
    delete toDelete;

    return dummy.next;
}
*/


// ============================================================
// 常见错误示范
// ============================================================

// 错误1: fast 只走 k-1 步
// for (int i = 0; i < k-1; i++) fast = fast->next;
// → slow 最终停在倒数第 k+1 个位置 (差了一步)

// 错误2: 循环条件用 fast->next 而非 fast
// while (fast->next) { ... }
// → fast 在最后一个节点就停了，slow 也少走一步
// → slow 停在倒数第 k+1 个位置

// 错误3: 同步移动时只移了 fast 忘了 slow
// while (fast) { fast = fast->next; }  // 忘了 slow = slow->next;
// → slow 一直在 head，返回第一个节点


// ============================================================
// 【解法对比】
// ============================================================
// 快慢指针 O(n)/O(1) — 一次遍历，空间最优，面试首选
// 两次遍历   O(n)/O(1) — 代码最直观，但需两遍扫描
// 递归回溯   O(n)/O(n) — 递归栈占 O(n)，不如快慢指针
// 栈         O(n)/O(n) — 全部压栈再弹出，浪费空间
//
// 选用场景:
//   面试/生产代码 → 快慢指针（一次遍历 + O(1)空间）
//   快速实现/可读性优先 → 两次遍历
//   链表不能二次遍历（流式数据）→ 快慢指针是唯一选择

// ============================================================
// 【易错点】
// ============================================================
// ✗ for (int i = 0; i < k-1; i++) fast = fast->next;  // fast 只走 k-1 步
// ✓ for (int i = 0; i < k; i++) fast = fast->next;    // fast 必须走 k 步
//
// ✗ while (fast->next) { fast = fast->next; slow = slow->next; }
// ✓ while (fast) { fast = fast->next; slow = slow->next; }
//   用 fast->next 做条件 → slow 少走一步，停在倒数第 k+1 个
//
// ✗ 删除倒数第 k 个时 fast 先走 k 步（和查找一样）
// ✓ 删除时 fast 先走 k+1 步，slow 从 dummy 出发 → slow 停在目标前一个

// ============================================================
// 【面试追问 Interview Follow-ups】
// ============================================================
// Q1: 如果 k 可能大于链表长度怎么办？
// A1: fast 先走 k 步时检测是否提前到 null。若 fast==null 且还没走够 k 步，
//     返回 -1 或抛出异常。也可先遍历求长度 n，若 k>n 直接返回无效。
//
// Q2: 如何把"查找"改为"删除"倒数第 k 个节点？(LeetCode 19)
// A2: 引入 dummy head，让 slow 从 dummy 出发，fast 先走 k+1 步。
//     当 fast==null 时 slow 恰好在目标节点的前一个，执行
//     slow->next = slow->next->next 即可删除。dummy head 处理头节点被删的边界。
//
// Q3: 如果是双向链表，有没有更快的方法？
// A3: 直接从 tail 向前走 k-1 步即可到达倒数第 k 个节点，无需快慢指针。
//     时间仍为 O(k)，但常数更优。如果 k 很小（如 k=1），比快慢指针快很多。
