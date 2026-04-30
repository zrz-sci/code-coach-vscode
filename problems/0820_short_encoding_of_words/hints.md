# 820. 单词的压缩编码

## 核心思路

如果一个单词是另一个单词的**后缀**，那么较短的单词可以被较长的单词"包含"在编码中（不需要额外空间）。问题变成：**去除所有是其他单词后缀的单词，剩余单词各自长度+1（加 '#'）的总和**。可以用 **Trie（前缀树，插入反转后的单词）** 或 **哈希集合 + 后缀删除** 来解。

## 思维链

1. **读完题第一反应** → 编码字符串 `s` 中每个单词以 `#` 结尾。如果 "me" 是 "time" 的后缀，那 "time#" 中已经包含了 "me#" 的信息。
2. **问题简化** → 只需要保留那些**不是其他单词后缀**的单词。这些单词各自长度+1 的总和就是答案。
3. **怎么判断后缀关系？** → 方法1：对每个单词，检查它的所有后缀是否在集合中，有的话删掉后缀。方法2：把单词反转后插入 Trie，后缀关系变成前缀关系。
4. **Trie 方案** → 反转单词插入 Trie 后，只有 Trie 的叶子节点（没有更长的延伸）对应的单词需要保留在编码中。
5. **哈希集合方案** → 把所有单词放入集合，对每个单词的所有后缀，从集合中删除。剩下的单词都不是任何其他单词的后缀。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希集合 + 后缀删除 | 删除所有是后缀的短单词 | O(sum(w_i^2)) | O(sum(w_i)) | 代码简洁 |
| Trie（反转插入） | 反转后建 Trie，统计叶子节点深度 | O(sum(w_i)) | O(sum(w_i)) | 面试首选，展示 Trie |

其中 w_i 是第 i 个单词的长度。

## 关键提示

1. **后缀关系**："me" 是 "time" 的后缀，所以 "me" 可以被 "time#" 包含。"me#" 不需要单独出现。
2. **反转后变前缀**：把单词反转后，后缀关系变成前缀关系，可以用 Trie 高效处理。
3. **哈希方案的技巧**：对单词 "time"，它的后缀有 "ime", "me", "e"（不包括自身）。从集合中删掉这些后缀即可。
4. **Trie 方案的统计**：建完 Trie 后，只需统计叶子节点。每个叶子节点代表一个不被其他单词包含的词，贡献 = 深度 + 1（加 '#'）。
5. **去重**：words 中可能有重复单词，用 set 去重后再处理。

## 解法详解

### 解法1: 哈希集合 + 后缀删除 — O(sum(w_i^2)) / O(sum(w_i))

**思考过程**: 把所有单词放入集合。对每个单词，把它的所有**真后缀**从集合中删掉。剩下的单词就是需要保留的。

```
示例: words = ["time", "me", "bell"]

Step 1: 集合 = {"time", "me", "bell"}
Step 2: 处理 "time" 的后缀: "ime", "me", "e"
        删掉 "me" → 集合 = {"time", "bell"}
Step 3: 处理 "me" 的后缀: "e"
        "e" 不在集合中, 无操作
Step 4: 处理 "bell" 的后缀: "ell", "ll", "l"
        都不在集合中, 无操作

剩余: {"time", "bell"}
编码长度: (4+1) + (4+1) = 10
```

```cpp
class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        unordered_set<string> wordSet(words.begin(), words.end());
        
        // 对每个单词，删掉它的所有真后缀
        for (const string& word : words) {
            for (int i = 1; i < word.size(); i++) {
                wordSet.erase(word.substr(i));
            }
        }
        
        // 剩余单词各自 长度+1 的总和
        int result = 0;
        for (const string& word : wordSet) {
            result += word.size() + 1;
        }
        return result;
    }
};
```

**关键点**:
- `word.substr(i)` 取从位置 i 开始的后缀。
- 从 `i = 1` 开始，不包含单词自身（不能删自己）。
- 时间复杂度：对每个长度为 L 的单词，产生 L-1 个后缀，每个后缀的 substr 和 erase 操作是 O(L)，总体 O(sum(L^2))。由于 L <= 7，实际很快。

---

