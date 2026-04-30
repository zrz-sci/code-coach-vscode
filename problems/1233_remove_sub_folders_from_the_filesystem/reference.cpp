/*
 * LeetCode 1233: Remove Sub-Folders from the Filesystem
 * 
 * 【题目本质】
 * 给定一组文件夹路径，找出所有"根文件夹"——即不被任何其他路径包含的路径。
 * 判断"包含"时必须以 '/' 为分隔（"/a/b" 是 "/a" 的子文件夹，但 "/a/bc" 不是）。
 *
 * 【解法总览】
 * 解法1: 排序 + 线性扫描  — O(n·L·log n) / O(排序)  — 面试首选 ⭐
 * 解法2: HashSet 逐级检查  — O(n·L²) / O(n·L)       — 最直观
 * 解法3: Trie + DFS        — O(n·L) / O(n·L)         — 理论最优 / 工程最灵活
 */

// ============================================================
// 解法1: 排序 + 线性扫描 — 面试首选 ⭐
// 时间: O(n·L·log n)  空间: O(排序开销)
//
// 【思路】
// 核心观察：字典序排序后，父文件夹一定排在它所有子文件夹的前面。
// 例如排序后: "/a", "/a/b", "/a/b/c", "/a/bc", "/c/d", "/c/d/e", "/c/f"
//
// 我们只需维护"上一个被保留的父路径"，每个新路径与之比较：
// - 如果是它的子文件夹 → 跳过
// - 否则 → 加入结果，成为新的"父路径"
//
// 为什么字典序能保证这一点？
// 因为 "/a" 是 "/a/..." 的前缀，字典序中前缀 < 扩展串。
// 所以 "/a" 之后紧接着的就是所有以 "/a/" 开头的子文件夹。
//
// 判断子文件夹的三个条件（缺一不可）:
// 1. folder[i].size() > parent.size()        (子比父长)
// 2. folder[i] 以 parent 开头                 (前缀匹配)
// 3. folder[i][parent.size()] == '/'          (分隔符检查，防止 "/a/bc" 误判)
// ============================================================
class Solution1 {
public:
    vector<string> removeSubfolders(vector<string>& folder) {
        sort(folder.begin(), folder.end());
        
        vector<string> result;
        // 排序后第一个路径一定不是任何路径的子文件夹
        result.push_back(folder[0]);
        
        for (int i = 1; i < (int)folder.size(); i++) {
            const string& parent = result.back();
            
            // 检查 folder[i] 是否是 parent 的子文件夹
            // 条件: folder[i] 以 "parent/" 为前缀
            bool isSub = folder[i].size() > parent.size()
                && folder[i].compare(0, parent.size(), parent) == 0
                // 关键: 紧跟 parent 之后必须是 '/'
                // 这防止了 "/a/bc" 被误判为 "/a/b" 的子文件夹
                && folder[i][parent.size()] == '/';
            
            if (!isSub) {
                result.push_back(folder[i]);
            }
        }
        
        return result;
    }
};


// ============================================================
// 解法2: HashSet 逐级检查前缀
// 时间: O(n·L²)  空间: O(n·L)
//
// 【思路】
// 换个角度思考：对于每个路径，问自己"它的任何祖先路径是否也在列表中？"
// 
// 做法：
// 1. 把所有路径放入 HashSet
// 2. 对每个路径，逐个字符扫描，每遇到 '/' 就截取前缀检查
//    - 例如 "/a/b/c"：检查 "/a" → 检查 "/a/b"
//    - 如果任一前缀在集合中 → 是子文件夹，跳过
//
// 为什么只在 '/' 处检查？
// 因为子文件夹关系必须以完整路径段为单位。
// "/a/bc" 的前缀 "/a/b" 虽然是字符串前缀，但 "/a/b" 不是 "/a/bc" 的父路径。
// 只有在 '/' 处截取才能保证是完整的路径。
//
// 时间复杂度分析: 对每个路径(长度L), 最多检查O(L)个前缀, 
// 每个前缀的substr操作O(L), 所以单个路径O(L²), 总共O(n·L²)
// ============================================================
class Solution2 {
public:
    vector<string> removeSubfolders(vector<string>& folder) {
        unordered_set<string> folderSet(folder.begin(), folder.end());
        
        vector<string> result;
        for (const string& f : folder) {
            bool isSubfolder = false;
            
            // 从 i=1 开始（跳过开头的 '/'）
            for (int i = 1; i < (int)f.size(); i++) {
                if (f[i] == '/') {
                    // f[0..i-1] 是一个完整的祖先路径
                    // 例如 f="/a/b/c", i=2 时 prefix="/a", i=4 时 prefix="/a/b"
                    string prefix = f.substr(0, i);
                    if (folderSet.count(prefix)) {
                        isSubfolder = true;
                        break; // 找到一个祖先就够了
                    }
                }
            }
            
            if (!isSubfolder) {
                result.push_back(f);
            }
        }
        
        return result;
    }
};


