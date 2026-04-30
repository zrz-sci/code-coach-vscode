# 3721. 最长平衡子数组 II (Longest Balanced Subarray II)

## 核心思路

子数组"平衡"的定义是：其中**不同偶数**的数量等于**不同奇数**的数量。注意关键词是 **distinct**（去重后的）——`[2,2,3]` 中 distinct even = 1 (`{2}`)，distinct odd = 1 (`{3}`)，是平衡的。

本题 Hard 难度，核心难点：**distinct count 不具备可减性**。普通计数（如"偶数的总出现次数"）可以用前缀和做差得到区间值，但 distinct count 不行。这意味着不能套用经典的"前缀差 + 哈希表"模式。

- **O(n^2) 枚举解法**：固定左端点，右端点扩展，用 `unordered_set` 增量维护 distinct count。面试中作为基础解。
- **O(n log n) 线段树解法**：利用 "last occurrence" 确定哪些左端点会受到新元素影响，用线段树做区间更新和查询 diff=0 的最小索引。竞赛级最优解。

## 思维链

1. **读完题第一反应** → 找最长子数组，使 distinct even count == distinct odd count。暴力枚举所有子数组 O(n^2)，每个子数组统计 distinct 需要 O(n)，总 O(n^3)。但可以用增量维护优化到 O(n^2)。

2. **能否用经典前缀差？** → 如果题目问的是"偶数**总个数** = 奇数**总个数**"（不要求 distinct），那可以直接用前缀差——遇偶 +1 遇奇 -1，找相同差值的最远距离。但本题要求 **distinct count**，不能做前缀差：
```
普通计数: count(even in [l,r]) = prefix_even[r] - prefix_even[l-1]  ✓ 可减
distinct:  distinct_even([l,r]) != prefix_distinct[r] - prefix_distinct[l-1]  ✗ 不可减

反例: nums = [2, 3, 2]
  prefix_distinct_even = [0, 1, 1, 1]  （第二个2不增加distinct）
  [0,2] 区间的 distinct even = 1, prefix[3] - prefix[0] = 1 ✓ 恰好对
  [1,2] 区间的 distinct even = 1, 但 prefix[3] - prefix[1] = 0 ✗ 错了！
  因为 [0,0] 出现的 2 被减掉了，但 [1,2] 也包含 2
```

3. **O(n^2) 枚举法——增量维护 distinct count** →
   - 固定左端点 `l`，右端点 `r` 从 `l` 向右扩展
   - 用两个 `unordered_set<int>` 分别追踪窗口中的偶数集合和奇数集合
   - 每加入 `nums[r]`，insert 到对应 set。set 自动去重，`size()` 就是 distinct count
   - 比较两个 set 的 size，相等则更新答案

4. **为什么不能用滑动窗口？** → 标准滑窗要求收缩左端点时条件单调变化。但收缩时移除 `nums[l]`，如果 `nums[l]` 在窗口内还有其他副本，distinct count 不变。不满足单调性。

5. **O(n log n) 线段树解法的核心 insight** →
```
维护数组 diff[0..n-1]，其中 diff[i] 表示子数组 [i, 当前j] 的
distinct_even - distinct_odd

当右端点 j 扩展到新位置:
  v = nums[j], prev = last[v]（v 上次出现位置，-1 表示没出现过）
  
  if v 是偶数:
    对 diff[prev+1 .. j] 都 += 1
    含义: 左端点在 [prev+1, j] 范围的子数组，v 是首次出现的新 distinct 偶数
    左端点在 [0, prev] 的子数组，[i,j-1] 已包含 v，不增加 distinct
  
  if v 是奇数:
    对 diff[prev+1 .. j] 都 -= 1
  
  更新 last[v] = j
  
  查询: diff[i] == 0 的最小 i → ans = max(ans, j - i + 1)
```
用线段树支持：区间加减（lazy propagation）、查询区间最小值及最左索引。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| O(n^2) 枚举 + set | 固定左端点，set 维护 distinct | O(n^2) | O(n) | ⭐ 基础可写 |
| O(n^2) 枚举 + 频次 map | 固定左端点，map 维护频次 | O(n^2) | O(n) | 等价变体 |
| O(n log n) 线段树 | 区间更新 diff，查询 diff=0 最小索引 | O(n log n) | O(n) | 竞赛级最优 |

## 关键提示

