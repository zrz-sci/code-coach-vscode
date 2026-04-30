# 256. 粉刷房子

## 核心思路

本质上是一个**带约束的多阶段决策最优化问题**：每一阶段（每栋房子）有3种选择（颜色），约束是相邻阶段选择不能相同，求总成本最小。这是经典的线性 DP 入门题。

## 思维链

1. **读完题第一反应**：每栋房子有3种颜色可选，相邻不能同色——这是一个排列选择问题。最朴素的做法：枚举所有合法的涂色方案，计算每种方案的总成本，取最小值。

2. **暴力解的瓶颈**：n 栋房子，每栋 3 种颜色，总方案数最多 3^n（虽然有约束会减少，但仍是指数级）。n=100 时完全不可行。

3. **发现重叠子问题**：当我决定第 i 栋房子涂红色时，我只关心"前 i-1 栋房子在第 i-1 栋不是红色的前提下的最小成本"。第 i-1 栋涂蓝还是绿，取决于它们各自的最优子结构——这就是 DP！

4. **定义状态**：`dp[i][j]` = 把第 0~i 栋房子都粉刷完，且第 i 栋涂颜色 j 的最小总成本。

5. **状态转移**：第 i 栋涂颜色 j，则第 i-1 栋只能涂另外两种颜色中更便宜的那个。`dp[i][j] = costs[i][j] + min(dp[i-1][k])` 其中 k ≠ j。

6. **空间优化**：`dp[i]` 只依赖 `dp[i-1]`，可以用滚动变量把 O(n) 空间降到 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举所有合法涂色方案 | O(2^n) | O(n) 递归栈 | 能说出即可 |
| 记忆化搜索 | 自顶向下 + memo | O(n) | O(n) | 能说出即可 |
| 二维 DP | 自底向上递推填表 | O(n) | O(n) | ⭐ 必须写出 |
| 滚动变量 DP | 空间优化到 O(1) | O(n) | O(1) | 加分项 |

> 注：这里说 O(n) 是因为颜色数固定为 3，每步的操作是常数。严格说是 O(n·k) 其中 k=3。

## 关键提示

1. **状态定义是关键**：想想"第 i 栋房子涂了颜色 j 之后的最小成本"需要记录哪些信息？只需要知道第 i 栋的颜色，不需要知道之前每栋的颜色——因为约束只涉及相邻两栋。

2. **转移方程的直觉**：涂红色的成本 = 红色油漆费 + min(前一栋涂蓝的最优, 前一栋涂绿的最优)。三种颜色对称地各写一个。

3. **空间优化的条件**：`dp[i]` 只用到 `dp[i-1]`，所以不需要整张表，只需要"上一行"的3个值。

4. **DP 填表过程示意**（以示例1为例）：
```
costs:      [17, 2,17]  [16,16, 5]  [14, 3,19]
             R   B  G     R   B  G     R   B  G

dp[0]:      [17, 2,17]                          ← base case
dp[1]:      [2+16, 17+16, 2+5]  = [18, 33, 7]  ← 选最小的非同色
dp[2]:      [7+14, 7+3, 18+19]  = [21, 10, 37]

答案: min(21, 10, 37) = 10 ✓
```

5. **系列题关联**：这是 Paint House 系列的第一题。265. Paint House II 将颜色从 3 种扩展到 k 种，核心思路相同，但需要优化"找另外 k-1 种颜色中最小值"的过程。

## 解法详解

### 解法1: 暴力递归 — O(2^n) / O(n)

**思考过程**：最直觉的方式——对每栋房子尝试所有合法颜色，递归到下一栋。本质是 DFS 遍历决策树。

```
// 决策树（前2层）:
//                    start
//              /       |       \
//          h0=R       h0=B      h0=G
//          c=17       c=2       c=17
//         /   \      /   \     /   \
//      h1=B  h1=G  h1=R h1=G h1=R h1=B
//      c=16  c=5   c=16 c=5  c=16 c=16
//       ...   ...   ...  ...  ...  ...
```

```cpp
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        int ans = INT_MAX;
        // 第0栋房子尝试3种颜色
        for (int c = 0; c < 3; c++) {
            ans = min(ans, dfs(costs, 0, c));
        }
        return ans;
    }
    
    // 返回：从第i栋开始（第i栋涂颜色c）到最后一栋的最小成本
    int dfs(vector<vector<int>>& costs, int i, int c) {
        int n = costs.size();
        if (i == n - 1) return costs[i][c]; // 最后一栋，直接返回成本
        
        int best = INT_MAX;
        for (int next = 0; next < 3; next++) {
            if (next != c) { // 相邻不同色
                best = min(best, dfs(costs, i + 1, next));
            }
        }
        return costs[i][c] + best;
    }
};
```

