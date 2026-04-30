# 322. 零钱兑换

## 核心思路

本质上是一个**完全背包问题**：每种硬币可以用无限次，求凑满目标金额所需的最少硬币数。也可以理解为一个最短路径问题——从金额 0 到金额 amount，每次可以"跳"一个硬币面值，求最少跳几步。

## 思维链

1. **读完题第一反应**：要凑出 amount，每一步我可以选择任意一种硬币。这是一个"做选择"的问题 → 暴力递归穷举所有组合方式。

2. **暴力递归的结构**：要凑出金额 `n`，我可以先用一枚面值为 `c` 的硬币，问题变成凑出 `n - c`。所以 `minCoins(n) = 1 + min(minCoins(n - c))` 对所有硬币 `c`。这是一棵决策树，每个节点分出 `len(coins)` 个分支。

3. **暴力解的瓶颈**：决策树中有大量重复子问题！比如 coins=[1,2,5], amount=11 时，`minCoins(9)` 会被从 `minCoins(11-2)` 和 `minCoins(10-1)` 两条路径重复计算。指数级时间复杂度。

4. **突破瓶颈**：既然有重叠子问题 + 最优子结构 → **动态规划**。用 `dp[i]` 记录凑出金额 `i` 最少需要多少枚硬币，避免重复计算。

5. **状态转移**：`dp[i] = min(dp[i - c] + 1)` 对所有满足 `c <= i` 的硬币 `c`。从 `dp[0] = 0` 开始，一路填到 `dp[amount]`。

6. **另一个视角（BFS）**：把每个金额看作图中的节点，每种硬币代表一条边。从 `amount` 出发 BFS，每次减去一枚硬币面值，最先到达 0 的层数就是最少硬币数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 决策树穷举 | O(S^n) 指数级 | O(S) 递归栈 | 能说出即可 |
| 记忆化搜索 | 递归 + memo | O(S × n) | O(S) | 能说出即可 |
| 动态规划(递推) | 自底向上填表 | O(S × n) | O(S) | ⭐ 必须写出 |
| BFS | 最短路径 | O(S × n) | O(S) | 加分项 |

> S = amount, n = coins.length

## 关键提示

1. **从递归入手**：先想 `coinChange(amount)` 怎么用递归定义 —— 选一枚硬币后，问题规模减小了多少？

2. **画出递归树，找重叠子问题**：
   ```
   coins = [1, 2, 5], amount = 11
                        11
                     /   |   \
                   10    9    6      ← 减去 1, 2, 5
                  /|\   /|\  /|\
                 9 8 5 8 7 4 5 4 1   ← 注意：9 出现了两次！
   ```
   → 这就是加 memo 或转 DP 的信号。

3. **DP 初始化**：`dp[0] = 0`（凑出金额 0 需要 0 枚硬币），其余初始化为 `amount + 1`（一个不可能达到的大值，比 INT_MAX 安全，避免 +1 溢出）。

4. **贪心为什么不行**：直觉上先用大面额硬币再补小面额，但 coins=[1,3,4], amount=6 时，贪心选 4+1+1=3枚，而最优是 3+3=2枚。

5. **BFS 的直觉**：如果你把"每种金额"看作图中的节点，BFS 的层数天然就是最短路径（最少硬币数）。

## 解法详解

### 解法1: 暴力递归 — O(S^n) / O(S)

**思考过程**：最原始的想法——要凑金额 amount，我枚举每种硬币，选一枚后递归处理剩余金额，取所有选择中的最小值。

```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        // base case: 金额为0，不需要任何硬币
        if (amount == 0) return 0;
        // 金额为负，说明这条路走不通
        if (amount < 0) return -1;
        
        int res = INT_MAX;
        for (int coin : coins) {
            // 选一枚 coin，递归解决剩余金额
            int sub = coinChange(coins, amount - coin);
            if (sub == -1) continue;  // 子问题无解，跳过
            res = min(res, sub + 1);  // +1 是当前选的这枚硬币
        }
        return res == INT_MAX ? -1 : res;
    }
};
```

**关键点**：这个解法会超时（指数级），但它展示了问题的递归结构，是后续所有优化的基础。

---

### 解法2: 记忆化搜索（自顶向下 DP） — O(S × n) / O(S)

**从解法1优化**：解法1中大量子问题被重复计算（如上面递归树中 `9` 出现多次）。加一个 memo 数组，算过的金额直接返回。

