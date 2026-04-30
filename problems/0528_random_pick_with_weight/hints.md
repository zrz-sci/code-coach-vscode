# 528. 按权重随机选择

## 核心思路

本质上是**如何将均匀随机数映射到非均匀的概率分布**——把权重数组想象成一条数轴上的线段，每个下标占据的线段长度等于其权重，然后在总长度上随机扔一个点，看它落在哪个线段内。

## 思维链

1. **读完题第一反应**：权重越大的下标被选中概率越大。最朴素的想法——把每个下标按权重"展开"成多个副本，然后均匀随机选一个。例如 `w = [1, 3]`，展开成 `[0, 1, 1, 1]`，随机选一个即可。
2. **暴力解的瓶颈**：展开数组的大小等于所有权重之和，如果权重很大（最大 10^5，数组长度 10^4），展开后数组长达 10^9，内存和时间都不可接受。
3. **怎么突破？** 不需要真的展开——想象一条长度为 `sum(w)` 的数轴，每个下标占一段。随机生成 `[1, sum(w)]` 的整数，然后**找到它落在哪一段**。这就是前缀和 + 查找的思路。
4. **查找怎么加速？** 前缀和数组是有序的！随机数落在哪一段 = 在有序的前缀和数组中找第一个 ≥ 随机数的位置 → **二分查找**！
5. **总结**：构造时 O(n) 建前缀和，每次 `pickIndex` 只需 O(log n) 的二分查找。

## 解法概览