**关键点**：这个解法直观但效率极差。n=20 就开始变慢了。瓶颈在于大量重复计算——比如 `dfs(2, R)` 会被从不同路径调用多次。

---

### 解法2: 记忆化搜索 — O(n) / O(n)

**从解法1优化**：解法1中 `dfs(i, c)` 的参数空间只有 n×3 种组合，但被重复调用了指数次。加一个 memo 数组缓存结果，每个 (i, c) 只算一次。

```cpp
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        // memo[i][c] = -1 表示尚未计算
        vector<vector<int>> memo(n, vector<int>(3, -1));
        int ans = INT_MAX;
        for (int c = 0; c < 3; c++) {
            ans = min(ans, dfs(costs, memo, 0, c));
        }
        return ans;
    }
    
    int dfs(vector<vector<int>>& costs, vector<vector<int>>& memo, int i, int c) {
        if (i == costs.size() - 1) return costs[i][c];
        if (memo[i][c] != -1) return memo[i][c]; // 命中缓存
        
        int best = INT_MAX;
        for (int next = 0; next < 3; next++) {
            if (next != c) {
                best = min(best, dfs(costs, memo, i + 1, next));
            }
        }
        return memo[i][c] = costs[i][c] + best;
    }
};
```

**关键点**：记忆化搜索是从"暴力递归"到"DP"的桥梁。它和递推 DP 的计算结果完全一致，只是方向不同（自顶向下 vs 自底向上）。

---

### 解法3: 二维 DP（递推）— O(n) / O(n) ⭐ 面试首选

**从记忆化搜索翻译**：把递归改成 for 循环，从前往后填表。

```
// dp 填表方向和依赖关系:
// dp[i][0] ← min(dp[i-1][1], dp[i-1][2]) + costs[i][0]
// dp[i][1] ← min(dp[i-1][0], dp[i-1][2]) + costs[i][1]
// dp[i][2] ← min(dp[i-1][0], dp[i-1][1]) + costs[i][2]
//
// 每一行只依赖上一行 ↑
//
// 示例 costs = [[17,2,17],[16,16,5],[14,3,19]]:
//
// i=0: dp = [17,  2, 17]   ← 直接取 costs[0]
// i=1: dp = [ 2+16, 17+16, 2+5] = [18, 33, 7]
//            ↑min(2,17)  ↑min(17,17)  ↑min(17,2)
// i=2: dp = [ 7+14, 7+3, 18+19] = [21, 10, 37]
//            ↑min(33,7) ↑min(18,7)  ↑min(18,33)
//
// 答案 = min(21, 10, 37) = 10
```

```cpp
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        // dp[i][j] = 粉刷第0~i栋，第i栋涂颜色j的最小总成本
        vector<vector<int>> dp(n, vector<int>(3, 0));
        
        // base case: 第0栋房子，直接就是自身的成本
        dp[0][0] = costs[0][0];
        dp[0][1] = costs[0][1];
        dp[0][2] = costs[0][2];
        
        for (int i = 1; i < n; i++) {
            // 第i栋涂红：前一栋只能是蓝或绿
            dp[i][0] = costs[i][0] + min(dp[i-1][1], dp[i-1][2]);
            // 第i栋涂蓝：前一栋只能是红或绿
            dp[i][1] = costs[i][1] + min(dp[i-1][0], dp[i-1][2]);
            // 第i栋涂绿：前一栋只能是红或蓝
            dp[i][2] = costs[i][2] + min(dp[i-1][0], dp[i-1][1]);
        }
        
        // 最后一栋涂哪种颜色成本最低
        return min({dp[n-1][0], dp[n-1][1], dp[n-1][2]});
    }
};
```

**关键点**：状态转移方程的直觉——"我选了颜色 j，那前一栋就不能是 j，从其他颜色里挑最便宜的"。

---

### 解法4: 滚动变量 DP（空间优化）— O(n) / O(1)

**从解法3优化**：`dp[i]` 只依赖 `dp[i-1]`，所以不需要整个二维数组，只需要3个变量记录上一行的值。

```cpp
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        // 上一栋房子涂红/蓝/绿的最优成本
        int prevR = costs[0][0];
        int prevB = costs[0][1];
        int prevG = costs[0][2];
        
        for (int i = 1; i < n; i++) {
            // 必须用临时变量，因为 prevR/prevB/prevG 会互相依赖
            int curR = costs[i][0] + min(prevB, prevG);
            int curB = costs[i][1] + min(prevR, prevG);
            int curG = costs[i][2] + min(prevR, prevB);
            prevR = curR;
            prevB = curB;
            prevG = curG;
        }
        
        return min({prevR, prevB, prevG});
    }
};
```

