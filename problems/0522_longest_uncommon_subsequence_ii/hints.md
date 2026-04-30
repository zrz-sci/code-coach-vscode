# 522. 最长特殊序列 II (Longest Uncommon Subsequence II)

**难度**: Medium | **标签**: Array, Hash Table, Two Pointers, String, Sorting

## 核心思路

找出数组中某个字符串本身不是其他任何字符串的子序列，则该字符串就是一个"特殊序列"。关键洞察：**答案一定是数组中某个完整字符串本身**（而非某个字符串的真子序列），因为如果某子序列是特殊的，那么包含它的原始字符串也是特殊的且更长。

## 思维链

1. **暴力思路**: 枚举所有子序列，检查是否只属于一个字符串 -> 指数级复杂度，不可行
2. **关键洞察**: 最长特殊序列一定是某个原始字符串本身
3. **优化**: 只需检查每个字符串是否不是其他字符串的子序列
4. **排序优化**: 按长度降序排序，优先检查长的字符串
5. **子序列判断**: 双指针法 O(n) 判断 a 是否是 b 的子序列

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 排序 + 逐个检查 | O(n^2 * L) | O(1) | ⭐⭐⭐⭐⭐ |
| 暴力枚举子序列 | O(2^L * n) | O(L) | ⭐ |

## 关键提示

1. 一个字符串如果和另一个字符串**完全相同**，那它一定是对方的子序列
2. 较短的字符串可能是较长字符串的子序列，反之不可能
3. 按长度降序排序后，找到第一个不是其他任何字符串子序列的字符串即为答案
4. 判断子序列用双指针：i 指向候选串，j 指向目标串，逐字符匹配

## 解法详解

### 解法一：排序 + 子序列检查

```cpp
class Solution {
public:
    bool isSubseq(const string& a, const string& b) {
        // 判断 a 是否是 b 的子序列
        int i = 0;
        for (int j = 0; i < a.size() && j < b.size(); j++) {
            if (a[i] == b[j]) i++;
        }
        return i == a.size();
    }
    
    int findLUSlength(vector<string>& strs) {
        // 按长度降序排序
        sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
            return a.size() > b.size();
        });
        
        for (int i = 0; i < strs.size(); i++) {
            bool isUncommon = true;
            for (int j = 0; j < strs.size(); j++) {
                if (i != j && isSubseq(strs[i], strs[j])) {
                    isUncommon = false;
                    break;
                }
            }
            if (isUncommon) return strs[i].size();
        }
        return -1;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只比较相邻字符串 | ✓ 与所有其他字符串比较 | 需要确认不是任何字符串的子序列 |
| ✗ `if (i != j && strs[i] == strs[j])` 就跳过 | ✓ 用 `isSubseq` 判断 | 相等只是子序列的特殊情况 |
| ✗ 忘记 `i != j` 的判断 | ✓ 跳过自身比较 | 自己一定是自己的子序列 |

## 面试追问

**Q1**: 为什么答案一定是某个完整字符串，而不是某个字符串的真子序列？
> 如果 t 是 s 的真子序列且 t 是特殊的，那么 s 也不可能是其他字符串的子序列（否则 t 也会是），所以 s 也是特殊的且更长。

**Q2**: 如果字符串数量很大（比如 10^5），如何优化？
> 可以先用哈希表统计重复字符串，重复出现的字符串一定不是答案（互为子序列）。再按长度降序排序后，只需检查唯一出现的字符串。

**Q3**: 如何判断 a 是否是 b 的子序列？时间复杂度是多少？
> 双指针法，i 遍历 a、j 遍历 b，当 a[i]==b[j] 时 i++，最终 i==a.size() 则是子序列。时间 O(max(|a|,|b|))。

## 相关题型

- [392. 判断子序列](https://leetcode.com/problems/is-subsequence/) - 子序列判断基础
- [521. 最长特殊序列 I](https://leetcode.com/problems/longest-uncommon-subsequence-i/) - 简化版本
- [1143. 最长公共子序列](https://leetcode.com/problems/longest-common-subsequence/) - 子序列 DP
