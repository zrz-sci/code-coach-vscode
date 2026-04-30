# 616. 给字符串添加加粗标签

## 核心思路

本题本质上是一个**区间标记 + 区间合并**问题：先找出字符串 `s` 中所有被 `words` 中单词覆盖的字符位置，然后把相邻或重叠的覆盖区间合并，最后在合并后的区间边界插入 `<b>` 和 `</b>` 标签。

## 思维链

1. **读完题第一反应**：对于 `words` 中的每个单词，找到它在 `s` 中所有出现的位置，每次出现会覆盖一段连续字符。问题是怎么处理重叠和相邻的覆盖？

2. **拆解问题**：与其直接考虑标签插入，不如分两步走：
   - **Step 1**：标记哪些字符位置需要加粗（布尔数组 `bold[i]`）
   - **Step 2**：扫描布尔数组，在 `true` 区间的起止位置插入标签

3. **Step 1 怎么做（暴力）**：对每个位置 `i`，尝试每个 word，看 `s` 从位置 `i` 开始是否匹配该 word。如果匹配，把 `bold[i..i+len-1]` 都标记为 true。这样重叠和相邻自然就合并了——因为我们标记的是单个字符。

4. **暴力的瓶颈**：对每个位置尝试每个 word，每次匹配是 O(word.length)。总体 O(n × Σ|word|)。当 words 很多或 word 很长时可能慢。

5. **优化方向**：用 **Trie（前缀树）** 存储所有 words。对每个位置 `i`，沿着 Trie 往下走，一次性检查所有以 `s[i]` 开头的 word 是否匹配，避免重复比较公共前缀。

6. **Step 2 始终不变**：线性扫描 `bold` 数组，遇到 `true` 开头插 `<b>`，遇到 `true→false` 边界插 `</b>`。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力匹配 + 布尔标记 | 逐位置逐 word 检查 substr | O(n × m × L) | O(n) | ⭐ 必须写出 |
| Trie + 布尔标记 | 用前缀树加速多模式匹配 | O(n × L + Σ\|word\|) | O(Σ\|word\|) | 加分项 |
| 区间合并 | 找出所有匹配区间后排序合并 | O(n × m × L + k log k) | O(k) | 能说出即可 |

> n = s.length, m = words.length, L = 最长 word 长度, k = 匹配区间数

## 关键提示

1. **别直接想标签插入，先想标记**：用一个布尔数组 `bold[i]` 表示位置 `i` 的字符是否需要加粗。重叠/相邻的合并问题就自然消失了。

2. **匹配用 `s.substr(i, word.size()) == word`** 或 `s.compare(i, word.size(), word) == 0`，后者不创建临时字符串，更高效。

3. **标签插入的扫描**：观察 `bold` 数组的边界变化：
   ```
   s:    a  a  a  b  b  b
   bold: T  T  T  T  T  T
         ↑                 ↑
       插<b>            插</b>
   ```
   只需检查 `bold[i] && (i==0 || !bold[i-1])` 就是一段 bold 的起点。

4. **Trie 优化的关键洞察**：多个 words 可能共享前缀（如 "abc" 和 "ab"）。在 Trie 上从位置 `i` 开始走，每到一个 `isEnd` 节点就标记一段区间，无需重复匹配公共前缀。

5. **边界**：`words` 可能为空（直接返回 `s`），word 可能比 `s` 还长（匹配时注意不越界）。

## 解法详解

### 解法1: 暴力匹配 + 布尔标记 — O(n × m × L) / O(n) ⭐ 面试首选

**思考过程**：

问题可以拆成两步：
1. 对 `s` 的每个位置 `i`，检查是否有某个 `word` 从这里开始匹配。如果匹配了长度为 `len` 的 word，就把 `bold[i]` 到 `bold[i+len-1]` 全标 true。
2. 扫描 `bold` 数组，在 true 段的起点插 `<b>`，终点后插 `</b>`。

为什么布尔数组能自动处理合并？因为 "aa" 在位置0匹配标记 `[0,1]`，在位置1匹配标记 `[1,2]`，结果 `bold = [T,T,T]` 自然合并了。

