# 14. 最长公共前缀

## 核心思路

本质上在问：**所有字符串从第 0 个字符开始，最多能"齐步走"多远？** 也就是逐列扫描，找到第一个"不一致"的位置。

## 思维链

1. **读完题第一反应**：公共前缀 = 所有字符串都拥有的起始部分。最朴素的想法是一个字符一个字符地检查，所有字符串的第 0 个字符一样吗？第 1 个呢？第 2 个呢？直到发现不一样或某个字符串到头了。
2. **换个角度想**：也可以先拿第一个字符串当"候选前缀"，逐个和剩下的字符串比较，不断"削短"候选前缀，直到它是所有字符串的前缀为止。
3. **能不能更快？** 如果先排序，最小和最大的字符串的公共前缀就是答案（因为字典序排序后，差异最大的一对一定是首尾两个）。
4. **进阶思路**：分治法——把数组分成两半，分别求公共前缀，再合并。或者用 Trie（前缀树）——所有字符串插入后，沿着只有单一孩子的路径走到底。
5. **面试中怎么选？** 纵向扫描（逐列比较）代码最简洁、思路最直接，是面试首选。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 纵向扫描 | 逐列比较所有字符串的同一位置 | O(S) | O(1) | ⭐ 必须写出 |
| 横向扫描 | 用第一个字符串逐个与后续比较，不断削短 | O(S) | O(1) | 能说出即可 |
| 排序后比首尾 | 排序后只需比较第一个和最后一个 | O(S + n·m·log n) | O(1)* | 加分项 |
| 分治 | 分成两半分别求，再合并 | O(S) | O(m·log n) | 加分项 |

> S = 所有字符串字符总数, n = 字符串数量, m = 最短字符串长度

## 关键提示

1. **从最简单的情况想起**：如果只有 2 个字符串，你怎么求公共前缀？扩展到 n 个呢？
2. **提前终止**：一旦发现某个位置不匹配，或某个字符串已经到末尾，就可以立刻返回。
3. **谁来当"标尺"？** 纵向扫描时，拿第一个字符串的长度作为外层循环上界，但别忘了其他字符串可能更短。
4. **排序的妙用**：字典序排序后，`strs[0]` 和 `strs[n-1]` 的公共前缀一定 ≤ 任何两个字符串的公共前缀。想想为什么？
5. **边界**：数组为空？只有一个字符串？字符串本身为空？

```
纵向扫描可视化 (strs = ["flower","flow","flight"]):

位置:   0   1   2   3   4   5
       f   l   o   w   e   r     ← "flower"
       f   l   o   w             ← "flow"
       f   l   i   g   h   t    ← "flight"
       ✓   ✓   ✗                 ← 位置2: o≠i, 停止!
       
结果: "fl"
```

## 解法详解

### 解法1: 纵向扫描 — O(S) / O(1) ⭐ 面试首选

**思考过程**：最直觉的方法——把所有字符串"竖着对齐"，从第 0 列开始逐列检查。每一列的字符都相同就继续，遇到不同或某个字符串到头了就停止。

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        // 以第一个字符串为基准，逐列检查
        for (int col = 0; col < (int)strs[0].size(); col++) {
            char c = strs[0][col];
            // 检查所有其他字符串在这一列是否都等于 c
            for (int row = 1; row < (int)strs.size(); row++) {
                // 两种停止条件：
                // 1) 当前字符串已经到末尾（它比基准短）
                // 2) 当前字符不匹配
                if (col >= (int)strs[row].size() || strs[row][col] != c) {
                    return strs[0].substr(0, col);
                }
            }
        }
        // 第一个字符串本身就是公共前缀（它是最短的或所有字符串都相同）
        return strs[0];
    }
};
```

**关键点**：
- 外层遍历"列"（字符位置），内层遍历"行"（每个字符串）
- `col >= strs[row].size()` 必须在 `strs[row][col]` 之前判断，否则越界

---

### 解法2: 横向扫描 — O(S) / O(1)

**思考过程**：换个角度——先把 `strs[0]` 当作候选前缀 `prefix`，然后拿它和 `strs[1]` 比较，削短 `prefix`，再和 `strs[2]` 比较，继续削短……最终 `prefix` 就是答案。

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        string prefix = strs[0];
        
        for (int i = 1; i < (int)strs.size(); i++) {
            // 不断削短 prefix，直到 strs[i] 以 prefix 开头
            while (strs[i].find(prefix) != 0) {
                prefix = prefix.substr(0, prefix.size() - 1);
                if (prefix.empty()) return "";
            }
        }
        return prefix;
    }
};
```

**关键点**：
- `strs[i].find(prefix) != 0` 表示 `prefix` 不是 `strs[i]` 的前缀（`find` 返回的起始位置不是 0）
- 每次削掉一个字符，最坏情况效率不如纵向扫描（但渐近一样）

---

### 解法3: 排序后比较首尾 — O(n·m·log n) / O(1)*

**从解法1优化角度**：字典序排序后，差异最大的一定是排序后的第一个和最后一个字符串。它们的公共前缀就是所有字符串的公共前缀。

