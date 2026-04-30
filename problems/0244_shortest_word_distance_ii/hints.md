# 244. 最短单词距离 II

## 核心思路

这道题本质上是一个**预处理 + 多次查询**的设计问题：给定一个单词数组，需要多次查询两个单词之间的最短距离。关键在于如何在构造阶段做预处理，使得每次查询尽可能快。

## 思维链

1. **读完题第一反应**：这和 LeetCode 243（最短单词距离 I）很像，243 只查一次，直接线性扫描 O(n) 就行。但这道题要查多次（最多 5000 次），每次都线性扫描会不会太慢？

2. **暴力做法**：每次调用 `shortest()` 时，都遍历整个数组找 word1 和 word2 的位置，然后计算最短距离。这就是把 243 的解法重复调用。时间：每次查询 O(n)，总共 O(Q·n)。

3. **瓶颈在哪**：每次查询都要从头遍历数组找位置，但位置信息是固定的！同一个单词的出现位置不会变，为什么要反复找？

4. **突破思路**：在构造函数中用**哈希表预处理**，把每个单词出现的所有下标存起来。查询时，只需要拿到 word1 和 word2 的下标列表，找这两个列表中元素的最小差值。

5. **如何高效找两个列表的最小差值？** 关键观察：预处理存的下标列表天然是**有序的**（按原数组顺序存入）。对两个有序数组找最小差值，可以用**双指针**——谁小移谁，时间 O(L1 + L2)，远比暴力配对 O(L1 × L2) 快。

6. **设计总结**：构造 O(n) 建哈希表，查询 O(L1 + L2) 双指针扫描，空间 O(n) 存所有下标。

## 解法概览

| 解法 | 思路 | 构造时间 | 查询时间 | 空间 | 面试 |
|------|------|----------|----------|------|------|
| 暴力查询 | 每次查询遍历数组 | O(1) | O(n) | O(1) | 能说出即可 |
| 哈希表 + 双指针 | 预处理下标，双指针找最小差 | O(n) | O(L1+L2) | O(n) | ⭐ 必须写出 |
| 哈希表 + 缓存结果 | 在双指针基础上缓存查询结果 | O(n) | 均摊更优 | O(n+Q) | 加分项 |

> n = 数组长度，L1/L2 = 两个单词各自出现次数，Q = 查询次数

## 关键提示

1. **预处理 vs 查询的权衡**：只查一次用 243 的方法就够了；多次查询就值得花时间预处理。这是设计题的核心思想。

2. **下标列表天然有序**：构造时从左到右遍历数组存下标，存入顺序就是递增的，不需要额外排序。

3. **双指针找两个有序数组的最小差值**：谁小移谁。因为如果 `list1[i] < list2[j]`，那 `list1[i]` 和 `list2[j+1], list2[j+2]...` 的差只会更大，所以应该移动 `i` 去尝试缩小差值。

4. **和 243 的关系**：243 是一次性线性扫描；244 把"找位置"这一步提前到了构造阶段，查询阶段只需要对比两个列表。

5. 示意图：
```
wordsDict: ["practice", "makes", "perfect", "coding", "makes"]
索引:          0          1         2          3         4

哈希表预处理结果:
  "practice" → [0]
  "makes"    → [1, 4]     ← 出现两次
  "perfect"  → [2]
  "coding"   → [3]

查询 shortest("makes", "coding"):
  list1 = [1, 4]    list2 = [3]
  
  双指针过程:
  i=0, j=0: |1-3| = 2, 1<3 所以 i++
  i=1, j=0: |4-3| = 1, 3<4 所以 j++
  j 越界，结束。最小距离 = 1
```

## 解法详解

### 解法1: 暴力查询 — O(1)构造 / O(n)查询

**思考过程**: 最简单的做法——每次 `shortest` 被调用时，像 LeetCode 243 一样线性扫描整个数组。维护两个变量 `idx1` 和 `idx2` 记录最近一次看到 word1 和 word2 的位置，遇到就更新最小距离。

