# 826. 最大利润分配工作 (Most Profit Assigning Work)

## 核心思路

本题本质上是：**将工作按难度排序后，为每个工人找到他能胜任的所有工作中利润最高的那个**。关键在于预处理"难度 ≤ d 时的最大利润"，然后对每个工人快速查询。

## 思维链

1. **读完题的第一反应**：每个工人能力有限，只能做难度 ≤ 自身能力的工作。多个工人可以做同一份工作。要让总利润最大化。

2. **暴力想法**：对每个工人，遍历所有工作找最大利润。O(n*m) 太慢。

3. **关键观察 — 利润不一定随难度递增**：难度高的工作利润不一定高。所以不能简单二分找"难度刚好 ≤ worker[j]"的工作。我们需要的是"难度 ≤ worker[j] 的所有工作中的最大利润"。

4. **预处理前缀最大值**：把工作按难度排序后，维护一个"前缀最大利润"数组：`maxProfit[i] = max(profit[0..i])`。这样对于任何难度上界，二分找到位置后直接查表。

5. **双指针优化**：如果把工人也排序，就可以用双指针同步扫描——工人能力递增，工作难度递增，指针只需要单调前进。

6. **最终方案**：排序工作 + 排序工人 + 双指针，O(n log n + m log m)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 + 双指针 | 工作按难度排序，工人按能力排序，双指针匹配 | O(n log n + m log m) | O(n) | ⭐ 必须写出 |
| 排序 + 二分查找 | 工作按难度排序 + 前缀最大利润，对每个工人二分 | O((n+m) log n) | O(n) | 能说出即可 |
| 桶 / 数组映射 | 用数组 maxProfit[d] 记录难度 d 的最大利润，前缀 max | O(n + m + D) | O(D) | D 小时可用 |

## 关键提示

1. **利润不随难度单调**：`difficulty = [2, 4, 6]`, `profit = [10, 50, 20]`，难度 6 的利润反而最低。所以必须对"难度 ≤ d"取前缀最大值。

2. **同一工作可被多人做**：不是分配问题（不需要匈牙利算法），每个工人独立选最优工作。

3. **双指针的前提**：工人排序后，需要记住原始索引来累加结果；或者先把 maxProfit 算好，直接累加即可。

4. **ASCII 示意图 — 示例1**：
```
工作排序后: difficulty = [2, 4, 6, 8, 10]
            profit     = [10, 20, 30, 40, 50]
前缀最大利润:            [10, 20, 30, 40, 50]  (本例恰好递增)

工人排序后: worker = [4, 5, 6, 7]

双指针扫描:
  worker=4: 移动j直到 difficulty[j]>4 → j停在2, maxP=20 → 累加20
  worker=5: j不动(difficulty[2]=6>5), maxP=20 → 累加20
  worker=6: 移动j到3(difficulty[3]=8>6), maxP=30 → 累加30
  worker=7: j不动(difficulty[3]=8>7), maxP=30 → 累加30
  总利润 = 20+20+30+30 = 100
```

5. **边界**：如果工人能力比所有工作难度都低，利润为 0。

## 解法详解

### 解法1: 排序 + 双指针 — O(n log n + m log m) / O(n) ⭐ 面试首选

**思考过程**：

将 (difficulty, profit) 配对按难度排序。将 worker 也排序。用双指针：工人能力递增扫描时，工作指针只前进不后退，同时维护已扫过工作中的最大利润。

```cpp
class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int n = difficulty.size();
        vector<pair<int,int>> jobs(n);
        for (int i = 0; i < n; i++) {
            jobs[i] = {difficulty[i], profit[i]};
        }
        sort(jobs.begin(), jobs.end()); // 按难度升序
        sort(worker.begin(), worker.end()); // 按能力升序
        
        int totalProfit = 0;
        int j = 0; // 工作指针
        int maxP = 0; // 当前难度范围内的最大利润
        
        for (int w : worker) {
            // 把所有难度 <= w 的工作都考虑进来
            while (j < n && jobs[j].first <= w) {
                maxP = max(maxP, jobs[j].second);
                j++;
            }
            totalProfit += maxP;
        }
        
        return totalProfit;
    }
};
```

