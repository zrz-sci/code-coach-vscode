# 588. 设计内存文件系统

## 核心思路

这道题本质上是**设计一棵多叉树（Trie/前缀树的变体）**，其中每个节点代表一个目录或文件，路径就是从根到某个节点的链路。所有操作都是"沿着路径走到目标节点，然后执行操作"。

## 思维链

1. **读完题第一反应**：文件系统天然是树形结构——根目录 `/` 是根节点，每个子目录/文件是子节点。我们需要一种数据结构来表示这种层级关系。

2. **最朴素的想法**：用两个 HashMap，一个存"路径→子条目列表"（目录），一个存"路径→内容"（文件）。但这样 `mkdir` 需要手动维护所有中间目录的子条目列表，容易出错且冗余。

3. **更自然的思路**：既然是树形结构，为什么不直接建一棵树？每个节点用 HashMap 存储"子节点名 → 子节点指针"，这就是 **Trie（前缀树）** 的思想！路径 `/a/b/c` 就是从根走到 `a`，再走到 `b`，再走到 `c`。

4. **区分文件和目录**：节点需要一个标志区分自己是文件还是目录。文件节点额外存储 `content` 字符串。

5. **ls 的特殊处理**：如果 `ls` 的路径指向文件，返回文件名本身；如果指向目录，返回该目录下所有子条目名（排序后）。

6. **路径解析是公共操作**：所有操作都需要"从根沿路径走到目标节点"，可以抽取成一个公共的 `traverse` 方法。

## 解法概览

| 解法 | 思路 | 时间（单次操作） | 空间 | 面试 |
|------|------|------|------|------|
| Trie 树（节点用 map） | 每个节点是目录/文件，children 用 map 存 | O(m + k·log k)，m=路径深度，k=子条目数 | O(总节点数 + 文件内容) | ⭐ 必须写出 |
| 纯 HashMap 方案 | 两个 map 分别存目录内容和文件内容 | O(m + k·log k) | O(总路径数 + 文件内容) | 能说出即可 |

> 注：这是设计题，核心在于数据结构选择和接口实现的清晰性。Trie 方案是面试标准答案。

## 关键提示

1. **路径解析**：`"/a/b/c"` 需要拆分成 `["a", "b", "c"]`。注意根目录 `"/"` 拆分后是空数组，不是 `[""]`。

2. **文件 vs 目录**：同一个名字在同一父目录下不会既是文件又是目录（题目保证）。需要在节点上用 `isFile` 标志区分。

3. **ls 的两种行为**：对文件返回 `[文件名]`，对目录返回排序后的子条目名列表。如果用 `std::map`（有序），子条目天然有序；如果用 `unordered_map`，需要手动排序。

4. **mkdir 要创建中间目录**：和 Linux 的 `mkdir -p` 一样，沿路径走，不存在的节点就创建。

5. **addContentToFile 的路径**：题目保证父目录存在，但文件本身可能不存在（需要创建）或已存在（追加内容）。

```
// 文件系统的 Trie 结构示意:
//
//       root (/)
//        |
//        a          ← 目录节点
//        |
//        b          ← 目录节点
//       / \
//      c   e.txt    ← c是目录, e.txt是文件(isFile=true, content="...")
//      |
//     d.txt         ← 文件节点
//
// ls("/")       → ["a"]
// ls("/a/b")    → ["c", "e.txt"]
// ls("/a/b/c/d.txt") → ["d.txt"]  (文件，返回文件名本身)
```

## 解法详解

### 解法1: 纯 HashMap 方案 — O(m + k·log k) / O(总路径数)

**思考过程**：最直接的想法——不建树，用两个 map 直接记录。`dirs[path]` 存储该目录下的所有子条目名，`files[path]` 存储文件内容。

