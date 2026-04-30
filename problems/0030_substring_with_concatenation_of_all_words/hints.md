# 30. 串联所有单词的子串

## 核心思路

本题本质是**滑动窗口 + 哈希表**：在字符串 `s` 中找到所有起点，使得从该起点开始的子串恰好由 `words` 中所有单词拼接而成（顺序任意）。关键突破口是 **words 中每个单词长度相同**，这意味着我们可以把字符串按固定步长切分成"单词槽"，然后用滑动窗口高效匹配。

## 思维链

1. **读完题第一反应**：要找 `words` 所有排列的拼接在 `s` 中出现的位置。如果 `words` 有 n 个单词，排列有 n! 种，暴力枚举排列再匹配完全不可行。

2. **关键观察1 — 排列等价于计数**：我们不需要真的枚举排列。一个子串是 `words` 的某种排列拼接，等价于：把子串按单词长度切分后，每个"块"出现的次数恰好等于 `words` 中对应单词的频次。所以用 **哈希表记录频次** 比较即可。

3. **关键观察2 — 所有单词等长**：设单词长度为 `w`，单词个数为 `n`，则目标子串的总长度固定为 `w * n`。我们可以从每个位置 `i` 截取长度 `w*n` 的子串，然后每 `w` 个字符切一块，检查频次是否匹配。这是 O(s_len * n * w) 的暴力法。

4. **能不能更快？** 注意到当我们从位置 `i` 移动到 `i + w` 时，窗口只是"吐出"最左边一个单词、"吃进"最右边一个单词。这是典型的 **固定大小滑动窗口** 模式！但窗口的滑动步长是 `w`（一个单词的长度），而非 1。

5. **起始偏移量**：如果我们以步长 `w` 滑动窗口，起始位置只有 `0, 1, 2, ..., w-1` 这 `w` 种偏移。对每种偏移分别跑一轮滑动窗口，每轮内部窗口步进 `w`，总共处理 `s_len` 个字符，所以总时间 O(s_len * w)。相比暴力法有明显提升。

6. **窗口内维护什么？** 一个 `window` 哈希表记录当前窗口内各单词的出现次数，一个计数器 `count` 记录已匹配的单词总数。当 `count == wordCount` 时，整个窗口匹配成功。

