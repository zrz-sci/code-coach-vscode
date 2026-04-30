# 642. 设计搜索自动补全系统

## 核心思路

本质上是一个**前缀匹配 + Top-K 排序**的流式设计问题：用户每输入一个字符，你需要在所有历史句子中，找到与当前输入前缀匹配的句子，并按热度（频次降序、字典序升序）返回前 3 个。核心挑战在于如何高效地进行前缀查找和排序。

## 思维链

1. **读完题第一反应**：每次 `input(c)` 时，我有一个不断增长的前缀字符串，需要从所有历史句子中找出以该前缀开头的句子，按规则排序后返回 Top 3。最朴素的做法？遍历所有句子，逐一检查前缀匹配，全部收集后排序取前 3。

2. **暴力解的瓶颈**：每次 `input` 都要遍历全部 N 个句子做 `startsWith` 检查，每次检查 O(L)（L = 前缀长度），再排序 O(K log K)（K = 匹配数）。如果句子数量大、调用频繁，反复扫全集效率低。

3. **怎么突破？Trie（前缀树）天然支持前缀查找**。把所有句子插入 Trie，每输入一个字符就往 Trie 下走一步。当前节点的子树包含所有匹配的句子，不需要扫描全集。

4. **Trie 怎么高效取 Top-3？**
   - 方案 A：在 Trie 上做 DFS 收集所有匹配句子，排序取前 3。
   - 方案 B：每个 Trie 节点维护一个"经过此前缀的 Top-3 热门句子"列表，查询时直接返回，O(1) 查询但插入/更新较复杂。
   - 方案 C：维护一个当前前缀匹配的候选集合，每输入一个字符就缩小候选集（类似逐步过滤）。

5. **状态维护的关键**：用一个 `curNode` 指针记录当前在 Trie 中走到的节点，每次 `input` 只需往下走一步。遇到 `#` 时，把完整句子插入 Trie 并重置状态。

6. **排序细节**：按 `(-频次, 字典序)` 排序，取前 3。可以用 `partial_sort` 或小顶堆来优化，但数据量不大时直接排序即可。

## 解法概览

| 解法 | 思路 | input 时间 | 空间 | 面试 |
|------|------|-----------|------|------|
| HashMap 暴力 | 用 map 存句子→频次，每次遍历全部做前缀匹配 | O(N·L + K log K) | O(N·L) | 能说出即可 |
| Trie + DFS 收集 | Trie 前缀定位 + DFS 收集子树所有句子 + 排序 | O(DFS子树 + K log K) | O(所有字符数) | ⭐ 必须写出 |
| Trie + 节点缓存 Top-3 | 每个节点维护 Top-3 列表，查询 O(1) | O(1) 查询 / O(?) 更新 | O(所有字符数 · 3) | 加分项 |

> N = 句子总数, L = 前缀长度, K = 匹配句子数

## 关键提示

1. **字符映射**：句子中包含小写字母和空格，共 27 种字符。Trie 节点的 children 数组大小为 27（或用 `unordered_map<char, TrieNode*>`）。空格的索引可以映射为 26。

2. **`curNode` 指针**：不要每次 `input` 都从根节点重新查找。维护一个指针，每输入一个字符就往下走一步。如果走不下去（无匹配），后续字符都返回空，直到遇到 `#` 重置。

3. **遇到 `#` 做两件事**：① 把当前完整句子插入 Trie 并更新频次；② 重置 `curNode` 和当前输入缓冲区。

4. **排序比较器**：先按频次降序，频次相同按字典序升序。
   ```
   比较规则: (a.freq > b.freq) || (a.freq == b.freq && a.sentence < b.sentence)
   ```

5. **ASCII 示意 — Trie 结构**：
   ```
   插入 "i love you"(5), "island"(3), "iroman"(2), "i love leetcode"(2):
   
   root
    └─ 'i' ─┬─ ' ' ─ 'l' ─ 'o' ─ 'v' ─ 'e' ─ ' ' ─┬─ 'y' ─ 'o' ─ 'u' [5]
             │                                         └─ 'l' ─ 'e' ─ ... [2]
             ├─ 's' ─ 'l' ─ 'a' ─ 'n' ─ 'd' [3]
             └─ 'r' ─ 'o' ─ 'm' ─ 'a' ─ 'n' [2]
   
   输入 "i" → curNode 走到 'i' 节点 → DFS 子树收集所有句子 → 排序取 Top-3
   输入 " " → curNode 走到 ' ' 节点 → DFS 子树只有 "i love you" 和 "i love leetcode"
   输入 "a" → curNode 在 ' ' 的子节点中找不到 'a' → 返回 []
   ```

