# 474. 一和零

## 核心思路

这道题本质是一个**二维费用的 0/1 背包问题**：每个字符串是一件"物品"，它消耗两种资源（0 的个数和 1 的个数），价值为 1（选一个算一个），背包容量是 `(m, n)`，求最大价值。

## 思维链

1. **读完题第一反应**：要从 `strs` 中选出一个子集，使得所有字符串的 0 总数 ≤ m、1 总数 ≤ n，且子集尽量大。最朴素的做法？枚举所有子集（2^len 个），检查每个子集是否合法，取最大的。

2. **暴力解的瓶颈**：枚举所有子集是 O(2^len)，len 最大 600，完全不可行。但这个"选/不选"的结构是不是很眼熟？

3. **联想到背包**：每个字符串有两个"重量"（0 的个数 `zeros` 和 1 的个数 `ones`），有两个容量限制（`m` 和 `n`）。每个字符串只能选一次 → **0/1 背包**。唯一的区别是普通背包只有一维容量，这里有二维容量。

4. **状态定义**：`dp[i][j]` = 在 0 的预算为 `i`、1 的预算为 `j` 时，最多能选多少个字符串。

5. **状态转移**：对于每个字符串（消耗 `zeros` 个 0，`ones` 个 1），要么不选（`dp[i][j]` 不变），要么选（`dp[i-zeros][j-ones] + 1`），取较大值。

6. **空间优化**：和经典 0/1 背包一样，遍历物品时**倒序**遍历容量维度，就能把三维 DP 压缩到二维。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有 2^len 子集 | O(2^len × L) | O(1) | 能说出即可 |
| 记忆化搜索 | 自顶向下递归 + memo | O(len × m × n) | O(len × m × n) | 能说出即可 |
| 三维 DP（递推） | 自底向上填表 | O(len × m × n) | O(len × m × n) | 加分项 |
| 二维 DP（空间优化）| 滚动数组压缩 | O(len × m × n) | O(m × n) | ⭐ 必须写出 |

## 关键提示

1. **先不管优化，想想递归怎么写**：对于第 `k` 个字符串，你有两个选择——选它或不选它。如果选，剩余 0 的预算减少 `zeros[k]`，1 的预算减少 `ones[k]`。这就是递归结构。

2. **重叠子问题在哪？** 不同的"选/不选"组合可能到达相同的 `(k, 剩余0, 剩余1)` 状态。比如"选了第1个不选第2个" vs "不选第1个选第2个"，到第3个时剩余预算可能相同。

3. **和经典 0/1 背包的对应关系**：物品=字符串，重量=(zeros, ones) 是二维的，价值=1，容量=(m, n)。

4. **空间优化的关键**：为什么倒序遍历容量？因为 0/1 背包每件物品只能选一次，倒序保证 `dp[i-zeros][j-ones]` 用的是"没选当前物品"时的旧值。如果正序，`dp[i-zeros][j-ones]` 可能已经被当前物品更新过了，相当于同一件物品被选了多次（那就变成完全背包了）。

5. **DP 填表方向**：
```
对于每个字符串 str (外层循环):
  对于 i 从 m 到 zeros (倒序):
    对于 j 从 n 到 ones (倒序):
      dp[i][j] = max(dp[i][j], dp[i-zeros][j-ones] + 1)
```

## 解法详解

### 解法1: 暴力递归（DFS）— O(2^len) / O(len)

**思考过程**: 最直觉的方式——对每个字符串"选或不选"，递归地枚举所有可能。

```cpp
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        return dfs(strs, 0, m, n);
    }
    
    int dfs(vector<string>& strs, int idx, int zeroLeft, int oneLeft) {
        if (idx == strs.size()) return 0;
        
        // 统计当前字符串的 0 和 1 数量
        int zeros = count(strs[idx].begin(), strs[idx].end(), '0');
        int ones = strs[idx].size() - zeros;
        
        // 不选当前字符串
        int skip = dfs(strs, idx + 1, zeroLeft, oneLeft);
        
        // 选当前字符串（前提：预算够）
        int pick = 0;
        if (zeroLeft >= zeros && oneLeft >= ones) {
            pick = 1 + dfs(strs, idx + 1, zeroLeft - zeros, oneLeft - ones);
        }
        
        return max(skip, pick);
    }
};
```

**关键点**: 时间复杂度 O(2^600)，完全不可行，但它展示了"选/不选"的递归结构，是后续 DP 的基础。

---

