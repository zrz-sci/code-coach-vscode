# 320. 列举单词的全部缩写 (Generalized Abbreviation)

**难度**: Medium | **标签**: Bit Manipulation, String, Backtracking

## 核心思路

每个字符有两种选择：**保留原字母**或**被数字替换**。用回溯/位操作枚举所有 2^n 种组合，连续被替换的字符合并为一个数字。关键约束：两个数字之间必须有至少一个字母（不相邻）。

## 思维链

1. **建模**: 每个字符位置是一个二选一决策 —— 保留字母 or 计入缩写数字
2. **状态设计**: 回溯时追踪当前位置 `pos` 和当前累积的缩写计数 `count`
3. **分支逻辑**:
   - 选择"缩写当前字符"：count + 1，继续递归
   - 选择"保留当前字符"：先输出 count（如果>0），再追加字母
4. **终止条件**: pos == n 时，如果还有未输出的 count，追加数字
5. **位操作替代**: 用 0~2^n-1 的二进制位表示每个字符的选择

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 回溯法 | O(n * 2^n) | O(n) | ⭐⭐⭐⭐⭐ | 最直观，面试首选 |
| 位操作枚举 | O(n * 2^n) | O(n) | ⭐⭐⭐⭐ | 利用二进制位编码选择 |

## 关键提示

1. **count 变量**: 追踪连续被缩写的字符数，遇到保留字母时输出 count
2. **不相邻约束**: 回溯时自然满足 —— 保留字母后 count 归零
3. **结果总数**: 恰好 2^n 个（每个字符独立二选一）
4. **空串处理**: 长度1的字符串只有 "1" 和 "a" 两种

## 解法详解

### 解法一：回溯法 (推荐)

```cpp
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> result;
        string current;
        backtrack(word, 0, 0, current, result);
        return result;
    }
    
private:
    void backtrack(const string& word, int pos, int count, 
                   string& current, vector<string>& result) {
        int origLen = current.size();
        
        if (pos == word.size()) {
            if (count > 0) current += to_string(count);
            result.push_back(current);
        } else {
            // 选择1: 缩写当前字符（count+1）
            backtrack(word, pos + 1, count + 1, current, result);
            
            // 选择2: 保留当前字符
            if (count > 0) current += to_string(count);
            current += word[pos];
            backtrack(word, pos + 1, 0, current, result);
        }
        
        // 回溯：恢复 current
        current.resize(origLen);
    }
};
```

### 解法二：位操作枚举

```cpp
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        int n = word.size();
        vector<string> result;
        
        for (int mask = 0; mask < (1 << n); mask++) {
            string abbr;
            int count = 0;
            
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    // bit=1: 缩写该字符
                    count++;
                } else {
                    // bit=0: 保留该字符
                    if (count > 0) {
                        abbr += to_string(count);
                        count = 0;
                    }
                    abbr += word[i];
                }
            }
            if (count > 0) abbr += to_string(count);
            result.push_back(abbr);
        }
        
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 忘记在末尾处理残余 count | ✓ `if (count > 0) current += to_string(count)` | 字符串末尾可能有未输出的数字 |
| ✗ 回溯时没有恢复 current | ✓ 用 `origLen` + `resize` 或传值 | 必须正确回溯字符串状态 |
| ✗ 两个数字相邻如 "23" | ✓ 连续缩写字符合并为一个数字 | 回溯法中 count 自然处理 |
| ✗ 位操作时 n>15 溢出 | ✓ 题目保证 n<=15，int足够 | 2^15 = 32768 在 int 范围内 |

## 面试追问

**Q1: 结果总数是多少？时间复杂度如何分析？**
> 结果恰好 2^n 个。每个结果最长 O(n)，所以总时间 O(n * 2^n)。

**Q2: 如果要求只返回缩写长度最短的那些，如何修改？**
> 最短缩写就是将整个单词替换为一个数字 "n"，永远只有一个。如果要求"最短且有效"的不同缩写，需要定义额外规则。

**Q3: 如何判断一个缩写是否匹配某个单词？（LC 408）**
> 双指针扫描：遇到字母逐个匹配，遇到数字跳过对应长度的字符。

## 相关题型

- [408. 有效单词缩写](https://leetcode.com/problems/valid-word-abbreviation/) - 缩写匹配
- [411. 最短独占单词缩写](https://leetcode.com/problems/minimum-unique-word-abbreviation/) - 缩写进阶
- [78. 子集](https://leetcode.com/problems/subsets/) - 类似的二选一枚举模式
- [784. 字母大小写全排列](https://leetcode.com/problems/letter-case-permutation/) - 每位二选一
