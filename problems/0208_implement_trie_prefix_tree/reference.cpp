/*
 * LeetCode 208: 实现 Trie (前缀树)
 *
 * 【题目本质】
 * 设计一个树形数据结构，使得插入字符串、精确查找、前缀匹配
 * 三种操作都能在 O(L) 时间完成（L 为操作字符串的长度）。
 * 关键是让共享前缀的字符串共享树上的路径。
 *
 * 【解法总览】
 * 解法1: HashSet 暴力       — insert O(L), search O(L), startsWith O(N·L) — 理解题意
 * 解法2: 数组实现 Trie       — 三种操作均 O(L) / 空间 O(Σ·26) — 面试首选
 * 解法3: HashMap 实现 Trie   — 三种操作均 O(L) / 空间 O(Σ)   — 字符集大时的变体
 */

// ============================================================
// 解法1: HashSet 暴力 — 用集合存所有单词
// 时间: insert O(L), search O(L), startsWith O(N·L)
// 空间: O(N·L)，N = 单词数，L = 平均长度
//
// 【思路】
// 最直觉的做法：把所有单词丢进 unordered_set。
// search 直接查集合 O(L)，没问题。
// 但 startsWith 必须遍历所有单词逐一检查前缀，
// 这就是 O(N·L) 的瓶颈 —— 正是 Trie 要解决的问题。
// ============================================================
class Trie1 {
    unordered_set<string> words;
public:
    Trie1() {}

    void insert(string word) {
        words.insert(word);
    }

    bool search(string word) {
        return words.count(word);
    }

    bool startsWith(string prefix) {
        // 瓶颈：必须遍历所有已存储单词
        for (const string& w : words) {
            if (w.size() >= prefix.size() &&
                w.substr(0, prefix.size()) == prefix) {
                return true;
            }
        }
        return false;
    }
};


// ============================================================
// 解法2: 数组实现 Trie (面试首选) — 每个节点有 children[26]
// 时间: insert / search / startsWith 均为 O(L)
// 空间: O(Σ·26)，Σ 为所有插入字符串的总字符数
//
// 【思路】
// 解法1的瓶颈：startsWith 需要遍历所有单词。
// 如果我们把字符串按字符拆开，用树结构组织，
// 让共享前缀的字符串走同一条路径，
// 那么前缀查询只需沿路径走一遍 → O(L)。
//
// 每个节点代表一个字符位置，包含：
// - children[26]: 26 个子节点指针（对应 a~z）
// - isEnd: 是否是某个完整单词的结尾
//
// 三个操作本质上都是"沿字符路径往下走"：
// - insert: 走路径，缺节点就创建，最后标记 isEnd
// - search: 走路径，中途断了返回 false，走完检查 isEnd
// - startsWith: 走路径，中途断了返回 false，走完返回 true
//
// 插入 "apple", "app", "bat" 后的 Trie:
//
//           root
//          /    \
//         a      b
//         |      |
//         p      a
//         |      |
//         p*     t*       (* 表示 isEnd=true)
//         |
//         l
//         |
//         e*
//
// search("app")     → 走 root→a→p→p, isEnd=true  → true
// search("ap")      → 走 root→a→p,   isEnd=false → false
// startsWith("ap")  → 走 root→a→p,   路径存在    → true
// search("bat")     → 走 root→b→a→t, isEnd=true  → true
// ============================================================
class Trie {
    struct TrieNode {
        TrieNode* children[26] = {};  // C++11 值初始化，自动全为 nullptr
        bool isEnd = false;           // 是否是某个完整单词的结尾
    };

    TrieNode* root;

