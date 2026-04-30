# 1166. 设计文件系统

## 核心思路

本质上是设计一个**路径 → 值**的映射系统，核心难点在于 `createPath` 时需要验证「父路径必须存在」且「当前路径不能已存在」。这和真实文件系统的 `mkdir` 逻辑一致（没有 `-p` 选项时，父目录必须存在）。

## 思维链

1. **读完题第一反应**：路径是字符串，值是整数，最简单的做法就是用一个 HashMap 存 `path → value`。`get` 直接查表，`createPath` 先检查再插入。

2. **createPath 的难点是什么？** 需要验证父路径存在。比如创建 `"/a/b/c"` 时，`"/a/b"` 必须在 map 中。那怎么得到父路径？——从 path 中找到最后一个 `/` 的位置，截取前面的部分就是父路径。

3. **HashMap 方案够用吗？** 对于这道题的约束（路径长度 ≤ 100，调用次数 ≤ 10⁴），HashMap 完全够用。每次操作是 O(路径长度)，总体非常快。

4. **有没有更"结构化"的方案？** 既然路径天然是树形结构（`/a/b/c` 是 `/a/b` 的子节点），可以用 **Trie（前缀树）** 来存储。每个节点代表路径的一个组件（如 `a`、`b`、`c`），值存在叶子节点或中间节点上。

5. **Trie vs HashMap 的权衡**：HashMap 实现简单，适合面试快速写出；Trie 更贴近真实文件系统的树形结构，展示设计能力。面试中先说 HashMap，被追问再给 Trie。

## 解法概览

| 解法 | 思路 | 时间 (createPath / get) | 空间 | 面试 |
|------|------|------------------------|------|------|
| HashMap | 直接用哈希表存路径→值，父路径通过字符串截取获得 | O(L) / O(L) | O(所有路径总长) | ⭐ 必须写出 |
| Trie | 路径按 `/` 拆分成组件，构建前缀树 | O(L) / O(L) | O(所有路径总长) | 加分项 |

> L = 路径长度

## 关键提示

1. **父路径怎么得到？** 对 `"/a/b/c"`，父路径是 `"/a/b"`。用 `rfind('/')` 找到最后一个 `/` 的位置，然后 `substr(0, pos)` 就是父路径。

2. **特殊情况：一级路径如 `"/a"`**。它的父路径是 `""`（空字符串）或者你可以认为根目录 `/` 总是存在的。想清楚 base case 怎么处理。

3. **Trie 方案中，节点的 key 是什么？** 不是单个字符，而是两个 `/` 之间的完整字符串（如 `"leet"`、`"code"`）。这和传统 Trie（每个节点一个字符）不同。

4. **`createPath` 的两个失败条件别漏**：① 路径已存在 → 返回 false；② 父路径不存在 → 返回 false。两个条件的检查顺序不影响正确性，但要都检查。

5. **路径格式提示**：题目保证路径以 `/` 开头，且至少有一个组件（长度 ≥ 2）。不需要处理 `""` 或 `"/"` 作为输入。

## 解法详解

### 解法1: HashMap — O(L) / O(L) ⭐ 面试首选

**思考过程**：路径是字符串，值是整数，最自然的映射就是 HashMap。`get` 直接查表。`createPath` 的关键是提取父路径——找到最后一个 `/`，截取前面的部分。对于一级路径（如 `"/a"`），父路径是空串，我们规定空串"总是存在"（相当于根目录始终存在）。

```cpp
class FileSystem {
    unordered_map<string, int> pathMap;
public:
    FileSystem() {}
    
    bool createPath(string path, int value) {
        // 如果路径已存在，返回 false
        if (pathMap.count(path)) return false;
        
        // 提取父路径：找最后一个 '/' 的位置
        int lastSlash = path.rfind('/');
        string parent = path.substr(0, lastSlash);
        
        // 父路径为空串表示一级路径（如 "/a"），根目录默认存在
        // 否则父路径必须在 map 中
        if (!parent.empty() && !pathMap.count(parent)) return false;
        
        pathMap[path] = value;
        return true;
    }
    
    int get(string path) {
        if (pathMap.count(path)) return pathMap[path];
        return -1;
    }
};
```

**关键点**：
- `rfind('/')` 找最后一个斜杠，而不是第一个。对 `"/a/b/c"`，`rfind` 返回 4（指向 `b` 后面的 `/`），`substr(0,4)` 得到 `"/a/b"`。
- 一级路径如 `"/a"` 的 `lastSlash = 0`，`substr(0,0)` 得到空串 `""`，空串我们视为根目录，直接放行。

