# 451. 根据字符出现频率排序

## 核心思路

本质上就是一个**统计+排序**问题：先统计每个字符出现的次数，再按次数从高到低把字符拼接起来。关键在于"排序"这一步用什么方式最高效。

## 思维链

1. **读完题第一反应**：需要知道每个字符出现了多少次 → 用哈希表统计频率，这一步没得跑。
2. **频率统计完了，然后呢？** 要按频率降序排列 → 最直觉：把 (字符, 频率) 对收集起来，按频率排序，然后逐个拼接。
3. **排序的代价是什么？** 字符种类最多 62 个（a-z, A-Z, 0-9），所以排序的对象很少，O(k log k) 其中 k ≤ 62，基本可以忽略。瓶颈其实在统计 O(n) 和最终拼接 O(n)。
4. **有没有不用比较排序的方法？** 频率的范围是 [1, n]，可以用**桶排序**：创建 n+1 个桶，桶 i 放所有出现了 i 次的字符，然后从大桶到小桶遍历。这样连 O(k log k) 的排序都省了。
5. **还有什么方式？** 用**最大堆（优先队列）**：每次弹出频率最高的字符，天然有序。适合需要"动态取最大"的场景。
6. **总结**：三种方式殊途同归 — 区别仅在"按频率排序"这一步用什么数据结构。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希表 + 排序 | 统计频率，对字符按频率排序 | O(n + k log k) | O(n) | ⭐ 必须写出 |
| 哈希表 + 最大堆 | 统计频率，用堆逐个取最大 | O(n + k log k) | O(n) | 能说出即可 |
| 哈希表 + 桶排序 | 统计频率，桶排序避免比较排序 | O(n) | O(n) | 加分项 |

> 其中 k 是不同字符数量，本题 k ≤ 62，所以三种方法实际差异极小。面试中解法1最简洁易写。

## 关键提示

1. **字符种类有限**：只有大小写字母和数字，最多 62 种。这意味着"排序"的规模极小，不要被 n ≤ 5×10⁵ 吓到。
2. **统计频率是所有解法的共同第一步**：先想清楚"统计完之后怎么排"，再选数据结构。
3. **桶排序的桶索引就是频率值**：频率范围 [1, n]，桶 i 存放所有出现 i 次的字符，从桶 n 倒序遍历到桶 1。
4. **拼接字符串时，用 `string(count, char)` 一次性生成**，避免逐字符 push_back。
5. **大小写敏感**：'A' 和 'a' 是不同字符，不要合并统计。

## 解法详解

### 解法1: 哈希表 + 排序 — O(n + k log k) / O(n) ⭐ 面试首选

**思考过程**：最直觉的做法。统计完频率后，把所有出现过的字符收集起来，按频率降序排序，然后逐个字符按其频率拼接到结果字符串。

```
示例: s = "tree"
Step1: 统计频率 → {t:1, r:1, e:2}
Step2: 按频率排序 → [(e,2), (t,1), (r,1)]  (t和r顺序任意)
Step3: 拼接 → "ee" + "t" + "r" = "eetr"
```

```cpp
class Solution {
public:
    string frequencySort(string s) {
        // Step1: 统计每个字符的频率
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }
        
        // Step2: 把所有出现过的字符收集起来，按频率降序排序
        vector<pair<char, int>> chars(freq.begin(), freq.end());
        sort(chars.begin(), chars.end(), [](const pair<char,int>& a, const pair<char,int>& b) {
            return a.second > b.second; // 频率高的排前面
        });
        
        // Step3: 按排序结果拼接字符串
        string result;
        for (auto& [ch, cnt] : chars) {
            result += string(cnt, ch); // 一次性追加 cnt 个 ch
        }
        return result;
    }
};
```

**关键点**：排序的对象是"字符种类"而非原字符串，排序规模极小（≤62），所以极高效。

### 解法2: 哈希表 + 最大堆 — O(n + k log k) / O(n)

**从解法1的视角看**：排序本质上就是找"频率最高→次高→..."的顺序，这正是最大堆擅长的事。用堆的好处是：如果题目改成"只要前 K 个最高频字符"，堆可以提前终止。

```cpp
class Solution {
public:
    string frequencySort(string s) {
        // Step1: 统计频率
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }
        
        // Step2: 放入最大堆（按频率排序）
        // pair<频率, 字符>，默认按 first 排序，所以频率放前面
        priority_queue<pair<int, char>> maxHeap;
        for (auto& [ch, cnt] : freq) {
            maxHeap.push({cnt, ch});
        }
        
        // Step3: 逐个弹出堆顶，拼接结果
        string result;
        while (!maxHeap.empty()) {
            auto [cnt, ch] = maxHeap.top();
            maxHeap.pop();
            result += string(cnt, ch);
        }
        return result;
    }
};
```

