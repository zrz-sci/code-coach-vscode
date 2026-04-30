# 895. 最大频率栈

## 核心思路

本质上是一个**数据结构设计问题**：需要维护一个特殊的栈，pop 时弹出频率最高的元素（频率相同时弹出最近 push 的）。关键洞察是用**频率分桶**——按频率建多个栈，每个频率对应一个栈，pop 时从最高频率的栈顶弹出。

## 思维链

1. **读完题第一反应**：push 容易，难点在 pop——需要快速找到频率最高的元素，频率相同时还要找最近 push 的。这像是需要某种优先队列或特殊索引。

2. **堆/优先队列方案**：用 max-heap，按 (频率, push 时间) 排序。push 时 O(log n)，pop 时 O(log n)。可行但不是最优。

3. **频率分桶方案（更优）**：维护一个 `freq_map` 记录每个值的当前频率，以及一个 `group` 其中 `group[f]` 是一个栈，存储所有当前频率为 f 的元素（按 push 顺序）。同时维护 `maxFreq` 记录当前最大频率。
   - **push(val)**：`freq_map[val]++`，然后把 val 压入 `group[freq_map[val]]`，更新 `maxFreq`。
   - **pop()**：从 `group[maxFreq]` 弹出栈顶，把该值的频率减 1，如果 `group[maxFreq]` 空了就 `maxFreq--`。

4. **为什么频率分桶是对的？** 一个元素 val 如果频率为 3，它会同时出现在 group[1]、group[2]、group[3] 中。pop 从 group[3] 弹出它后，它在 group[2] 和 group[1] 中的记录仍然有效——这正好模拟了"频率降低"后它应该按频率 2 参与下一次 pop。

5. **复杂度分析**：push 和 pop 都是 O(1)。空间 O(n)，其中 n 是总 push 次数。

## 解法概览

| 解法 | 思路 | push 时间 | pop 时间 | 空间 | 面试 |
|------|------|----------|---------|------|------|
| 频率分桶 | 按频率建栈组 | O(1) | O(1) | O(n) | ⭐ 面试首选 |
| 优先队列 | max-heap 按(频率,时间)排序 | O(log n) | O(log n) | O(n) | 备选方案 |

> n = 总 push 次数

## 关键提示

1. **频率分桶的核心思想**：把所有元素按当前频率分组，`group[f]` 是一个栈，存储当前频率为 f 的元素。一个元素如果 push 了 3 次，它会分别出现在 group[1]、group[2]、group[3] 中。

2. **为什么同一个元素出现在多个频率栈中？** 因为 push(5) 第一次时 freq=1 进入 group[1]，第二次 freq=2 进入 group[2]，第三次 freq=3 进入 group[3]。pop 时从 group[3] 弹出 5，它的频率变为 2，而它在 group[2] 中的位置保持不变，下次如果 maxFreq=2 就能正确弹出。

3. **maxFreq 只会减 1**：pop 后如果 group[maxFreq] 为空，maxFreq 只需减 1（不需要搜索下一个非空频率）。因为 group[maxFreq-1] 一定非空——maxFreq 频率的元素在 push 到频率 maxFreq 之前，必然经历过频率 maxFreq-1。

4. **不需要删除 group 中的空栈**：即使 group[maxFreq] 变空了，留着也没关系，因为 maxFreq 会减小，不会再访问它，直到某个元素再次被 push 到这个频率。

5. **优先队列方案的要点**：堆中存 (频率, push 时间戳, 值)，按频率降序、时间降序排序。push 时 O(log n)，pop 时 O(log n)。实现简单但不如频率分桶方案的 O(1) 优。

## 解法详解

### 解法1: 频率分桶 — O(1) push/pop ⭐ 面试首选

**思考过程**：维护三个数据结构：
- `freq`: map，记录每个值的当前频率
- `group`: map，`group[f]` 是一个栈，存储频率为 f 的元素
- `maxFreq`: 当前最大频率

```
push(5): freq[5]=1, group[1].push(5), maxFreq=1
push(7): freq[7]=1, group[1].push(7), maxFreq=1
push(5): freq[5]=2, group[2].push(5), maxFreq=2
push(7): freq[7]=2, group[2].push(7), maxFreq=2
push(4): freq[4]=1, group[1].push(4), maxFreq=2
push(5): freq[5]=3, group[3].push(5), maxFreq=3

此时:
  group[1] = [5, 7, 4]    (栈底到栈顶)
  group[2] = [5, 7]
  group[3] = [5]
  maxFreq = 3

pop(): maxFreq=3, 弹出 group[3].top()=5, freq[5]=2, group[3]空 → maxFreq=2
pop(): maxFreq=2, 弹出 group[2].top()=7, freq[7]=1, group[2]非空 → maxFreq=2
pop(): maxFreq=2, 弹出 group[2].top()=5, freq[5]=1, group[2]空 → maxFreq=1
pop(): maxFreq=1, 弹出 group[1].top()=4, freq[4]=0, group[1]非空 → maxFreq=1
```