## 解法详解

### 解法1: HashMap 暴力 — O(N·L) / O(N·L)

**思考过程**: 最直觉的做法——用一个 `unordered_map<string, int>` 存储所有句子及其频次。每次 `input(c)` 时，把 c 追加到当前前缀，然后遍历 map 中所有句子，检查哪些以当前前缀开头，收集后排序取前 3。

```cpp
class AutocompleteSystem {
    unordered_map<string, int> freq;  // 句子 → 频次
    string curInput;                   // 当前正在输入的前缀

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        for (int i = 0; i < sentences.size(); i++) {
            freq[sentences[i]] = times[i];
        }
        curInput = "";
    }
    
    vector<string> input(char c) {
        if (c == '#') {
            // 句子结束：存入系统，重置状态
            freq[curInput]++;
            curInput = "";
            return {};
        }
        
        curInput += c;
        
        // 遍历所有句子，找前缀匹配的
        vector<pair<string, int>> matches;
        for (auto& [sentence, count] : freq) {
            // 检查 sentence 是否以 curInput 开头
            if (sentence.size() >= curInput.size() &&
                sentence.substr(0, curInput.size()) == curInput) {
                matches.push_back({sentence, count});
            }
        }
        
        // 按规则排序：频次降序，字典序升序
        sort(matches.begin(), matches.end(), [](auto& a, auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
        
        // 取前 3 个
        vector<string> result;
        for (int i = 0; i < min((int)matches.size(), 3); i++) {
            result.push_back(matches[i].first);
        }
        return result;
    }
};
```

**关键点**: 
- 每次 `input` 都遍历全部句子，N 大时效率差。
- `substr` 比较可以优化为 `compare`，但本质瓶颈是遍历全集。

---

### 解法2: Trie + DFS 收集 — 前缀定位 + 子树搜索 ⭐ 面试首选

**从解法1优化**: 暴力解的瓶颈是"每次都遍历全部句子做前缀匹配"。Trie 天然按前缀组织数据，沿着前缀走到对应节点后，只需搜索该子树，大幅减少搜索范围。同时维护 `curNode` 指针避免每次从根重新查找。

```cpp
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    unordered_map<string, int> counts;  // 经过此节点的所有句子及其频次
    // 为什么存在每个节点？因为 DFS 时不用走到叶子才能知道句子和频次
};

class AutocompleteSystem {
    TrieNode* root;
    TrieNode* curNode;   // 当前在 Trie 中的位置
    string curInput;      // 当前输入的前缀
    bool dead;            // 标记当前前缀是否已无匹配（走不下去了）

    void insert(const string& s, int count) {
        TrieNode* node = root;
        for (char c : s) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
            node->counts[s] += count;  // 路径上每个节点都记录
        }
    }

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode();
        curNode = root;
        curInput = "";
        dead = false;
        for (int i = 0; i < sentences.size(); i++) {
            insert(sentences[i], times[i]);
        }
    }
    
    vector<string> input(char c) {
        if (c == '#') {
            // 句子结束：插入当前句子，重置状态
            insert(curInput, 1);
            curInput = "";
            curNode = root;
            dead = false;
            return {};
        }
        
        curInput += c;
        
        // 如果之前已经走不下去了，后续也都无匹配
        if (dead) return {};
        
        // 尝试往下走一步
        if (!curNode->children.count(c)) {
            dead = true;
            return {};
        }
        curNode = curNode->children[c];
        
        // 当前节点的 counts 包含所有以 curInput 为前缀的句子
        vector<pair<string, int>> matches(curNode->counts.begin(), curNode->counts.end());
        
        // 排序：频次降序，字典序升序
        sort(matches.begin(), matches.end(), [](auto& a, auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
        
        vector<string> result;
        for (int i = 0; i < min((int)matches.size(), 3); i++) {
            result.push_back(matches[i].first);
        }
        return result;
    }
};
```

**关键点**:
- 每个 Trie 节点存了 `counts` map：经过此节点的所有完整句子及频次。这避免了 DFS 遍历子树。
- `dead` 标志：一旦当前前缀无匹配，后续加任何字符也不可能匹配，直接短路返回空。
- 插入时路径上每个节点都更新 counts，空间换时间。

---

### 解法3: Trie + DFS（不在节点缓存 counts） — 经典 Trie 实现

