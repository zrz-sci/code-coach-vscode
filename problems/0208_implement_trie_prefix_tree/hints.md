# 208. 实现 Trie (前缀树)

## 核心思路

本题要求你从零设计一个数据结构，使得**插入字符串、精确查找字符串、前缀匹配**三种操作都能在 O(L)（L 为字符串长度）时间内完成。核心问题是：如何让多个字符串**共享公共前缀**，避免重复存储？

## 思维链

1. **读完题第一反应**：最朴素的做法是用一个 `vector<string>` 或 `unordered_set<string>` 存所有插入过的单词。`search` 直接查集合，`startsWith` 遍历所有单词看是否有匹配前缀。
2. **暴力解的瓶颈**：`startsWith` 需要遍历所有已存储的字符串并逐一检查前缀，最坏 O(N·L)，N 是单词数量。能不能让前缀查询也是 O(L)？
3. **突破瓶颈的关键观察**：如果 "apple" 和 "app" 共享前缀 "app"，我们能不能用树结构把公共前缀只存一次？每个节点代表一个字符，从根到某个节点的路径就是一个前缀。
4. **Trie 的结构设计**：每个节点有 26 个子节点指针（对应 a-z），加一个 `isEnd` 标志表示"到这个节点为止是否构成一个完整单词"。
5. **三个操作的统一模式**：insert/search/startsWith 本质上都是"沿着字符路径往下走"，区别只在终止时检查什么——search 要求 `isEnd=true`，startsWith 只要路径存在即可。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| HashSet 暴力 | 用集合存所有单词，前缀匹配遍历 | insert O(L), search O(L), startsWith O(N·L) | O(N·L) | 能说出即可 |
| 数组实现 Trie | 每个节点用 `children[26]` 数组 | 三种操作均 O(L) | O(Σ·26)，Σ为总字符数 | ⭐ 必须写出 |
| HashMap 实现 Trie | 每个节点用 `unordered_map<char, Node*>` | 三种操作均 O(L) | O(Σ)，更紧凑 | 加分项 |

## 关键提示

1. **search vs startsWith 的唯一区别是什么？** 想想 insert("apple") 后 search("app") 和 startsWith("app") 为什么结果不同。这决定了你在节点中需要存什么额外信息。

2. **Trie 的树结构长什么样？** 插入 "apple" 和 "app" 后：
```
         root
          |
          a
          |
          p
          |
          p  ← isEnd=true (因为 "app" 被插入)
          |
          l
          |
          e  ← isEnd=true (因为 "apple" 被插入)
```

3. **insert 时如果路径上的节点已存在怎么办？** 不需要创建新节点，直接沿着已有路径走，只在缺失的地方创建。

4. **数组 vs HashMap 存子节点**：字符集只有 26 个小写字母时，固定大小数组更简单高效；如果字符集很大（如 Unicode），HashMap 更节省空间。

5. **内存管理**：面试中用 `new` 分配节点通常可以接受（不需要手写析构），但如果面试官追问内存泄漏，可以讨论智能指针或对象池。

## 解法详解

### 解法1: HashSet 暴力 — O(N·L) / O(N·L)

**思考过程**: 最直觉的方式——把所有单词丢进一个集合。search 就是查集合，startsWith 需要遍历所有单词逐一比对前缀。

```cpp
class Trie {
    unordered_set<string> words;
public:
    Trie() {}
    
    void insert(string word) {
        words.insert(word);
    }
    
    bool search(string word) {
        return words.count(word);
    }
    
    bool startsWith(string prefix) {
        // 瓶颈在这里：必须遍历所有单词
        for (const string& w : words) {
            if (w.size() >= prefix.size() && 
                w.substr(0, prefix.size()) == prefix) {
                return true;
            }
        }
        return false;
    }
};
```

**关键点**: search 是 O(L) 没问题，但 startsWith 退化成 O(N·L)。当单词数量多时，前缀查询成为瓶颈。这正是 Trie 要解决的问题。

### 解法2: 数组实现 Trie — O(L) / O(Σ·26) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是前缀查询需要遍历所有单词。如果我们把字符串按字符拆开，用树结构组织，让共享前缀的字符串走同一条路径，那么前缀查询就只需要沿路径走一遍，O(L)。

```
// 插入 "apple", "app", "bat" 后的 Trie 结构:
//
//           root
//          /    \
//         a      b
//         |      |
//         p      a
//         |      |
//         p*     t*     (* 表示 isEnd=true)
//         |
//         l
//         |
//         e*
//
// search("app")     → 走到第二个 p，isEnd=true  → true
// search("ap")      → 走到第一个 p，isEnd=false → false
// startsWith("ap")  → 走到第一个 p，路径存在    → true
// search("bat")     → 走到 t，isEnd=true        → true
// search("ba")      → 走到 a，isEnd=false       → false
```

```cpp
class Trie {
    // 每个节点：26个子节点指针 + 是否是某个单词的结尾
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;
        TrieNode() : isEnd(false) {
            // 初始化所有子节点为 nullptr
            for (int i = 0; i < 26; i++) {
                children[i] = nullptr;
            }
            // 也可以用 memset(children, 0, sizeof(children));
        }
    };
    
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            // 如果该字符对应的子节点不存在，创建它
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            // 沿着路径往下走
            node = node->children[idx];
        }
        // 走完整个单词后，标记当前节点为单词结尾
        node->isEnd = true;
    }
    
    bool search(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            // 路径断了，说明这个单词不存在
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        // 关键：必须是一个完整单词的结尾，不能只是某个更长单词的前缀
        return node->isEnd;
    }
    
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        // 与 search 的唯一区别：只要路径存在就行，不要求 isEnd
        return true;
    }
};
```

