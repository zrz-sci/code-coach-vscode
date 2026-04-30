# 303. 区域和检索 - 数组不可变

## 核心思路

经典**前缀和**入门题。每次查询都遍历求和是 O(n)，当查询量大时太慢。通过预处理前缀和数组 `prefix[i]` = 前 `i` 个元素之和，可以将每次区间求和降为 **O(1)**：`sumRange(left, right) = prefix[right+1] - prefix[left]`。

这道题虽然简单，但前缀和是数组类题目中最基础、最常用的技巧之一，几乎所有"区间和"相关的题目都以它为基础。

## 思维链

1. **读完题第一反应** → 每次 `sumRange` 直接 for 循环从 `left` 到 `right` 累加。构造 O(1)，查询 O(n)。如果调用 10^4 次，每次 O(n)，总 O(n * q) = 10^8，可能超时。
2. **优化的关键** → "多次查询、数组不变" — 典型的**预处理**场景。能否 O(1) 回答每次查询？
3. **前缀和的定义** → `prefix[0] = 0`，`prefix[i] = nums[0] + nums[1] + ... + nums[i-1]`。那么 `nums[left] + ... + nums[right] = prefix[right+1] - prefix[left]`。
4. **为什么 prefix 长度是 n+1？** → `prefix[0] = 0` 作为哨兵，处理 `left = 0` 的情况时不需要特殊判断。`sumRange(0, right) = prefix[right+1] - prefix[0] = prefix[right+1]`，自然正确。
5. **预处理时间 O(n)，每次查询 O(1)** → 总时间 O(n + q)，远优于暴力的 O(n * q)。

## 解法概览

| 解法 | 思路 | 构造时间 | 查询时间 | 空间 | 面试 |
|------|------|---------|---------|------|------|
| 暴力求和 | 每次遍历累加 | O(1) | O(n) | O(1) | 能说出即可 |
| 前缀和 | 预处理前缀和数组 | O(n) | O(1) | O(n) | ⭐ 必须写出 |
| 线段树 | 区间查询数据结构 | O(n) | O(log n) | O(n) | 本题杀鸡用牛刀 |

## 关键提示

1. **前缀和数组长度为 n+1**：`prefix[0] = 0` 是哨兵，避免 `left = 0` 时的边界特殊处理。
2. **区间和公式**：`sumRange(left, right) = prefix[right + 1] - prefix[left]`。注意是 `right + 1` 不是 `right`。
3. **前缀和数组的构建**：`prefix[i+1] = prefix[i] + nums[i]`，一次线性扫描即可。
4. **前缀和的本质**：把"区间操作"转化为"两个端点的操作"。这个思想在差分数组、二维前缀和等高级技巧中反复出现。
5. **不可变的重要性**：数组不可变意味着前缀和只需建一次。如果数组会修改（LC307），就需要树状数组或线段树来维护。

## 解法详解

### 解法1: 暴力求和 — 构造 O(1) / 查询 O(n)

**思考过程**: 最直接的做法——存下原数组，每次查询时遍历累加。

```
示例: nums = [-2, 0, 3, -5, 2, -1]

sumRange(0, 2): -2 + 0 + 3 = 1  (遍历3个元素)
sumRange(2, 5): 3 + (-5) + 2 + (-1) = -1  (遍历4个元素)
sumRange(0, 5): -2 + 0 + 3 + (-5) + 2 + (-1) = -3  (遍历6个元素)
```

```cpp
class NumArray {
public:
    vector<int> data;
    
    NumArray(vector<int>& nums) : data(nums) {}
    
    int sumRange(int left, int right) {
        int sum = 0;
        for (int i = left; i <= right; i++) {
            sum += data[i];
        }
        return sum;
    }
};
```

**关键点**:
- 构造 O(1)（只是保存引用/拷贝数组），但每次查询 O(n)。
- 如果有 q 次查询，总复杂度 O(n * q)。n = q = 10^4 时为 10^8，可能超时。
- 这个解法的价值：**明确了瓶颈是"每次查询都要重新遍历"**。

---

### 解法2: 前缀和 — 构造 O(n) / 查询 O(1) ⭐ 面试首选

**从解法1优化**: 多次查询同一个不变数组，每次都重新计算太浪费。预处理前缀和，之后每次查询 O(1)。

```
前缀和构建:
nums =   [-2, 0, 3, -5, 2, -1]
prefix = [0, -2, -2, 1, -4, -2, -3]
           ↑   ↑   ↑  ↑   ↑   ↑   ↑
          空  n[0] n[0..1] n[0..2] ...

区间和公式: sumRange(l, r) = prefix[r+1] - prefix[l]

sumRange(0, 2) = prefix[3] - prefix[0] = 1 - 0 = 1 ✓
sumRange(2, 5) = prefix[6] - prefix[2] = -3 - (-2) = -1 ✓
sumRange(0, 5) = prefix[6] - prefix[0] = -3 - 0 = -3 ✓

为什么 prefix[0] = 0？
→ 当 left = 0 时, sumRange(0, r) = prefix[r+1] - prefix[0] = prefix[r+1]
  不需要特殊处理 "left 从第一个元素开始" 的情况
```

```cpp
class NumArray {
public:
    vector<int> prefix;
    
    NumArray(vector<int>& nums) {
        int n = nums.size();
        prefix.resize(n + 1, 0);
        // prefix[0] = 0 (哨兵)
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }
    
    int sumRange(int left, int right) {
        return prefix[right + 1] - prefix[left];
    }
};
```

