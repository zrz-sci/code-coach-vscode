# 676. Implement Magic Dictionary - 实现一个魔法字典

## 核心思路

设计一个数据结构，支持 `buildDict` 建词典和 `search` 查询。search 要求输入字符串恰好修改一个字符后能匹配词典中的某个单词。核心难点在于"恰好一个"——完全相同(0个不同)不算，2个以上也不算。由于数据规模小(词典最多100个单词，长度<=100)，按长度分组后暴力逐字符比较即可。进阶可用 Trie 树实现。

## 思维链

1. 读题：`search("hhllo")` 应返回 true，因为改第二个字符 `h->e` 可得到 `"hello"`。
2. 关键约束："恰好一个字符不同" -> 长度必须相同 + diff count == 1。
3. 暴力思路：遍历字典中每个同长度单词，逐字符比较，统计不同数。
4. 优化：用 HashMap 按长度分组，search 时只比较同长度单词。
5. 时间分析：search 最坏 O(N * L)，N=100, L=100，即 10^4，完全够快。
6. 进阶：Trie + DFS，允许恰好一次"走不同分支"，适合词典很大的场景。
7. 另一种思路：通配符哈希——将每个单词的每个位置替换为 `*`，存入 set，search 时也逐位替换检查。需额外判断原词是否在字典中。

## 解法概览

| 解法 | buildDict | search | 空间 | 优劣 |
|------|----------|--------|------|------|
| 按长度分组 + 暴力比较 | O(N*L) | O(N*L) | O(N*L) | 实现最简单，面试首选 |
| 通配符哈希 | O(N*L^2) | O(L^2) | O(N*L^2) | 查询更快，但 build 和空间更大 |
| Trie + DFS | O(N*L) | O(26*L) | O(N*L*26) | 进阶方案，适合超大词典 |

## 关键提示

1. **长度不同直接跳过**：如果 searchWord 和字典单词长度不同，不可能恰好差一个字符，直接跳过。
2. **diff == 1 而非 diff >= 1**：必须恰好是 1，完全匹配(diff==0)要返回 false。
3. **提前剪枝**：diff 超过 1 时可以 break 跳出内层循环，小幅优化。
4. **通配符方法的陷阱**：`search("hello")` 词典中有 `"hello"`，通配符 `"*ello"` 会匹配，但实际 diff==0 不合法。需要额外记录每个通配符模式对应的原词集合，判断是否存在不同的原词。
5. **Trie 方法**：DFS 时维护一个 `changed` 标志，初始 false。在某一层选择一个不同字符分支时置为 true。到达终点时 `changed` 必须为 true。

## 解法详解

### 解法一：按长度分组 + 暴力比较（推荐）

**buildDict**：将所有单词按长度存入 `unordered_map<int, vector<string>>`。

**search**：
1. 取 `searchWord` 的长度 L
2. 在 map 中找 key=L 对应的单词列表
3. 逐个比较，统计字符差异数
4. 若存在 diff==1 的单词，返回 true

```
遍历同长度单词列表:
    diff = 0
    for i in 0..L-1:
        if word[i] != searchWord[i]: diff++
        if diff > 1: break
    if diff == 1: return true
return false
```

### 解法二：Trie + DFS

**buildDict**：将所有单词插入 Trie 树。

**search**：在 Trie 上做 DFS(node, idx, changed)：
- 如果 `idx == len`：返回 `changed && node.isEnd`
- 如果还没 changed：尝试当前层所有 26 个子节点
  - 与 searchWord[idx] 相同的分支：不改变 changed
  - 不同的分支：标记 changed = true
- 如果已经 changed：只能走与 searchWord[idx] 相同的分支

### 解法三：通配符哈希

**buildDict**：对每个单词，逐位替换为 `*`，将模式存入 `set<string>`。同时把原词存入另一个 `set<string>`。

**search**：逐位替换 searchWord 为 `*`，检查模式是否在 set 中。若模式存在，还需确认不是原词自身造成的匹配（通过检查原词集合中该模式对应的词是否 **仅有** searchWord 本身）。

## 易错点

1. **diff == 0 返回 true**：最常见错误。search("hello") 字典有 "hello" 时应返回 false，因为不需要修改任何字符。
2. **忘记长度检查**：不同长度的词不可能通过改一个字符匹配，必须先过滤。
3. **通配符方法漏判重复**：若字典中有 "hello" 和 "hallo"，模式 "h*llo" 同时匹配两个词，search("hello") 应返回 true（可以匹配 "hallo"），但如果只存在 "hello" 则应返回 false。
4. **Trie 方法中 changed 标志传递错误**：DFS 回溯时 changed 不应被重置，需要作为参数传递而非全局变量。
5. **空字典**：buildDict 传入空数组时，所有 search 应返回 false。

## 面试追问

1. **Q: 如果字典非常大（百万级别），哪种方法更好？**
   A: Trie + DFS 更优，search 复杂度与字典大小无关，仅与字符串长度和字母表大小相关。

2. **Q: 如果允许修改最多 k 个字符呢？**
   A: Trie + DFS，DFS 参数增加 `remaining_changes`，每次走不同分支时 remaining_changes--，到达终点时检查 remaining_changes >= 0 且至少改了一次。

3. **Q: 如何支持动态添加单词（不止一次 buildDict）？**
   A: 暴力方法直接 push_back 到对应长度的列表。Trie 方法直接插入新单词。通配符方法需要重新计算新单词的所有模式。

4. **Q: search 调用频率远高于 buildDict，如何优化？**
   A: 用通配符哈希预处理，search 降为 O(L)。或者用 Trie 让 search 更高效。

5. **Q: 能否用编辑距离来解这道题？**
   A: 可以但过于复杂。编辑距离包含插入、删除、替换，本题只需替换且恰好一次。逐字符比较更直接。

## 相关题型

| 题号 | 题名 | 关联 |
|------|------|------|
| 208 | Implement Trie | Trie 基础实现 |
| 211 | Design Add and Search Words Data Structure | Trie + 通配符搜索 |
| 720 | Longest Word in Dictionary | Trie 应用 |
| 72 | Edit Distance | 编辑距离（更泛化的问题） |
| 1065 | Index Pairs of a String | 字典匹配变体 |
