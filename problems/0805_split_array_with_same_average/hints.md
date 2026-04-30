# 805. 数组的均值分割

## 核心思路

本质是一个**子集和**问题的变形。将数组分成两个非空子集 A 和 B，使得 `avg(A) == avg(B)`。数学推导后等价于：能否选出大小为 k（1 <= k <= n/2）的子集，使得子集的和等于 `k * totalSum / n`。由于 n 最大为 30，用**Meet in the Middle（折半搜索）**或**DP**来解决。

## 思维链

1. **读完题第一反应** → 枚举所有子集？2^30 ≈ 10^9，太慢。
2. **数学化简** → `sum(A)/|A| == sum(B)/|B|` 等价于 `sum(A)/|A| == totalSum/n`。即：找大小为 k 的子集使得 `sum = k * totalSum / n`。
3. **进一步简化** → 令 `nums[i] -= totalSum / n`（将平均值标准化为 0），则问题变成：找大小为 k 的子集使得子集和为 0。
4. **方法选择** → n=30 适合折半搜索：将数组分成两半（各 15 个），分别枚举所有子集（2^15 ≈ 32768），然后在两半之间配对。
5. **DP 方法** → 也可以用 DP：`dp[k]` = 选 k 个数可能达到的所有和的集合。时间 O(n^2 * sum)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| Meet in the Middle | 折半枚举子集，配对查找 | O(2^(n/2) * n) | O(2^(n/2)) | ⭐ 必须写出 |
| DP（子集和） | dp[k] 记录选 k 个数可达的和 | O(n^2 * totalSum) | O(n * totalSum) | 备选方案 |

## 关键提示

1. **数学核心**：`avg(A) == avg(B) == avg(全数组)`。所以只需要找一个子集使得其平均值等于整个数组的平均值。
2. **整数化处理**：为避免浮点精度问题，将条件转化为 `sum(A) * n == totalSum * |A|`，全程用整数运算。
3. **折半搜索**：数组分成两半 L 和 R。对 L 枚举所有子集，记录 `(size, sum)` 对。对 R 也枚举所有子集，在 L 中查找互补的 `(size, sum)` 对。
4. **注意子集大小约束**：A 和 B 都必须非空，所以 k 的范围是 `1 <= k <= n-1`。折半搜索时左右两半可以有空子集，但不能两半都空。
5. **减去平均值的技巧**：`nums[i] = nums[i] * n - totalSum`，这样目标和变为 0，简化了搜索条件。

## 解法详解

### 解法1: Meet in the Middle -- O(2^(n/2) * n) / O(2^(n/2)) ⭐ 面试首选

**思考过程**: n=30 意味着 2^30 太大，但 2^15 只有 32768，可以接受。折半枚举的经典应用。

```
数学推导:
avg(A) = avg(B)
⟹ sum(A)/|A| = (totalSum - sum(A)) / (n - |A|)
⟹ sum(A) * n = totalSum * |A|

令 nums'[i] = nums[i] * n - totalSum
⟹ 找子集 A 使得 sum(A') = 0 且 1 ≤ |A| ≤ n-1
```

```cpp
class Solution {
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return false;

        int totalSum = 0;
        for (int x : nums) totalSum += x;

        // 标准化: nums[i] = nums[i] * n - totalSum
        // 目标：找非空真子集使得和为 0
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = nums[i] * n - totalSum;
        }

        int half = n / 2;
        // 左半部分: a[0..half-1]
        // 右半部分: a[half..n-1]

        // 枚举左半所有子集，按大小分组存储可能的 sum
        // left[k] = 选 k 个元素时可能的 sum 集合
        vector<unordered_set<int>> left(half + 1);
        for (int mask = 0; mask < (1 << half); mask++) {
            int s = 0, cnt = 0;
            for (int j = 0; j < half; j++) {
                if (mask & (1 << j)) {
                    s += a[j];
                    cnt++;
                }
            }
            left[cnt].insert(s);
        }

        int rLen = n - half;
        for (int mask = 0; mask < (1 << rLen); mask++) {
            int s = 0, cnt = 0;
            for (int j = 0; j < rLen; j++) {
                if (mask & (1 << j)) {
                    s += a[half + j];
                    cnt++;
                }
            }
            // 在左半中查找互补: 需要 leftSum = -s, leftCnt + cnt ∈ [1, n-1]
            for (int lc = 0; lc <= half; lc++) {
                int total = lc + cnt;
                if (total == 0 || total == n) continue;  // 子集非空且非全集
                if (left[lc].count(-s)) return true;
            }
        }
        return false;
    }
};
```