1. **区分"总出现次数"和"distinct 个数"**：`[2,2,3]` 偶数出现 2 次但 distinct 偶数只有 1 个。`set.insert(v)` 天然处理去重——重复 insert 不改变 `size()`。

2. **O(n^2) 解法中用 `unordered_set` 自动处理 distinct**。`set.insert(v)` 不管值是否已存在都安全，`set.size()` 直接反映 distinct count。

3. **线段树解法的关键**：每个值只在"首次出现的位置"贡献 distinct count。用 `last[v]` 记录上次出现位置，确定哪些左端点会受到新元素的影响。

4. **线段树查询技巧**：维护区间最小值 `minVal`。diff == 0 的最小索引等价于"最小值为 0 时最小值所在的最左位置"。如果 `minVal > 0`，说明没有 diff = 0 的左端点。

5. **数据范围**: n <= 10^5，O(n^2) = 10^10 理论上超时。但面试中说明 O(n^2) 思路后指出线段树优化方向，是完全合格的回答。

## 示例演算

```
示例 1: nums = [2, 5, 4, 3]

l=0:
  r=0: 2(偶) → evens={2}, odds={}        → 1 != 0
  r=1: 5(奇) → evens={2}, odds={5}       → 1 == 1 ✓ → ans=2
  r=2: 4(偶) → evens={2,4}, odds={5}     → 2 != 1
  r=3: 3(奇) → evens={2,4}, odds={5,3}   → 2 == 2 ✓ → ans=4

l=1:
  r=1: 5(奇) → evens={}, odds={5}        → 0 != 1
  r=2: 4(偶) → evens={4}, odds={5}       → 1 == 1 ✓ → ans=max(4,2)=4
  r=3: 3(奇) → evens={4}, odds={5,3}     → 1 != 2

答案: 4   ← 整个数组 [2,5,4,3] 有 2 个 distinct 偶数和 2 个 distinct 奇数
```

```
示例 2: nums = [3, 2, 2, 5, 4]

l=0:
  r=0: 3(奇)        → e={}, o={3}         → 0 != 1
  r=1: 2(偶)        → e={2}, o={3}        → 1 == 1 ✓ → ans=2
  r=2: 2(偶,重复!)  → e={2}, o={3}        → 1 == 1 ✓ → ans=3
       ↑ set.insert(2) 不改变 size，所以 distinct_even 还是 1
  r=3: 5(奇)        → e={2}, o={3,5}      → 1 != 2
  r=4: 4(偶)        → e={2,4}, o={3,5}    → 2 == 2 ✓ → ans=5

答案: 5   ← 整个数组虽然有重复的 2，但 distinct even 只有 {2,4}
```

```
示例 3: nums = [1, 2, 3, 2]

l=0:
  r=0: 1(奇) → e={}, o={1}          → 0 != 1
  r=1: 2(偶) → e={2}, o={1}         → 1 == 1 ✓ → ans=2
  r=2: 3(奇) → e={2}, o={1,3}       → 1 != 2
  r=3: 2(偶,重复) → e={2}, o={1,3}  → 1 != 2

l=1:
  r=1: 2(偶) → e={2}, o={}          → 1 != 0
  r=2: 3(奇) → e={2}, o={3}         → 1 == 1 ✓ → ans=2
  r=3: 2(偶,重复) → e={2}, o={3}    → 1 == 1 ✓ → ans=3

答案: 3   ← 子数组 [2,3,2] 中 distinct even={2}, distinct odd={3}
```

## 解法详解

### 解法1: O(n^2) 枚举 + unordered_set ⭐

**思考过程**: distinct count 不能前缀差，那就枚举所有子数组。固定左端点，右端点逐步扩展，用 set 增量维护 distinct count。每步 insert O(1) 均摊。

```cpp
class Solution {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;
        
        for (int l = 0; l < n; l++) {
            unordered_set<int> evens;  // 窗口内的 distinct 偶数
            unordered_set<int> odds;   // 窗口内的 distinct 奇数
            
            for (int r = l; r < n; r++) {
                if (nums[r] % 2 == 0) {
                    evens.insert(nums[r]);  // 重复值不影响 size
                } else {
                    odds.insert(nums[r]);
                }
                
                if (evens.size() == odds.size()) {
                    ans = max(ans, r - l + 1);
                }
            }
        }
        
        return ans;
    }
};
```

**关键点**:
- `unordered_set::insert` 自动去重，`size()` 就是 distinct count
- 每个左端点重新创建两个空 set，从零开始构建
- 时间 O(n^2)，空间 O(n)

