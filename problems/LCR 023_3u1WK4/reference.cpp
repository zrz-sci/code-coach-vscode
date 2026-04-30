/*
 * LCR 023: 相交链表 (同主站 160. Intersection of Two Linked Lists)
 * 难度: Easy
 *
 * 题目: 给定两个单链表 headA 和 headB, 找出相交的起始节点
 * 进阶: O(m+n) 时间, O(1) 空间
 *
 * 核心思想: 双指针法 — pA 走完 A 接着走 B, pB 走完 B 接着走 A
 *           两者走过的总路程相同 (a+c+b == b+c+a), 必在交点相遇
 */

// 链表节点定义

// =====================================================
// 解法一: 双指针法 (最优解, 推荐)
// =====================================================
// 思路: 两个指针各自走完自己的链表后, 切换到对方的链表继续走
//       走过的总距离相同, 在交点相遇 (或同时到达 null)
// 时间: O(m + n)  空间: O(1)
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *pA = headA;
        ListNode *pB = headB;

        // 当 pA == pB 时退出:
        //   - 有交点: 在交点相遇
        //   - 无交点: 同时到达 null
        while (pA != pB) {
            // pA 走到 null 后切换到 headB, 否则继续走
            pA = pA ? pA->next : headB;
            // pB 走到 null 后切换到 headA, 否则继续走
            pB = pB ? pB->next : headA;
        }

        return pA;  // 交点节点, 或 null (无交点)
    }
};

/*
 * 指针移动图解:
 *
 * A: [4] → [1] → [8] → [4] → [5] → null       (lenA=5, a=2, c=3)
 *                  ↑ 交点
 * B: [5] → [6] → [1] → [8] → [4] → [5] → null (lenB=6, b=3, c=3)
 *
 * pA 路径: 4→1→8→4→5→null→[切headB]→5→6→1→[8]  总步数: 5+1+3+1 = a+c+b+1
 * pB 路径: 5→6→1→8→4→5→null→[切headA]→4→1→[8]  总步数: 6+1+2+1 = b+c+a+1
 *                                                 a+c+b == b+c+a ✓
 *
 * 无交点情况:
 * A: [1] → [2] → null                    (lenA=2)
 * B: [3] → null                           (lenB=1)
 *
 * pA 路径: 1→2→null→[切headB]→3→null     总: 2+1+1+1 = lenA + lenB + 1
 * pB 路径: 3→null→[切headA]→1→2→null     总: 1+1+2+1 = lenB + lenA + 1
 *                                          同时到 null, 返回 null ✓
 */


// =====================================================
// 解法二: 长度差法
// =====================================================
// 思路: 先求两链表长度, 让长链表先走差值步, 然后同时走到交点
// 时间: O(m + n)  空间: O(1)
class Solution2 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // Step 1: 计算两条链表的长度
        int lenA = getLength(headA);
        int lenB = getLength(headB);

        // Step 2: 让较长的链表先走 |lenA - lenB| 步
        ListNode *pA = headA, *pB = headB;
        if (lenA > lenB) {
            for (int i = 0; i < lenA - lenB; i++) pA = pA->next;
        } else {
            for (int i = 0; i < lenB - lenA; i++) pB = pB->next;
        }

        // Step 3: 同时走, 找到第一个相同的节点
        while (pA != pB) {
            pA = pA->next;
            pB = pB->next;
        }

        return pA;  // 交点或 null
    }

private:
    int getLength(ListNode *head) {
        int len = 0;
        while (head) {
            len++;
            head = head->next;
        }
        return len;
    }
};

/*
 * 长度差法图解:
 *
 * A: [4] → [1] → [8] → [4] → [5]          lenA = 5
 *                  ↑ 交点
 * B: [5] → [6] → [1] → [8] → [4] → [5]   lenB = 6
 *
 * 差值 = 6 - 5 = 1, 让 pB 先走 1 步
 *
 * pB 先走:  [5] → pB现在在[6]
 *
 * 然后同时走:
 *   pA=[4]  pB=[6]  → 不同
 *   pA=[1]  pB=[1]  → 不同 (不同节点对象, 只是值相同)
 *   pA=[8]  pB=[8]  → 相同! (同一个节点对象) → 返回
 */


