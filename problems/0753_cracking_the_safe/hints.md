# 753. Cracking the Safe - 破解保险箱

## 核心思路

本题的本质是**求 De Bruijn 序列**：找一个最短的字符串，使得所有长度为 n、字符集为 [0, k-1] 的排列都作为子串出现。这等价于在有向图上找**欧拉回路**。图的节点是所有长度为 n-1 的串，边是所有长度为 n 的串（从前 n-1 个字符指向后 n-1 个字符），欧拉回路经过每条边恰好一次，对应恰好覆盖每个密码一次。

## 思维链

1. 所有可能的密码共 k^n 个（n 位，每位 k 种选择）
2. 最短序列长度为 `k^n + n - 1`（共享前后缀，类似滑动窗口）
3. 建图：节点 = 所有 (n-1) 位串，共 k^(n-1) 个节点
4. 边：从节点 u = s[0..n-2] 指向 v = s[1..n-1]，其中 s 是一个 n 位密码
5. 每个节点恰好有 k 条出边和 k 条入边 -> 欧拉回路一定存在
6. 使用 Hierholzer 算法（DFS + 后序插入）找欧拉回路
7. 将回路上的边对应的字符拼接即为答案

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 说明 |
|------|-----------|-----------|------|
| DFS 欧拉回路 (Hierholzer) | O(k^n) | O(k^n) | 经典做法，DFS后序 |
| 贪心 DFS + HashSet | O(k^n) | O(k^n) | 实现更直观 |

## 关键提示

1. **De Bruijn 序列**：在组合数学中，De Bruijn 序列 B(k,n) 是 k 元字符集上所有 n 长度子串恰好出现一次的循环序列
2. **图建模**：节点为 (n-1)-gram，边为 n-gram，这样每条边对应一个密码
3. **Hierholzer 算法**：DFS 时先递归所有未访问的邻边，回溯时将字符加入结果（后序），最后反转
4. **n=1 特殊情况**：直接拼接 0 到 k-1 即可
5. **起始节点**：可以从任意节点开始（全 0 串是常见选择）
6. **欧拉回路存在性**：因为每个节点的入度 = 出度 = k，所以一定存在

## 解法详解

### 解法一：DFS + HashSet（直观版）

直接以密码串为节点，DFS 尝试在当前串后追加字符 0~k-1，如果形成的新密码未被访问过，就标记并继续 DFS。

**核心逻辑**：
- 从 `n` 个 0 开始（如 "00"）
- 取当前串的后 n-1 位，尝试追加 0~k-1
- 如果新形成的 n 位串未被访问，标记访问并 DFS
- 最终串的长度恰好为 `k^n + n - 1`

### 解法二：Hierholzer 欧拉回路（标准版）

在 (n-1)-gram 图上跑 Hierholzer 算法：
- 每个节点维护下一个要尝试的出边编号
- DFS 从 "00...0"（n-1 个 0）开始
- 后序地将边上的字符追加到结果
- 最后反转结果，并加上起始节点

**为什么用后序**：Hierholzer 算法的关键——先递归完子图，再记录当前边，保证不会遗漏被「桥」隔开的子图。

## 易错点

1. **n=1 特殊情况**：此时节点为空串，边就是单个字符，直接拼接 0~k-1
2. **字符串截取**：取后 n-1 位时注意 substr 的参数
3. **DFS 方法的终止条件**：已访问的密码数等于 k^n 时停止
4. **Hierholzer 中的追加顺序**：必须在 DFS 回溯时追加（后序），不能前序
5. **结果反转**：Hierholzer 后序得到的是反向的，需要 reverse
6. **数值节点 vs 字符串节点**：可以将 (n-1)-gram 编码为 k 进制整数加速

## 面试追问

1. **Q: 为什么最短长度是 k^n + n - 1？**
   A: 共 k^n 个密码，每个长度为 n。最优情况下相邻密码共享 n-1 位，所以总长 = n + (k^n - 1) * 1 = k^n + n - 1。

2. **Q: Hierholzer 和普通 DFS 有什么区别？**
   A: 普通 DFS 可能在走完一条路径后无法覆盖所有边（类似欧拉路径的死胡同问题）。Hierholzer 通过后序插入确保所有分支都被探索。

3. **Q: 能否用 BFS 解决？**
   A: 理论上可以用 BFS 找欧拉路径，但实现复杂度高，不如 DFS + Hierholzer。

4. **Q: De Bruijn 序列有哪些实际应用？**
   A: DNA 测序中的 k-mer 组装、密码破解、轮盘编码、伪随机数生成等。

5. **Q: 如果 k 和 n 很大，如何优化内存？**
   A: 用整数编码节点代替字符串，用 k 进制数表示 (n-1)-gram，用位数组记录访问状态。

## 相关题型

- [332. Reconstruct Itinerary](https://leetcode.com/problems/reconstruct-itinerary/) - 重新安排行程（欧拉路径）
- [2097. Valid Arrangement of Pairs](https://leetcode.com/problems/valid-arrangement-of-pairs/) - 有效序列排列（欧拉路径）
- [17. Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number/) - 排列组合
- [79. Word Search](https://leetcode.com/problems/word-search/) - DFS 搜索
- [126. Word Ladder II](https://leetcode.com/problems/word-ladder-ii/) - 图搜索
