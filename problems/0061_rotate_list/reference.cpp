/*
 * LeetCode 61: 旋转链表 (Rotate List)
 * 
 * 【题目本质】
 * "向右旋转 k 位" = 在链表第 (n-k) 个节点处断开，后半段移到前面。
 * 核心：求长度 → k 取模 → 找断点 → 重新拼接。
 *
 * 【解法总览】
 * 解法1: 暴力模拟   — O(n × (k%n)) / O(1) — 每次移一个尾节点到头
 * 解法2: 成环断开法 — O(n) / O(1)          — 首尾相连成环，在断点处断开 ⭐
 * 解法3: 快慢指针法 — O(n) / O(1)          — 快指针先走k步定位断点
 */


// ============================================================
// 解法1: 暴力模拟 — 每次将尾节点移到头部，重复 k%n 次
// 时间: O(n × (k%n))  空间: O(1)
//
// 【思路】
// 最直觉的做法：旋转一次 = 把最后一个节点放到最前面。
// 但 k 可达 2×10⁹，所以先对 n 取模减少次数。
// 每次仍需遍历到倒数第二个节点，效率不高，但思路清晰。
//
// 过程示意 [1,2,3,4,5] k=2:
//   第1次: 找到尾5, 移到头 → 5→1→2→3→4
//   第2次: 找到尾4, 移到头 → 4→5→1→2→3 ✓
// ============================================================
class Solution1 {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        // 求长度用于取模
        int n = 0;
        ListNode* cur = head;
        while (cur) {
            n++;
            cur = cur->next;
        }
        k %= n;
        if (k == 0) return head;
        
        for (int i = 0; i < k; i++) {
            // 找倒数第二个节点
            ListNode* prev = head;
            while (prev->next->next) {
                prev = prev->next;
            }
            // 尾节点移到头部
            ListNode* tail = prev->next;
            prev->next = nullptr;
            tail->next = head;
            head = tail;
        }
        return head;
    }
};

// ============================================================
// 解法2: 成环断开法 — O(n) / O(1) ⭐ 面试首选
//
// 【思路】
// 暴力法每次只移一个节点，核心瓶颈是"重复遍历找尾节点"。
// 
// 关键观察：旋转结果就是在某个位置断开后，后半段接到前面。
// 如果先把链表连成环，只需在正确位置断开一次！
//
// 算法：
//   1. 遍历求长度 n，顺便拿到尾节点 tail
//   2. k %= n，若 k==0 直接返回
//   3. tail->next = head，成环
//   4. 从 tail 往前走 n-k 步，到达新尾巴 newTail
//   5. newHead = newTail->next，断开 newTail->next = nullptr
//
// 为什么从 tail 走 n-k 步？
//   向右旋转k位 = 倒数第k个变成新头 = 正数第(n-k+1)个变成新头
//   新尾巴是正数第(n-k)个，从tail(正数第n个)走n-k步正好到达
//
//   [1,2,3,4,5] k=2, n=5
//
//   Step1: 遍历得 n=5, tail=节点5
//     1 → 2 → 3 → 4 → 5 → NULL
//                           ↑tail
//
//   Step2: k = 2%5 = 2, 不为0
//
//   Step3: 成环 tail->next = head
//     1 → 2 → 3 → 4 → 5 ─┐
//     ↑                     |
//     └─────────────────────┘
//
//   Step4: 从 tail 走 n-k=3 步
//     5 → 1 → 2 → 3  (走了3步到达节点3)
//     newTail = 节点3
//     newHead = 节点4
//
//   Step5: 断开
//     4 → 5 → 1 → 2 → 3 → NULL  ✓
// ============================================================
class Solution2 {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        // 第一步：求长度 + 找尾节点
        // 注意 n 从 1 开始，循环条件是 tail->next，这样结束时 tail 在最后一个节点
        int n = 1;
        ListNode* tail = head;
        while (tail->next) {
            n++;
            tail = tail->next;
        }
        
        // k 取模：旋转 n 的整数倍 = 不旋转
        k %= n;
        if (k == 0) return head;
        
        // 第二步：尾接头，形成环
        tail->next = head;
        
        // 第三步：找新尾巴 — 从 tail 走 (n - k) 步
        ListNode* newTail = tail;
        for (int i = 0; i < n - k; i++) {
            newTail = newTail->next;
        }
        
        // 第四步：断开环
        ListNode* newHead = newTail->next;
        newTail->next = nullptr;
        
        return newHead;
    }
};

