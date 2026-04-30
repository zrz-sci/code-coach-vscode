# 527. 单词缩写 (Word Abbreviation)

**难度**: Hard | **标签**: Greedy, Trie, Array, String, Sorting

## 核心思路

**贪心迭代解冲突**：先为每个单词生成最短缩写（首字母 + 中间字符数 + 尾字母），然后检查缩写冲突。有冲突的单词增加前缀长度，重复此过程直到所有缩写唯一。如果缩写不比原词短，则保留原词。

## 思维链

1. **初始缩写**: 每个词 -> 首字母 + (len-2) + 尾字母，如 "internal" -> "i6l"
2. **分组检测**: 将缩写相同的词分组
3. **冲突解决**: 有冲突的词增加前缀长度（如 "i6l" -> "in5l" -> "int4l"）
4. **终止条件**: 每个缩写唯一，或者缩写不比原词短时保留原词
5. **优化方案**: 也可以用 Trie 一次性确定每个词需要的最小前缀长度

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 贪心迭代解冲突 | O(n * L) 摊销 | O(n) | ⭐⭐⭐⭐⭐ |
| 分组 + Trie | O(n * L) | O(n * L) | ⭐⭐⭐⭐ |

## 关键提示

1. **缩写规则**: `prefix + (总长 - prefix长度 - 1) + 尾字母`
2. **保留原词**: 当缩写长度 >= 原词长度时，保留原词
3. **分组关键**: 只有长度相同且首尾字母相同的词才可能冲突
4. **前缀递增**: 每次冲突时前缀长度 +1，最多增加到整个词

## 解法详解

### 解法一：贪心迭代解冲突

```cpp
class Solution {
public:
    string makeAbbr(const string& word, int prefixLen) {
        int n = word.size();
        int abbrLen = n - prefixLen - 1; // 中间被缩写的字符数
        if (abbrLen <= 1) return word;    // 缩写不比原词短
        return word.substr(0, prefixLen) + to_string(abbrLen) + word.back();
    }
    
    vector<string> wordsAbbreviation(vector<string>& words) {
        int n = words.size();
        vector<string> result(n);
        vector<int> prefix(n, 1); // 每个词的前缀长度，初始为1
        
        // 初始化缩写
        for (int i = 0; i < n; i++) {
            result[i] = makeAbbr(words[i], 1);
        }
        
        // 迭代解冲突
        for (int i = 0; i < n; i++) {
            while (true) {
                // 找出所有与 result[i] 冲突的索引
                vector<int> conflicts;
                for (int j = i + 1; j < n; j++) {
                    if (result[i] == result[j]) {
                        conflicts.push_back(j);
                    }
                }
                if (conflicts.empty()) break;
                
                // 冲突的词都增加前缀长度
                conflicts.push_back(i);
                for (int idx : conflicts) {
                    prefix[idx]++;
                    result[idx] = makeAbbr(words[idx], prefix[idx]);
                }
            }
        }
        return result;
    }
};
```

### 解法二：分组 + Trie

```cpp
class Solution {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        int count = 0; // 经过此节点的单词数
    };
    
    string makeAbbr(const string& word, int prefixLen) {
        int n = word.size();
        int abbrLen = n - prefixLen - 1;
        if (abbrLen <= 1) return word;
        return word.substr(0, prefixLen) + to_string(abbrLen) + word.back();
    }
    
    vector<string> wordsAbbreviation(vector<string>& words) {
        int n = words.size();
        vector<string> result(n);
        
        // 按 (长度, 首字母, 尾字母) 分组
        unordered_map<string, vector<int>> groups;
        for (int i = 0; i < n; i++) {
            string key = to_string(words[i].size()) + "_" + words[i].front() + words[i].back();
            groups[key].push_back(i);
        }
        
        for (auto& [key, indices] : groups) {
            // 为每组建 Trie
            TrieNode* root = new TrieNode();
            for (int idx : indices) {
                TrieNode* node = root;
                for (char c : words[idx]) {
                    if (!node->children[c - 'a']) {
                        node->children[c - 'a'] = new TrieNode();
                    }
                    node = node->children[c - 'a'];
                    node->count++;
                }
            }
            
            // 找每个词的最短唯一前缀
            for (int idx : indices) {
                TrieNode* node = root;
                int prefixLen = 0;
                for (char c : words[idx]) {
                    node = node->children[c - 'a'];
                    prefixLen++;
                    if (node->count == 1) break; // 唯一前缀
                }
                result[idx] = makeAbbr(words[idx], prefixLen);
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 缩写长度 >= 原词长度时仍用缩写 | ✓ 返回原词 | "god" 缩写为 "g1d" 长度 3=3，应保留 |
| ✗ 只与相邻词比较冲突 | ✓ 与所有词比较 | 冲突可能发生在任意位置 |
| ✗ 解冲突时只增加冲突方之一的前缀 | ✓ 所有冲突方都要增加 | 否则下一轮仍会冲突 |
| ✗ 缩写中间数字算错 | ✓ `word.size() - prefixLen - 1` | 减去前缀和尾字母 |

## 面试追问

**Q1**: 迭代解冲突法的时间复杂度分析？
> 最坏情况每个词前缀增加到最大，但每次解冲突后至少一个词不再冲突。摊销 O(n * L)，其中 L 是最大词长。

**Q2**: Trie 方法相比迭代法有什么优势？
> Trie 一次性确定每个词的最短唯一前缀，避免了迭代收敛的过程。对于大量冲突的情况更高效。

**Q3**: 如何证明分组时只需要按 (长度, 首字母, 尾字母) 分？
> 缩写格式是 `prefix + 数字 + 尾字母`。不同长度的词缩写中间的数字不同；不同尾字母的缩写尾字母不同；不同首字母的缩写开头不同。所以只有三者都相同的词才可能产生相同缩写。

## 相关题型

- [408. 有效单词缩写](https://leetcode.com/problems/valid-word-abbreviation/) - 缩写验证
- [411. 最短独占单词缩写](https://leetcode.com/problems/minimum-unique-word-abbreviation/) - 更难的缩写优化
- [208. 实现 Trie](https://leetcode.com/problems/implement-trie-prefix-tree/) - Trie 基础