---

### 解法1b: O(n^2) 用频次 map

如果后续需要收缩窗口（虽然本题不需要），频次 map 更灵活。

```cpp
class Solution {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;
        
        for (int l = 0; l < n; l++) {
            unordered_map<int, int> even_freq, odd_freq;
            int dist_even = 0, dist_odd = 0;
            
            for (int r = l; r < n; r++) {
                if (nums[r] % 2 == 0) {
                    if (even_freq[nums[r]]++ == 0) dist_even++;
                } else {
                    if (odd_freq[nums[r]]++ == 0) dist_odd++;
                }
                
                if (dist_even == dist_odd) {
                    ans = max(ans, r - l + 1);
                }
            }
        }
        
        return ans;
    }
};
```

---

### 解法2: O(n log n) 线段树 — 竞赛级最优

**核心 idea**:
- 枚举右端点 `j`，维护数组 `diff[0..j]`：`diff[i]` = 子数组 `[i, j]` 的 distinct_even - distinct_odd
- 每次 `j` 扩展，只需更新受影响的左端点范围：
  - `v = nums[j]`, `prev = last[v]`（-1 if 首次出现）
  - 偶数：`diff[prev+1..j] += 1`
  - 奇数：`diff[prev+1..j] -= 1`
- 线段树支持：区间加减（lazy propagation）+ 查询最小值及最左索引

```
线段树演算: nums = [2, 5, 4, 3]

初始: diff = [0, 0, 0, 0]

j=0: v=2(偶), prev=-1 → diff[0..0] += 1 → diff = [1, 0, 0, 0]
     min(diff[0..0]) = 1 ≠ 0

j=1: v=5(奇), prev=-1 → diff[0..1] -= 1 → diff = [0, -1, 0, 0]
     min(diff[0..1]) = -1, 但 diff[0]=0 → minIdx=0 → ans = 1-0+1 = 2

j=2: v=4(偶), prev=-1 → diff[0..2] += 1 → diff = [1, 0, 1, 0]
     min = 0 at idx 1 → ans = max(2, 2-1+1) = 2

j=3: v=3(奇), prev=-1 → diff[0..3] -= 1 → diff = [0, -1, 0, -1]
     min = -1, 但有 diff[0]=0 → minIdx=0 → ans = max(2, 3-0+1) = 4

答案: 4 ✓
```

```cpp
class Solution {
    struct Node {
        int minVal, minIdx, lazy;
    };
    
    vector<Node> tree;
    
    void build(int node, int lo, int hi) {
        tree[node].lazy = 0;
        if (lo == hi) {
            tree[node] = {0, lo, 0};
            return;
        }
        int mid = (lo + hi) / 2;
        build(2*node, lo, mid);
        build(2*node+1, mid+1, hi);
        pushUp(node);
    }
    
    void pushUp(int node) {
        auto& l = tree[2*node];
        auto& r = tree[2*node+1];
        if (l.minVal <= r.minVal) {
            tree[node].minVal = l.minVal;
            tree[node].minIdx = l.minIdx;
        } else {
            tree[node].minVal = r.minVal;
            tree[node].minIdx = r.minIdx;
        }
    }
    
    void pushDown(int node) {
        if (tree[node].lazy) {
            for (int c : {2*node, 2*node+1}) {
                tree[c].minVal += tree[node].lazy;
                tree[c].lazy += tree[node].lazy;
            }
            tree[node].lazy = 0;
        }
    }
    
    void rangeAdd(int node, int lo, int hi, int ql, int qr, int val) {
        if (ql > hi || qr < lo) return;
        if (ql <= lo && hi <= qr) {
            tree[node].minVal += val;
            tree[node].lazy += val;
            return;
        }
        pushDown(node);
        int mid = (lo + hi) / 2;
        rangeAdd(2*node, lo, mid, ql, qr, val);
        rangeAdd(2*node+1, mid+1, hi, ql, qr, val);
        pushUp(node);
    }
    
    pair<int,int> queryMin(int node, int lo, int hi, int ql, int qr) {
        if (ql > hi || qr < lo) return {INT_MAX, -1};
        if (ql <= lo && hi <= qr) return {tree[node].minVal, tree[node].minIdx};
        pushDown(node);
        int mid = (lo + hi) / 2;
        auto left = queryMin(2*node, lo, mid, ql, qr);
        auto right = queryMin(2*node+1, mid+1, hi, ql, qr);
        return left.first <= right.first ? left : right;
    }
    
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        tree.resize(4 * n);
        build(1, 0, n - 1);
        
        unordered_map<int, int> last;
        int ans = 0;
        
        for (int j = 0; j < n; j++) {
            int v = nums[j];
            int prev = last.count(v) ? last[v] : -1;
            int delta = (v % 2 == 0) ? 1 : -1;
            
            rangeAdd(1, 0, n-1, prev+1, j, delta);
            last[v] = j;
            
            auto [minVal, minIdx] = queryMin(1, 0, n-1, 0, j);
            if (minVal == 0) {
                ans = max(ans, j - minIdx + 1);
            }
        }
        
        return ans;
    }
};
```

