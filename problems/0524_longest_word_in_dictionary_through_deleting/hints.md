# 524. 通过删除字母匹配到字典里最长单词 (Longest Word in Dictionary through Deleting)

**难度**: Medium | **标签**: Array, Two Pointers, String, Sorting

## 核心思路

对字典中每个单词，用**双指针**判断它是否是字符串 s 的子序列。在所有匹配的单词中，选择最长的；若长度相同，选字典序最小的。

## 思维链

1. **问题转化**: "通过删除 s 中某些字符得到" = "是 s 的子序列"
2. **子序列判断**: 双指针法 O(|s| + |word|)
3. **最优选择**: 遍历字典，维护当前最优（最长 + 字典序最小）
4. **可选优化**: 先排序字典（长度降序、字典序升序），找到第一个匹配即为答案

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 双指针逐个检查 | O(n * S) | O(1) | ⭐⭐⭐⭐⭐ |
| 排序 + 双指针 | O(n*log(n) + n*S) | O(1) | ⭐⭐⭐⭐ |

其中 n 为字典大小，S 为字符串 s 的长度。

## 关键提示

1. **子序列判断**: 两个指针分别遍历 s 和 word，匹配时 word 指针前进
2. **比较规则**: 优先最长，其次字典序最小
3. **排序法**: 按 `(-length, word)` 排序后找第一个匹配
4. **无需排序法**: 直接遍历，用条件更新最优答案

## 解法详解

### 解法一：双指针逐个检查

```cpp
class Solution {
public:
    bool isSubseq(const string& s, const string& word) {
        int i = 0; // word 的指针
        for (int j = 0; i < word.size() && j < s.size(); j++) {
            if (word[i] == s[j]) i++;
        }
        return i == word.size();
    }
    
    string findLongestWord(string s, vector<string>& dictionary) {
        string result = "";
        for (auto& word : dictionary) {
            if (isSubseq(s, word)) {
                // 更长，或等长但字典序更小
                if (word.size() > result.size() ||
                    (word.size() == result.size() && word < result)) {
                    result = word;
                }
            }
        }
        return result;
    }
};
```

### 解法二：排序 + 双指针

```cpp
class Solution {
public:
    bool isSubseq(const string& s, const string& word) {
        int i = 0;
        for (int j = 0; i < word.size() && j < s.size(); j++) {
            if (word[i] == s[j]) i++;
        }
        return i == word.size();
    }
    
    string findLongestWord(string s, vector<string>& dictionary) {
        // 按长度降序，长度相同按字典序升序
        sort(dictionary.begin(), dictionary.end(), [](const string& a, const string& b) {
            return a.size() != b.size() ? a.size() > b.size() : a < b;
        });
        
        for (auto& word : dictionary) {
            if (isSubseq(s, word)) return word;
        }
        return "";
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 比较时忘记字典序 | ✓ 相同长度时比较 `word < result` | 题目要求长度相同取字典序最小 |
| ✗ 子序列判断方向反了 | ✓ word 是 s 的子序列（不是反过来） | 是从 s 中删字符得到 word |
| ✗ 排序只按长度 | ✓ 长度相同按字典序升序 | 否则可能遗漏字典序更小的答案 |

## 面试追问

**Q1**: 不排序和排序两种方法各有什么优缺点？
> 不排序省去 O(n*log(n)) 的排序时间，但需要完整遍历字典。排序后可以提前返回，但排序本身有开销。总体不排序更优。

**Q2**: 如果需要多次查询不同的 s，字典不变，如何优化？
> 预处理字典建立字符位置索引或 Trie。也可以对 s 建立每个字符下一个出现位置的跳表，实现二分查找匹配。

**Q3**: 子序列匹配的双指针为什么是正确的（贪心证明）？
> 对于 word 中每个字符，在 s 中尽早匹配是最优的，因为越早匹配留给后续字符的空间越大。

## 相关题型

- [392. 判断子序列](https://leetcode.com/problems/is-subsequence/) - 子序列判断基础
- [522. 最长特殊序列 II](https://leetcode.com/problems/longest-uncommon-subsequence-ii/) - 类似子序列检查
- [792. 匹配子序列的单词数](https://leetcode.com/problems/number-of-matching-subsequences/) - 多词子序列匹配优化
