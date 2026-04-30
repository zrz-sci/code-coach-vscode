/*
 * LeetCode 143: 重排链表 (Reorder List)
 *
 * 【题目本质】
 * 把链表重排为"前半段正序 + 后半段逆序"交替穿插的形式。
 * L0→Ln→L1→Ln-1→L2→Ln-2→...
 *
 * 【解法总览】
 * 解法1: 数组+双指针      — O(n) / O(n) — 最直觉，转化为数组随机访问
 * 解法2: 找中点+反转+合并  — O(n) / O(1) — 面试首选，三个经典操作组合
 * 解法3: 递归              — O(n) / O(n) — 利用递归回溯从尾部取节点
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

// ============================================================
// 解法1: 数组+双指针 — 转化为随机访问问题
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 单链表的痛点是无法从尾部访问节点。
// 把所有节点存入数组后，就能用左右双指针从两端交替取节点重连。
//
// 示意:
//   数组: [1, 2, 3, 4, 5]
//          ↑left      ↑right
//   取1,取5 → 1→5
//          ↑left  ↑right
//   取2,取4 → 1→5→2→4
//             ↑left=right
//   取3     → 1→5→2→4→3
// ============================================================
class Solution1 {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        
        // 存入数组获得随机访问能力
        vector<ListNode*> nodes;
        ListNode* curr = head;
        while (curr) {
            nodes.push_back(curr);
            curr = curr->next;
        }
        
        int left = 0, right = (int)nodes.size() - 1;
        while (left < right) {
            nodes[left]->next = nodes[right];  // 前指向后
            left++;
            if (left == right) break;          // 偶数长度时在此退出
            nodes[right]->next = nodes[left];  // 后指向下一个前
            right--;
        }
        // 关键：最后一个节点的 next 必须置空，否则形成环！
        nodes[left]->next = nullptr;
    }
};

// ============================================================
// 解法2: 找中点 + 反转后半段 + 交替合并 — O(1) 空间
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈是 O(n) 空间。观察目标序列的结构：
//   原始:  1 → 2 → 3 → 4 → 5
//   前半:  1 → 2 → 3
//   后半:  5 → 4          (逆序!)
//   合并:  1→5→2→4→3      (交替穿插)
//
// 三步操作，每一步都是经典链表基本功：
//   Step1: 快慢指针找中点
//   Step2: 反转后半段
//   Step3: 交替合并两个链表
//
// 指针变化全过程 (1→2→3→4→5):
//
// Step1: 快慢指针
//   slow=1, fast=1
//   → slow=2, fast=3
//   → slow=3, fast=5(到尾,退出)
//   slow停在3，前半段: 1→2→3
//
// Step2: 断开 + 反转
//   second = slow->next = 4→5
//   slow->next = NULL  → 前半: 1→2→3→NULL
//   反转 4→5:
//     prev=NULL, curr=4
//     → 4→NULL, prev=4, curr=5
//     → 5→4→NULL, prev=5, curr=NULL
//   second = 5→4→NULL
//
// Step3: 合并 first=1→2→3, second=5→4
//   Round1: tmp1=2, tmp2=4, 连: 1→5→2, first=2, second=4
//   Round2: tmp1=3, tmp2=NULL, 连: 2→4→3, first=3, second=NULL
//   second=NULL → 结束
//   结果: 1→5→2→4→3  ✓
// ============================================================
class Solution2 {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        
        // === Step1: 快慢指针找中点 ===
        // 终止条件: fast->next && fast->next->next
        // 这保证 slow 停在前半段的最后一个节点
        // (而不是后半段的第一个节点，这个区别很重要)
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // === Step2: 断开，反转后半段 ===
        ListNode* second = slow->next;  // 后半段起点
        slow->next = nullptr;           // 断开前后两半
        second = reverseList(second);   // 反转后半段
        
        // === Step3: 交替合并 ===
        // 后半段长度 ≤ 前半段长度，所以以 second 为循环条件
        ListNode* first = head;
        while (second) {
            // 先保存各自的下一个，因为接下来要改 next 指针
            ListNode* tmp1 = first->next;
            ListNode* tmp2 = second->next;
            // 穿插连接: first → second → tmp1
            first->next = second;
            second->next = tmp1;
            // 前进到下一对
            first = tmp1;
            second = tmp2;
        }
    }
    
private:
    // 反转链表 (迭代法)
    // prev=NULL, curr=head
    // 每一步: curr->next 指向 prev, 然后两者前进
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* next = curr->next;  // 保存下一个
            curr->next = prev;            // 反转指向
            prev = curr;                  // prev前进
            curr = next;                  // curr前进
        }
        return prev;  // prev 就是新的头
    }
};

// ============================================================
// 解法3: 递归 — 利用递归栈从尾部回收节点
// 时间: O(n)  空间: O(n) 递归栈
//
// 【思路】
// 递归天然能到达链表尾部。用一个成员变量 front 从头开始走，
// 递归返回时 back 从尾部回来，两者配对连接。
// 当 front 和 back 相遇或交错时，说明配对完成。
//
// 递归展开 (1→2→3→4→5):
//   调用栈:  reorder(1) → reorder(2) → ... → reorder(5) → reorder(NULL)
//   返回时:  front=1, back=5 → 连接 1→5, front前进到2
//            front=2, back=4 → 连接 2→4, front前进到3
//            front=3, back=3 → front==back, 结束
// ============================================================
class Solution3 {
public:
    void reorderList(ListNode* head) {
        front = head;
        reorder(head);
    }
    
private:
    ListNode* front;
    
    void reorder(ListNode* back) {
        if (!back) return;
        
        // 递归到链表尾部
        reorder(back->next);
        
        // 终止条件:
        // front == back: 奇数长度，front和back指向同一个中间节点
        // front->next == back: 偶数长度，front和back相邻
        if (front == back || front->next == back) {
            back->next = nullptr;  // 收尾
            return;
        }
        
        // 把 back 插入到 front 后面
        // 原: front → nextFront → ... → back
        // 变: front → back → nextFront → ...
        ListNode* nextFront = front->next;
        front->next = back;
        back->next = nextFront;
        
        // front 前进到下一个待配对位置
        front = nextFront;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间 | 空间 | 优点                    | 缺点              |
// |-------------|------|------|-------------------------|--------------------|
// | 数组+双指针  | O(n) | O(n) | 直觉简单，不易出错      | 额外空间           |
// | 三步法 ⭐   | O(n) | O(1) | 最优空间，面试首选      | 步骤多，指针易搞混  |
// | 递归        | O(n) | O(n) | 代码简洁，展示递归功底  | 栈空间大，不好调试  |
//
// 面试建议: 先口述解法1展示理解，再写解法2展示编码能力。
//
// 【易错点】
// 1. 快慢指针终止条件:
//    ✗ while (fast && fast->next)      → slow停在后半段第一个节点
//    ✓ while (fast->next && fast->next->next) → slow停在前半段最后一个节点
//    对于 1→2→3→4:  ✗ 方式 slow=3,  ✓ 方式 slow=2
//    影响: 断开位置错误导致前后半段划分不对
//
// 2. 断开链表:
//    ✗ ListNode* second = slow->next; reverseList(second);
//       // 忘了 slow->next = nullptr，前半段尾部仍连着后半段
//    ✓ ListNode* second = slow->next; slow->next = nullptr; second = reverseList(second);
//
// 3. 合并时指针覆盖:
//    ✗ first->next = second; second->next = first->next;
//       // first->next 已被改成 second，second->next = second 形成自环！
//    ✓ 先 tmp1 = first->next; tmp2 = second->next; 再操作
//
// 4. 数组方案末尾置空:
//    ✗ 循环结束后不做任何处理 → 最后一个节点可能指向之前的节点形成环
//    ✓ nodes[left]->next = nullptr;
//
// 【面试追问】
// Q1: 最直觉的思路是什么？瓶颈在哪？
//   → 数组方案，O(n)空间。瓶颈是单链表无法反向遍历。
//
// Q2: 能否 O(1) 空间？
//   → 三步法: 找中点(快慢指针) + 反转后半段 + 交替合并。
//     每一步都是经典操作，组合起来就是 O(1) 空间。
//
// Q3: 如果是双向链表呢？
//   → 直接头尾双指针交替连接即可，不需要找中点和反转。
//     head指针往后走，tail指针往前走，交替连接到 head==tail 或 head->next==tail。
//
// Q4: 如果要返回新链表而不是原地修改呢？
//   → 可以创建新节点，数组方案最直接。
//     但原地修改更常见，因为考察的是链表指针操作。
// ============================================================