```cpp
class Solution {
public:
    string addBoldTag(string s, vector<string>& words) {
        int n = s.size();
        vector<bool> bold(n, false);
        
        // Step1: 标记每个字符是否需要加粗
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                // 检查从位置i开始是否匹配word
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    // 标记 [i, i+len-1] 为加粗
                    for (int j = i; j < i + len; j++) {
                        bold[j] = true;
                    }
                }
            }
        }
        
        // Step2: 根据bold数组构建结果
        string result;
        for (int i = 0; i < n; i++) {
            // bold段的起点：当前是true，且前一个是false或不存在
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                result += "<b>";
            }
            result += s[i];
            // bold段的终点：当前是true，且下一个是false或不存在
            if (bold[i] && (i == n - 1 || !bold[i + 1])) {
                result += "</b>";
            }
        }
        
        return result;
    }
};
```

**关键点**：
- `s.compare(i, len, word)` 比 `s.substr(i, len) == word` 更高效（不创建临时字符串）
- 内层 `for (int j = ...)` 的标记看似可以优化，但对于面试来说清晰度最重要

### 解法1.5: 暴力匹配优化（end 指针法）— O(n × m × L) / O(n)

**从解法1微调**：不用 `bold` 数组逐个标记，而是维护一个 `end` 变量记录"当前需要加粗的最远右边界"。遇到匹配时更新 `end = max(end, i + len)`。

```cpp
class Solution {
public:
    string addBoldTag(string s, vector<string>& words) {
        int n = s.size();
        vector<bool> bold(n, false);
        
        // 对每个位置，找到从这里开始匹配的最长覆盖终点
        int end = 0; // 当前覆盖的最远右端点
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    end = max(end, i + (int)len);
                }
            }
            // 如果位置i还在某次匹配的覆盖范围内
            if (i < end) {
                bold[i] = true;
            }
        }
        
        // 构建结果（同上）
        string result;
        for (int i = 0; i < n; i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) result += "<b>";
            result += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) result += "</b>";
        }
        return result;
    }
};
```

### 解法2: Trie + 布尔标记 — O(n × L + Σ|word|) / O(Σ|word| + n)

**从解法1的瓶颈出发**：解法1中，对于每个位置 `i`，我们遍历所有 words 做匹配。如果 words 有公共前缀（如 "abc", "abd", "ab"），会重复比较公共部分。Trie 把所有 words 的公共前缀合并，从位置 `i` 开始只需沿 Trie 走一遍，每遇到一个单词终止节点就标记一段。

```
// Trie 结构示意 (words = ["abc", "ab", "abd"]):
//
//    root
//     |
//     a
//     |
//     b (isEnd=true, 对应"ab")
//    / \
//   c   d
// (end) (end)
//
// 从位置i开始匹配时，沿着 root→a→b 就能同时发现 "ab"、"abc"、"abd"
// 无需重复匹配 "a"→"b" 这个公共前缀
```

```cpp
class Solution {
public:
    struct TrieNode {
        TrieNode* children[128] = {};  // ASCII字符
        bool isEnd = false;
    };
    
    string addBoldTag(string s, vector<string>& words) {
        // 构建 Trie
        TrieNode* root = new TrieNode();
        for (const string& word : words) {
            TrieNode* node = root;
            for (char c : word) {
                if (!node->children[c]) {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }
            node->isEnd = true;
        }
        
        int n = s.size();
        vector<bool> bold(n, false);
        
        // 对每个起始位置，沿 Trie 尽可能匹配
        for (int i = 0; i < n; i++) {
            TrieNode* node = root;
            for (int j = i; j < n; j++) {
                // 如果当前字符在 Trie 中没有后继，停止
                if (!node->children[s[j]]) break;
                node = node->children[s[j]];
                // 如果走到某个 word 的结尾，标记 [i, j]
                if (node->isEnd) {
                    for (int k = i; k <= j; k++) {
                        bold[k] = true;
                    }
                }
            }
        }
        
        // 构建结果
        string result;
        for (int i = 0; i < n; i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) result += "<b>";
            result += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) result += "</b>";
        }
        return result;
    }
};
```

**关键点**：Trie 的优势在 words 数量多且有公共前缀时最明显。如果 words 都是长度为1的不同字符，Trie 不会比暴力快。

### 解法3: 区间收集 + 排序合并 — O(n × m × L + k log k) / O(k)

**另一种思路**：不用布尔数组，而是收集所有匹配产生的区间 `[start, end)`，然后排序后合并重叠/相邻区间。

