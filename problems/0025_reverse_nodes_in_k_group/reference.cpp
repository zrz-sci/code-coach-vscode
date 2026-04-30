/*
 * LeetCode 25: K 个一组翻转链表 (Reverse Nodes in k-Group)
 *
 * 【题目本质】
 * 把链表按每 k 个为一组进行分段翻转，不足 k 个的最后一段保持原样。
 * 核心子问题是"翻转一段链表"（206题），难点是翻转后如何正确连接各段。
 *
 * 【解法总览】
 * 解法1: 递归法 — O(n) / O(n/k) — 最简洁，递归处理每一组
 * 解法2: 迭代法 — O(n) / O(1)  — 满足 Follow-up O(1) 空间，面试首选
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
// 解法1: 递归法 — 每次翻转 k 个节点，递归处理剩余部分
// 时间: O(n)  空间: O(n/k) 递归栈深度
//
// 【思路】
// 链表天然适合递归思考：
//   1. 检查当前位置开始是否有 k 个节点（不够就直接返回）
//   2. 翻转当前 k 个节点（标准三指针翻转法）
//   3. 原来的 head 翻转后成了尾，把它的 next 接上递归处理的结果
//   4. 返回翻转后的新头 prev
//
// 示意图 (k=3, 链表 1→2→3→4→5):
//
//   第1层递归: 翻转 [1,2,3]
//     翻转前: 1 → 2 → 3 | 4 → 5 → NULL
//     翻转后: 3 → 2 → 1   curr=4
//     head(=1).next = reverseKGroup(4, 3)  ← 递归
//
//   第2层递归: 处理 [4,5]
//     检查: 从4开始走3步，走到5后 next=NULL，不足3个
//     返回 head(=4) 不翻转
//
//   回到第1层: head(=1).next = 4
//   最终: 3 → 2 → 1 → 4 → 5 → NULL
//   返回 prev(=3)
// ============================================================
class Solution1 {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // Step 1: 检查是否有 k 个节点
        // 为什么要先检查？因为不足 k 个的最后一段不能翻转
        ListNode* check = head;
        for (int i = 0; i < k; i++) {
            if (!check) return head;  // 不足 k 个，base case
            check = check->next;
        }
        // 此时 check 指向第 k+1 个节点（即下一组的起点）
        
        // Step 2: 翻转从 head 开始的 k 个节点
        // 这就是 206 题的标准翻转，只不过翻转 k 次而非到 NULL
        //
        //  翻转过程 (k=3):
        //  初始: prev=NULL  curr=1→2→3→...
        //  i=0:  NULL←1  prev=1  curr=2→3→...
        //  i=1:  NULL←1←2  prev=2  curr=3→...
        //  i=2:  NULL←1←2←3  prev=3  curr=check(下一组起点)
        ListNode* prev = nullptr;
        ListNode* curr = head;
        for (int i = 0; i < k; i++) {
            ListNode* nxt = curr->next;
            curr->next = prev;  // 翻转指针方向
            prev = curr;
            curr = nxt;
        }
        // prev = 新头（原来的第 k 个节点）
        // head = 新尾（原来的第 1 个节点）
        // curr = 下一组的起点
        
        // Step 3: 新尾连接递归处理后续链表的结果
        // 为什么是 head->next？因为翻转后 head 已经从头变成了尾
        head->next = reverseKGroup(curr, k);
        
        return prev;  // 返回翻转后的新头
    }
};

// ============================================================
// 解法2: 迭代法 — dummy节点 + 逐段翻转 + 手动串联
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 递归法的空间是 O(n/k)，不满足 Follow-up 的 O(1) 要求。
// 改用迭代：
//   - 用 dummy 节点简化头节点被翻转的情况
//   - 用 prevGroupEnd 维护"上一组翻转后的最后一个节点"
//   - 每轮：检查够 k 个 → 翻转 → 连接 → 更新 prevGroupEnd
//
// 详细过程 (k=2, 链表 1→2→3→4→5):
//
// 初始:
//   dummy → 1 → 2 → 3 → 4 → 5 → NULL
//    ↑ prevGroupEnd
//
// === 第1轮 ===
// 检查: 从 dummy 走 2 步到 2，够了。check=2
// 记录: groupHead=1, nextGroupHead=3
// 翻转 [1,2]:
//   i=0: NULL←1  prev=1 curr=2
//   i=1: NULL←1←2  prev=2 curr=3
// 连接: prevGroupEnd(dummy).next = 2(prev/新头)
//       1(groupHead/新尾).next = 3(nextGroupHead)
// 结果: dummy → 2 → 1 → 3 → 4 → 5 → NULL
// 更新: prevGroupEnd = 1(groupHead)
//
// === 第2轮 ===
// 检查: 从 1 走 2 步到 4，够了。check=4
// 记录: groupHead=3, nextGroupHead=5
// 翻转 [3,4]:
//   i=0: NULL←3  prev=3 curr=4
//   i=1: NULL←3←4  prev=4 curr=5
// 连接: prevGroupEnd(1).next = 4(prev/新头)
//       3(groupHead/新尾).next = 5(nextGroupHead)
// 结果: dummy → 2 → 1 → 4 → 3 → 5 → NULL
// 更新: prevGroupEnd = 3(groupHead)
//
// === 第3轮 ===
// 检查: 从 3 走 1 步到 5，再走 next=NULL，不足 2 个 → 结束
//
// 返回 dummy.next = 2
// 最终: 2 → 1 → 4 → 3 → 5 → NULL
// ============================================================
class Solution2 {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // dummy 节点：因为原来的 head 可能翻转到中间去，需要 dummy 记录新头
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prevGroupEnd = &dummy;
        
        while (true) {
            // Step 1: 检查从 prevGroupEnd 后面是否有 k 个节点
            // 为什么从 prevGroupEnd 开始？因为它是上一组的尾，后面才是当前组
            ListNode* check = prevGroupEnd;
            for (int i = 0; i < k; i++) {
                check = check->next;
                if (!check) return dummy.next;  // 不足 k 个，整个过程结束
            }
            // check 现在指向当前组的最后一个节点
            
            // Step 2: 记录关键指针（翻转会打乱 next，必须先记录）
            ListNode* groupHead = prevGroupEnd->next;  // 当前组第一个节点（翻转后变尾）
            ListNode* nextGroupHead = check->next;     // 下一组的第一个节点
            
            // Step 3: 翻转从 groupHead 开始的 k 个节点
            ListNode* prev = nullptr;
            ListNode* curr = groupHead;
            for (int i = 0; i < k; i++) {
                ListNode* nxt = curr->next;
                curr->next = prev;
                prev = curr;
                curr = nxt;
            }
            // prev = 翻转后的新头 (== 原来的 check)
            // groupHead = 翻转后的新尾
            // curr = nextGroupHead
            
            // Step 4: 将翻转后的段接回链表
            // 为什么是 prev 不是 check？虽然它们相等，但语义上 prev 是翻转后的新头更清晰
            prevGroupEnd->next = prev;            // 前一段尾 → 当前段新头
            groupHead->next = nextGroupHead;      // 当前段新尾 → 下一段头
            
            // Step 5: 移动 prevGroupEnd 到当前组翻转后的尾部
            // 为什么是 groupHead？因为翻转后原来的头变成了尾
            prevGroupEnd = groupHead;
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 解法1(递归)     | 解法2(迭代)      |
// |----------|----------------|-----------------|
// | 时间     | O(n)           | O(n)            |
// | 空间     | O(n/k) 递归栈   | O(1) ✅         |
// | 代码量   | ~15行，简洁     | ~25行，需维护指针 |
// | Follow-up| 不满足O(1)空间  | 满足 ✅          |
// | 面试建议 | 先口述思路      | 必须能写出       |
//
// 两种解法每个节点都恰好被访问两次（检查一次+翻转一次），
// 所以时间都是 O(n)。
//
// ============================================================
// 【易错点】
//
// 1. 检查节点数量的起点搞错:
//    ✗ 始终从 head 开始检查 → 第二轮开始就不对了
//    ✓ 从 prevGroupEnd 开始检查，因为它是上一组的尾
//
// 2. 翻转后连接方向搞反:
//    ✗ prevGroupEnd->next = groupHead  ← groupHead 翻转后是尾！
//    ✓ prevGroupEnd->next = prev       ← prev 才是翻转后的新头
//
// 3. 忘记更新 prevGroupEnd:
//    ✗ 翻转完不更新，下一轮 prevGroupEnd 还指向上上一组的尾
//    ✓ prevGroupEnd = groupHead (翻转后 groupHead 变成了当前组的尾)
//
// 4. 翻转循环次数差一:
//    ✗ for (int i = 0; i < k-1; i++) → 少翻转一个节点
//    ✓ for (int i = 0; i < k; i++)   → 每个节点的 next 都要改
//
// 5. nextGroupHead 没提前记录:
//    ✗ 翻转后再取 check->next，但此时 check->next 已经被翻转改掉了
//    ✓ 翻转前用 nextGroupHead = check->next 提前保存
//
// ============================================================
// 【面试追问】
//
// Q1: 206题翻转链表你怎么做？
//     → 三指针 prev/curr/nxt，逐个翻转。这是本题的核心子操作。
//
// Q2: 翻转后怎么和前后段连接？
//     → groupHead 翻转后变尾，prev 变头。
//       prevGroupEnd->next = prev (接新头)
//       groupHead->next = nextGroupHead (尾接下一段)
//
// Q3(Follow-up): 能否 O(1) 空间？
//     → 递归有 O(n/k) 栈空间，改成迭代用 while 循环 + prevGroupEnd 手动串联。
//
// Q4(变体): 如果最后不足 k 个的也要翻转？
//     → 去掉"不足 k 个就 return"的检查。最后一段不管够不够都翻转。
//       具体：把 for 循环中的 check 逻辑改为"走到 NULL 就停"，
//       记录实际走的步数作为翻转次数。
//
// Q5(变体): 奇数组翻转、偶数组不翻转？
//     → 加一个 groupIndex 计数器，在 while 循环开头判断是否翻转。
//       如果不翻转，直接 prevGroupEnd 向后移动 k 步即可。
// ============================================================