### 解法2: Trie（前缀树）— O(L) / O(L)

**从解法1延伸**：HashMap 把路径当做扁平字符串，丢失了路径的树形层次关系。如果面试官追问"如何支持列出某目录下的所有子路径"之类的操作，Trie 天然支持这种层次遍历。

```
// Trie 结构示意（创建 "/leet" 和 "/leet/code" 后）:
//
//  [root]
//    |
//  "leet" (val=1)
//    |
//  "code" (val=2)
//
// 每个节点用 children map 存子节点，key 是路径组件名
```

```cpp
class FileSystem {
    struct TrieNode {
        unordered_map<string, TrieNode*> children;
        int value;
        TrieNode() : value(-1) {}  // -1 表示该节点未被 createPath 设置过值
    };
    
    TrieNode* root;
    
    // 将路径 "/a/b/c" 拆分为 ["a", "b", "c"]
    vector<string> split(const string& path) {
        vector<string> parts;
        int i = 1; // 跳过开头的 '/'
        while (i < path.size()) {
            int j = path.find('/', i);
            if (j == string::npos) j = path.size();
            parts.push_back(path.substr(i, j - i));
            i = j + 1;
        }
        return parts;
    }
    
public:
    FileSystem() {
        root = new TrieNode();
    }
    
    bool createPath(string path, int value) {
        vector<string> parts = split(path);
        TrieNode* node = root;
        
        for (int i = 0; i < parts.size(); i++) {
            if (i == parts.size() - 1) {
                // 最后一个组件：它不能已存在
                if (node->children.count(parts[i])) return false;
                node->children[parts[i]] = new TrieNode();
                node->children[parts[i]]->value = value;
                return true;
            } else {
                // 中间组件：它必须存在（父路径必须存在）
                if (!node->children.count(parts[i])) return false;
                node = node->children[parts[i]];
            }
        }
        return false; // 不应到达
    }
    
    int get(string path) {
        vector<string> parts = split(path);
        TrieNode* node = root;
        for (const string& part : parts) {
            if (!node->children.count(part)) return -1;
            node = node->children[part];
        }
        return node->value;
    }
};
```

**关键点**：
- Trie 节点的 `children` key 不是单个字符，而是完整的路径组件（如 `"leet"`），这是和普通字符 Trie 的区别。
- `createPath` 中，最后一个组件要**新建**节点，而中间组件必须**已存在**。这个逻辑区分非常关键。
- 注意：`createPath` 检查最后一个组件**已存在于 children 中**时应返回 false（路径已存在）。但我们还需要检查该节点的 value 是否被设置过。其实题目保证路径要么完全不存在要么已被创建，所以只要节点存在于 children 中就说明路径已创建。

### 解法2 改进版: Trie（更严谨的已存在检查）

上面的 Trie 有个微妙问题：如果先创建 `"/a/b"`，那中间节点 `"a"` 也被创建了（作为父路径的节点），但 `"a"` 的 value 是 -1，它并没有被 `createPath("/a", ...)` 创建过。后续调用 `createPath("/a", 5)` 应该成功还是失败？

根据题意，`"/a"` 必须先被显式创建才能作为 `"/a/b"` 的父路径。所以**不会出现中间节点"隐式创建"的情况**——题目保证 `createPath("/a/b", ...)` 之前一定已经成功调用过 `createPath("/a", ...)`。

但为了更健壮，我们可以用 `value != -1` 来标记一个节点是否被正式创建：

```cpp
class FileSystem {
    struct TrieNode {
        unordered_map<string, TrieNode*> children;
        int value;
        TrieNode() : value(-1) {}
    };
    
    TrieNode* root;
    
    vector<string> split(const string& path) {
        vector<string> parts;
        int i = 1;
        while (i < (int)path.size()) {
            int j = path.find('/', i);
            if (j == string::npos) j = path.size();
            parts.push_back(path.substr(i, j - i));
            i = j + 1;
        }
        return parts;
    }
    
public:
    FileSystem() { root = new TrieNode(); }
    
    bool createPath(string path, int value) {
        vector<string> parts = split(path);
        TrieNode* node = root;
        
        for (int i = 0; i < (int)parts.size(); i++) {
            if (i < (int)parts.size() - 1) {
                // 父路径的每个组件必须存在且被正式创建
                if (!node->children.count(parts[i])) return false;
                node = node->children[parts[i]];
                if (node->value == -1) return false; // 该节点未被正式创建
            } else {
                // 最后一个组件：不能已存在
                if (node->children.count(parts[i]) && 
                    node->children[parts[i]]->value != -1) return false;
                if (!node->children.count(parts[i]))
                    node->children[parts[i]] = new TrieNode();
                node->children[parts[i]]->value = value;
            }
        }
        return true;
    }
    
    int get(string path) {
        vector<string> parts = split(path);
        TrieNode* node = root;
        for (const string& part : parts) {
            if (!node->children.count(part)) return -1;
            node = node->children[part];
        }
        return node->value;
    }
};
```

