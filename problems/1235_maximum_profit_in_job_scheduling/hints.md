# 1235. 规划兼职工作

## 核心思路

这道题本质上是一个**带权重的区间调度问题**：从一堆可能重叠的工作中选出互不冲突的子集，使总报酬最大。这是经典的"选或不选"DP 模型，关键在于按结束时间排序后，用二分查找快速找到"不冲突的上一份工作"。

## 思维链

1. **读完题第一反应**：每份工作有时间区间和报酬，要选互不重叠的工作使报酬最大。暴力做法是枚举所有子集 2^n，检查每个子集是否冲突，取最大报酬。→ 指数级，完全不可行。

2. **联想经典模型**：这和"活动选择问题"很像，但活动选择只是选最多活动（贪心），这里每个工作有不同的报酬（权重），贪心不再适用 → 需要 DP。

3. **排序是关键的第一步**：为了让 DP 有序推进，按**结束时间**排序。这样处理第 i 个工作时，所有结束时间 ≤ endTime[i] 的工作都已经被考虑过。

4. **DP 状态定义**：`dp[i]` = 考虑前 i 个工作（按结束时间排序后），能获得的最大报酬。

5. **状态转移的关键**：对于第 i 个工作，有两种选择：
   - **不做第 i 个工作**：`dp[i] = dp[i-1]`
   - **做第 i 个工作**：需要找到最后一个结束时间 ≤ startTime[i] 的工作 j，`dp[i] = dp[j] + profit[i]`

