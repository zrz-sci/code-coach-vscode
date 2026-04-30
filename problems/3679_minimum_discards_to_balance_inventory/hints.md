# 3679. 使库存平衡的最少丢弃次数 - 提示与详解

## 难度：Medium | 标签：Array, Hash Table, Counting, Sliding Window, Simulation

---

## 一、核心思路

本题本质是一个 **滑动窗口 + 贪心** 问题。对每种物品类型维护一个保留记录队列，追踪窗口内该类型被保留的天数。当新物品到达时，先清除窗口外的过期记录，再判断是否需要丢弃。

**核心洞察**：每个物品要么在到达当天被丢弃，要么永远保留。为了最小化丢弃数量，我们应该贪心地尽量保留每个物品 —— 只有当保留会违反约束时才丢弃。这个贪心策略是最优的，因为保留当前物品不会让未来的情况更糟（同类型的过期时间更早）。

---

## 二、思维链：从题目到解法

### Step 1：理解窗口约束

```
窗口定义: 对于第 i 天, 窗口为 [max(1, i-w+1), i]
即最近 w 天内（含第 i 天本身）

例: arrivals = [1,2,3,3,3,4], w=3, m=2

天   1    2    3    4    5    6
到达  1    2    3    3    3    4

第3天窗口: [1,2,3]  → 天1到天3
第4天窗口: [2,3,4]  → 天2到天4
第5天窗口: [3,4,5]  → 天3到天5
第6天窗口: [4,5,6]  → 天4到天6

约束: 每个窗口内, 同类型物品被保留的数量 <= m
```

### Step 2：为什么贪心是最优的？

```
假设在第 i 天到达类型 t 的物品, 当前窗口中类型 t 已有 cnt 个保留:

情况1: cnt < m → 可以保留
  保留不会影响其他类型, 也不会影响更早的物品
  对于未来的同类物品: 当前这个会比之前的更晚过期
  → 如果要丢弃, 应该丢弃更早的吗?
  → 不行! 更早的已经保留了, 不能撤回

情况2: cnt >= m → 必须丢弃
  窗口约束被违反, 没有选择余地

关键: 每个物品只能在到达当天决定保留或丢弃（不可撤回）
贪心策略: 能保留就保留 → 这就是最优解
```

### Step 3：为什么不需要"撤回"之前保留的物品？

```
反证法: 假设在第 i 天丢弃当前物品更好
（即使窗口中该类型数量 < m）

那么保留它只会影响未来 [i+1, i+w-1] 天内同类型物品
如果未来某天因为今天保留而必须丢弃:
  → 那一天丢弃 vs 今天丢弃, 丢弃总数相同
  → 但今天保留了, 实际上不会更差

直觉: "尽量保留"永远不比"提前丢弃"更差
数学: 丢弃总数是单调的 — 保留越多, 丢弃越少
```

### Step 4：数据结构选择

```
需求分析:
1. 对每种类型, 记录被保留的日期
2. 快速移除窗口外的过期日期 → 队列前端出队
3. 快速查询窗口内保留数量 → 队列长度
4. 新保留日期总是最大的 → 队列后端入队

→ 完美匹配 deque (双端队列) 的特性!

数据结构: HashMap<类型, Deque<日期>>
- key = 物品类型
- value = 该类型被保留的日期列表（按时间顺序）
```

### Step 5：完整模拟过程

```
arrivals = [1,2,3,3,3,4], w=3, m=2

第1天: type=1
  kept[1] = []  (清理后为空)
  size=0 < m=2 → 保留, kept[1] = [0]
  discards = 0

第2天: type=2
  kept[2] = []
  size=0 < 2 → 保留, kept[2] = [1]
  discards = 0

第3天: type=3
  kept[3] = []
  size=0 < 2 → 保留, kept[3] = [2]
  discards = 0

第4天: type=3
  kept[3] = [2]  (清理: 2 >= 4-3+1=2, 不过期)
  size=1 < 2 → 保留, kept[3] = [2, 3]
  discards = 0

第5天: type=3
  kept[3] = [2, 3]  (清理: 2 < 5-3+1=3, 弹出!)
  kept[3] = [3]      (3 >= 3, 不过期)
  size=1... 等等, 让我重新检查

  0-indexed: i=4, w=3
  过期条件: 日期 < i - w + 1 = 4 - 3 + 1 = 2
  kept[3] = [2, 3]
  2 < 2? No → 不弹出
  size=2 >= m=2 → 必须丢弃!
  discards = 1

第6天: type=4
  kept[4] = []
  size=0 < 2 → 保留, kept[4] = [5]
  discards = 1

最终结果: 1 ✓
```

---

## 三、解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|------|-----------|-----------|----------|
| ⭐ 滑动窗口+队列 | 每种类型维护保留日期deque | O(n) | O(n) | 最优解 |
| 暴力模拟 | 每天重建窗口统计 | O(n*w) | O(n) | 理解题意 |