**关键点**: 
- `search` 和 `startsWith` 的代码几乎一模一样，唯一区别是最后返回 `node->isEnd` 还是 `true`。
- 可以抽取公共的"沿路径查找"逻辑为一个私有辅助函数，减少代码重复（见下方重构版本）。

**重构版本（抽取公共逻辑）**:

```cpp
class Trie {
    struct TrieNode {
        TrieNode* children[26] = {};  // C++11 值初始化，自动全为 nullptr
        bool isEnd = false;
    };
    
    TrieNode* root;
    
    // 辅助函数：沿着 s 的路径走到尽头，返回终点节点
    // 如果中途路径断裂，返回 nullptr
    TrieNode* traverse(const string& s) {
        TrieNode* node = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!node->children[idx]) return nullptr;
            node = node->children[idx];
        }
        return node;
    }
    
public:
    Trie() : root(new TrieNode()) {}
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx])
                node->children[idx] = new TrieNode();
            node = node->children[idx];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        TrieNode* node = traverse(word);
        return node != nullptr && node->isEnd;  // 存在 且 是完整单词
    }
    
    bool startsWith(string prefix) {
        return traverse(prefix) != nullptr;      // 只要路径存在
    }
};
```

### 解法3: HashMap 实现 Trie — O(L) / O(Σ)

**与解法2的区别**: 把固定大小的 `children[26]` 数组替换为 `unordered_map`。优点是不浪费空间（只存实际存在的子节点），缺点是哈希表有常数开销。

```cpp
class Trie {
    struct TrieNode {
        unordered_map<char, TrieNode*> children;
        bool isEnd = false;
    };
    
    TrieNode* root;
    
public:
    Trie() : root(new TrieNode()) {}
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c)) return false;
            node = node->children[c];
        }
        return node->isEnd;
    }
    
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children.count(c)) return false;
            node = node->children[c];
        }
        return true;
    }
};
```

**关键点**: 当字符集很大（如支持 Unicode 而非仅 26 个字母）时，HashMap 版本更实用。对于本题只有小写字母的场景，数组版本更快且代码更简洁。

## 解法对比

| | HashSet 暴力 | 数组 Trie | HashMap Trie |
|---|---|---|---|
| insert | O(L) | O(L) | O(L) |
| search | O(L) | O(L) | O(L) |
| startsWith | **O(N·L)** 瓶颈 | **O(L)** | **O(L)** |
| 空间 | O(N·L) | O(Σ·26) | O(Σ) |
| 适用场景 | 不需要前缀查询时 | 字符集小（26字母） | 字符集大（Unicode） |
| 面试推荐 | ❌ | ✅ 首选 | ✅ 追问时说 |

**核心区别**: Trie 的价值在于把"前缀"这个概念结构化——共享前缀的字符串共享树上的路径，使得前缀查询不再需要遍历所有字符串。

## 易错点

1. **忘记初始化 children 数组**
   - ✗ `TrieNode* children[26];` 不初始化 → 野指针，判断 `!node->children[idx]` 会出错
   - ✓ `TrieNode* children[26] = {};` 或在构造函数中 `memset` / 循环初始化为 nullptr

2. **search 和 startsWith 混淆**
   - ✗ search 最后返回 `true`（应该返回 `node->isEnd`）
   - ✓ search 返回 `node->isEnd`，startsWith 返回 `true`
   - 这是本题最核心的区别点。insert("apple") 后 search("app") 应返回 false！

3. **insert 时忘记在最后标记 isEnd**
   - ✗ 只创建了路径上的节点，但没有 `node->isEnd = true`
   - ✓ 遍历完所有字符后，最后一个节点必须标记 `isEnd = true`

4. **字符索引计算错误**
   - ✗ `int idx = c;` → 会用 ASCII 值（97~122）作为下标，越界
   - ✓ `int idx = c - 'a';` → 映射到 0~25

## 面试追问

**Q1: search 和 startsWith 的代码几乎一样，你怎么消除重复？**
→ 抽取一个私有的 `traverse(string)` 函数，返回路径终点的节点指针（或 nullptr）。search 检查 `node && node->isEnd`，startsWith 检查 `node != nullptr`。（见解法2的重构版本）

**Q2: 如果需要支持删除操作 `delete(word)`，怎么实现？**
→ 两种思路：(1) 给每个节点加引用计数，delete 时沿路径递减，计数为 0 时删除节点；(2) 懒删除——只把 isEnd 改为 false。方案2简单但不释放空间，方案1更完整但需要递归清理。

**Q3: 如果需要支持通配符搜索（如 `.` 匹配任意字符），怎么改？**
→ 这就是 LeetCode 211（添加与搜索单词）。search 时遇到 `.` 需要对所有非空子节点递归搜索（DFS/回溯），最坏 O(26^L)，但平均情况远好于此。

**Q4: 如果字符集不是 26 个字母而是 Unicode，数组方案还可行吗？**
→ 不可行，每个节点开 65536 大小的数组太浪费。应该用 HashMap 版本（解法3），只存实际存在的子节点。

## 相关题型

- **211. 添加与搜索单词** — 直接复用本题的 Trie 结构，区别是 search 时需要处理通配符 `.`，改成 DFS 回溯搜索所有分支
- **212. 单词搜索 II** — 在二维网格上 DFS + Trie 剪枝，复用本题的 Trie 作为字典加速前缀判断
- **14. 最长公共前缀** — 可以用 Trie 解决：所有字符串插入后，从根开始走到第一个分叉点（有多个子节点或 isEnd 为 true 的位置）
- **648. 替换单词** — 复用 Trie 的 startsWith 逻辑，在 Trie 中查找最短前缀匹配
- **677. 键值映射** — Trie 的变体，节点存值而非只存 isEnd，复用相同的插入/遍历框架