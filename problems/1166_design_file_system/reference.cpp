/*
 * LeetCode 1166: 设计文件系统 (Design File System)
 * 
 * 【题目本质】
 * 设计一个路径→值的映射系统，createPath 需要验证"父路径已存在"且"当前路径不存在"。
 * 本质是在处理树形层次结构的键值存储。
 *
 * 【解法总览】
 * 解法1: HashMap — O(L) / O(总路径长) — 最简洁，面试首选
 * 解法2: Trie   — O(L) / O(总路径长) — 更贴近真实文件系统，支持扩展操作
 *
 * 其中 L 为单次操作的路径长度。
 */

// ============================================================
// 解法1: HashMap — 直接用哈希表存 path → value
// 时间: createPath O(L), get O(L)  空间: O(所有路径总长度)
//
// 【思路】
// 最直觉的想法：路径是字符串，值是整数，用 HashMap 存映射。
// 
// createPath 的关键：怎么获取父路径？
//   "/a/b/c" 的父路径是 "/a/b"
//   → 用 rfind('/') 找最后一个 '/'，截取前面部分
//   → 特殊情况："/a" 的父路径截取后是 ""（空串），
//     表示一级路径，根目录默认存在，直接放行
//
// 示例流程:
//   createPath("/leet", 1):
//     rfind('/') = 0, parent = "" → 空串放行 → 插入 {"/leet": 1}
//   createPath("/leet/code", 2):
//     rfind('/') = 5, parent = "/leet" → map 中存在 → 插入 {"/leet/code": 2}
//   createPath("/c/d", 1):
//     rfind('/') = 2, parent = "/c" → map 中不存在 → 返回 false
// ============================================================
class FileSystem1 {
    unordered_map<string, int> pathMap;
public:
    FileSystem1() {}
    
    bool createPath(string path, int value) {
        // 条件1: 路径已存在 → 返回 false
        if (pathMap.count(path)) return false;
        
        // 提取父路径: 找最后一个 '/' 的位置并截取
        // 为什么用 rfind 而不是 find？
        // find 找第一个 '/'（总是位置0），rfind 找最后一个才是父子分界点
        int lastSlash = path.rfind('/');
        string parent = path.substr(0, lastSlash);
        
        // 条件2: 父路径不存在 → 返回 false
        // 注意: parent 为空表示一级路径（如 "/a"），根目录默认存在
        if (!parent.empty() && !pathMap.count(parent)) return false;
        
        pathMap[path] = value;
        return true;
    }
    
    int get(string path) {
        auto it = pathMap.find(path);
        if (it != pathMap.end()) return it->second;
        return -1;
    }
};

// ============================================================
// 解法2: Trie（前缀树）— 用树形结构存储路径层次
// 时间: createPath O(L), get O(L)  空间: O(所有路径总长 + 节点指针开销)
//
// 【思路】
// 路径天然是树形结构:
//
//   创建 "/leet"(1), "/leet/code"(2), "/leet/problems"(3) 后:
//
//       [root]
//         |
//       "leet" (val=1)
//       /          \
//   "code" (val=2)  "problems" (val=3)
//
// 每个 Trie 节点对应路径的一个组件（两个 / 之间的字符串），
// 用 unordered_map<string, TrieNode*> children 存子节点。
//
// 与解法1的对比:
// - HashMap 把路径当扁平字符串，丢失层次关系
// - Trie 保留了父子关系，天然支持"列出子目录"等扩展操作
//
// createPath("/a/b/c", val) 的逻辑:
//   拆分路径 → ["a", "b", "c"]
//   遍历组件:
//     "a" → 必须在 root.children 中存在（父路径必须存在）
//     "b" → 必须在 a.children 中存在（父路径必须存在）
//     "c" → 必须不存在（当前路径不能已存在）→ 创建新节点，设值
// ============================================================
class FileSystem2 {
    struct TrieNode {
        unordered_map<string, TrieNode*> children;
        int value;
        TrieNode() : value(-1) {}  // -1 表示未被 createPath 设置过
    };
    
    TrieNode* root;
    
