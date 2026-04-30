# 997. Find the Town Judge

[file://solution.cpp](./solution.cpp)

## 核心思路

图的入度/出度分析。将信任关系建模为有向图，法官是入度为 n-1 且出度为 0 的节点。可以用一个数组同时追踪每个人的"净信任度"（被信任次数 - 信任他人次数），净信任度为 n-1 的人就是法官。

## 思维链

1. **建模**：trust[i] = [a, b] 表示 a -> b 的有向边（a 信任 b）。
2. **法官特征**：
   - 法官不信任任何人 -> 出度 = 0
   - 所有其他人信任法官 -> 入度 = n - 1
3. **优化观察**：出度 = 0 且入度 = n-1 等价于 入度 - 出度 = n-1。
4. **实现**：一个数组 `trust_score[i]`，被信任时 +1，信任别人时 -1。
5. **最终检查**：找 trust_score == n-1 的人。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 净信任度数组 | O(E + n) | O(n) | 最简洁 |
| 分别统计出度入度 | O(E + n) | O(n) | 更直观 |
| HashSet 法 | O(E + n) | O(n) | 用 set 存信任他人的人 |

## 关键提示

1. **净信任度技巧**：将入度和出度合并为一个度量，`score[b]++, score[a]--`，最后找 score == n-1 的人。
2. **唯一性保证**：题目保证最多一个法官，所以找到 score == n-1 的即可返回。
3. **n=1 边界**：只有一个人时，他自己就是法官（没有信任关系）。
4. **人的编号从 1 开始**：数组大小为 n+1，索引 0 不用。

## 解法详解

### 解法一：净信任度数组（推荐）

**思路**：对每个人维护一个 score。被信任时 +1，信任别人时 -1。法官的 score 恰好为 n-1（被 n-1 个人信任，且不信任任何人）。

**步骤**：
1. 创建 `score[n+1]`，初始全 0。
2. 遍历 trust 数组，对 `[a, b]`：`score[a]--, score[b]++`。
3. 遍历 1 到 n，找 `score[i] == n-1` 的 i。
4. 找到返回 i，没找到返回 -1。

**复杂度**：时间 O(E + n)，空间 O(n)。

### 解法二：分别统计入度和出度

**思路**：分别用两个数组记录入度和出度。

**步骤**：
1. 创建 `inDegree[n+1]` 和 `outDegree[n+1]`。
2. 遍历 trust：`outDegree[a]++, inDegree[b]++`。
3. 找 `inDegree[i] == n-1 && outDegree[i] == 0` 的 i。

**复杂度**：时间 O(E + n)，空间 O(n)。

### 解法三：HashSet 排除法

**思路**：先用 HashSet 收集所有信任他人的人（这些人不可能是法官），再在剩余的人中找被所有人信任的。

**步骤**：
1. 收集所有 `trust[i][0]` 到 set（出度 > 0 的人）。
2. 遍历 1 到 n，不在 set 中的人是候选法官。
3. 验证候选法官的入度是否为 n-1。

**复杂度**：时间 O(E + n)，空间 O(n)。

## 易错点

1. **编号从 1 开始**：数组下标注意 1-indexed，分配 n+1 大小。
2. **n=1 时 trust 为空**：只有一个人且没有信任关系，他就是法官，score[1] = 0 = n-1 = 0。
3. **不存在法官**：可能所有人都信任别人，没有 score == n-1 的人，返回 -1。
4. **多人 score 高但达不到 n-1**：只有恰好等于 n-1 才是法官。
5. **误用无向图**：信任关系是单向的，a 信任 b 不代表 b 信任 a。

## 面试追问

1. **Q: 如果要找"副法官"（被 n-2 人信任且只信任法官）怎么做？**
   A: 先找法官，然后找 inDegree == n-2 且 outDegree == 1（只信任法官）的人。

2. **Q: 如果信任关系可以动态增删，如何高效维护？**
   A: 维护 score 数组，每次增删时更新相应 score，O(1) 更新，O(n) 查询。可用额外变量追踪当前 score == n-1 的人。

3. **Q: 这道题的图论本质是什么？**
   A: 法官是图中的"通用汇点"（universal sink），入度 n-1，出度 0。经典问题是用 O(n) 时间找到邻接矩阵中的通用汇点。

4. **Q: 如何用 O(n) 时间在邻接矩阵中找通用汇点？**
   A: 双指针法：从候选 (1, 2) 开始，若 1 信任 2 则排除 1，否则排除 2。最后验证剩下的候选。

## 相关题型

- [277. Find the Celebrity](https://leetcode.com/problems/find-the-celebrity/) - 经典通用汇点问题
- [207. Course Schedule](https://leetcode.com/problems/course-schedule/) - 有向图入度/出度
- [210. Course Schedule II](https://leetcode.com/problems/course-schedule-ii/) - 拓扑排序
- [1557. Minimum Number of Vertices to Reach All Nodes](https://leetcode.com/problems/minimum-number-of-vertices-to-reach-all-nodes/) - 入度为 0 的节点
- [802. Find Eventual Safe States](https://leetcode.com/problems/find-eventual-safe-states/) - 图的出度分析