**关键点**:
- `prefix[i]` 表示 `nums[0..i-1]` 的和，即前 `i` 个元素之和。
- 区间 `[left, right]` 的和 = "前 `right+1` 个元素之和" - "前 `left` 个元素之和"。
- `prefix[0] = 0` 是关键的哨兵设计，使公式在 `left = 0` 时也成立。

---

### 解法3: 线段树 — 构造 O(n) / 查询 O(log n)

**不同角度**: 线段树是处理区间查询的通用数据结构，同时支持单点/区间修改。本题数组不可变，线段树大材小用，但了解它有助于理解 LC307（数组可修改版本）。

```cpp
class NumArray {
public:
    vector<int> seg;
    int n;
    
    NumArray(vector<int>& nums) {
        n = nums.size();
        seg.resize(4 * n, 0);
        build(nums, 1, 0, n - 1);
    }
    
    int sumRange(int left, int right) {
        return query(1, 0, n - 1, left, right);
    }
    
private:
    void build(vector<int>& nums, int node, int lo, int hi) {
        if (lo == hi) { seg[node] = nums[lo]; return; }
        int mid = (lo + hi) / 2;
        build(nums, 2 * node, lo, mid);
        build(nums, 2 * node + 1, mid + 1, hi);
        seg[node] = seg[2 * node] + seg[2 * node + 1];
    }
    
    int query(int node, int lo, int hi, int l, int r) {
        if (l <= lo && hi <= r) return seg[node];
        if (lo > r || hi < l) return 0;
        int mid = (lo + hi) / 2;
        return query(2 * node, lo, mid, l, r) +
               query(2 * node + 1, mid + 1, hi, l, r);
    }
};
```

**关键点**:
- 本题用线段树查询是 O(log n)，反而比前缀和的 O(1) 慢。
- 但如果题目允许修改数组（LC307），前缀和修改需要 O(n) 重建，而线段树修改只需 O(log n)。
- 面试中本题只需写前缀和。提到"如果数组可变就需要线段树"是加分项。

## 解法对比

|  | 暴力 | 前缀和 | 线段树 |
|--|------|--------|--------|
| 构造 | O(1) | **O(n)** | O(n) |
| 查询 | O(n) | **O(1)** | O(log n) |
| 修改 | O(1) | O(n) 需重建 | **O(log n)** |
| 空间 | O(n) | O(n) | O(4n) |
| 适用场景 | 极少次查询 | **不可变数组多次查询** | 可变数组多次查询 |

**面试选择**: 本题直接写前缀和，复杂度最优且代码最短。

## 易错点

1. **前缀和数组大小错误**：
   - ✗ `prefix.resize(n)` → `sumRange(0, n-1)` 时 `prefix[n]` 越界
   - ✓ `prefix.resize(n + 1)` → 多一个位置给哨兵

2. **区间和公式下标错误**：
   - ✗ `prefix[right] - prefix[left]` → 漏了 `nums[right]`
   - ✓ `prefix[right + 1] - prefix[left]`

3. **忘记 `prefix[0] = 0`**：
   - ✗ `prefix[0] = nums[0]` → 公式 `prefix[right+1] - prefix[left]` 在 `left=0` 时多加了 `nums[0]`
   - ✓ `prefix[0] = 0`，然后 `prefix[i+1] = prefix[i] + nums[i]`

4. **构造函数中参数类型**：
   - ✗ `NumArray(vector<int> nums)` → 拷贝整个数组
   - ✓ `NumArray(vector<int>& nums)` → 引用传递，避免不必要的拷贝

## 面试追问

**Q1: 如果数组会被修改（某个位置的值会变化），前缀和还能用吗？**
> 修改一个位置后，所有后续的前缀和都要更新，需要 O(n) 时间。如果修改频繁，应该用树状数组（BIT）或线段树，修改和查询都是 O(log n)。这就是 LC307 的场景。

**Q2: 如何扩展到二维？（LC304）**
> 二维前缀和：`prefix[i][j]` = 矩阵左上角 `(0,0)` 到 `(i-1,j-1)` 的元素总和。区间和用容斥原理：`sum(r1,c1,r2,c2) = prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1]`。

**Q3: 前缀和还有哪些经典应用？**
> (1) LC560 和为K的子数组：前缀和 + 哈希表查找 `prefix_sum - k`。(2) LC238 除自身以外的积：前缀积 + 后缀积。(3) 差分数组：前缀和的逆操作，用于"区间加"场景（如 LC370）。

## 相关题型

- **[304. 二维区域和检索 - 矩阵不可变](../0304_range_sum_query_2d_immutable/)** — 前缀和从一维扩展到二维，用容斥原理计算矩形区域和。构建 O(mn)，查询 O(1)。
- **[307. 区域和检索 - 数组可修改](../0307_range_sum_query_mutable/)** — 数组可变版本。前缀和退化为 O(n) 修改，需要升级到树状数组/线段树。直接复用本题的线段树解法3。
- **[560. 和为 K 的子数组](../0560_subarray_sum_equals_k/)** — 前缀和 + 哈希表。把"子数组和 = k"转化为"两个前缀和之差 = k"，然后用 Two Sum 的哈希表思路查找。
- **[53. 最大子数组和](../0053_maximum_subarray/)** — 前缀和视角：`max(prefix[j] - prefix[i])`，等价于求前缀和数组中"后面的最大值减前面的最小值"。但 Kadane 算法更优雅。
