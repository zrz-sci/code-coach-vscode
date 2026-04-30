# 992. K 个不同整数的子数组

## 核心思路

这道题要统计**恰好**有 k 个不同整数的子数组个数。"恰好等于 k"很难直接用滑动窗口处理，但"最多 k 个"可以。核心转化：**atMost(k) - atMost(k-1) = exactly(k)**。

## 思维链

1. **读完题第一反应** → 枚举所有子数组，统计每个子数组中不同整数的个数，等于 k 就计数。暴力 O(n²) 或 O(n³)。

2. **暴力的瓶颈** → O(n²) 个子数组，每个还要统计不同整数个数。即使用哈希表优化到 O(n²)，n=2×10⁴ 时约 4×10⁸，太慢。

3. **想到滑动窗口** → 子数组 + 条件约束 = 经典滑动窗口场景。但标准滑动窗口擅长处理"最多 k 个"或"最少 k 个"，因为窗口的扩大/缩小具有单调性。"恰好 k 个"不具有这种单调性——窗口扩大可能让不同数增多，也可能不变。

4. **关键转化** → `exactly(k) = atMost(k) - atMost(k-1)`。这个等式把"恰好"拆成两个"最多"的差，而"最多 k 个不同整数"完美适配滑动窗口！

5. **atMost(k) 怎么算** → 维护一个窗口 [left, right]，用哈希表记录窗口内每个数的出现次数。当不同数 > k 时，左指针右移直到 ≤ k。此时以 right 为右端点的合法子数组个数 = right - left + 1。

6. **另一种思路（三指针）** → 不拆分，直接维护两个左指针 left1 和 left2，使得 [left1, right] 恰好有 k 种，[left2, right] 恰好有 k-1 种，差值就是答案的贡献。本质相同，但一次遍历完成。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子数组，哈希表计数 | O(n²) | O(n) | 能说出即可 |
| atMost 差值法 | exactly(k) = atMost(k) - atMost(k-1) | O(n) | O(n) | ⭐ 必须写出 |
| 三指针法 | 一次遍历，两个左指针 | O(n) | O(n) | 加分项 |

## 关键提示

1. **为什么"恰好 k 个"不好直接滑窗？** 当窗口内恰好有 k 种不同数时，左指针要缩到哪里？缩一步可能变成 k-1 种，但中间有很多合法位置。无法用一个 left 指针精确控制。

2. **atMost(k) 中每个 right 贡献多少子数组？** 当窗口 [left, right] 内最多有 k 种不同数时，以 right 结尾的合法子数组是 [left, right], [left+1, right], ..., [right, right]，共 right - left + 1 个。

3. **哈希表清零的时机**：当某个数的计数减到 0 时，必须从哈希表中删除（或减少 distinct 计数），否则 distinct 数会虚高。

4. **滑动窗口中各变量的含义对照**：
```
nums: [1, 2, 1, 2, 3],  k = 2

atMost(2) 的窗口演变:
right=0: [1]         left=0, 贡献 1  (1种)
right=1: [1,2]       left=0, 贡献 2  (2种)
right=2: [1,2,1]     left=0, 贡献 3  (2种)
right=3: [1,2,1,2]   left=0, 贡献 4  (2种)
right=4: [1,2,1,2,3] → 3种 > 2, 缩左
         [2,1,2,3]   → 3种 > 2, 缩左
         [1,2,3]     → 3种 > 2, 缩左
         [2,3]       left=3, 贡献 2  (2种)
atMost(2) = 1+2+3+4+2 = 12

atMost(1) = 1+1+1+1+1 = 5

exactly(2) = 12 - 5 = 7 ✓
```

5. **三指针法的直觉**：对于固定的 right，找到两个左边界 left1（窗口内恰好 k 种的最左位置）和 left2（窗口内恰好 k-1 种的最左位置，即再缩一步就不够 k 种了）。[left1, left2) 这些起点都能形成恰好 k 种的子数组。

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(n)

**思考过程**: 最直接的方法。固定左端点，右端点从左往右扩展，用哈希表维护当前窗口内不同整数的个数。

