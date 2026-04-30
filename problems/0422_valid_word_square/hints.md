# 422. Valid Word Square - 有效的单词方块

## 核心思路
逐元素检查矩阵是否关于主对角线对称：`words[i][j] == words[j][i]`，注意处理不等长字符串的越界情况。

## 思维链
1. 遍历每个位置 (i, j)
2. 检查 `words[i][j]` 是否等于 `words[j][i]`
3. 若 j 超出 words 的行数，或 i 超出 words[j] 的长度，则不对称 → false
4. 所有位置都满足 → true

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1 | 逐元素对称性检查 ⭐ | O(n*m) | O(1) | 首选 |
| 解法2 | 构造转置后比较 | O(n*m) | O(n*m) | 直观但费空间 |

## 关键提示
1. 字符串长度可能不同，访问前必须做边界检查
2. 只需检查上三角（或遍历所有位置），关键是 `words[i][j]` 存在时 `words[j][i]` 也必须存在且相等
3. 行数不等于最长字符串长度也可能导致 false

## 解法详解

### 解法1: 逐元素对称性检查
```cpp
class Solution {
public:
    bool validWordSquare(vector<string>& words) {
        int n = words.size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < (int)words[i].size(); j++) {
                // j >= 行数 或 i >= words[j]长度 → 越界
                if (j >= n || i >= (int)words[j].size())
                    return false;
                if (words[i][j] != words[j][i])
                    return false;
            }
        }
        return true;
    }
};
```

### 解法2: 构造转置后比较
```cpp
class Solution {
public:
    bool validWordSquare(vector<string>& words) {
        int n = words.size();
        int maxLen = 0;
        for (auto& w : words) maxLen = max(maxLen, (int)w.size());
        if (maxLen != n) return false; // 行列数不一致
        for (int j = 0; j < n; j++) {
            string col;
            for (int i = 0; i < n; i++) {
                if (j < (int)words[i].size()) col += words[i][j];
            }
            if (col != words[j]) return false;
        }
        return true;
    }
};
```

## 易错点
| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 不检查 j >= n（列索引超出行数） | ✓ j >= n 时返回 false |
| ✗ 不检查 i >= words[j].size() | ✓ i 超出第 j 行长度时返回 false |
| ✗ 用 words[i].size() 做 int 比较不 cast | ✓ 显式转 (int) 避免无符号比较问题 |

## 面试追问
**Q1: 时间复杂度是多少？**
> O(n * m)，其中 n 为行数，m 为最长字符串长度。每个字符最多被访问常数次。

**Q2: 能否只检查上三角？**
> 理论上可以，但需要额外处理长度不等的边界情况。直接遍历所有位置代码更简洁且不影响复杂度。

**Q3: 这道题和 425. Word Squares 的关系？**
> 422 是验证一个给定数组是否构成 Word Square，425 是从词库中找出所有可能的 Word Square 组合（需要 Trie + 回溯）。

## 相关题型
- [425. Word Squares](https://leetcode.com/problems/word-squares/) - 构造所有单词方块
- [48. Rotate Image](https://leetcode.com/problems/rotate-image/) - 矩阵对称操作