```cpp
class WordDistance {
public:
    vector<string> words;
    
    WordDistance(vector<string>& wordsDict) {
        words = wordsDict;  // 只是保存数组
    }
    
    int shortest(string word1, string word2) {
        int minDist = INT_MAX;
        int idx1 = -1, idx2 = -1;
        for (int i = 0; i < words.size(); i++) {
            if (words[i] == word1) idx1 = i;
            if (words[i] == word2) idx2 = i;
            if (idx1 != -1 && idx2 != -1) {
                minDist = min(minDist, abs(idx1 - idx2));
            }
        }
        return minDist;
    }
};
```

**关键点**: 这个解法在查询次数少时完全够用，但如果 `shortest` 被调用 5000 次，总共要扫描 5000 × 30000 = 1.5 亿次，可能超时。

### 解法2: 哈希表 + 双指针 — O(n)构造 / O(L1+L2)查询 ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是每次查询都要遍历整个数组找位置。但单词位置是固定的，应该在构造时一次性记录下来。查询时，利用两个下标列表都是有序的这一事实，用双指针高效找最小差值。

```cpp
class WordDistance {
private:
    // 哈希表: 单词 → 该单词在数组中出现的所有下标（有序）
    unordered_map<string, vector<int>> wordIndices;
    
public:
    WordDistance(vector<string>& wordsDict) {
        // 预处理: 把每个单词的所有下标存入哈希表
        // 因为从左到右遍历，存入的下标天然递增
        for (int i = 0; i < wordsDict.size(); i++) {
            wordIndices[wordsDict[i]].push_back(i);
        }
    }
    
    int shortest(string word1, string word2) {
        const vector<int>& list1 = wordIndices[word1];
        const vector<int>& list2 = wordIndices[word2];
        
        int i = 0, j = 0;
        int minDist = INT_MAX;
        
        // 双指针: 两个有序列表找最小差值
        // 谁小移谁 — 因为小的那个往后移才有可能缩小差值
        while (i < list1.size() && j < list2.size()) {
            int idx1 = list1[i], idx2 = list2[j];
            minDist = min(minDist, abs(idx1 - idx2));
            
            if (idx1 < idx2) {
                i++;  // idx1 更小，移动 i 试图让差值变小
            } else {
                j++;  // idx2 更小，移动 j
            }
        }
        
        return minDist;
    }
};
```

**关键点**: 
- 用 `const vector<int>&` 引用而非拷贝，避免不必要的开销
- 双指针移动的正确性：如果 `idx1 < idx2`，那么 `idx1` 和 `list2[j+1], list2[j+2]...` 的差只会更大（因为 list2 递增），所以应该移动 `i` 去试更大的 `idx1`

### 解法3: 哈希表 + 双指针 + 查询缓存 — 均摊更优（加分）

**从解法2优化**: 如果同一对单词被反复查询，可以缓存结果。用一个额外的哈希表记录已查询过的 (word1, word2) 对的结果。

```cpp
class WordDistance {
private:
    unordered_map<string, vector<int>> wordIndices;
    // 缓存: "word1#word2" → 最短距离
    unordered_map<string, int> cache;
    
public:
    WordDistance(vector<string>& wordsDict) {
        for (int i = 0; i < wordsDict.size(); i++) {
            wordIndices[wordsDict[i]].push_back(i);
        }
    }
    
    int shortest(string word1, string word2) {
        // 统一 key 的顺序，使得 (a,b) 和 (b,a) 共享缓存
        string key = (word1 < word2) ? word1 + "#" + word2 
                                     : word2 + "#" + word1;
        
        if (cache.count(key)) return cache[key];
        
        const vector<int>& list1 = wordIndices[word1];
        const vector<int>& list2 = wordIndices[word2];
        
        int i = 0, j = 0;
        int minDist = INT_MAX;
        
        while (i < list1.size() && j < list2.size()) {
            int idx1 = list1[i], idx2 = list2[j];
            minDist = min(minDist, abs(idx1 - idx2));
            if (idx1 < idx2) i++;
            else j++;
        }
        
        cache[key] = minDist;
        return minDist;
    }
};
```

