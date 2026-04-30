/*
 * LeetCode 328: 奇偶链表 (Odd Even Linked List)
 *
 * 【题目本质】
 * 把链表按位置奇偶拆成两条链，再拼接起来。
 * 要求 O(1) 额外空间，O(n) 时间。
 *
 * 【解法总览】
 * 解法1: 额外数组法 — O(n) / O(n) — 最直觉，但不满足空间要求
 * 解法2: 双指针原地重排 — O(n) / O(1) — 面试首选，原地修改指针
 */

// Definition for singly-linked list.
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode() : val(0), next(nullptr) {}
//     ListNode(int x) : val(x), next(nullptr) {}
//     ListNode(int x, ListNode *next) : val(x), next(next) {}
// };

// ============================================================
// 解法1: 额外数组法 — 收集值再写回
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最朴素的想法：遍历一遍链表，奇数位的值放一个数组，偶数位的值放另一个数组，
// 然后按"先奇后偶"顺序写回链表。
// 这个方法修改的是节点的值而非指针，不满足 O(1) 空间要求，
// 但能帮助理解题意：本质就是把奇数位聚到前面、偶数位聚到后面。
// ============================================================
class Solution1 {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (!head || !head->next) return head;

        std::vector<int> oddVals, evenVals;
        ListNode* curr = head;
        int idx = 1;

        while (curr) {
            if (idx % 2 == 1)
                oddVals.push_back(curr->val);
            else
                evenVals.push_back(curr->val);
            curr = curr->next;
            idx++;
        }

        // 按先奇后偶写回
        curr = head;
        for (int v : oddVals) {
            curr->val = v;
            curr = curr->next;
        }
        for (int v : evenVals) {
            curr->val = v;
            curr = curr->next;
        }

        return head;
    }
};

