# 1233. 删除子文件夹

## 核心思路

本质上是：给定一组路径字符串，找出所有"祖先路径"——即那些不是任何其他路径的子路径的路径。关键难点在于正确判断"子文件夹"关系（必须以 `/` 分隔，`/a/b` 是 `/a` 的子文件夹，但 `/a/bc` 不是）。

## 思维链

1. **读完题第一反应**：对于每个文件夹，检查它是否是列表中某个其他文件夹的子文件夹。最暴力的做法是两两比较——对每对 (i, j)，检查 folder[i] 是否以 `folder[j] + "/"` 开头。
2. **暴力解的瓶颈**：两两比较是 O(n²)，每次比较字符串又是 O(L)，总共 O(n²·L)。当 n 达到 4×10⁴ 时可能超时。
3. **关键观察**：如果我们把文件夹**按字典序排序**，父文件夹一定排在它所有子文件夹的前面（因为 `/a` < `/a/b` < `/a/b/c`）。排序后，只需要线性扫描，用一个"当前父文件夹"来过滤子文件夹。
4. **另一个方向**：文件夹路径天然是树形结构，可以用 **Trie（前缀树）** 按 `/` 分割路径段来建树。在 Trie 中标记"终止节点"后，DFS 搜索时遇到终止节点就不再向下——其子节点全是子文件夹。
5. **还有一个方向**：用 **HashSet** 存所有路径，对每个路径逐级检查它的所有前缀是否在集合中。如果某个前缀存在，说明它是子文件夹。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 + 线性扫描 | 排序后父文件夹在前，逐个比较前缀 | O(n·L·log n) | O(排序) | ⭐ 必须写出 |
| HashSet 逐级检查前缀 | 对每个路径检查所有祖先是否在集合中 | O(n·L²) | O(n·L) | 能说出即可 |
| Trie（前缀树） | 按路径段建 Trie，DFS 收集非子文件夹 | O(n·L) | O(n·L) | 加分项 |

> n = folder.length, L = 路径平均长度

## 关键提示

1. **排序的妙用**：排序后 `"/a"` 一定在 `"/a/b"` 前面，但 `"/a/bc"` 也在 `"/a/b"` 后面。判断子文件夹时不能只看 `startsWith`，还要检查父路径后面紧跟的是 `/`。
2. **易混淆的边界**：`"/a/b"` 是 `"/a"` 的子文件夹，但 `"/a/bc"` **不是** `"/a/b"` 的子文件夹。关键区分点是 `/` 字符。
3. **Trie 的分割粒度**：不要按字符建 Trie（那样 `/a/bc` 和 `/a/b` 会共享前缀导致误判），应该按 `/` 分割后按**路径段**建 Trie。
4. **HashSet 方法**：对路径 `"/a/b/c"`，依次检查 `"/a"` 和 `"/a/b"` 是否在集合中。找到任一存在即可判定为子文件夹。
5. 示例 3 是关键测试用例：`"/a/b/c"` 和 `"/a/b/ca"` 不构成父子关系！

```
排序后的顺序示意:
  "/a"          ← 父
  "/a/b"        ← "/a" 的子文件夹，跳过
  "/a/b/c"      ← "/a" 的子文件夹，跳过
  "/a/bc"       ← 不是 "/a/b" 的子！因为 'c' ≠ '/'
  "/c/d"        ← 新的父
  "/c/d/e"      ← "/c/d" 的子文件夹，跳过
  "/c/f"        ← 不是 "/c/d" 的子（"f" ≠ "d/..."）
```

## 解法详解

### 解法1: 排序 + 线性扫描 — O(n·L·log n) / O(排序) ⭐ 面试首选

**思考过程**: 
如果文件夹列表是有序的，父文件夹一定出现在它所有子文件夹之前。我们只需要维护"上一个被保留的父文件夹"，每遇到新路径就检查它是否是当前父文件夹的子文件夹。如果是就跳过，如果不是就把它作为新的父文件夹加入结果。

判断子文件夹的条件：`folder[i]` 以 `lastParent + "/"` 开头。

```cpp
class Solution {
public:
    vector<string> removeSubfolders(vector<string>& folder) {
        // 排序：父文件夹一定排在子文件夹前面
        sort(folder.begin(), folder.end());
        
        vector<string> result;
        result.push_back(folder[0]); // 排序后第一个一定不是任何路径的子文件夹
        
        for (int i = 1; i < folder.size(); i++) {
            const string& lastParent = result.back();
            // 判断 folder[i] 是否是 lastParent 的子文件夹:
            // 条件1: folder[i] 以 lastParent 开头
            // 条件2: lastParent 之后紧跟 '/'（防止 "/a/b" 误判 "/a/bc"）
            if (folder[i].size() > lastParent.size()
                && folder[i].substr(0, lastParent.size()) == lastParent
                && folder[i][lastParent.size()] == '/') {
                // 是子文件夹，跳过
                continue;
            }
            result.push_back(folder[i]);
        }
        
        return result;
    }
};
```

