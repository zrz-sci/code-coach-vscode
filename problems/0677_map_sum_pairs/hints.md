# 677. Map Sum Pairs - 键值映射

## 核心思路

设计一个支持 `insert(key, val)` 和 `sum(prefix)` 的数据结构。insert 插入或覆盖键值对，sum 返回所有以 prefix 开头的 key 对应 value 之和。最优解法是 Trie + HashMap 组合：Trie 每个节点维护经过该前缀的累计 value 和，HashMap 记录每个 key 的已有 value 以便计算 delta。insert 时沿 Trie 路径加 delta，sum 时直接读节点值。

## 思维链

1. 读题：insert("apple", 3) + insert("app", 2) 后，sum("ap") = 5。
2. 核心操作：sum 需要快速获取所有以某前缀开头的 key 的 value 总和。
3. 朴素思路：HashMap 存所有键值对，sum 时遍历所有 key 检查前缀，O(N*L) 每次查询。
4. 优化思路：前缀树（Trie）天然支持前缀查询。
5. 关键挑战：insert 可能覆盖已有 key 的 value，需要更新 Trie 中的累计和。
6. 解决方案：用 HashMap 记录旧值，计算 delta = newVal - oldVal，沿 Trie 路径每个节点加 delta。
7. 时间分析：insert O(L)，sum O(L)，L 为 key/prefix 长度。
8. 也可以不在节点存累计和，而是在叶子节点存值，sum 时 DFS/BFS 遍历子树求和，但每次 sum 更慢。

## 解法概览

| 解法 | insert | sum | 空间 | 优劣 |
|------|--------|-----|------|------|
| HashMap + 遍历 | O(L) | O(N*L) | O(N*L) | 最简单，sum 慢 |
| Trie + DFS 求和 | O(L) | O(子树大小) | O(N*L*26) | sum 不稳定 |
| Trie + HashMap + delta 更新 | O(L) | O(L) | O(N*L*26) | 最优，推荐 |

## 关键提示

1. **delta 更新是精髓**：insert 覆盖时不能简单加 val，而要加 `val - oldVal`。否则覆盖时 Trie 中的累计和会偏大。
2. **HashMap 记录旧值**：用 `unordered_map<string, int>` 存每个 key 的当前 value，insert 前查询旧值计算 delta。
3. **节点的 sum 含义**：Trie 中每个节点的 sum 字段表示"所有经过该节点（以该节点对应前缀开头）的 key 的 value 总和"。
4. **前缀不存在**：sum 查询时如果某一层的子节点不存在，直接返回 0。
5. **不需要 isEnd 标记**：与标准 Trie 不同，本题不需要判断某个前缀是否是完整 key，因为 sum 只需要累计值。

## 解法详解

### 解法一：Trie + HashMap + delta 更新（推荐）

**数据结构**：
- `TrieNode`：26 个子指针 + `int sum`（经过该节点的所有 key 的 value 累计和）
- `unordered_map<string, int> keyMap`：记录每个 key 的当前 value

**insert(key, val)**：
1. 查 keyMap 获取旧值 oldVal（不存在则为 0）
2. 计算 delta = val - oldVal
3. 更新 keyMap[key] = val
4. 沿 Trie 路径遍历（不存在则创建节点），每个节点 `node->sum += delta`

**sum(prefix)**：
1. 沿 Trie 路径遍历 prefix 的每个字符
2. 如果某个字符对应的子节点不存在，返回 0
3. 到达 prefix 末尾节点，返回 `node->sum`

### 解法二：HashMap + 暴力遍历

**insert**：直接 `map[key] = val`。

**sum**：遍历 map 中所有 entry，检查 key 是否以 prefix 开头（`key.substr(0, prefix.size()) == prefix`），累加对应 value。

简单但 sum 慢，数据量小时可用。

### 解法三：Trie + DFS 子树求和

**insert**：将 key 插入 Trie，在叶子节点存 value（覆盖时直接替换）。

**sum**：沿 Trie 找到 prefix 末尾节点，然后 DFS 遍历整棵子树，累加所有叶子节点的 value。

不需要 delta 计算，但 sum 的时间取决于子树大小。

## 易错点

1. **覆盖时直接加 val**：insert("apple", 3) 后再 insert("apple", 5)，sum 应该是 5 而不是 8。必须用 delta 更新。
2. **忘记初始化 keyMap 默认值**：如果 key 不在 keyMap 中，oldVal 应该是 0，不是未定义值。
3. **Trie 节点的 children 未初始化**：C++ 中数组成员必须初始化为 nullptr，否则访问未初始化指针会崩溃。
4. **前缀 prefix 比所有 key 都长**：此时 Trie 中走到某层没有子节点，应返回 0。
5. **混淆"节点值"和"路径值"**：每个节点的 sum 是所有经过它的 key 的值之和，而非该节点自身的值。

## 面试追问

1. **Q: 如果 key 数量非常大（百万级），Trie 内存如何优化？**
   A: 用压缩 Trie（Radix Tree / Patricia Trie），将单子链路径压缩为一个节点存储字符串片段，大幅减少节点数。

2. **Q: 如何支持 delete(key) 操作？**
   A: 从 keyMap 取出旧值 oldVal，delta = -oldVal，沿 Trie 路径每个节点减去 oldVal。从 keyMap 中删除该 key。可选地清理 Trie 中引用计数为 0 的节点。

3. **Q: 如果 sum 需要支持通配符前缀（如 "a?p"），怎么做？**
   A: 类似 211 题的 Trie + DFS，遇到通配符时遍历所有子节点分支。

4. **Q: 不使用 Trie，能否用排序 + 二分做 sum？**
   A: 可以。将所有 key 排序，用 lower_bound 找到第一个 >= prefix 的位置，用 upper_bound 找到最后一个以 prefix 开头的位置（prefix 最后一个字符 +1 作为上界），遍历区间求和。insert 需要维护有序结构（如 std::map）。

5. **Q: insert 和 sum 的调用频率差异很大时，如何选择方案？**
   A: 如果 sum 远多于 insert，Trie + delta 方案最优（sum O(L)）。如果 insert 远多于 sum，暴力 HashMap 也可以接受（insert O(1)，sum 时才遍历）。

## 相关题型

| 题号 | 题名 | 关联 |
|------|------|------|
| 208 | Implement Trie (Prefix Tree) | Trie 基础实现 |
| 211 | Design Add and Search Words | Trie + 通配符 DFS |
| 676 | Implement Magic Dictionary | Trie 设计题 |
| 1804 | Implement Trie II | 进阶 Trie（带计数） |
| 745 | Prefix and Suffix Search | 前后缀同时查询 |