```cpp
class Solution {
public:
    string addBoldTag(string s, vector<string>& words) {
        int n = s.size();
        vector<pair<int,int>> intervals;
        
        // 收集所有匹配区间
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    intervals.push_back({i, i + len});
                }
            }
        }
        
        if (intervals.empty()) return s;
        
        // 按起点排序，起点相同按终点排序
        sort(intervals.begin(), intervals.end());
        
        // 合并重叠/相邻区间
        vector<pair<int,int>> merged;
        merged.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); i++) {
            auto& last = merged.back();
            // 当前区间起点 <= 上一个区间终点 → 重叠或相邻，合并
            if (intervals[i].first <= last.second) {
                last.second = max(last.second, intervals[i].second);
            } else {
                merged.push_back(intervals[i]);
            }
        }
        
        // 插入标签（从后往前插，避免偏移计算）
        string result;
        int mi = 0; // merged 区间指针
        for (int i = 0; i < n; i++) {
            if (mi < merged.size() && i == merged[mi].first) {
                result += "<b>";
            }
            result += s[i];
            if (mi < merged.size() && i + 1 == merged[mi].second) {
                result += "</b>";
                mi++;
            }
        }
        
        return result;
    }
};
```

## 解法对比

| | 解法1: 暴力+布尔 | 解法2: Trie+布尔 | 解法3: 区间合并 |
|---|---|---|---|
| 核心思想 | 逐位置逐word匹配 | Trie共享前缀匹配 | 收集区间后排序合并 |
| 时间 | O(n·m·L) | O(n·L + Σ\|w\|) | O(n·m·L + k log k) |
| 空间 | O(n) | O(Σ\|w\| + n) | O(k + n) |
| 代码复杂度 | 最简单 | 需要实现 Trie | 中等 |
| 面试推荐 | ⭐ 首选 | words 多时优势大 | 区间合并通用模板 |

**选择建议**：
- 面试首选解法1，代码简单、思路清晰，时间复杂度对于本题数据范围完全够用。
- 如果面试官追问"words 很多怎么办"，讲解法2（Trie）。
- 如果面试官追问"你还知道其他思路吗"，讲解法3（区间合并），展示通用算法思维。

## 易错点

1. **匹配越界**
   - ✗ `s.substr(i, len) == word` 且没检查 `i + len <= n`
   - ✓ 先检查 `i + len <= n` 再匹配，否则 `substr` 可能越界或得到不完整子串（`substr` 不会崩但长度不够时匹配结果错误）

2. **标签插入位置判断写反**
   - ✗ 在 `bold[i] && bold[i-1]` 时插入 `<b>`（这是段中间不是起点）
   - ✓ 在 `bold[i] && (i==0 || !bold[i-1])` 时插入 `<b>`（前一个不加粗或不存在 = 起点）

3. **区间合并时忘记处理"相邻"**
   - ✗ `intervals[i].first < last.second`（只处理了重叠）
   - ✓ `intervals[i].first <= last.second`（`<=` 同时处理重叠和相邻，因为区间是左闭右开 `[start, end)`）

4. **`words` 为空时未处理**
   - 直接进入匹配循环不会出错（内层不执行），但区间合并解法中如果 `intervals` 为空，`merged[0]` 会越界。

## 面试追问

**Q1: 暴力解的时间复杂度是多少？瓶颈在哪？**
> O(n × m × L)，其中 n = |s|, m = |words|, L = 最长 word 长度。瓶颈是对每个位置 i 都要尝试匹配每个 word，且不同 word 的公共前缀被重复比较。

**Q2: 如果 words 有上万个且很多共享前缀，怎么优化？**
> 用 Trie 存储所有 words。对每个位置 i 从 Trie root 开始走，一次遍历就能检查所有以 s[i] 开头的 word。复杂度降为 O(n × L + Σ|word|)，和 words 数量 m 无关（只和最长匹配长度有关）。

**Q3: 如果要求不用布尔数组，还有什么思路？**
> 区间合并：收集所有匹配产生的区间 [start, end)，排序后合并重叠/相邻区间，最后根据合并后的区间插入标签。这是一个通用的区间合并模板。

**Q4: 这道题和 758. Bold Words in String 是什么关系？**
> 完全相同的题，只是编号不同。学会本题后 758 直接用同样的代码提交。

## 相关题型

- **758. 字符串中的加粗单词** — 完全相同的题，代码直接复用。
- **56. 合并区间** — 解法3 直接复用区间排序合并模板，区别在于本题区间来源是字符串匹配。
- **208. 实现 Trie (前缀树)** — 解法2 中 Trie 的构建和查询，学会 208 后可以直接在本题中应用。
- **139. 单词拆分** — 同样是在字符串中匹配字典中的词，区别是 139 要求完全覆盖（DP），本题只需要标记覆盖位置。