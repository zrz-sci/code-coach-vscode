# 891. 子序列宽度之和

## 核心思路

本质上是一个**排序 + 贡献度计算问题**：子序列的"宽度"只取决于最大值和最小值，与元素顺序无关。因此可以先排序，然后计算每个元素作为最大值和最小值分别对总宽度的贡献。排序后，对于 `nums[i]`：它作为最大值的子序列数量是 `2^i`（从前 i 个元素中任选若干）；作为最小值的子序列数量是 `2^(n-1-i)`（从后面的元素中任选若干）。总贡献 = `nums[i] * (2^i - 2^(n-1-i))`。

## 思维链

1. **读完题第一反应**：暴力枚举所有子序列太慢（2^n 个子序列）。但子序列的宽度 = max - min，只和最大最小值有关，和中间元素无关。

2. **关键洞察——排序不影响结果**：子序列的宽度只取决于最大值和最小值，排序不改变任何子序列的宽度。排序后问题变得规整。

3. **贡献度思维**：不按子序列枚举，而是按元素枚举——计算每个元素 `nums[i]` 在所有子序列中作为最大值的次数和作为最小值的次数。

4. **排序后计算贡献**：排序后 `nums[0] <= nums[1] <= ... <= nums[n-1]`。
   - `nums[i]` 作为**最大值**：子序列的所有元素都 <= `nums[i]`，即从 `nums[0..i]` 中选（必须包含 `nums[i]`，其余 i 个元素任选）→ `2^i` 个子序列。
   - `nums[i]` 作为**最小值**：子序列的所有元素都 >= `nums[i]`，即从 `nums[i..n-1]` 中选（必须包含 `nums[i]`，其余 n-1-i 个元素任选）→ `2^(n-1-i)` 个子序列。

5. **公式**：
   ```
   答案 = sum_{i=0}^{n-1} nums[i] * (2^i - 2^(n-1-i))  mod (10^9 + 7)
   ```

6. **取模注意**：中间结果可能溢出，需要每步取模。减法可能产生负数，需要加 MOD 再取模。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子序列 | O(2^n * n) | O(n) | TLE |
| 排序 + 贡献度 | 排序后按元素计算贡献 | O(n log n) | O(n) | ⭐ 面试首选 |
| 排序 + 双指针优化 | 从两端配对计算 | O(n log n) | O(1) | 加分项 |

> n = nums.length

## 关键提示

1. **子序列的宽度只看 max 和 min**：`width = max(subsequence) - min(subsequence)`。中间的元素完全不影响宽度。这意味着排序后处理会大大简化问题。

2. **2^i 的含义**：排序后 `nums[i]` 作为最大值时，子序列从 `nums[0..i-1]` 中任选若干元素加上 `nums[i]` 本身。`nums[0..i-1]` 有 i 个元素，每个可选可不选，共 `2^i` 种组合。

3. **预处理 2 的幂**：避免重复计算，预先算好 `pow2[0], pow2[1], ..., pow2[n-1]`，其中 `pow2[k] = 2^k mod (10^9+7)`。

4. **取模陷阱**：`nums[i] * (2^i - 2^(n-1-i))` 中的减法可能产生负数。解决方案：`((2^i - 2^(n-1-i)) % MOD + MOD) % MOD` 或者分开计算正贡献和负贡献。

5. **对称性优化**：注意 `nums[i]` 的贡献系数是 `2^i - 2^(n-1-i)`，当 `i` 和 `n-1-i` 互补时可以配对计算：`(nums[i] - nums[n-1-i]) * 2^i`。

## 解法详解

### 解法1: 暴力枚举 — O(2^n * n) / O(n) [TLE]

**思考过程**：枚举所有非空子序列，对每个子序列找 max 和 min，累加宽度。

```cpp
class Solution {
public:
    int sumSubseqWidths(vector<int>& nums) {
        int n = nums.size();
        long long MOD = 1e9 + 7;
        long long ans = 0;
        
        // 枚举所有子序列 (用 bitmask)
        for (int mask = 1; mask < (1 << n); mask++) {
            int maxVal = INT_MIN, minVal = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    maxVal = max(maxVal, nums[i]);
                    minVal = min(minVal, nums[i]);
                }
            }
            ans = (ans + maxVal - minVal) % MOD;
        }
        return (int)ans;
    }
};
```

**关键点**：n 最大 10^5，2^n 完全不可行。但这个思路帮助理解问题本质——宽度只看 max 和 min。

### 解法2: 排序 + 贡献度 — O(n log n) / O(n) ⭐ 面试首选

**从暴力到贡献度**：不按子序列枚举，而是按元素计算贡献。排序后 `nums[i]` 作为最大值出现 `2^i` 次，作为最小值出现 `2^(n-1-i)` 次。

**推导过程**：
```
答案 = Σ (max(subseq) - min(subseq))
     = Σ max(subseq) - Σ min(subseq)

排序后，nums[i] 作为最大值出现在 2^i 个子序列中
（从 nums[0..i-1] 中任选 + 必须包含 nums[i]）

nums[i] 作为最小值出现在 2^(n-1-i) 个子序列中
（从 nums[i+1..n-1] 中任选 + 必须包含 nums[i]）

答案 = Σ nums[i] * 2^i - Σ nums[i] * 2^(n-1-i)
     = Σ nums[i] * (2^i - 2^(n-1-i))
```