```cpp
class Solution {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        int n = nums.size(), count = 0;
        for (int i = 0; i < n; i++) {
            unordered_map<int, int> freq;
            int distinct = 0;
            for (int j = i; j < n; j++) {
                if (freq[nums[j]]++ == 0) distinct++;
                if (distinct == k) count++;
                if (distinct > k) break; // 剪枝：超过k种，后面只会更多
            }
        }
        return count;
    }
};
```

**关键点**: break 剪枝很重要，但最坏情况仍是 O(n²)，当 k 很大时几乎遍历所有子数组。

---

### 解法2: atMost 差值法 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**: 暴力的瓶颈是每个左端点都要线性扫描。滑动窗口能把这个优化到 O(n)，但直接处理"恰好 k 个"不方便。关键洞察：**exactly(k) = atMost(k) - atMost(k-1)**。

**为什么等式成立？** atMost(k) 统计了所有不同数 ≤ k 的子数组，包含了 exactly(1), exactly(2), ..., exactly(k)。减去 atMost(k-1)（包含 exactly(1) 到 exactly(k-1)），剩下的就是 exactly(k)。

```cpp
class Solution {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        return atMost(nums, k) - atMost(nums, k - 1);
    }
    
private:
    int atMost(vector<int>& nums, int k) {
        int n = nums.size(), left = 0, result = 0;
        unordered_map<int, int> freq;
        int distinct = 0;
        
        for (int right = 0; right < n; right++) {
            // 右指针扩展：新元素入窗
            if (freq[nums[right]]++ == 0) distinct++;
            
            // 不满足条件时，左指针收缩
            while (distinct > k) {
                if (--freq[nums[left]] == 0) distinct--;
                left++;
            }
            
            // 以 right 为右端点，左端点可以是 left, left+1, ..., right
            // 这些子数组内的不同数都 ≤ k
            result += right - left + 1;
        }
        return result;
    }
};
```

**关键点**:
- `result += right - left + 1` 这一步是精髓：不是只加 1，而是加上所有以 right 结尾的合法子数组数量。
- 调用两次 atMost，总复杂度仍是 O(n)，因为每次调用 left 和 right 各最多移动 n 次。

---

### 解法3: 三指针法 — O(n) / O(n)

**思路**: 一次遍历完成，不需要两次调用 atMost。对于每个 right，维护两个左指针：
- `left1`: 窗口 [left1, right] 内恰好有 k 种不同数（left1 尽可能小）
- `left2`: 窗口 [left2, right] 内恰好有 k-1 种不同数（left2 是刚好让不同数从 k 变成 k-1 的位置）

那么 left1 到 left2-1 这些位置作为起点，与 right 构成的子数组恰好有 k 种不同数。

```cpp
class Solution {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        int n = nums.size();
        // freq1 维护 [left1, right] 的计数，freq2 维护 [left2, right] 的计数
        unordered_map<int, int> freq1, freq2;
        int left1 = 0, left2 = 0;
        int distinct1 = 0, distinct2 = 0;
        int result = 0;
        
        for (int right = 0; right < n; right++) {
            // 右指针扩展
            if (freq1[nums[right]]++ == 0) distinct1++;
            if (freq2[nums[right]]++ == 0) distinct2++;
            
            // left1 收缩到窗口内恰好 ≤ k 种
            while (distinct1 > k) {
                if (--freq1[nums[left1]] == 0) distinct1--;
                left1++;
            }
            // left2 收缩到窗口内恰好 ≤ k-1 种
            while (distinct2 > k - 1) {
                if (--freq2[nums[left2]] == 0) distinct2--;
                left2++;
            }
            
            // [left1, right] 最多 k 种，[left2, right] 最多 k-1 种
            // left1 到 left2-1 的起点恰好给出 exactly k 种
            result += left2 - left1;
        }
        return result;
    }
};
```

**直觉理解**:
```
nums: [1, 2, 1, 2, 3],  k = 2,  right = 3 (指向第二个2)

left1 = 0: [1,2,1,2] 有 2 种 ≤ 2 ✓
left2 = 2: [1,2] 有 2 种 > 1, 继续缩...  → left2=2: [1,2] 有 2种 > 1... 
           实际 left2 会缩到使得窗口 ≤ 1 种

让我重新展示:
right=3, nums[right]=2
  freq1 窗口: [1,2,1,2], distinct1=2 ≤ 2, left1=0
  freq2 窗口: 缩到 [2], distinct2=1 ≤ 1, left2=3

  贡献 = left2 - left1 = 3 - 0 = 3
  对应: [1,2,1,2], [2,1,2], [1,2] — 都恰好有 2 种 ✓
```

