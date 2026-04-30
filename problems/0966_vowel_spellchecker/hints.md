# LeetCode 966 - Vowel Spellchecker

## 核心思路

用三层哈希表分别处理三种匹配优先级：精确匹配、忽略大小写匹配、元音模糊匹配。对于每个查询词，按优先级依次查找：先查精确匹配（unordered_set），再查忽略大小写（全小写映射），最后查元音模糊（将元音替换为统一占位符后映射）。每层映射只保留词表中第一个匹配的词。

## 思维链

1. **分析匹配规则** - 三种匹配按优先级排列：精确匹配 > 忽略大小写 > 元音模糊。需要按顺序尝试。
2. **精确匹配** - 直接用 unordered_set 存储原始词表，O(1) 查找。
3. **忽略大小写匹配** - 将词表中每个词转为全小写作为 key，映射到原始词。如果多个词小写后相同，只保留第一个（题目要求返回第一个匹配）。
4. **元音模糊匹配** - 在忽略大小写的基础上，再将所有元音替换为统一字符（如 '*'）作为 key。同样只保留词表中第一个对应的原始词。
5. **查询处理** - 对每个查询词，依次检查三层：精确 set -> 小写 map -> 元音 map。第一个命中的就是答案。都没命中返回空串。
6. **关键细节** - 映射中只存第一次出现的词（`insert` 或 `emplace` 而非 `[]` 赋值），保证返回词表中最先出现的匹配。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|----------|----------|------|
| 三层哈希表 | O((M+N)*L) | O(M*L) | 推荐，清晰高效 |
| 暴力逐一比较 | O(M*N*L) | O(1) | 最慢，但逻辑直观 |
| Trie 变体 | O((M+N)*L) | O(M*L) | 过度设计，不推荐 |

其中 M 为词表长度，N 为查询数，L 为单词平均长度。

## 关键提示

1. 元音字母是 a, e, i, o, u（大小写都算）。
2. 三种匹配的优先级很重要：精确 > 大小写 > 元音。不能跳过中间层。
3. 哈希表中只保留第一次出现的映射，后续相同 key 的词忽略。这保证了"返回词表中第一个匹配"的语义。
4. 元音模糊匹配是在忽略大小写的基础上进行的（query 也要先转小写再替换元音）。
5. 词表和查询的长度都不超过 7，字符串操作很快。
6. `unordered_set` 用于精确匹配，`unordered_map` 用于模糊匹配，各司其职。

## 解法详解

### 解法一：三层哈希表（推荐）

**预处理词表**：
1. 精确匹配集合：`unordered_set<string> exact` - 存储所有原始单词
2. 大小写映射：`unordered_map<string, string> caseMap` - key 是全小写，value 是原始词（第一个）
3. 元音映射：`unordered_map<string, string> vowelMap` - key 是小写+元音替换，value 是原始词（第一个）

**辅助函数**：
- `toLower(s)`: 将字符串转为全小写
- `toVowelKey(s)`: 先转小写，再将所有元音替换为 '*'

**预处理**（遍历词表）：
```
for word in wordlist:
    exact.insert(word)
    lower = toLower(word)
    if lower not in caseMap: caseMap[lower] = word
    vkey = toVowelKey(word)
    if vkey not in vowelMap: vowelMap[vkey] = word
```

**查询处理**：
```
for query in queries:
    if query in exact: result = query
    elif toLower(query) in caseMap: result = caseMap[toLower(query)]
    elif toVowelKey(query) in vowelMap: result = vowelMap[toVowelKey(query)]
    else: result = ""
```

**为什么用 insert 而不是直接赋值？** 因为题目要求返回词表中第一个匹配的词。如果用 `map[key] = word` 会被后面的词覆盖。`insert` 或 `emplace` 在 key 已存在时不会覆盖。

### 解法二：暴力比较

对于每个查询，遍历整个词表，按优先级检查：
1. 先扫描一遍找精确匹配
2. 没找到就再扫描一遍找忽略大小写匹配
3. 还没找到就再扫描一遍找元音模糊匹配

可以优化为单次扫描：维护三个候选变量，扫描完词表后按优先级返回。

缺点是 O(M*N*L) 时间复杂度，对于 5000*5000 可能达到 25M 次比较，较慢。

### 辅助函数实现细节

**toLower**：遍历每个字符，如果是大写字母就加 32（或用 `tolower()`）。

**toVowelKey**：先调用 toLower，然后将 a/e/i/o/u 全部替换为同一个占位符字符（如 '*' 或 '#'）。

**isVowel**：检查字符是否为 a/e/i/o/u（已小写化后只需检查小写）。

## 易错点

1. **映射覆盖问题** - 必须只保留第一个匹配的词。用 `map[key] = word` 会覆盖，应该先检查 key 是否存在。
2. **元音替换必须在小写化之后** - 先转小写再替换元音，否则大小写不一致会导致 key 不匹配。
3. **精确匹配必须用原始大小写** - 精确匹配是 case-sensitive 的，不能用小写比较。
4. **查询词本身在词表中时** - 即使查询词的大小写与词表不同，精确匹配优先。例如词表有 "KiTe"，查询 "KiTe" 应该精确匹配返回 "KiTe"。
5. **空串处理** - 没有任何匹配时返回空串 ""，不是 "null" 或其他。
6. **元音替换不改变长度** - 只替换元音字符为占位符，不删除或增加字符。"yeellow" 和 "yellow" 长度不同，不会匹配。

## 面试追问

1. **如果词表非常大（百万级），如何优化？** - 哈希表方案不变，时间仍然是线性的。可以考虑预分配 reserve 减少 rehash。
2. **如果要支持模糊匹配中替换任意 k 个字符怎么办？** - 变成编辑距离问题，需要用 BK-tree 或 locality-sensitive hashing。
3. **如果查询非常频繁，预处理可以怎么优化？** - 预处理是一次性的 O(M*L)，之后每次查询 O(L)。可以把哈希表持久化。
4. **如果元音定义可以动态变化，怎么设计？** - 将元音集合参数化，重新构建 vowelMap。
5. **如何扩展到支持通配符匹配？** - 将通配符位置也用占位符替换，类似元音处理的泛化。

## 相关题型

- **LeetCode 720: Longest Word in Dictionary** - 字典查找相关，哈希表应用
- **LeetCode 676: Implement Magic Dictionary** - 模糊匹配，允许修改一个字符
- **LeetCode 211: Design Add and Search Words Data Structure** - 通配符匹配，Trie 应用
- **LeetCode 1065: Index Pairs of a String** - 字符串匹配，多模式搜索
- **LeetCode 524: Longest Word in Dictionary through Deleting** - 字典匹配变体
