# 621. 任务调度器

## 核心思路

本质上是一个**填空排列问题**：出现次数最多的任务决定了时间线的"骨架"，其他任务和 idle 填入骨架的空隙中。最终答案取决于"骨架产生的最小长度"和"任务总数"哪个更大。

## 思维链

1. **读完题第一反应**：每种任务之间要隔 n 个间隔，那我能不能模拟这个过程？每次选一个"当前可执行且剩余次数最多"的任务来做 → 这就是贪心+堆的模拟思路。

2. **模拟的本质是什么？** 每一轮（n+1 个时间槽）里，优先做剩余次数最多的任务。因为高频任务最容易产生 idle，优先消耗它们能最大化利用时间槽。

3. **能不能不模拟，直接算？** 观察规律：假设出现最多的任务是 A，出现了 maxFreq 次。那 A 至少需要 `(maxFreq - 1)` 个间隔段，每段长 `n + 1`，再加上最后一批任务。

4. **最后一批是什么？** 就是所有出现次数等于 maxFreq 的任务。比如 A 和 B 都出现 3 次，那最后一个时间段需要放 A 和 B 两个。

5. **公式推导**：`(maxFreq - 1) * (n + 1) + countOfMax`，但如果任务种类很多、n 很小，可能根本不需要 idle，此时答案就是 `tasks.length`。所以最终：`max(公式结果, tasks.length)`。

6. **为什么取 max？** 公式只考虑了最高频任务的约束，但不管怎样，每个任务至少占 1 个时间单位，所以答案不可能比任务总数少。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 贪心+堆模拟 | 每轮 n+1 个槽，优先做频率最高的任务 | O(tasks × log26) ≈ O(tasks) | O(1)（26个字母） | 能说出即可 |
| 贪心+排序模拟 | 每轮排序频率数组，选最多的 n+1 种任务执行 | O(tasks × 26log26) ≈ O(tasks) | O(1) | 能说出即可 |
| 数学公式 | 直接用最高频率计算框架大小 | O(tasks) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **画一个表格**：以示例1 `["A","A","A","B","B","B"], n=2` 为例，把时间轴按 `n+1=3` 为一组分段，看看每段能放什么：
   ```
   | A | B | idle |   ← 第1段 (长度 n+1 = 3)
   | A | B | idle |   ← 第2段
   | A | B |         ← 最后一段（不需要填满）
   总长 = 3 + 3 + 2 = 8
   ```

2. **谁决定了骨架？** 出现次数最多的那个任务。它把时间线分成了 `maxFreq - 1` 个"间隔段"，每个间隔段有 `n` 个空位需要被其他任务或 idle 填满。

3. **什么时候不需要 idle？** 当任务种类很多时，间隔段全部被其他任务填满甚至不够放，此时答案就是 tasks.length，不会出现 idle。

4. **堆模拟方法的关键**：每轮（n+1 个时间单位）从堆中取出最多 n+1 个不同任务执行，执行后频率减 1，非零的再放回堆。一轮结束如果堆非空但取出不到 n+1 个，差额就是 idle。

5. **数学公式的边界**：`countOfMax` 是出现次数 == maxFreq 的任务种类数，不是 maxFreq 本身。

## 解法详解

### 解法1: 贪心+堆模拟 — O(N) / O(1)

**思考过程**：最直觉的贪心策略——每个时间单位，在所有"当前可执行"的任务中，选剩余次数最多的那个来做。为什么？因为高频任务是产生 idle 的根源，越早消耗它们，后面越不容易产生空闲。

用最大堆维护"各任务的剩余次数"，按 `n+1` 为一轮进行模拟：
- 每轮从堆中取出最多 `n+1` 个任务（频率最高的优先）
- 每个任务频率减 1，非零的攒起来
- 一轮结束后，把非零的放回堆
- 如果这一轮取出的任务不足 `n+1` 个且堆还有剩余，差额就是 idle