    // 辅助函数：沿着字符串 s 的路径走到尽头
    // 返回终点节点指针；如果中途路径断裂，返回 nullptr
    // 抽取公共逻辑，消除 search 和 startsWith 之间的代码重复
    TrieNode* traverse(const string& s) {
        TrieNode* node = root;
        for (char c : s) {
            int idx = c - 'a';  // 映射到 0~25，不能直接用 ASCII 值
            if (!node->children[idx]) return nullptr;  // 路径断了
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
            // 如果该字符对应的子节点不存在，创建新节点
            // 如果已存在，直接复用（这就是前缀共享的关键）
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        // 走完整个单词后，标记当前节点为单词结尾
        // 这一行不能忘！否则 search 永远返回 false
        node->isEnd = true;
    }

    bool search(string word) {
        TrieNode* node = traverse(word);
        // 两个条件缺一不可：路径存在 且 是完整单词的结尾
        return node != nullptr && node->isEnd;
    }

    bool startsWith(string prefix) {
        // 与 search 的唯一区别：只要路径存在就行，不需要 isEnd
        return traverse(prefix) != nullptr;
    }
};


// ============================================================
// 解法3: HashMap 实现 Trie — 每个节点用 unordered_map 存子节点
// 时间: insert / search / startsWith 均为 O(L)
// 空间: O(Σ)，只存实际存在的子节点，比数组版更节省空间
//
// 【思路】
// 与解法2的唯一区别：把 children[26] 换成 unordered_map<char, TrieNode*>。
// 
// 优点：当字符集很大（如 Unicode）时，不需要每个节点开巨大数组。
//       即使字符集是 26 个字母，如果 Trie 很稀疏（大部分节点只有 1-2 个子节点），
//       HashMap 版本实际内存占用更少。
//
// 缺点：哈希表有常数开销（哈希计算、桶分配），
//       对于小字符集场景（26字母），数组版通常更快。
//
// 面试中：先写数组版（解法2），面试官追问"字符集很大怎么办"时提出此方案。
// ============================================================
class Trie3 {
    struct TrieNode {
        unordered_map<char, TrieNode*> children;
        bool isEnd = false;
    };

    TrieNode* root;

public:
    Trie3() : root(new TrieNode()) {}

    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            // count 检查 key 是否存在，不存在则创建
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


// ============================================================
// 【解法对比】
//
// | 解法              | insert | search | startsWith | 空间     | 适用场景            |
// |-------------------|--------|--------|------------|----------|---------------------|
// | HashSet 暴力      | O(L)   | O(L)   | O(N·L)    | O(N·L)   | 不需要前缀查询      |
// | 数组 Trie (解法2) | O(L)   | O(L)   | O(L)      | O(Σ·26)  | 字符集小(26字母)    |
// | HashMap Trie(解法3)| O(L)  | O(L)   | O(L)      | O(Σ)     | 字符集大(Unicode)   |
//
// 面试首选解法2（数组版），简洁高效，面试官最常见期望。
// 解法2和解法3的核心逻辑完全相同，只是子节点存储方式不同。
//
// ============================================================
// 【易错点】
//
// 1. children 数组未初始化:
//    ✗ TrieNode* children[26]; // 不初始化 → 野指针
//    ✓ TrieNode* children[26] = {}; // 值初始化全为 nullptr
//    或在构造函数中 memset/循环初始化。不初始化会导致
//    判断 !node->children[idx] 时访问随机内存。
//
// 2. search 和 startsWith 返回值混淆:
//    ✗ search 最后 return true; // 应该 return node->isEnd;
//    insert("apple") 后 search("app") 应返回 false，
//    因为 "app" 路径上的最后一个 'p' 的 isEnd=false。
//    只有 startsWith("app") 才返回 true。
//
// 3. insert 结尾忘记标记 isEnd:
//    ✗ 只创建路径，不设 node->isEnd = true
//    ✓ 遍历完 word 的所有字符后，必须 node->isEnd = true
//    否则 search 永远返回 false。
//
// 4. 字符到索引的映射错误:
//    ✗ int idx = c; // ASCII 值 97~122，越界！
//    ✓ int idx = c - 'a'; // 映射到 0~25
//
// ============================================================
// 【面试追问】（递进链）
//
// Q1（基础理解）: search 和 startsWith 几乎相同，怎么消除重复？
// → 抽取 traverse() 辅助函数，返回路径终点。
//   search: node && node->isEnd
//   startsWith: node != nullptr
//
// Q2（功能扩展）: 如何支持 delete(word) 操作？
// → 方案A: 懒删除——只把 isEnd 改为 false，简单但不释放空间。
//   方案B: 加引用计数，delete 时沿路径递减，计数为 0 则删节点。
//   方案C: 递归删除——DFS 到单词末尾，回溯时如果节点无其他分支则删除。
//
// Q3（通配符）: 支持 '.' 匹配任意字符，怎么改 search？
// → 即 LeetCode 211。遇到 '.' 时对所有非空子节点递归搜索，
//   本质是 DFS/回溯。最坏 O(26^L)，但有 Trie 结构剪枝。
//
// Q4（内存优化）: Trie 浪费内存怎么办？
// → (1) 用 HashMap 代替数组（解法3）
//   (2) 路径压缩：连续的单分支节点合并为一个（压缩 Trie / Radix Tree）
//   (3) 对象池：预分配大数组，避免频繁 new
//
// ============================================================
