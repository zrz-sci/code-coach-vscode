# 291. Word Pattern II (单词规律 II)

**难度**: Medium | **标签**: Hash Table, String, Backtracking

## 核心思路

建立 pattern 字符到 string 子串的**双射映射**（bijective mapping），用回溯法枚举所有可能的映射关系。每次从 pattern 取一个字符，从 s 中尝试不同长度的子串作为其映射值，递归验证剩余部分是否匹配。

## 思维链

1. pattern 中每个字符对应 s 的一个非空子串
2. 不同字符必须映射到不同子串（双射）
3. 对于每个 pattern[i]，如果已有映射则直接匹配；否则枚举所有可能的子串长度
4. 需要同时维护 char->string 和 string->char 两个方向的映射，保证双射
5. 剪枝：剩余字符串长度必须足够分配给剩余 pattern 字符

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| 回溯 + 双射HashMap | 枚举子串映射 + 回溯验证 | O(n^m) | O(m+n) | ⭐ |
| 回溯 + Set去重 | HashMap + Set 确保唯一性 | O(n^m) | O(m+n) | |

其中 n = s.length(), m = pattern.length()

## 关键提示

```
pattern = "abab"
s       = "redblueredblue"

回溯树示意:
a -> "r"
  b -> "e" ... 匹配失败，回溯
  b -> "ed" ... 匹配失败，回溯
  b -> "edb" ... 匹配失败，回溯
  b -> "edbl" ... 匹配失败，回溯
  b -> "edblue" ... 匹配 a->"r" 失败 (s剩余不足)
a -> "re"
  b -> "d" ... 匹配失败
  b -> "db" ... 
  ...
a -> "red"
  b -> "blue"
    a -> 已映射"red", 检查 s[7..10]=="red" ✓
      b -> 已映射"blue", 检查 s[10..14]=="blue" ✓ => true!
```

**双射检查**:
- 正向: char -> string (HashMap)
- 反向: string -> char (HashMap 或 Set)
- 缺一不可！否则 "ab" 可能同时映射到 "red"

## 解法详解

### 解法一：回溯 + 双射HashMap（推荐）

```cpp
class Solution {
public:
    bool wordPatternMatch(string pattern, string s) {
        unordered_map<char, string> charToStr;
        unordered_map<string, char> strToChar;
        return backtrack(pattern, 0, s, 0, charToStr, strToChar);
    }
    
private:
    bool backtrack(const string& pattern, int pi,
                   const string& s, int si,
                   unordered_map<char, string>& charToStr,
                   unordered_map<string, char>& strToChar) {
        // 两者同时到达末尾则匹配成功
        if (pi == pattern.size() && si == s.size()) return true;
        // 只有一个到达末尾则失败
        if (pi == pattern.size() || si == s.size()) return false;
        
        char c = pattern[pi];
        
        // 如果当前字符已有映射
        if (charToStr.count(c)) {
            const string& mapped = charToStr[c];
            // 检查 s 中对应位置是否匹配
            if (s.compare(si, mapped.size(), mapped) != 0) return false;
            return backtrack(pattern, pi + 1, s, si + mapped.size(),
                           charToStr, strToChar);
        }
        
        // 当前字符无映射，枚举所有可能的子串长度
        int remainPattern = pattern.size() - pi - 1;
        // 剪枝：剩余字符串长度至少要够剩余 pattern 字符各分1个字符
        for (int len = 1; si + len <= (int)s.size() - remainPattern; len++) {
            string sub = s.substr(si, len);
            
            // 检查该子串是否已被其他字符映射（保证双射）
            if (strToChar.count(sub)) continue;
            
            // 建立双向映射
            charToStr[c] = sub;
            strToChar[sub] = c;
            
            if (backtrack(pattern, pi + 1, s, si + len,
                         charToStr, strToChar)) {
                return true;
            }
            
            // 回溯：撤销映射
            charToStr.erase(c);
            strToChar.erase(sub);
        }
        
        return false;
    }
};
```

### 解法二：回溯 + Set去重

```cpp
class Solution {
public:
    bool wordPatternMatch(string pattern, string s) {
        unordered_map<char, string> map;
        unordered_set<string> used;
        return dfs(pattern, 0, s, 0, map, used);
    }
    
private:
    bool dfs(const string& p, int pi, const string& s, int si,
             unordered_map<char, string>& map, unordered_set<string>& used) {
        if (pi == p.size()) return si == s.size();
        if (si == s.size()) return false;
        
        char c = p[pi];
        if (map.count(c)) {
            const string& w = map[c];
            if (s.compare(si, w.size(), w) != 0) return false;
            return dfs(p, pi + 1, s, si + w.size(), map, used);
        }
        
        for (int len = 1; si + len <= (int)s.size(); len++) {
            string sub = s.substr(si, len);
            if (used.count(sub)) continue;
            
            map[c] = sub;
            used.insert(sub);
            if (dfs(p, pi + 1, s, si + len, map, used)) return true;
            map.erase(c);
            used.erase(sub);
        }
        return false;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只用 char->string 映射 | ✓ 同时维护 string->char 反向映射 | 必须是双射，否则不同字符可映射到同一子串 |
| ✗ `for (int len = 1; si+len <= s.size(); ...)` | ✓ 加剪枝 `si+len <= s.size() - remainPattern` | 不剪枝会超时，剩余字串不够分 |
| ✗ 忘记回溯撤销映射 | ✓ 在递归失败后 erase 映射 | 经典回溯要点 |
| ✗ 子串比较用 `==` 拼接 | ✓ 用 `s.compare(si, len, mapped)` | 避免多余的 substr 调用，提升性能 |

## 面试追问

**Q1: 时间复杂度怎么分析？**
> 最坏情况下，每个 pattern 字符有 O(n) 种可能的子串长度，共 m 个字符，所以是 O(n^m)。但实际通过双射约束和剪枝，平均情况远小于此。

**Q2: 与 LC 290 Word Pattern 的区别？**
> LC 290 中单词已经用空格分隔好了，直接建立映射即可，是 O(n) 的。而 291 需要自己确定分割点，必须用回溯搜索。

**Q3: 如何进一步优化剪枝？**
> 1) 统计 pattern 中各字符出现次数，计算最小所需字符串长度；2) 如果某字符出现 k 次，对应子串长度 l，则 k*l 不能超过剩余 s 长度；3) 优先处理出现次数最多的字符可减少搜索空间。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 290 | Word Pattern | 简化版，单词已分隔 |
| 205 | Isomorphic Strings | 双射映射基础 |
| 10 | Regular Expression Matching | 模式匹配 + 回溯 |
| 44 | Wildcard Matching | 模式匹配 |
| 17 | Letter Combinations of a Phone Number | 回溯枚举 |