**与解法2的区别**: 解法2在每个节点冗余存储了完整句子列表，空间开销大。这个版本只在叶子节点（句子结尾）存频次，查询时做 DFS 遍历子树收集。空间更省，但查询时间取决于子树大小。

```cpp
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    int freq = 0;       // > 0 表示某个句子在此结尾
    string sentence;     // 完整句子（仅结尾节点有效）
};

class AutocompleteSystem {
    TrieNode* root;
    TrieNode* curNode;
    string curInput;
    bool dead;

    void insert(const string& s, int count) {
        TrieNode* node = root;
        for (char c : s) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->freq += count;
        node->sentence = s;
    }

    // DFS 收集子树中所有句子
    void dfs(TrieNode* node, vector<pair<string, int>>& matches) {
        if (!node) return;
        if (node->freq > 0) {
            matches.push_back({node->sentence, node->freq});
        }
        for (auto& [ch, child] : node->children) {
            dfs(child, matches);
        }
    }

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode();
        curNode = root;
        curInput = "";
        dead = false;
        for (int i = 0; i < sentences.size(); i++) {
            insert(sentences[i], times[i]);
        }
    }

    vector<string> input(char c) {
        if (c == '#') {
            insert(curInput, 1);
            curInput = "";
            curNode = root;
            dead = false;
            return {};
        }

        curInput += c;
        if (dead) return {};

        if (!curNode->children.count(c)) {
            dead = true;
            return {};
        }
        curNode = curNode->children[c];

        // DFS 收集当前子树的所有句子
        vector<pair<string, int>> matches;
        dfs(curNode, matches);

        // 用 partial_sort 取前3，比全排序更高效
        int k = min((int)matches.size(), 3);
        partial_sort(matches.begin(), matches.begin() + k, matches.end(),
            [](auto& a, auto& b) {
                if (a.second != b.second) return a.second > b.second;
                return a.first < b.first;
            });

        vector<string> result;
        for (int i = 0; i < k; i++) {
            result.push_back(matches[i].first);
        }
        return result;
    }
};
```

**关键点**:
- 空间更省：不在每个中间节点存句子列表。
- 查询可能慢：如果某前缀下有大量句子，DFS 遍历代价高。
- 用 `partial_sort` 优化：只需前 3 个，不必全排序。

---

### 解法4: Trie + 每节点缓存 Top-3（最优查询） — 加分项

**从解法2/3进一步优化**: 既然每次只要 Top-3，为什么不在每个节点直接维护一个 Top-3 列表？查询时直接返回，O(1) 查询。代价是插入/更新时要维护这个列表。

```cpp
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    // 维护经过此节点的 Top-3 热门句子（按排序规则）
    vector<pair<int, string>> top3;  // {-freq, sentence} 方便排序
    
    void updateTop3(const string& s, int freq) {
        // 先查找是否已存在
        for (auto& p : top3) {
            if (p.second == s) {
                p.first = -freq;  // 更新频次
                sort(top3.begin(), top3.end());
                return;
            }
        }
        // 不存在，加入
        top3.push_back({-freq, s});
        sort(top3.begin(), top3.end());
        if (top3.size() > 3) top3.pop_back();
    }
};

class AutocompleteSystem {
    TrieNode* root;
    TrieNode* curNode;
    string curInput;
    bool dead;
    unordered_map<string, int> freqMap;  // 全局频次记录

    void insert(const string& s, int totalFreq) {
        TrieNode* node = root;
        for (char c : s) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
            node->updateTop3(s, totalFreq);
        }
    }

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        root = new TrieNode();
        curNode = root;
        curInput = "";
        dead = false;
        for (int i = 0; i < sentences.size(); i++) {
            freqMap[sentences[i]] = times[i];
            insert(sentences[i], times[i]);
        }
    }

    vector<string> input(char c) {
        if (c == '#') {
            freqMap[curInput]++;
            insert(curInput, freqMap[curInput]);
            curInput = "";
            curNode = root;
            dead = false;
            return {};
        }

        curInput += c;
        if (dead) return {};

        if (!curNode->children.count(c)) {
            dead = true;
            return {};
        }
        curNode = curNode->children[c];

        // 直接从节点取 Top-3
        vector<string> result;
        for (auto& [negFreq, s] : curNode->top3) {
            result.push_back(s);
        }
        return result;
    }
};
```

**关键点**:
- 查询真正 O(1)（直接读 top3 列表）。
- 插入时要更新路径上所有节点的 top3，每个节点最多排序 4 个元素，常数时间。
- `freqMap` 用来追踪全局频次，因为 `insert` 时传的是累计频次而非增量。
- ⚠️ 更新已有句子频次时（遇到 `#`），要对整条路径的 top3 做 update 而非 append。

