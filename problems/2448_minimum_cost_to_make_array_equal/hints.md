# 2448. 使数组相等的最小开销

## 核心思路

本题本质是**带权中位数**问题：把每个 `nums[i]` 看作一个位置，`cost[i]` 看作该位置的权重，所有元素移动到某个目标值的最小总开销，等价于找加权中位数。这和"无权重时所有数移到中位数最优"是同一个模型的推广。

## 思维链

1. **读完题第一反应**：要把所有数变成同一个值，目标值的范围是 `[min(nums), max(nums)]`，最朴素的做法是枚举每个可能的目标值，计算总开销，取最小。
2. **暴力解的瓶颈**：目标值范围可达 `[1, 10^6]`，对每个目标值要遍历数组算开销 O(n)，总共 O(n × V)，V 可达 10^6，极限约 10^11，太慢。
3. **观察代价函数的形状**：固定目标值 t，总开销 `f(t) = Σ cost[i] × |nums[i] - t|`。这是若干个"绝对值函数的加权和"，每一项都是 V 字形。它们的和是一个**凸函数**（下凸），意味着只有一个最低点，没有局部极小值。
4. **凸函数 → 三分搜索**：既然 f(t) 是凸的，可以在 `[min, max]` 上三分搜索最小值点，每次计算 f(mid1) 和 f(mid2)，O(n) 求开销，总共 O(n log V)。
5. **更优雅的方法 — 加权中位数**：类比无权版本"最小绝对偏差在中位数取到"，带权版本的最优点就是**加权中位数**——按 nums 排序后，找到使累计权重首次 ≥ 总权重一半的位置。排序 O(n log n) + 一次遍历 O(n)。
6. **还有一种思路 — 前缀和**：排序后，利用前缀和 O(1) 计算"从目标值 nums[i] 切换到 nums[i+1] 时开销的变化量"，从而 O(n) 扫一遍找最小值。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举每个可能目标值 | O(nV) | O(1) | 能说出即可 |
| 三分搜索 | 利用凸函数性质三分 | O(n log V) | O(1) | 加分项 |
| 排序+加权中位数 | 找加权中位数 | O(n log n) | O(n) | ⭐ 必须写出 |
| 排序+前缀和 | 前缀和算增量 | O(n log n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **凸性观察**：f(t) = Σ cost[i] × |nums[i] - t| 是关于 t 的凸函数。画个图：每一项都是 V 字形，求和还是凸的。凸函数有什么好性质？
2. **中位数联想**：如果没有权重（所有 cost=1），最优目标值是中位数。加了权重之后呢？想想"带权中位数"。
3. **排序后的递推**：假设已经按 nums 排序，已知把所有数移到 nums[0] 的总开销，当目标值从 nums[0] 变成 nums[1] 时，开销怎么变？左边的元素少移一段，右边的元素多移一段。
4. **溢出警惕**：nums 和 cost 都到 10^6，n 到 10^5，最坏总开销约 10^6 × 10^6 × 10^5 = 10^17，必须用 `long long`。
5. **三分搜索的适用条件**：只要证明 f(t) 是凸函数（先减后增/单调），三分就能在 O(log V) 次迭代内找到极值点。

## 解法详解

### 解法1: 暴力枚举 — O(nV) / O(1)

**思考过程**：最直觉的做法——目标值只可能取 `[min(nums), max(nums)]` 中的整数，对每个目标值算一遍总开销。

```cpp
class Solution {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int lo = *min_element(nums.begin(), nums.end());
        int hi = *max_element(nums.begin(), nums.end());
        long long ans = LLONG_MAX;
        for (int t = lo; t <= hi; t++) {
            long long total = 0;
            for (int i = 0; i < nums.size(); i++) {
                total += (long long)cost[i] * abs(nums[i] - t);
            }
            ans = min(ans, total);
        }
        return ans;
    }
};
```

**关键点**：V 可达 10^6，n 可达 10^5，O(nV) ≈ 10^11 会超时。但这个解法帮你确认了思路——我们需要高效地找到使 f(t) 最小的 t。

---

### 解法2: 三分搜索 — O(n log V) / O(1)

**从暴力到三分**：暴力的瓶颈是枚举了所有 V 个目标值。但 f(t) 是凸函数（加权绝对值之和），凸函数的极值可以用三分搜索在 O(log V) 次找到。

```
// f(t) 的形状（凸函数）:
//
// cost
//  |  \         /
//  |   \       /
//  |    \     /
//  |     \   /
//  |      \_/    ← 最低点就是答案
//  +------------- t
//  lo          hi
//
// 三分过程: 每次把区间 [lo, hi] 分成三段
// 比较 f(m1) 和 f(m2)，较大的那端不可能含极小值，砍掉
```

```cpp
class Solution {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        // 计算把所有数移到目标值 t 的总开销
        auto calc = [&](long long t) -> long long {
            long long total = 0;
            for (int i = 0; i < nums.size(); i++) {
                total += (long long)cost[i] * abs(nums[i] - t);
            }
            return total;
        };
        
        long long lo = *min_element(nums.begin(), nums.end());
        long long hi = *max_element(nums.begin(), nums.end());
        
        // 三分搜索：凸函数求极小值
        while (lo < hi) {
            long long m1 = lo + (hi - lo) / 3;
            long long m2 = hi - (hi - lo) / 3;
            if (calc(m1) < calc(m2)) {
                hi = m2 - 1;  // 极小值在左侧
            } else {
                lo = m1 + 1;  // 极小值在右侧
            }
        }
        return calc(lo);
    }
};
```

**关键点**：
- 三分的正确性依赖于 f(t) 的凸性。为什么凸？每个 `cost[i] × |nums[i] - t|` 是凸的，凸函数之和还是凸的。
- 循环结束时 `lo == hi`，即找到了最优的 t。
- 整数三分要注意终止条件，`lo < hi` 最终会收敛。

---

### 解法3: 排序 + 加权中位数 — O(n log n) / O(n) ⭐ 面试首选

**核心数学事实**：对于 f(t) = Σ w_i × |x_i - t|，最小值在**加权中位数**处取到。加权中位数定义：按 x 排序后，累计权重首次 ≥ 总权重一半的那个 x 值。

**为什么？** 想象所有权重摆在数轴上。当 t 从左往右移动 1 单位：
- 左边所有点的距离减少 1，减少的开销 = 左侧总权重
- 右边所有点的距离增加 1，增加的开销 = 右侧总权重
- 当左侧总权重 < 右侧总权重时，t 往右移是划算的
- 当左侧总权重 ≥ 右侧总权重时，就不该再往右了
- 所以最优点就是累计权重恰好过半的位置

```
// 示例: nums=[1,3,5,2], cost=[2,3,1,14]
// 排序后: (1,2), (2,14), (3,3), (5,1)
// 总权重 = 2+14+3+1 = 20, 半值 = 10
//
// 累计权重:
//   nums[0]=1: 累计 2   < 10 → 继续
//   nums[1]=2: 累计 16  ≥ 10 → 加权中位数 = 2 ✓
//
// 验证: 移到2的开销 = 2×|1-2| + 14×|2-2| + 3×|3-2| + 1×|5-2|
//                   = 2 + 0 + 3 + 3 = 8 ✓
```

```cpp
class Solution {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int n = nums.size();
        // 按 nums 值排序，绑定对应的 cost
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums[a] < nums[b];
        });
        
        // 找加权中位数：累计权重首次 ≥ 总权重一半
        long long totalWeight = 0;
        for (int c : cost) totalWeight += c;
        
        long long cumWeight = 0;
        int target = -1;
        for (int i : idx) {
            cumWeight += cost[i];
            if (cumWeight * 2 >= totalWeight) {
                // 加权中位数就是 nums[i]
                target = nums[i];
                break;
            }
        }
        
        // 计算所有元素移到 target 的总开销
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            ans += (long long)cost[i] * abs(nums[i] - target);
        }
        return ans;
    }
};
```

**关键点**：判断条件是 `cumWeight * 2 >= totalWeight`（即 cumWeight ≥ totalWeight / 2），注意不要用浮点除法。

---

### 解法4: 排序 + 前缀和递推 — O(n log n) / O(n)

**另一个角度**：排序后，先算出 f(nums[0])（所有数移到最小值的开销），然后递推 f(nums[i])。

当目标值从 nums[i-1] 变到 nums[i] 时，差值 d = nums[i] - nums[i-1]：
- 左边（≤ nums[i-1]）的元素每个多走 d，增加开销 = d × 左侧总权重
- 右边（≥ nums[i]）的元素每个少走 d，减少开销 = d × 右侧总权重

```
// 递推示意 (排序后):
// 位置:    1    2    3    5
// 权重:    2   14    3    1
//
// f(1) = 2×0 + 14×1 + 3×2 + 1×4 = 24
//
// 从 t=1 → t=2: d=1
//   左侧权重(≤1) = 2, 右侧权重(≥2) = 14+3+1 = 18
//   f(2) = f(1) + 1×2 - 1×18 = 24 + 2 - 18 = 8  ✓
//
// 从 t=2 → t=3: d=1
//   左侧权重(≤2) = 2+14 = 16, 右侧权重(≥3) = 3+1 = 4
//   f(3) = f(2) + 1×16 - 1×4 = 8 + 16 - 4 = 20
//
// 最小值 = 8，在 t=2 处取到 ✓
```

```cpp
class Solution {
public:
    long long minCost(vector<int>& nums, vector<int>& cost) {
        int n = nums.size();
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums[a] < nums[b];
        });
        
        // 先算 f(nums[排序后第0个])
        long long totalCost = 0;
        long long rightWeight = 0;  // 右侧总权重（不含当前）
        for (int i : idx) {
            totalCost += (long long)cost[i] * (nums[i] - nums[idx[0]]);
            rightWeight += cost[i];
        }
        
        long long ans = totalCost;
        long long leftWeight = 0;
        
        for (int k = 1; k < n; k++) {
            long long d = nums[idx[k]] - nums[idx[k - 1]];
            // idx[k-1] 从右侧移到左侧
            leftWeight += cost[idx[k - 1]];
            rightWeight -= cost[idx[k - 1]];
            // 目标值右移 d：左侧多走 d，右侧少走 d
            totalCost += d * leftWeight - d * rightWeight;
            ans = min(ans, totalCost);
        }
        return ans;
    }
};
```

**关键点**：leftWeight 和 rightWeight 的更新顺序——先把 idx[k-1] 归入左侧，再算差值。因为当目标从 nums[idx[k-1]] 移到 nums[idx[k]] 时，nums[idx[k-1]] 已经在目标的左侧了。

## 解法对比

| | 暴力枚举 | 三分搜索 | 加权中位数 | 前缀和递推 |
|---|---|---|---|---|
| 时间 | O(nV) | O(n log V) | O(n log n) | O(n log n) |
| 空间 | O(1) | O(1) | O(n) | O(n) |
| 核心思想 | 穷举 | 凸函数性质 | 统计学结论 | 增量计算 |
| 面试推荐 | ✗ | 加分项 | ⭐ 首选 | ⭐ 首选 |

- **加权中位数** 最能体现数学直觉，面试中说出"这是加权中位数"会让面试官眼前一亮。
- **前缀和递推** 不需要知道加权中位数的结论，纯编程技巧就能推出来，更"工程"。
- **三分搜索** 是通用技巧，适用于所有"凸函数求极值"场景，值得掌握。

## 易错点

1. **溢出**：
   - ✗ `int total = cost[i] * abs(nums[i] - t);` → cost 和 nums 都是 int，乘积可达 10^12
   - ✓ `long long total = (long long)cost[i] * abs(nums[i] - t);`

2. **加权中位数判断条件**：
   - ✗ `if (cumWeight >= totalWeight / 2)` — 整数除法截断，totalWeight=5 时 totalWeight/2=2，会提前触发
   - ✓ `if (cumWeight * 2 >= totalWeight)` — 避免除法，逻辑正确

3. **前缀和递推中 leftWeight/rightWeight 更新时机**：
   - ✗ 先算 totalCost 再更新 leftWeight → 漏掉了 idx[k-1] 的归类
   - ✓ 先把 idx[k-1] 归入 leftWeight、从 rightWeight 移出，再算增量

4. **排序的是索引而非值**：
   - 因为 nums 和 cost 是配对的，如果直接排序 nums 会丢失对应关系
   - 用索引数组间接排序，或者用 `vector<pair<int,int>>` 打包

## 面试追问

**Q1（基础理解）**：如果没有权重（所有 cost 相同），最优目标值是什么？
> 中位数。因为 Σ|x_i - t| 在中位数处取最小值，这是经典结论。

**Q2（优化）**：加了权重之后为什么变成加权中位数？能直观解释吗？
> 想象每个位置 nums[i] 放了 cost[i] 个人。现在要选一个集合点使所有人走的总路程最短。等价于无权版本的 cost[i] 个重复元素，中位数就是加权中位数。

**Q3（变体 — 三分搜索的通用性）**：如果开销不是 `cost[i] × |nums[i] - t|` 而是 `cost[i] × (nums[i] - t)²`（平方代价），怎么做？
> 平方代价的最优解是**加权平均数**。f(t) 对 t 求导令其为零：Σ 2·cost[i]·(t - nums[i]) = 0 → t = Σ(cost[i]·nums[i]) / Σ cost[i]。仍然是凸函数，三分搜索也可以用。

**Q4（Follow-up）**：能否 O(n) 解决（不排序）？
> 理论上可以。加权中位数等价于找第 k 大（k = totalWeight/2），可以用类似快速选择的算法 O(n) 期望时间。但面试中一般不要求。

## 相关题型

- **462. 最小操作次数使数组元素相等 II** — 本题的无权版本（所有 cost=1），直接找中位数即可。学会 2448 后，462 就是它的特殊情况。
- **296. 最佳会议地点** — 二维版本，横纵坐标分别求加权中位数。复用本题的加权中位数框架。
- **1231. 分享巧克力** — 同样可以用三分搜索/二分搜索在凸函数上求极值。复用三分搜索模板。
- **LeetCode 2033. 获取单值网格的最小操作数** — 矩阵版本的"移到中位数"，复用中位数最优的结论。