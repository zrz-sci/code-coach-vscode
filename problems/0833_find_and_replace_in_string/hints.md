# 833. 字符串中的查找与替换

## 核心思路

本质上就是：在原字符串的若干指定位置同时做"查找-替换"操作。关键难点在于"同时"——替换操作要基于**原始字符串的索引**，而替换后字符串长度会变化导致索引偏移。我们需要一种方式让所有替换都"看到"原始索引。

## 思维链

1. **读完题第一反应**：对每个替换操作，检查 `s` 在 `indices[i]` 处是否匹配 `sources[i]`，匹配则替换成 `targets[i]`。如果从左到右逐个替换，每次替换后字符串长度变了，后续的 `indices` 就对不上了。

2. **暴力想法 — 从右到左替换**：如果按 `indices` 从大到小排序再替换，那么前面的索引不会受后面替换的影响（因为替换发生在更后面的位置）。这是一个简单且正确的思路。

3. **另一个角度 — 逐字符构建**：不修改原字符串，而是从左到右遍历原字符串，逐字符决定"这个位置是直接拷贝原字符，还是用 target 替换"。用哈希表预处理每个 `indices[i]` 对应哪个替换操作，遍历时 O(1) 查表。

4. **匹配检查**：在 `s` 的位置 `idx` 处检查是否以 `sources[i]` 开头，直接用 `s.substr(idx, len) == sources[i]` 或逐字符比较。

5. **构建结果**：匹配成功则追加 `targets[i]` 并跳过 `sources[i].length()` 个字符；不匹配则追加原字符并前进一步。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 从右到左排序替换 | 按索引降序排序，直接在原字符串上替换 | O(n + k·m) | O(k) | 能说出即可 |
| 哈希表 + 逐字符构建 | 哈希表记录替换操作，从左到右一遍扫描构建 | O(n + k·m) | O(n + k) | ⭐ 必须写出 |

> 其中 n = s.length(), k = 操作数, m = sources 最大长度

## 关键提示

1. **"同时替换"怎么理解？** 所有替换都基于原始字符串的索引判断，不受其他替换的影响。关键是避免一次替换导致后续索引错位。

2. **从右到左是经典技巧**：字符串/数组的"多处修改不互相干扰"问题，从后往前处理是常见思路（类似合并两个有序数组从后往前填）。

3. **哈希表映射**：用 `indices[i] → i` 的映射表，在遍历 `s` 时 O(1) 查询当前位置是否有替换操作。

4. **匹配失败要跳过**：如果 `indices[i]` 处的子串不匹配 `sources[i]`，该位置的原字符要保留，不能遗漏。

5. **遍历时的跳跃**：匹配成功后，指针要跳过 `sources[i].length()` 个位置，不是跳过 `targets[i].length()`（因为我们是在原字符串上定位）。

```
// 示例演示 s = "abcd", indices=[0,2], sources=["a","cd"], targets=["eee","ffff"]
//
// 原始字符串:  a   b   c   d
// 索引:        0   1   2   3
//
// 操作0: idx=0, "a" 匹配 s[0..0]="a" ✓ → 替换成 "eee"
// 操作1: idx=2, "cd" 匹配 s[2..3]="cd" ✓ → 替换成 "ffff"
//
// 逐字符构建:
// i=0: 查表发现有操作0, s[0..0]=="a" ✓ → 追加"eee", 跳到 i=1
// i=1: 查表无操作 → 追加"b", i=2
// i=2: 查表发现有操作1, s[2..3]=="cd" ✓ → 追加"ffff", 跳到 i=4
// i=4: 超出范围，结束
// 结果: "eee" + "b" + "ffff" = "eeebffff"
```

## 解法详解

### 解法1: 从右到左排序替换 — O(n + k·m) / O(k)

**思考过程**: 直觉上想直接在字符串上做 `s.replace()`，但从左到右替换会导致索引偏移。反过来想，如果从右到左替换，较后位置的替换不影响较前位置的索引。所以只需按 `indices` 降序排序后依次替换即可。

```cpp
class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        int k = indices.size();
        // 创建排序索引数组，按 indices 降序排
        vector<int> order(k);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return indices[a] > indices[b]; // 降序：从右到左处理
        });
        
        for (int i : order) {
            int idx = indices[i];
            // 检查 s 在 idx 处是否匹配 sources[i]
            if (s.substr(idx, sources[i].size()) == sources[i]) {
                // 直接替换：因为从右到左，不影响左边的索引
                s = s.substr(0, idx) + targets[i] + s.substr(idx + sources[i].size());
            }
        }
        return s;
    }
};
```