```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        // memo[i] = -2 表示还没算过，-1 表示无解，>=0 表示最少硬币数
        vector<int> memo(amount + 1, -2);
        return dfs(coins, amount, memo);
    }
    
    int dfs(vector<int>& coins, int amount, vector<int>& memo) {
        if (amount == 0) return 0;
        if (amount < 0) return -1;
        if (memo[amount] != -2) return memo[amount];  // 已经算过，直接返回
        
        int res = INT_MAX;
        for (int coin : coins) {
            int sub = dfs(coins, amount - coin, memo);
            if (sub == -1) continue;
            res = min(res, sub + 1);
        }
        memo[amount] = (res == INT_MAX) ? -1 : res;
        return memo[amount];
    }
};
```

**关键点**：memo 的初始值不能用 0 或 -1（它们都是合法的返回值），必须用一个"未计算"的标记值（这里用 -2）。

---

### 解法3: 动态规划（自底向上递推） — O(S × n) / O(S) ⭐ 面试首选

**从记忆化搜索翻译**：把递归改成循环，从小金额到大金额依次填表。

```
// dp 填表过程 (coins = [1, 2, 5], amount = 11):
//
// dp[i] 表示凑出金额 i 所需的最少硬币数
// 初始: dp[0]=0, 其余=12 (amount+1, 一个"不可能"的大值)
//
// i=1:  dp[1] = min(dp[1-1]+1) = min(dp[0]+1) = 1          用1枚1
// i=2:  dp[2] = min(dp[2-1]+1, dp[2-2]+1) = min(2, 1) = 1  用1枚2
// i=3:  dp[3] = min(dp[2]+1, dp[1]+1) = min(2, 2) = 2      用1+2
// i=4:  dp[4] = min(dp[3]+1, dp[2]+1) = min(3, 2) = 2      用2+2
// i=5:  dp[5] = min(dp[4]+1, dp[3]+1, dp[0]+1) = 1         用1枚5
// ...
// i=11: dp[11] = min(dp[10]+1, dp[9]+1, dp[6]+1) = 3       用5+5+1
```

```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        // dp[i]: 凑出金额 i 最少需要多少枚硬币
        // 初始化为 amount+1 (一个不可能的上界，比 INT_MAX 安全)
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;  // 凑出金额 0 需要 0 枚硬币
        
        // 从小到大枚举每个金额
        for (int i = 1; i <= amount; i++) {
            // 尝试每种硬币
            for (int coin : coins) {
                if (coin <= i) {
                    // 选这枚硬币，需要 dp[i - coin] + 1 枚
                    // 不选（保持当前 dp[i]），取更小的
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        
        // 如果 dp[amount] 仍然是初始值，说明凑不出来
        return dp[amount] > amount ? -1 : dp[amount];
    }
};
```

**关键点**：
- 为什么用 `amount + 1` 而不是 `INT_MAX`：因为 `dp[i - coin] + 1` 会溢出！
- 这就是**完全背包**问题：每种硬币可以选无限次，正序遍历 `i` 保证了这一点。如果是 0/1 背包（每种只能选一次），需要逆序遍历。

---

### 解法4: BFS — O(S × n) / O(S)

**换个角度思考**：把问题看成图的最短路径。节点是 0 到 amount 的每个金额，从节点 `v` 到节点 `v - coin` 有一条边。从 `amount` 出发 BFS，第一次到达 `0` 的层数就是答案。

```
// BFS 过程 (coins = [1, 2, 5], amount = 11):
//
// Level 0: {11}
// Level 1: {10, 9, 6}           ← 11-1, 11-2, 11-5
// Level 2: {9, 8, 5, 8, 7, 4, 5, 4, 1}  ← 去重后 {8, 7, 5, 4, 1}
// Level 3: {..., 0}             ← 第3层到达0，答案=3
```

```cpp
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        if (amount == 0) return 0;
        
        queue<int> q;
        // visited 避免重复访问同一金额（关键优化！）
        vector<bool> visited(amount + 1, false);
        q.push(amount);
        visited[amount] = true;
        int steps = 0;
        
        while (!q.empty()) {
            steps++;
            int size = q.size();
            for (int i = 0; i < size; i++) {
                int curr = q.front();
                q.pop();
                for (int coin : coins) {
                    int next = curr - coin;
                    if (next == 0) return steps;  // 到达目标
                    if (next > 0 && !visited[next]) {
                        visited[next] = true;
                        q.push(next);
                    }
                }
            }
        }
        
        return -1;  // BFS 遍历完仍未到达 0
    }
};
```

**关键点**：必须用 `visited` 数组去重，否则同一金额会被反复入队，时间退化为指数级。BFS 天然保证第一次到达就是最短路径。

## 解法对比

