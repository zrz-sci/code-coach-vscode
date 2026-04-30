# 411. Minimum Unique Word Abbreviation - 最短独占单词缩写

## 核心思路
给定 target 和字典，找 target 的最短缩写使其不是字典中任何单词的缩写。核心是**位掩码 + 暴力/贪心**：用位掩码表示 target 的哪些位置保留字符（1=保留，0=数字替换），先过滤字典中与 target 等长的词，对每个词计算"必须区分位"的掩码，然后找最短的合法缩写。

## 思维链
1. 缩写可以用 bitmask 表示：位为 1 表示保留字符，位为 0 表示用数字替换
2. 过滤字典：只保留与 target 等长的单词（不等长的缩写不会冲突）
3. 对每个字典单词，计算 diff mask：target 与 word 在哪些位置字符不同
4. 合法缩写 = 缩写的 mask 与每个 diff mask 的交集非空（至少有一个保留位能区分）
5. 在所有合法 mask 中找"缩写长度"最短的

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 位掩码枚举 | O(2^m * n) | O(n) | 直观但可能 TLE（m<=21） |
| ⭐ 位掩码+剪枝 | O(2^m * n) 最坏 | O(n) | 按缩写长度从小到大 BFS |

## 关键提示
1. **缩写长度计算**：mask 中 1 的个数 + 连续 0 段的个数（每段数字计 1 长度）
2. **diff 掩码**：`diff[i] = target 与 dict[i] 不同字符的位集合`
3. **合法条件**：`mask & diff[i] != 0` 对所有字典词成立（即保留的字符中至少有一个能区分）
4. **优化**：只需关注等长字典词；mask 按 popcount 从小到大枚举可早停

## 解法详解

### 解法一：位掩码枚举
```cpp
class Solution {
public:
    // 计算缩写长度：连续0段计为1，每个1计为1
    int abbrLen(int mask, int m) {
        int len = 0;
        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) {
                len++;  // 保留字符
            } else {
                len++;  // 数字段开始
                while (i < m && !(mask & (1 << i))) i++;
                i--;
            }
        }
        return len;
    }
    
    string minAbbreviation(string target, vector<string>& dictionary) {
        int m = target.size();
        // 计算 diff masks
        vector<int> diffs;
        for (auto& w : dictionary) {
            if ((int)w.size() != m) continue;
            int d = 0;
            for (int i = 0; i < m; i++)
                if (target[i] != w[i]) d |= (1 << i);
            diffs.push_back(d);
        }
        if (diffs.empty()) return to_string(m); // 没有等长词，最短就是全数字
        
        int bestMask = (1 << m) - 1, bestLen = m;
        for (int mask = 0; mask < (1 << m); mask++) {
            bool valid = true;
            for (int d : diffs) {
                if ((mask & d) == 0) { valid = false; break; }
            }
            if (valid) {
                int len = abbrLen(mask, m);
                if (len < bestLen) { bestLen = len; bestMask = mask; }
            }
        }
        // 根据 bestMask 生成缩写字符串
        string res;
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            if (bestMask & (1 << i)) {
                if (cnt > 0) { res += to_string(cnt); cnt = 0; }
                res += target[i];
            } else {
                cnt++;
            }
        }
        if (cnt > 0) res += to_string(cnt);
        return res;
    }
};
```

### 解法二：位掩码 + 贪心集合覆盖
```cpp
class Solution {
public:
    string minAbbreviation(string target, vector<string>& dictionary) {
        int m = target.size();
        vector<int> diffs;
        for (auto& w : dictionary) {
            if ((int)w.size() != m) continue;
            int d = 0;
            for (int i = 0; i < m; i++)
                if (target[i] != w[i]) d |= (1 << i);
            if (d == 0) return ""; // target 在字典中（题目保证不会）
            diffs.push_back(d);
        }
        if (diffs.empty()) return to_string(m);
        
        // 按缩写长度从小到大搜索
        int bestMask = (1 << m) - 1, bestLen = m;
        // 枚举所有 mask，按 popcount 分组
        for (int bits = 1; bits <= m; bits++) {
            // 枚举恰好有 bits 个 1 的 mask（Gosper's hack）
            int mask = (1 << bits) - 1;
            while (mask < (1 << m)) {
                bool valid = true;
                for (int d : diffs)
                    if ((mask & d) == 0) { valid = false; break; }
                if (valid) {
                    int len = 0, cnt = 0;
                    for (int i = 0; i < m; i++) {
                        if (mask & (1 << i)) {
                            if (cnt > 0) { len++; cnt = 0; }
                            len++;
                        } else cnt++;
                    }
                    if (cnt > 0) len++;
                    if (len < bestLen) { bestLen = len; bestMask = mask; }
                }
                // Gosper's hack: next mask with same popcount
                int c = mask & -mask, r = mask + c;
                mask = (((r ^ mask) >> 2) / c) | r;
            }
            if (bestLen <= bits + 1) break; // 不可能更优
        }
        string res;
        int cnt = 0;
        for (int i = 0; i < m; i++) {
            if (bestMask & (1 << i)) {
                if (cnt > 0) { res += to_string(cnt); cnt = 0; }
                res += target[i];
            } else cnt++;
        }
        if (cnt > 0) res += to_string(cnt);
        return res;
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 对所有字典词计算 diff | 只对等长词计算 diff | 不等长的词不可能有相同缩写 |
| 缩写长度 = popcount(mask) | 缩写长度 = 保留字符数 + 数字段数 | "a3e" 长度是 3 不是 2+3 |
| `mask & diff == 0` 判断为合法 | `mask & diff != 0` 才合法 | 至少一个保留位能区分才行 |

## 面试追问

**Q1: 为什么用 bitmask 表示缩写？**
> 缩写的本质是选择 target 的哪些位置保留原字符、哪些用数字替代。长度 m<=21，2^21 约 200 万，可枚举。

**Q2: Gosper's hack 是什么？**
> 一种位运算技巧，用于枚举所有恰好有 k 个 1 的 n 位二进制数。公式：`c = x & -x; r = x + c; next = (((r ^ x) >> 2) / c) | r`。

**Q3: 如果 m 很大（>21）怎么办？**
> 题目约束 `log2(n) + m <= 21`，保证总搜索空间可控。如果 m 更大，需要启发式搜索或贪心近似。

## 相关题型
- [320. Generalized Abbreviation](https://leetcode.com/problems/generalized-abbreviation/) - 生成所有缩写
- [408. Valid Word Abbreviation](https://leetcode.com/problems/valid-word-abbreviation/) - 验证缩写
- [527. Word Abbreviation](https://leetcode.com/problems/word-abbreviation/) - 批量最短缩写
