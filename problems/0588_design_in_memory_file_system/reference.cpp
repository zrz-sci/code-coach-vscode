/*
 * LeetCode 588: 设计内存文件系统 (Design In-Memory File System)
 *
 * 【题目本质】
 * 设计一棵多叉 Trie 树来模拟文件系统的层级结构。
 * 每个节点代表一个目录或文件，所有操作都是"沿路径遍历到目标节点"。
 *
 * 【解法总览】
 * 解法1: 纯 HashMap — 两个 map 分别存目录和文件，靠字符串路径关联
 * 解法2: Trie 树   — 显式建树，节点间有父子关系 — ⭐ 面试首选
 *
 * 两种解法时间复杂度相同（单次操作 O(m + k·log k)，m=路径深度，k=子条目数），
 * 但 Trie 方案更直观、更易扩展（支持 rm/mv 等操作）。
 */

/*
 * 文件系统的 Trie 结构示意:
 *
 *       root (/)
 *        |
 *        a          ← 目录节点 (isFile=false)
 *        |
 *        b          ← 目录节点
 *       / \
 *      c   e.txt    ← c 是目录, e.txt 是文件 (isFile=true, content="world")
 *      |
 *     d.txt         ← 文件节点 (isFile=true, content="hello")
 *
 * ls("/")           → ["a"]         (根目录只有一个子条目 a)
 * ls("/a/b")        → ["c", "e.txt"] (目录 b 下有 c 和 e.txt，字典序)
 * ls("/a/b/c/d.txt") → ["d.txt"]    (文件路径，返回文件名本身)
 * readContentFromFile("/a/b/c/d.txt") → "hello"
 */

// ============================================================
// 解法1: 纯 HashMap 方案
// 时间: O(m + k·log k) 每次操作，m=路径深度，k=子条目数
// 空间: O(总路径数 + 文件内容总长)
//
// 【思路】
// 不建树，用两个 map 直接管理：
//   dirs[path] = set<子条目名>    存储目录下的子条目
//   files[filePath] = 文件内容    存储文件内容
//
// 优点：实现简单。
// 缺点：目录和文件的关系靠字符串路径维护，难以扩展（如删除、移动）。
// ============================================================
class FileSystem1 {
    // dirs["/a/b"] = {"c", "d.txt"} 表示 /a/b 目录下有 c 和 d.txt
    unordered_map<string, set<string>> dirs;
    // files["/a/b/c/d.txt"] = "hello" 表示文件内容
    unordered_map<string, string> files;

public:
    FileSystem1() {
        dirs["/"] = {};  // 初始化根目录为空
    }

    vector<string> ls(string path) {
        // 情况1: path 是文件 → 返回文件名
        if (files.count(path)) {
            int pos = path.find_last_of('/');
            return {path.substr(pos + 1)};
        }
        // 情况2: path 是目录 → 返回子条目列表（set 天然有序）
        return vector<string>(dirs[path].begin(), dirs[path].end());
    }

    void mkdir(string path) {
        string cur = "";
        istringstream ss(path);
        string name;
        getline(ss, name, '/');  // 跳过首个空串（路径以 / 开头）
        while (getline(ss, name, '/')) {
            // 确定父目录路径
            string parent = cur.empty() ? "/" : cur;
            cur += "/" + name;
            // 在父目录的子条目集合中注册当前目录名
            dirs[parent].insert(name);
            // 确保当前目录也有条目记录（即使暂时为空）
            if (!dirs.count(cur)) dirs[cur] = {};
        }
    }

    void addContentToFile(string filePath, string content) {
        // 追加内容到文件（文件不存在则 files[filePath] 初始化为空串再追加）
        files[filePath] += content;
        // 将文件名注册到父目录的子条目集合中
        int pos = filePath.find_last_of('/');
        string parentDir = (pos == 0) ? "/" : filePath.substr(0, pos);
        string fileName = filePath.substr(pos + 1);
        dirs[parentDir].insert(fileName);
    }

    string readContentFromFile(string filePath) {
        return files[filePath];
    }
};