**关键点**：
- 双指针的正确性：worker 递增，所以 j 只需前进，不会错过任何工作
- `maxP` 是全局累计最大值，不是当前工作的利润
- 工人能力相同时，`maxP` 不变，直接累加

### 解法2: 排序 + 二分查找 — O((n+m) log n) / O(n)

**思考过程**：

不排序 worker，而是对每个 worker 在排好序的 jobs 中二分找到"难度 ≤ worker[j]"的最后一个位置，然后查前缀最大利润表。

```cpp
class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int n = difficulty.size();
        vector<pair<int,int>> jobs(n);
        for (int i = 0; i < n; i++) {
            jobs[i] = {difficulty[i], profit[i]};
        }
        sort(jobs.begin(), jobs.end());
        
        // 构建前缀最大利润
        vector<int> maxP(n);
        maxP[0] = jobs[0].second;
        for (int i = 1; i < n; i++) {
            maxP[i] = max(maxP[i-1], jobs[i].second);
        }
        
        int totalProfit = 0;
        for (int w : worker) {
            // 二分找最后一个 difficulty <= w 的位置
            int lo = 0, hi = n - 1, pos = -1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (jobs[mid].first <= w) {
                    pos = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            if (pos >= 0) totalProfit += maxP[pos];
        }
        return totalProfit;
    }
};
```

### 解法3: 桶映射 — O(n + m + D) / O(D)

当难度值域 D 不大时（本题 D ≤ 10^5），可以用数组 `bucket[d]` 记录难度恰好为 d 的最大利润，然后做前缀 max。

```cpp
class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int maxD = *max_element(difficulty.begin(), difficulty.end());
        maxD = max(maxD, *max_element(worker.begin(), worker.end()));
        vector<int> bucket(maxD + 1, 0);
        for (int i = 0; i < (int)difficulty.size(); i++) {
            bucket[difficulty[i]] = max(bucket[difficulty[i]], profit[i]);
        }
        for (int i = 1; i <= maxD; i++) {
            bucket[i] = max(bucket[i], bucket[i-1]);
        }
        int total = 0;
        for (int w : worker) total += bucket[w];
        return total;
    }
};
```

## 易错点

1. **利润不随难度递增**：不能假设 `difficulty[i] < difficulty[j]` 就有 `profit[i] < profit[j]`。必须用前缀最大值或双指针维护 maxP。

2. **双指针中 maxP 不重置**：maxP 是累计最大值，不能在每个工人处重置为 0。因为能力更强的工人一定也能做之前的工作。

3. **忘记处理"工人能力低于所有工作难度"**：此时 j 不动，maxP 仍然是 0，累加 0 即可。但如果用二分查找，`pos = -1` 时不能访问 `maxP[-1]`。

4. **排序 worker 后忘了累加问题**：双指针解法不需要知道每个工人对应的结果，只需要总和，所以排序 worker 无碍。

## 面试追问

**Q1: 双指针和二分查找各自的适用场景？**
→ 双指针要求工人也排序，适合离线处理。二分不需要排序 worker，适合在线查询（每次来一个工人直接查）。如果工人是流式到达的，二分更好。

**Q2: 如果每个工作只能被分配给一个工人呢？**
→ 那就变成了经典的**任务分配/二分图匹配**问题。贪心策略：按工人能力从大到小排序，每次分配他能做的工作中利润最高且未被分配的。用 multiset 或优先队列维护可用工作。

**Q3: 如果要求恰好分配 k 个工人呢？**
→ 先对所有工人求出各自的最大利润，然后取 top-k 求和。

**Q4: 时间复杂度能否优于 O(n log n)？**
→ 如果值域有限（本题 ≤ 10^5），可以用桶排序 + 前缀最大值做到 O(n + m + D)，避免比较排序的 O(n log n)。

## 相关题型

- **875. 爱吃香蕉的珂珂 (Koko Eating Bananas)** — 同样是排序 + 二分思想。区别：875 是在答案空间上二分，本题是在数据空间上二分/双指针。

- **455. 分发饼干 (Assign Cookies)** — 贪心 + 排序 + 双指针的经典题。区别：455 每个饼干只能给一个孩子（一对一），本题一个工作可被多人做。

- **1029. 两地调度 (Two City Scheduling)** — 贪心分配问题。区别：1029 要平均分配到两个城市，本题是单向最优选择。
