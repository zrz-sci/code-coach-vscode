# 813. Largest Sum of Averages - 提示与解析

## 核心思路

本题要求将数组分成**最多 k 个**非空连续子数组，使得各子数组平均值之和最大。这是一个经典的**区间 DP + 前缀和**问题。定义 dp[i][j] 表示将前 i 个元素分成 j 组所能获得的最大平均值之和。利用前缀和快速计算任意子数组的平均值，通过枚举最后一组的起始位置进行状态转移。关键洞察：分的组数越多，平均值之和一定不会减小（单独一个大数的平均值就是它本身），所以"最多 k 组"的最优解就是"恰好 k 组"的最优解。

## 思维链

1. **识别 DP 模式**：将数组分成 k 段，求最优值 -> 典型的分段 DP
2. **定义状态**：dp[i][j] = 前 i 个元素分成 j 组的最大平均值之和
3. **预处理前缀和**：prefix[i] = nums[0] + ... + nums[i-1]，用于 O(1) 计算子数组和
4. **状态转移**：dp[i][j] = max(dp[m][j-1] + avg(m+1..i))，枚举 m 从 j-1 到 i-1
5. **基础情况**：dp[i][1] = prefix[i] / i（全部放在一组的平均值）
6. **答案**：dp[n][k]
7. **空间优化**：可以将二维 dp 压缩为一维

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|-----------|-----------|---------|
| 二维 DP | O(n^2 * k) | O(n * k) | 标准解法，思路清晰 |
| 一维 DP（空间优化） | O(n^2 * k) | O(n) | 优化空间 |
| 记忆化搜索 | O(n^2 * k) | O(n * k) | 代码更直观 |

## 关键提示

1. **前缀和加速**：不用前缀和的话每次计算子数组平均值都是 O(n)，总时间会变成 O(n^3 * k)
2. **"最多 k 组" = "恰好 k 组"**：多分一组不会让结果变差。直觉：把一组拆成两组，大的数单独一组，平均值之和只会增大或不变
3. **枚举范围**：分 j 组至少需要 j 个元素，所以 i >= j，枚举切分点 m 时 m >= j-1
4. **一维优化关键**：第 j 轮只依赖第 j-1 轮的值，所以可以滚动数组，但注意 i 要从大到小遍历

## 解法详解

### 解法一：二维 DP（推荐）

**状态定义**：
- dp[i][j] = 将 nums[0..i-1]（前 i 个元素）分成 j 组的最大平均值之和
- prefix[i] = nums[0] + nums[1] + ... + nums[i-1]

**转移方程**：
```
dp[i][j] = max over m in [j-1, i-1] of:
    dp[m][j-1] + (prefix[i] - prefix[m]) / (i - m)
```
含义：前 m 个元素分 j-1 组，剩下 nums[m..i-1] 作为第 j 组。

**基础情况**：
- dp[i][1] = prefix[i] / i（前 i 个元素全放一组）

**答案**：dp[n][k]

### 解法二：一维 DP（空间优化）

观察到 dp[i][j] 只依赖 dp[*][j-1]，可以用一维数组滚动更新。
- 外层循环 j 从 2 到 k
- 内层循环 i 从 n 到 j（**逆序**，防止覆盖旧值）
- dp[i] = max(dp[m] + avg(m+1..i)) for m in [j-1, i-1]

### 解法三：记忆化搜索

定义 dfs(start, groups) 表示从下标 start 开始，还可以分 groups 组的最大结果。
- 枚举当前组的结束位置 end 从 start 到 n-groups
- 递归调用 dfs(end+1, groups-1)
- 加上当前段的平均值
- 用 memo 记忆化

## 易错点

1. **忘记前缀和**：直接在转移中重新求和会导致 TLE
2. **循环边界不对**：分 j 组至少需要 j 个元素，i 的范围是 [j, n]，切分点 m 的范围是 [j-1, i-1]
3. **一维优化时正序遍历**：必须逆序遍历 i，否则会用到已更新的 dp[m]（本轮值而非上轮值）
4. **浮点精度**：除法用 double，前缀和用 double 或做类型转换
5. **"最多 k 组"的误解**：不需要枚举 1 到 k 取最大，恰好 k 组就是最优

## 面试追问

1. **Q: 为什么分更多组结果一定不差？**
   A: 把一组 [a1, a2, ..., am] 拆成 [a1] 和 [a2, ..., am]，根据数学不等式，如果 a1 >= avg(a2..am)，则拆开后平均值之和变大；否则也可以选择不拆。所以最优解不会因为允许更多分组而变差。

2. **Q: 能否用贪心替代 DP？**
   A: 不能。贪心无法确定最优的切分位置，因为一次切分会影响后续所有段的平均值。

3. **Q: 时间复杂度能否优于 O(n^2 * k)？**
   A: 在一般情况下很难。可以用分治优化（SMAWK 或 divide-and-conquer optimization）将某些特殊情况优化到 O(nk log n)，但实现复杂且适用范围有限。

4. **Q: 如果要求恰好分成 k 组呢？**
   A: 实际上由于"更多分组不会更差"的性质，恰好 k 组和最多 k 组的答案相同。

## 相关题型

- [LeetCode 410. Split Array Largest Sum](https://leetcode.com/problems/split-array-largest-sum/) - 分段最小化最大子数组和
- [LeetCode 1043. Partition Array for Maximum Sum](https://leetcode.com/problems/partition-array-for-maximum-sum/) - 分段DP，每段替换为最大值
- [LeetCode 1278. Palindrome Partitioning III](https://leetcode.com/problems/palindrome-partitioning-iii/) - 分段DP，最小修改代价
- [LeetCode 1335. Minimum Difficulty of a Job Schedule](https://leetcode.com/problems/minimum-difficulty-of-a-job-schedule/) - 分段DP经典题
- [LeetCode 312. Burst Balloons](https://leetcode.com/problems/burst-balloons/) - 区间DP
