# 692. 前K个高频单词

## 核心思路

本质上是一个**TopK 问题 + 自定义排序规则**的组合。先用哈希表统计每个单词的出现频率，然后按照"频率降序、同频按字典序升序"的规则取前 k 个。暴力做法是排序后取前 k 个（O(n log n)），进阶做法用大小为 k 的**最小堆**实现 O(n log k)。堆的排序规则需要**特别小心地反转**。

## 思维链

1. **读完题第一反应** → 统计频率 + 排序 + 取前 k 个。用 HashMap 统计频率，然后按频率降序排序，频率相同时按字典序升序。
2. **暴力排序的时间** → O(n log n)，其中 n 是不同单词的数量。能不能更快？
3. **TopK 问题的经典优化** → 用大小为 k 的最小堆。堆中始终维护"当前最重要的 k 个元素"。每次新元素进来，如果比堆顶更重要，就替换堆顶。
4. **堆的排序规则要反转** → 这是本题最容易出错的地方！堆顶是"最不重要的"（最小堆的堆顶是最小值）。我们要的是"频率最高的 k 个"，所以堆中维护的是最重要的 k 个，堆顶是其中最不重要的。当堆满时，新来的元素如果比堆顶更重要，就替换堆顶。
5. **同频的字典序处理** → 这是第二个坑。排序时"频率降序、字典序升序"，但在最小堆中，堆顶应该是"最先被淘汰的"——即频率最低的，或频率相同但字典序最大的。所以堆的比较器要**反过来**。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希表 + 排序 | 统计频率后全排序取前k | O(n log n) | O(n) | ⭐ 面试首选（简单不易错） |
| 最小堆（大小k） | 维护 k 个最重要的元素 | O(n log k) | O(n) | ⭐ 进阶必会 |
| 桶排序 | 按频率分桶 | O(n) | O(n) | 可以讨论 |

## 关键提示

1. **排序比较器的核心逻辑**：`频率不同 → 频率高的排前面；频率相同 → 字典序小的排前面`。用 C++ lambda 表达就是：`freq[a] != freq[b] ? freq[a] > freq[b] : a < b`。
2. **最小堆比较器要反转**：
   - 最终输出要求：频率高优先，同频字典序小优先
   - 堆顶应该是"最不优先的"（最先被淘汰的）：频率低优先，同频字典序大优先
   - 所以堆的比较器和排序比较器**方向相反**
3. **C++ priority_queue 的坑**：`priority_queue` 默认是最大堆，传入 `less<>` 是最大堆，传入 `greater<>` 是最小堆。自定义比较器时，`operator()` 返回 true 表示第一个参数优先级**更低**（即被排在后面/下沉）。
4. **堆弹出后需要反转**：最小堆弹出的顺序是从"最不重要"到"最重要"，所以弹出 k 个元素后需要反转数组才是正确的降序顺序。

## 解法详解

### 解法1: 哈希表 + 排序 — O(n log n) / O(n) ⭐ 面试首选

**思考过程**: 最直接——统计频率，把所有不同的单词放进数组，按规则排序，取前 k 个。

```
示例: words = ["i","love","leetcode","i","love","coding"], k = 2

Step 1 - 统计频率:
  freq = {"i":2, "love":2, "leetcode":1, "coding":1}

Step 2 - 候选列表 + 排序:
  排序前: ["i", "love", "leetcode", "coding"]

  排序规则: 频率降序，同频字典序升序
  ┌──────────┬──────┬────────────────┐
  │   word   │ freq │     排名       │
  ├──────────┼──────┼────────────────┤
  │   "i"    │  2   │ 1 (频率2, 字典序最小) │
  │  "love"  │  2   │ 2 (频率2, 字典序较大) │
  │ "coding" │  1   │ 3 (频率1, 字典序较小) │
  │"leetcode"│  1   │ 4 (频率1, 字典序较大) │
  └──────────┴──────┴────────────────┘

Step 3 - 取前 k=2 个:
  结果: ["i", "love"]
```

```cpp
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        // Step 1: 统计频率
        unordered_map<string, int> freq;
        for (const string& w : words) {
            freq[w]++;
        }

        // Step 2: 提取所有不同的单词
        vector<string> candidates;
        for (auto& [word, _] : freq) {
            candidates.push_back(word);
        }

        // Step 3: 自定义排序
        sort(candidates.begin(), candidates.end(),
            [&](const string& a, const string& b) {
                if (freq[a] != freq[b]) return freq[a] > freq[b]; // 频率降序
                return a < b;  // 同频字典序升序
            });

        // Step 4: 取前 k 个
        return vector<string>(candidates.begin(), candidates.begin() + k);
    }
};
```