---

## 四、关键提示（逐步递进）

<details>
<summary>提示 1：贪心方向是什么？</summary>

尽量保留每个到达的物品。只有当保留会导致窗口内同类型超过 m 个时，才丢弃。因为已经保留的物品无法撤回，而当前保留不会让未来更差。

</details>

<details>
<summary>提示 2：用什么数据结构追踪每种类型？</summary>

对每种类型维护一个 deque（双端队列），存放该类型被保留的天数索引。新保留的天数从尾部入队，过期的天数从头部出队。队列长度就是当前窗口内该类型的保留数量。

</details>

<details>
<summary>提示 3：如何判断"窗口过期"？</summary>

使用 0-indexed 索引时，第 i 天的窗口起始位置是 `i - w + 1`。队列中所有日期 < `i - w + 1` 的记录都已经过期，需要从队列头部弹出。

</details>

<details>
<summary>提示 4：为什么时间复杂度是 O(n)？</summary>

每个元素最多被 push 一次、pop 一次。虽然外层遍历 n 次，内层 while 循环总次数不超过 n（因为每个入队的元素最多出队一次），所以总时间是 O(n)，而不是 O(n*w)。

</details>

<details>
<summary>提示 5：注意 0-indexed 和 1-indexed 的转换</summary>

题目说天数从 1 开始，但数组下标从 0 开始。可以直接用 0-indexed 处理，窗口起始位置为 `max(0, i - w + 1)`。过期条件是 `日期 < i - w + 1`。

</details>

---

## 五、解法详解

### 解法一：滑动窗口 + 队列（最优解）

```cpp
class Solution {
public:
    int minArrivalsToDiscard(vector<int>& arrivals, int w, int m) {
        // 每种类型 -> 保留日期的队列
        unordered_map<int, deque<int>> kept;
        int discards = 0;

        for (int i = 0; i < (int)arrivals.size(); i++) {
            int type = arrivals[i];
            auto& q = kept[type];

            // Step 1: 弹出窗口外的过期日期
            // 0-indexed: 窗口起始 = i - w + 1
            while (!q.empty() && q.front() < i - w + 1) {
                q.pop_front();
            }

            // Step 2: 判断是否可以保留
            if ((int)q.size() >= m) {
                // 窗口内已有 m 个同类型, 必须丢弃
                discards++;
            } else {
                // 可以保留
                q.push_back(i);
            }
        }

        return discards;
    }
};
```

**执行过程可视化** (Example 2):

```
arrivals = [1,2,3,3,3,4], w=3, m=2

i=0: type=1, q=[]
     过期清理: 无
     q.size()=0 < 2 → 保留, q=[0]
     discards=0

i=1: type=2, q=[]
     过期清理: 无
     q.size()=0 < 2 → 保留, q=[1]
     discards=0

i=2: type=3, q=[]
     过期清理: 无
     q.size()=0 < 2 → 保留, q=[2]
     discards=0

i=3: type=3, q=[2]
     过期清理: 2 < 3-3+1=1? No
     q.size()=1 < 2 → 保留, q=[2,3]
     discards=0

i=4: type=3, q=[2,3]
     过期清理: 2 < 4-3+1=2? No → 不弹出
     q.size()=2 >= 2 → 丢弃!
     discards=1

i=5: type=4, q=[]
     过期清理: 无
     q.size()=0 < 2 → 保留, q=[5]
     discards=1

答案: 1 ✓
```

### 解法二：暴力模拟（用于理解和验证）

```cpp
class Solution {
public:
    int minArrivalsToDiscard(vector<int>& arrivals, int w, int m) {
        int n = arrivals.size();
        vector<bool> kept(n, false); // 记录每天物品是否被保留
        int discards = 0;

        for (int i = 0; i < n; i++) {
            // 统计当前窗口内该类型被保留的数量
            int count = 0;
            int start = max(0, i - w + 1);
            for (int j = start; j < i; j++) {
                if (kept[j] && arrivals[j] == arrivals[i]) {
                    count++;
                }
            }

            if (count < m) {
                kept[i] = true; // 保留
            } else {
                discards++; // 丢弃
            }
        }

        return discards;
    }
};
```

---

## 六、窗口滑动图解

```
arrivals = [1, 2, 1, 3, 1], w=4, m=2

天:        0    1    2    3    4
到达:      1    2    1    3    1
                                    
窗口演变:
i=0: [1]                           kept[1]=[0]
i=1: [1, 2]                        kept[2]=[1]
i=2: [1, 2, 1]                     kept[1]=[0,2] (size=2, 刚好=m)
i=3: [1, 2, 1, 3]                  kept[3]=[3]
i=4:    [2, 1, 3, 1]               kept[1]=[0,2] → 清理0(0<4-4+1=1) → [2]
                                    size=1 < 2 → 保留, kept[1]=[2,4]

所有物品都被保留, discards=0 ✓

窗口内类型1的数量变化:
i=0: 窗口[0,0]:    1个  ← 保留
i=2: 窗口[0,2]:    2个  ← 保留（刚好=m）
i=4: 窗口[1,4]:    先清理i=0 → 1个 → 保留 → 2个
```