| 解法 | 思路 | 构造时间 | pickIndex 时间 | 空间 | 面试 |
|------|------|----------|---------------|------|------|
| 展开数组 | 按权重复制下标，均匀随机选 | O(sum) | O(1) | O(sum) | 能说出即可 |
| 前缀和 + 线性扫描 | 前缀和 + 遍历查找 | O(n) | O(n) | O(n) | 能说出即可 |
| 前缀和 + 二分查找 | 前缀和 + 二分定位 | O(n) | O(log n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **数轴模型**：把 `w = [1, 3, 2]` 想象成数轴 `[1][2,3,4][5,6]`，前缀和 `[1, 4, 6]`。随机数 3 落在第二段（下标 1）。

```
权重:     w = [1,   3,   2]
数轴:     |1|  2  3  4|5  6|
           ↑          ↑    ↑
前缀和: prefix = [1,  4,   6]

随机数 = 3  →  在前缀和中找第一个 ≥ 3 的位置  →  prefix[1]=4  →  返回下标 1
```

2. **随机数范围**：生成 `[1, totalSum]` 的整数（不是 `[0, totalSum-1]`），这样和前缀和配合最自然。用浮点数 `[0, totalSum)` 也行，但整数更简洁不易出错。

3. **二分查找目标**：在前缀和数组中找**第一个 ≥ target 的位置**，即 `lower_bound`。

4. **不要用浮点除法算概率**：直接用整数前缀和 + 整数随机数，避免浮点精度问题。

5. **`rand()` vs 标准库**：面试中可以用 `rand() % total + 1`，但要知道这不是完美均匀的（模偏差）。更好的做法是 `uniform_int_distribution`。

## 解法详解

### 解法1: 展开数组 — O(sum) / O(sum)

**思考过程**：最直觉的想法——权重为 3 就复制 3 份，然后均匀随机选。

```cpp
class Solution {
public:
    vector<int> expanded;
    
    Solution(vector<int>& w) {
        for (int i = 0; i < w.size(); i++) {
            for (int j = 0; j < w[i]; j++) {
                expanded.push_back(i);  // 下标 i 出现 w[i] 次
            }
        }
    }
    
    int pickIndex() {
        return expanded[rand() % expanded.size()];
    }
};
```

**关键点**：当权重总和很大时（最多 10^9），内存直接爆炸。这个解法只适合权重很小的场景，但它清晰地展示了问题的本质。

### 解法2: 前缀和 + 线性扫描 — O(n) / O(n)

**从解法1优化**：不展开数组，而是用前缀和"虚拟"地表示每个区间。随机一个数后线性扫描找它属于哪个区间。

```cpp
class Solution {
public:
    vector<int> prefix;
    int total;
    
    Solution(vector<int>& w) {
        prefix.resize(w.size());
        prefix[0] = w[0];
        for (int i = 1; i < w.size(); i++) {
            prefix[i] = prefix[i - 1] + w[i];
        }
        total = prefix.back();
    }
    
    int pickIndex() {
        int target = rand() % total + 1;  // [1, total]
        // 线性扫描：找第一个前缀和 >= target 的位置
        for (int i = 0; i < prefix.size(); i++) {
            if (prefix[i] >= target) {
                return i;
            }
        }
        return prefix.size() - 1;  // 理论上不会到这里
    }
};
```

**关键点**：构造 O(n)，但每次 pickIndex 是 O(n)。如果调用次数很多，瓶颈在线性扫描。

### 解法3: 前缀和 + 二分查找 — O(n) 构造 / O(log n) 查询 ⭐ 面试首选

**从解法2优化**：前缀和数组天然有序！线性扫描改成二分查找，pickIndex 从 O(n) 降到 O(log n)。

```cpp
class Solution {
public:
    vector<int> prefix;
    int total;
    
    Solution(vector<int>& w) {
        prefix.resize(w.size());
        prefix[0] = w[0];
        for (int i = 1; i < w.size(); i++) {
            prefix[i] = prefix[i - 1] + w[i];
        }
        total = prefix.back();
    }
    
    int pickIndex() {
        // 生成 [1, total] 的随机整数
        int target = rand() % total + 1;
        
        // 二分查找：在 prefix 中找第一个 >= target 的位置
        int lo = 0, hi = prefix.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (prefix[mid] < target) {
                lo = mid + 1;   // mid 对应的区间装不下 target，往右找
            } else {
                hi = mid;       // prefix[mid] >= target，mid 可能是答案
            }
        }
        return lo;
    }
};
```

**也可以直接用 STL**：

```cpp
int pickIndex() {
    int target = rand() % total + 1;
    // lower_bound 找第一个 >= target 的位置
    return lower_bound(prefix.begin(), prefix.end(), target) - prefix.begin();
}
```

**为什么二分条件是 `prefix[mid] < target`？**

```
w = [1, 3, 2], prefix = [1, 4, 6], total = 6

数轴:  |  1  |  2  3  4  |  5  6  |
       idx=0     idx=1       idx=2
前缀和: prefix[0]=1  prefix[1]=4  prefix[2]=6

target=1: 找第一个 >=1 的 → prefix[0]=1 → 返回 0 ✓
target=2: 找第一个 >=2 的 → prefix[1]=4 → 返回 1 ✓
target=4: 找第一个 >=4 的 → prefix[1]=4 → 返回 1 ✓
target=5: 找第一个 >=5 的 → prefix[2]=6 → 返回 2 ✓
target=6: 找第一个 >=6 的 → prefix[2]=6 → 返回 2 ✓
```

下标 0 对应 target ∈ [1,1]（1个数），概率 1/6 ✓  
下标 1 对应 target ∈ [2,4]（3个数），概率 3/6 ✓  
下标 2 对应 target ∈ [5,6]（2个数），概率 2/6 ✓

## 解法对比

| | 展开数组 | 前缀和+线性扫描 | 前缀和+二分查找 |
|---|---|---|---|
| 构造时间 | O(sum) | O(n) | O(n) |
| 查询时间 | O(1) | O(n) | O(log n) |
| 空间 | O(sum) | O(n) | O(n) |
| 适用场景 | 权重和很小 | 理解原理 | **面试标准答案** |
| 核心技巧 | 无 | 前缀和 | 前缀和+二分 |

**什么时候选哪个？** 面试直接写解法3。解法1和2用来展示思考过程，证明你理解问题本质。

## 易错点

1. **随机数范围错误**：
   - ✗ `rand() % total` → 生成 `[0, total-1]`，如果用 `prefix[mid] >= target`，target=0 永远返回 0
   - ✓ `rand() % total + 1` → 生成 `[1, total]`，和前缀和的含义一致

2. **二分边界写错**：
   - ✗ `if (prefix[mid] <= target) lo = mid + 1` → 当 `prefix[mid] == target` 时跳过了正确答案
   - ✓ `if (prefix[mid] < target) lo = mid + 1` → 严格小于才排除，等于时保留

3. **前缀和溢出**：权重最大 10^5，数组最长 10^4，总和最大 10^9，int 够用。但如果权重更大需要用 long long。

4. **`rand()` 的模偏差**：`rand() % total` 在 `RAND_MAX` 不是 `total` 整数倍时不完美均匀。面试时提一句即可，OJ 上这不影响通过。

## 面试追问

**Q1: 为什么不能直接生成 [0, n-1] 的随机数然后返回？**
> 那样每个下标概率相等（1/n），但题目要求按权重分配概率。权重大的被选中概率要更高。

**Q2: 如果权重会动态变化（增加/修改某个权重），怎么优化？**
> 用**树状数组 (Binary Indexed Tree)** 或**线段树**维护前缀和。修改操作 O(log n)，查询也是 O(log n)。普通前缀和数组修改需要 O(n) 重新计算。

**Q3: 如果要求完美均匀随机（无模偏差），怎么做？**
> 使用 C++ 的 `<random>` 库：`mt19937` 引擎 + `uniform_int_distribution<int>(1, total)`。或者用拒绝采样：生成 [0, RAND_MAX] 的数，如果 ≥ 最大的 total 的整数倍则重新生成。

**Q4: 如果有 n 个权重且 pickIndex 调用极其频繁（10^8 次），还能优化吗？**
> 可以用 **Alias Method**（别名法），预处理 O(n)，每次查询 O(1) 且无需二分。核心思想是将非均匀分布转化为一张查找表+硬币翻转。

## 相关题型

- **398. 随机数索引 (Random Pick Index)** — 同样是加权/条件随机选择，但用蓄水池抽样(Reservoir Sampling)处理流式数据，无需预知总数
- **710. 黑名单中的随机数 (Random Pick with Blacklist)** — 均匀随机但要排除黑名单，复用"映射+随机"的思想
- **497. 非重叠矩形中的随机点** — 复用本题的"前缀和+二分"框架：先按面积权重选矩形，再在矩形内均匀选点
- **470. 用 Rand7() 实现 Rand10()** — 随机数变换的另一个经典问题，用拒绝采样