**关键点**: 
- 判断子文件夹时 `folder[i][lastParent.size()] == '/'` 这个检查至关重要，否则 `"/a/bc"` 会被误认为 `"/a/b"` 的子文件夹。
- 可以用 `compare` 或 `rfind` 替代 `substr` 以减少不必要的拷贝，但面试中清晰度更重要。

### 解法2: HashSet 逐级检查前缀 — O(n·L²) / O(n·L)

**思考过程**: 
换个角度：对于每个路径，我要回答"它的任何祖先路径是否也在列表中？"这本质上是一个**查找问题**。把所有路径放入 HashSet，然后对每个路径，逐级截取前缀检查。

例如对 `"/a/b/c"`，检查 `"/a"` 和 `"/a/b"` 是否在集合中。

```cpp
class Solution {
public:
    vector<string> removeSubfolders(vector<string>& folder) {
        unordered_set<string> folderSet(folder.begin(), folder.end());
        
        vector<string> result;
        for (const string& f : folder) {
            bool isSubfolder = false;
            // 逐个字符扫描，遇到 '/' 就检查前缀是否在集合中
            for (int i = 1; i < f.size(); i++) { // 从1开始跳过开头的 '/'
                if (f[i] == '/') {
                    // f[0..i-1] 是一个前缀路径
                    string prefix = f.substr(0, i);
                    if (folderSet.count(prefix)) {
                        isSubfolder = true;
                        break;
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
```

**关键点**: 
- 只在遇到 `/` 时才检查前缀，这保证了我们检查的是完整的路径段而非任意前缀。
- 内层循环从 `i=1` 开始，因为 `i=0` 处是开头的 `/`，`f.substr(0,0)` 是空串无意义。

### 解法3: Trie（前缀树）— O(n·L) / O(n·L)

**思考过程**: 
文件系统本身就是一棵树！我们按 `/` 分割路径段，每段作为 Trie 的一个节点。插入所有路径后，DFS 遍历 Trie：一旦遇到标记为"终止"的节点（即该路径存在于列表中），就收集它并停止向下——其所有后代都是子文件夹。

```
Trie 结构示意（示例1）:
           (root)
          /      \
        "a"      "c"
        [✓]     /    \
        / \   "d"    "f"
      "b"    [✓]    [✓]
      [✓]     |
             "e"
             [✓]

DFS 遍历:
→ 到 "a" 发现 isEnd=true，收集 "/a"，不再向下（"b" 被过滤）
→ 到 "c" 不是终止，继续
  → 到 "d" 发现 isEnd=true，收集 "/c/d"，不再向下（"e" 被过滤）
  → 到 "f" 发现 isEnd=true，收集 "/c/f"
结果: ["/a", "/c/d", "/c/f"]
```

```cpp
class Solution {
public:
    struct TrieNode {
        unordered_map<string, TrieNode*> children;
        bool isEnd = false; // 标记此处是否有一个完整的文件夹路径
    };
    
    vector<string> removeSubfolders(vector<string>& folder) {
        TrieNode* root = new TrieNode();
        
        // 1. 将所有路径插入 Trie
        for (const string& f : folder) {
            TrieNode* node = root;
            // 按 '/' 分割路径段（跳过开头的 '/'）
            int i = 1; // 跳过第一个 '/'
            while (i < f.size()) {
                int j = i;
                while (j < f.size() && f[j] != '/') j++;
                string segment = f.substr(i, j - i);
                if (!node->children.count(segment)) {
                    node->children[segment] = new TrieNode();
                }
                node = node->children[segment];
                i = j + 1;
            }
            node->isEnd = true;
        }
        
        // 2. DFS 遍历 Trie，遇到 isEnd 就收集路径并停止向下
        vector<string> result;
        string path;
        dfs(root, path, result);
        return result;
    }
    
private:
    void dfs(TrieNode* node, string& path, vector<string>& result) {
        if (node->isEnd) {
            // 当前路径是一个存在的文件夹，收集它
            // 不再向下递归——所有后代都是子文件夹
            result.push_back(path);
            return;
        }
        for (auto& [segment, child] : node->children) {
            path += "/" + segment;
            dfs(child, path, result);
            // 回溯：恢复 path
            path.erase(path.size() - segment.size() - 1); // 删掉 "/" + segment
        }
    }
};
```

