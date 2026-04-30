# LCR 119. 最长连续序列

## 核心思路

给定未排序的整数数组，找出数字连续的最长序列的长度（不要求元素在原数组中连续）。要求时间复杂度 O(n)。关键洞察：**只从序列的起点开始计数**——如果 `num-1` 不在集合中，说明 `num` 是某条连续链的起点，从它开始往后数。

## 思维链

1. **读完题第一反应** → 排序后扫描连续段，O(n log n)。但题目要求 O(n)。
2. **O(n) 怎么做？** → 需要 O(1) 的查找。HashSet！把所有数扔进 unordered_set。
3. **直接对每个数向后延伸？** → 如果对每个数都 while 查 num+1, num+2...，最坏情况还是 O(n^2)。比如 [1,2,3,...,n]，对每个数都向后延伸。
4. **怎么保证总体 O(n)？** → 只从"没有前驱的数"开始！如果 num-1 在集合中，说明 num 不是起点，跳过。这样每条连续链只从起点被遍历一次。
5. **为什么总体是 O(n)？** → 每个元素最多被"从起点向后数"的过程访问一次。总计所有链的长度加起来就是 n。加上外层遍历 n 次，总计 2n = O(n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 + 扫描 | 排序后线性扫描连续段 | O(n log n) | O(1)/O(n) | 能说出即可 |
| HashSet + 起点枚举 | 只从没有前驱的数开始延伸 | O(n) | O(n) | ⭐ 必须写出 |
| 并查集 (Union Find) | 将相邻数合并，查最大集合 | O(n) | O(n) | 进阶加分项 |

## 关键提示

1. **去重**：`unordered_set` 构造时自动去重。重复元素不影响连续序列长度（如 [0,0,1] 最长连续是 [0,1] 长度 2）。
2. **只从起点开始**：`if (!s.count(num - 1))` 这个判断是保证 O(n) 的关键。没有这个判断，时间复杂度退化为 O(n^2)。
3. **遍历 set 而非原数组**：如果遍历原数组且有重复元素（如 [0,0,1]），同一个起点会被触发多次。遍历 set 保证每个值只处理一次。
4. **空数组**：直接返回 0。set 为空时 for 循环不执行。
5. **本题与主站 128 题完全相同**。

## 解法详解

### 解法1: HashSet + 起点枚举 — O(n) / O(n) ⭐ 面试首选

**思考过程**: 对每个数，如果它是一条连续链的起点（前面没有 num-1），就往后数这条链有多长。

```
示例: nums = [100, 4, 200, 1, 3, 2]

Step 1: 构建 HashSet = {100, 4, 200, 1, 3, 2}

Step 2: 遍历集合中的每个数

  num = 100:
    100-1=99 不在集合中 → 100 是起点
    100 在? ✓ → len=1
    101 在? ✗ → 结束
    这条链: [100], 长度 1

  num = 4:
    4-1=3 在集合中 → 4 不是起点, 跳过

  num = 200:
    200-1=199 不在集合中 → 200 是起点
    200 在? ✓ → len=1
    201 在? ✗ → 结束
    这条链: [200], 长度 1

  num = 1:
    1-1=0 不在集合中 → 1 是起点
    1 在? ✓ → len=1
    2 在? ✓ → len=2
    3 在? ✓ → len=3
    4 在? ✓ → len=4
    5 在? ✗ → 结束
    这条链: [1,2,3,4], 长度 4 ← 最长！

  num = 3:
    3-1=2 在集合中 → 3 不是起点, 跳过

  num = 2:
    2-1=1 在集合中 → 2 不是起点, 跳过

最终答案: 4

---

示例: nums = [0, 3, 7, 2, 5, 8, 4, 6, 0, 1]

HashSet = {0, 1, 2, 3, 4, 5, 6, 7, 8}  (去重后)

起点分析:
  0: 0-1=-1 不在 → 起点! 向后: 0,1,2,3,4,5,6,7,8 → 长度 9
  1: 1-1=0 在 → 跳过
  2: 2-1=1 在 → 跳过
  ... 其余都跳过

最终答案: 9
```

```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        // 构建 HashSet，自动去重，O(1) 查找
        unordered_set<int> s(nums.begin(), nums.end());
        int longest = 0;

        // 遍历集合（不是原数组！避免重复处理）
        for (int num : s) {
            // 只从起点开始：num-1 不在集合中 → num 是链的第一个
            if (!s.count(num - 1)) {
                int cur = num;
                int len = 1;
                // 从起点向后延伸
                while (s.count(cur + 1)) {
                    cur++;
                    len++;
                }
                longest = max(longest, len);
            }
        }
        return longest;
    }
};
```

**关键点**:
- **只从起点开始的意义**：假设有连续序列 [1,2,3,4]。如果不加判断，对 1,2,3,4 四个数都会触发 while 循环，分别延伸 4,3,2,1 次，总计 10 次查找。加了判断后，只有 1 触发 while（因为 0 不在集合中），延伸 4 次，其余数直接跳过。
- **时间复杂度证明**：外层循环 O(n)。内层 while 循环总共跑多少次？每个元素最多被某个起点的 while 循环访问一次。所有 while 循环的执行次数加起来不超过 n。因此总时间 O(n) + O(n) = O(n)。
- **遍历 set 而非 nums**：避免重复元素导致同一起点被多次处理。

---

### 解法2: 排序 + 扫描 — O(n log n) / O(1)

**不同视角**: 排序后相邻元素如果差 1 就是连续的，扫一遍即可。不满足 O(n) 要求但面试中可以作为初始想法。

```
示例: nums = [100, 4, 200, 1, 3, 2]
排序后: [1, 2, 3, 4, 100, 200]

扫描:
  1→2: 差1, len=2
  2→3: 差1, len=3
  3→4: 差1, len=4
  4→100: 差>1, 重置 len=1
  100→200: 差>1, 重置 len=1

最长: 4
```

```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        
        sort(nums.begin(), nums.end());
        int longest = 1, curLen = 1;
        
        for (int i = 1; i < (int)nums.size(); i++) {
            if (nums[i] == nums[i-1]) {
                continue;  // 跳过重复
            } else if (nums[i] == nums[i-1] + 1) {
                curLen++;
            } else {
                curLen = 1;
            }
            longest = max(longest, curLen);
        }
        return longest;
    }
};
```

**关键点**:
- 必须处理重复元素（跳过 `nums[i] == nums[i-1]` 的情况）。
- 时间 O(n log n) 来自排序，不满足题目 O(n) 的进阶要求。

---

### 解法3: 并查集 (Union Find) — O(n) / O(n)

**进阶视角**: 把数组中的数看作图中的节点。如果 num 和 num+1 都在数组中，就在它们之间连一条边。最终找最大连通分量的大小。

```cpp
class Solution {
public:
    unordered_map<int, int> parent, rank_, size_;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);  // 路径压缩
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        // 按秩合并
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        size_[px] += size_[py];
        if (rank_[px] == rank_[py]) rank_[px]++;
    }
    
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        
        // 初始化：每个数是自己的 parent，size = 1
        for (int num : nums) {
            parent[num] = num;
            rank_[num] = 0;
            size_[num] = 1;
        }
        
        // 合并相邻数
        for (int num : nums) {
            if (parent.count(num + 1)) {
                unite(num, num + 1);
            }
        }
        
        // 找最大集合
        int longest = 0;
        for (auto& [num, p] : parent) {
            if (find(num) == num) {  // 根节点
                longest = max(longest, size_[num]);
            }
        }
        return longest;
    }
};
```

**关键点**:
- 并查集的 find 和 union 操作均摊 O(alpha(n))，近似 O(1)。
- 这种解法在面试中展示数据结构功底，但代码量较大，一般不作为首选。
- 适合当面试官追问"还有其他 O(n) 的方法吗？"时提出。

## 解法对比

|  | HashSet + 起点枚举 | 排序 + 扫描 | 并查集 |
|--|-------------------|------------|--------|
| 时间 | **O(n)** | O(n log n) | **O(n)** |
| 空间 | O(n) | O(1)~O(n) | O(n) |
| 代码量 | 少 | 中 | 多 |
| 实现难度 | 低 | 低 | 高 |
| 面试推荐 | ⭐ 首选 | 初始想法 | 进阶加分 |

**面试策略**: 先提排序思路表示理解题意，然后说"排序是 O(n log n)，为了达到 O(n) 用 HashSet"，写出解法1。

## 易错点

1. **遍历原数组而非 set**：
   - ✗ `for (int num : nums)` → 重复元素导致同一起点多次触发 while
   - ✓ `for (int num : s)` → 集合中每个值只出现一次

2. **忘记"只从起点开始"的判断**：
   - ✗ 对每个数都 while 向后延伸 → 时间退化为 O(n^2)
   - ✓ `if (!s.count(num - 1))` 才进入 while

3. **排序解法忘记跳过重复**：
   - ✗ [1,1,2,3] 中 1→1 差 0，以为断了，重置长度
   - ✓ `if (nums[i] == nums[i-1]) continue;`

4. **空数组崩溃**：
   - ✗ 排序解法直接访问 nums[0] → 空数组越界
   - ✓ 开头加 `if (nums.empty()) return 0;`

## 面试追问

**Q1: 为什么 HashSet 解法是 O(n) 而不是 O(n^2)？**
> 关键在于 `if (!s.count(num-1))` 这个判断。它保证只有链的起点才会进入 while 循环。每个元素最多被某个起点的 while 循环访问一次。外层循环 n 次 + 所有 while 循环总计 n 次 = O(2n) = O(n)。

**Q2: 如果数组很大且有大量重复怎么办？**
> 放入 HashSet 时自动去重。set 的大小是不同元素的个数 k <= n。后续所有操作基于 k 而非 n，效率更高。

**Q3: 并查集解法的优势是什么？**
> 并查集天然适合"合并"和"查询连通性"的场景。虽然本题用 HashSet 更简洁，但如果题目变为在线处理（数字动态加入，每次询问最长连续序列），并查集更适合。

**Q4: 如果要返回最长连续序列本身（不只是长度），怎么改？**
> 在找到最长链时记录起点和长度。或者在 while 循环中记录链的起始值 `start` 和结束值 `cur`，最后用 [start, cur] 构造结果。

## 相关题型

- **128. 最长连续序列** — 本题原题（主站版本）。完全相同的解法。
- **298. 二叉树最长连续序列** — 从数组推广到二叉树，DFS 沿路径找连续递增。
- **549. 二叉树最长连续序列 II** — 允许递增或递减，需要同时维护两个方向。
- **1040. 移动石子直到连续 II** — 利用连续序列的思想，结合滑动窗口。