**关键点**：更新时必须用临时变量！如果直接写 `prevR = costs[i][0] + min(prevB, prevG)` 然后接着用 `prevR` 算 `prevB`，此时 `prevR` 已经被覆盖了，结果就错了。

---

### 解法4b: 原地修改 costs 数组 — O(n) / O(1)

如果允许修改输入，可以直接在 costs 上累加，连临时变量都省了：

```cpp
class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int n = costs.size();
        for (int i = 1; i < n; i++) {
            costs[i][0] += min(costs[i-1][1], costs[i-1][2]);
            costs[i][1] += min(costs[i-1][0], costs[i-1][2]);
            costs[i][2] += min(costs[i-1][0], costs[i-1][1]);
        }
        return min({costs[n-1][0], costs[n-1][1], costs[n-1][2]});
    }
};
```

> 注意：面试中修改输入前应该问面试官是否允许。

## 解法对比

| | 暴力递归 | 记忆化搜索 | 二维DP | 滚动变量DP |
|---|---|---|---|---|
| 时间 | O(2^n) | O(n) | O(n) | O(n) |
| 空间 | O(n) 栈 | O(n) | O(n) | O(1) |
| 思维难度 | 最简单 | 简单 | 中等 | 中等 |
| 面试推荐 | 说思路即可 | 可以作为第一步 | ⭐ 首选 | 追问时给出 |

**核心区别**：
- 暴力→记忆化：加缓存，消除重复计算
- 记忆化→递推：递归改循环，避免栈溢出风险
- 递推→滚动变量：利用"只依赖上一行"的特性压缩空间

## 易错点

1. **滚动变量更新顺序错误**
   - ✗ `prevR = costs[i][0] + min(prevB, prevG); prevB = costs[i][1] + min(prevR, prevG);` ← 第二行用了已被覆盖的 prevR
   - ✓ 先算出 curR, curB, curG 三个临时变量，再一起赋值回去

2. **dp 初始化遗漏**
   - ✗ `dp[0][j] = 0` — 忘了第0栋房子也要花钱
   - ✓ `dp[0][j] = costs[0][j]` — base case 是第0栋的实际涂色成本

3. **最终答案取法错误**
   - ✗ `return dp[n-1][0]` — 只看了红色
   - ✓ `return min({dp[n-1][0], dp[n-1][1], dp[n-1][2]})` — 最后一栋涂哪种颜色不确定，要取三者最小

4. **n=1 的边界情况**
   - 只有一栋房子时，直接返回 `min(costs[0][0], costs[0][1], costs[0][2])`
   - 上面的 DP 代码天然处理了这种情况（循环不执行，直接返回 dp[0] 的最小值）

## 面试追问

**Q1（基础理解）：为什么贪心不行？每一步选当前最便宜的颜色不行吗？**
> 不行。例如 `[[1,100,100],[100,1,100],[100,100,1]]`，贪心每步选最小：红(1)→蓝(1)→绿(1)=3。但如果第一步选了成本稍高的颜色，可能后面省更多。贪心只看局部，DP 看全局。（不过这个例子恰好贪心也对，更好的反例是 `[[1,2,100],[100,1,2],[1,100,100]]`）

**Q2（空间优化）：能否只用 O(1) 额外空间？**
> 可以。`dp[i]` 只依赖 `dp[i-1]`，用 3 个滚动变量即可。见解法4。或者直接在 costs 数组上原地修改（解法4b）。

**Q3（扩展变体）：如果颜色不是 3 种而是 k 种怎么办？**
> 这就是 LeetCode 265. Paint House II。朴素做法 O(nk²)：每种颜色都要遍历上一行找非同色的最小值。优化：维护上一行的最小值和次小值，转移时 O(1)，总体 O(nk)。关键观察：除了上一行最小值对应的颜色需要用次小值，其他颜色都用最小值。

**Q4（变体）：如果房子排成环形（首尾相邻）怎么办？**
> 类似 House Robber II (213)。分两种情况：(1) 第0栋和最后一栋不同色——但因为颜色有3种，不太好直接拆。一种做法：枚举第0栋的颜色（3种），然后 DP 到最后一栋时排除该颜色，取三种情况的最小值。

## 相关题型

- **265. Paint House II** — 颜色从 3 种扩展到 k 种。复用本题的 DP 框架，转移时需要维护最小值/次小值来避免 O(k) 查找，关键改动：`min(dp[i-1][k!=j])` 用预处理的 first_min/second_min 实现。
- **198. House Robber** — 同样是线性 DP + 相邻约束，区别：本题是"相邻不同色"，House Robber 是"相邻不能都选"。DP 框架几乎一样。
- **213. House Robber II** — 环形约束的处理思路可以类比到本题的环形变体。
- **Paint House III (1473)** — 加入了"分组"约束，状态多一维，但核心思想相同。