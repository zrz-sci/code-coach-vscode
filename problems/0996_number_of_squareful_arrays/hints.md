# 996. Number of Squareful Arrays

[file://solution.cpp](./solution.cpp)

## 核心思路

回溯/状态压缩 DP + 完全平方数判断。核心是找出数组的所有排列，使得相邻两数之和为完全平方数。由于 n <= 12，可以用回溯（带去重）或状压 DP 来枚举。预处理出所有元素对之间是否满足"squareful"条件，将问题转化为在"兼容图"上寻找哈密尔顿路径的数量。

## 思维链

1. **建模**：将每个元素视为图的节点，如果 nums[i] + nums[j] 是完全平方数，则 i 和 j 之间连边。
2. **目标**：找出图中哈密尔顿路径的数量（经过所有节点恰好一次的路径）。
3. **方法选择**：n <= 12 很小，可以用：
   - 回溯 + 剪枝（经典排列枚举）
   - 状压 DP（bitmask 表示已访问集合）
4. **去重处理**：数组可能有重复元素，需要排序后跳过同层重复选择。
5. **完全平方数判断**：`sqrt(x)` 后检查 `(int)sqrt(x) * (int)sqrt(x) == x`。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 回溯 + 剪枝 | O(n!) 最坏 | O(n) | 实现简单，重复元素多时高效 |
| 状压 DP | O(n^2 * 2^n) | O(n * 2^n) | 适合 n <= 20，无需手动去重 |
| 图论 + 回溯 | O(n!) | O(n^2) | 预建邻接表，剪枝更高效 |

## 关键提示

1. **完全平方数判断**：`int root = sqrt(sum); return root * root == sum;` 注意浮点精度，建议用 `(long long)round(sqrt(sum))` 再验证。
2. **去重关键**：排序数组后，同一层递归中跳过相同值的元素（`if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;`）。
3. **状压 DP 定义**：`dp[mask][i]` = 已选集合为 mask，最后一个选的是 i 的排列数。
4. **状压去重**：对于重复元素，限制相同值的元素按索引顺序使用（即 mask 中低索引必须在高索引之前选）。

## 解法详解

### 解法一：回溯 + 排序去重（推荐面试）

**思路**：经典排列枚举，加上两个剪枝条件：(1) 同层去重；(2) 与前一个选中元素不构成平方数时剪枝。

**步骤**：
1. 排序 nums。
2. 回溯函数参数：当前排列 path，已使用标记 used。
3. 每层选择时：
   - 跳过已使用的元素。
   - 跳过同层重复元素（`nums[i] == nums[i-1] && !used[i-1]`）。
   - 检查 `path.back() + nums[i]` 是否为完全平方数。
4. path 长度 == n 时，计数 +1。

### 解法二：状态压缩 DP

**思路**：用 bitmask 表示已选择的元素集合。

**转移**：
```
dp[mask | (1<<j)][j] += dp[mask][i]
条件: j 未在 mask 中, 且 nums[i]+nums[j] 是完全平方数
```

**初始化**：`dp[1<<i][i] = 1` 对所有 i。

**去重**：对于相同值的元素，只允许索引最小的那个作为"首次出现"。即如果 `nums[i] == nums[j] && i < j`，则 `dp[1<<j][j] = 0`（只有当 mask 中包含 i 时，j 才能被选）。

**答案**：`sum(dp[(1<<n)-1][i])` 对所有 i。

### 解法三：图论建模 + DFS

**思路**：建立邻接表，对每个可能的起点 DFS 搜索哈密尔顿路径。用 `count` map 去重。

## 易错点

1. **浮点精度**：`sqrt` 返回浮点数，直接取 `int` 可能有误差。建议 `int s = round(sqrt(x)); return s * s == x;`。
2. **去重逻辑混淆**：排列去重用 `!used[i-1]`（同层跳过）或 `used[i-1]`（强制顺序），两种都对但要统一。
3. **状压 DP 去重遗漏**：忘记处理重复元素，导致相同排列被重复计数。
4. **空数组/单元素**：n=1 时直接返回 1，无需检查相邻和。
5. **0 的处理**：nums[i] 可以是 0，`0 + 0 = 0` 是完全平方数，需要正确处理。

## 面试追问

1. **Q: 回溯和状压 DP 哪个更好？**
   A: n <= 12 时两者都可以。回溯更直观、代码量少；状压 DP 在 n 较大（如 15-20）时更稳定，因为有严格的多项式边界。

2. **Q: 如何优化判断完全平方数？**
   A: 预处理所有可能的和（最大 2*10^9），或者对每对 (i,j) 预计算并缓存结果。

3. **Q: 如果要输出所有合法排列而非计数呢？**
   A: 回溯法天然支持输出排列。状压 DP 需要额外回溯路径。

4. **Q: 这道题与哈密尔顿路径问题的关系？**
   A: 本质就是在"squareful 兼容图"上数哈密尔顿路径，是 NP-hard 问题，n 小时才可行。

## 相关题型

- [46. Permutations](https://leetcode.com/problems/permutations/) - 基础排列
- [47. Permutations II](https://leetcode.com/problems/permutations-ii/) - 有重复元素的排列
- [526. Beautiful Arrangement](https://leetcode.com/problems/beautiful-arrangement/) - 状压 DP 计数排列
- [943. Find the Shortest Superstring](https://leetcode.com/problems/find-the-shortest-superstring/) - 状压 DP + 哈密尔顿路径
- [784. Letter Case Permutation](https://leetcode.com/problems/letter-case-permutation/) - 回溯枚举