**关键点**: key 的顺序需要统一（如字典序小的在前），否则 `("coding","makes")` 和 `("makes","coding")` 会被当成不同的查询。

## 解法对比

| | 解法1: 暴力查询 | 解法2: 哈希表+双指针 | 解法3: +缓存 |
|---|---|---|---|
| 构造时间 | O(1) | O(n) | O(n) |
| 单次查询 | O(n) | O(L1+L2) | 首次 O(L1+L2)，重复 O(1) |
| 空间 | O(n) 存原数组 | O(n) 存下标 | O(n+Q) |
| 适用场景 | 查询极少 | 通用 | 重复查询多 |

**选择建议**: 面试时直接写解法2，如果面试官追问"如果同一对单词查很多次怎么办"，再优化到解法3。

## 易错点

1. **双指针移动方向写反**：
   - ✗ `if (idx1 < idx2) j++;` — 移动大的那个，差值只会更大
   - ✓ `if (idx1 < idx2) i++;` — 移动小的那个，才有机会缩小差值

2. **用拷贝而非引用获取列表**：
   - ✗ `vector<int> list1 = wordIndices[word1];` — 每次查询都拷贝一次
   - ✓ `const vector<int>& list1 = wordIndices[word1];` — 引用，零开销

3. **忘记处理 `shortest(a,b)` 和 `shortest(b,a)` 等价**（解法3的缓存场景）：
   - ✗ 直接用 `word1+"#"+word2` 作 key
   - ✓ 先排序保证 key 唯一：`word1 < word2 ? ... : ...`

4. **误以为需要排序下标列表**：下标是从左到右遍历时存入的，天然有序，不需要 `sort()`。

## 面试追问

**Q1（基础理解）**: 这道题和 243 最短单词距离有什么区别？为什么 243 的解法不能直接用？
> **A**: 243 只查一次，O(n) 线性扫描就够了。244 要查多次（最多5000次），如果每次都 O(n) 扫描，总共 O(Qn) 可能太慢。所以需要预处理把位置信息存下来，查询时只比对相关下标。

**Q2（优化追问）**: 两个有序列表找最小差值，为什么双指针是 O(L1+L2) 而不需要 O(L1×L2)？
> **A**: 因为两个列表都是有序的。当 `list1[i] < list2[j]` 时，`list1[i]` 和 `list2[j+k]`（k>0）的差只会更大，所以不需要枚举后面的配对，直接移动 `i`。每个指针最多移动到末尾，所以总共 O(L1+L2)。

**Q3（变体追问）**: 如果不仅要返回最短距离，还要返回具体是哪两个位置，怎么改？
> **A**: 在双指针过程中，每次更新 `minDist` 时同时记录 `bestIdx1 = idx1, bestIdx2 = idx2`，最后返回这两个位置即可。

**Q4（进阶追问）**: 如果单词可以动态添加（不仅仅是初始化时给定），怎么办？
> **A**: 哈希表结构不用变，新增一个 `addWord(string word, int index)` 方法，往对应列表末尾 push 新下标。由于按添加顺序 push，列表仍然有序。如果有缓存，则需要清除涉及该单词的缓存。

## 相关题型

- **243. 最短单词距离** — 单次查询版本，只需线性扫描 O(n)，不需要预处理。244 是把 243 的"找位置"步骤提前到构造阶段。
- **245. 最短单词距离 III** — word1 和 word2 可能相同，需要在 243 基础上额外处理 word1 == word2 的情况（同一个列表中相邻元素的最小差值）。
- **两个有序数组的最小差值** — 本题查询阶段的核心子问题，双指针模板可以直接复用。
- **981. 基于时间的键值存储** — 同样是"预处理有序列表 + 二分/双指针查询"的设计题模式。