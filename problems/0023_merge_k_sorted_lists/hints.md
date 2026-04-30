# 23. 合并 K 个升序链表

## 核心思路

本质上是一个 **多路归并** 问题：k 条有序流，每次从 k 个"当前最小候选"中选出全局最小，接到结果链表后面。瓶颈在于"怎么高效地从 k 个候选中找最小值"。

## 思维链

1. **读完题第一反应**：我会合并两个有序链表（LeetCode 21），能不能复用？→ 把 k 个链表逐个两两合并，每次拿结果和下一个链表合并。
2. **暴力的瓶颈**：逐个合并时，第 1 次合并处理 n 个节点，第 2 次处理 2n 个，…第 k-1 次处理 (k-1)n 个 → 总工作量 O(k²n)，k 很大时太慢。
3. **优化方向 A — 分治**：不要"1 个 vs 1 个"地合并，而是像归并排序一样两两配对、分层合并。每层处理 O(N) 个节点，共 log k 层 → O(N log k)。
4. **优化方向 B — 最小堆**：换个角度想，k 条链表各有一个"头指针"，每次需要在 k 个候选中选最小的。暴力扫描 k 个是 O(k)，用最小堆维护这 k 个候选，选最小只需 O(log k) → 总 O(N log k)。
5. **两种最优解殊途同归**：分治和堆的时间复杂度都是 O(N log k)，但思考方式不同——分治是"递归拆问题"，堆是"实时选最小"。面试中两种都要能讲。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力逐个合并 | 依次把 k 个链表合并到结果中 | O(k²n) | O(1) | 能说出即可 |
| 最小堆（优先队列） | 维护大小为 k 的最小堆，每次弹出最小节点 | O(N log k) | O(k) | ⭐ 必须写出 |
| 分治归并 | 两两配对递归合并，类似归并排序 | O(N log k) | O(log k) | ⭐ 必须写出 |

> N = 所有节点总数，n = 每条链表平均长度，k = 链表个数，N = k·n

## 关键提示

1. **从 LeetCode 21 出发**：你已经会合并 2 个有序链表了。本题的核心问题是"怎么高效地把 k 个的问题归结到 2 个的问题"。

2. **堆的直觉**：想象 k 条传送带，每条上面的物品按重量从小到大排列。你站在出口处，每次需要从 k 条传送带的"最前面"那个物品中选最轻的。手动比较 k 个太慢，用一个"自动排序的容器"(堆)来帮你。

3. **分治的直觉**：k 个链表合并，和归并排序的 merge 阶段一样——先两两合并变成 k/2 个，再两两合并变成 k/4 个，… 直到只剩 1 个。

4. **画图理解分治层数**：

```
// k=8 个链表的分治合并过程:
// 第0层: [L1] [L2] [L3] [L4] [L5] [L6] [L7] [L8]    (8个)
//          \/       \/       \/       \/
// 第1层: [L1+L2] [L3+L4] [L5+L6] [L7+L8]             (4个)
//           \     /           \     /
// 第2层:  [L1..L4]          [L5..L8]                   (2个)
//              \              /
// 第3层:        [L1..L8]                               (1个)
// 
// 共 log₂(8) = 3 层，每层处理 N 个节点 → 总 O(N·log k)
```

5. **堆中存什么**：不要把所有节点都丢进堆（那就变成排序了，O(N log N)）。只存 k 个链表的"当前头节点"，弹出一个后，把它的 next 推入堆。堆始终维持最多 k 个元素。

## 解法详解

### 解法1: 暴力逐个合并 — O(k²n) / O(1)

**思考过程**：最直觉的想法——我已经会合并两个有序链表，那就把 k 个链表一个一个合并进去。先合并第 1、2 个，结果再和第 3 个合并，再和第 4 个合并……

```
// 合并过程 (k=4, 每条链表约 n 个节点):
// Step1: merge(L1, L2)        → 处理 2n 个节点
// Step2: merge(结果, L3)      → 处理 3n 个节点
// Step3: merge(结果, L4)      → 处理 4n 个节点
// 总工作: 2n + 3n + 4n = 9n ≈ O(k²n)
// 一般: n(2+3+...+k) = O(k²n)
```

```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        
        ListNode* result = lists[0];
        // 逐个合并：result 越来越长，后面的合并越来越慢
        for (int i = 1; i < lists.size(); i++) {
            result = mergeTwoLists(result, lists[i]);
        }
        return result;
    }
    
private:
    // 复用 LeetCode 21 的合并两个有序链表
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
```