// ============================================================
// 解法3: Trie（前缀树）+ DFS
// 时间: O(n·L)  空间: O(n·L)
//
// 【思路】
// 文件系统本身就是一棵树！我们按 '/' 分割路径段来建 Trie。
//
// 重点：Trie 的每条边代表一个**路径段**（如 "a", "b", "cd"），
// 而不是单个字符。如果按字符建 Trie，"/a/b" 和 "/a/bc" 会
// 共享前缀，导致误判。
//
// Trie 结构示意（示例1: ["/a","/a/b","/c/d","/c/d/e","/c/f"]）:
//
//           (root)
//          /      \
//        "a"      "c"
//        [✓]     /    \
//         |    "d"    "f"
//        "b"   [✓]   [✓]
//        [✓]    |
//              "e"
//              [✓]
//
// DFS 遍历策略:
// → 到 "a", isEnd=true → 收集 "/a", 停止向下（"b" 被跳过）
// → 到 "c", isEnd=false → 继续
//   → 到 "d", isEnd=true → 收集 "/c/d", 停止向下（"e" 被跳过）
//   → 到 "f", isEnd=true → 收集 "/c/f"
// 
// 结果: ["/a", "/c/d", "/c/f"] ✓
//
// 为什么遇到 isEnd 就停止？
// 因为如果一个节点标记为 isEnd（该路径存在于列表中），
// 它下面所有的节点对应的路径都是它的子文件夹，应该被删除。
// ============================================================
class Solution3 {
public:
    struct TrieNode {
        unordered_map<string, TrieNode*> children;
        bool isEnd = false;
    };
    
    vector<string> removeSubfolders(vector<string>& folder) {
        TrieNode* root = new TrieNode();
        
        // 第一步：将所有路径插入 Trie
        for (const string& f : folder) {
            TrieNode* node = root;
            int i = 1; // 跳过开头的 '/'
            while (i < (int)f.size()) {
                // 找到当前路径段的结束位置
                int j = i;
                while (j < (int)f.size() && f[j] != '/') j++;
                string segment = f.substr(i, j - i);
                
                if (!node->children.count(segment)) {
                    node->children[segment] = new TrieNode();
                }
                node = node->children[segment];
                i = j + 1; // 跳过 '/'
            }
            node->isEnd = true; // 标记完整路径
        }
        
        // 第二步：DFS 收集结果
        vector<string> result;
        string path;
        dfs(root, path, result);
        return result;
    }
    
private:
    void dfs(TrieNode* node, string& path, vector<string>& result) {
        // 如果当前节点标记为一个完整路径，收集它并停止向下
        // 因为下面所有的都是子文件夹
        if (node->isEnd) {
            result.push_back(path);
            return; // 关键：不再递归子节点
        }
        
        for (auto& [segment, child] : node->children) {
            // 构建路径：加上 "/" + 当前段
            path += "/" + segment;
            dfs(child, path, result);
            // 回溯：移除刚加上的 "/" + segment
            path.erase(path.size() - segment.size() - 1);
        }
    }
};


// ============================================================
// 【解法对比】
//
// | 解法           | 时间            | 空间    | 代码量 | 适用场景        |
// |----------------|-----------------|---------|--------|-----------------|
// | 排序+线性扫描  | O(n·L·log n)    | O(排序) | 最少   | 面试首选 ⭐     |
// | HashSet        | O(n·L²)         | O(n·L)  | 适中   | 思路最直观      |
// | Trie + DFS     | O(n·L)          | O(n·L)  | 最多   | 动态查询/工程   |
//
// - 排序法: 代码最简洁, 面试中最推荐。缺点是需要排序(修改原数组)。
// - HashSet: 不需要排序, 但 substr 导致额外开销。
// - Trie: 理论最优, 且支持动态添加删除, 但实现最复杂。
//
// ============================================================
// 【易错点】
//
// 1. ✗ 只用 startsWith 判断, 忘记检查 '/' 分隔符:
//      if (folder[i].find(parent) == 0) → "/a/bc" 被误判为 "/a/b" 的子文件夹
//    ✓ 必须加 folder[i][parent.size()] == '/'
//
// 2. ✗ 排序法中循环从 i=0 开始, result 为空时 result.back() 崩溃
//    ✓ 先把 folder[0] 加入结果, 从 i=1 开始
//
// 3. ✗ HashSet 方法中对路径 "/a" 检查前缀, i 从 0 开始:
//      i=0 时 f[0]='/', substr(0,0)="" 空串查找无意义
//    ✓ i 从 1 开始, 只在 f[i]=='/' 时检查
//
// 4. ✗ Trie 按单个字符建树:
//      "/a/b" 和 "/a/bc" 共享前缀 "a","b", 
//      "/a/b" 标记 isEnd 后 "/a/bc" 被误判为子文件夹
//    ✓ 按 '/' 分割后以路径段(如 "a","b","bc")为单位建 Trie
//
// 5. ✗ Trie 回溯时 erase 长度算错:
//      path += "/" + segment; 加了 segment.size()+1 个字符
//      path.erase(path.size() - segment.size()) 少删了 '/'
//    ✓ path.erase(path.size() - segment.size() - 1)
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1(基础): 暴力法是 O(n²·L), 能过吗？怎么优化到 O(n log n · L)?
//    → n=4×10⁴, 暴力 ~1.6×10¹¹ 超时。排序后线性扫描降到 O(n·L·log n)。
//
// Q2(深入): 排序法中为什么字典序排序能保证父文件夹在前？
//    → 前缀在字典序中 < 扩展串。所有以 "/a/" 开头的路径紧跟 "/a" 后面,
//      只需和 result.back() 比较, 不需要回头检查。
//
// Q3(变体): 如果文件夹列表是动态的(可增删), 还能用排序法吗？
//    → 不适合。应该用 Trie: 添加时检查祖先是否已 isEnd,
//      删除时直接在 Trie 中定位节点并移除子树。
//
// Q4(扩展): 能否 O(n·L) 时间不额外建 Trie？
//    → HashSet 方法接近, 但 substr 导致 O(L²)。如果用 string_view
//      避免拷贝, 可以做到 O(n·L) (C++17 的 string_view 哈希)。
// ============================================================