### 解法2: 记忆化搜索 — O(len × m × n) / O(len × m × n)

**从解法1优化**: 暴力递归中，状态由 `(idx, zeroLeft, oneLeft)` 唯一确定。不同的选择路径可能到达同一个状态 → 重叠子问题 → 加 memo。

```cpp
class Solution {
public:
    int memo[601][101][101]; // memo[idx][zeroLeft][oneLeft]
    
    int findMaxForm(vector<string>& strs, int m, int n) {
        memset(memo, -1, sizeof(memo));
        return dfs(strs, 0, m, n);
    }
    
    int dfs(vector<string>& strs, int idx, int zeroLeft, int oneLeft) {
        if (idx == strs.size()) return 0;
        if (memo[idx][zeroLeft][oneLeft] != -1) 
            return memo[idx][zeroLeft][oneLeft];
        
        int zeros = count(strs[idx].begin(), strs[idx].end(), '0');
        int ones = strs[idx].size() - zeros;
        
        int skip = dfs(strs, idx + 1, zeroLeft, oneLeft);
        int pick = 0;
        if (zeroLeft >= zeros && oneLeft >= ones) {
            pick = 1 + dfs(strs, idx + 1, zeroLeft - zeros, oneLeft - ones);
        }
        
        return memo[idx][zeroLeft][oneLeft] = max(skip, pick);
    }
};
```

**关键点**: 空间 O(len × m × n) 可能很大（600 × 100 × 100 = 6,000,000），但时间已经可行了。注意 memo 数组用 `memset` 初始化为 -1 表示未计算。

---

### 解法3: 三维 DP（递推）— O(len × m × n) / O(len × m × n)

**从记忆化搜索翻译**: 把递归改成 for 循环填表。`dp[k][i][j]` = 前 `k` 个字符串中，使用不超过 `i` 个 0 和 `j` 个 1 时，最多能选多少个。

```
// dp 填表过程示例:
// strs = ["10", "0001", "111001", "1", "0"], m=5, n=3
//
// 对于每个字符串 k:
//   dp[k][i][j] = max(
//       dp[k-1][i][j],              ← 不选第k个
//       dp[k-1][i-zeros][j-ones]+1  ← 选第k个(如果预算够)
//   )
//
// 依赖关系: dp[k] 只依赖 dp[k-1] → 可以压缩掉第一维!
```

```cpp
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        // dp[k][i][j]: 前k个字符串，最多i个0和j个1时的最大子集大小
        vector<vector<vector<int>>> dp(len + 1, vector<vector<int>>(m + 1, vector<int>(n + 1, 0)));
        
        for (int k = 1; k <= len; k++) {
            int zeros = count(strs[k-1].begin(), strs[k-1].end(), '0');
            int ones = strs[k-1].size() - zeros;
            
            for (int i = 0; i <= m; i++) {
                for (int j = 0; j <= n; j++) {
                    // 不选第k个字符串
                    dp[k][i][j] = dp[k-1][i][j];
                    // 选第k个字符串（前提：预算够）
                    if (i >= zeros && j >= ones) {
                        dp[k][i][j] = max(dp[k][i][j], dp[k-1][i-zeros][j-ones] + 1);
                    }
                }
            }
        }
        
        return dp[len][m][n];
    }
};
```

**关键点**: 这个版本让我们清楚看到 `dp[k]` 只依赖 `dp[k-1]`，所以可以压缩掉物品维度。

---

### 解法4: 二维 DP（空间优化）— O(len × m × n) / O(m × n) ⭐ 面试首选

**从解法3优化**: 既然 `dp[k]` 只依赖 `dp[k-1]`，我们只保留一个二维数组。关键是**倒序遍历** `i` 和 `j`，保证读到的是"上一轮"的值。

```
// 为什么倒序? 以一维背包类比:
// 
// 正序遍历 (错误 — 相当于完全背包，物品可重复选):
//   dp[3] 被更新后, dp[5] 用到的 dp[3] 已经是"选了当前物品"的值
//   → dp[5] 可能再选一次当前物品 → 一件物品被选了多次!
//
// 倒序遍历 (正确 — 0/1 背包，每件只选一次):
//   先更新 dp[5], 此时 dp[3] 还是旧值 (没选当前物品)
//   → 保证每件物品最多选一次
//
// 同理推广到二维: i 从 m→zeros, j 从 n→ones 倒序遍历
```

