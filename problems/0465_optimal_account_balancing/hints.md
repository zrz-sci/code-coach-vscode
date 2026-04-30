# 465. 最优账单平衡

## 核心思路

这道题的本质是：先把所有交易汇总成每个人的**净余额**（正数=别人欠他钱，负数=他欠别人钱），然后问：**最少需要几笔转账才能把所有非零余额清零？** 这是一个 NP-hard 的集合划分问题。

## 思维链

1. **读完题第一反应 → 先化简问题**：具体谁给谁转了多少不重要，重要的是每个人最终的净余额。比如 A 给 B 10块，B 给 A 3块，等价于 A 的净余额 -7，B 的净余额 +7。所以第一步是用哈希表算出每个人的 `balance`，过滤掉余额为 0 的人。

2. **简化后的问题是什么？** 我们有一组非零数（正负数之和为 0），每次可以选两个数做一笔交易（一个加、一个减同样金额），目标是用最少的交易次数把所有数变成 0。

3. **暴力怎么做？** 对于第一个非零余额，尝试和后面每一个符号相反的余额进行配对（做一笔交易），递归处理剩余的。这就是**回溯**——选一个配对 → 递归 → 撤销。

4. **瓶颈在哪？** 回溯的最坏时间复杂度是 O(n!)，因为每一步都在枚举所有可能的配对。不过 n 最多 12 个人，去掉余额为 0 的，实际非零余额数量很小。

5. **能否更优？→ 状态压缩 DP**：关键观察——如果一组人的余额之和恰好为 0，他们可以**内部自行清算**，需要 `|组| - 1` 笔交易。所以问题变成：**把所有非零余额划分成尽可能多的子集，每个子集和为 0**。子集越多，总交易次数越少（总交易次数 = n - 子集数量）。用 Bitmask DP 枚举所有子集。

6. **Bitmask DP 怎么做？** 
   - `dp[mask]` = mask 这个子集最多能被分成多少个和为 0 的独立组
   - 先预处理每个 mask 的余额和 `sum[mask]`
   - 如果 `sum[mask] == 0`，它可以作为一个整体（至少分成 1 组），然后枚举它的子集看能不能分成更多组

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 | 逐个配对余额，DFS 枚举所有方案 | O(n!) | O(n) | ⭐ 必须写出 |
| Bitmask DP | 把余额集合划分为最多的和为0子集 | O(3^n) | O(2^n) | 加分项 |

> 注：n 是非零余额的个数，最大约 12（但实际受 transactions.length ≤ 8 限制，通常 ≤ 8~10）

## 关键提示

1. **第一步永远是化简**：不要被 from/to 迷惑，先算每个人的净余额，过滤掉为 0 的。正负余额之和一定为 0（钱守恒）。

2. **回溯的优化**：对于当前第一个非零余额 `debt[i]`，只和符号相反的 `debt[j]` 配对才有意义（正配负）。如果配对后 `debt[j]` 恰好变成 0，可以直接 return（这是一个很强的剪枝）。

3. **Bitmask DP 的关键洞察**：一组和为 0 的 k 个人，内部最少 k-1 笔交易就能清算。所以总交易数 = `n - 最多分出的和为0的子集数`。

4. **枚举子集的子集**：`for (int sub = mask; sub > 0; sub = (sub - 1) & mask)` 是枚举 mask 所有非空子集的经典技巧，总复杂度 O(3^n)。

5. **回溯中"恰好清零"的剪枝为什么正确？** 如果 `debt[i] + debt[j] == 0`，那么用一笔交易同时消除两个人一定不比分开消除差，所以可以贪心地直接返回。

## 解法详解

### 解法1: 回溯 — O(n!) / O(n) ⭐ 面试首选

**思考过程**: 把所有非零余额放进数组 `debt`。从第一个位置开始，找后面所有和它符号相反的位置配对。配对后两个余额互相抵消一部分（或完全抵消），递归处理剩下的。取所有分支的最小值。

```
// 决策树示意 (debt = [-5, 3, 2]):
//
//              debt[0]=-5
//             /           \
//    和debt[1]=3配对      和debt[2]=2配对
//   debt变[-2,0,2]       debt变[-3,3,0]
//       |                     |
//   debt[0]=-2和            debt[0]=-3和
//   debt[2]=2配对           debt[1]=3配对
//   debt变[0,0,0]          debt变[0,0,0]
//   交易数=2               交易数=2
```