```cpp
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        // 统计每种任务的频率
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        // 最大堆，只存非零频率
        priority_queue<int> maxHeap;
        for (int f : freq) {
            if (f > 0) maxHeap.push(f);
        }
        
        int time = 0;
        while (!maxHeap.empty()) {
            vector<int> temp; // 本轮执行后仍有剩余的任务
            int cycle = n + 1; // 每轮有 n+1 个时间槽
            
            // 在一轮中尽可能多地执行不同任务
            for (int i = 0; i < cycle && !maxHeap.empty(); i++) {
                int top = maxHeap.top(); maxHeap.pop();
                if (top - 1 > 0) {
                    temp.push_back(top - 1); // 还有剩余，暂存
                }
                time++; // 执行了一个任务
            }
            
            // 把暂存的放回堆
            for (int t : temp) maxHeap.push(t);
            
            // 如果堆还有任务但本轮没填满，需要 idle 补齐
            if (!maxHeap.empty()) {
                time += (cycle - (int)temp.size() - 
                    // 注意：本轮取出的总数 = temp.size() + 已完成的
                    // 简化：本轮实际执行数已经加过了，idle 是 cycle 减去本轮取出数
                    0); // 这里逻辑需要修正，见下方正确版本
            }
        }
        return time;
    }
};
```

等等，上面的 idle 计算有点绕。让我写一个更清晰的版本：

```cpp
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        priority_queue<int> maxHeap;
        for (int f : freq) {
            if (f > 0) maxHeap.push(f);
        }
        
        int time = 0;
        while (!maxHeap.empty()) {
            int cycle = n + 1;
            int taskCount = 0;    // 本轮实际执行的任务数
            vector<int> temp;
            
            for (int i = 0; i < cycle && !maxHeap.empty(); i++) {
                int cur = maxHeap.top(); maxHeap.pop();
                if (cur - 1 > 0) temp.push_back(cur - 1);
                taskCount++;
            }
            
            for (int t : temp) maxHeap.push(t);
            
            // 如果堆非空，本轮必须填满 n+1（不足的用 idle）
            // 如果堆空了，本轮只算实际执行的任务数（最后一轮不补 idle）
            time += maxHeap.empty() ? taskCount : cycle;
        }
        return time;
    }
};
```

**关键点**：最后一轮不需要补 idle！只有"后面还有任务"时才需要补齐冷却时间。

---

### 解法2: 数学公式（贪心） — O(N) / O(1) ⭐ 面试首选

**从模拟到公式**：模拟虽然正确，但面试中能直接推导出数学公式更加优雅。关键观察：

```
以 tasks = ["A","A","A","B","B","B"], n = 2 为例:
maxFreq = 3 (A和B都出现3次), countOfMax = 2 (A和B)

把最高频任务 A 当骨架，按 n+1=3 为一组：

| A | _ | _ |    ← 第1段 (间隔段)
| A | _ | _ |    ← 第2段 (间隔段)
| A |            ← 最后一段

间隔段数 = maxFreq - 1 = 2
每个间隔段长度 = n + 1 = 3
最后一段的长度 = countOfMax = 2 (A和B都要放)

填入B：
| A | B | idle |
| A | B | idle |
| A | B |

总时间 = (maxFreq - 1) × (n + 1) + countOfMax
       = 2 × 3 + 2 = 8
```

**但如果任务种类很多呢？**
```
tasks = ["A","A","A","B","B","B","C","C","C","D","D","E"], n = 2
maxFreq = 3, countOfMax = 3 (A,B,C)

骨架：
| A | B | C |    ← 完全填满，不需要idle！
| A | B | C |
| A | B | C | + D, D, E 还需要额外位置

这种情况 公式算出 2×3+3 = 9，但实际任务有12个
答案 = max(9, 12) = 12
```

```cpp
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        int maxFreq = *max_element(freq.begin(), freq.end());
        
        // 统计有多少种任务的频率等于最高频率
        int countOfMax = 0;
        for (int f : freq) {
            if (f == maxFreq) countOfMax++;
        }
        
        // 骨架长度 = (maxFreq-1) 个完整间隔段 + 最后一段
        // 每个完整段长 n+1，最后一段长 countOfMax
        int formulaResult = (maxFreq - 1) * (n + 1) + countOfMax;
        
        // 答案不可能比任务总数少
        return max(formulaResult, (int)tasks.size());
    }
};
```

**为什么公式是对的？**
- `(maxFreq - 1)` 个间隔段：最高频任务的前 maxFreq-1 次执行后，每次都需要等待 n 个间隔
- 每个间隔段长 `n + 1`：1个高频任务 + n个空位（可以放其他任务或idle）
- 最后加 `countOfMax`：所有最高频任务都需要在最后一段出现一次
- 取 max 与 tasks.size()：当空位不够放所有其他任务时，说明不需要任何 idle，此时答案就是任务总数

---

### 解法3: 贪心+排序模拟 — O(N) / O(1)