**关键点**：`priority_queue<pair<int,char>>` 默认是大顶堆，pair 先按第一个元素比较，所以把频率放在 first 位置。

### 解法3: 哈希表 + 桶排序 — O(n) / O(n)

**从解法1优化**：解法1 用了比较排序 O(k log k)。虽然 k 很小可以忽略，但如果追求理论最优，可以用桶排序彻底消除比较。桶的索引就是频率值，范围 [1, n]。

```
示例: s = "tree" (n=4)
频率统计: {t:1, r:1, e:2}

桶:
bucket[0]: (空)
bucket[1]: [t, r]    ← 出现1次的字符
bucket[2]: [e]       ← 出现2次的字符
bucket[3]: (空)
bucket[4]: (空)

从后往前遍历桶: bucket[4]→bucket[3]→bucket[2]→bucket[1]
→ "ee" + "tr" = "eetr"
```

```cpp
class Solution {
public:
    string frequencySort(string s) {
        int n = s.size();
        
        // Step1: 统计频率
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }
        
        // Step2: 桶排序 — bucket[i] 存所有出现 i 次的字符
        vector<vector<char>> buckets(n + 1);
        for (auto& [ch, cnt] : freq) {
            buckets[cnt].push_back(ch);
        }
        
        // Step3: 从高频到低频遍历桶，拼接结果
        string result;
        for (int i = n; i >= 1; i--) {
            for (char ch : buckets[i]) {
                result += string(i, ch); // 字符 ch 出现了 i 次
            }
        }
        return result;
    }
};
```

**关键点**：桶的大小是 n+1 而不是某个固定值，因为一个字符最多出现 n 次。遍历桶时大部分桶是空的，但总工作量仍是 O(n)。

## 解法对比

| | 解法1: 排序 | 解法2: 最大堆 | 解法3: 桶排序 |
|---|---|---|---|
| 时间 | O(n + k log k) | O(n + k log k) | O(n) |
| 空间 | O(n) | O(n) | O(n) |
| 代码量 | 最少 | 中等 | 稍多 |
| 核心操作 | sort + lambda | priority_queue | 额外数组 |
| 适用场景 | 通用，面试首选 | 需要动态取最大时 | 追求理论最优 |

> 由于 k ≤ 62，三种方法实际运行时间几乎相同。面试中写解法1最快最清晰。

## 易错点

1. **忘记大小写敏感**：
   - ✗ 把 `'A'` 和 `'a'` 当同一字符处理
   - ✓ 题目明确说 `'A'` 和 `'a'` 是不同字符，直接用 `unordered_map<char, int>` 不会有问题

2. **桶排序的桶大小写成固定值**：
   - ✗ `vector<vector<char>> buckets(128)` — 如果字符串长度 > 128，频率可能超出范围
   - ✓ `vector<vector<char>> buckets(n + 1)` — 桶大小取决于字符串长度

3. **排序方向写反**：
   - ✗ `a.second < b.second`（升序，低频在前）
   - ✓ `a.second > b.second`（降序，高频在前）

4. **拼接效率**：
   - ✗ 逐字符 `result += ch` 循环 cnt 次 — 没问题但不够优雅
   - ✓ `result += string(cnt, ch)` — 一行搞定，更清晰

## 面试追问

**Q1: 时间复杂度是多少？O(n log n) 还是 O(n)？**
→ 取决于排序的对象。排序的是字符种类（最多62个），不是整个字符串，所以排序部分是 O(k log k) ≈ O(1)，总体 O(n)。桶排序版本严格 O(n)。

**Q2: 如果字符集不是 62 个，而是整个 Unicode（几十万种字符），哪种解法更好？**
→ 桶排序的桶大小取决于字符串长度 n 而非字符种类数，所以仍然 O(n)。排序变成 O(k log k)，k 可能很大。堆也是 O(k log k)。所以桶排序最优。

**Q3: 如果要求频率相同的字符按字典序排列呢？**
→ 解法1：sort 的 lambda 加第二个比较条件 `if (a.second == b.second) return a.first < b.first;`。解法3：桶内字符排序。堆：pair 改为 `{cnt, -ch}` 或自定义比较器。

**Q4: 能否原地排序，不用额外空间？**
→ 理论上可以对原字符串自定义排序（先统计频率，再按频率排序原字符串每个字符），时间 O(n log n)，空间 O(k)。但实际中拼接新字符串更清晰，面试中不推荐原地做法。

## 相关题型

- **347. 前 K 个高频元素** — 复用同样的"统计频率 + 排序/堆/桶排序"框架，区别：347只取前K个（堆更有优势），本题要全部输出
- **692. 前K个高频单词** — 从字符频率变成单词频率，需要处理同频时的字典序，复用桶排序但桶内需要排序
- **1636. 按照频率将数组升序排序** — 几乎同一道题但要求升序，且同频按值降序。排序 lambda 条件改一下即可