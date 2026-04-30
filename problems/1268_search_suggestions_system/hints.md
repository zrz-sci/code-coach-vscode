# 1268. 搜索推荐系统

## 核心思路

本质上是：对于 searchWord 的每个前缀，从 products 中找出所有匹配该前缀的产品，按字典序取前 3 个。关键观察是——**排序后，匹配同一前缀的产品一定是连续的一段**，这使得二分查找或 Trie 都能高效解决。

## 思维链

1. **读完题第一反应**：每输入一个字符就要搜索一次，暴力做法就是每次遍历所有产品检查前缀是否匹配，匹配的放到候选列表里排序取前3个。
2. **暴力解的瓶颈**：searchWord 长度 L，products 数量 N，每个产品平均长度 M。每次输入一个字符都遍历 N 个产品做前缀比较，总共 O(L × N × M)。当数据量大时太慢。
3. **关键观察**：如果先把 products 排序，那么拥有相同前缀的产品在排序后数组中是**连续的一段**。找"第一个 ≥ prefix 的位置"正是二分查找（lower_bound）擅长的事！
4. **排序 + 二分**：排序后，对每个前缀用 lower_bound 定位起始位置，然后从这个位置开始最多取 3 个检查前缀是否匹配即可。
5. **另一种思路——Trie**：把所有产品插入前缀树，每个节点维护经过该节点的字典序最小的 3 个产品，然后沿着 searchWord 的字符逐层查询。
6. **二分还有个妙处**：随着前缀越来越长，匹配范围只会缩小不会扩大，所以搜索区间可以逐步收窄（双指针变体）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力过滤 | 每次遍历所有产品检查前缀 | O(L × N × M) | O(1) 额外 | 能说出即可 |
| 排序 + 二分查找 | 排序后 lower_bound 定位 | O(N log N + L × (log N + M)) | O(排序) | ⭐ 必须写出 |
| 排序 + 双指针 | 排序后逐步收窄候选区间 | O(N log N + L × M) | O(1) 额外 | ⭐ 必须写出 |
| Trie 前缀树 | 每个节点存 top3 产品 | O(总字符数 + L) | O(总字符数) | 加分项 |

## 关键提示

1. **排序是关键的第一步**：排序后，相同前缀的产品聚集在一起，这是二分和双指针都能用的前提。
2. **lower_bound 的妙用**：`lower_bound(begin, end, prefix)` 找到的位置就是第一个 ≥ prefix 的产品，从这里开始往后看最多 3 个。
3. **怎么判断匹配**：从 lower_bound 位置开始，检查 `products[i].substr(0, prefix.size()) == prefix` 或者更高效地用 `products[i].find(prefix) == 0`。但还有个更巧妙的判断：只要 `products[i] >= prefix && products[i] < prefix的下一个字符串`。
4. **双指针的思路**：维护 [left, right] 表示当前所有可能匹配的范围，每输入一个新字符，从两端收缩不匹配的产品。
5. **Trie 适合什么场景**：如果 searchWord 会有很多个（大量查询），Trie 的预处理更划算。但本题只有一个 searchWord，排序+二分更简洁。

```
排序后的 products 示意（示例1）:
 index:  0         1          2         3       4
       "mobile" "moneypot" "monitor" "mouse" "mousepad"
         ↑                                      ↑
前缀 "m": lower_bound 找到 index 0，取 [0,1,2] → 3个
前缀 "mo": lower_bound 找到 index 0，取 [0,1,2] → 3个
前缀 "mou": lower_bound 找到 index 3，取 [3,4]  → 2个
前缀 "mous": lower_bound 找到 index 3，取 [3,4] → 2个
前缀 "mouse": lower_bound 找到 index 3，取 [3,4] → 2个
```

## 解法详解

### 解法1: 暴力过滤 — O(L × N × M) / O(sort)

**思考过程**：最直接的想法——先排序保证字典序，然后每输入一个字符，遍历所有产品，检查是否以当前前缀开头，收集前 3 个匹配的。

```cpp
class Solution {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> result;
        string prefix;
        
        for (char c : searchWord) {
            prefix += c;
            vector<string> suggestions;
            for (const string& p : products) {
                // 检查 p 是否以 prefix 开头
                if (p.size() >= prefix.size() && p.compare(0, prefix.size(), prefix) == 0) {
                    suggestions.push_back(p);
                    if (suggestions.size() == 3) break; // 因为已排序，前3个就是字典序最小
                }
            }
            result.push_back(suggestions);
        }
        return result;
    }
};
```