// ============================================================
// 解法3: 快慢指针法 — O(n) / O(1)
//
// 【思路】
// 和解法2本质相同（都在找第 n-k 个节点），但不成环，
// 用快慢指针的间距来定位断点。
//
// 算法：
//   1. 求长度 n，k %= n
//   2. 快指针先走 k 步，此时快慢相距 k
//   3. 快慢一起走，快到末尾时慢在第 n-k 个节点（新尾巴）
//   4. 断开 + 拼接
//
//   [1,2,3,4,5] k=2
//
//   快指针先走2步:
//     1   2   3   4   5
//     ↑slow  ↑fast
//
//   一起走到 fast 到末尾:
//     1   2   3   4   5
//             ↑slow      ↑fast
//
//   slow=3(新尾巴), slow->next=4(新头)
//   fast=5(旧尾巴), fast->next = head(1)(旧尾接旧头)
//   slow->next = NULL (断开)
//   结果: 4 → 5 → 1 → 2 → 3 → NULL  ✓
// ============================================================
class Solution3 {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;
        
        // 必须先求长度，因为 k 可能远大于 n
        int n = 0;
        ListNode* cur = head;
        while (cur) {
            n++;
            cur = cur->next;
        }
        k %= n;
        if (k == 0) return head;
        
        // 快指针先走 k 步
        ListNode* fast = head;
        for (int i = 0; i < k; i++) {
            fast = fast->next;
        }
        
        // 快慢一起走，直到快指针到最后一个节点
        ListNode* slow = head;
        while (fast->next) {
            fast = fast->next;
            slow = slow->next;
        }
        
        // slow 是新尾巴，fast 是旧尾巴
        ListNode* newHead = slow->next;
        slow->next = nullptr;       // 在新尾巴处断开
        fast->next = head;           // 旧尾巴接上旧头
        
        return newHead;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间          | 空间 | 核心操作         | 遍历次数 |
// |------------|---------------|------|------------------|----------|
// | 暴力模拟   | O(n·(k%n))    | O(1) | 每次移一个尾节点 | 1+k%n次  |
// | 成环断开   | O(n)          | O(1) | 成环+走到断点    | 2次      |
// | 快慢指针   | O(n)          | O(1) | 快慢指针差值定位 | 2次      |
//
// 成环法代码最简洁，快慢指针法概念更直觉。
// 两者本质相同：都是找第 n-k 个节点作为新尾巴。
//
// ============================================================
// 【易错点】
//
// 1. 忘记 k %= n
//    ✗ 直接 for(i < k) 走 k 步 — 当 k=2×10⁹ 时超时或走过头
//    ✓ k %= n; if(k==0) return head;
//
// 2. 求长度时丢失尾节点
//    ✗ n=0, while(tail) {n++; tail=tail->next;} — 结束时 tail==NULL
//    ✓ n=1, while(tail->next) {n++; tail=tail->next;} — 结束时 tail 在最后节点
//
// 3. 断点位置差一
//    ✗ 从 tail 走 n-k-1 步（少走一步，断错位置）
//    ✓ 从 tail 走 n-k 步。验证: n=5,k=2, 从5走3步到3，节点3确实是新尾巴
//
// 4. 忘记处理 k%n==0
//    ✗ 成环后走 n-0=n 步回到原位，但链表已经被改成环了没断开
//    ✓ 取模后立即检查，k==0 直接返回原 head
//
// 5. 空链表 / 单节点
//    ✗ 直接访问 head->next 空指针崩溃
//    ✓ 开头检查 if(!head || !head->next) return head;
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 暴力解的时间复杂度？能接受吗？
//     → O(n·(k%n))，最坏 O(n²)。n≤500可以过，但不是最优。
//
// Q2: 怎么优化到 O(n)？核心观察是什么？
//     → 旋转结果就是在某点断开重连。成环或快慢指针一次定位断点。
//
// Q3: 如果改成"向左旋转 k 位"代码怎么改？
//     → 向左k = 向右(n-k)。取模后 k = n - k，其余不变。
//
// Q4: 如果是数组而不是链表？
//     → 数组用三次翻转法: reverse(0,n-1), reverse(0,k-1), reverse(k,n-1)
//     → 参见 189. 轮转数组
//
// Q5: 如果需要频繁对同一链表做不同的 k 旋转，怎么优化？
//     → 考虑转成数组操作（O(1) 随机访问），或者用循环数组/deque。
//     → 链表每次旋转至少 O(n)，无法做得更快。
// ============================================================