```cpp
class FreqStack {
public:
    unordered_map<int, int> freq;              // val -> 当前频率
    unordered_map<int, stack<int>> group;       // 频率 -> 该频率的元素栈
    int maxFreq = 0;
    
    FreqStack() {}
    
    void push(int val) {
        int f = ++freq[val];
        group[f].push(val);
        maxFreq = max(maxFreq, f);
    }
    
    int pop() {
        int val = group[maxFreq].top();
        group[maxFreq].pop();
        freq[val]--;
        if (group[maxFreq].empty()) {
            maxFreq--;
        }
        return val;
    }
};
```

**关键点**：push 和 pop 都是 O(1)。pop 时 maxFreq 只减 1 是正确的，因为下一层频率栈一定非空。

### 解法2: 优先队列 — O(log n) push/pop

**不同思路**：用 max-heap 存储 (频率, 时间戳, 值)，按频率降序、时间戳降序排序。pop 时堆顶就是频率最高且最近 push 的元素。

```cpp
class FreqStack {
public:
    unordered_map<int, int> freq;
    // {频率, 时间戳, 值}，默认大顶堆
    priority_queue<tuple<int, int, int>> pq;
    int timestamp = 0;
    
    FreqStack() {}
    
    void push(int val) {
        freq[val]++;
        pq.push({freq[val], timestamp++, val});
    }
    
    int pop() {
        auto [f, t, val] = pq.top();
        pq.pop();
        freq[val]--;
        return val;
    }
};
```

**关键点**：tuple 的默认比较是字典序，先比频率（大的优先），再比时间戳（大的优先）。代码更简洁但时间复杂度差一些。

## 解法对比

| | 频率分桶 | 优先队列 |
|--|---------|---------|
| push | O(1) | O(log n) |
| pop | O(1) | O(log n) |
| 空间 | O(n) | O(n) |
| 代码长度 | 中等 | ⭐ 更短 |
| 面试表现 | ⭐ 最优 | 可接受 |

**选择建议**：
- 面试首选频率分桶方案：O(1) 操作，展示对问题结构的深刻理解
- 如果时间紧可以先写优先队列方案：代码更短，然后优化到频率分桶
- 面试官可能追问"能否做到 O(1) pop？"——此时给出频率分桶

## 易错点

1. **pop 后 maxFreq 多减了**
   - x 每次 pop 都 `maxFreq--`，不管 group[maxFreq] 是否为空
   - o 只在 group[maxFreq] 为空时才 `maxFreq--`

2. **push 时忘记更新 maxFreq**
   - x 只更新了 freq 和 group，忘了 `maxFreq = max(maxFreq, f)`
   - o push 后新频率可能超过当前 maxFreq，必须更新

3. **pop 时忘记更新 freq**
   - x 只从 group 弹出，没有 `freq[val]--` -> 下次 pop 频率信息错误
   - o pop 后必须 `freq[val]--`

4. **优先队列方案中 pop 后没删除旧的堆记录**
   - 实际上不需要删除——每次 push 都会加新记录，pop 弹出的是最新的（时间戳最大的），旧记录自然被新记录"覆盖"。但如果实现有误可能导致弹出已过期的记录。

5. **以为 maxFreq 减少后需要搜索下一个非空频率**
   - x 写了 `while (group[maxFreq].empty()) maxFreq--` -> 虽然结果正确但多此一举
   - o maxFreq 只需减 1，因为 group[maxFreq-1] 一定非空

## 面试追问

**Q1: 为什么 group[maxFreq-1] 一定非空？**
-> 假设某个元素 val 的当前频率是 maxFreq，那么在它频率变成 maxFreq 的那次 push 之前，它的频率是 maxFreq-1，此时 val 被 push 到了 group[maxFreq-1]。即使 val 被 pop 了（从 group[maxFreq] 弹出），它在 group[maxFreq-1] 中的记录仍然存在。所以 group[maxFreq-1] 一定至少有一个元素。

**Q2: 频率分桶方案的空间复杂度具体是多少？**
-> O(n)，n 是总 push 次数。每次 push 都在 group 中添加一个元素，pop 时移除一个。freq 的大小最多是不同元素的数量。总空间不超过 O(n)。

**Q3: 如果还需要支持 peek（查看但不弹出最大频率元素），怎么实现？**
-> 直接返回 `group[maxFreq].top()`，O(1) 时间。

**Q4: 如果需要支持 remove(val)（移除某个特定值的一次出现），怎么做？**
-> 频率分桶方案难以高效支持任意删除（需要在栈中间删除元素）。此时优先队列 + 延迟删除可能更合适，用一个 deleted 集合标记被删除的元素，pop 时跳过。

## 相关题型

- **LeetCode 460 LFU 缓存** — 同为频率相关的数据结构设计。LFU 需要在 O(1) 时间内淘汰频率最低的元素，本题是弹出频率最高的。都可以用频率分桶思想，但 LFU 更复杂（需要维护双向链表+哈希表）。
- **LeetCode 155 最小栈** — 设计题，栈+辅助栈的经典模式。本题是栈+频率映射，思路有相似之处。
- **LeetCode 347 前 K 个高频元素** — 频率统计 + 选择，可以用桶排序（类似频率分桶）实现 O(n) 解法。
- **LeetCode 146 LRU 缓存** — 经典设计题，和 LFU/FreqStack 一起构成数据结构设计三件套。