**关键点**：这个解法的瓶颈在于"前面已经合并好的结果链表"会被反复扫描。第 i 次合并时，结果链表已经有 i·n 个节点，每次都要全部走一遍。

---

### 解法2: 最小堆（优先队列） — O(N log k) / O(k) ⭐ 面试首选

**从解法1优化**：解法1每次只能和"一个"链表合并。换个思路——k 个链表各有一个当前最小值（头节点），我们每次在 k 个候选中取全局最小，接到结果后面，然后该链表前进一步。瓶颈是"从 k 个候选中找最小"，暴力要 O(k)，用最小堆只要 O(log k)。

```
// 堆的工作过程 (lists = [[1,4,5],[1,3,4],[2,6]]):
//
// 初始堆 (存各链表头节点): {1, 1, 2}   (最小堆, 堆顶=1)
//
// Step1: 弹出 1(来自L1), 结果: 1→
//        L1 前进到 4, 推入堆 → 堆: {1, 2, 4}
//
// Step2: 弹出 1(来自L2), 结果: 1→1→
//        L2 前进到 3, 推入堆 → 堆: {2, 3, 4}
//
// Step3: 弹出 2(来自L3), 结果: 1→1→2→
//        L3 前进到 6, 推入堆 → 堆: {3, 4, 6}
//
// Step4: 弹出 3(来自L2), 结果: 1→1→2→3→
//        L2 前进到 4, 推入堆 → 堆: {4, 4, 6}
//
// Step5: 弹出 4(来自L1), 结果: 1→1→2→3→4→
//        L1 前进到 5, 推入堆 → 堆: {4, 5, 6}
//
// Step6: 弹出 4(来自L2), 结果: 1→1→2→3→4→4→
//        L2 到头(null), 不推入 → 堆: {5, 6}
//
// Step7: 弹出 5(来自L1), 结果: 1→1→2→3→4→4→5→
//        L1 到头(null), 不推入 → 堆: {6}
//
// Step8: 弹出 6(来自L3), 结果: 1→1→2→3→4→4→5→6→
//        L3 到头(null), 不推入 → 堆: {} 空 → 结束
```

```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 自定义比较器：val 小的优先级高（小顶堆）
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;  // 注意：priority_queue 中 > 表示小顶堆
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
        
        // 把每条链表的头节点推入堆（跳过空链表）
        for (auto* head : lists) {
            if (head) pq.push(head);
        }
        
        ListNode dummy(0);
        ListNode* tail = &dummy;
        
        while (!pq.empty()) {
            // 弹出当前全局最小的节点
            ListNode* smallest = pq.top();
            pq.pop();
            
            // 接到结果链表尾部
            tail->next = smallest;
            tail = tail->next;
            
            // 该链表前进一步，如果还有节点就推入堆
            if (smallest->next) {
                pq.push(smallest->next);
            }
        }
        
        return dummy.next;
    }
};
```

**关键点**：
- 堆中始终最多 k 个元素（每条链表最多一个代表），所以每次 push/pop 是 O(log k)
- 总共弹出 N 个节点，所以总时间 O(N log k)
- `a->val > b->val` 是小顶堆——priority_queue 默认是大顶堆，比较器返回 true 表示"a 的优先级比 b 低"

---

### 解法3: 分治归并 — O(N log k) / O(log k) ⭐ 必须写出

**换个角度**：与其"k 个中选最小"，不如把问题拆分——把 k 个链表分成两半，各自递归合并成一个链表，最后合并两个有序链表。这就是归并排序的思想。

```
// 分治过程 (k=4):
//
//   merge([L1, L2, L3, L4])
//       /                \
//  merge([L1, L2])    merge([L3, L4])
//     /    \             /    \
//   L1     L2          L3     L4
//     \    /             \    /
//  mergeTwoLists      mergeTwoLists
//       \                /
//       mergeTwoLists
//            |
//         最终结果
//
// 每层合并处理 N 个节点，共 log k 层 → O(N log k)
```

```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        return divideAndConquer(lists, 0, lists.size() - 1);
    }
    
private:
    ListNode* divideAndConquer(vector<ListNode*>& lists, int left, int right) {
        // 只有一个链表，直接返回
        if (left == right) return lists[left];
        
        // 分：从中间劈开
        int mid = left + (right - left) / 2;
        
        // 治：递归合并左右两半
        ListNode* l1 = divideAndConquer(lists, left, mid);
        ListNode* l2 = divideAndConquer(lists, mid + 1, right);
        
        // 合：合并两个有序链表
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
```