// ============================================================
// 解法2: Trie 树（多叉树）方案 — ⭐ 面试首选
// 时间: O(m + k·log k) 每次操作，m=路径深度，k=子条目数
// 空间: O(总节点数 + 文件内容总长)
//
// 【思路】
// 文件系统天然是树形结构，直接用 Trie 树建模：
//   - 每个 TrieNode 代表一个目录或文件
//   - children 用 map<string, TrieNode*> 存子节点（map 有序 → ls 天然排序）
//   - isFile 标记是否为文件，content 存文件内容
//
// 所有操作的核心都是 traverse()：沿路径从根走到目标节点。
//   - ls:       走到节点 → 如果是文件返回文件名，否则遍历 children
//   - mkdir:    走到节点 → 沿途 create=true 自动创建不存在的中间目录
//   - addContent: 走到节点 → 标记 isFile，追加 content
//   - readContent: 走到节点 → 返回 content
//
// 为什么比 HashMap 方案好：
//   1. 树结构直观对应文件系统层级，代码更清晰
//   2. 删除/移动操作只需操作子树，不用遍历所有路径 key
//   3. 共享路径前缀，内存更紧凑
// ============================================================
class FileSystem {
    struct TrieNode {
        // 用 map（有序）而非 unordered_map：ls 遍历时天然字典序
        map<string, TrieNode*> children;
        bool isFile = false;
        string content = "";
    };

    TrieNode* root;

    // 路径解析: "/a/b/c" → ["a","b","c"], "/" → []
    // 为什么要单独抽出来：所有操作都需要解析路径，避免代码重复
    vector<string> parsePath(const string& path) {
        vector<string> parts;
        istringstream ss(path);
        string token;
        getline(ss, token, '/');  // 跳过开头空串（路径以 '/' 开头）
        while (getline(ss, token, '/')) {
            if (!token.empty()) parts.push_back(token);
        }
        return parts;
    }

    // 沿路径遍历到目标节点
    // create=true 时，沿途自动创建不存在的中间节点（用于 mkdir 和 addContent）
    TrieNode* traverse(const string& path, bool create = false) {
        vector<string> parts = parsePath(path);
        TrieNode* cur = root;
        for (const string& part : parts) {
            if (!cur->children.count(part)) {
                if (create) {
                    cur->children[part] = new TrieNode();
                } else {
                    return nullptr;
                }
            }
            cur = cur->children[part];
        }
        return cur;
    }

public:
    FileSystem() {
        root = new TrieNode();
    }

    vector<string> ls(string path) {
        TrieNode* node = traverse(path);

        // 如果是文件路径，返回文件名本身
        if (node->isFile) {
            vector<string> parts = parsePath(path);
            return {parts.back()};
        }

        // 如果是目录路径，返回所有子条目名
        // map 的遍历顺序就是字典序，无需额外排序
        vector<string> result;
        for (auto& [name, child] : node->children) {
            result.push_back(name);
        }
        return result;
    }

    void mkdir(string path) {
        // 沿路径走，create=true 自动创建所有中间目录
        // 这一行就完成了"递归创建"的全部工作
        traverse(path, true);
    }

    void addContentToFile(string filePath, string content) {
        // 走到文件节点（如果不存在则沿途创建）
        TrieNode* node = traverse(filePath, true);
        // 两步缺一不可：标记为文件 + 追加内容
        node->isFile = true;
        node->content += content;
    }

    string readContentFromFile(string filePath) {
        TrieNode* node = traverse(filePath);
        return node->content;
    }
};


