/*
 * LeetCode 708: 循环有序列表的插入 (Insert into a Sorted Circular Linked List)
 *
 * 【题目本质】
 * 在循环非递减链表中找到正确位置插入新值，需处理三种场景：
 * 中间位置、转折点（最大→最小）、以及所有值相同的兜底情况。
 *
 * 【解法总览】
 * 解法1: 一次遍历分情况讨论 — O(n) / O(1) — 面试首选
 * 解法2: 找最大节点后线性查找 — O(n) / O(1) — 更直观但需两次遍历
 */

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node() {}
    Node(int _val) { val = _val; next = NULL; }
    Node(int _val, Node* _next) { val = _val; next = _next; }
};
*/

// ============================================================
// 解法1: 一次遍历分情况讨论 — O(n) / O(1) ⭐ 面试首选
//
// 【思路】
// 遍历链表中每一对相邻节点 (curr, next)，判断 insertVal 是否应该
// 插在它们之间。三种情况任一满足即可插入：
//
//   Case A: 正常中间位置
//           curr.val <= insertVal <= next.val
//
//   Case B: 转折点 + insertVal 是全局最大
//           curr.val > next.val (到了转折点) && insertVal >= curr.val
//
//   Case C: 转折点 + insertVal 是全局最小
//           curr.val > next.val (到了转折点) && insertVal <= next.val
//
// 如果遍历一圈回到起点都没找到 → 所有值相同 → 任意位置插入
//
// 循环链表示意图 (head 指向 3):
//
//     ┌──→ 3 ──→ 4 ──→ 1 ──┐
//     └─────────────────────┘
//               ↑
//          转折点: 4 → 1
//
// 插入 insertVal=2 的过程:
//   curr=3, next=4: 3<=2? No  → 继续
//   curr=4, next=1: 转折点! 2>=4? No, 2<=1? No → 继续
//   curr=1, next=3: 1<=2<=3? Yes → 插在 1 和 3 之间!
//
//     ┌──→ 3 ──→ 4 ──→ 1 ──→ [2] ──┐
//     └─────────────────────────────┘
//
// 插入 insertVal=5 的过程:
//   curr=3, next=4: 3<=5<=4? No → 继续
//   curr=4, next=1: 转折点! 5>=4? Yes → 插在 4 和 1 之间!
//
//     ┌──→ 3 ──→ 4 ──→ [5] ──→ 1 ──┐
//     └─────────────────────────────┘
// ============================================================
class Solution1 {
public:
    Node* insert(Node* head, int insertVal) {
        Node* newNode = new Node(insertVal);
        
        // 空链表：创建自环
        if (!head) {
            newNode->next = newNode;
            return newNode;
        }
        
        Node* curr = head;
        
        // 用 do...while 保证至少遍历一次
        // （因为 curr 初始就等于 head，while(curr!=head) 会直接退出）
        do {
            Node* next = curr->next;
            
            // Case A: insertVal 夹在两个有序节点之间
            if (curr->val <= insertVal && insertVal <= next->val) {
                break;
            }
            
            // Case B/C: 到达转折点（从最大值跳到最小值）
            // insertVal 比最大值还大 或 比最小值还小 → 都插在这里
            if (curr->val > next->val) {
                if (insertVal >= curr->val || insertVal <= next->val) {
                    break;
                }
            }
            
            curr = next;
        } while (curr != head);
        // 遍历一圈没 break → 所有值相同 → curr 停在 head
        // 直接在 head 后插入也是正确的（有序性不受影响）
        
        // 在 curr 和 curr->next 之间插入新节点
        newNode->next = curr->next;
        curr->next = newNode;
        
        return head;
    }
};