```cpp
class FileSystem {
    // dirs["/a/b"] = {"c", "d.txt"} 表示 /a/b 下有 c 和 d.txt
    unordered_map<string, set<string>> dirs;
    // files["/a/b/c/d.txt"] = "hello" 表示文件内容
    unordered_map<string, string> files;

public:
    FileSystem() {
        dirs["/"] = {};  // 根目录初始为空
    }

    vector<string> ls(string path) {
        // 如果是文件，返回文件名
        if (files.count(path)) {
            // 从路径中提取文件名：最后一个 '/' 后面的部分
            int pos = path.find_last_of('/');
            return {path.substr(pos + 1)};
        }
        // 如果是目录，返回排序后的子条目（set 自动排序）
        return vector<string>(dirs[path].begin(), dirs[path].end());
    }

    void mkdir(string path) {
        string cur = "";
        istringstream ss(path);
        string name;
        getline(ss, name, '/');  // 跳过第一个空字符串（因为路径以 / 开头）
        while (getline(ss, name, '/')) {
            string parent = cur.empty() ? "/" : cur;
            cur += "/" + name;
            // 把当前目录名加入父目录的子条目集合
            dirs[parent].insert(name);
            // 确保当前目录也有条目（即使为空）
            if (!dirs.count(cur)) dirs[cur] = {};
        }
    }

    void addContentToFile(string filePath, string content) {
        // 追加内容（如果不存在则创建）
        files[filePath] += content;
        // 把文件名注册到父目录
        int pos = filePath.find_last_of('/');
        string parentDir = (pos == 0) ? "/" : filePath.substr(0, pos);
        string fileName = filePath.substr(pos + 1);
        dirs[parentDir].insert(fileName);
    }

    string readContentFromFile(string filePath) {
        return files[filePath];
    }
};
```

**关键点**：
- 用 `set<string>` 存子条目可以自动排序，避免每次 `ls` 手动排序。
- `mkdir` 需要逐层创建中间目录，且每层都要注册到父目录的 `set` 中。
- 这种方案的问题：目录和文件的关系靠字符串路径维护，不够直观，容易出 bug。

### 解法2: Trie 树（多叉树）方案 — O(m + k·log k) / O(总节点数) ⭐ 面试首选

**从解法1优化**：解法1用字符串路径做 key，逻辑分散且不自然。文件系统天然就是树，直接建一棵 Trie 树，每个节点代表一个目录或文件，操作就变成了"沿路径遍历树节点"。

```cpp
class FileSystem {
    // Trie 节点：代表一个目录或文件
    struct TrieNode {
        // children: 子节点名 → 子节点指针
        // 用 map 而非 unordered_map，这样遍历时天然有序（字典序）
        map<string, TrieNode*> children;
        bool isFile = false;       // 标记是否为文件
        string content = "";       // 如果是文件，存储内容
    };

    TrieNode* root;

    // 公共方法：解析路径字符串为目录/文件名列表
    // "/a/b/c" → ["a", "b", "c"]
    // "/"      → []
    vector<string> parsePath(const string& path) {
        vector<string> parts;
        istringstream ss(path);
        string token;
        getline(ss, token, '/');  // 跳过开头的空串
        while (getline(ss, token, '/')) {
            if (!token.empty()) parts.push_back(token);
        }
        return parts;
    }

    // 公共方法：沿路径走到目标节点（如果 create=true，沿途创建不存在的节点）
    TrieNode* traverse(const string& path, bool create = false) {
        vector<string> parts = parsePath(path);
        TrieNode* cur = root;
        for (const string& part : parts) {
            if (!cur->children.count(part)) {
                if (create) {
                    cur->children[part] = new TrieNode();
                } else {
                    return nullptr;  // 路径不存在
                }
            }
            cur = cur->children[part];
        }
        return cur;
    }

public:
    FileSystem() {
        root = new TrieNode();  // 根节点代表 "/"
    }

    vector<string> ls(string path) {
        TrieNode* node = traverse(path);
        // 如果是文件，返回文件名本身
        if (node->isFile) {
            // 提取路径最后一段作为文件名
            vector<string> parts = parsePath(path);
            return {parts.back()};
        }
        // 如果是目录，返回所有子条目名（map 遍历天然有序）
        vector<string> result;
        for (auto& [name, child] : node->children) {
            result.push_back(name);
        }
        return result;
    }

    void mkdir(string path) {
        // 沿路径走，create=true 自动创建中间目录
        traverse(path, true);
    }

    void addContentToFile(string filePath, string content) {
        // 沿路径走到文件节点（沿途创建不存在的节点）
        TrieNode* node = traverse(filePath, true);
        node->isFile = true;       // 标记为文件
        node->content += content;  // 追加内容
    }

    string readContentFromFile(string filePath) {
        TrieNode* node = traverse(filePath);
        return node->content;
    }
};
```

