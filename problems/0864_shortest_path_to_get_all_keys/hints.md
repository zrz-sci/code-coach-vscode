# 864. Shortest Path to Get All Keys

## 核心思路

本题是一道 BFS + 状态压缩 的经典题目。网格中有钥匙（小写字母）和锁（大写字母），需要找到从起点出发收集所有钥匙的最短路径。关键洞察在于：普通 BFS 只用 `(row, col)` 来判重，但本题中同一个位置、携带不同钥匙集合时是完全不同的状态。因此需要将已拥有的钥匙集合编码进状态里。由于钥匙最多 6 把，可以用一个 6 位的二进制掩码（bitmask）来表示当前持有的钥匙集合，从而将状态空间定义为 `(row, col, keys_bitmask)`。

## 思维链

1. **读题抽象**：起点 `@`，收集所有钥匙，遇锁需对应钥匙，求最少步数 → 最短路径 → BFS。
2. **状态定义**：仅 `(row, col)` 不够 → 同一位置持有不同钥匙代表不同状态 → 三元组 `(row, col, keys)`。
3. **钥匙集合表示**：最多 6 把钥匙 → 用 6 位 bitmask → `keys` 取值 0~63。
4. **终止条件**：`keys == (1 << k) - 1`，即所有钥匙都收集齐。
5. **转移规则**：四方向移动，遇墙跳过，遇锁检查是否有对应钥匙，遇钥匙更新 bitmask。
6. **去重**：三维 visited 数组 `visited[row][col][keys]`。
7. **复杂度验证**：状态数 `30 * 30 * 64 = 57600`，完全可接受。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| BFS + 状态压缩 | O(m * n * 2^k) | O(m * n * 2^k) | 最优解，利用 bitmask 编码钥匙状态 |
| Dijkstra + 状态压缩 | O(m * n * 2^k * log) | O(m * n * 2^k) | 若边权不同时使用，本题边权均为1故BFS即可 |

## 关键提示

1. **Bitmask 编码钥匙**：钥匙 `'a'` 对应 bit 0，`'b'` 对应 bit 1，以此类推。拾取钥匙：`keys | (1 << (ch - 'a'))`。
2. **检查锁**：遇到大写字母 `'A'` 时，检查 `keys & (1 << (ch - 'A'))` 是否非零。
3. **统计钥匙总数**：遍历网格，记录出现的小写字母数量 `k`，目标 bitmask 为 `(1 << k) - 1`。
4. **BFS 队列元素**：`(row, col, keys, steps)` 或将 steps 作为 BFS 层数。
5. **visited 数组**：三维布尔数组 `m x n x (1 << k)`，或用 set 存储 `(row, col, keys)` 元组。

## 解法详解

### BFS + Bitmask 状态压缩

**算法步骤**：

1. 遍历网格，找到起点 `@` 的坐标和所有钥匙的数量 `k`。
2. 初始化 BFS 队列，将起点状态 `(startRow, startCol, 0)` 入队（0 表示未持有任何钥匙）。
3. 初始化三维 visited 数组，标记起点初始状态已访问。
4. BFS 按层遍历（每层代表一步）：
   - 取出当前状态 `(r, c, keys)`。
   - 若 `keys == (1 << k) - 1`，返回当前步数。
   - 向四个方向扩展：
     - 越界或墙壁 → 跳过。
     - 锁且无对应钥匙 → 跳过。
     - 钥匙 → 更新 `newKeys = keys | (1 << (grid[nr][nc] - 'a'))`。
     - 其他 → `newKeys = keys`。
     - 若 `(nr, nc, newKeys)` 未访问 → 标记访问并入队。
5. 队列耗尽仍未收集全部钥匙 → 返回 -1。

**为什么 BFS 保证最短**：所有移动代价相同（每步代价为 1），BFS 天然按步数分层，第一次到达目标状态即为最短路径。

**状态空间分析**：网格最大 30x30，钥匙最多 6 把，总状态数 `30 * 30 * 2^6 = 57600`，非常高效。

## 易错点

1. **忘记将钥匙状态纳入去重**：若只用 `(row, col)` 去重，同一位置但持有不同钥匙的路径会被错误剪枝，导致答案错误。
2. **bitmask 操作写错**：`1 << (ch - 'a')` 与 `1 << (ch - 'A')` 要区分大小写场景。
3. **起点可能就在钥匙上**：起始位置可能恰好是某把钥匙的位置（虽然题目用 `@` 标记起点，但要确认）。
4. **visited 在入队时标记 vs 出队时标记**：BFS 中应在入队时标记 visited，否则同一状态会被重复入队导致 TLE。
5. **终止条件检查时机**：应在出队时（或入队时）立即检查是否收集完所有钥匙，不要等到下一轮。

## 面试追问

1. **如果钥匙数量可达 26 个怎么办？** bitmask 需要 26 位，状态空间爆炸为 `30 * 30 * 2^26`，不可接受。需要考虑剪枝、A* 搜索或其他策略。
2. **如果网格中有传送门（两个相同字母可瞬移）？** 在 BFS 扩展时加入传送门目标位置即可。
3. **如果移动代价不同（如地形不同步数不同）？** 需要将 BFS 替换为 Dijkstra。
4. **如何输出最短路径本身而非步数？** 在 BFS 中记录每个状态的前驱状态，到达目标后回溯。
5. **能否用 DFS + 记忆化？** 理论上可以，但在最短路径问题中 BFS 更直观且保证最优。

## 相关题型

- [787. Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) — BFS/Dijkstra + 状态约束
- [847. Shortest Path Visiting All Nodes](https://leetcode.com/problems/shortest-path-visiting-all-nodes/) — BFS + bitmask 状态压缩（非常相似）
- [1293. Shortest Path in a Grid with Obstacles Elimination](https://leetcode.com/problems/shortest-path-in-a-grid-with-obstacles-elimination/) — BFS + 额外状态维度
- [752. Open the Lock](https://leetcode.com/problems/open-the-lock/) — BFS 状态搜索
- [127. Word Ladder](https://leetcode.com/problems/word-ladder/) — 经典 BFS 最短路径