## 解法对比

| | O(n^2) 枚举 | O(n log n) 线段树 |
|---|---|---|
| 时间 | O(n^2) | **O(n log n)** |
| 空间 | O(n) | O(n) |
| 实现难度 | 简单 | 高（线段树 + lazy） |
| 面试推荐 | ⭐ 先写这个 | 说明思路即可 |
| 适用数据量 | n <= ~3000 | n <= 10^5 |

## 易错点

1. **混淆"总出现次数"和"distinct 个数"**：
   - ✗ `[2,2,3]` 中偶数出现 2 次，奇数出现 1 次 → 不平衡？
   - ✓ `[2,2,3]` 中 distinct 偶数 1 个，distinct 奇数 1 个 → 平衡！

2. **用 count 替代 set 但忘记处理去重**：
   - ✗ 每遇到偶数就 `even_count++` → 重复值会被多次计数
   - ✓ 用 set 或 `if (freq[v]++ == 0) distinct++` 正确维护

3. **前缀差不可用**：
   - ✗ 套用 LC 525 的前缀差方法 → distinct count 不可减，结果错误
   - ✓ 识别出本题不满足前缀差的前提条件

4. **线段树查询 diff=0 的最小索引而非最大索引**：
   - 我们要最长子数组，所以需要最**小**的左端点 i（使 j-i+1 最大）
   - 线段树维护 minVal 并在 minVal==0 时返回 minIdx（最左的 0）

## 面试追问

**Q1: "O(n^2) 太慢怎么办？"**
> 说明线段树思路：每个值只在"首次出现位置"贡献 distinct，用 `last[v]` 确定影响范围，线段树做区间更新 + 全局查询 diff=0 的最小索引。时间 O(n log n)。

**Q2: "和 LC 525 Contiguous Array 有什么区别？"**
> LC 525 是简单计数（0和1的个数相等），count 可减，直接用前缀差 + 哈希表 O(n)。本题是 distinct count，不可减，不能直接前缀差，需要线段树。

**Q3: "滑动窗口可以吗？"**
> 不行。标准滑窗要求收缩左端点时目标函数单调变化。但收缩时移除 `nums[l]`，如果窗口内还有 `nums[l]` 的副本，distinct count 不变。不满足单调性要求。

**Q4: "时间能到 O(n) 吗？"**
> 目前主流解法是 O(n log n)（线段树）。是否存在 O(n) 解法取决于是否能找到避免区间更新的方式，目前没有已知的 O(n) 解。

## 复杂度分析

- **O(n^2) 解法**: 时间 O(n^2)，空间 O(n)（set 最多存 n 个不同值）
- **O(n log n) 线段树**: 时间 O(n log n)（每个元素做一次区间更新 O(log n)），空间 O(n)（线段树大小 4n）

## 相关题目

| 题目 | 关联 |
|------|------|
| [525. 连续数组 (Contiguous Array)](https://leetcode.cn/problems/contiguous-array/) | 前缀差+哈希表找最长子数组（简单计数，不要求 distinct） |
| [992. K 个不同整数的子数组](https://leetcode.cn/problems/subarrays-with-k-different-integers/) | 滑动窗口 + distinct count |
| [2461. 长度为 K 子数组中的最大和](https://leetcode.cn/problems/maximum-sum-of-distinct-subarrays-with-length-k/) | 固定窗口 + distinct 约束 |
| [930. 和相同的二元子数组](https://leetcode.cn/problems/binary-subarrays-with-sum/) | 前缀和统计子数组 |
| [3097. 或值至少为 K 的最短子数组 II](https://leetcode.cn/problems/shortest-subarray-with-or-at-least-k-ii/) | 位运算子数组问题 |