**关键点**：
- `traverse` 是核心方法，所有操作都复用它——走到目标节点，然后做具体操作。
- 用 `std::map` 存 children 使得 `ls` 遍历时天然字典序，无需额外排序。
- `mkdir` 只需一行 `traverse(path, true)`，因为 traverse 会自动创建中间节点。
- 文件节点也是 TrieNode，只是 `isFile=true` 且有 `content`。

## 解法对比

| | HashMap 方案 | Trie 树方案 |
|--|-------------|------------|
| 数据结构 | 两个 map，靠路径字符串关联 | 一棵树，节点间有显式父子关系 |
| 直观性 | 路径操作分散，容易遗漏 | 树形结构和文件系统一一对应 |
| ls 排序 | 用 set 自动排序 | 用 map 自动排序 |
| 扩展性 | 难以支持"删除目录"等操作 | 直接删除子树即可 |
| 内存管理 | 字符串路径有冗余存储 | 共享前缀，更紧凑 |
| **面试推荐** | 能说即可 | **⭐ 首选** |

## 易错点

1. **根目录路径解析**：
   - ✗ `"/".split("/")` 得到 `["", ""]`，直接用会在 Trie 中创建空名节点
   - ✓ 正确做法：根目录 `"/"` 解析后应得到空列表 `[]`，traverse 直接返回 root

2. **ls 对文件路径的处理**：
   - ✗ 忘记判断 `isFile`，对文件也返回其 children（文件没有 children）
   - ✓ 先检查 `node->isFile`，如果是文件则提取最后一段路径名返回

3. **addContentToFile 忘记 `isFile = true`**：
   - ✗ 只追加了 content，没有标记 `isFile`，导致 `ls` 将其当目录处理
   - ✓ `node->isFile = true; node->content += content;` 两步缺一不可

4. **ls 根目录返回空列表**：
   - ✗ 如果根目录没有子条目，错误返回 `["/"]` 或 `[""]`
   - ✓ 根目录无子条目时应返回空 `vector`

5. **路径解析时多余的空串**：
   - ✗ 用 `getline(ss, token, '/')` 但不跳过第一个空串，导致创建名为 `""` 的子节点
   - ✓ 要么先 `getline` 跳过一次，要么在循环中检查 `!token.empty()`

## 面试追问

**Q1: 为什么选 Trie 而不是 HashMap？**
> Trie 天然表达树形层级关系，每次操作只需沿路径走到目标节点，复杂度只和路径深度有关。HashMap 方案需要用字符串路径做 key，难以高效支持"列出子目录"这种操作（要么冗余存储，要么遍历所有 key 做前缀匹配）。

**Q2: 如果要支持 `rm` 和 `mv` 操作怎么办？**
> - `rm(path)`: 走到父节点，从 `children` 中删除目标子节点（及其整棵子树）。如果要支持递归删除需要 DFS 释放内存。
> - `mv(src, dst)`: 从源路径的父节点 `children` 中摘下子树，挂到目标路径的父节点 `children` 上。
> - Trie 方案做这些操作非常自然，HashMap 方案则需要遍历所有受影响的路径 key 来更新。

**Q3: 如果 `ls` 被高频调用，且目录下有百万级条目，怎么优化？**
> - 当前用 `std::map`，遍历所有子条目是 O(k)，这已经是最优了（必须遍历才能输出）。
> - 如果只需要分页返回（如前100个），可以利用 `map` 的有序性，用 `lower_bound` 实现分页迭代器。
> - 如果需要模糊匹配（如通配符 `*.txt`），可以额外维护按后缀分组的索引。

**Q4: 内存释放怎么处理？**
> 当前实现有内存泄漏（`new TrieNode()` 但从未 `delete`）。生产代码中可以用 `unique_ptr<TrieNode>` 替代裸指针，或者在析构函数中 DFS 释放整棵树。

## 相关题型

- **208. 实现 Trie (前缀树)** — 本题的基础版，区别：208 的 Trie 每个节点对应一个字符，本题每个节点对应一个目录/文件名（即一段路径）。掌握 208 后做本题只需把"字符级 Trie"改成"路径段级 Trie"。
- **1166. 设计文件系统** — 简化版，只有 `createPath` 和 `get`，不需要 `ls` 和文件内容。可以直接复用本题的 `traverse` 方法。
- **642. 设计搜索自动补全系统** — 同样是 Trie + 设计题，区别在于需要按热度排序返回前缀匹配结果，复用 Trie 遍历 + 排序的技巧。