---

## 七、复杂度分析

### 时间复杂度：O(n)

```
外层循环: n 次
内层 while 清理过期:
  每个元素最多入队1次、出队1次
  总出队次数 <= n
所以总时间 = O(n) + O(n) = O(n)

注意: 这是均摊分析 (amortized analysis)
单次循环可能出队多个元素, 但全局不超过 n 次
```

### 空间复杂度：O(n)

```
HashMap + Deque 存储所有被保留的日期
最坏情况: 所有物品都被保留 → 存储 n 个日期
所以空间 = O(n)

更精确: O(min(n, 类型数 * m * w))
但 O(n) 是简洁的上界
```

---

## 八、易错点

### Bug 1：过期条件写错

```cpp
// 错误: 使用 <= 而非 <
while (!q.empty() && q.front() <= i - w + 1)
// 正确: 窗口起始位 = i-w+1, 只有严格小于才过期
while (!q.empty() && q.front() < i - w + 1)
```

### Bug 2：忘记将丢弃的物品加入队列

```cpp
// 错误: 丢弃时也 push_back
discards++;
q.push_back(i); // 丢弃的不应该加入!

// 正确: 只有保留时才入队
if ((int)q.size() >= m) {
    discards++;
    // 不入队
} else {
    q.push_back(i); // 只有保留才入队
}
```

### Bug 3：混淆 0-indexed 和 1-indexed

```cpp
// 题目说天数从1开始, 但数组从0开始
// 如果用1-indexed:
for (int i = 1; i <= n; i++) {
    while (!q.empty() && q.front() < i - w + 1) // 这里的 i 和 w 要一致
}
// 建议统一用 0-indexed, 更不容易出错
```

### Bug 4：size() 返回 unsigned int 与 m 比较

```cpp
// 潜在问题: size() 返回 size_t (unsigned), m 是 int
if (q.size() >= m) // 当 m 为负数时会有问题（虽然约束中 m>=1）
// 安全写法:
if ((int)q.size() >= m)
```

---

## 九、面试追问（递进链）

### Q1：为什么贪心策略能保证最优？

可以用交换论证证明：假设存在一个最优方案 A，在第 i 天丢弃了一个本可以保留的物品（窗口内该类型 < m 个），那么把它改为保留，未来的决策不会变得更差。因为保留它最坏的影响是让未来某天多一个同类型在窗口内，但那天本来就会做出正确的判断。所以贪心方案的丢弃数 <= 最优方案。

### Q2：如果物品可以在任意天丢弃（不仅仅是到达当天），会怎样？

问题变得更复杂 — 需要考虑是否"延迟丢弃"更优。但实际上答案不变，因为尽量保留的贪心仍然是最优的。只是如果允许"撤回保留"，可能需要更复杂的策略。

### Q3：如果要返回具体哪些物品被丢弃了呢？

只需在丢弃时记录下标即可。将 `discards++` 改为 `result.push_back(i)` 就能记录所有被丢弃的天数。

### Q4：如果 w 和 m 会随天数变化（每天的约束不同），怎么办？

队列方法仍然适用，只需在每天用当天的 w 值清理过期、用当天的 m 值判断是否丢弃。时间复杂度不变。

### Q5：这道题和 LRU Cache 有什么联系？

两者都涉及"滑动窗口中的资源管理"和"按时间顺序淘汰"的思想。LRU 按访问时间淘汰，本题按到达时间在窗口外自动过期。

---

## 十、相关题型

| 题号 | 题名 | 关联点 |
|------|------|--------|
| 239 | Sliding Window Maximum | 滑动窗口 + deque 模板 |
| 904 | Fruit Into Baskets | 滑动窗口 + HashMap 统计 |
| 1438 | Longest Continuous Subarray With Abs Diff <= Limit | 滑动窗口 + deque |
| 340 | Longest Substring with At Most K Distinct Characters | 窗口内类型计数 |
| 146 | LRU Cache | 时间顺序淘汰机制 |

---

## 十一、总结

```
           3679. Minimum Discards to Balance Inventory
                         |
              贪心: 能保留就保留
                         |
              每种类型维护 deque<日期>
                    /              \
         清理过期日期              判断是否保留
         front < i-w+1             size < m ?
              |                   /        \
         pop_front()          Yes: 保留     No: 丢弃
                           push_back(i)    discards++
```

**记忆口诀**：类型配队列，过期先弹出；没超就保留，超了必丢弃。