```cpp
class Solution {
public:
    int sumSubseqWidths(vector<int>& nums) {
        long long MOD = 1e9 + 7;
        int n = nums.size();
        
        sort(nums.begin(), nums.end());
        
        // 预处理 2 的幂
        vector<long long> pow2(n);
        pow2[0] = 1;
        for (int i = 1; i < n; i++) {
            pow2[i] = pow2[i - 1] * 2 % MOD;
        }
        
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            // nums[i] 作为最大值贡献 + nums[i] * 2^i
            // nums[i] 作为最小值贡献 - nums[i] * 2^(n-1-i)
            long long contribution = (long long)nums[i] % MOD * ((pow2[i] - pow2[n - 1 - i] + MOD) % MOD) % MOD;
            ans = (ans + contribution) % MOD;
        }
        
        return (int)ans;
    }
};
```

**关键点**：`(pow2[i] - pow2[n-1-i] + MOD) % MOD` 处理了减法可能产生的负数。

### 解法3: 排序 + 配对优化 — O(n log n) / O(n)

**从解法2优化**：利用对称性，将 `i` 和 `n-1-i` 配对。`nums[i]` 的系数是 `2^i - 2^(n-1-i)`，`nums[n-1-i]` 的系数是 `2^(n-1-i) - 2^i`，两者符号相反。因此可以配对计算 `(nums[i] - nums[n-1-i]) * 2^i`。

```cpp
class Solution {
public:
    int sumSubseqWidths(vector<int>& nums) {
        long long MOD = 1e9 + 7;
        int n = nums.size();
        
        sort(nums.begin(), nums.end());
        
        // 预处理 2 的幂
        vector<long long> pow2(n);
        pow2[0] = 1;
        for (int i = 1; i < n; i++) {
            pow2[i] = pow2[i - 1] * 2 % MOD;
        }
        
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            // 配对: nums[i] 贡献 +2^i，nums[n-1-i] 贡献 -2^i
            // 等价于 (nums[i] - nums[n-1-i]) * 2^i
            long long diff = ((long long)nums[i] - nums[n - 1 - i]) % MOD;
            ans = (ans + diff % MOD * pow2[i] % MOD + MOD) % MOD;
        }
        
        return (int)ans;
    }
};
```

**关键点**：配对写法更对称，也更容易理解——每一对 `(nums[i], nums[n-1-i])` 对应的贡献是 `(nums[i] - nums[n-1-i]) * 2^i`。

## 解法对比

| | 暴力枚举 | 排序+贡献度 | 排序+配对优化 |
|--|---------|-----------|-------------|
| 时间 | O(2^n * n) | O(n log n) | O(n log n) |
| 空间 | O(1) | O(n) | O(n) |
| 思路难度 | 低 | 高 | 高 |
| 代码难度 | 低 | 中 | 中 |
| 面试建议 | 说思路 | ⭐首选 | 可选优化 |

**选择建议**：
- 面试先说暴力，引出"子序列宽度只看 max 和 min → 排序 → 贡献度"的思考链
- 写出排序+贡献度解法（解法2），公式推导要清晰
- 配对优化是锦上添花，但不是必须的

## 易错点

1. **忘记排序**
   - 错：直接在未排序数组上用 `2^i` 计算 → 结果完全错误
   - 对：必须先排序，才能说"nums[i] 作为最大值的子序列数 = 2^i"

2. **取模时的负数问题**
   - 错：`(pow2[i] - pow2[n-1-i]) % MOD` 可能是负数 → C++ 中负数取模结果仍是负数
   - 对：`(pow2[i] - pow2[n-1-i] + MOD) % MOD` 先加 MOD 保证非负

3. **乘法溢出**
   - 错：`nums[i] * pow2[i]` 用 int 计算 → 溢出
   - 对：转换为 long long 后相乘，每步取模

4. **2 的幂预处理范围**
   - 错：`pow2` 数组开太小 → 越界
   - 对：至少开 n 个（索引 0 到 n-1）

5. **混淆"子序列"和"子数组"**
   - 子序列可以不连续，子数组必须连续
   - 本题是子序列，所以排序后宽度不变（子序列的 max/min 不受排序影响）

## 面试追问

**Q1: 为什么排序不影响结果？**
→ 子序列的宽度 = max - min，只取决于值而非位置。排序改变了元素的相对位置，但不改变任何子序列（作为集合）的最大值和最小值。更本质地说，subsequence 在这里等价于 subset（子集），因为宽度只看值域。

**Q2: 2^i 为什么是 nums[i] 作为最大值的子序列数？**
→ 排序后 `nums[0] <= nums[1] <= ... <= nums[i]`。要让 `nums[i]` 成为最大值，子序列只能包含 `nums[0..i]` 中的元素。必须包含 `nums[i]` 自身，其余 i 个元素（`nums[0..i-1]`）每个可选可不选，共 `2^i` 种。

**Q3: 如果有重复元素怎么办？**
→ 排序后相等的元素相邻。对于相等的元素，它们在排序后的位置不同，2^i 的系数也不同。但每个元素都独立计算贡献，重复元素不需要特殊处理——公式自然正确。

**Q4: 能否做到 O(n) 时间？**
→ 排序的 O(n log n) 是瓶颈。如果输入已排序则是 O(n)。但通用情况下需要排序，除非用桶排序（在值域有限时可以 O(值域+n)）。

## 相关题型

- **LeetCode 907 子数组的最小值之和** — 同样是"贡献度"思维，但针对子数组（连续），用单调栈确定每个元素作为最小值的范围。
- **LeetCode 2104 子数组范围和** — 求子数组的 max - min 之和，和本题思路相似但针对连续子数组。
- **LeetCode 1856 最大子序列分数** — 涉及排序后的贡献计算和子序列选择。
- **LeetCode 78 子集** — 理解"每个元素选或不选 → 2^n 个子集"的组合计数，是本题贡献度公式的基础。