**关键点**：因为已排序，找到 3 个就可以 break，不需要收集全部再取前 3。但每次还是从头遍历，最坏情况依然是 O(N)。

---

### 解法2: 排序 + 二分查找 — O(N log N + L × (log N + M)) / O(1) ⭐ 面试首选

**从解法1优化**：暴力解每次从头遍历找匹配产品，瓶颈在"定位第一个匹配的位置"。排序后，匹配同一前缀的产品是连续的，用 lower_bound 可以 O(log N) 定位到第一个 ≥ prefix 的位置，然后只需检查从该位置开始的最多 3 个产品。

```cpp
class Solution {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> result;
        string prefix;
        
        for (char c : searchWord) {
            prefix += c;
            // 二分查找: 找第一个 >= prefix 的位置
            auto it = lower_bound(products.begin(), products.end(), prefix);
            
            vector<string> suggestions;
            // 从 it 开始最多看 3 个，检查前缀是否匹配
            for (int i = 0; i < 3 && it + i != products.end(); i++) {
                const string& candidate = *(it + i);
                // 判断 candidate 是否以 prefix 开头
                if (candidate.size() >= prefix.size() && 
                    candidate.compare(0, prefix.size(), prefix) == 0) {
                    suggestions.push_back(candidate);
                } else {
                    break; // 排序后，一旦不匹配，后面也不会匹配
                }
            }
            result.push_back(suggestions);
        }
        return result;
    }
};
```

**关键点**：
- `lower_bound` 返回第一个 ≥ prefix 的位置，这正好是第一个可能匹配前缀的产品。
- 因为排序过，所以从这个位置开始连续的产品要么匹配前缀，要么不匹配，一旦不匹配就可以 break。
- 只需看 3 个产品，所以每次查询后的检查是 O(M) 常数级。

---

### 解法3: 排序 + 双指针 — O(N log N + L × M) / O(1) ⭐ 同样推荐

**另一个角度**：排序后维护一个候选区间 [left, right]，初始覆盖所有产品。每输入一个新字符，从两端收缩：左端不匹配的 left++，右端不匹配的 right--。收缩后取 [left, min(left+2, right)] 即为答案。

**为什么正确**：前缀每多一个字符，匹配条件更严格，候选范围只会缩小。排序保证了候选始终是连续区间。

```cpp
class Solution {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> result;
        int left = 0, right = products.size() - 1;
        
        for (int i = 0; i < searchWord.size(); i++) {
            char c = searchWord[i];
            // 从左端收缩：产品太短或第i个字符不匹配
            while (left <= right && 
                   (products[left].size() <= i || products[left][i] != c)) {
                left++;
            }
            // 从右端收缩：产品太短或第i个字符不匹配
            while (left <= right && 
                   (products[right].size() <= i || products[right][i] != c)) {
                right--;
            }
            
            vector<string> suggestions;
            // 从 left 开始取最多 3 个
            int count = min(3, right - left + 1);
            for (int j = 0; j < count; j++) {
                suggestions.push_back(products[left + j]);
            }
            result.push_back(suggestions);
        }
        return result;
    }
};
```

**关键点**：
- `products[left].size() <= i` 是必要的检查——产品长度不够，说明不可能匹配当前前缀。
- 双指针只会向内收缩，不会往回走，所以所有字符处理完后总收缩次数 ≤ N。

---

### 解法4: Trie 前缀树 — O(总字符数) / O(总字符数)

**思路**：构建 Trie，每个节点存储经过该节点的字典序最小的 3 个产品（索引或字符串）。查询时沿 searchWord 的字符逐层走，每层的节点上直接存好了答案。

**为什么用 Trie**：如果有多个 searchWord 需要查询，Trie 预处理一次后每次查询都是 O(L)。