**关键点**: 
- Trie 的每个边代表一个**路径段**（如 `"a"`, `"b"`），不是单个字符。
- DFS 中遇到 `isEnd=true` 就停止向下是核心逻辑——这正是"删除子文件夹"的语义。
- 回溯时 `path.erase` 的长度是 `segment.size() + 1`（+1 是前面的 `/`）。

## 解法对比

| | 排序 + 线性扫描 | HashSet | Trie |
|---|---|---|---|
| 时间 | O(n·L·log n) | O(n·L²) | O(n·L) |
| 空间 | O(排序) | O(n·L) | O(n·L) |
| 代码量 | 最少 | 适中 | 最多 |
| 面试推荐 | ⭐ 首选 | 备选 | 展示深度 |
| 核心操作 | 排序 + 前缀比较 | 集合查找 | 建树 + DFS |

**怎么选？**
- **面试**：排序解法代码最短、最不容易出错，是首选。
- **工程场景**：如果需要多次查询（动态添加/删除文件夹），Trie 更灵活。
- **HashSet**：思路最直观，但由于 `substr` 操作，实际性能不如排序法。

## 易错点

1. **✗ 只用 `startsWith` 判断子文件夹**
   ```cpp
   // 错误："/a/bc".startsWith("/a/b") == true，但 "/a/bc" 不是 "/a/b" 的子文件夹！
   if (folder[i].find(lastParent) == 0) { ... }
   ```
   **✓ 必须额外检查分隔符 `/`**
   ```cpp
   if (folder[i].find(lastParent) == 0 && folder[i][lastParent.size()] == '/') { ... }
   ```

2. **✗ 排序解法中忘记第一个元素一定要加入结果**
   ```cpp
   // 错误：从 i=0 开始比较，但 result 为空，result.back() 会崩溃
   for (int i = 0; i < folder.size(); i++) {
       if (/*...*/ result.back() /*...*/) { ... }
   }
   ```
   **✓ 先把 folder[0] 加入结果，从 i=1 开始**

3. **✗ HashSet 方法中从 i=0 开始检查前缀**
   ```cpp
   // 错误：i=0 时 f[0]='/'，substr(0,0)="" 空串
   for (int i = 0; i < f.size(); i++) { ... }
   ```
   **✓ 从 i=1 开始，只在遇到 '/' 时检查**

4. **✗ Trie 中按字符建树而非路径段**
   ```cpp
   // 错误：按字符建 Trie 会导致 "/a/b" 和 "/a/bc" 共享前缀 "/a/b"
   // 在 "/a/b" 标记 isEnd 后，"/a/bc" 会被误判为子文件夹
   ```
   **✓ 按 `/` 分割后以路径段为单位建 Trie**

## 面试追问

**Q1: 暴力法的复杂度是多少？能过吗？**
→ 暴力两两比较 O(n²·L)，n=4×10⁴, L=100，大约 1.6×10¹¹ 次操作，超时。但如果先排序再线性扫描，O(n·L·log n) ≈ 6×10⁷，完全没问题。

**Q2: 排序法中为什么字典序排序能保证父文件夹在子文件夹前面？**
→ 因为 `"/a"` 是 `"/a/..."` 的前缀，字典序中前缀总排在扩展串前面。排序后所有以 `"/a/"` 开头的路径都紧跟在 `"/a"` 后面，只需要和当前"最近的父"比较即可。

**Q3: 如果文件夹列表是动态的（可以添加/删除），用什么数据结构？**
→ 用 Trie。添加新路径时：如果路径上已有 isEnd 标记，说明新路径是子文件夹，不插入；如果新路径比已有路径更短，则删除其子树。删除时在 Trie 中定位并移除节点即可。排序法不适合动态场景。

**Q4: 能否不用排序，O(n·L) 时间内解决？**
→ 可以，用 Trie 建树 + DFS，或者用 HashSet 方法（虽然 HashSet 理论上是 O(n·L²)，但如果路径深度有限则接近线性）。Trie 方法是真正的 O(n·L)。

## 相关题型

- **208. 实现 Trie (前缀树)** — 本题 Trie 解法的基础，复用 Trie 的插入和查找框架，区别：本题按路径段建树而非按字符
- **14. 最长公共前缀** — 同样涉及前缀比较，复用排序后相邻比较的思路
- **588. 设计内存文件系统** — 直接复用本题的 Trie 结构，扩展为支持 mkdir/ls/addContent 等操作
- **720. 词典中最长的单词** — Trie + DFS 的组合，复用"建 Trie 后 DFS 收集结果"的模式