---

### 解法2: DP（子集和） -- O(n^2 * totalSum) / O(n * totalSum)

```cpp
class Solution {
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size(), totalSum = 0;
        for (int x : nums) totalSum += x;

        // 剪枝: 检查是否存在 k 使得 totalSum * k % n == 0
        bool possible = false;
        for (int k = 1; k <= n / 2; k++) {
            if ((long long)totalSum * k % n == 0) {
                possible = true;
                break;
            }
        }
        if (!possible) return false;

        // dp[k] = 选 k 个数可达的所有 sum 的集合
        vector<unordered_set<int>> dp(n / 2 + 1);
        dp[0].insert(0);

        for (int num : nums) {
            // 逆序更新避免重复使用
            for (int k = min((int)dp.size() - 1, (int)dp.size() - 1); k >= 1; k--) {
                for (int prevSum : dp[k - 1]) {
                    dp[k].insert(prevSum + num);
                }
            }
        }

        for (int k = 1; k <= n / 2; k++) {
            if ((long long)totalSum * k % n == 0) {
                int target = totalSum * k / n;
                if (dp[k].count(target)) return true;
            }
        }
        return false;
    }
};
```

## 易错点

1. **浮点精度问题**：
   - ✗ 直接比较 `sum(A) / |A| == totalSum / n` → 浮点误差
   - ✓ 转化为整数条件 `sum(A) * n == totalSum * |A|`

2. **子集大小为 0 或 n**：
   - ✗ 允许空子集或全集 → 不满足"两个非空子集"的要求
   - ✓ 检查 `total ∈ [1, n-1]`

3. **折半搜索中两半都空**：
   - ✗ mask=0 时 cnt=0，如果左右都是 0，total=0 → 空集
   - ✓ 需要排除 `total == 0` 和 `total == n` 的情况

4. **DP 更新方向错误**：
   - ✗ 正序更新 k → 同一个 num 被选多次
   - ✓ 逆序更新 k → 每个 num 最多选一次（类似 01 背包）

## 面试追问

**Q1: 为什么标准化后目标变成找子集和为 0？**
> 令 `a[i] = nums[i] * n - totalSum`。选 k 个元素的子集 A 满足 `sum(A) * n = totalSum * k`，等价于 `sum(a_A) = sum(nums_A * n - totalSum) = n * sum(nums_A) - k * totalSum = 0`。所以标准化后找和为 0 的非空真子集。

**Q2: Meet in the Middle 的思想是什么？适用场景？**
> 将搜索空间一分为二，分别枚举两半的所有可能，再在两半间查找匹配。将 O(2^n) 降到 O(2^(n/2))。适用于 n 在 20-40 之间、暴力太慢但 n/2 可接受的场景。

**Q3: 能否用 bitset 优化 DP？**
> 可以。将 `dp[k]` 从 set 替换为 bitset，每个 bit 表示某个 sum 是否可达。bitset 的位运算能大幅加速（常数因子降低 64 倍），空间也更紧凑。

## 相关题型

- **416. 分割等和子集** — 子集和 DP 的经典题。本题是其推广：不仅要和相等，还要均值相等。
- **698. 划分为k个相等的子集** — 将数组分成 k 个等和子集，回溯+剪枝。
- **1755. 最接近目标值的子序列和** — Meet in the Middle 的直接应用，将数组折半枚举后查找。
- **494. 目标和** — 子集和 DP 的变体，通过正负号选择达到目标值。