**思路**：和堆模拟类似，但用排序代替堆。每轮将频率数组降序排序，取前 n+1 个执行。由于只有 26 种任务，排序 O(26log26) 是常数。

```cpp
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        // 降序排列
        sort(freq.begin(), freq.end(), greater<int>());
        
        int time = 0;
        while (freq[0] > 0) { // 最高频率 > 0 说明还有任务
            int taskCount = 0;
            
            // 每轮取前 n+1 个非零频率的任务
            for (int i = 0; i <= n && i < 26; i++) {
                if (freq[i] > 0) {
                    freq[i]--;
                    taskCount++;
                }
            }
            
            // 重新排序，为下一轮准备
            sort(freq.begin(), freq.end(), greater<int>());
            
            // 最后一轮不补 idle
            time += (freq[0] > 0) ? (n + 1) : taskCount;
        }
        return time;
    }
};
```

## 解法对比

| | 贪心+堆模拟 | 数学公式 | 贪心+排序模拟 |
|---|---|---|---|
| 时间 | O(N) | O(N) | O(N) |
| 空间 | O(1) | O(1) | O(1) |
| 代码量 | 中等 | 极短 | 中等 |
| 思维难度 | 中等（模拟思路直观） | 较高（需要推导公式） | 中等 |
| 面试推荐 | 展示模拟能力 | ⭐ 首选（简洁高效） | 备选 |

**什么时候选哪个？**
- **面试首选数学公式**：代码短，思路清晰，容易讲解
- **堆模拟用于追问**：如果面试官问"请输出具体的调度序列"，模拟法可以直接给出序列

## 易错点

1. **忘记取 max 与 tasks.size()**
   - ✗ `return (maxFreq - 1) * (n + 1) + countOfMax;`
   - ✓ `return max((maxFreq - 1) * (n + 1) + countOfMax, (int)tasks.size());`
   - 当任务种类远多于 n+1 时，完全不需要 idle，公式算出的值会小于任务数

2. **countOfMax 只数频率等于 maxFreq 的，不是所有非零频率**
   - ✗ 遍历时 `if (f > 0) countOfMax++;`
   - ✓ 遍历时 `if (f == maxFreq) countOfMax++;`

3. **堆模拟中最后一轮的处理**
   - ✗ 每轮都加 `n + 1`（最后一轮也补 idle）
   - ✓ 最后一轮只加实际执行的任务数，因为后面没有任务需要等冷却

4. **n = 0 的边界**
   - 此时没有冷却限制，答案就是 tasks.size()
   - 公式自动处理了这个情况：`(maxFreq-1)*1 + countOfMax = maxFreq-1+countOfMax`，而 `tasks.size() >= maxFreq`，取 max 后正确

## 面试追问

**Q1: 这道题的暴力解是什么？瓶颈在哪？**

暴力：模拟时间轴，每个时间点遍历所有任务类型，找一个"可执行且剩余次数最多"的任务。需要维护每种任务的上次执行时间来判断冷却。瓶颈：每个时间点都要扫描所有任务类型，且总时间可能远大于任务数（很多 idle）。用堆可以优化选择过程；用数学公式可以 O(1) 直接计算。

**Q2: 如果要输出具体的调度序列（而不是只要长度），怎么做？**

用堆模拟法。每轮从堆中取出实际的任务类型（需要把堆中存储的信息改为 `{频率, 任务名}`），记录每轮取出的任务名，不足的用 "idle" 补齐。注意最后一轮不补 idle。

**Q3: 如果有多个 CPU 核心可以并行执行呢？**

假设有 k 个核心，每个核心独立受冷却约束。这变成了一个更复杂的调度问题。一种思路是把 `n+1` 个时间槽的框架变成 `k*(n+1)` 的框架，但需要更细致的分析。这已经超出 LeetCode 范围，是操作系统调度的经典问题。

## 相关题型

- **358. K 距离间隔重排字符串** — 几乎同一个问题，但需要输出具体的排列结果而不只是长度。复用堆模拟方法，但要记录实际字符。
- **767. 重组字符串** — n=1 的特殊情况：相同字符不能相邻。复用"最高频率决定可行性"的判断：maxFreq > (len+1)/2 时无解。
- **1953. 你可以工作的最大周数** — 类似思路，判断最高频任务是否会导致 idle。
- **984. 不含 AAA 或 BBB 的字符串** — 贪心构造，优先安排剩余多的字符，和堆模拟方法的思路一致。