```
窗口滑动示意（w=3, n=3, 总长9, words=["bar","foo","the"]）:

s = "barfoofoobarthefoobarman"
偏移0 (从位置0开始，步长3):
  right=0:  加入 "bar" → window={bar:1}, count=1
  right=3:  加入 "foo" → window={bar:1,foo:1}, count=2
  right=6:  加入 "foo" → window={bar:1,foo:2}, count=3
            foo 超过目标(1)! → 收缩: 移除 left=0 "bar", count=2, left=3
            继续收缩: 移除 left=3 "foo", count=1, left=6
  right=9:  加入 "bar" → window={foo:1,bar:1}, count=2
  right=12: 加入 "the" → window={foo:1,bar:1,the:1}, count=3
            count==3! → 记录 left=6 ✓
            收缩: 移除 left=6 "foo", count=2, left=9
  right=15: 加入 "foo" → window={bar:1,the:1,foo:1}, count=3
            count==3! → 记录 left=9 ✓
            收缩: 移除 left=9 "bar", count=2, left=12
  right=18: 加入 "bar" → window={the:1,foo:1,bar:1}, count=3
            count==3! → 记录 left=12 ✓
            收缩: 移除 left=12 "the", count=2, left=15
  right=21: 加入 "man" → 不在 target 中! → 重置窗口

结果: [6, 9, 12] ✓
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力哈希表 | 每个起点截取子串，切分后比频次 | O(s_len * n * w) | O(n * w) | 能说出即可 |
| 滑动窗口 + 哈希表 | w种偏移，固定窗口滑动 | O(s_len * w) | O(n * w) | ⭐ 必须写出 |

## 关键提示

1. **偏移量是精髓**：滑动窗口的起始位置不是从 0 到 `s_len`，而是 `0, 1, ..., w-1`。每种偏移独立跑一轮窗口，窗口步长为 `w`。忘记偏移量是最常见的错误。

2. **words 可能有重复单词**：比如 `words = ["word","good","best","word"]`，`"word"` 出现了2次。必须用频次哈希表而非集合。

3. **窗口收缩条件**：当某个单词在窗口中的出现次数**超过**目标频次时，需要从左侧收缩窗口，逐个移除最左的单词，直到该单词次数不超。

4. **遇到无效单词要重置**：当右侧加入的单词不在 `target` 中时，当前窗口作废。清空 `window`，`left` 跳到 `right + w`，`count = 0`。

5. **边界检查**：`right + wordLen <= sLen` 用 `<=` 而非 `<`。如果 `s.length() < w * n`，直接返回空。

## 解法详解

### 解法1: 暴力哈希表 — O(s_len * n * w) / O(n * w)

**思考过程**：最直接的方式——枚举每个起始位置 `i`，截取长度为 `w*n` 的子串，按 `w` 的步长切分成 `n` 个单词，统计频次后和目标频次比较。遇到不在 target 中的词或超频的词，立即剪枝。

```cpp
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (words.empty() || s.empty()) return res;
        
        int wordLen = words[0].size();
        int wordCount = words.size();
        int totalLen = wordLen * wordCount;
        
        // 构建目标频次表
        unordered_map<string, int> target;
        for (auto& w : words) target[w]++;
        
        // 枚举每个起始位置
        for (int i = 0; i + totalLen <= (int)s.size(); i++) {
            unordered_map<string, int> seen;
            int j = 0;
            for (; j < wordCount; j++) {
                string word = s.substr(i + j * wordLen, wordLen);
                // 不在 target 中，直接跳出
                if (target.find(word) == target.end()) break;
                seen[word]++;
                // 出现次数超过目标，也跳出
                if (seen[word] > target[word]) break;
            }
            if (j == wordCount) res.push_back(i);
        }
        return res;
    }
};
```

**复杂度分析**：外层 O(s_len) 个起点，内层每次切 n 个单词、每个 `substr` 花 O(w)，总共 O(s_len * n * w)。空间 O(n*w) 用于哈希表和子串。

### 解法2: 滑动窗口 + 哈希表（最优解）— O(s_len * w) / O(n * w) ⭐

**思考过程**：
- 从暴力法出发，发现相邻起点（差 `w`）的窗口内容只差一个单词的进出。
- 用滑动窗口避免重复计算：左右指针以 `w` 为步长移动。
- 需要 `w` 种偏移量分别处理，每种偏移内独立运行一个窗口。
- 遇到无效单词时，彻底重置窗口跳过。

```cpp
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (words.empty() || s.empty()) return res;
        
        int sLen = s.size();
        int wordLen = words[0].size();
        int wordCount = words.size();
        int totalLen = wordLen * wordCount;
        if (sLen < totalLen) return res;
        
        // 目标频次表
        unordered_map<string, int> target;
        for (auto& w : words) target[w]++;
        
        // 对每种偏移量 offset = 0, 1, ..., wordLen-1 分别跑滑动窗口
        for (int offset = 0; offset < wordLen; offset++) {
            unordered_map<string, int> window;
            int count = 0;       // 窗口内匹配的单词总数
            int left = offset;   // 窗口左边界
            
            for (int right = offset; right + wordLen <= sLen; right += wordLen) {
                // 取出右侧新进入的单词
                string wordIn = s.substr(right, wordLen);
                
                if (target.count(wordIn)) {
                    // 有效单词，加入窗口
                    window[wordIn]++;
                    count++;
                    
                    // 如果该单词次数超过目标，从左侧收缩
                    while (window[wordIn] > target[wordIn]) {
                        string wordOut = s.substr(left, wordLen);
                        window[wordOut]--;
                        count--;
                        left += wordLen;
                    }
                    
                    // 检查是否完全匹配
                    if (count == wordCount) {
                        res.push_back(left);
                        // 移除最左单词，为下一个位置做准备
                        // （也可以不移除，下一轮循环自然处理）
                    }
                } else {
                    // 无效单词，彻底重置窗口
                    window.clear();
                    count = 0;
                    left = right + wordLen;
                }
            }
        }
        return res;
    }
};
```

**为什么窗口步长是 `w` 而非 1？**

因为所有单词等长，合法的拼接一定是从某个对齐偏移开始的。以 `w=3` 为例：
```
偏移0的对齐点: 0, 3, 6, 9, 12, ...
偏移1的对齐点: 1, 4, 7, 10, 13, ...
偏移2的对齐点: 2, 5, 8, 11, 14, ...
```
不对齐的位置不可能切出完整单词。所以只需 `w` 种偏移就能覆盖所有可能。

**为什么匹配后不需要主动收缩？**

当 `count == wordCount` 时，我们记录了 `left` 作为答案。接下来 `right` 继续右移 `w`，新加入一个单词后 `count` 变成 `wordCount + 1`，while 循环自然会收缩 `left` 来保持窗口大小。

## 解法对比

| 维度 | 暴力哈希表 | 滑动窗口 |
|------|-----------|---------|
| 时间复杂度 | O(s_len * n * w) | O(s_len * w) |
| 核心思想 | 每个起点独立统计 | 增量更新，进出一个词 |
| 代码复杂度 | 简单（15行核心） | 中等（需要理解偏移和三种情况） |
| 对 words 重复处理 | 自然支持 | 自然支持 |
| 面试推荐度 | 先说这个展示理解 | 必须最终写出 |

## 易错点

1. **忘记 `w` 种偏移量**：直接从 0 开始步进 `w`，漏掉了偏移 1, 2, ..., w-1 的情况。例如 `s = "afoobar"`, `words = ["foo","bar"]`，正确答案是 `[1]`，偏移 0 会漏掉。

2. **遇到无效单词未重置窗口**：当右侧加入的单词不在 `target` 中时，当前窗口作废。如果只是 `continue` 而不清空 `window` 和重置 `left`，后续匹配会出错。正确做法：
   ```cpp
   // 错误: 只跳过这个词
   if (!target.count(wordIn)) continue;  // BUG!
   
   // 正确: 彻底重置
   if (!target.count(wordIn)) {
       window.clear();
       count = 0;
       left = right + wordLen;
   }
   ```

3. **窗口超过 n 个单词时没有收缩**：只判断 `count == wordCount` 就加入结果，忽略了窗口可能包含额外的有效单词。用 `while` 收缩超频单词来保证正确性。

4. **substr 越界**：循环条件应为 `right + wordLen <= sLen` 而非 `right + wordLen < sLen`，差一错误非常经典。

5. **words 有重复但用了 set**：`words = ["word","word"]`，如果用 set 去重就会误以为只需要匹配 1 个 "word"。必须用 `unordered_map` 记频次。

## 面试追问

1. **"如果 words 中的单词长度不同呢？"**
   → 偏移量 + 固定步长的技巧失效。退化为 Aho-Corasick 多模式匹配 + 某种状态机计数，复杂度和实现难度大增。可以提到 AC 自动机或 Trie + DFS 作为思路方向。

2. **"能否用 rolling hash 优化 substr 的开销？"**
   → 可以。每次 `substr` 开销 O(w)，可以用 Rabin-Karp 滚动哈希在 O(1) 时间计算单词哈希值，将整体复杂度降到 O(s_len)。但需要处理哈希冲突，实际面试中一般不要求实现。

3. **"如果 s 非常长（10^6）而 words 很少（5个长度3的词），哪个解法更好？"**
   → 滑动窗口明显更优。暴力法 O(s_len * n * w) = O(10^6 * 5 * 3) = 1.5 * 10^7，滑动窗口 O(s_len * w) = O(10^6 * 3) = 3 * 10^6，快 5 倍。

4. **"窗口法中，如何处理 words 全部相同的情况？例如 words = ['aa','aa','aa']？"**
   → 没有特殊处理，频次表 `target = {"aa": 3}`，窗口正常运作。`s = "aaaaaa"` 时偏移 0 的窗口匹配 "aa|aa|aa" 得到位置 0，偏移 1 也可能匹配。不同偏移独立运行不会重复。

5. **"时间复杂度 O(s_len * w) 能否进一步优化？"**
   → 理论下界是 O(s_len)（需要读完整个字符串）。用 rolling hash 避免 substr 可做到 O(s_len) + O(n*w) 预处理。但面试中写出 O(s_len * w) 的滑动窗口已经是最优答案。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 76 | Minimum Window Substring | 经典滑动窗口 + 频次匹配，本题的基础模型 |
| 438 | Find All Anagrams in a String | 固定窗口 + 频次，本题的字符级简化版 |
| 567 | Permutation in String | 判断s2是否包含s1的排列，固定窗口 |
| 3 | Longest Substring Without Repeating Characters | 滑动窗口基础入门 |
| 187 | Repeated DNA Sequences | 固定长度子串 + 哈希 |
| 49 | Group Anagrams | 字符频次作为key，相关思维 |