    // 将 "/a/b/c" 拆分为 ["a", "b", "c"]
    // 为什么从 i=1 开始？因为路径以 '/' 开头，第一个字符总是 '/'，跳过它
    vector<string> split(const string& path) {
        vector<string> parts;
        int i = 1;
        while (i < (int)path.size()) {
            int j = path.find('/', i);
            if (j == string::npos) j = path.size(); // 没有更多的 '/'，取到末尾
            parts.push_back(path.substr(i, j - i));
            i = j + 1;
        }
        return parts;
    }
    
public:
    FileSystem2() {
        root = new TrieNode();
    }
    
    bool createPath(string path, int value) {
        vector<string> parts = split(path);
        TrieNode* node = root;
        
        for (int i = 0; i < (int)parts.size(); i++) {
            bool isLast = (i == (int)parts.size() - 1);
            
            if (!isLast) {
                // 中间组件: 对应父路径的某一层，必须已存在
                if (!node->children.count(parts[i])) return false;
                node = node->children[parts[i]];
                // 额外检查: 该节点是否被正式创建过（value != -1）
                if (node->value == -1) return false;
            } else {
                // 最后一个组件: 当前要创建的路径，不能已存在
                if (node->children.count(parts[i]) && 
                    node->children[parts[i]]->value != -1) {
                    return false;
                }
                // 创建新节点
                if (!node->children.count(parts[i])) {
                    node->children[parts[i]] = new TrieNode();
                }
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
        return node->value;  // 如果未创建过，返回 -1（默认值）
    }
};

// ============================================================
// 【解法对比】
//
// HashMap vs Trie:
//   - 实现复杂度: HashMap 约15行核心逻辑，Trie 需要拆分路径 + 管理树节点
//   - 时间复杂度: 两者都是 O(L)
//   - 扩展性: HashMap 无法高效支持 ls（列子目录）、delete（删子树），
//             Trie 天然支持这些操作
//   - 面试策略: 先写 HashMap（快速AC），再主动提 Trie（展示设计思维）
//
// 【易错点】
//
// 1. 用 find('/') 而不是 rfind('/') 提取父路径:
//    ✗ path.find('/') 对 "/a/b/c" 返回 0，substr(0,0) = ""，永远不检查父路径
//    ✓ path.rfind('/') 返回 4，substr(0,4) = "/a/b"，正确获得父路径
//
// 2. 忘记处理一级路径:
//    ✗ 对 "/a"，parent="" 去 map 查找空串，找不到返回 false → 一级路径永远创建不了
//    ✓ 加条件 if (!parent.empty() && ...) 空串直接放行
//
// 3. Trie 的 split 漏掉最后一个组件:
//    ✗ find('/', i) 对最后一个组件返回 npos，没处理 npos → 跳过最后一个组件
//    ✓ if (j == npos) j = path.size(); → 最后一个组件正确截取
//
// 4. createPath 只检查一个失败条件:
//    ✗ 只检查了父路径不存在，忘了检查路径已存在 → 对已有路径重复创建返回 true
//    ✓ 两个条件都要检查: 已存在 → false，父不存在 → false
//
// 【面试追问】
//
// Q1(基础): "HashMap 方案中，createPath 和 get 的时间复杂度是多少？"
//   → 都是 O(L)。createPath 做一次 rfind O(L) + 一次 substr O(L) + 两次 map 查找 O(L)。
//     get 做一次 map 查找 O(L)（字符串哈希是 O(L)）。
//
// Q2(扩展): "如果要支持 delete(path) 删除路径及其所有子路径，怎么改？"
//   → HashMap: 需要遍历所有 key 检查前缀匹配，O(N·L)，很低效。
//     Trie: 找到对应节点，递归删除整棵子树，O(子树大小)，高效。
//     这就是选择 Trie 的核心理由。
//
// Q3(设计): "如果要支持 ls(path) 列出某路径下的直接子文件/子目录呢？"
//   → HashMap: 要遍历所有 key，找以 path 为前缀且只多一层的，O(N·L)。
//     Trie: 直接返回该节点的 children 的 key 列表，O(子节点数)。
//     这就是 588 题（设计内存文件系统）的做法。
//
// Q4(性能): "大量短路径 vs 少量超长路径，哪种方案更好？"
//   → 大量短路径: HashMap 的哈希冲突可能增加，但总体还行。
//     超长路径: HashMap 的字符串哈希 O(L) 开销大，且 substr 产生拷贝。
//     Trie 按组件存储，相同前缀共享节点，对超长路径更省空间。
// ============================================================
