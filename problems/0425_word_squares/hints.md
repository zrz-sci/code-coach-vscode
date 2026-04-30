# 425. Word Squares - 单词方块

## 核心思路
用 Trie 建立前缀索引 + 回溯搜索。逐行选词，每选一行后，下一行的前缀已由前面已选词的对应列确定，用 Trie 快速查找匹配前缀的候选词。

## 思维链
1. 将所有单词插入 Trie（或用 HashMap 存前缀→单词列表）
2. 从第0行开始，尝试每个单词作为第一行
3. 选定前 k 行后，第 k+1 行的前缀 = 前 k 行中第 k+1 列的字符拼接
4. 在 Trie 中查找该前缀的所有单词作为候选
5. 若所有行都填满（k == wordLen），收集结果

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1 | Trie + 回溯 ⭐ | O(N * 26^L) | O(N*L) | 首选 |
| 解法2 | HashMap前缀 + 回溯 | O(N * 26^L) | O(N*L) | 实现更简单 |

## 关键提示
1. **前缀约束推导**: 已选前 k 行后，第 k 行的前缀就是 `words[0][k] + words[1][k] + ... + words[k-1][k]`
2. **Trie 加速前缀查找**: 每个 Trie 节点存储以该前缀开头的所有单词索引
3. 同一个单词可以重复使用
4. 所有单词长度相同（题目约束）

## 解法详解

### 解法1: Trie + 回溯
```cpp
class Solution {
    struct TrieNode {
        vector<int> wordIndices; // 以该前缀开头的单词索引
        TrieNode* children[26] = {};
    };
    
    TrieNode* root;
    
    void insert(const string& word, int index) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c - 'a'])
                node->children[c - 'a'] = new TrieNode();
            node = node->children[c - 'a'];
            node->wordIndices.push_back(index);
        }
    }
    
    vector<int> findByPrefix(const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children[c - 'a']) return {};
            node = node->children[c - 'a'];
        }
        return node->wordIndices;
    }
    
public:
    vector<vector<string>> wordSquares(vector<string>& words) {
        root = new TrieNode();
        int n = words[0].size();
        for (int i = 0; i < words.size(); i++) insert(words[i], i);
        
        vector<vector<string>> result;
        vector<string> square;
        
        function<void(int)> backtrack = [&](int row) {
            if (row == n) { result.push_back(square); return; }
            // 构造当前行的前缀约束
            string prefix;
            for (int i = 0; i < row; i++) prefix += square[i][row];
            
            for (int idx : findByPrefix(prefix)) {
                square.push_back(words[idx]);
                backtrack(row + 1);
                square.pop_back();
            }
        };
        
        backtrack(0);
        return result;
    }
};
```

### 解法2: HashMap前缀 + 回溯
```cpp
class Solution {
public:
    vector<vector<string>> wordSquares(vector<string>& words) {
        unordered_map<string, vector<int>> prefixMap;
        int n = words[0].size();
        for (int i = 0; i < words.size(); i++) {
            for (int len = 0; len <= n; len++)
                prefixMap[words[i].substr(0, len)].push_back(i);
        }
        
        vector<vector<string>> result;
        vector<string> square;
        
        function<void(int)> backtrack = [&](int row) {
            if (row == n) { result.push_back(square); return; }
            string prefix;
            for (int i = 0; i < row; i++) prefix += square[i][row];
            if (prefixMap.find(prefix) == prefixMap.end()) return;
            for (int idx : prefixMap[prefix]) {
                square.push_back(words[idx]);
                backtrack(row + 1);
                square.pop_back();
            }
        };
        
        backtrack(0);
        return result;
    }
};
```

## 易错点
| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 前缀取 square[i][row] 时忘记 row 是列索引 | ✓ square 的第 i 行第 row 列 = 将来第 row 行第 i 列 |
| ✗ 忘记空前缀（第0行无约束） | ✓ 空前缀时 Trie 返回所有单词 |
| ✗ Trie 中只存叶子节点的索引 | ✓ 每个中间节点也要存经过它的所有单词索引 |

## 面试追问
**Q1: Trie 和 HashMap 哪个更好？**
> Trie 在前缀查找时不需要存储所有子串（空间更优），但 HashMap 实现更简单。当词库很大时 Trie 更有优势。

**Q2: 回溯的剪枝效果如何？**
> 前缀约束是天然剪枝：每多选一行，前缀多一个字符，候选词数量大幅减少。单词长度最多4，搜索空间有限。

**Q3: 如果单词长度不固定怎么办？**
> 需要额外处理不等长情况，前缀约束变得更复杂：某些位置可能没有字符，需要允许"短词"在该位置为空。

## 相关题型
- [422. Valid Word Square](https://leetcode.com/problems/valid-word-square/) - 验证单词方块
- [79. Word Search](https://leetcode.com/problems/word-search/) - 回溯搜索
- [212. Word Search II](https://leetcode.com/problems/word-search-ii/) - Trie + 回溯
