# 245. 最短单词距离 III

## 核心思路

LC 243 的变体——word1 和 word2 可能相同。当相同时，需要追踪同一个词的**前一次和当前出现位置**之间的距离。统一处理的技巧：每次遇到 word1/word2 时，先把旧的 idx1 保存到 idx2（当 word1 == word2 时），再更新 idx1。

## 思维链

1. **识别与 LC 243 的区别**：word1 和 word2 可能相同
2. **word1 != word2 时**：与 LC 243 完全相同，分别跟踪位置
3. **word1 == word2 时**：需要跟踪同一个词的**两次不同出现**的最小距离
4. **统一处理**：当 word1 == word2 时，遇到该词就把"上一次位置"和"本次位置"配对
5. **巧妙实现**：用一个 `prev` 变量记录上一次出现位置，每次更新前先保存旧值

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 特点 |
|------|-----------|-----------|--------|------|
| 统一一次扫描 | O(n) | O(1) | ⭐⭐⭐⭐⭐ | 优雅处理两种情况 |
| 分类讨论 | O(n) | O(1) | ⭐⭐⭐⭐ | 逻辑清晰，代码稍长 |

## 关键提示

```
Case 1: word1 != word2 (与 LC 243 相同)
  ["practice", "makes", "perfect", "coding", "makes"]
   word1="makes", word2="coding"
   
   i=1: "makes"=word1, idx1=1
   i=3: "coding"=word2, idx2=3, dist=|3-1|=2
   i=4: "makes"=word1, idx1=4, dist=|4-3|=1 -> 答案 1

Case 2: word1 == word2 (本题新增)
  ["practice", "makes", "perfect", "coding", "makes"]
   word1="makes", word2="makes"
   
   i=1: "makes", idx1=1 (第一次出现)
   i=4: "makes", 先把旧 idx1 存为 idx2: idx2=1
              再更新 idx1=4
              dist=|4-1|=3 -> 答案 3

统一处理的关键:
  遇到 word1 时: 如果 word1==word2, 先 idx2=idx1 (保存旧位置)
                 然后 idx1=i
  遇到 word2 时: idx2=i (word1!=word2 才走这个分支)
  
  这样无论哪种 case，idx1 和 idx2 总是"最近的两个不同出现位置"
```

## 解法详解

### 解法1: 统一一次扫描

**核心思想**：遇到 word1 时，如果 word1 == word2，先把旧位置移到 idx2，再更新 idx1。这样 idx1/idx2 始终是两个最近的不同出现位置。

```cpp
class Solution {
public:
    int shortestWordDistance(vector<string>& wordsDict, string word1, string word2) {
        int idx1 = -1, idx2 = -1;
        int minDist = INT_MAX;
        bool same = (word1 == word2);
        
        for (int i = 0; i < wordsDict.size(); i++) {
            if (wordsDict[i] == word1) {
                if (same) {
                    idx2 = idx1; // word1==word2: 旧位置变成 idx2
                }
                idx1 = i;
            } else if (wordsDict[i] == word2) {
                idx2 = i;
            }
            
            if (idx1 != -1 && idx2 != -1) {
                minDist = min(minDist, abs(idx1 - idx2));
            }
        }
        
        return minDist;
    }
};
```

**复杂度**：时间 O(n)，空间 O(1)

### 解法2: 分类讨论

**核心思想**：显式分为 word1 == word2 和 word1 != word2 两种情况。代码更长但逻辑更直白。

```cpp
class Solution {
public:
    int shortestWordDistance(vector<string>& wordsDict, string word1, string word2) {
        int minDist = INT_MAX;
        
        if (word1 == word2) {
            // 同一个词的两次出现之间最小距离
            int prev = -1;
            for (int i = 0; i < wordsDict.size(); i++) {
                if (wordsDict[i] == word1) {
                    if (prev != -1) {
                        minDist = min(minDist, i - prev);
                    }
                    prev = i;
                }
            }
        } else {
            // 不同词的最近出现位置（与 LC 243 相同）
            int idx1 = -1, idx2 = -1;
            for (int i = 0; i < wordsDict.size(); i++) {
                if (wordsDict[i] == word1) idx1 = i;
                else if (wordsDict[i] == word2) idx2 = i;
                
                if (idx1 != -1 && idx2 != -1) {
                    minDist = min(minDist, abs(idx1 - idx2));
                }
            }
        }
        
        return minDist;
    }
};
```

**复杂度**：时间 O(n)，空间 O(1)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ word1==word2 时用 `else if` | ✓ 同一个词用 `if` 并保存旧位置 | else if 会导致同一个词永远只更新 idx1 |
| ✗ 不保存旧位置直接覆盖 | ✓ `idx2 = idx1` 再 `idx1 = i` | 覆盖后两次出现的距离信息丢失 |
| ✗ word1==word2 时计算 `abs(idx1-idx1)` = 0 | ✓ 要比较的是两次不同出现 | 同一次出现距离为0没有意义 |
| ✗ 忘记处理 word1==word2 的 case | ✓ 提前判断 `same` 标志 | 直接套用 LC 243 代码会在 same case 出错 |

## 面试追问

**Q1: 与 LC 243 相比多了什么边界条件？**
→ word1 == word2 时，不能用 `else if`，必须追踪同一个词的"前一次出现"和"当前出现"。核心变化就一个：遇到 word1 时先 `idx2 = idx1` 再 `idx1 = i`。

**Q2: 如何同时支持 LC 243 和 LC 245？**
→ 解法1 的统一写法天然支持两种情况。当 word1 != word2 时，`same` 为 false，`idx2 = idx1` 这行不执行，逻辑完全等同于 LC 243。

**Q3: 如果需要返回所有最短距离的词对位置呢？**
→ 在 minDist 更新时记录 `{idx1, idx2}` 对。如果新距离更小则清空旧结果；距离相等则追加。

## 相关题型

- [243. 最短单词距离](https://leetcode.com/problems/shortest-word-distance/) - 基础版本（word1 != word2）
- [244. 最短单词距离 II](https://leetcode.com/problems/shortest-word-distance-ii/) - 多次查询优化
- [246. 中心对称数](https://leetcode.com/problems/strobogrammatic-number/) - 同系列
