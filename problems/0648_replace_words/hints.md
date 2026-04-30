# 648. 单词替换 (Replace Words)

**难度**: Medium | **标签**: Trie, Array, Hash Table, String

## 核心思路

本题要求用词根（root）替换句子中的衍生词（derivative）。对于每个单词，需要找到匹配的**最短词根**。这是一个经典的**前缀匹配**问题，最适合用 **Trie（前缀树）** 来解决。

将所有词根插入 Trie，然后对句子中的每个单词，在 Trie 中查找最短匹配前缀即可。如果 Trie 中存在以某个前缀结尾的词根，就用该词根替换整个单词。

## 思维链

1. **识别问题类型**：多个模式串的前缀匹配 -> Trie 是首选数据结构
2. **构建 Trie**：将 dictionary 中所有词根插入 Trie
3. **逐词处理**：将 sentence 按空格分割为单词列表
4. **查找最短前缀**：对每个单词，在 Trie 中逐字符查找，遇到第一个 isEnd=true 的节点就返回该前缀
5. **替换或保留**：如果找到词根前缀，替换；否则保留原词
6. **拼接结果**：将处理后的单词用空格拼接

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Trie | O(D + S) | O(D) | 最优，标准解法 |
| HashSet 暴力 | O(S * W * W) | O(D) | 简单但较慢 |
| 排序 + 逐词检查 | O(D log D + S * W) | O(D) | 不用 Trie 的替代方案 |

其中 D = 所有词根的总字符数，S = sentence 的总字符数，W = 单个单词最大长度

## 关键提示

1. **Trie 节点设计**：每个节点包含 children[26]（或 unordered_map）和 isEnd 标记
2. **最短词根优先**：在 Trie 中查找时，遇到第一个 isEnd 就停止，自然保证了最短前缀
3. **字符串分割技巧**：C++ 中可以用 istringstream 或手动按空格分割
4. **HashSet 替代方案**：将所有词根放入 set，对每个单词逐渐截取前缀查找，找到就替换
5. **结果拼接优化**：用字符串流或预分配 string，避免频繁的字符串拼接

## 解法详解

### 解法一：Trie（最优）

**第一步：构建 Trie**
- 定义 TrieNode：children 数组（26个指针）+ isEnd 布尔标记
- 遍历 dictionary，将每个词根逐字符插入 Trie
- 插入完成后标记末尾节点的 isEnd = true

**第二步：分割句子**
- 用 istringstream 或手动遍历将 sentence 按空格分割成单词列表

**第三步：逐词查找最短前缀**
- 对每个单词，从 Trie 根节点开始，逐字符向下走
- 如果当前节点 isEnd == true，说明找到了一个词根前缀，返回该前缀
- 如果字符不在 children 中，说明无匹配词根，返回原词
- 如果遍历完单词都没有遇到 isEnd，返回原词

**第四步：拼接**
- 将结果单词用空格拼接，返回最终字符串

### 解法二：HashSet

**思路**：将所有词根放入 unordered_set，对每个单词，从长度 1 开始截取前缀，检查是否在 set 中。

**步骤**：
1. 将 dictionary 所有词根插入 unordered_set
2. 对每个单词 w，遍历 len = 1 到 w.size()
3. 如果 w.substr(0, len) 在 set 中，替换为该前缀
4. 否则保留原词

## 易错点

1. **Trie 内存管理**：C++ 中手动 new TrieNode 需要注意内存释放（面试中一般不要求）
2. **空格处理**：sentence 首尾无空格，单词间恰好一个空格，但处理时需要确保拼接正确
3. **词根本身也是一个合法单词**：如果单词恰好等于某个词根，应该用该词根替换（实际上保持不变）
4. **多个词根匹配同一单词**：题目要求用最短的词根。Trie 自然保证最短前缀优先；HashSet 法需要从短到长遍历
5. **重复词根**：dictionary 可能有重复词根，Trie 和 HashSet 都天然去重
6. **字符串拼接性能**：大量 string + string 操作在 C++ 中可能导致性能问题，建议用 ostringstream 或 reserve

## 面试追问

1. **Q: 为什么 Trie 比 HashSet 更适合这道题？**
   A: Trie 在查找最短前缀时只需遍历一次单词（O(W)），而 HashSet 需要尝试所有可能的前缀长度并计算哈希值。当词根数量大且单词长度长时，Trie 更高效。此外 Trie 的前缀查找是 O(L)（L 为词根长度），而 HashSet 的 substr + 查找是 O(L) per attempt。

2. **Q: Trie 的空间复杂度如何？**
   A: 最坏情况下 O(26^L * L) 其中 L 是最大词根长度，但实际上由于公共前缀共享，通常远小于此。本题词根总长度 <= 1000*100 = 10^5，空间完全够用。

3. **Q: 如果 dictionary 非常大（百万级别），怎么优化？**
   A: 可以用压缩 Trie (Patricia/Radix Tree) 减少节点数量。也可以用 Aho-Corasick 算法做多模式匹配，一次扫描完成所有替换。

4. **Q: 如何处理中文/Unicode 的情况？**
   A: children 不能用固定大小数组，改为 unordered_map<char, TrieNode*> 或 unordered_map<wchar_t, TrieNode*>。

5. **Q: 如果要替换为最长词根而不是最短词根？**
   A: Trie 中不在遇到第一个 isEnd 时停止，而是继续向下走，记录最后一个 isEnd 对应的前缀。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 208 | Implement Trie (Prefix Tree) | Trie 基础实现 |
| 211 | Design Add and Search Words Data Structure | Trie + 通配符搜索 |
| 212 | Word Search II | Trie + 回溯 |
| 14 | Longest Common Prefix | 前缀处理 |
| 720 | Longest Word in Dictionary | Trie + DFS |
| 1065 | Index Pairs of a String | 多模式匹配 |
