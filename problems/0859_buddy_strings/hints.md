# 859. Buddy Strings

## 核心思路

本题要求判断能否通过恰好交换字符串 s 中的两个字符使其等于 goal。这是一道分类讨论题，需要考虑两种主要情况：

1. **s 和 goal 不相等时**：找出所有不同位置。如果恰好有 2 个位置不同，且交换后能匹配，则返回 true。
2. **s 和 goal 完全相等时**：需要存在至少一对相同的字符（即字符串中有重复字符），这样交换这对字符后字符串不变。

关键在于不遗漏任何边界情况：长度不同直接返回 false，完全相同时检查是否有重复字符。

## 思维链

1. **长度检查**：如果 s.length != goal.length，直接返回 false。
2. **找不同位置**：遍历两个字符串，记录所有 s[i] != goal[i] 的位置到列表 diff 中。
3. **分类讨论**：
   - diff.size() == 0（完全相同）：检查 s 中是否有重复字符。
   - diff.size() == 2：检查 s[diff[0]] == goal[diff[1]] 且 s[diff[1]] == goal[diff[0]]。
   - 其他情况：返回 false。
4. **返回结果**。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 分类讨论 + 哈希/数组 | O(n) | O(1) | 最优解，26个字母计数 |
| 分类讨论 + set | O(n) | O(n) | 用 set 判断重复字符 |

## 关键提示

1. **恰好交换一次**：题目要求必须交换（不能不交换），且恰好交换一次（不能交换多次）。
2. **完全相等的特殊情况**：当 s == goal 时，必须有重复字符才能"交换后不变"。
3. **字符频率必须相同**：如果 s 和 goal 的字符频率不同，无论怎么交换都不可能相等（虽然检查 diff 已经隐含了这一点）。
4. **只有小写字母**：可以用大小为 26 的数组代替哈希表。

## 解法详解

### 解法一：分类讨论（最优解）

**思路**：

首先检查长度是否相同。然后遍历字符串找出所有不同位置。根据不同位置的数量分类处理：

- **0 个不同位置**：s 和 goal 完全相同。检查 s 中是否有重复字符（用字符计数数组，如果任何字符出现 >= 2 次则可以交换）。
- **2 个不同位置**：设位置为 i 和 j，检查 s[i]==goal[j] 且 s[j]==goal[i]。
- **其他数量**：不可能通过一次交换实现，返回 false。

**步骤**：
1. 若 s.size() != goal.size()，返回 false。
2. 遍历，收集 diff 位置。
3. 若 diff 为空：用 freq[26] 计数，检查是否有 freq[c] >= 2。
4. 若 diff.size() == 2：交叉比较。
5. 否则返回 false。

**复杂度分析**：
- 时间：O(n) 一次遍历
- 空间：O(1) 只需常数额外空间（diff 最多存 3 个就可以提前返回，freq 大小固定 26）

### 解法二：利用 set 判断重复

**思路**：与解法一相同，但在 s == goal 的情况下，用 set 存储所有字符。如果 set.size() < s.size()，说明有重复字符。

## 易错点

1. **忘记处理 s == goal 的情况**：这是最常见的错误。当两个字符串相等时，必须有重复字符。
2. **忽略长度不同**：长度不同时直接返回 false，不需要进一步检查。
3. **diff 数量为 1 或 >= 3 时未正确返回 false**：一个不同位置无法通过交换修复，三个以上更不行。
4. **交叉比较时只检查了一半**：必须同时检查 s[i]==goal[j] AND s[j]==goal[i]。
5. **误以为可以交换 0 次**：题目说"swap two letters"，必须执行交换操作。如果 s 已经等于 goal 但没有重复字符，交换任何两个不同字符都会破坏等式。

## 面试追问

1. **如果允许最多交换一次（可以不交换）呢？**
   - 增加 diff.size() == 0 时直接返回 true 的分支，不需要检查重复字符。

2. **如果允许交换 k 次呢？**
   - 需要检查不同位置的数量 <= 2k，且字符频率匹配。对于偶数个不同位置可以用 k 次交换修复。

3. **如果是在两个不同的字符串 s 和 goal 之间交换（s 的一个字符和 goal 的一个字符交换）呢？**
   - 完全不同的问题，需要考虑跨字符串交换的效果。

4. **如何扩展到 Unicode 字符？**
   - 将 freq[26] 替换为 unordered_map<char, int> 或 unordered_set<char>。

5. **能否在一次遍历中同时完成所有检查？**
   - 可以。遍历时同时记录 diff 位置和字符频率。若 diff 超过 2 个立即返回 false。

## 相关题型

- [242. Valid Anagram](https://leetcode.com/problems/valid-anagram/) - 字符频率比较
- [1657. Determine if Two Strings Are Close](https://leetcode.com/problems/determine-if-two-strings-are-close/) - 字符串变换可达性
- [1790. Check if One String Swap Can Make Strings Equal](https://leetcode.com/problems/check-if-one-string-swap-can-make-strings-equal/) - 非常相似，但是两个不同字符串之间交换
- [299. Bulls and Cows](https://leetcode.com/problems/bulls-and-cows/) - 位置匹配 + 字符频率
- [438. Find All Anagrams in a String](https://leetcode.com/problems/find-all-anagrams-in-a-string/) - 字符频率 + 滑动窗口