```cpp
class Solution {
public:
    int minTransfers(vector<vector<int>>& transactions) {
        unordered_map<int, int> balance;
        for (auto& t : transactions) {
            balance[t[0]] -= t[2];
            balance[t[1]] += t[2];
        }
        
        vector<int> debt;
        for (auto& [id, bal] : balance) {
            if (bal != 0) debt.push_back(bal);
        }
        
        return dfs(debt, 0);
    }
    
private:
    int dfs(vector<int>& debt, int start) {
        // 跳过已经清零的位置
        while (start < debt.size() && debt[start] == 0) start++;
        
        // 所有余额都清零了，不需要更多交易
        if (start == debt.size()) return 0;
        
        int res = INT_MAX;
        for (int i = start + 1; i < debt.size(); i++) {
            // 只和符号相反的配对才有意义
            if ((long long)debt[start] * debt[i] < 0) {
                // 做一笔交易：start 把钱给/从 i 转
                debt[i] += debt[start];
                res = min(res, 1 + dfs(debt, start + 1));
                debt[i] -= debt[start]; // 撤销
                
                // 关键剪枝：如果恰好抵消，这一定是最优配对之一
                if (debt[i] + debt[start] == 0) break;
            }
        }
        return res;
    }
};
```

**关键点**: 
- `while (start < debt.size() && debt[start] == 0) start++;` 跳过已清零的位置是必须的，否则会做无意义的递归。
- `if (debt[i] + debt[start] == 0) break;` 这个剪枝效果极强——当两个余额恰好互相抵消时，贪心地选择这个配对，不再尝试其他配对。

### 解法2: Bitmask DP — O(3^n) / O(2^n) — 加分项

**从解法1优化**: 回溯的问题是分支太多，且难以证明最优性。换一个角度：如果 n 个非零余额能被划分成 k 个子集，每个子集和为 0，则总交易数恰好是 n - k。所以我们要**最大化 k**。

用 bitmask 表示选了哪些余额，`dp[mask]` 表示 mask 这个集合最多能划分成多少个和为 0 的子集。

```
// 示例: debt = [-5, 3, 2]
// 用 bitmask: 0b111 表示选了全部三个
// 
// 预处理 sum:
//   0b001=-5, 0b010=3, 0b100=2
//   0b011=-2, 0b101=-3, 0b110=5
//   0b111=0  ← 和为0！
//
// dp[0b111]: 枚举子集
//   子集0b110(和=5≠0), 0b101(和=-3≠0), 0b011(和=-2≠0)
//   没有更小的和为0子集 → dp[0b111] = 1
//   答案 = 3 - 1 = 2
```

```cpp
class Solution {
public:
    int minTransfers(vector<vector<int>>& transactions) {
        unordered_map<int, int> balance;
        for (auto& t : transactions) {
            balance[t[0]] -= t[2];
            balance[t[1]] += t[2];
        }
        
        vector<int> debt;
        for (auto& [id, bal] : balance) {
            if (bal != 0) debt.push_back(bal);
        }
        
        int n = debt.size();
        int total = 1 << n;
        
        // 预处理每个子集的余额和
        vector<int> sum(total, 0);
        for (int mask = 1; mask < total; mask++) {
            // 利用最低位快速计算：mask 的和 = 去掉最低位的和 + 最低位对应的值
            int lowbit = mask & (-mask);
            int idx = __builtin_ctz(lowbit); // 最低位的索引
            sum[mask] = sum[mask ^ lowbit] + debt[idx];
        }
        
        // dp[mask] = mask 集合最多能分成多少个和为0的子集
        vector<int> dp(total, 0);
        
        for (int mask = 1; mask < total; mask++) {
            if (sum[mask] != 0) {
                // 整体和不为0，不可能所有元素都被分到和为0的子集
                // 但仍可能有部分子集和为0
                dp[mask] = 0; // 先初始化
            }
            
            // 枚举 mask 的所有非空真子集
            for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
                if (sum[sub] == 0) {
                    // sub 是一个和为0的子集，剩余是 mask ^ sub
                    dp[mask] = max(dp[mask], dp[mask ^ sub] + 1);
                }
            }
        }
        
        // 最少交易数 = 非零余额数 - 最多能分出的和为0子集数
        return n - dp[total - 1];
    }
};
```

