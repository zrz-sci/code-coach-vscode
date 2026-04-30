/*
 * 面试题 02.05: 链表求和 (Sum Lists LCCI)
 * 同 LeetCode 2. 两数相加
 *
 * 难度: Medium
 * 标签: Recursion, Linked List, Math
 *
 * =============================================
 * 解法一: 模拟竖式加法 (迭代, 推荐)
 * =============================================
 *
 * 核心思想:
 *   数位逆序存储, 链表头 = 个位。从头同时遍历两链表,
 *   逐位相加并处理进位, 构建结果链表。
 *
 * 关键技巧:
 *   1. 哨兵节点 dummy → 避免头节点特判
 *   2. 统一循环 while(l1 || l2 || carry) → 覆盖所有情况
 *   3. 短链表遍历完 → 视为 0 继续参与计算
 *
 * 时间: O(max(m,n)) — 遍历较长链表
 * 空间: O(max(m,n)) — 结果链表
 */

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        // 哨兵节点: 不需要特判结果链表的第一个节点
        // dummy -> [d1] -> [d2] -> ... -> null
        //  ^伪头     ^真正的结果头
        ListNode dummy(0);
        ListNode* cur = &dummy;  // cur 始终指向结果链表的最后一个节点
        int carry = 0;           // 进位, 只可能是 0 或 1

        // 统一循环条件:
        //   l1 有值 → 继续 (l1 还没遍历完)
        //   l2 有值 → 继续 (l2 还没遍历完)
        //   carry=1 → 继续 (还有最终进位要处理, 如 999+1=1000)
        while (l1 || l2 || carry) {
            int sum = carry;

            // 取 l1 当前位的值 (遍历完则视为 0)
            if (l1) {
                sum += l1->val;
                l1 = l1->next;  // 指针前进
            }

            // 取 l2 当前位的值
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }

            // 计算当前位的数字和新的进位
            // 最大: 9 + 9 + 1 = 19, carry 最大为 1
            carry = sum / 10;                       // 0 或 1
            cur->next = new ListNode(sum % 10);     // 当前位数字
            cur = cur->next;                        // 指针前进
        }

        // 跳过哨兵节点, 返回真正的结果头
        return dummy.next;
    }
};


/*
 * =============================================
 * 解法二: 递归写法
 * =============================================
 *
 * 递归思路:
 *   当前位 = (l1->val + l2->val + carry) % 10
 *   递归处理下一位, 传递新的 carry
 *   基础情况: 两链表都空且无进位 → 返回 null
 *
 * 时间: O(max(m,n))
 * 空间: O(max(m,n)) — 递归栈深度
 */

class Solution_Recursive {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        return helper(l1, l2, 0);
    }

private:
    ListNode* helper(ListNode* l1, ListNode* l2, int carry) {
        // 基础情况: 两链表都遍历完且无进位
        if (!l1 && !l2 && carry == 0) return nullptr;

        int sum = carry;
        if (l1) sum += l1->val;
        if (l2) sum += l2->val;

        // 创建当前节点
        ListNode* node = new ListNode(sum % 10);

        // 递归处理下一位
        node->next = helper(
            l1 ? l1->next : nullptr,  // l1 遍历完则传 null
            l2 ? l2->next : nullptr,  // l2 遍历完则传 null
            sum / 10                   // 新的进位
        );

        return node;
    }
};


/*
 * =============================================
 * 解法三 (进阶): 正向存储 — 翻转链表法
 * =============================================
 *
 * 进阶问题: 数位正向存储 (高位在头部)
 *   (6 -> 1 -> 7) + (2 -> 9 -> 5) = (9 -> 1 -> 2)
 *
 * 思路: 翻转两链表 → 变成逆序 → 逆序相加 → 翻转结果
 *
 * 时间: O(max(m,n))
 * 空间: O(1) 额外 (不算结果链表)
 */

class Solution_Forward_Reverse {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        // 翻转两个链表
        l1 = reverseList(l1);
        l2 = reverseList(l2);

        // 逆序相加 (与解法一完全相同)
        ListNode dummy(0);
        ListNode* cur = &dummy;
        int carry = 0;

        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) { sum += l1->val; l1 = l1->next; }
            if (l2) { sum += l2->val; l2 = l2->next; }
            carry = sum / 10;
            cur->next = new ListNode(sum % 10);
            cur = cur->next;
        }

        // 翻转结果 (将逆序结果变回正向)
        return reverseList(dummy.next);
    }

private:
    // 链表原地翻转: prev 指针法
    //   null <- [1] <- [2] <- [3]
    //   prev                  head→null 后变成:
    //   [3] -> [2] -> [1] -> null
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        while (head) {
            ListNode* next = head->next;  // 暂存下一节点
            head->next = prev;            // 反转指向
            prev = head;                  // prev 前进
            head = next;                  // head 前进
        }
        return prev;  // prev 是新的头
    }
};