```cpp
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        // dp[i][j]: 使用不超过 i 个 0 和 j 个 1 时，最多能选多少个字符串
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (const string& s : strs) {
            // 统计当前字符串的 0 和 1 数量
            int zeros = count(s.begin(), s.end(), '0');
            int ones = s.size() - zeros;
            
            // 倒序遍历: 保证每个字符串只被"选"一次 (0/1 背包核心)
            for (int i = m; i >= zeros; i--) {
                for (int j = n; j >= ones; j--) {
                    // 不选 s: dp[i][j] 保持不变 (隐含在 max 中)
                    // 选 s:   dp[i-zeros][j-ones] + 1
                    dp[i][j] = max(dp[i][j], dp[i - zeros][j - ones] + 1);
                }
            }
        }
        
        return dp[m][n];
    }
};
```

**关键点**: 
- 外层循环遍历物品（字符串），内层倒序遍历两个容量维度
- `i` 从 `m` 递减到 `zeros`，`j` 从 `n` 递减到 `ones`——小于 `zeros`/`ones` 的部分不需要遍历，因为预算不够选当前物品

## 解法对比

| | 暴力递归 | 记忆化搜索 | 三维DP | 二维DP(空间优化) |
|---|---------|----------|-------|----------------|
| 时间 | O(2^len) | O(len×m×n) | O(len×m×n) | O(len×m×n) |
| 空间 | O(len) | O(len×m×n) | O(len×m×n) | **O(m×n)** |
| 核心价值 | 理解递归结构 | 发现重叠子问题 | 理解填表依赖 | 面试标准答案 |
| 关系 | 基础 | +memo | 递归→递推 | +空间压缩 |

**什么时候选哪个：** 面试直接写解法4；如果面试官要求你"从头推导"，从解法1讲起，自然过渡到解法4。

## 易错点

1. **正序遍历容量导致物品被重复选**
   - ✗ `for (int i = zeros; i <= m; i++)` → 一个字符串可能被选多次
   - ✓ `for (int i = m; i >= zeros; i--)` → 0/1 背包必须倒序

2. **dp 初始化错误**
   - ✗ `dp[i][j] = -1` 或 `dp[i][j] = INT_MIN` → 不选任何字符串时答案是 0
   - ✓ `dp[i][j] = 0` → 什么都不选，子集大小为 0

3. **统计 0 和 1 数量搞反**
   - ✗ 混淆 zeros 和 ones，特别是用 `count(s.begin(), s.end(), '1')` 后忘记 `ones = s.size() - zeros`
   - ✓ 统计完后打印验证一下（调试时）

4. **循环下界写错**
   - ✗ `for (int i = m; i >= 0; i--)` → 浪费计算，且当 `i < zeros` 时 `dp[i-zeros]` 下标越界
   - ✓ `for (int i = m; i >= zeros; i--)` → 既避免越界，又跳过无意义计算

## 面试追问

**Q1: 这道题和经典 0/1 背包有什么区别？**
→ 经典 0/1 背包是一维容量（重量 ≤ W），这道题是二维容量（0 的个数 ≤ m，1 的个数 ≤ n）。把 dp 数组从一维扩展到二维即可，核心的"选/不选"逻辑和倒序遍历技巧完全一致。

**Q2: 如果改成每个字符串可以选无限次呢？**
→ 变成**二维完全背包**。代码只需改一行：把倒序遍历改成正序遍历。即 `for (int i = zeros; i <= m; i++)` 和 `for (int j = ones; j <= n; j++)`。正序保证 `dp[i-zeros][j-ones]` 是"已经考虑过选当前物品"的值，从而允许重复选。

**Q3: 能否输出具体选了哪些字符串？**
→ 需要回溯路径。用三维 DP（不压缩空间），从 `dp[len][m][n]` 开始，如果 `dp[k][i][j] != dp[k-1][i][j]`，说明第 k 个字符串被选了，然后跳到 `dp[k-1][i-zeros][j-ones]` 继续追踪。

## 相关题型

- **416. 分割等和子集** — 0/1 背包（一维容量），本题的简化版。复用"选/不选 + 倒序遍历"模板，区别：416 的 dp 值是 bool（能否达到），本题是 int（最大数量）。
- **494. 目标和** — 0/1 背包变体（方案数），复用同样的框架，区别：dp 转移用 `+=` 而不是 `max`。
- **322. 零钱兑换** — **完全背包**（物品可重复选），对比：倒序→正序。
- **518. 零钱兑换 II** — 完全背包（方案数），和本题的"0/1 vs 完全"对比学习效果最好。