### 解法2: Trie（反转插入） — O(sum(w_i)) / O(sum(w_i))  面试首选

**思考过程**: 后缀关系不好直接用 Trie 处理（Trie 处理前缀更自然）。把单词反转后，后缀关系变成前缀关系。建完 Trie 后，只有叶子节点代表的单词需要保留。

```
示例: words = ["time", "me", "bell"]
反转: ["emit", "em", "lleb"]

Trie:
  root → e → m → i → t  (叶子, 深度4, 贡献 4+1=5)
              ↑
              "em" 到此为止, 但不是叶子(有 i → t 延伸), 不贡献
       → l → l → e → b  (叶子, 深度4, 贡献 4+1=5)

总编码长度: 5 + 5 = 10
```

```cpp
class Solution {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        int depth = 0;
    };
    
    int minimumLengthEncoding(vector<string>& words) {
        TrieNode* root = new TrieNode();
        
        // 去重并按长度降序排列（先插长词）
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.size() > b.size();
        });
        
        int result = 0;
        unordered_set<string> seen;
        
        for (const string& word : words) {
            if (seen.count(word)) continue;
            seen.insert(word);
            
            TrieNode* node = root;
            bool isNew = false;
            
            // 反转插入
            for (int i = word.size() - 1; i >= 0; i--) {
                int c = word[i] - 'a';
                if (!node->children[c]) {
                    node->children[c] = new TrieNode();
                    isNew = true;
                }
                node = node->children[c];
            }
            
            if (isNew) {
                result += word.size() + 1;
            }
        }
        
        return result;
    }
};
```

**关键点**:
- **按长度降序排列**很重要：先插入长词，短词如果是长词的后缀，插入时不会创建新节点（`isNew` 为 false），不贡献长度。
- 如果先插入短词再插入长词，需要更复杂的叶子节点检测逻辑。

## 易错点

1. **忘记去重**：
   - words 中可能有重复单词如 ["time", "time", "bell"]
   - 如果不去重，"time" 会被计算两次

2. **后缀包含自身**：
   - 删除后缀时应从 `i = 1` 开始，不能删除单词自身
   - "time" 的后缀是 "ime", "me", "e"，不包括 "time"

3. **Trie 方案中插入顺序**：
   - 如果不按长度排序，先插入 "me" 再插入 "time" 时，"me" 已经贡献了长度但后来被 "time" 包含
   - 解决方案1：按长度降序排列
   - 解决方案2：全部插入后遍历 Trie 只统计叶子节点

4. **编码长度 = 单词长度 + 1**：
   - 别忘了每个单词后面有个 '#'，所以贡献是 `length + 1`

5. **空 words 数组**：
   - 如果 words 为空，返回 0。但题目约束 words.length >= 1

## 面试追问

**Q1: 两种方法的时间复杂度对比？**
> 哈希方法是 O(sum(L_i^2))，因为每个长度 L 的单词产生 O(L) 个后缀，每个 substr 是 O(L)。Trie 方法是 O(sum(L_i))，每个字符只被处理一次。但由于 L <= 7，实际差异不大。

**Q2: Trie 方案有内存泄漏，怎么处理？**
> 面试中一般不要求处理。如果要求，可以用智能指针（`unique_ptr`）管理 Trie 节点，或在析构函数中递归删除。

**Q3: 如果单词长度没有上限（可能很长），哈希方法还适用吗？**
> 哈希方法变成 O(n * L^2)，L 很大时不可接受。此时 Trie 方法 O(n * L) 更优。

**Q4: 能否用排序解决？**
> 可以。对单词按反转后的字典序排序。排序后，如果一个单词是下一个单词的后缀，它们在排序结果中会相邻。只需检查相邻单词的包含关系。

## 相关题型

- **208. 实现 Trie (前缀树)** — Trie 的基础实现，本题是 Trie 的应用。如果不熟悉 Trie，先做 208。
- **648. 单词替换** — 也使用 Trie 处理前缀关系。本题处理后缀关系（通过反转转化为前缀）。
- **676. 实现一个魔法字典** — Trie 的变体应用，支持模糊匹配。
- **14. 最长公共前缀** — 字符串前缀处理的基础题。可以用 Trie 建模。