**关键点**:
- 比较器中 `freq[a] > freq[b]` 实现降序（频率高排前面）。
- `a < b` 实现升序（字典序小排前面）。
- 时间 O(m + m log m)，其中 m 是不同单词数量（m <= n）。

---

### 解法2: 最小堆（大小 k） — O(n log k) / O(n) ⭐ 进阶必会

**从解法1优化**: 排序是 O(n log n)，如果 k 远小于 n，我们只关心前 k 个，不需要全排序。用大小为 k 的最小堆，只保留"当前最重要的 k 个元素"。

```
TopK 堆的工作原理:

要找 Top 2 高频词 (k=2):
使用最小堆 (堆顶 = 最不重要的):
  堆的比较: 频率低优先; 同频字典序大优先 (和最终排序方向相反!)

处理顺序:
  "i"(2) → 堆: ["i"(2)]                      堆大小<k, 直接入堆
  "love"(2) → 堆: ["love"(2), "i"(2)]        堆大小<k, 直接入堆
                    ↑ 堆顶: love (同频2, 字典序大 → 最不重要)
  "leetcode"(1) → 频率1 < 堆顶频率2 → 丢弃    不够格进堆
  "coding"(1) → 频率1 < 堆顶频率2 → 丢弃      不够格进堆

堆中: ["love"(2), "i"(2)]
弹出顺序: "love", "i" (从最不重要到最重要)
反转后: ["i", "love"] ✓
```

```cpp
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        // Step 1: 统计频率
        unordered_map<string, int> freq;
        for (const string& w : words) freq[w]++;

        // Step 2: 定义最小堆的比较器
        // 堆顶 = 最不重要的 = 频率最低的 / 同频字典序最大的
        // 比较器返回 true 表示 a 比 b 更"重要"(a 沉在下面, b 浮在上面)
        auto cmp = [&](const string& a, const string& b) {
            if (freq[a] != freq[b]) return freq[a] > freq[b]; // 频率高的沉底(更重要)
            return a < b;  // 字典序小的沉底(更重要)
        };

        // Step 3: 维护大小为 k 的最小堆
        priority_queue<string, vector<string>, decltype(cmp)> minHeap(cmp);

        for (auto& [word, _] : freq) {
            minHeap.push(word);
            if ((int)minHeap.size() > k) {
                minHeap.pop();  // 弹出最不重要的
            }
        }

        // Step 4: 弹出并反转
        vector<string> result(k);
        for (int i = k - 1; i >= 0; i--) {
            result[i] = minHeap.top();
            minHeap.pop();
        }

        return result;
    }
};
```

**关键点**:
- **比较器方向的理解（最难的部分）**：
  - `priority_queue` 的比较器中，`cmp(a, b) = true` 表示 a 的优先级**低于** b（a 排在 b 后面）。
  - 我们想让堆顶是"最不重要的"（频率最低 / 同频字典序最大），所以"频率高"和"字典序小"的元素应该沉底（返回 true）。
- `result[i] = minHeap.top()` 从 i=k-1 填到 i=0，等效于弹出后反转。
- 时间 O(n log k)：每个元素入堆/出堆是 O(log k)，共 n 个元素。

---

### 解法3: 桶排序 — O(n) / O(n)

**不同视角**: 按频率建桶，频率为 i 的单词放入第 i 个桶。从高频桶开始取，直到取够 k 个。桶内需要排序（字典序）。

```cpp
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        unordered_map<string, int> freq;
        for (const string& w : words) freq[w]++;

        int maxFreq = 0;
        for (auto& [_, f] : freq) maxFreq = max(maxFreq, f);

        // 桶: bucket[i] = 出现 i 次的单词列表
        vector<vector<string>> buckets(maxFreq + 1);
        for (auto& [word, f] : freq) {
            buckets[f].push_back(word);
        }

        // 桶内按字典序排序
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end());
        }

        // 从高频桶开始取
        vector<string> result;
        for (int i = maxFreq; i >= 0 && (int)result.size() < k; i--) {
            for (const string& w : buckets[i]) {
                result.push_back(w);
                if ((int)result.size() == k) break;
            }
        }

        return result;
    }
};
```