**关键点**: 
- `s.substr()` 拼接虽然直观但有额外字符串拷贝开销，对于 n ≤ 1000 完全可以接受。
- 排序保证从右到左处理，后面的替换不影响前面的索引。

### 解法2: 哈希表 + 逐字符构建 — O(n + k·m) / O(n + k) ⭐ 面试首选

**从解法1优化**: 解法1每次替换都要拼接字符串（隐含拷贝）。更优雅的方式是：不修改原字符串，从左到右一遍扫描，用哈希表 O(1) 查询当前位置是否有替换操作，直接构建结果字符串。

```cpp
class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        int n = s.size(), k = indices.size();
        
        // 哈希表：原始索引 → 操作编号
        unordered_map<int, int> lookup;
        for (int i = 0; i < k; i++) {
            lookup[indices[i]] = i;
        }
        
        string result;
        int i = 0;
        while (i < n) {
            auto it = lookup.find(i);
            if (it != lookup.end()) {
                int opIdx = it->second;
                int len = sources[opIdx].size();
                // 检查原字符串在位置 i 是否匹配 sources[opIdx]
                if (s.substr(i, len) == sources[opIdx]) {
                    result += targets[opIdx]; // 匹配成功，追加 target
                    i += len;                 // 跳过 source 长度（原字符串上的长度）
                    continue;
                }
            }
            // 没有替换操作，或者匹配失败，拷贝原字符
            result += s[i];
            i++;
        }
        return result;
    }
};
```

**关键点**:
- 整个过程只扫描原字符串一遍，不需要排序。
- 哈希表查询 O(1)，匹配检查 O(m)，总体 O(n + k·m)。
- 跳过的是 `sources[opIdx].size()` 而不是 `targets[opIdx].size()`，因为我们在原字符串上移动指针。

## 解法对比

| | 解法1: 从右到左替换 | 解法2: 哈希表+逐字符构建 |
|---|---|---|
| **核心思想** | 排序后从右到左直接修改 | 不修改原串，从左到右构建 |
| **排序需求** | 需要按索引降序排序 | 不需要排序 |
| **字符串拷贝** | 每次替换都有字符串拼接 | 只追加，无中间拷贝 |
| **代码简洁度** | 更直观 | 更高效 |
| **面试推荐** | 作为第一想法说出来 | 作为优化方案写出来 |

**什么时候选哪个？** 面试时先说解法1的思路展示理解，然后写解法2展示编码能力。

## 易错点

1. **跳过长度搞错**：
   - ✗ `i += targets[opIdx].size()` — 用了 target 的长度
   - ✓ `i += sources[opIdx].size()` — 应该用 source 的长度，因为指针在原字符串上移动

2. **匹配失败时忘记拷贝原字符**：
   - ✗ 匹配失败后直接 `continue`，导致该位置的字符丢失
   - ✓ 匹配失败后 `result += s[i]; i++;`

3. **哈希表建反了**：
   - ✗ `lookup[i] = indices[i]` — 键值对反了
   - ✓ `lookup[indices[i]] = i` — 键是原字符串索引，值是操作编号

4. **解法1忘记排序就直接替换**：
   - ✗ 按原始顺序从左到右替换 → 前面的替换导致后面索引错位
   - ✓ 必须按 `indices` 降序排序后再替换

## 面试追问

**Q1: 为什么不能直接从左到右逐个替换？**
A: 替换可能改变字符串长度（source 和 target 长度不同），导致后续操作的索引在修改后的字符串上不再准确。题目要求"同时"替换，即所有操作都基于原始字符串的索引。

**Q2: 如果替换操作可能重叠怎么办？（例如 indices=[0,1], sources=["ab","bc"]）**
A: 题目保证不重叠。但如果允许重叠，需要定义优先级（如先匹配长的、或按给定顺序优先）。可以按索引排序后贪心匹配，匹配成功的操作标记其覆盖范围，后续操作如果与已匹配区间冲突则跳过。

**Q3: 如果 s 非常长（10^6），k 非常大（10^5），sources 也很长，如何优化匹配？**
A: 当前的 `substr` 比较是 O(m)，总体 O(n + k·m)。如果 m 很大，可以用 Rabin-Karp 哈希做 O(1) 的子串比较（预计算哈希），或用 Trie 树对所有 sources 建索引，在遍历 s 时快速查找匹配。

## 相关题型

- **LeetCode 722. 删除注释** — 同样是字符串扫描+状态判断的逐字符构建思路，区别在于需要维护"是否在注释内"的状态。
- **LeetCode 68. 文本左右对齐** — 字符串构建题，复用"逐段构建结果字符串"的模式。
- **LeetCode 616. 给字符串添加加粗标签** — 也是在原字符串的多个位置做标记/替换，复用哈希表标记位置 + 逐字符构建的框架。