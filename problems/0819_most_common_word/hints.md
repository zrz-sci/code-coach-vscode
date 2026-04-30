# 819. 最常见的单词

## 核心思路

典型的**字符串解析 + 哈希计数**问题。先把段落中的单词提取出来（忽略标点、统一小写），用哈希表统计每个非禁用单词的出现次数，返回出现最多的那个。

## 思维链

1. **读完题第一反应** → 分词 + 计数 + 排除禁用词。标准的字符串处理题。
2. **分词的难点在哪？** → 段落中有标点符号（`!?',;.`）和空格作为分隔符。不能简单按空格分割，因为 `"ball,"` 应该提取为 `"ball"`。
3. **怎么处理？** → 遍历字符串，遇到字母就收集进当前单词，遇到非字母就结束当前单词。或者先把所有非字母字符替换为空格，再按空格分割。
4. **大小写** → 题目说 case-insensitive，全部转小写后再处理。
5. **禁用词查找** → 用 `unordered_set` 存禁用词，O(1) 判断。
6. **最终答案** → 在哈希计数中找频次最大的非禁用词。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 手动分词 + 哈希计数 | 遍历字符逐个提取单词 | O(n + m) | O(n + m) | 面试首选 |
| 正则表达式 | 用 regex 提取单词 | O(n + m) | O(n + m) | 代码简洁但面试不推荐 |

其中 n = paragraph 长度，m = banned 总长度。

## 关键提示

1. **统一转小写**：在处理之前把整个段落转成小写，避免后续到处 tolower。
2. **分词策略**：遇到字母字符 `isalpha(c)` 就加入当前单词，遇到非字母就结束当前单词并处理。别忘了字符串末尾如果是字母，最后一个单词也要处理。
3. **禁用词用 set 存**：频繁查找用 `unordered_set` 而非 vector 的线性查找。
4. **边分词边计数**：不需要先提取所有单词再计数，可以提取一个就处理一个，节省空间。
5. **空单词过滤**：连续非字母字符可能产生空字符串，要过滤掉。

## 解法详解

### 解法1: 手动分词 + 哈希计数 — O(n + m) / O(n + m)

**思考过程**: 最直接的做法——遍历段落，提取单词，统计频次，找最大值。

```
示例: paragraph = "Bob hit a ball, the hit BALL flew far after it was hit."
      banned = ["hit"]

Step 1: 转小写 → "bob hit a ball, the hit ball flew far after it was hit."
Step 2: 分词 → ["bob", "hit", "a", "ball", "the", "hit", "ball", "flew", "far", "after", "it", "was", "hit"]
Step 3: 禁用集合 = {"hit"}
Step 4: 计数(排除禁用):
  bob:1, a:1, ball:2, the:1, flew:1, far:1, after:1, it:1, was:1
Step 5: 最大频次 → "ball" (2次)
```

```cpp
class Solution {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        // 禁用词集合
        unordered_set<string> ban(banned.begin(), banned.end());
        // 单词频次
        unordered_map<string, int> count;
        
        string word;
        string result;
        int maxCount = 0;
        
        // 遍历段落，提取并统计单词
        for (int i = 0; i <= paragraph.size(); i++) {
            if (i < paragraph.size() && isalpha(paragraph[i])) {
                word += tolower(paragraph[i]);
            } else {
                if (!word.empty() && !ban.count(word)) {
                    count[word]++;
                    if (count[word] > maxCount) {
                        maxCount = count[word];
                        result = word;
                    }
                }
                word.clear();
            }
        }
        
        return result;
    }
};
```

**关键点**:
- 循环到 `i <= paragraph.size()`（多走一步），确保最后一个单词也被处理。
- `tolower` 逐字符转换，避免遗漏。
- 每个单词提取后立即检查是否被禁用并计数，不需要分两步。

---

### 解法1 变体: 预处理替换法

**不同角度**: 先把所有非字母字符替换为空格，然后用 `istringstream` 按空格分割。

```cpp
class Solution {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        unordered_set<string> ban(banned.begin(), banned.end());
        unordered_map<string, int> count;
        
        // 非字母字符全部替换为空格，同时转小写
        for (char& c : paragraph) {
            if (isalpha(c)) {
                c = tolower(c);
            } else {
                c = ' ';
            }
        }
        
        // 用 istringstream 按空格分割
        istringstream iss(paragraph);
        string word, result;
        int maxCount = 0;
        
        while (iss >> word) {
            if (!ban.count(word)) {
                count[word]++;
                if (count[word] > maxCount) {
                    maxCount = count[word];
                    result = word;
                }
            }
        }
        
        return result;
    }
};
```

**关键点**:
- `istringstream` 的 `>>` 操作自动跳过连续空格，不会产生空字符串。
- 修改了原始 paragraph 字符串（如果不想修改，可以复制一份）。

## 易错点

1. **忘记转小写**：
   - 错误：直接统计 "Bob" 和 "bob" 作为不同单词
   - 正确：全部转成小写后再处理

2. **忘记处理最后一个单词**：
   - 错误：如果段落以字母结尾（如 "a."），循环到最后一个字符是 '.'，当前 word = "a" 被处理了；但如果段落是 "hello"，最后没有非字母字符来触发处理
   - 正确：循环多走一步到 `i == paragraph.size()`，此时 `i < paragraph.size()` 为 false，触发处理最后一个单词

3. **banned 的查找效率**：
   - 错误：每次用 `find(banned.begin(), banned.end(), word)` 线性查找
   - 正确：预先建立 `unordered_set`

4. **空单词**：
   - 连续标点如 `"a,,b"` 分词后可能产生空字符串
   - 在计数前要检查 `!word.empty()`

5. **标点种类遗漏**：
   - 题目列出了 `!?',;.` 和空格，但用 `isalpha` 判断更安全——只要不是字母就当分隔符

## 面试追问

**Q1: 时间和空间复杂度？**
> 时间 O(n + m)：n 是 paragraph 长度（遍历一次），m 是 banned 总长度（建集合）。空间 O(n + m)：哈希表和集合。

**Q2: 如果 paragraph 非常大（GB 级别），怎么优化？**
> 流式处理：不需要把整个 paragraph 加载到内存。逐字符读取，边分词边计数。最终只保留哈希表（大小 = 不同单词数）。可以用 MapReduce 并行化。

**Q3: 如果有多个出现次数相同的非禁用词（都是最大频次），怎么办？**
> 题目保证答案唯一。但如果需要返回所有，遍历完后扫描哈希表找所有频次 == maxCount 的词即可。

**Q4: 能否不用哈希表？**
> 可以用 Trie（前缀树）存储单词和频次。插入和查找都是 O(L)（L 是单词长度），和哈希表的平均性能相当。但实现更复杂，面试中不推荐。

**Q5: banned 列表中可能有重复吗？**
> 可能有，但放入 `unordered_set` 自动去重，不影响正确性。

## 相关题型

- **692. 前 K 个高频单词** — 也是单词计数问题，但需要返回前 K 个而非第 1 个。用堆（priority_queue）或排序。本题是 K=1 的特例。
- **387. 字符串中的第一个唯一字符** — 字符频次统计的简化版。用哈希计数后找频次为 1 的第一个字符。
- **49. 字母异位词分组** — 也涉及字符串预处理和哈希分组。不同的是分组而非计数。
- **242. 有效的字母异位词** — 字符频次比较，哈希计数的基础应用。
