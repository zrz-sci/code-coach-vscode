# 745. 前缀和后缀搜索

## 核心思路

设计一种数据结构，支持同时按**前缀**和**后缀**查询单词。核心挑战在于如何将「前缀匹配 + 后缀匹配」这两个独立的约束合并到一个高效的查询结构中。

经典方法是将 `suffix + '#' + word` 的所有后缀变体插入到一棵 Trie 中。查询时搜索 `suff + '#' + pref`，这样一棵 Trie 就能同时处理前缀和后缀。另一种思路是用 HashMap 将所有 `(prefix, suffix)` 组合预计算存储。

## 思维链

1. **朴素思路**：对每次查询，遍历所有单词检查前缀和后缀 => O(n * L) 每次查询，太慢
2. **预计算所有组合**：每个单词长度最多 7，所以前缀最多 7 个，后缀最多 7 个，组合最多 49 个 => HashMap 存 (pref, suff) -> maxIndex
3. **Trie 融合方案**：将 word 的每个后缀变体 `suffix[i:] + '#' + word` 全部插入 Trie。查询 `suff + '#' + pref` 就能在一棵 Trie 中完成
4. **复杂度权衡**：HashMap 方案构造 O(n * L^2)，查询 O(L)；Trie 方案构造 O(n * L^2)，查询 O(L)，但 Trie 方案更节省空间（共享前缀）

## 解法概览

| 解法 | 构造复杂度 | 查询复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|-----------|------|
| HashMap 枚举所有组合 | O(n * L^2) | O(L) | O(n * L^2) | 实现简单，查询极快 |
| 后缀包装 Trie | O(n * L^2) | O(L) | O(n * L^2) | 经典 Trie 技巧，共享前缀节省空间 |
| 双 Trie + 集合交集 | O(n * L) | O(n) 最坏 | O(n * L) | 两棵 Trie 分别处理前缀/后缀 |

## 关键提示

1. **单词长度极短**：`words[i].length <= 7`，这是本题的关键约束，使得枚举所有前缀后缀组合成为可行方案
2. **返回最大下标**：有多个单词满足条件时，返回下标最大的那个。构造时从前往后遍历，后面的覆盖前面的即可
3. **分隔符选择**：Trie 方案中 `'#'` 作为分隔符，必须选一个不在 `'a'-'z'` 范围内的字符，确保不和正常字符冲突
4. **后缀变体的含义**：对 word = "apple"，生成 "apple#apple", "pple#apple", "ple#apple", "le#apple", "e#apple" 五个字符串插入 Trie

## 解法详解

### 解法一：HashMap 枚举所有前缀后缀组合（推荐）

**思路**：对每个 word[i]，枚举它的所有前缀和后缀组合 (pref, suff)，存入 HashMap。由于单词长度 <= 7，每个单词最多生成 7*7 = 49 个组合。

**构造过程**：
1. 遍历每个 word[i]
2. 枚举所有前缀 p (长度 1 到 L) 和后缀 s (长度 1 到 L)
3. 将 `(p, s)` 映射到当前下标 i
4. 后遍历的自然覆盖先遍历的，保证返回最大下标

**查询过程**：
1. 在 HashMap 中查找 `(pref, suff)`
2. 找到返回对应下标，找不到返回 -1

### 解法二：后缀包装 Trie

**思路**：将每个 word 的后缀变体 `word[j:] + '#' + word` 全部插入 Trie。查询 `suff + '#' + pref` 即可。

**构造过程**（对 word = "apple", index = 0）：
1. 生成并插入 "apple#apple"（完整后缀）
2. 生成并插入 "pple#apple"
3. 生成并插入 "ple#apple"
4. 生成并插入 "le#apple"
5. 生成并插入 "e#apple"
6. 每个 Trie 节点记录经过的最大 word index

**查询过程**：
1. 构造查询字符串 `suff + '#' + pref`
2. 沿 Trie 向下走
3. 到达末端节点时，返回该节点记录的最大 index

### 解法三：双 Trie + 集合交集

**思路**：建两棵 Trie，一棵正向存所有前缀及其 word indices，一棵反向存所有后缀及其 word indices。查询时取交集找最大值。

**查询过程**：
1. 在前缀 Trie 中找到所有匹配 pref 的 word indices
2. 在后缀 Trie 中找到所有匹配 suff 的 word indices
3. 取交集，返回最大值

**缺点**：交集操作最坏 O(n)，不如前两种方法。

## 易错点

1. **忘记返回最大下标**：题目要求多个匹配时返回最大下标，构造 HashMap/Trie 时必须用后面的索引覆盖前面的
2. **分隔符冲突**：Trie 方案中分隔符必须是 `'a'-'z'` 之外的字符（如 `'#'` 或 `'{'`），否则会和正常字符混淆
3. **空前缀/后缀**：虽然题目约束 `pref.length, suff.length >= 1`，但构造时也要考虑包含空串的情况以防万一
4. **HashMap 的 key 构造**：用 `pref + "#" + suff` 作为 key 时，要确保分隔符不会出现在单词中
5. **重复单词**：words 中可能有重复单词，后面出现的（下标更大）应覆盖前面的

## 面试追问

1. **Q: 如果单词长度不限（比如可达 1000），哪种方法更好？**
   A: HashMap 枚举所有组合不再可行（组合数爆炸）。应使用双 Trie 方案或 suffix + prefix Trie 的混合方案。

2. **Q: 如何优化 Trie 方案的空间？**
   A: 使用压缩 Trie（Patricia Trie），将只有一个孩子的链合并为一条边。但实现复杂度大幅增加。

3. **Q: 查询频率远高于构造，如何优化？**
   A: HashMap 方案最优，构造时间换查询 O(1)。

4. **Q: 如何支持通配符查询？**
   A: 需要在 Trie 的每一层做分支搜索，类似 LC 211 Design Add and Search Words。

5. **Q: 这道题和 LC 211 (Design Add and Search Words) 有什么异同？**
   A: 211 只有前缀/通配符匹配，本题额外要求后缀匹配。后缀包装 Trie 的技巧将后缀约束转化为前缀约束。

## 相关题型

- [208. Implement Trie (Prefix Tree)](../0208_implement_trie_prefix_tree/) - Trie 基础实现
- [211. Design Add and Search Words Data Structure](../0211_design_add_and_search_words_data_structure/) - Trie + 通配符搜索
- [212. Word Search II](../0212_word_search_ii/) - Trie 在矩阵搜索中的应用
- [336. Palindrome Pairs](../0336_palindrome_pairs/) - Trie + 字符串匹配
- [1032. Stream of Characters](../1032_stream_of_characters/) - 反向 Trie 流式匹配