**关键点**:
- 枚举子集的子集 `for (int sub = (mask-1) & mask; sub > 0; sub = (sub-1) & mask)` 总复杂度 O(3^n)，因为每个元素要么在 sub 中、在 mask\sub 中、或不在 mask 中。
- 预处理 `sum[mask]` 用 lowbit 技巧，避免每次都遍历所有位。

## 解法对比

| | 回溯 | Bitmask DP |
|--|------|------------|
| 时间复杂度 | O(n!) 最坏，实际有剪枝 | O(3^n) 确定性 |
| 空间复杂度 | O(n) 递归栈 | O(2^n) dp 数组 |
| 实现难度 | 中等 | 较高 |
| 面试推荐 | ⭐ 首选，容易讲清 | 加分项 |
| 适用场景 | n 较小且剪枝有效 | 需要确定性复杂度保证 |

**什么时候选哪个？** 面试中优先写回溯解法，思路清晰容易解释。如果面试官追问"能否避免最坏情况"，再给出 Bitmask DP。

## 易错点

1. **余额计算方向搞反**
   - ✗ `balance[t[0]] += t[2]; balance[t[1]] -= t[2];`（from 加了钱？不对）
   - ✓ `balance[t[0]] -= t[2]; balance[t[1]] += t[2];`（from 付出钱所以减，to 收到钱所以加）

2. **回溯中忘记跳过余额为 0 的位置**
   - ✗ 直接用 `if (start == debt.size()) return 0;`
   - ✓ 先 `while (start < debt.size() && debt[start] == 0) start++;` 再判断
   - 如果中间某个位置被之前的交易清零了但不跳过，会导致错误配对

3. **回溯中配对符号相同的**
   - ✗ 不检查符号，`debt[start]` 和同号的 `debt[i]` 配对 → 金额越滚越大，永远清不了
   - ✓ `if ((long long)debt[start] * debt[i] < 0)` 只配对异号

4. **Bitmask DP 中 `dp[0]` 的初始化**
   - ✓ `dp[0] = 0`（空集分成 0 个和为 0 的子集）
   - 如果初始化为其他值，整个递推都会出错

5. **整数溢出**
   - ✗ `debt[start] * debt[i] < 0` 当 debt 值较大时可能溢出 int
   - ✓ 强转 `(long long)debt[start] * debt[i] < 0`

## 面试追问

**Q1: 为什么先化简为净余额是正确的？（基础理解）**

> 每笔原始交易只改变两个人的余额。最终目标是让所有人余额为 0。中间经过了哪些具体交易不影响最终状态，只要净效果一样就行。所以可以先汇总成净余额，再找最少的新交易使之清零。

**Q2: 回溯中 `break` 剪枝为什么是正确的？（优化）**

> 当 `debt[i] + debt[start] == 0` 时，一笔交易就能同时消除两个人的债务。如果不选这个配对，而是选另一个部分抵消的配对，那么 `debt[start]` 消了但 `debt[i]` 还在，后面还需要额外交易来消除 `debt[i]`。所以选择完全抵消的配对一定不比其他选择差。

**Q3: 能否用贪心（每次选最大正值和最小负值配对）？（变体 / 反例）**

> 不行。反例：`debt = [-5, -3, 2, 3, 3]`。贪心会选 -5 和 3 配对（最大负配最大正），需要 4 笔交易。但最优是 `-5+2+3=0` 和 `-3+3=0`，分成两组只需 3 笔交易。这就是为什么需要回溯或 DP 而不能贪心。

**Q4: Bitmask DP 的 O(3^n) 是怎么来的？**

> 每个元素在枚举子集的子集时有三种状态：在内层子集 sub 中、在外层 mask\sub 中、不在当前 mask 中。根据组合恒等式 ∑C(n,k)·2^k = 3^n，总操作数为 O(3^n)。

## 相关题型

- **1049. 最后一块石头的重量 II** — 也是把集合分成两组使差最小，复用 bitmask/DP 子集和的思想
- **698. 划分为 k 个相等的子集** — 复用 Bitmask DP 划分子集的框架，区别是本题每个子集和为 0 而非固定值
- **NP-hard 集合划分问题** — 本题是经典的 minimum edge cover in debt graph，理解本题有助于理解 NP-hard 问题的状压解法