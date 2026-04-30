# 754. Reach a Number - 到达终点数字

## 核心思路

利用对称性取 `target = |target|`，然后贪心地累加 `1+2+3+...+steps` 直到总和 sum >= target。如果 `sum == target`，直接返回 steps。如果 `sum > target`，差值 `diff = sum - target`：若 diff 为偶数，则将第 `diff/2` 步的方向从 +1 翻转为 -1 即可（等价于减去 `2*(diff/2) = diff`），答案就是 steps；若 diff 为奇数，则再走一步或两步使 diff 变为偶数。

## 思维链

1. 由对称性，`target` 和 `-target` 答案相同，取绝对值处理
2. 每步 i 可以选择 +i 或 -i，等价于选择一个子集 S 使得：`sum(所有i) - 2*sum(S中的i) = target`
3. 即 `sum - target = 2 * sum(S)` 必须为非负偶数
4. 先累加 `sum = 1+2+...+steps` 直到 `sum >= target`
5. 此时 `diff = sum - target`
6. 若 diff 为偶数 -> 将第 diff/2 步翻转，答案为 steps
7. 若 diff 为奇数 -> 再加一步：
   - 如果 steps+1 为奇数，新的 diff = diff + (steps+1) 为偶数 -> 答案为 steps+1
   - 如果 steps+1 为偶数，新的 diff 仍为奇数，再加一步 steps+2 必为奇数 -> 答案为 steps+2
8. 综合：最多额外走 2 步

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 说明 |
|------|-----------|-----------|------|
| 数学贪心 | O(sqrt(target)) | O(1) | 累加到 sum >= target 后调整奇偶性 |
| 二分搜索 | O(log(target)) | O(1) | 用公式 n*(n+1)/2 >= target 二分找 n |

## 关键提示

1. **对称性**：问题关于原点对称，取绝对值不影响答案
2. **翻转等价**：将某步从 +i 改为 -i，总和减少 2i，所以 diff 必须是偶数才能通过翻转一步来精确到达
3. **贪心累加**：先走到 sum >= target，再微调
4. **奇偶性调整**：diff 为奇数时，再走奇数步即可（使 diff 变偶数）
5. **公式加速**：`1+2+...+n = n*(n+1)/2`，可用二分或求根公式直接算出 n

## 解法详解

### 解法一：数学贪心（逐步累加）

**算法步骤**：
1. `target = abs(target)`
2. `sum = 0, steps = 0`
3. `while sum < target: steps++; sum += steps`
4. `diff = sum - target`
5. `if diff % 2 == 0: return steps`
6. 否则继续走直到 diff 为偶数（最多再走 2 步）

**直觉理解**：所有步都取正号得到最大 sum。如果 sum 刚好等于 target，完美。如果多了 diff，只要 diff 是偶数，就可以把第 diff/2 步翻转（从 +diff/2 变成 -diff/2），总共少了 diff，恰好到达 target。

### 解法二：二分搜索

利用 `n*(n+1)/2 >= target` 直接二分找到最小的 n，然后检查奇偶性。比逐步累加快一个常数因子。

### 解法三：数学公式直接求解

使用求根公式 `n = ceil((-1 + sqrt(1+8*target)) / 2)` 直接找到最小 n，O(1) 求解后微调奇偶性。

## 易错点

1. **忘记取绝对值**：target 可以为负数，必须先取绝对值
2. **差值的奇偶判断**：C++ 中负数取模结果可能为负，建议用 `& 1` 判断奇偶更安全
3. **翻转步骤的存在性**：diff/2 不需要恰好是某个步骤编号——任意子集和等于 diff/2 即可（由于 1+2+...+steps 可以表示 1 到 sum 内的任意整数，所以一定存在）
4. **int 溢出**：target 最大 10^9，sum 最大约 10^9 级别，注意使用 long
5. **steps+1 或 steps+2**：最多额外走 2 步，不能直接假设只走 1 步

## 面试追问

1. **Q: 为什么 diff 为偶数就一定可以翻转某些步使总和恰好减少 diff？**
   A: diff/2 是一个不超过 sum/2 的正整数。由于 sum = 1+2+...+steps，任意不超过 sum 的正整数都可以表示为这些数的子集和（这是一个已知结论，类似于二进制表示）。所以一定能找到一个子集使其和等于 diff/2。

2. **Q: 为什么最多额外走 2 步？**
   A: diff 为奇数时需要加一个奇数使其变偶数。steps+1 和 steps+2 中必有一个是奇数。

3. **Q: 如何从 O(sqrt(n)) 优化到 O(log(n))？**
   A: 用二分搜索或直接用求根公式 `n = ceil((-1 + sqrt(1 + 8*target)) / 2)` 找到最小 n。

4. **Q: 如果每步可以走 1~k 步（不是固定 i 步），问题怎么变？**
   A: 变成了一个更复杂的可达性问题，可能需要 BFS 或 DP。

5. **Q: 能否输出具体的路径而非步数？**
   A: 可以。找到 steps 后，通过贪心从大到小确定哪些步取负号（使翻转集合的和 = diff/2）。

## 相关题型

- [780. Reaching Points](https://leetcode.com/problems/reaching-points/) - 到达终点
- [70. Climbing Stairs](https://leetcode.com/problems/climbing-stairs/) - 爬楼梯（步数决策）
- [1523. Count Odd Numbers in an Interval Range](https://leetcode.com/problems/count-odd-numbers-in-an-interval-range/) - 奇偶性分析
- [279. Perfect Squares](https://leetcode.com/problems/perfect-squares/) - 数学最优分解
- [1025. Divisor Game](https://leetcode.com/problems/divisor-game/) - 数学博弈