## 解法对比

| | 暴力枚举 | atMost 差值法 | 三指针法 |
|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) |
| 空间 | O(n) | O(n) | O(n) |
| 代码量 | 少 | 中 | 多 |
| 思维难度 | 低 | 中（需要想到转化） | 高 |
| 面试推荐 | 开场说思路 | ⭐ 首选 | 如果面试官追问一次遍历 |

**atMost 差值法是最经典、最通用的**：这个 `exactly(k) = atMost(k) - atMost(k-1)` 的思想可以复用到很多"恰好"类问题上。三指针法虽然一次遍历，但两个哈希表的空间相同，代码更复杂，面试中不一定有优势。

## 易错点

1. **atMost 中忘记加 right - left + 1，只加了 1**
   - ✗ `result += 1;` → 这只统计了窗口本身，漏掉了窗口内的所有子数组
   - ✓ `result += right - left + 1;` → 以 right 结尾的所有合法子数组

2. **哈希表计数减到 0 时没有更新 distinct**
   - ✗ `freq[nums[left]]--; left++;` → distinct 永远不会减小，窗口一直在缩
   - ✓ `if (--freq[nums[left]] == 0) distinct--; left++;`

3. **atMost(0) 的边界情况**
   - 当 k=1 时，调用 atMost(0)。此时 while 条件 `distinct > 0` 会让 left 始终追着 right，结果为 0。这是正确的，不需要特判。

4. **三指针法中两个哈希表没有独立维护**
   - ✗ 共用一个哈希表 → 两个左指针的状态混在一起
   - ✓ 必须维护两个独立的 freq1 和 freq2

5. **用 `freq[x] == 0` 判断是否是新数时，unordered_map 的默认值行为**
   - `freq[x]++` 会自动插入 key=x, value=0 再 ++。这意味着 `freq[x]++ == 0` 是安全的（先读 0 再 ++）。但如果改成 `++freq[x] == 1` 也可以，要保持一致。

## 面试追问

**Q1: 暴力解能过吗？瓶颈在哪？**
> n ≤ 2×10⁴，O(n²) ≈ 4×10⁸，大概率 TLE。瓶颈在于每个左端点都要线性扫描右端点。

**Q2: 为什么不能直接用一个滑动窗口处理"恰好 k 个"？**
> 标准滑动窗口依赖单调性：窗口扩大 → 不同数只增不减 → 超过阈值就缩。但"恰好 k 个"没有这种单调性：不同数 < k 时要扩，> k 时要缩，= k 时既可以扩也可以缩（因为加一个已有的数不改变计数）。两个方向都可能合法，一个 left 指针搞不定。

**Q3: `atMost(k) - atMost(k-1)` 这个技巧还能用在哪里？**
> 所有"恰好等于 k"的计数问题都能用这个转化：
> - LC 930: 和为 k 的子数组个数（前缀和 + atMost 差值）
> - LC 1248: 优美子数组个数（奇数个数恰好为 k）
> - 通用模式：把 exactly 转成 atMost 差值，用标准滑动窗口解决。

**Q4: 如果数据范围扩大到 10⁶，这个解法还能用吗？**
> 可以。时间 O(n)，空间 O(n)。unordered_map 的常数因子可能较大，可以改用数组 `freq[nums[i]]`（因为 1 ≤ nums[i] ≤ n），常数更小。

## 相关题型

- **LC 340: 至多包含 K 个不同字符的最长子串** — 直接就是 atMost(k) 的模板题，本题的 atMost 函数可以直接复用，只是问的是最大长度而不是计数
- **LC 1248: 统计优美子数组** — 复用 `exactly = atMost(k) - atMost(k-1)` 的转化技巧，把"恰好 k 个奇数"替换为"恰好 k 个不同整数"
- **LC 930: 和相同的二元子数组** — 同样的 atMost 差值转化，窗口维护的是前缀和而不是不同数个数
- **LC 76: 最小覆盖子串** — 同样用哈希表 + 滑动窗口，但问的是包含所有字符的最短窗口，不需要 atMost 转化