/*
 * ============================================================
 * 【操作过程示意】
 *
 * 操作序列: FileSystem(), ls("/"), mkdir("/a/b/c"),
 *           addContentToFile("/a/b/c/d", "hello"), ls("/"),
 *           readContentFromFile("/a/b/c/d")
 *
 * Step1: FileSystem()
 *   root(/)        ← 空根节点
 *
 * Step2: ls("/")
 *   root 没有 children → 返回 []
 *
 * Step3: mkdir("/a/b/c")
 *   parsePath → ["a", "b", "c"]
 *   traverse(create=true):
 *     root → 创建 a → 创建 b → 创建 c
 *
 *       root(/)
 *        |
 *        a
 *        |
 *        b
 *        |
 *        c
 *
 * Step4: addContentToFile("/a/b/c/d", "hello")
 *   parsePath → ["a", "b", "c", "d"]
 *   traverse(create=true):
 *     root → a → b → c → 创建 d
 *   node->isFile = true, node->content = "hello"
 *
 *       root(/)
 *        |
 *        a
 *        |
 *        b
 *        |
 *        c
 *        |
 *       d (file, "hello")
 *
 * Step5: ls("/")
 *   root 的 children 有 {"a": ...} → 返回 ["a"]
 *
 * Step6: readContentFromFile("/a/b/c/d")
 *   traverse 走到 d 节点 → 返回 "hello"
 *
 * ============================================================
 * 【解法对比】
 *
 * HashMap 方案:
 *   + 实现简单，不需要定义节点结构
 *   - 目录和文件关系靠字符串路径维护，难以扩展
 *   - 删除/移动操作需遍历所有受影响的路径 key
 *
 * Trie 方案 (⭐推荐):
 *   + 树形结构直观对应文件系统层级
 *   + traverse 方法统一所有操作，代码清晰
 *   + 删除/移动只需操作子树引用
 *   + map 存 children → ls 天然有序
 *   - 需要定义 TrieNode 结构（但这恰好体现了设计能力）
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 根目录路径解析:
 *    ✗ parsePath("/") 返回 [""] → 在 Trie 中创建名为 "" 的子节点
 *    ✓ parsePath("/") 应返回 []，traverse 直接返回 root
 *
 * 2. ls 对文件路径的处理:
 *    ✗ if (!node->isFile) { ... } else { 遍历 children }
 *       — 逻辑反了，文件时应返回文件名，不是遍历 children
 *    ✓ if (node->isFile) return {parts.back()};
 *
 * 3. addContentToFile 漏掉 isFile 标记:
 *    ✗ node->content += content;  // 忘记 node->isFile = true
 *       — 导致后续 ls 将文件当成目录处理
 *    ✓ node->isFile = true; node->content += content;
 *
 * 4. ls 根目录无子条目时:
 *    ✗ 返回 {"/"}  或 {""}
 *    ✓ 返回空 vector<string>{}
 *
 * 5. 内存泄漏:
 *    ✗ 只 new 不 delete，面试中可能被追问
 *    ✓ 用 unique_ptr<TrieNode> 或在析构函数中 DFS 释放
 *
 * ============================================================
 * 【面试追问 — 递进链】
 *
 * Q1: 为什么用 Trie 而不是 HashMap？
 *     → Trie 天然表达树形层级，所有操作统一为 traverse + 节点操作。
 *       HashMap 需要手动维护路径间的父子关系，难以扩展。
 *
 * Q2: 如何支持 rm(path) 和 mv(src, dst)？
 *     → rm: 走到父节点，children.erase(目标名)（子树自动断开）。
 *       mv: 从源父节点摘下子树，挂到目标父节点。
 *       Trie 方案做这些只需操作引用，HashMap 要遍历所有路径 key。
 *
 * Q3: 如果 ls 被高频调用且子条目极多，怎么优化？
 *     → 当前 map 遍历已是 O(k)。若需分页，可利用 map 的 lower_bound
 *       实现游标分页。若需模糊匹配，可额外维护按后缀分组的索引。
 *
 * Q4: 当前实现有内存泄漏，怎么修？
 *     → 方案1: TrieNode 的 children 改用 map<string, unique_ptr<TrieNode>>
 *     → 方案2: 在 FileSystem 析构函数中 DFS 释放整棵树
 *
 * Q5: 如何支持并发访问（多线程安全）？
 *     → 读写锁：ls/readContent 加读锁，mkdir/addContent 加写锁。
 *       或者更细粒度：每个 TrieNode 一把锁，只锁当前操作路径上的节点。
 * ============================================================
 */