// ============================================================
// 解法2: 双指针原地重排 — 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈是 O(n) 额外空间。
// 链表的优势就是通过修改 next 指针来"移动"节点，无需复制数据。
//
// 核心做法：
//   - odd 指针沿奇数位前进，even 指针沿偶数位前进
//   - 每步 odd 跳过 even 连到下一个奇数节点，even 跳过 odd 连到下一个偶数节点
//   - 保存 evenHead，循环结束后把偶数链接到奇数链尾部
//
// 【指针变化图 — 以 [1,2,3,4,5] 为例】
//
//  初始状态:
//    1 → 2 → 3 → 4 → 5 → NULL
//    ↑   ↑
//   odd even              evenHead = node(2)
//
//  第1轮循环:
//    odd->next = even->next  → 1 → 3 (跳过2)
//    odd = odd->next         → odd 移到 3
//    even->next = odd->next  → 2 → 4 (跳过3)
//    even = even->next       → even 移到 4
//
//    状态: 奇数链 1→3→5  偶数链 2→4→5(暂时)
//                   ↑                  ↑
//                  odd               even
//
//  第2轮循环:
//    odd->next = even->next  → 3 → 5 (跳过4)
//    odd = odd->next         → odd 移到 5
//    even->next = odd->next  → 4 → NULL
//    even = even->next       → even = NULL
//
//    状态: 奇数链 1→3→5→NULL  偶数链 2→4→NULL
//                       ↑
//                      odd
//
//  循环结束 (even == NULL):
//    odd->next = evenHead    → 5 → 2
//
//  最终: 1 → 3 → 5 → 2 → 4 → NULL
//
// 【偶数长度 [1,2,3,4] 的情况】
//
//  初始:  1 → 2 → 3 → 4 → NULL
//         ↑   ↑
//        odd even
//
//  第1轮:
//    1→3, odd=3, 2→4, even=4
//
//  第2轮:
//    odd->next = even->next = NULL  → 3→NULL
//    odd = NULL? 不，odd = odd->next = NULL 之前...
//    等等，让我仔细看：
//    odd->next = even->next (4→NULL)  → 3→NULL
//    odd = odd->next = NULL
//    even->next = odd->next  ← 这里 odd 是 NULL，会段错误！
//
//    所以循环条件 even && even->next 在此轮就会停止：
//    第1轮结束后 even = 4, even->next = NULL → 循环不进入第2轮 ✓
//
//    状态: 奇数链 1→3→NULL(暂)  偶数链 2→4→NULL(暂)
//    但注意 3→next 还是指向 4（原始的），需要检查...
//    实际上第1轮中 odd->next = even->next = 3, odd 移到 3,
//    even->next = odd->next = 4, even 移到 4.
//    此时 3->next 仍然是 4(原来的)? 不对，让我重新推导：
//
//    初始: 1→2→3→4→NULL, odd=1, even=2
//    odd->next = even->next = 3  → 1→3  (现在 1 跳过了 2)
//    odd = 3
//    even->next = odd->next = 4  → 2→4  (现在 2 跳过了 3)
//    等等 odd->next 此时是什么？odd=3, 3 原来指向 4, 没改过 → 3→4
//    所以 even->next = 3->next = 4  → 2→4 ✓
//    even = 4
//    
//    现在 even=4, even->next=NULL → 循环结束
//    odd=3, 3->next 还是 4 → 需要在拼接时覆盖
//    odd->next = evenHead = 2
//    → 3→2→4→NULL ✓
//    完整链: 1→3→2→4→NULL ✓
//
// ============================================================
class Solution2 {
public:
    ListNode* oddEvenList(ListNode* head) {
        // 空链表或只有1-2个节点，无需重排
        if (!head || !head->next) return head;

        ListNode* odd = head;           // 奇数链当前尾部
        ListNode* even = head->next;    // 偶数链当前尾部
        ListNode* evenHead = even;      // 偶数链头部 — 必须提前保存！

        // 为什么看 even 而不是 odd？
        // even 在 odd 后面，even 先到达末尾
        // even==NULL → 偶数长度，无更多节点
        // even->next==NULL → 奇数长度，无更多奇数节点
        while (even && even->next) {
            // 奇数节点跳过偶数节点，连接到下一个奇数节点
            odd->next = even->next;
            odd = odd->next;

            // 偶数节点跳过（新的）奇数节点，连接到下一个偶数节点
            even->next = odd->next;
            even = even->next;
        }

        // 关键一步：奇数链尾部 → 偶数链头部
        odd->next = evenHead;

        return head;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1(额外数组) | 解法2(双指针原地) |
// |-----------|----------------|------------------|
// | 时间       | O(n)           | O(n)             |
// | 空间       | O(n)           | O(1)             |
// | 修改方式   | 改节点值        | 改 next 指针      |
// | 满足要求   | ✗              | ✓                |
// | 面试推荐   | 仅做思路铺垫    | ⭐ 首选           |
//
// 解法2 是唯一满足题目 O(1) 空间要求的解法，面试直接写这个。
//
// ============================================================
// 【易错点】
//
// 1. 循环条件写错：
//    ✗ while (odd && odd->next)
//      → 偶数长度时 even 可能为 NULL，循环体内访问 even->next 段错误
//    ✓ while (even && even->next)
//      → even 在 odd 后面，它先到达末尾，用它做条件更安全
//
// 2. 忘记保存 evenHead：
//    ✗ 循环结束后 even 已经在尾部或 NULL，找不到偶数链头
//    ✓ 循环前 ListNode* evenHead = even; 提前保存
//
// 3. 四行操作顺序搞混：
//    ✗ 先 odd = odd->next 再 odd->next = even->next
//      → odd 已经移动了，修改的是新位置的 next，逻辑全错
//    ✓ 先改 next 再移动指针：
//      odd->next = ... → odd = ... → even->next = ... → even = ...
//
// 4. 漏掉空链表/单节点的边界检查：
//    ✗ 直接 head->next 在 head==NULL 时段错误
//    ✓ 开头 if (!head || !head->next) return head;
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 循环条件为什么是 even && even->next，不是 odd？
//   → even 的位置始终 >= odd 的位置（偶数链在奇数链后面一步），
//     even 先触及链表尾部。偶数长度时 even 最终为 NULL，
//     奇数长度时 even->next 最终为 NULL，两种都被正确覆盖。
//
// Q2: 如果改成"按节点值的奇偶"分组呢？
//   → 不能用交替前进，需要遍历判断 val%2，
//     用两个 dummy head 分别收集奇值和偶值节点，最后拼接。
//     本质变成了 LeetCode 86 (Partition List) 的变体。
//
// Q3: 如果扩展成分三组（位置 %3 == 0, 1, 2），代码怎么改？
//   → 维护三个链头和三个当前指针，遍历时按 idx%3 分配，
//     最后依次拼接三条链。框架相同，从两组扩展到三组。
//
// ============================================================
// 【相关题型】
//
// - LeetCode 86 (分隔链表) — 复用"两条链收集+拼接"的框架，
//   区别：86 按值大小分，328 按位置奇偶分。
//
// - LeetCode 206 (反转链表) — 同样是纯指针操作的基本功，
//   可以练习"画指针变化图"的习惯。
//
// - LeetCode 143 (重排链表) — 复用"拆链+拼接"思路，
//   多了"找中点"和"反转后半段"两步。
//
// - LeetCode 725 (分隔链表) — 链表分组的进阶变体，
//   按份数均匀分割，需要计算每份长度。
// ============================================================
