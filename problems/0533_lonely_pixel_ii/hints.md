# 533. 孤独像素 II (Lonely Pixel II)

**难度**: Medium | **标签**: Array, Hash Table, Matrix

## 核心思路

在 Lonely Pixel I 基础上增加两个约束：(1) 行 r 和列 c 中黑色像素数恰好为 target；(2) 列 c 中所有有黑色像素的行必须与行 r 完全相同。关键是**用行的字符串编码做分组**，将"行相同"的约束转化为哈希表查找。

## 思维链

1. 统计每行/每列的 `'B'` 数量
2. 将每行转为字符串作为 key，统计相同行模式出现的次数
3. 对于每个 `'B'` 在 (r, c) 位置，检查三个条件：
   - rowCount[r] == target
   - colCount[c] == target
   - 该行模式出现的次数 == target（保证列 c 中所有行都一样）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 行编码 + 哈希表 | O(m*n) | O(m*n) | ⭐⭐⭐⭐⭐ |
| 暴力验证 | O(m^2 * n) | O(m+n) | ⭐⭐ |

## 关键提示

1. "列 c 中所有有 B 的行必须与行 r 相同" → 这些行的 pattern 完全一致
2. 如果列 c 有 target 个 B，且这些 B 所在的行都相同，那么相同行的数量恰好是 target
3. 将行转为字符串做 key，可以 O(1) 比较两行是否相同
4. 三个条件缺一不可：rowCount == target, colCount == target, 行模式出现次数 == target

## 解法详解

### 解法一：行编码 + 哈希表（推荐）

```cpp
class Solution {
public:
    int findBlackPixel(vector<vector<char>>& picture, int target) {
        int m = picture.size(), n = picture[0].size();
        vector<int> colCount(n, 0);
        unordered_map<string, int> rowPatternCount; // 行模式 -> 出现次数
        vector<string> rowStr(m);
        
        // 统计每列B数量 + 构建行字符串
        for (int i = 0; i < m; i++) {
            string s(picture[i].begin(), picture[i].end());
            rowStr[i] = s;
            rowPatternCount[s]++;
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B') colCount[j]++;
            }
        }
        
        int result = 0;
        for (int i = 0; i < m; i++) {
            // 行中B的数量必须等于target
            int rowB = count(picture[i].begin(), picture[i].end(), 'B');
            if (rowB != target) continue;
            // 相同行模式出现的次数必须等于target
            if (rowPatternCount[rowStr[i]] != target) continue;
            
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B' && colCount[j] == target) {
                    result++;
                }
            }
        }
        return result;
    }
};
```

### 解法二：暴力验证

```cpp
class Solution {
public:
    int findBlackPixel(vector<vector<char>>& picture, int target) {
        int m = picture.size(), n = picture[0].size();
        vector<int> rowCount(m, 0), colCount(n, 0);
        
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (picture[i][j] == 'B') {
                    rowCount[i]++;
                    colCount[j]++;
                }
        
        int result = 0;
        for (int i = 0; i < m; i++) {
            if (rowCount[i] != target) continue;
            for (int j = 0; j < n; j++) {
                if (picture[i][j] != 'B' || colCount[j] != target) continue;
                // 验证列j中所有有B的行是否和行i相同
                bool valid = true;
                for (int r = 0; r < m && valid; r++) {
                    if (picture[r][j] == 'B' && r != i) {
                        for (int c = 0; c < n; c++) {
                            if (picture[r][c] != picture[i][c]) {
                                valid = false;
                                break;
                            }
                        }
                    }
                }
                if (valid) result++;
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 只检查 rowCount 和 colCount 等于 target | ✓ 还需要检查行模式出现次数等于 target |
| ✗ 行模式比较只比较 B 的位置 | ✓ 必须整行所有字符完全一致 |
| ✗ 忘记检查 `picture[i][j] == 'B'` | ✓ 只对 B 位置计数 |
| ✗ rowPatternCount 条件写成 >= target | ✓ 必须恰好等于 target |

## 面试追问

**Q1: 为什么行模式出现次数 == target 就能保证"列中所有有B的行与行r相同"？**

> 因为 colCount[j] == target 意味着列 j 有 target 个 B。如果行模式相同的行有 target 个，且这些行在列 j 都有 B（因为模式相同），那么列 j 的所有 B 都来自这些相同的行。

**Q2: 行字符串的空间开销能否优化？**

> 可以用行的哈希值代替完整字符串，但需要处理哈希冲突。实际面试中字符串方案足够清晰。

**Q3: 与 Lonely Pixel I 相比，时间复杂度有变化吗？**

> 理论上都是 O(m*n)，但 II 需要额外的字符串构建和哈希操作，常数更大。

## 相关题型

- [531. 孤独像素 I](https://leetcode.com/problems/lonely-pixel-i/) - 简化版，target=1 无行相同约束
- [249. 移位字符串分组](https://leetcode.com/problems/group-shifted-strings/) - 字符串编码分组
- [49. 字母异位词分组](https://leetcode.com/problems/group-anagrams/) - 哈希分组思想
