/*
 * LeetCode 23: 合并 K 个升序链表 (Merge k Sorted Lists)
 *
 * 【题目本质】
 * 多路归并问题：k 条有序链表，合并成一条有序链表。
 * 核心挑战是"怎么高效地从 k 个候选头节点中选出最小的"。
 *
 * 【解法总览】
 * 解法1: 暴力逐个合并  — O(k²n) / O(1)    — 最直觉，面试开场
 * 解法2: 最小堆        — O(N logk) / O(k)  — 面试首选
 * 解法3: 分治归并      — O(N logk) / O(logk) — 面试首选
 * 解法4: 迭代分治(自底向上) — O(N logk) / O(1) — 加分项
 *
 * 其中 N = 所有节点总数, k = 链表个数, n = 平均每条链表长度
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
// 解法1: 暴力逐个合并 — O(k²n) / O(1)
//
// 【思路】
// 已经会合并两个有序链表(LeetCode 21)，复用它。
// 把 k 个链表一个一个合并进去：
//   result = merge(L1, L2)
//   result = merge(result, L3)
//   result = merge(result, L4)
//   ...
//
// 【为什么是 O(k²n)】
// 第 i 次合并时，result 有 ~i·n 个节点，每次都要完整扫描。
// 总工作量 = n(2+3+...+k) = O(k²n)
//
// 合并过程示意 (k=3, 各链表 n 个节点):
//
//  L1: 1→4→5
//  L2: 1→3→4       merge(L1,L2) → 1→1→3→4→4→5  (扫描 2n=6 个)
//  L3: 2→6         merge(结果,L3) → 1→1→2→3→4→4→5→6  (扫描 3n=8 个)
//
//  总扫描: 6+8 = 14 ≈ O(k²n)
// ============================================================
class Solution1 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        
        ListNode* result = lists[0];
        for (int i = 1; i < (int)lists.size(); i++) {
            result = mergeTwoLists(result, lists[i]);
        }
        return result;
    }

private:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // dummy 节点避免处理头节点的特殊情况
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

// ============================================================
// 解法2: 最小堆（优先队列） — O(N log k) / O(k)
//
// 【思路】
// 解法1的瓶颈：每次只和一条链表合并，result 被反复扫描。
// 换个角度：k 条链表各有一个"当前头节点"，我们需要反复从
// k 个候选中选出最小的。
//
// 暴力选最小要 O(k)，总 O(Nk)；
// 用最小堆维护 k 个候选，选最小只要 O(log k)，总 O(N log k)。
//
// 【过程示意】
//
//  初始: 三条链表的头节点入堆
//  L1: [1]→4→5     L2: [1]→3→4     L3: [2]→6
//                   堆: {1(L1), 1(L2), 2(L3)}
//
//  Step1: 弹出 1(L1), result: 1→
//         L1 前进到 4, 推入堆 → 堆: {1(L2), 2(L3), 4(L1)}
//
//  Step2: 弹出 1(L2), result: 1→1→
//         L2 前进到 3, 推入堆 → 堆: {2(L3), 3(L2), 4(L1)}
//
//  Step3: 弹出 2(L3), result: 1→1→2→
//         L3 前进到 6, 推入堆 → 堆: {3(L2), 4(L1), 6(L3)}
//
//  Step4: 弹出 3(L2), result: 1→1→2→3→
//         L2 前进到 4, 推入堆 → 堆: {4(L1), 4(L2), 6(L3)}
//
//  Step5: 弹出 4(L1), result: 1→1→2→3→4→
//         L1 前进到 5, 推入堆 → 堆: {4(L2), 5(L1), 6(L3)}
//
//  Step6: 弹出 4(L2), result: 1→1→2→3→4→4→
//         L2 到头 → 不推入    → 堆: {5(L1), 6(L3)}
//
//  Step7: 弹出 5(L1), result: 1→1→2→3→4→4→5→
//         L1 到头 → 不推入    → 堆: {6(L3)}
//
//  Step8: 弹出 6(L3), result: 1→1→2→3→4→4→5→6
//         L3 到头 → 不推入    → 堆: {}  → 结束
// ============================================================
class Solution2 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 比较器：val 大的优先级低 → 小顶堆
        // 注意：priority_queue 的比较器语义和 sort 相反
        // "a->val > b->val" 返回 true 表示 a 的优先级比 b 低
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
        
        // 只把非空链表的头节点入堆，堆大小始终 ≤ k
        for (auto* head : lists) {
            if (head) pq.push(head);
        }
        
        ListNode dummy(0);
        ListNode* tail = &dummy;
        
        while (!pq.empty()) {
            // O(log k): 弹出全局最小节点
            ListNode* smallest = pq.top();
            pq.pop();
            
            // 接到结果链表尾部
            tail->next = smallest;
            tail = tail->next;
            
            // 该链表前进一步：如果还有后续节点，推入堆
            if (smallest->next) {
                pq.push(smallest->next);
            }
        }
        
        return dummy.next;
    }
};

// ============================================================
// 解法3: 分治归并 (递归) — O(N log k) / O(log k)
//
// 【思路】
// 像归并排序一样，把 k 个链表两两配对递归合并。
// - 分：把 k 个链表从中间劈成两组
// - 治：递归地各自合并成一个链表
// - 合：合并两个有序链表 (LeetCode 21)
//
// 和解法1的关键区别：
// 解法1 是串行合并(线性链式)，每条链表被扫描的次数是 O(k)
// 解法3 是两两合并(树状)，每条链表被扫描的次数是 O(log k)
//
// 【分治过程示意】
//
//  merge([L1, L2, L3, L4])         ← 4个链表
//       /              \
//  merge([L1,L2])  merge([L3,L4])  ← 各2个链表
//    /    \           /    \
//   L1    L2        L3     L4      ← 递归到底
//    \    /           \    /
// mergeTwoLists    mergeTwoLists   ← 合：合并两个有序链表
//       \              /
//      mergeTwoLists               ← 最后合并成一个
//           |
//        最终结果
//
// 每层总共处理 N 个节点，共 log k 层 → O(N log k)
// 递归深度 O(log k) → 空间 O(log k)
// ============================================================
class Solution3 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        return divideAndConquer(lists, 0, (int)lists.size() - 1);
    }

private:
    ListNode* divideAndConquer(vector<ListNode*>& lists, int left, int right) {
        // 只有一个链表，直接返回（递归终止）
        if (left == right) return lists[left];
        
        int mid = left + (right - left) / 2;
        
        // 递归合并左半部分和右半部分
        ListNode* l1 = divideAndConquer(lists, left, mid);
        ListNode* l2 = divideAndConquer(lists, mid + 1, right);
        
        // 合并两个有序链表
        return mergeTwoLists(l1, l2);
    }
    
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

// ============================================================
// 解法4: 迭代分治 (自底向上归并) — O(N log k) / O(1)
//
// 【思路】
// 解法3的递归版需要 O(log k) 栈空间。可以改成自底向上迭代：
// - 步长 interval = 1: 把 (0,1), (2,3), (4,5)... 两两合并
// - 步长 interval = 2: 把 (0,2), (4,6)... 两两合并
// - 步长 interval = 4: ...
// - 直到 interval >= k，此时 lists[0] 就是最终结果
//
// 这和归并排序的自底向上版本思路完全一致。
// 好处：不需要递归栈，额外空间 O(1)。
//
// 【过程示意】(k=5)
//
// 初始: [L0] [L1] [L2] [L3] [L4]
//
// interval=1: merge(L0,L1), merge(L2,L3), L4不动
//   → [L0+L1] [_] [L2+L3] [_] [L4]
//
// interval=2: merge(L0+L1, L2+L3), L4不动
//   → [L0..L3] [_] [_] [_] [L4]
//
// interval=4: merge(L0..L3, L4)
//   → [L0..L4] → 完成
// ============================================================
class Solution4 {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int k = lists.size();
        if (k == 0) return nullptr;
        
        // 步长从 1 开始，每次翻倍
        for (int interval = 1; interval < k; interval *= 2) {
            // 每次跳 2*interval，把相邻的两组合并
            for (int i = 0; i + interval < k; i += interval * 2) {
                lists[i] = mergeTwoLists(lists[i], lists[i + interval]);
            }
        }
        return lists[0];
    }

private:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间      | 空间     | 核心思想         | 适用场景           |
// |-----------|-----------|---------|-----------------|-------------------|
// | 逐个合并   | O(k²n)   | O(1)    | 串行合并         | 面试开场/说思路    |
// | 最小堆     | O(NlogK) | O(k)    | 实时选最小       | 面试首选/流式场景  |
// | 分治(递归) | O(NlogK) | O(logk) | 递归拆问题       | 面试首选          |
// | 分治(迭代) | O(NlogK) | O(1)    | 自底向上归并     | 追问O(1)空间时     |
//
// 堆 vs 分治:
// - 堆更适合"流式"场景(新链表可以动态加入)
// - 分治代码更简洁(直接复用 mergeTwoLists)
// - 迭代分治空间最优 O(1)，适合追问"能不能不用递归"
//
// ============================================================
// 【易错点】
//
// 1. 堆的比较器方向:
//    ✗ return a->val < b->val;  → 大顶堆，每次取最大值，结果降序
//    ✓ return a->val > b->val;  → 小顶堆，每次取最小值
//    C++ priority_queue 的 compare 语义: 返回 true 表示第一个参数优先级更低
//
// 2. 空链表 nullptr 入堆导致崩溃:
//    ✗ for (auto* h : lists) pq.push(h);  → 如果某条链表为空, push nullptr 后
//       弹出时访问 nullptr->val 会 segfault
//    ✓ if (head) pq.push(head);  → 跳过空链表
//
// 3. lists 为空时 size()-1 下溢:
//    ✗ divideAndConquer(lists, 0, lists.size() - 1);
//       → lists 为空时 size()=0, size()-1 = SIZE_MAX (无符号下溢)
//    ✓ 先判断 if (lists.empty()) return nullptr;
//       或转成 int: (int)lists.size() - 1
//
// 4. 迭代分治中忘记检查 i + interval < k:
//    ✗ lists[i + interval] 越界
//    ✓ for 循环条件加 i + interval < k
//
// ============================================================
// 【面试追问】
//
// Q1(基础): "暴力逐个合并的时间为什么是 O(k²n) 不是 O(kn)?"
//    → 第 i 次合并处理 i·n 个节点，总和 = n·Σ(i=2..k) = O(k²n)
//
// Q2(优化): "堆中为什么只存 k 个节点而不是所有 N 个?"
//    → 只存 k 个头节点，每次 push/pop 是 O(logk); 若存 N 个则 O(logN)。
//       因为 k≤N，所以 O(NlogK) ≤ O(NlogN)。且当 k 远小于 N 时优势大。
//
// Q3(变体): "能否 O(1) 额外空间实现 O(NlogK)?"
//    → 用自底向上的迭代分治(解法4)，步长从1开始翻倍，原地在 lists 数组上合并。
//       不需要递归栈，不需要堆，额外空间 O(1)。
//
// Q4(延伸): "如果链表是动态添加的（流式），哪种方案最好?"
//    → 最小堆。新链表加入时只需把头节点 push 进堆即可，不影响已有逻辑。
//       分治需要知道总量才能拆分，不适合流式场景。
// ============================================================