6. **瓶颈与优化**：找"最后一个结束时间 ≤ startTime[i] 的工作"如果线性扫描是 O(n)，总体 O(n²)。但结束时间已排序，可以用**二分查找**，把查找降到 O(log n)，总体 O(n log n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举每个工作选/不选 | O(2^n) | O(n) | 能说出即可 |
| 记忆化搜索 | 排序 + 递归 + memo + 二分 | O(n log n) | O(n) | 加分项 |
| DP + 二分 | 按结束时间排序，dp[i] 选或不选 | O(n log n) | O(n) | ⭐ 必须写出 |
| DP (endTime 为 key) | 用 map 存离散化的结束时间点 | O(n log n) | O(n) | 加分项 |

## 关键提示

1. **为什么按结束时间排序而不是开始时间？** — 按结束时间排序后，dp[i] 的定义才有意义：前 i 个工作的最优解包含了所有"早结束"的工作，保证状态转移时依赖的子问题已经求解。

2. **二分查找的目标是什么？** — 对于第 i 个工作，在已排序的 endTime 数组中找最大的 j 使得 `endTime[j] <= startTime[i]`。这是 `upper_bound` 的变体。

3. **dp[i] 只有两种来源**：不选第 i 个（继承 dp[i-1]），或者选第 i 个（找到不冲突的前驱 + profit[i]）。取 max 即可。

4. **注意"结束时间 = 开始时间"算不冲突** — 题目说"在时间 X 结束可以立刻做时间 X 开始的工作"，所以二分时是 `endTime[j] <= startTime[i]`，不是 `<`。

5. **排序时三元组一起排** — 不要只排 endTime 忘了同步移动 startTime 和 profit。

```
示例1 排序后的工作（按endTime）:
工作索引:  0      1      2      3
时间区间: [1,3]  [2,4]  [3,5]  [3,6]
报酬:      50     10     40     70

dp 填表过程:
dp[0] = 50                          (只有工作0，必选)
dp[1] = max(dp[0], 10+0) = 50      (选工作1只得10，不如不选)
dp[2] = max(dp[1], 40+dp[0]) = 90  (工作2的start=3, 二分找endTime≤3 → 工作0, dp[0]+40=90)
        但实际 dp[2] = max(50, 40+50) = 90
dp[3] = max(dp[2], 70+dp[0]) = 120 (工作3的start=3, 二分找endTime≤3 → 工作0, dp[0]+70=120)

答案: dp[3] = 120
```

## 解法详解

### 解法1: 暴力递归（自顶向下，无记忆化）— O(2^n) / O(n)

**思考过程**: 最直觉的做法——按结束时间排序后，对每个工作"选或不选"递归下去。如果选了，跳到下一个不冲突的工作；如果不选，看下一个工作。这是所有 DP 的起点。

```cpp
class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        // 打包排序
        vector<array<int,3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());
        
        // 递归：从第 idx 个工作开始，能获得的最大报酬
        function<int(int)> dfs = [&](int idx) -> int {
            if (idx >= n) return 0;
            
            // 选择1: 不做第 idx 个工作
            int skip = dfs(idx + 1);
            
            // 选择2: 做第 idx 个工作，找下一个不冲突的工作
            int nextJob = idx + 1;
            while (nextJob < n && jobs[nextJob][1] < jobs[idx][0])
                nextJob++;
            int take = jobs[idx][2] + dfs(nextJob);
            
            return max(skip, take);
        };
        
        return dfs(0);
    }
};
```

**关键点**: 这个解法会超时（指数级），但它清晰展示了"选或不选"的决策树结构，是理解后续优化的基础。

```
决策树（示例1，简化展示）:
                    dfs(0)
                  /        \
          不选工作0         选工作0(+50)
           dfs(1)            dfs(1)
          /     \           /     \
      不选1     选1(+10)  不选1   选1(+10)
      dfs(2)    dfs(2)    dfs(2)  dfs(?)
       ...       ...       ...     ...
```

### 解法2: 记忆化搜索 + 二分 — O(n log n) / O(n)

**从解法1优化**: 解法1有大量重复子问题（同一个 idx 被反复计算），加 memo 缓存。同时把线性查找"下一个不冲突工作"改为二分查找。

```cpp
class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int,3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());
        
        // 提取排序后的 endTime，供二分查找使用
        vector<int> ends(n);
        for (int i = 0; i < n; i++) ends[i] = jobs[i][0];
        
        vector<int> memo(n, -1);
        
        function<int(int)> dfs = [&](int idx) -> int {
            if (idx >= n) return 0;
            if (memo[idx] != -1) return memo[idx];
            
            // 不选
            int skip = dfs(idx + 1);
            
            // 选: 二分找第一个 startTime >= endTime[idx] 的工作
            // 即找 ends 中第一个 > jobs[idx][1] 的前一个... 不对
            // 我们要找第一个 endTime 数组中，startTime[next] >= endTime[idx]
            // 换个思路：找排序后的 jobs 中，startTime >= jobs[idx][0] 的最小 index
            int lo = idx + 1, hi = n;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (jobs[mid][1] >= jobs[idx][0]) // startTime[mid] >= endTime[idx]
                    hi = mid;
                else
                    lo = mid + 1;
            }
            int take = jobs[idx][2] + dfs(lo);
            
            return memo[idx] = max(skip, take);
        };
        
        return dfs(0);
    }
};
```

**关键点**: 二分查找的目标是在 idx+1..n-1 中找第一个 `startTime[mid] >= endTime[idx]` 的位置。注意这里二分的是 startTime，但数组是按 endTime 排序的，startTime 不一定有序，所以这个二分其实**不完全正确**。更稳妥的做法是用解法3的方式。

### 解法3: DP + 二分（按结束时间排序）— O(n log n) / O(n) ⭐ 面试首选

**从记忆化翻转成递推**: 自底向上更直观。`dp[i]` 表示考虑前 i 个工作（排序后），能获得的最大报酬。

核心转移：
- 不选第 i 个：`dp[i] = dp[i-1]`
- 选第 i 个：在 endTime 数组中二分找最大的 j 使得 `endTime[j] <= startTime[i]`，`dp[i] = dp[j+1] + profit[i]`（注意偏移）

```cpp
class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int,3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());
        
        // dp[i] = 考虑前 i 个工作（1-indexed），能获得的最大报酬
        // dp[0] = 0（不选任何工作）
        vector<int> dp(n + 1, 0);
        
        // 提取排序后的 endTime，用于二分
        vector<int> ends(n);
        for (int i = 0; i < n; i++) ends[i] = jobs[i][0];
        
        for (int i = 1; i <= n; i++) {
            // 当前工作（0-indexed: i-1）
            int start_i = jobs[i - 1][1];
            int profit_i = jobs[i - 1][2];
            
            // 不选第 i 个工作
            dp[i] = dp[i - 1];
            
            // 选第 i 个工作：二分找最后一个 endTime <= start_i 的工作
            // 在 ends[0..i-2] 中找 upper_bound(start_i) 的前一个位置
            // upper_bound 返回第一个 > start_i 的位置，减1就是最后一个 <= start_i 的
            int pos = upper_bound(ends.begin(), ends.begin() + (i - 1), start_i) - ends.begin();
            // pos 个工作（0-indexed: 0..pos-1）不冲突 → dp[pos] 是它们的最优解
            dp[i] = max(dp[i], dp[pos] + profit_i);
        }
        
        return dp[n];
    }
};
```

```
DP 填表过程（示例1）:
排序后: jobs = [{3,1,50}, {4,2,10}, {5,3,40}, {6,3,70}]
              end  start profit
ends = [3, 4, 5, 6]

dp[0] = 0  (不选任何工作)

i=1, 工作[1,3,50], start=1:
  不选: dp[1] = dp[0] = 0
  选:   二分 ends[0..0) 中 ≤ 1 的 → pos=0 → dp[0]+50 = 50
  dp[1] = max(0, 50) = 50

i=2, 工作[2,4,10], start=2:
  不选: dp[2] = dp[1] = 50
  选:   二分 ends[0..1) 中 ≤ 2 的 → ends=[3], 无 ≤ 2 的 → pos=0 → dp[0]+10 = 10
  dp[2] = max(50, 10) = 50

i=3, 工作[3,5,40], start=3:
  不选: dp[3] = dp[2] = 50
  选:   二分 ends[0..2) = [3,4] 中 ≤ 3 的 → upper_bound(3)=1 → pos=1 → dp[1]+40 = 90
  dp[3] = max(50, 90) = 90

i=4, 工作[3,6,70], start=3:
  不选: dp[4] = dp[3] = 90
  选:   二分 ends[0..3) = [3,4,5] 中 ≤ 3 的 → upper_bound(3)=1 → pos=1 → dp[1]+70 = 120
  dp[4] = max(90, 120) = 120

答案: dp[4] = 120 ✓
```

**关键点**:
- 为什么用 `upper_bound` 而不是 `lower_bound`？因为我们要找 endTime **≤** startTime[i]（不是 <），`upper_bound` 返回第一个 > 的位置，前一个就是最后一个 ≤ 的。
- 二分搜索范围是 `ends[0..i-2]`（不包含当前工作自己），对应 `ends.begin() + (i-1)`。

### 解法4: DP（用 TreeMap / 有序映射）— O(n log n) / O(n)

**不同视角**: 按结束时间排序后，维护一个映射 `time → maxProfit`，表示在时间 time 之前（含）能获得的最大报酬。对每个工作，在 map 中查找 ≤ startTime[i] 的最大值。

```cpp
class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int,3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());
        
        // map: endTime → 截至该时间点的最大报酬
        // 初始化: 时间0时报酬为0
        map<int, int> dp;
        dp[0] = 0;
        
        for (auto& [end, start, pro] : jobs) {
            // 找 map 中 key <= start 的最大 value
            // upper_bound(start) 指向第一个 > start 的，往前一步就是 <= start 的
            auto it = prev(dp.upper_bound(start));
            int candidate = it->second + pro;
            
            // 当前最大值 = map 中最后一个 entry 的 value（即目前全局最优）
            int curMax = dp.rbegin()->second;
            
            // 只有当 candidate > curMax 时才更新（保证 map 的 value 单调递增）
            if (candidate > curMax) {
                dp[end] = candidate;
            }
        }
        
        return dp.rbegin()->second;
    }
};
```

**关键点**: 这个解法的精妙之处在于 map 中的 value 天然单调递增（因为时间越晚，可选工作越多，最优解只增不减）。如果新的 candidate 不超过当前最大值，就不需要插入。

## 解法对比

| | 解法1 暴力递归 | 解法2 记忆化 | 解法3 DP+二分 ⭐ | 解法4 TreeMap |
|---|---|---|---|---|
| 时间 | O(2^n) | O(n log n) | O(n log n) | O(n log n) |
| 空间 | O(n) | O(n) | O(n) | O(n) |
| 优点 | 易理解 | 自然过渡 | 最直观最标准 | 代码简洁 |
| 缺点 | 超时 | 二分对象需注意 | 需理解偏移 | map 常数较大 |
| 面试 | 说思路即可 | 可以先写这个 | **首选** | 会追问原理 |

**面试推荐路线**: 先说解法1的思路（展示你理解问题），然后直接写解法3（DP+二分），这是面试官最期待的标准答案。

## 易错点

1. **✗ 排序只排 endTime，没同步 startTime 和 profit**
   ```cpp
   sort(endTime.begin(), endTime.end()); // 错！startTime 和 profit 没跟着动
   ```
   **✓ 打包成三元组一起排序**
   ```cpp
   vector<array<int,3>> jobs(n);
   for (int i = 0; i < n; i++) jobs[i] = {endTime[i], startTime[i], profit[i]};
   sort(jobs.begin(), jobs.end());
   ```

2. **✗ 二分找 < startTime 而不是 ≤ startTime**
   ```cpp
   // 错：用 lower_bound 找 startTime[i]，会排除掉 endTime == startTime 的工作
   int pos = lower_bound(ends.begin(), ends.begin() + (i-1), start_i) - ends.begin();
   ```
   **✓ 用 upper_bound 找 > startTime 的第一个位置**
   ```cpp
   // 正确：upper_bound 返回第一个 > start_i 的位置，其左边全是 <= start_i 的
   int pos = upper_bound(ends.begin(), ends.begin() + (i-1), start_i) - ends.begin();
   ```
   题目说"在时间 X 结束可以立刻做时间 X 开始的工作"，所以 endTime == startTime 是合法的。

3. **✗ dp 数组 0-indexed 和 1-indexed 搞混**
   ```cpp
   dp[i] = max(dp[i-1], dp[pos-1] + profit_i); // pos-1? pos? 容易搞混
   ```
   **✓ 定义清楚**: dp 用 1-indexed，`dp[i]` 表示前 i 个工作。二分得到的 pos 是 0-indexed 的 endTime 数组位置，恰好对应 `dp[pos]`（前 pos 个工作的最优解）。

4. **✗ 二分的搜索范围包含了当前工作自身**
   ```cpp
   int pos = upper_bound(ends.begin(), ends.begin() + i, start_i) - ends.begin();
   // 错！ends[i-1] 是当前工作自己的 endTime
   ```
   **✓ 搜索范围到 i-1**
   ```cpp
   int pos = upper_bound(ends.begin(), ends.begin() + (i - 1), start_i) - ends.begin();
   ```

## 面试追问

**Q1: 暴力能过吗？瓶颈在哪？**
→ 暴力枚举所有子集 O(2^n)，n 最大 5×10^4，完全不行。即使排序后递归"选/不选"，不加记忆化也是指数级。瓶颈是重叠子问题——同一个起始位置被多条路径反复计算。

**Q2: 为什么按结束时间排序？按开始时间行不行？**
→ 按结束时间排序使得 dp[i]="前 i 个工作的最优" 有明确含义。按开始时间排序也可以做（从后往前 DP），但按结束时间排序更自然：考虑到第 i 个工作时，所有更早结束的工作都已处理，二分查找"不冲突的前驱"在有序的 endTime 数组上高效完成。

**Q3: 如果要求输出具体选了哪些工作（不只是最大报酬），怎么改？**
→ 在 DP 过程中记录每个状态的"选择来源"：`dp[i]` 来自 `dp[i-1]`（不选）还是 `dp[pos] + profit[i]`（选）。最后从 dp[n] 回溯，如果 dp[i] != dp[i-1] 说明选了第 i 个工作，跳到 pos 继续回溯。

**Q4: 这道题和 0/1 背包有什么关系？**
→ 非常类似！每个工作"选或不选"，约束不是重量而是时间区间不冲突。区别在于背包的约束是一维的（总重量），而这里的约束是区间不重叠。排序 + 二分的技巧正是处理这种"区间约束"的标准手法。

## 相关题型

- **435. 无重叠区间** — 同样是区间调度，但没有权重（贪心即可）。本题多了权重后贪心失效，必须用 DP。
- **646. 最长数对链** — 类似的区间 DP，按结束时间排序后的选或不选。
- **1751. 最多可以参加的会议数目 II** — 本题的直接变体，限制最多参加 k 个会议，需要增加一维状态 dp[i][j]。复用本题的排序+二分框架，dp 状态多一维。
- **300. 最长递增子序列** — 同样的"选或不选"DP 框架 + 二分优化，但约束从区间不重叠变成值递增。
- **背包系列 (0/1 Knapsack)** — 框架一致：选或不选，区别在约束条件。本题的"时间不冲突"对应背包的"重量不超"。