**为什么正确？** 假设排序后 `strs[0]` 和 `strs[n-1]` 的公共前缀长度为 L。对于任意中间的 `strs[k]`，根据字典序 `strs[0] ≤ strs[k] ≤ strs[n-1]`，所以 `strs[k]` 的前 L 个字符一定和 `strs[0]` 相同（否则字典序关系不成立）。

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        sort(strs.begin(), strs.end());
        
        // 只需比较排序后第一个和最后一个
        const string& first = strs.front();
        const string& last = strs.back();
        
        int i = 0;
        while (i < (int)first.size() && i < (int)last.size() 
               && first[i] == last[i]) {
            i++;
        }
        return first.substr(0, i);
    }
};
```

**关键点**：
- 排序开销 O(n·m·log n)，对大规模数据不如纵向扫描
- 但代码非常简洁优雅，面试中提一下能展示思维广度

---

### 解法4: 分治法 — O(S) / O(m·log n)

**思考过程**：分治思路——把数组分成左右两半，分别求各自的公共前缀 `leftLCP` 和 `rightLCP`，然后再求 `leftLCP` 和 `rightLCP` 的公共前缀，就是整体答案。

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        return divideAndConquer(strs, 0, strs.size() - 1);
    }
    
private:
    string divideAndConquer(vector<string>& strs, int left, int right) {
        if (left == right) return strs[left];  // base case: 单个字符串
        
        int mid = left + (right - left) / 2;
        string leftLCP = divideAndConquer(strs, left, mid);
        string rightLCP = divideAndConquer(strs, mid + 1, right);
        
        return commonPrefix(leftLCP, rightLCP);
    }
    
    // 求两个字符串的公共前缀
    string commonPrefix(const string& a, const string& b) {
        int len = min(a.size(), b.size());
        int i = 0;
        while (i < len && a[i] == b[i]) i++;
        return a.substr(0, i);
    }
};
```

**关键点**：
- 递归深度 O(log n)，每层处理 O(m)，总时间 O(S)
- 额外空间来自递归栈 O(log n) 和中间字符串 O(m)

---

## 解法对比

| | 纵向扫描 | 横向扫描 | 排序比首尾 | 分治 |
|---|---|---|---|---|
| 时间 | O(S) | O(S) | O(S + n·m·logn) | O(S) |
| 空间 | O(1) | O(m) substr | O(1)* | O(m·log n) |
| 代码量 | 最少 | 简单 | 简洁 | 较多 |
| 面试推荐 | ⭐⭐⭐ | ⭐⭐ | ⭐ | ⭐ |
| 优势 | 最优且最直觉 | 易理解 | 巧妙 | 展示分治思维 |

> *排序本身可能需要额外空间取决于实现，但不额外开辟数组

**面试中选纵向扫描**——思路清晰、代码简洁、时间最优、空间最优。

## 易错点

1. **忘记检查其他字符串的长度导致越界**
   - ✗ `if (strs[row][col] != c)` — 当 `strs[row]` 比 `strs[0]` 短时直接越界
   - ✓ `if (col >= strs[row].size() || strs[row][col] != c)` — 先判长度

2. **横向扫描用 `find` 的返回值误判**
   - ✗ `while (strs[i].find(prefix))` — `find` 返回 `string::npos`（一个很大的正数），非零也可能表示在其他位置找到
   - ✓ `while (strs[i].find(prefix) != 0)` — 明确要求必须在位置 0 找到

3. **输入只有一个字符串时，直接返回它本身**
   - 纵向扫描自然处理（循环不进入内层），但横向扫描如果从 `i=0` 开始可能多比较

4. **空字符串在数组中**
   - `strs = ["", "abc"]` → 应返回 `""`
   - 纵向扫描：`col >= strs[row].size()` 即 `0 >= 0`，在第一列就返回空串 ✓

## 面试追问

**Q1: 你的解法时间复杂度是多少？能不能比 O(S) 更快？**
> 不能。最坏情况下所有字符串完全相同，必须检查每个字符，所以 O(S) 是下界。

**Q2: 如果这个函数要被频繁调用，每次查询不同的字符串子集，怎么优化？**
> 预处理阶段把所有字符串插入 **Trie（前缀树）**。查询时从根节点往下走，只要当前节点只有一个子节点且不是某个字符串的结尾，就继续走。走到分叉点或结尾就是公共前缀。预处理 O(S)，每次查询 O(m)。

**Q3: 如果字符串非常长（百万级），但数量很少（10个），有什么优化？**
> 可以用**二分查找**优化前缀长度。在 `[0, minLen]` 上二分，`mid` 位置检查所有字符串的前 `mid` 个字符是否相同。时间仍然 O(S·log m)——但实际比较次数可能更少因为可以提前排除长前缀。

## 相关题型

- **LeetCode 28. 找出字符串中第一个匹配项的下标** — 复用逐字符比较的模式，但这里是子串匹配而非前缀
- **LeetCode 208. 实现 Trie (前缀树)** — 本题 Follow-up 的核心数据结构，学会后可以用 Trie 解决本题的批量查询版本
- **LeetCode 720. 词典中最长的单词** — 也是前缀相关问题，Trie 上做 DFS/BFS 找最长可构建单词