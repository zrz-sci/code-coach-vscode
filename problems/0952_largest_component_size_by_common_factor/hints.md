[file://solution.cpp](./solution.cpp)

# 952. Largest Component Size by Common Factor

## 核心思路

给定一组正整数，两个数之间有公因子（>1）就连边，求最大连通分量的大小。核心方法是**Union-Find（并查集） + 质因数分解**：对每个数进行质因数分解，将该数与它的所有质因数进行 union 操作。这样，共享相同质因数的数会被自动归入同一连通分量。最后统计每个连通分量中原始数字的个数，取最大值。关键优化是按质因数建边而非两两比较 GCD，将 O(n^2) 降低到 O(n * sqrt(max_val))。

## 思维链

1. **暴力思路**：两两计算 GCD，GCD > 1 就 union。O(n^2 * log(max_val))，太慢
2. **优化方向**：如果两个数共享一个质因数 p，它们一定要连通。可以把"质因数"作为中间桥梁
3. **核心idea**：对每个数 nums[i]，分解质因数得到 {p1, p2, ...}，将 nums[i] 与每个 pj union
4. **Union-Find 的 key 空间**：需要涵盖所有数字和所有可能的质因数（最大到 max(nums[i])）
5. **统计结果**：遍历 nums 数组，对每个 nums[i] 找到其根节点，用哈希表统计每个根节点的频次
6. **答案**：哈希表中最大的频次值

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Union-Find + 质因数分解 | O(n * sqrt(M)) | O(M) | 标准最优解，M = max(nums[i]) |
| 暴力 GCD + Union-Find | O(n^2 * logM) | O(n) | 简单但超时 |
| 埃拉托色尼筛 + Union-Find | O(M * loglogM + n) | O(M) | 预处理质数表 |

## 关键提示

1. **质因数是桥梁**：不需要两两比较，而是让质因数充当"中转节点"将共享因数的数连接起来
2. **Union-Find 的索引空间**：parent 数组大小要开到 max(nums[i]) + 1，因为质因数本身也是节点
3. **质因数分解的优化**：只需枚举到 sqrt(n)，如果分解完剩余部分 > 1，说明还有一个大质因数
4. **统计时只数原始数字**：最终统计只遍历 nums 数组，不统计"质因数节点"
5. **路径压缩和按秩合并**：对大数据集至关重要，否则可能超时

## 解法详解

### 解法一：Union-Find + 质因数分解（推荐）

**思路**：构建一个并查集，其中节点包括 nums 中的每个数以及所有涉及的质因数。对每个 nums[i]，分解质因数，将 nums[i] 与每个质因数 union。最后统计原始数字的根分布。

**步骤**：
1. 初始化 Union-Find，parent 数组大小为 max(nums) + 1
2. 对每个 nums[i]：
   - 分解质因数：试除法从2开始到 sqrt(nums[i])
   - 对每个质因数 p，执行 union(nums[i], p)
   - 如果除完后剩余 > 1，说明有大质因数，也要 union
3. 遍历 nums，对每个数找根节点，用 map 统计每个根的频次
4. 返回最大频次

**质因数分解细节**：
```
n = nums[i]
for p = 2; p * p <= n; p++:
    if n % p == 0:
        union(nums[i], p)
        while n % p == 0: n /= p
    if n > 1:
        union(nums[i], n)  // n 是一个大质因数
```

**复杂度**：
- 时间 O(n * sqrt(M))：n 个数各做 sqrt(M) 级别的因数分解。Union-Find 操作近似 O(1)。
- 空间 O(M)：parent 数组大小为 max(nums)。

### 解法二：埃氏筛预处理最小质因数

**思路**：用埃拉托色尼筛预处理每个数的最小质因数（SPF），分解时直接查表，避免重复试除。

**步骤**：
1. 用筛法建立 SPF[i] 表（1 到 max(nums)）
2. 对每个 nums[i]，用 SPF 快速分解质因数
3. 后续同解法一

**复杂度**：时间 O(M * loglogM + n * logM)，空间 O(M)。预处理更快但空间更大。

## 易错点

1. **parent 数组太小**：必须开到 max(nums)+1，不是 nums.size()，因为质因数也是节点
2. **忘记处理大质因数**：试除到 sqrt(n) 后，剩余的 n > 1 就是一个大质因数，必须 union
3. **统计时混入质因数节点**：最终计数只能遍历 nums 数组中的元素，不能遍历整个 parent 数组
4. **数字 1 的特殊处理**：1 没有质因数，它不与任何数连通（除非 nums 中只有 1）
5. **Union-Find 未做路径压缩**：数据量大（2*10^4 个数，每个最大 10^5），不优化会 TLE
6. **重复质因数**：分解时要 while 循环除尽同一质因数（如 12 = 2*2*3），union 只需对每个不同质因数做一次

## 面试追问

1. **Q: 为什么不直接两两计算 GCD？**
   A: n 最大 2*10^4，两两比较是 O(n^2) = 4*10^8 次 GCD，每次 GCD 是 O(logM)，总体超时。质因数分解法将每个数独立处理，总时间 O(n*sqrt(M))。

2. **Q: Union-Find 的 parent 数组为什么不用 hash map？**
   A: 可以用 hash map，但数组访问更快。由于 max(nums[i]) <= 10^5，数组完全可行。Hash map 在质因数稀疏时节省空间，但常数更大。

3. **Q: 如果数组中有重复元素怎么办？**
   A: 题目保证 unique，但如果有重复，同一个数多次出现应该计入同一连通分量的不同"节点"。需要用额外的映射区分同值不同节点。

4. **Q: 这道题和社交网络中的"共同好友分组"有什么联系？**
   A: 完全类似。"质因数"就像"兴趣标签"，共享标签的人（数字）归为同一组。Union-Find 是处理这类动态连通性问题的标准工具。

5. **Q: 如何优化到线性时间？**
   A: 使用线性筛（欧拉筛）预处理最小质因数表，分解时 O(logM)。总时间 O(M + n*logM)，空间 O(M)。

## 相关题型

- [LC 684. Redundant Connection](https://leetcode.com/problems/redundant-connection/) - Union-Find 基础应用
- [LC 947. Most Stones Removed](https://leetcode.com/problems/most-stones-removed-with-same-row-or-column/) - Union-Find 按属性连通
- [LC 1627. Graph Connectivity With Threshold](https://leetcode.com/problems/graph-connectivity-with-threshold/) - 因数+连通性
- [LC 2709. Greatest Common Divisor Traversal](https://leetcode.com/problems/greatest-common-divisor-traversal/) - 非常相似的质因数+UF
- [LC 204. Count Primes](https://leetcode.com/problems/count-primes/) - 埃氏筛基础
- [LC 128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) - 另一种连通分量思路