## 解法对比

| | HashMap | Trie |
|---|---------|------|
| 实现复杂度 | 简单（~15行核心逻辑） | 中等（需要拆分路径、管理树节点） |
| 时间复杂度 | O(L)（哈希 + 字符串操作） | O(L)（逐组件遍历） |
| 空间复杂度 | O(总路径长度) | O(总路径长度 + 节点指针开销) |
| 扩展性 | 不支持"列出子目录"等操作 | 天然支持层次遍历 |
| 面试推荐 | **首选**，快速正确 | 追问"如何支持列目录"时给出 |

**建议策略**：面试先写 HashMap（3分钟写完），然后主动说"如果需要支持 `ls` 列目录操作，可以改用 Trie 结构"。

## 易错点

1. **父路径提取错误**
   - ✗ `path.find('/')` — 找到的是第一个 `/`（位置 0），截取出空串，永远不检查父路径
   - ✓ `path.rfind('/')` — 找最后一个 `/`，正确截取出父路径

2. **忘记处理一级路径的 base case**
   - ✗ 对 `"/a"`，父路径是 `""`，去 map 里查 `""`，找不到就返回 false
   - ✓ 判断 `parent.empty()` 时直接放行（根目录默认存在）

3. **createPath 只检查了一个失败条件**
   - ✗ 只检查父路径存在，忘了检查当前路径已存在的情况
   - ✓ 两个条件都要检查：路径已存在 → false，父路径不存在 → false

4. **Trie 的 split 函数漏掉最后一个组件**
   - ✗ 用 `find('/', i)` 但没处理找不到 `/` 的情况（`npos`），导致最后一个组件丢失
   - ✓ `if (j == string::npos) j = path.size();` 确保最后一个组件被加入

## 面试追问

**Q1: 为什么用 HashMap 就够了？时间复杂度是多少？**
> `createPath` 和 `get` 都是 O(L)，L 是路径长度。主要操作是字符串哈希和 `rfind`。题目约束 L ≤ 100，调用 ≤ 10⁴，总操作量约 10⁶，完全没问题。

**Q2: 如果要支持 `delete(path)` 操作（删除路径及其所有子路径），HashMap 方案怎么改？有什么问题？**
> HashMap 需要遍历所有 key 检查哪些以 `path` 为前缀，这是 O(N·L)（N 是总路径数）。而 Trie 只需找到对应节点，删除整棵子树，O(子树大小)。这就是 Trie 的优势场景。

**Q3: 如果要支持 `ls(path)` 列出某路径下的直接子路径，怎么做？**
> HashMap 方案很难高效实现（需要遍历所有 key）。Trie 方案直接返回该节点的 `children` 的 key 列表即可，O(子节点数)。这是面试官期望你主动提出 Trie 的典型追问。

**Q4: 如果路径长度可以很长（百万级），createPath 有什么性能瓶颈？**
> 字符串哈希和 `substr` 都是 O(L) 的。如果担心哈希冲突，可以用 Trie 避免哈希。另外 `substr` 会产生新字符串的拷贝，可以用 `string_view` 优化。

## 相关题型

- **588. 设计内存文件系统** — 本题的增强版，需要额外支持 `ls`、`mkdir -p`、`addContentToFile`、`readContentFromFile`。本题的 Trie 方案可以直接复用为基础框架，区别在于需要在节点上存文件内容，且 `mkdir` 支持递归创建。
- **208. 实现 Trie（前缀树）** — 标准 Trie 模板题，区别：208 的节点 key 是单个字符，本题的节点 key 是完整路径组件。掌握 208 后可以直接改写。
- **211. 添加与搜索单词** — Trie + 通配符搜索，复用 Trie 的遍历框架，增加 `.` 通配的分支递归。