```cpp
class Solution {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        vector<string> suggestions; // 经过此节点的字典序最小的≤3个产品
    };
    
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end()); // 排序保证插入顺序就是字典序
        
        // 建 Trie
        TrieNode* root = new TrieNode();
        for (const string& product : products) {
            TrieNode* node = root;
            for (char c : product) {
                int idx = c - 'a';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
                // 因为产品已排序，先插入的字典序更小，只保留前3个
                if (node->suggestions.size() < 3) {
                    node->suggestions.push_back(product);
                }
            }
        }
        
        // 查询
        vector<vector<string>> result;
        TrieNode* node = root;
        bool found = true;
        for (char c : searchWord) {
            if (found && node->children[c - 'a']) {
                node = node->children[c - 'a'];
                result.push_back(node->suggestions);
            } else {
                found = false; // 一旦 Trie 中无此前缀，后续都为空
                result.push_back({});
            }
        }
        return result;
    }
};
```

**关键点**：
- 先排序再插入 Trie，保证每个节点的 suggestions 列表天然有序。
- 每个节点最多存 3 个字符串，超过的直接跳过（因为排序后先来的更小）。
- 一旦某个字符在 Trie 中不存在，后续所有前缀都不可能匹配，直接返回空列表。

## 解法对比

| | 暴力 | 排序+二分 | 排序+双指针 | Trie |
|---|---|---|---|---|
| 时间 | O(L×N×M) | O(NlogN + L×logN) | O(NlogN + N) | O(总字符数) |
| 空间 | O(sort) | O(sort) | O(sort) | O(总字符数) |
| 代码量 | 最少 | 适中 | 适中 | 较多 |
| 适用场景 | 数据极小 | 通用，面试首选 | 通用，思路优雅 | 多次查询 |

**面试中怎么选**：
- 先说排序+二分，代码简洁，思路清晰，面试官满意度最高。
- 如果面试官追问"如果有大量 searchWord"，切到 Trie。
- 双指针是巧妙的变体，可以作为加分展示。

## 易错点

1. **产品长度不够的判断**：
   - ✗ 直接 `products[left][i] != c`，如果 `products[left].size() <= i` 会越界
   - ✓ 先检查 `products[left].size() <= i`，再检查字符

2. **lower_bound 后忘记验证前缀**：
   - ✗ 找到 lower_bound 位置后直接取 3 个
   - ✓ 还要验证这 3 个产品确实以 prefix 开头（lower_bound 只保证 ≥ prefix，不保证前缀匹配）
   - 例如 prefix="abc"，lower_bound 可能找到 "abd"，它 ≥ "abc" 但不以 "abc" 开头

3. **Trie 中 found 标记的必要性**：
   - ✗ 每次都检查 `node->children[c-'a']`，但忘记 node 已经是 nullptr
   - ✓ 用 found 标记，一旦断开后续全部返回空列表

4. **排序时机**：
   - ✗ 不排序直接建 Trie，导致 suggestions 不是字典序最小的 3 个
   - ✓ 必须先排序，保证插入顺序就是字典序

## 面试追问

**Q1: 暴力解的时间复杂度是多少？瓶颈在哪？**
→ O(L × N × M)。瓶颈在于每个前缀都要遍历所有产品。排序后可以利用有序性快速定位。

**Q2: 排序+二分 vs 排序+双指针，哪个更好？**
→ 二分每次独立查找，O(L × log N)；双指针利用"前缀越长匹配越少"的单调性，总收缩次数 ≤ N，更优雅。实际面试中二分更容易写对。

**Q3: 如果 products 很大且有大量不同的 searchWord 要查询怎么办？**
→ 用 Trie 预处理，每个节点存 top-3。预处理 O(总字符数)，之后每次查询 O(searchWord长度)。

**Q4: 如果 products 会动态增删怎么办？**
→ Trie 支持动态插入，但每个节点的 top-3 列表需要维护（可能需要用 priority_queue 或有序集合）。排序+二分的方案就不太适合了。

## 相关题型

- **[208. 实现 Trie (前缀树)](https://leetcode.com/problems/implement-trie-prefix-tree/)** — 本题的 Trie 解法直接复用 208 的 insert/search 框架，区别在于每个节点额外存储了 top-3 建议列表
- **[720. 词典中最长的单词](https://leetcode.com/problems/longest-word-in-dictionary/)** — 同样用 Trie + 排序，复用"排序后插入 Trie 保证字典序"的技巧
- **[642. 设计搜索自动补全系统](https://leetcode.com/problems/design-search-autocomplete-system/)** — 本题的升级版，需要支持热度排序和动态输入，Trie 解法可以直接扩展
- **[35. 搜索插入位置](https://leetcode.com/problems/search-insert-position/)** — 复用 lower_bound 的二分模板，本题的二分解法核心就是 lower_bound