| | 暴力递归 | 记忆化搜索 | DP 递推 ⭐ | BFS |
|---|---|---|---|---|
| 时间 | O(S^n) 指数级 | O(S × n) | O(S × n) | O(S × n) |
| 空间 | O(S) 递归栈 | O(S) memo+栈 | O(S) dp数组 | O(S) 队列+visited |
| 思路 | 穷举 | 缓存子问题 | 自底向上填表 | 最短路径 |
| 适用 | 理解问题 | 递归自然时 | **面试首选** | 换个视角 |

- **DP 递推**是面试标准答案：代码简洁、无递归栈溢出风险、好解释
- **记忆化搜索**在子问题空间稀疏时可能更快（不用填所有状态）
- **BFS**提供了一种完全不同的思考角度，面试中提及会加分

## 易错点

1. **初始化用 `INT_MAX` 导致溢出**：
   - ✗ `vector<int> dp(amount + 1, INT_MAX);` → `dp[i - coin] + 1` 整数溢出
   - ✓ `vector<int> dp(amount + 1, amount + 1);` → 安全的"不可能"上界（最多用 amount 枚面值1的硬币）

2. **返回值判断写错**：
   - ✗ `return dp[amount] == amount + 1 ? -1 : dp[amount];` → 当恰好需要 amount+1 枚时误判（实际上不可能，但逻辑不严谨）
   - ✓ `return dp[amount] > amount ? -1 : dp[amount];` → 更直观：合法答案最多是 amount（全用面值1）

3. **记忆化搜索中 memo 标记冲突**：
   - ✗ 用 `memo[amount] = -1` 表示未计算 → 和"无解"返回值 -1 冲突，导致无解的子问题每次都重新计算
   - ✓ 用 -2 或 `optional` 或单独 bool 数组标记是否已计算

4. **BFS 忘记 visited 去重**：
   - ✗ 不加 visited → 同一金额反复入队，时间退化为指数级，MLE/TLE
   - ✓ 入队前标记 `visited[next] = true`

5. **混淆完全背包和 0/1 背包的遍历顺序**：
   - 完全背包（本题）：外层遍历金额 `i` 从小到大 → `dp[i-coin]` 是当前轮的值，允许同一硬币重复使用
   - 0/1 背包：外层遍历物品，内层金额从大到小 → 保证每种物品只用一次

## 面试追问

**Q1: 暴力递归为什么会超时？能画出递归树吗？**
> 递归树是 n 叉树（n = coins.length），高度最深为 amount（全用面值1），大量节点重复。比如 coins=[1,2], amount=5 时，`f(3)` 会从 `f(5)-2` 和 `f(4)-1` 两条路径各算一次。重叠子问题导致指数级复杂度。

**Q2: 为什么贪心（优先用大面额）不行？能举反例吗？**
> coins=[1, 3, 4], amount=6。贪心：4+1+1=3枚。最优：3+3=2枚。贪心每步局部最优，但大面额可能"占位"导致后续无法高效填充。

**Q3: 这道题和"完全背包"是什么关系？如果每种硬币只能用一次，代码怎么改？**
> 本题就是完全背包的特例：物品=硬币，容量=amount，"价值"=1（每枚硬币贡献1个计数），目标是最小化总价值。如果每种只能用一次（0/1 背包），需要外层遍历硬币、内层金额**从大到小**遍历，保证每种硬币最多用一次。

**Q4: 如果 amount 非常大（比如 10^9），但 coins 面额也很大，怎么优化？**
> DP 的 O(amount × n) 在 amount=10^9 时不可行。此时可以考虑数学方法（如扩展欧几里得算法处理两种硬币的情况），或者利用硬币面额的数学性质剪枝。这已经超出常规面试范围，但说明你对问题规模有意识是加分的。

## 相关题型

- **518. 零钱兑换 II** — 复用同样的完全背包框架，区别：本题求**最少硬币数**（取 min），518 求**组合数**（求 sum）。转移方程从 `dp[i] = min(dp[i], dp[i-coin]+1)` 改为 `dp[i] += dp[i-coin]`。另外 518 外层遍历硬币、内层遍历金额（避免重复组合），本题两层顺序都可以。
- **279. 完全平方数** — 完全相同的问题结构！把"硬币面额"换成"完全平方数"，把 coins 数组换成 [1, 4, 9, 16, ...]，代码几乎一模一样。
- **377. 组合总和 Ⅳ** — 同样是完全背包变体，但求的是排列数（顺序有关），所以外层遍历金额、内层遍历硬币。
- **背包问题系列**：本题（完全背包求最小值） → 518（完全背包求组合数） → 0/1 背包（每种物品只用一次） → 多重背包（每种物品有数量限制）。理解本题后，做这个系列只需要调整遍历顺序和转移方程。