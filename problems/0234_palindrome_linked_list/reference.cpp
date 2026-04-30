/*
 * LeetCode 234: Palindrome Linked List (回文链表)
 *
 * 【题目本质】
 * 判断单链表是否回文。核心困难：单链表只能正向遍历，无法反向访问。
 * 所以需要想办法获取"逆序信息"来和正序对比。
 *
 * 【解法总览】
 * 解法1: 复制到数组 + 双指针  — O(n) / O(n)  — 最直觉
 * 解法2: 栈(半栈)            — O(n) / O(n)  — 利用栈的逆序特性
 * 解法3: 递归                — O(n) / O(n)  — 利用回溯的逆序特性
 * 解法4: 快慢指针 + 反转后半段 — O(n) / O(1)  — 最优，面试首选 ⭐
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
// 解法1: 复制到数组 + 双指针 — 最直觉的做法
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 单链表无法随机访问，但数组可以。
// 把链表的值全部复制到数组中，问题就退化为
// "判断数组是否回文"，这是一个双指针的经典题。
//
// 示例: 1 → 2 → 2 → 1 → NULL
// 数组: [1, 2, 2, 1]
//        ↑left    ↑right
//        1==1 ✓ → left++, right--
//           ↑  ↑
//           2==2 ✓ → left >= right, 结束
// → 回文!
// ============================================================
class Solution1 {
public:
    bool isPalindrome(ListNode* head) {
        std::vector<int> vals;
        while (head) {
            vals.push_back(head->val);
            head = head->next;
        }
        int left = 0, right = (int)vals.size() - 1;
        while (left < right) {
            if (vals[left] != vals[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};

// ============================================================
// 解法2: 半栈法 — 利用栈的LIFO特性获取逆序
// 时间: O(n)  空间: O(n/2) = O(n)
//
// 【思路】
// 只把前半部分入栈，后半部分逐个和栈顶对比。
// 栈顶弹出的顺序恰好是前半段的逆序，
// 正好可以和后半段的正序逐一对比。
//
// 链表: 1 → 2 → 3 → 2 → 1 (奇数长度)
// 前半入栈: stack = [1, 2] (栈顶是2)
// 后半: 2 → 1
//   比较: 栈顶2 == 节点2 ✓, pop
//   比较: 栈顶1 == 节点1 ✓, pop
// → 回文!
//
// 关键: 奇数长度时中间节点不参与比较,
// 通过 fast 是否为空来判断奇偶。
// ============================================================
class Solution2 {
public:
    bool isPalindrome(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 前半部分入栈
        std::stack<int> stk;
        ListNode* curr = head;
        while (curr != slow) {
            stk.push(curr->val);
            curr = curr->next;
        }
        
        // 奇数长度时 fast 非空, 跳过中间节点
        // 为什么? 奇数时 fast 停在最后一个节点(非NULL),
        // 偶数时 fast 停在 NULL
        if (fast) slow = slow->next;
        
        while (slow) {
            if (stk.top() != slow->val) return false;
            stk.pop();
            slow = slow->next;
        }
        return true;
    }
};

// ============================================================
// 解法3: 递归 — 利用函数调用栈回溯时的"逆序"特性
// 时间: O(n)  空间: O(n) 递归栈空间
//
// 【思路】
// 递归天然有"先深入到底部, 再回溯"的特性。
// 回溯时节点的访问顺序恰好是链表的逆序！
// 
// 我们维护一个 front 指针从头部正向推进,
// 递归回溯时 curr 从尾部逆向"回来",
// 每次比较 front->val 和 curr->val。
//
// 链表: 1 → 2 → 2 → 1
//
// 递归展开: recurse(1) → recurse(2) → recurse(2) → recurse(1) → recurse(NULL)
//                                                                  返回 true
// 回溯:
//   curr=1(尾), front=1(头) → 1==1 ✓, front → 2
//   curr=2,     front=2     → 2==2 ✓, front → 2(第二个)
//   curr=2,     front=2     → 2==2 ✓, front → 1(尾)
//   curr=1(头), front=1(尾) → 1==1 ✓
// → 回文!
// ============================================================
class Solution3 {
    ListNode* front;  // 成员变量: 从头部正向推进的指针
    
    bool recurse(ListNode* curr) {
        if (!curr) return true;
        // 先递归到链表尾部
        if (!recurse(curr->next)) return false;
        // 回溯时: curr 从后往前, front 从前往后
        if (front->val != curr->val) return false;
        front = front->next;  // front 正向前进一步
        return true;
    }
    
public:
    bool isPalindrome(ListNode* head) {
        front = head;
        return recurse(head);
    }
};

// ============================================================
// 解法4: 快慢指针 + 反转后半段 — O(1) 空间最优解 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 前面三种解法的空间瓶颈: 都需要存储"逆序信息"。
// 突破思路: 直接在原链表上把后半段反转!
// 反转操作本身只需要三个指针, 不需要额外空间。
//
// 三步走:
// 1. 快慢指针找中点
// 2. 反转后半段
// 3. 两个指针从两头向中间比较
//
// === 偶数长度示例: 1 → 2 → 2 → 1 ===
//
// Step1: 找中点
//   初始:    s=1, f=1
//   第1轮:   s=2, f=2(第二个)  → f->next->next == NULL, 停止
//   slow 停在前半段最后一个节点(节点2)
//
//   1 → 2 → 2 → 1 → NULL
//        ↑s
//
// Step2: 反转从 slow->next(=节点2') 开始的后半段
//   反转前: 2 → 1 → NULL
//   反转过程:
//     prev=NULL, curr=2
//     Round1: next=1, 2→NULL, prev=2, curr=1
//     Round2: next=NULL, 1→2, prev=1, curr=NULL
//   反转后: 1 → 2 → NULL
//
//   前半: 1 → 2 →|  后半: 1 → 2 → NULL
//         p1            p2
//
// Step3: 逐一比较
//   p1=1, p2=1 → 1==1 ✓
//   p1=2, p2=2 → 2==2 ✓
//   p2=NULL → 结束, 回文!
//
// === 奇数长度示例: 1 → 2 → 3 → 2 → 1 ===
//
// Step1: 找中点
//   初始:    s=1, f=1
//   第1轮:   s=2, f=3
//   第2轮:   s=3, f=1(最后)  → f->next == NULL, 停止
//   slow 停在中间节点(节点3), 但我们从 slow->next 开始反转
//
//   1 → 2 → 3 → 2 → 1 → NULL
//            ↑s
//
// Step2: 反转从 slow->next(=节点2') 开始的后半段
//   反转前: 2 → 1 → NULL
//   反转后: 1 → 2 → NULL
//
//   前半: 1 → 2 → (3)  后半: 1 → 2 → NULL
//         p1                  p2
//
// Step3: 逐一比较 (以 p2 为准, 中间节点3不参与)
//   p1=1, p2=1 → 1==1 ✓
//   p1=2, p2=2 → 2==2 ✓
//   p2=NULL → 结束, 回文!
// ============================================================
class Solution4 {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;
        
        // === Step 1: 快慢指针找中点 ===
        // 用 fast->next && fast->next->next 作为条件,
        // 使 slow 停在前半段的最后一个节点
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // 此时 slow 是前半段最后一个节点
        // 后半段从 slow->next 开始
        
        // === Step 2: 反转后半段链表 ===
        ListNode* secondHalf = reverseList(slow->next);
        
        // === Step 3: 逐一比较 ===
        ListNode* p1 = head;        // 前半段从头开始
        ListNode* p2 = secondHalf;  // 后半段从头(原来的尾)开始
        bool result = true;
        while (p2) {
            // 以后半段为终止条件, 因为:
            // 偶数长度: 前后等长
            // 奇数长度: 后半段比前半段短1个(中间节点留在前半段)
            if (p1->val != p2->val) {
                result = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        
        // === Step 4: 恢复链表(可选) ===
        // 如果面试官要求不修改原链表, 把后半段再反转回去
        slow->next = reverseList(secondHalf);
        
        return result;
    }
    
private:
    // 反转链表 — LeetCode 206 的经典解法
    // 
    // 反转过程 (以 2 → 1 → NULL 为例):
    //   初始:   prev=NULL  curr=2→1→NULL
    //   Round1: next=1     2→NULL    prev=2→NULL   curr=1→NULL
    //   Round2: next=NULL  1→2→NULL  prev=1→2→NULL curr=NULL
    //   返回 prev = 1→2→NULL
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* next = curr->next;  // 先存下一个, 因为马上要断开
            curr->next = prev;            // 反转指向
            prev = curr;                  // prev 前进
            curr = next;                  // curr 前进
        }
        return prev;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间 | 空间 | 修改原链表 | 适用场景          |
// |---------------|------|------|------------|-------------------|
// | 数组+双指针   | O(n) | O(n) | 否         | 最简单, 保底用    |
// | 半栈          | O(n) | O(n) | 否         | 理解栈的逆序特性  |
// | 递归          | O(n) | O(n) | 否         | 展示递归能力      |
// | 快慢+反转 ⭐  | O(n) | O(1) | 是(可恢复) | 面试首选          |
//
// 核心区别:
// - 解法1-3 都是 O(n) 空间, 各自用不同方式获取"逆序信息"
// - 解法4 通过原地反转避免额外空间, 代价是临时修改了链表结构
// - 如果面试官不允许修改链表且要求 O(1), 这是不可能的
//   (必须说明这个 trade-off)
//
// 【易错点】
//
// 1. 快慢指针循环条件导致 slow 位置不对:
//    ✗ while (fast && fast->next) → slow 多走一步到后半段开头
//      此时反转起点应该是 slow 本身而不是 slow->next
//    ✓ while (fast->next && fast->next->next) → slow 在前半段末尾
//      反转起点是 slow->next
//    关键: 两种写法都正确, 但后续逻辑必须匹配!
//
// 2. 反转链表时忘记暂存 next:
//    ✗ curr->next = prev;
//      curr = curr->next;    // BUG! curr->next 已经是 prev 了!
//    ✓ ListNode* next = curr->next;  // 先存
//      curr->next = prev;
//      curr = next;                  // 用存的值
//
// 3. 比较循环用 while(p1 && p2):
//    ✗ 虽然结果正确, 但如果后续还要恢复链表,
//      需要知道后半段头节点, 而 p2 可能已经走到 NULL
//    ✓ while(p2) 更清晰, 且 p2 走完说明后半段全部匹配
//
// 4. 递归解法中 front 用局部变量:
//    ✗ bool recurse(ListNode* curr, ListNode* front)
//      → front 是值传递, 子函数的修改不会影响上层
//    ✓ 用成员变量, 或 ListNode*& front 引用传递
//
// 5. 处理边界: 单节点链表
//    ✗ 没有特判 !head->next, 快慢指针可能空指针
//    ✓ 开头 if (!head || !head->next) return true;
//
// 【面试追问】(递进链)
//
// Q1: 最直觉的做法? 时间空间?
//     → 数组+双指针, O(n)/O(n)
//
// Q2: 能否 O(1) 空间? (Follow-up)
//     → 快慢指针找中点 + 反转后半段 + 逐一比较
//     → 反转操作本身只需三个指针, 所以 O(1)
//
// Q3: 修改了原链表, 如果不允许修改怎么办?
//     → 比较完后再把后半段反转回去(代码中 Step4)
//     → 如果连临时修改都不允许, 只能用 O(n) 空间方案
//
// Q4: 如果是双向链表呢?
//     → 更简单: 一个指针从头, 一个从尾, O(n/2)比较, O(1)空间
//     → 不需要反转
//
// Q5: 如果链表可能有环呢?
//     → 先用快慢指针检测环(LeetCode 141)
//     → 有环不可能是回文(回文链表必须有尾部 NULL)
// ============================================================