// ============================================================
// 解法2: 找最大节点后线性查找 — O(n) / O(1)
//
// 【思路】
// 先遍历一圈找到最大值节点（转折点的前一个），从而确定链表的
// "逻辑起点"（最小值节点）。然后分两种情况：
//   1. insertVal >= 最大值 或 <= 最小值 → 插在转折点
//   2. 否则从逻辑起点线性扫描找正确位置
//
// 这种方式更直观——先理清链表的逻辑结构，再做查找。
// 代价是可能需要两次遍历（一次找 max，一次找位置）。
//
// 示例: [3→4→1], insertVal=2
//
// Step1: 找最大节点 maxNode=4
// Step2: minNode = maxNode.next = 1
// Step3: 2 不是全局最大/最小 → 从 minNode=1 开始扫描
// Step4: 1.next=3, 2<3 → 插在 1 和 3 之间
//
//     ┌──→ 3 ──→ 4 ──→ 1 ──→ [2] ──┐
//     └─────────────────────────────┘
// ============================================================
class Solution2 {
public:
    Node* insert(Node* head, int insertVal) {
        Node* newNode = new Node(insertVal);
        
        if (!head) {
            newNode->next = newNode;
            return newNode;
        }
        
        // Step 1: 找到最大值节点
        // 用 >= 确保找到最后一个最大值（处理重复元素如 [3,3,4,4,1]）
        Node* maxNode = head;
        Node* curr = head->next;
        while (curr != head) {
            if (curr->val >= maxNode->val) {
                maxNode = curr;
            }
            curr = curr->next;
        }
        
        // Step 2: 最小值节点 = maxNode 的下一个
        Node* minNode = maxNode->next;
        
        // Step 3: insertVal 是全局最大或最小 → 插在转折点
        if (insertVal >= maxNode->val || insertVal <= minNode->val) {
            newNode->next = minNode;
            maxNode->next = newNode;
        } else {
            // Step 4: 从 minNode 开始线性扫描，找到 curr.next.val >= insertVal 的位置
            curr = minNode;
            while (curr->next->val < insertVal) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
        
        return head;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 一次遍历      | 解法2: 找最大节点     |
// |-------------|---------------------|---------------------|
// | 遍历次数     | 最多 1 次            | 最多 2 次            |
// | 代码复杂度   | 简洁，一个循环搞定     | 更长，但逻辑分层清晰   |
// | 思维难度     | 需要同时想三种 case   | 分步处理，更容易想清楚  |
// | 面试推荐     | ⭐ 首选              | 解法1想不清楚时的备选   |
//
// 【易错点】
// 1. 空链表新节点忘记自指:
//    ✗ newNode->next = NULL; return newNode;
//    ✓ newNode->next = newNode; return newNode;
//    → 循环链表中单个节点必须指向自己
//
// 2. 用 while(curr != head) 导致循环体不执行:
//    ✗ curr = head; while (curr != head) { ... }  // 初始条件就不满足
//    ✓ do { ... } while (curr != head);           // 保证至少执行一次
//
// 3. 转折点条件漏掉等号:
//    ✗ if (insertVal > curr->val || insertVal < next->val)
//    ✓ if (insertVal >= curr->val || insertVal <= next->val)
//    → "非递减"列表允许相等值，条件必须包含等号
//
// 4. 只考虑 Case A，忘了转折点情况:
//    → 链表 [3,4,1] 插入 5，永远不满足 curr<=5<=next
//    → 必须在转折点（4→1）处特殊判断
//
// 5. 解法2中找最大节点用 > 而非 >=:
//    ✗ if (curr->val > maxNode->val) maxNode = curr;
//    ✓ if (curr->val >= maxNode->val) maxNode = curr;
//    → 有重复最大值时（如 [1,3,3]），要定位到最后一个 3，
//      这样 maxNode->next 才是真正的最小值
//
// 【面试追问】
// Q1: 遍历一圈回到起点为什么可以直接在 head 后插入？
//     → 一圈都没找到说明所有值相同（否则转折点一定会匹配），
//       值全相同时插在任何位置都不破坏有序性。
//
// Q2: 如果是普通有序链表（不循环），代码怎么改？
//     → 更简单：(1) insertVal<=head.val → 插最前面
//              (2) 线性扫描找 curr.val<=insertVal<next.val
//              (3) 到末尾没找到 → 插最后面
//       不需要处理转折点和循环。
//
// Q3: 如果频繁调用 insert 怎么优化？
//     → 链表查找是 O(n)，可以改用跳表 (Skip List) 实现 O(log n) 的
//       查找+插入，或者用平衡 BST (如 std::set) 加双向链表维护。
// ============================================================
