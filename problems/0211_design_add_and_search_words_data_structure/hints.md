# 211. 添加与搜索单词 - 数据结构设计 (Medium)

**Tags:** Depth-First Search, Design, Trie, String

## 核心思路

本题要求设计一个支持 `addWord` 和 `search` 的数据结构，其中 `search` 支持通配符 `.` 匹配任意字符。核心在于使用 **Trie（前缀树）** 存储单词，遇到 `.` 时用 **DFS 回溯** 遍历所有子节点。

## 思维链

1. **暴力思考**：用一个 `vector<string>` 存所有单词，search 时逐个比较，遇到 `.` 跳过该位。时间 O(N*L)，N 为单词数，L 为单词长度。
2. **哈希分组优化**：按单词长度分组存入 `unordered_map<int, vector<string>>`，search 时只比较同长度的单词。减少了比较量但最坏仍是 O(N*L)。
3. **引入 Trie**：Trie 天然支持前缀匹配，addWord 为 O(L)，精确 search 也是 O(L)。
4. **处理通配符**：遇到 `.` 时，需要遍历当前节点的所有非空子节点继续匹配，这就是 DFS/回溯。
5. **剪枝优化**：只要任意一个分支返回 true 即可提前结束。由于题目限制最多 2 个 `.`，最坏分支因子为 26^2 = 676，实际非常高效。
6. **最终方案**：Trie + DFS 回溯，addWord O(L)，search 最坏 O(26^dots * L)，其中 dots <= 2。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Trie + DFS 回溯 ⭐ | add: O(L), search: O(26^d * L) | O(total chars * 26) | 标准做法，支持通配符 |
| 哈希分组 + 逐字比较 | O(N * L) per search | O(N * L) | 简单但效率低 |

## 关键提示

### Trie 结构示意

```
          root
        /  |  \
       b   d   m
       |   |   |
       a   a   a
       |   |   |
       d*  d*  d*

addWord("bad"), addWord("dad"), addWord("mad")
* 表示 isEnd = true

search(".ad"):
  '.' -> 遍历 root 的所有子节点 [b, d, m]
  对每个子节点，继续匹配 'a' -> 'd'
  任一路径到达 isEnd=true 的节点 -> return true
```

### DFS 搜索流程

```
search("b.."):
  'b' -> 走到节点 b
  '.' -> 遍历 b 的所有子节点 [a]
  '.' -> 遍历 a 的所有子节点 [d*]
  到达 d，isEnd=true -> return true
```

## 解法详解

### 解法1: Trie + DFS 回溯 (推荐)

**思路**：构建 26 叉 Trie 树。`addWord` 沿路径插入字符。`search` 遇到普通字符走对应子节点，遇到 `.` 遍历所有非空子节点递归搜索。

```cpp
class WordDictionary {
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isEnd = false;
    };
    
    TrieNode* root;
    
    bool dfs(const string& word, int idx, TrieNode* node) {
        if (idx == word.size()) return node->isEnd;
        
        if (word[idx] == '.') {
            // 通配符：遍历所有非空子节点
            for (int i = 0; i < 26; i++) {
                if (node->children[i] && dfs(word, idx + 1, node->children[i])) {
                    return true;
                }
            }
            return false;
        } else {
            // 普通字符：走对应子节点
            int c = word[idx] - 'a';
            return node->children[c] && dfs(word, idx + 1, node->children[c]);
        }
    }
    
public:
    WordDictionary() : root(new TrieNode()) {}
    
    void addWord(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!curr->children[idx]) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
        }
        curr->isEnd = true;
    }
    
    bool search(string word) {
        return dfs(word, 0, root);
    }
};
```

### 解法2: 哈希分组 + 暴力匹配

**思路**：按单词长度分组存储。search 时只遍历同长度的单词，逐字符比较，`.` 匹配任意字符。

```cpp
class WordDictionary {
    unordered_map<int, vector<string>> words;
    
public:
    WordDictionary() {}
    
    void addWord(string word) {
        words[word.size()].push_back(word);
    }
    
    bool search(string word) {
        for (const string& w : words[word.size()]) {
            bool match = true;
            for (int i = 0; i < word.size(); i++) {
                if (word[i] != '.' && word[i] != w[i]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `if (idx == word.size()) return true;` | ✓ `if (idx == word.size()) return node->isEnd;` | 必须检查是否是完整单词的结尾 |
| ✗ 遇到 `.` 只检查第一个子节点 | ✓ 遍历所有 26 个子节点 | `.` 是任意字符，需要全部尝试 |
| ✗ 忘记判断 `node->children[c]` 是否为空 | ✓ 先检查非空再递归 | 空指针访问会导致崩溃 |
| ✗ DFS 中忘记传递正确的 node 指针 | ✓ `dfs(word, idx+1, node->children[i])` | 每层递归需推进到子节点 |

## 面试追问

**Q1: 如果 `.` 的数量没有限制（可能全是 `.`），复杂度如何？**
→ 最坏情况 search 退化为 O(26^L)，即遍历 Trie 中所有路径。可以考虑按长度建索引辅助剪枝。

**Q2: 如何支持删除操作 `deleteWord`？**
→ 在 TrieNode 中增加 `count` 字段记录经过该节点的单词数。删除时沿路径递减 count，count 为 0 时可以释放节点。

**Q3: 如果要支持前缀搜索 `startsWith` 和通配符，如何设计？**
→ `startsWith` 只需不检查 `isEnd`，走到路径末尾即返回 true。可以在同一个 Trie 上同时支持 search 和 startsWith。

## 相关题型

- [208. Implement Trie (Prefix Tree)](https://leetcode.com/problems/implement-trie-prefix-tree/) - Trie 基础实现
- [212. Word Search II](https://leetcode.com/problems/word-search-ii/) - Trie + DFS 在矩阵中搜索
- [745. Prefix and Suffix Search](https://leetcode.com/problems/prefix-and-suffix-search/) - Trie 进阶应用
- [676. Implement Magic Dictionary](https://leetcode.com/problems/implement-magic-dictionary/) - 类似的模糊匹配
