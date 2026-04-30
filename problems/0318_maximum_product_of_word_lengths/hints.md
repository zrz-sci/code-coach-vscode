# 318. 最大单词长度乘积 (Maximum Product of Word Lengths)

**难度**: Medium | **标签**: Bit Manipulation, Array, String

## 核心思路

本题的关键在于**高效判断两个单词是否有公共字母**。朴素的方法需要对每对单词的每个字符做交集检查，时间复杂度高。

利用**位掩码（Bitmask）**可以将判断优化到 O(1)：
- 每个单词用一个 32 位整数表示，第 i 位为 1 表示包含字母 `'a' + i`
- 两个单词没有公共字母 ⟺ `mask[i] & mask[j] == 0`
- 然后枚举所有单词对，找到无公共字母且长度乘积最大的

## 思维链

1. **暴力**：双重循环枚举所有对，逐字符判断是否有公共字母 → O(n^2 * L)
2. **优化字符判断**：英文小写字母只有 26 个 → 可以用 26 位的 bitmask 表示
3. **预处理**：先为每个单词计算 bitmask，判断公共字母变成位与操作 O(1)
4. **枚举**：双重循环 O(n^2)，配合 O(1) 的位操作判断 → 总 O(n^2 + nL)
5. **进阶优化**：相同 bitmask 的单词只保留最长的（用 map 去重）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 位掩码 + 枚举 | O(n^2 + nL) | O(n) | ⭐⭐⭐⭐⭐ | 标准解法 |
| 位掩码 + map 去重 | O(n^2 + nL) | O(n) | ⭐⭐⭐⭐ | 相同 mask 只保留最长 |

## 关键提示

1. **Bitmask 构建**：`mask |= (1 << (c - 'a'))`，c 是字母
2. **无公共字母判断**：`(mask[i] & mask[j]) == 0`
3. **26 位足够**：只有小写字母 a-z，一个 int（32 位）绑绑有余
4. **长度乘积**：`words[i].size() * words[j].size()`，不是 mask 的乘积
5. **相同 mask 优化**：如果多个单词有相同的字母集合，只需保留最长的那个

## 解法详解

### 解法一：位掩码 + 双重枚举（推荐）

```cpp
class Solution {
public:
    int maxProduct(vector<string>& words) {
        int n = words.size();
        vector<int> mask(n, 0);
        
        // 预处理：为每个单词生成 bitmask
        for (int i = 0; i < n; i++)
            for (char c : words[i])
                mask[i] |= (1 << (c - 'a'));
        
        int maxProd = 0;
        // 枚举所有单词对
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if ((mask[i] & mask[j]) == 0) // 无公共字母
                    maxProd = max(maxProd, (int)(words[i].size() * words[j].size()));
        
        return maxProd;
    }
};
```

### 解法二：位掩码 + HashMap 去重优化

```cpp
class Solution {
public:
    int maxProduct(vector<string>& words) {
        // 相同 mask 只保留最大长度
        unordered_map<int, int> maskToLen;
        for (auto& w : words) {
            int mask = 0;
            for (char c : w)
                mask |= (1 << (c - 'a'));
            maskToLen[mask] = max(maskToLen[mask], (int)w.size());
        }
        
        int maxProd = 0;
        for (auto& [m1, l1] : maskToLen)
            for (auto& [m2, l2] : maskToLen)
                if ((m1 & m2) == 0)
                    maxProd = max(maxProd, l1 * l2);
        
        return maxProd;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `mask[i] & mask[j] == 0` | ✓ `(mask[i] & mask[j]) == 0` | 运算符优先级：== 高于 & |
| ✗ 用 set 存字母判断交集 | ✓ 用 bitmask，位与判断 | set 交集 O(26)，位与 O(1) |
| ✗ 返回 mask 的乘积 | ✓ 返回 `words[i].size() * words[j].size()` | 题目要的是长度乘积 |
| ✗ `1 << (c - 'a')` 用 char 类型 | ✓ 确保是 int 类型的位移 | 避免潜在的类型问题 |
| ✗ 忘记处理空结果 | ✓ 初始化 maxProd = 0 | 没有满足条件的对时返回 0 |

## 面试追问

**Q1: 为什么用 bitmask 而不是 set？**
→ 26 个小写字母恰好可以用一个 int 的低 26 位表示。bitmask 的交集判断只需一次位与操作 O(1)，而 set 交集需要 O(26)。同时 bitmask 占用空间更小（4 字节 vs set 对象）。

**Q2: 时间复杂度分析？**
→ 预处理 bitmask O(n * L)，其中 L 是平均单词长度。枚举所有对 O(n^2)。总计 O(n^2 + nL)。用 HashMap 去重后，枚举变成 O(K^2)，K = 不同 mask 的数量（最多 2^26，但实际远小于 n）。

**Q3: 如果要求找出具体的两个单词而不只是最大乘积？**
→ 在更新 maxProd 时同时记录 best_i 和 best_j 的索引，最后返回 `{words[best_i], words[best_j]}`。

## 相关题型

- [187. Repeated DNA Sequences](https://leetcode.com/problems/repeated-dna-sequences/) - 位操作 + 字符编码
- [136. Single Number](https://leetcode.com/problems/single-number/) - 位操作基础（异或）
- [201. Bitwise AND of Numbers Range](https://leetcode.com/problems/bitwise-and-of-numbers-range/) - 位操作应用
- [290. Word Pattern](https://leetcode.com/problems/word-pattern/) - 字符串模式匹配