**关键点**：
- 递归深度是 O(log k)，这就是空间复杂度（调用栈）
- 和解法1的区别：解法1是"串行合并"(1+2→结果+3→…)，分治是"并行两两合并"
- 分治的好处：每条链表被扫描的次数从 O(k) 降到 O(log k)

---

## 解法对比

| | 逐个合并 | 最小堆 | 分治归并 |
|------|---------|--------|----------|
| **时间** | O(k²n) | O(N log k) | O(N log k) |
| **空间** | O(1) | O(k) 堆 | O(log k) 栈 |
| **思维** | 最直觉 | "实时选最小" | "递归拆问题" |
| **代码量** | 少 | 中等 | 中等 |
| **面试推荐** | 说思路即可 | ⭐ 首选 | ⭐ 首选 |

- **堆 vs 分治怎么选**：面试时两种都能写。堆的思路更直观（每次选最小），分治的代码更简洁（复用 mergeTwoLists）。如果面试官问"还有别的方法吗"，能写出第二种就是加分项。
- **堆的优势**：适合流式处理（新链表可以随时加入）
- **分治的优势**：不需要额外数据结构，空间更优

## 易错点

1. **堆的比较器方向写反**
   - ✗ `return a->val < b->val;` → 变成大顶堆，每次取的是最大值
   - ✓ `return a->val > b->val;` → C++ priority_queue 中 `>` 才是小顶堆
   - 记忆法：priority_queue 的 compare 和 sort 的 compare 含义相反

2. **忘记跳过空链表**
   - ✗ 直接 `for (auto* h : lists) pq.push(h);` → 如果 lists 中有 nullptr，push 进堆后解引用会崩溃
   - ✓ `if (head) pq.push(head);`

3. **分治的递归终止条件**
   - ✗ 没有处理 `lists` 为空的情况 → `right = lists.size() - 1` 在空 vector 时下溢为一个巨大的正数（size_t 无符号）
   - ✓ 在调用 `divideAndConquer` 之前先检查 `lists.empty()`

4. **合并两个链表时忘记 dummy 节点**
   - ✗ 直接用 `result` 指针，需要额外处理"第一个节点"的特殊情况
   - ✓ 用 `dummy` 节点统一处理，最后返回 `dummy.next`

## 面试追问

**Q1: 暴力解的时间复杂度怎么分析？为什么是 O(k²n) 不是 O(kn)？**
> 每次合并时，已合并的结果链表越来越长。第 i 次合并处理约 i·n 个节点，总和 = n·(2+3+…+k) ≈ O(k²n/2) = O(k²n)。

**Q2: 堆解法中，为什么不把所有节点一次性扔进堆？**
> 如果所有 N 个节点都入堆，每次操作 O(log N)，总时间 O(N log N)。而只维护 k 个头节点，每次操作 O(log k)，总时间 O(N log k)。由于 k ≤ N，所以 O(N log k) ≤ O(N log N)，当 k 远小于 N 时优势更明显。

**Q3: 如果 k 非常大（比如 10^6）但每条链表只有几个元素，哪种解法更好？**
> 这时 N ≈ k，堆和分治都是 O(N log k) ≈ O(k log k)。但堆解法的常数更大（堆操作有较多 overhead），分治可能更快。另外如果 k 极大，可以考虑"把所有节点收集到数组排序再串链表"，O(N log N) = O(k log k)，代码更简单。

**Q4: 能否用迭代而非递归实现分治？**
> 可以。类似归并排序的自底向上版本：步长从 1 开始，每轮把相邻两个链表合并，步长翻倍，直到只剩一个。这样不需要递归栈，空间 O(1)。

## 相关题型

- **21. 合并两个有序链表** — 本题的基础积木。本题所有解法都复用了 `mergeTwoLists`。
- **148. 排序链表** — 同样用分治（归并排序）处理链表，区别是 148 需要自己"分"（快慢指针找中点），本题的"分"天然是按链表个数分。
- **264. 丑数 II / 373. 查找和最小的 K 对数字** — 同样是"多路归并"模型，用堆从多个有序序列中选最小值。复用本题堆的模板。
- **355. 设计推特** — 每个用户的推文按时间排序，获取 timeline 就是合并 k 个有序链表。