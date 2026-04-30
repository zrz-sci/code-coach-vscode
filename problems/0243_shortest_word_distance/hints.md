# 243. 最短单词距离

## 核心思路

一次扫描数组，维护 word1 和 word2 各自最近出现的位置索引。每次遇到其中一个词时，用另一个词最近的位置来计算距离，取全局最小值。

## 思维链

1. **理解距离**：两个单词在数组中的索引差的绝对值即为距离
2. **暴力思路**：收集 word1 和 word2 的所有位置，两两配对取最小 → O(n^2)
3. **优化观察**：对任一个 word1 出现位置，最近的 word2 一定是它前后最近的那个
4. **一次扫描**：只维护各自最近出现位置 `idx1` 和 `idx2`，遇到 word1/word2 时更新并计算距离
5. **初始值**：`idx1 = idx2 = -1`，只在两者都出现过后才计算距离

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 特点 |
|------|-----------|-----------|--------|------|
| 一次扫描（双指针） | O(n) | O(1) | ⭐⭐⭐⭐⭐ | 面试最优解 |
| 收集索引 + 双指针 | O(n) | O(n) | ⭐⭐⭐ | 多次查询场景更优 |

## 关键提示

```
示例: ["practice", "makes", "perfect", "coding", "makes"]
       idx: 0        1       2         3        4

word1 = "coding", word2 = "practice"

扫描过程:
  i=0: "practice" = word2 -> idx2=0
  i=1: 跳过
  i=2: 跳过
  i=3: "coding" = word1 -> idx1=3, dist = |3-0| = 3, minDist=3
  i=4: 跳过

答案: 3

word1 = "makes", word2 = "coding"

扫描过程:
  i=0: 跳过
  i=1: "makes" = word1 -> idx1=1
  i=2: 跳过
  i=3: "coding" = word2 -> idx2=3, dist = |3-1| = 2, minDist=2
  i=4: "makes" = word1 -> idx1=4, dist = |4-3| = 1, minDist=1

答案: 1
```

## 解法详解

### 解法1: 一次扫描

**核心思想**：维护两个词最近出现的索引，每次遇到一个词时，用另一个词的最近索引计算距离。

```cpp
class Solution {
public:
    int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
        int idx1 = -1, idx2 = -1;
        int minDist = INT_MAX;
        
        for (int i = 0; i < wordsDict.size(); i++) {
            if (wordsDict[i] == word1) {
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

### 解法2: 收集索引 + 双指针合并

**核心思想**：先收集两个词的所有出现位置，再用双指针在两个有序数组中找最小差。适合多次查询不同词对的场景。

```cpp
class Solution {
public:
    int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
        vector<int> pos1, pos2;
        
        for (int i = 0; i < wordsDict.size(); i++) {
            if (wordsDict[i] == word1) pos1.push_back(i);
            else if (wordsDict[i] == word2) pos2.push_back(i);
        }
        
        int minDist = INT_MAX;
        int i = 0, j = 0;
        while (i < pos1.size() && j < pos2.size()) {
            minDist = min(minDist, abs(pos1[i] - pos2[j]));
            if (pos1[i] < pos2[j]) i++;
            else j++;
        }
        
        return minDist;
    }
};
```

**复杂度**：时间 O(n)，空间 O(n)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 `if` + `if` 判断两个词 | ✓ 用 `if` + `else if` | word1 != word2 保证互斥，else if 更清晰 |
| ✗ 初始 `idx1 = 0, idx2 = 0` | ✓ `idx1 = -1, idx2 = -1` | 0 是合法索引，会误算距离 |
| ✗ 不检查 `idx1 != -1 && idx2 != -1` | ✓ 两者都出现过才计算距离 | 否则 `abs(-1 - i)` 产生错误结果 |
| ✗ 只在遇到 word1 时计算 | ✓ 遇到 word1 或 word2 时都计算 | 否则可能漏掉最优解 |

## 面试追问

**Q1: 如果需要多次查询不同的 word pair 怎么优化？**
→ 预处理：用 HashMap 存每个词的所有出现位置。查询时对两个有序位置列表做双指针合并，找最小差。预处理 O(n)，每次查询 O(m1 + m2)。（LC 244 Shortest Word Distance II）

**Q2: 如果 word1 和 word2 可能相同怎么办？**
→ 需要特殊处理：同一个词的两次不同出现之间的距离。不能用 `else if`，而是追踪同一个词的前一次出现位置。（LC 245 Shortest Word Distance III）

**Q3: 时间复杂度能否优于 O(n)？**
→ 单次查询不能，因为必须看完所有元素才能确定最小距离。但多次查询可以预处理优化到 O(m1 + m2) 或 O(log n)（二分查找）。

## 相关题型

- [244. 最短单词距离 II](https://leetcode.com/problems/shortest-word-distance-ii/) - 多次查询优化版
- [245. 最短单词距离 III](https://leetcode.com/problems/shortest-word-distance-iii/) - word1 == word2 的情况
- [21. 合并两个有序链表](https://leetcode.com/problems/merge-two-sorted-lists/) - 双指针合并思想