## 解法对比

| | 解法1 HashMap 暴力 | 解法2 Trie+节点缓存counts | 解法3 Trie+DFS | 解法4 Trie+Top3缓存 |
|--|--|--|--|--|
| 查询时间 | O(N·L) | O(K log K) | O(子树大小 + K log K) | O(1) |
| 插入时间 | O(1) | O(L·S) 路径上每节点更新 | O(L) | O(L·3) |
| 空间 | O(N·L) | O(所有字符·N) 冗余大 | O(所有字符) | O(所有字符·3) |
| 实现难度 | 低 | 中 | 中 | 中高 |
| 面试推荐 | 暖场 | ⭐ 首选 | 变体 | 追问加分 |

## 易错点

1. **✗ 忘记空格的映射**：Trie 的 children 用 `char` 做 key，空格 `' '` 也是合法字符。如果用固定大小数组 `children[27]`，空格必须映射到索引 26。
   - ✓ 用 `unordered_map<char, TrieNode*>` 更简单不易出错。

2. **✗ `#` 时忘记把当前句子频次累加而非覆盖**：
   ```cpp
   freq[curInput] = 1;  // ✗ 覆盖了之前的频次！
   freq[curInput]++;     // ✓ 累加
   ```

3. **✗ `dead` 标志忘记在 `#` 时重置**：
   ```cpp
   if (c == '#') {
       insert(curInput, 1);
       curInput = "";
       curNode = root;
       // dead = false;  ← 忘了这行，下一轮查询全部返回空！
   }
   ```

4. **✗ 解法4中 insert 传增量而非总频次**：
   ```cpp
   // ✗ 每次 # 都 insert(curInput, 1)，top3 中同一句子会出现多次且频次为1
   // ✓ 应该传 freqMap[curInput]（累计总频次），并在 updateTop3 中先查找再更新
   ```

5. **✗ 排序比较器方向搞反**：频次要**降序**（大的在前），字典序要**升序**（小的在前）。用 `{-freq, sentence}` 的 pair 可以统一为升序排序。

## 面试追问

**Q1（基础理解）**: "暴力法每次 input 的时间复杂度是多少？瓶颈在哪？"
> A: O(N·L)，N 是句子数，L 是前缀长度。瓶颈在于每次都要遍历全部句子做前缀匹配。

**Q2（优化）**: "用 Trie 后，查询时间变成了什么？还能进一步优化吗？"
> A: Trie 定位前缀节点只需 O(L)，然后 DFS 子树 O(子树大小)。进一步优化：在每个节点缓存 Top-3，查询降到 O(1)。

**Q3（空间权衡）**: "节点缓存 Top-3 的空间开销如何？如果句子非常长怎么办？"
> A: 每个节点额外存 3 个字符串指针（或索引），空间 O(总 Trie 节点数 × 3)。句子很长时节点多，但每个节点只存 3 个引用，开销可控。可以用句子 ID 替代完整字符串来省空间。

**Q4（变体/Follow-up）**: "如果要支持删除某个句子，该怎么改？"
> A: 需要在 Trie 路径上减少频次，如果频次降为 0 则从各节点的 top3 中移除。维护 `freqMap` 方便全局追踪。删除后 top3 可能需要从 counts 重新选取，复杂度上升。

**Q5（系统设计延伸）**: "如果这个系统要服务百万用户，你怎么设计？"
> A: 离线用 MapReduce 统计句子频次，在线用分布式 Trie（或 prefix-based sharding）。热门前缀可以预计算 Top-K 缓存在 Redis 中，冷门前缀实时查 Trie。

## 相关题型

- **208. 实现 Trie** — 本题的基础，先掌握 Trie 的 insert/search/startsWith 三个操作。本题在此基础上加了频次排序和流式输入。
- **211. 添加与搜索单词** — Trie + 通配符搜索，复用 Trie 框架但 DFS 搜索逻辑不同（`.` 需要尝试所有分支）。
- **692. 前K个高频单词** — 复用本题的"频次排序 + Top-K"逻辑，但不需要前缀匹配，可以用堆。
- **1268. 搜索推荐系统** — 和本题非常相似（Trie + 前缀推荐），但排序只需字典序，不需要频次，是本题的简化版。
- **745. 前缀和后缀搜索** — Trie 的变体应用，需要同时处理前缀和后缀约束。