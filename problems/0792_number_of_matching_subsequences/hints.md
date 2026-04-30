# 792. Number of Matching Subsequences (匹配子序列的单词数)

## 核心思路

给定字符串 s 和单词列表 words，统计有多少单词是 s 的子序列。暴力逐词检查是 O(|words| * |s|)。优化方法有两种主流思路：(1) **字符桶 + 并行推进**——将所有单词按"当前待匹配字符"分桶，遍历 s 一次即可同时匹配所有单词；(2) **二分查找**——预处理 s 中每个字符出现的位置列表，对每个单词用 upper_bound 查找下一个匹配位置。

## 思维链

1. **暴力思路**：对每个 word，用双指针扫描 s 判断是否为子序列。时间 O(|words| * |s|)，当 |s| = 5*10^4，|words| = 5000 时约 2.5*10^8，可能超时。
2. **优化一（桶分组）**：所有单词同时匹配。按当前需要匹配的字符将单词分到 26 个桶中。遍历 s 的每个字符 c 时，取出桶 c 中的所有单词，推进它们的匹配指针，然后放入新桶。匹配完成的单词计入答案。
3. **优化二（二分查找）**：预处理 s，对每个字符记录其出现的所有下标位置。对每个 word，从左到右逐字符在对应位置列表中二分查找（upper_bound），找到严格大于当前位置的下一个出现。
4. **时间比较**：桶方法 O(|s| + sum(|word_i|))，二分方法 O(sum(|word_i| * log|s|) + |s|)。桶方法通常更快。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 暴力双指针 | O(W * S) | O(1) | 简单但可能超时 |
| 字符桶并行推进 | O(S + sum(len_i)) | O(W) | 最优，推荐 |
| 二分查找 | O(sum(len_i * logS) + S) | O(S) | 预处理后单词间独立 |
| 桶方法 + 去重优化 | O(S + sum(unique_len_i)) | O(W) | 适合大量重复单词 |

其中 S = |s|, W = |words|, len_i = |words[i]|。

## 关键提示

1. **子序列判定的本质**：按顺序逐字符匹配，可以跳过中间字符，但不能改变顺序。
2. **桶方法的关键**：每个单词用一个迭代器 (word_index, char_index) 表示。按"当前需要的字符"入桶。
3. **二分查找的关键**：`upper_bound(positions[c], cur_pos)` 找到字符 c 在 s 中第一个大于 cur_pos 的位置。如果找不到则该 word 不是子序列。
4. **words 中可能有重复单词**：每个都要独立检查（或用 map 去重后乘以次数）。
5. **空字符串是任何字符串的子序列**：但题目保证 words[i].length >= 1。

## 解法详解

### 解法一：字符桶并行推进（推荐面试首选）

**核心思想**：将所有 word 的匹配状态并行管理。

**数据结构**：`vector<vector<pair<int,int>>> buckets(26)`，buckets[c] 中存储所有"当前需要字符 c"的单词，用 (word_index, char_position) 表示。

**算法**：
1. 初始化：对每个 word_i，将 (i, 0) 放入 buckets[words[i][0] - 'a']。
2. 遍历 s 的每个字符 c：
   - 取出 buckets[c - 'a'] 中所有元素（注意取出后清空该桶）
   - 对每个 (wi, ci)：如果 ci+1 == words[wi].size()，说明匹配完成，count++
   - 否则，将 (wi, ci+1) 放入 buckets[words[wi][ci+1] - 'a']
3. 返回 count。

**复杂度**：遍历 s 一次 O(S)，每个单词的每个字符最多进出桶一次 O(sum(len_i))。总计 O(S + sum(len_i))。

### 解法二：二分查找

**预处理**：`vector<vector<int>> charPos(26)`，charPos[c] 存储字符 c 在 s 中所有出现位置（升序）。

**查询**：对每个 word，维护 cur = -1（当前在 s 中的匹配位置），逐字符 c：
- 在 charPos[c - 'a'] 中 upper_bound(cur) 找下一个位置
- 找不到则 word 不是子序列
- 找到则 cur = 该位置

**复杂度**：预处理 O(S)，每个 word 查询 O(len * logS)。总 O(S + sum(len_i * logS))。

## 易错点

1. **桶方法中取出后忘记清空**：遍历 buckets[c] 的元素时要先 swap/move 出来或复制，否则往桶里加新元素会导致迭代器失效。
2. **二分查找的搜索目标**：要找 **严格大于** cur_pos 的位置，所以用 upper_bound(cur_pos)，不是 lower_bound。
3. **words 中有相同单词**：不能去重后只算一次，除非乘以出现次数。
4. **桶方法的初始化**：要按 words[i] 的第 0 个字符分桶，不是按 words[i] 本身。
5. **s 中没有某字符**：如果 word 需要的某字符在 s 中根本不存在，直接判定不匹配。可以提前用 set 过滤。

## 面试追问

1. **Q: 如果 words 中有大量重复单词，如何优化？** A: 先用 map 统计每个独立 word 的出现次数，只对不重复的 word 做匹配，最后结果乘以次数。
2. **Q: 如果 s 是动态变化的（字符会被插入/删除），哪种方法更好？** A: 二分查找方法更好，因为只需要更新 charPos 数组。桶方法需要重新遍历 s。
3. **Q: 如何判断一个单词是否是另一个的子序列？** A: 双指针法 O(N+M)，但这里需要判断多个 word 对同一个 s，所以桶方法或预处理更优。
4. **Q: 能否用 Trie 优化？** A: 可以将 words 建 Trie，然后在 Trie 上做类似桶推进。当 words 有大量公共前缀时效果好。但实现复杂度高，面试中不推荐。
5. **Q: 时间复杂度的下界是什么？** A: 至少需要 O(S + sum(len_i))，因为需要读取所有输入。桶方法已达到下界。

## 相关题型

- [392. Is Subsequence](../0392_is_subsequence/) - 子序列判定基础版
- [524. Longest Word in Dictionary through Deleting](../0524_longest_word_in_dictionary_through_deleting/) - 子序列匹配 + 最长
- [1055. Shortest Way to Form String](../1055_shortest_way_to_form_string/) - 子序列拼接
- [2825. Make String a Subsequence Using Cyclic Increments](../2825_make_string_a_subsequence_using_cyclic_increments/) - 子序列变体
- [727. Minimum Window Subsequence](../0727_minimum_window_subsequence/) - 子序列最小窗口