**关键点**:
- 桶排序的时间取决于桶内排序。如果桶内单词少，近似 O(n)。
- 但最坏情况下（所有单词频率相同），桶内排序退化为 O(n log n)。

## 解法对比

|  | 哈希+排序 | 最小堆 | 桶排序 |
|--|----------|--------|--------|
| 时间 | O(n log n) | **O(n log k)** | O(n) 平均 |
| 空间 | O(n) | O(n) | O(n) |
| 实现难度 | 低 | 中（比较器容易错） | 中 |
| Follow-up | 不满足 | ⭐ 满足 O(n log k) | 满足 |

**面试选择**: 先写排序解法（简单不易错），然后讲最小堆的优化思路（满足 follow-up）。桶排序在面试中不常要求，但知道思路是加分项。

## 易错点

1. **最小堆的比较器方向写反**：
   - ✗ 排序时 `freq[a] > freq[b]`（频率高排前面），堆里也用同样的比较器 → 变成最大堆，堆顶是最重要的，每次弹出的是最重要的，结果完全反了
   - ✓ 堆的比较器要和排序**方向相反**：频率高的沉底（返回 true），堆顶是频率最低的

2. **同频时字典序方向搞反**：
   - ✗ 排序时写 `a > b`（字典序降序）→ "love" 排在 "i" 前面
   - ✓ 排序时写 `a < b`（字典序升序）→ "i" 排在 "love" 前面

3. **堆弹出后忘记反转**：
   - ✗ 最小堆弹出顺序是从不重要到重要，直接返回 → 顺序反了
   - ✓ 从后往前填充 result 数组，或弹出后 reverse

4. **priority_queue 的比较器理解错误**：
   - C++ `priority_queue` 默认是最大堆，`less<>` 比较器的含义是"a < b 时 a 优先级低"
   - 自定义 lambda 时，返回 true 表示第一个参数优先级**低**（排在后面）

5. **频率统计时用 `map` 而非 `unordered_map`**：
   - 不是错误，但 `map` 是 O(log n) 插入/查找，`unordered_map` 是 O(1) 平均
   - 对于频率统计这种高频操作，`unordered_map` 更高效

## 面试追问

**Q1: Follow-up 要求 O(n log k) 时间和 O(n) 空间，你的哪个解法满足？**
> 最小堆解法满足。频率统计 O(n)，堆操作 O(m log k) 其中 m 是不同单词数（m <= n），总计 O(n log k)。空间 O(n) 来自哈希表 + O(k) 来自堆 = O(n)。

**Q2: 如果数据是流式的（单词一个一个到来），怎么实时维护 Top K？**
> 维护一个哈希表记录频率，和一个大小为 k 的堆。每来一个新单词，更新频率后，检查它是否应该进入堆。难点是堆中的元素频率也在变化，需要用支持 decrease-key 操作的堆（如 TreeMap/TreeSet），或者用"懒删除"策略。

**Q3: 如何用快速选择（QuickSelect）解决 TopK？**
> 快速选择可以在 O(n) 平均时间找到第 k 大的元素（分割点），但本题还需要排序（频率降序+字典序），所以快速选择后仍需对前 k 个排序，总时间 O(n + k log k)。当 k << n 时比全排序快。

**Q4: 桶排序方案最坏情况的时间复杂度？如何优化？**
> 所有单词频率相同时，只有一个桶包含所有单词，桶内排序 O(n log n)。优化：桶内用 Trie（前缀树）代替排序，按字典序遍历 Trie 取前 k 个，可以做到 O(n * L)（L 是单词平均长度）。

## 相关题型

- **[347. 前 K 个高频元素](../0347_top_k_frequent_elements/)** — 本题的简化版：只需要按频率排序，不需要处理同频字典序。可以用最小堆或桶排序。
- **[215. 数组中的第K个最大元素](../0215_kth_largest_element_in_an_array/)** — 纯粹的 TopK 问题，最小堆或快速选择。没有自定义排序规则。
- **[451. 根据字符出现频率排序](../0451_sort_characters_by_frequency/)** — 按频率排序所有字符（不是取 TopK），桶排序是最优解。
- **[973. 最接近原点的 K 个点](../0973_k_closest_points_to_origin/)** — TopK 的变体：用距离作为排序键，最大堆维护 k 个最近的点。