// =====================================================
// 解法三: 哈希集合法
// =====================================================
// 思路: 把链表 A 的所有节点存入集合, 遍历 B 找第一个在集合中的
// 时间: O(m + n)  空间: O(m) — 不满足 O(1) 空间要求
class Solution3 {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        unordered_set<ListNode*> visited;

        // 遍历链表 A, 存储所有节点的指针
        ListNode *curr = headA;
        while (curr) {
            visited.insert(curr);
            curr = curr->next;
        }

        // 遍历链表 B, 找第一个在集合中的节点
        curr = headB;
        while (curr) {
            if (visited.count(curr)) {
                return curr;  // 找到交点
            }
            curr = curr->next;
        }

        return nullptr;  // 无交点
    }
};

/*
 * 哈希法图解:
 *
 * 遍历 A, 存入 HashSet:
 * visited = {ptr(4), ptr(1), ptr(8), ptr(4'), ptr(5)}
 *
 * 遍历 B:
 *   [5] → 不在 visited
 *   [6] → 不在 visited
 *   [1] → 不在 visited (注意: 这是 B 的 [1], 不是 A 的 [1])
 *   [8] → 在 visited 中! (这是共享节点) → 返回 ptr(8)
 */


// =====================================================
// 常见错误示范
// =====================================================
/*
 * 错误1: 切换逻辑写错
 *
 * // 错误: 最后一个节点时就切换, 跳过了 null 状态
 * pA = pA->next ? pA->next : headB;
 *
 * // 正确: 到 null 后再切换
 * pA = pA ? pA->next : headB;
 *
 * 区别: 正确写法经过 null 这一"中间状态", 保证无交点时同时到 null
 *
 * 错误2: 比较值而非节点地址
 *
 * // 错误: 值相同不代表是同一节点
 * while (pA->val != pB->val) { ... }
 *
 * // 正确: 比较指针
 * while (pA != pB) { ... }
 */


// =====================================================
// 复杂度总结
// =====================================================
/*
 * 解法一 (双指针):   时间 O(m+n), 空间 O(1), 最优
 * 解法二 (长度差):   时间 O(m+n), 空间 O(1), 两次遍历
 * 解法三 (哈希集合): 时间 O(m+n), 空间 O(m), 简单但浪费空间
 *
 * 推荐: 解法一, 代码最简洁且满足进阶要求
 *
 * 关联题目:
 *   141. Linked List Cycle — 快慢指针判环
 *   142. Linked List Cycle II — 快慢指针找环入口
 *   21.  Merge Two Sorted Lists — 双指针合并链表
 */

// ============================================================
// 【总结】
//
// ——— 解法对比 ———
// | 解法              | 时间    | 空间  | 特点                  |
// |------------------|---------|------|-----------------------|
// | 双指针(切换链表)   | O(m+n) | O(1) | 最优解，代码最简 ⭐     |
// | 长度差法           | O(m+n) | O(1) | 直觉清晰，两次遍历      |
// | 哈希集合           | O(m+n) | O(m) | 最简单但不满足O(1)空间  |
//
// 双指针法的精髓: a+c+b == b+c+a，路程抹平长度差后必在交点相遇。
// 长度差法更直观：先算差，长链表先走差值步，再同步走。
//
// ——— 易错点 ———
// 1. 切换时机: pA = pA ? pA->next : headB（到null再切换）
//    不是 pA = pA->next ? pA->next : headB（跳过null态→无交点时死循环）
// 2. 比较指针不是比较值: while(pA != pB) 比较的是地址
//    值相同的不同节点不是交点！
// 3. 无交点保证终止: 两者都走 m+n+1 步后同时到 null，循环自然退出
//
// ——— 面试追问 ———
// Q1: 如果链表可能有环怎么办？
//     → 先用快慢指针 (LC141) 检测两条链表是否有环。
//       若一条有环一条无环，不可能相交。
//       若都有环，找环入口 (LC142)，判断入口是否在同一个环上。
//
// Q2: 能否不修改链表结构、不用额外空间地找到交点？
//     → 双指针法天然满足：不修改 next 指针，O(1) 空间。
//       长度差法也满足。哈希法不满足空间要求。
//
// Q3: 如果要求返回倒数第 k 个公共节点（不只是交点）怎么做？
//     → 先找到交点，计算公共部分长度 c。
//       如果 k <= c，从交点走 c-k 步即可。
//       如果 k > c，不存在。
// ============================================================