/*
 * =============================================
 * 解法四 (进阶): 正向存储 — 栈法
 * =============================================
 *
 * 思路: 用栈反转链表的值, 从栈顶(个位)开始相加,
 *       用头插法构建正向结果链表.
 *
 * 优点: 不修改原链表
 * 时间: O(max(m,n))
 * 空间: O(m+n)
 */

class Solution_Forward_Stack {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        stack<int> s1, s2;

        // 入栈: 链表头(高位)在栈底, 链表尾(低位)在栈顶
        while (l1) { s1.push(l1->val); l1 = l1->next; }
        while (l2) { s2.push(l2->val); l2 = l2->next; }

        ListNode* head = nullptr;  // 用头插法, 初始为空
        int carry = 0;

        while (!s1.empty() || !s2.empty() || carry) {
            int sum = carry;
            if (!s1.empty()) { sum += s1.top(); s1.pop(); }
            if (!s2.empty()) { sum += s2.top(); s2.pop(); }

            carry = sum / 10;

            // 头插法: 新节点插到最前面
            //   之前: head -> [1] -> [2]
            //   头插 [9]: [9] -> [1] -> [2]
            ListNode* node = new ListNode(sum % 10);
            node->next = head;
            head = node;
        }

        return head;
    }
};


/*
 * =============================================
 * 干跑示例 1: l1=[7,1,6], l2=[5,9,2]  (617+295=912)
 * =============================================
 *
 * 初始: cur→dummy[0], carry=0
 *
 * 第1轮: sum = 0+7+5 = 12
 *   carry = 1, digit = 2
 *   dummy[0] -> [2]
 *                ^cur
 *   l1→[1]->... l2→[9]->...
 *
 * 第2轮: sum = 1+1+9 = 11
 *   carry = 1, digit = 1
 *   dummy[0] -> [2] -> [1]
 *                        ^cur
 *   l1→[6] l2→[2]
 *
 * 第3轮: sum = 1+6+2 = 9
 *   carry = 0, digit = 9
 *   dummy[0] -> [2] -> [1] -> [9]
 *                               ^cur
 *   l1→null l2→null
 *
 * 退出: l1=null, l2=null, carry=0
 * 返回: [2] -> [1] -> [9] (即 912) ✓
 *
 * =============================================
 * 干跑: 进位传播 l1=[9,9,9], l2=[1]  (999+1=1000)
 * =============================================
 *
 * 第1轮: sum=0+9+1=10, carry=1, digit=0   → [0]
 * 第2轮: sum=1+9+0=10, carry=1, digit=0   → [0]->[0]
 *   (l2 已到 null, 视为 0)
 * 第3轮: sum=1+9+0=10, carry=1, digit=0   → [0]->[0]->[0]
 * 第4轮: sum=1+0+0=1,  carry=0, digit=1   → [0]->[0]->[0]->[1]
 *   (l1, l2 都是 null, 由 carry=1 触发这一轮!)
 *
 * 返回: [0]->[0]->[0]->[1] (即 1000) ✓
 *
 * =============================================
 * 边界情况
 * =============================================
 *
 * 1. l1=[0], l2=[0]
 *    sum=0, carry=0, digit=0 → [0]
 *    结果: [0] ✓
 *
 * 2. l1=[5], l2=[5]
 *    第1轮: sum=10, carry=1, digit=0 → [0]
 *    第2轮: sum=1, carry=0, digit=1 → [0]->[1]
 *    结果: [0]->[1] (即 10) ✓
 *
 * 3. l1=[1,8], l2=[0]
 *    第1轮: sum=1+0=1 → [1]
 *    第2轮: sum=8+0=8 → [1]->[8]
 *    结果: [1]->[8] (即 81) ✓
 *
 * 4. l1 和 l2 长度差很大:
 *    短链表遍历完后 l_short=null, 每轮 sum = carry + l_long->val
 *    自然处理, 无需额外逻辑
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 如果数位是正向存储的怎么办？
 *     → 方法1: 翻转两链表 → 逆序相加 → 翻转结果
 *     → 方法2: 用栈逆序取数 + 头插法构建结果 (不修改原链表)
 *     → 方法3: 递归对齐长度，回溯时处理进位
 *
 * Q2: 能否不创建新节点，原地修改？
 *     → 可以在较长链表上直接修改 val，但最高位进位时仍需 new 一个头节点
 *
 * Q3: 如何扩展到三个链表求和？
 *     → 循环条件: while(l1 || l2 || l3 || carry)
 *     → sum 最大 = 9+9+9+1 = 28, carry 最大 = 2
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * ✗ 忘记最后的进位 → 999+1=1000, carry=1 需创建新节点
 * ✗ 链表长度不等时越界 → 必须判断 l1!=nullptr 才取 l1->val
 * ✗ 忘记推进指针 l1=l1->next → 导致死循环
 * ✗ 试图转成整数再加 → 链表可能极长，超出 long long 范围
 */
