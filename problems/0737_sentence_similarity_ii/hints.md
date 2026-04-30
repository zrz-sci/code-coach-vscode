# 737. Sentence Similarity II - 句子相似性 II

## 难度: Medium

## 标签: DFS, BFS, Union Find, Array, Hash Table, String

## 核心思路

与第 734 题（Sentence Similarity I）的关键区别在于**相似关系具有传递性**：若 a ~ b 且 b ~ c，则 a ~ c。这意味着需要判断两个单词是否属于同一个**连通分量**。最优解法是使用 **Union-Find（并查集）**，将所有相似对合并到同一集合中，然后逐位检查两个句子对应位置的单词是否在同一集合。也可以用 DFS/BFS 建图判断可达性。

## 思维链

1. **长度检查**: 两个句子长度不同则直接返回 false
2. **识别传递性**: 相似关系是传递的 -> 这是连通性问题，不是简单的直接匹配
3. **建立连通关系**: 将 similarPairs 中的每对单词合并到同一集合
4. **逐位比较**: 对每个位置 i，检查 sentence1[i] 和 sentence2[i] 是否在同一集合
5. **自身相似**: 一个单词总是与自身相似，即使它不出现在任何 similarPair 中
6. **短路优化**: 如果 sentence1[i] == sentence2[i]，直接跳过，无需查集合

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Union-Find（推荐） | O(n + p * alpha(p)) | O(p) | 建完后查询近 O(1)，最优 |
| DFS/BFS 建图 | O(n * p) | O(p) | 每次查询需遍历图，较慢 |

其中 n 为句子长度，p 为相似对数量，alpha 为反阿克曼函数（近似常数）。

## 关键提示

1. **字符串并查集**: 由于键是字符串而非整数，使用 `unordered_map<string, string>` 作为 parent 映射
2. **路径压缩**: find 操作中 `parent[x] = find(parent[x])` 实现路径压缩，加速后续查询
3. **初始化时机**: 不需要预先初始化所有单词的 parent，在 find 时如果不存在则初始化为自身
4. **直接相等优先判断**: 比较前先检查 `sentence1[i] == sentence2[i]`，避免不必要的 find 调用
5. **单词可能不在 pairs 中**: 如果某个单词未出现在任何 similarPair 中，它的 parent 就是自身

## 解法详解

### 解法一：Union-Find（推荐）

**步骤**:
1. 遍历 similarPairs，对每对 `(a, b)` 执行 `unite(a, b)` 合并到同一集合
2. 遍历句子的每个位置 i：
   - 若 `sentence1[i] == sentence2[i]`，跳过
   - 否则调用 `find(sentence1[i])` 和 `find(sentence2[i])`，若根不同则返回 false
3. 全部通过则返回 true

**find 函数**:
```
find(x):
    if x not in parent: parent[x] = x
    if parent[x] != x: parent[x] = find(parent[x])  // 路径压缩
    return parent[x]
```

**unite 函数**:
```
unite(a, b):
    pa = find(a), pb = find(b)
    if pa != pb: parent[pa] = pb
```

**时间复杂度**: O(n + p * alpha(p))，alpha 为反阿克曼函数

**空间复杂度**: O(p) 存储 parent 映射

### 解法二：DFS/BFS 建图

**步骤**:
1. 用邻接表建图：对每个 similarPair `(a, b)`，添加双向边 `a <-> b`
2. 对每个位置 i，从 sentence1[i] 出发 DFS/BFS，检查能否到达 sentence2[i]
3. 每次查询的开销为 O(p)

**时间复杂度**: O(n * p) - 每个位置都可能遍历整个图

**空间复杂度**: O(p) 邻接表

## 易错点

1. **忘记长度检查**: 两个句子长度不同必须直接返回 false，这是最容易遗漏的边界条件
2. **字符串比较 vs 集合查询**: 不能只检查两个单词是否直接在 similarPairs 中出现，必须考虑传递性
3. **大小写敏感**: 题目约束中单词包含大小写字母，比较时区分大小写
4. **find 的延迟初始化**: 如果单词不在 parent 中就直接返回自身，否则会产生空键
5. **与第 734 题混淆**: 734 题相似关系不传递，只需 HashSet 直接查；737 题必须用连通性判断
6. **空 similarPairs**: 如果 similarPairs 为空，只有完全相同的句子才相似

## 面试追问

1. **Union-Find 为什么比 DFS 更优?** UF 建立后每次查询近 O(1)（均摊），DFS 每次查询 O(p)。当句子很长（n 大）时差距更明显
2. **如何支持按秩合并 (union by rank)?** 额外维护 `rank[x]` 记录树高，合并时将矮树接到高树下，进一步降低树高
3. **如果相似关系是双向的但不传递（即 Similarity I），该怎么做?** 直接用 HashSet<pair> 存储所有相似对，O(1) 查询即可
4. **如何动态增加新的相似对并重新判断?** Union-Find 天然支持动态 union 操作，无需重建
5. **如果单词有百万级别，如何优化内存?** 对字符串做编号映射（string -> int），用整数并查集替代字符串映射

## 相关题型

- [734. Sentence Similarity](https://leetcode.com/problems/sentence-similarity/) - 无传递性的简单版本
- [547. Number of Provinces](https://leetcode.com/problems/number-of-provinces/) - 标准 Union-Find 求连通分量
- [200. Number of Islands](https://leetcode.com/problems/number-of-islands/) - BFS/DFS/Union-Find 求连通性
- [323. Number of Connected Components in an Undirected Graph](https://leetcode.com/problems/number-of-connected-components-in-an-undirected-graph/) - Union-Find 求连通分量数
- [721. Accounts Merge](https://leetcode.com/problems/accounts-merge/) - 字符串并查集的经典应用
- [990. Satisfiability of Equality Equations](https://leetcode.com/problems/satisfiability-of-equality-equations/) - 等式关系的传递性判断
