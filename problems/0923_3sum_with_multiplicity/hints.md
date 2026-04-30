[file://solution.cpp](./solution.cpp)

# 923. 三数之和的多种可能

## 核心思路

值域只有 0~100，这是突破口。不要像经典 3Sum 一样用排序+双指针遍历下标三元组，而是**先统计每个值的出现次数**，然后在值域上枚举三个值 (a, b, c) 使得 a+b+c=target，用组合数学计算每种 (a,b,c) 组合对应多少个下标三元组。

## 思维链

1. **读完题第一反应**：三数之和等于 target，统计满足条件的 (i,j,k) 三元组数量。看起来像 3Sum 变体，但要的是数量而非去重后的组合。

2. **暴力 O(n^3) 太慢？** n 最大 3000，O(n^3) = 2.7 * 10^10，绝对超时。需要优化。

3. **关键约束：0 <= arr[i] <= 100**。值域极小！这意味着可以用计数数组 `count[v]` 记录每个值出现的次数，然后在值域 [0, 100] 上枚举，而非在下标 [0, n-1] 上枚举。

4. **值域枚举思路**：固定 a <= b <= c 且 a+b+c = target，对于每组 (a,b,c)，用组合数学算出有多少种选法：
   - a < b < c：`count[a] * count[b] * count[c]`
   - a = b < c：`C(count[a], 2) * count[c]`
   - a < b = c：`count[a] * C(count[b], 2)`
   - a = b = c：`C(count[a], 3)`

5. **另一种思路：排序 + 双指针**。固定 `arr[i]`，对后面的子数组做双指针找两数之和。但需要小心处理重复元素的计数。时间 O(n^2) 但实现稍复杂。

6. **取模**：答案可能很大，全程对 10^9+7 取模。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 值域计数 + 组合 | count 数组 + 枚举值三元组 + 组合数学 | O(V^2) | O(V) | ⭐ 面试首选 |
| 排序 + 双指针 | 固定一个数，双指针找另外两个 | O(n^2) | O(1) | 经典变体 |

> V = 101（值域大小）。值域计数法利用了 arr[i] <= 100 的关键约束，时间仅 O(V^2) 约 10000 次操作，极快。

## 关键提示

1. **值域小是关键** — arr[i] 范围只有 [0, 100]，共 101 个可能值。枚举值三元组最多 101^2 / 2 次，比枚举下标三元组快得多。

2. **三种重复情况要分别处理** — (a < b < c)、(a = b < c)、(a < b = c)、(a = b = c) 的组合计数公式不同。C(n,2) = n*(n-1)/2，C(n,3) = n*(n-1)*(n-2)/6。

3. **取模时的陷阱** — 乘法中间结果可能溢出 int，用 `long long` 计算后再取模。

4. 值域计数示意：
```
arr = [1,1,2,2,3,3,4,4,5,5], target = 8
count: {1:2, 2:2, 3:2, 4:2, 5:2}

枚举 (a,b,c) where a+b+c=8, a<=b<=c:
(1,2,5): 三个不同 → 2*2*2 = 8
(1,3,4): 三个不同 → 2*2*2 = 8
(2,2,4): 两个相同 → C(2,2)*2 = 1*2 = 2
(2,3,3): 两个相同 → 2*C(2,2) = 2*1 = 2
总计: 8+8+2+2 = 20 ✓
```

## 解法详解

### 解法1: 值域计数 + 组合数学 — O(V^2) / O(V) ⭐ 面试首选

**思考过程**：

统计每个值的出现次数。然后枚举所有满足 a <= b <= c 且 a+b+c = target 的值三元组。根据 a, b, c 之间的相等关系，用不同的组合公式计算下标三元组数：

- 三个值都不同：`count[a] * count[b] * count[c]`
- 两个值相同（a=b 或 b=c）：用 C(n,2) * 另一个值的 count
- 三个值都相同：C(n,3)

```cpp
class Solution {
public:
    int threeSumMulti(vector<int>& arr, int target) {
        const int MOD = 1e9 + 7;
        long long count[101] = {};
        
        for (int x : arr) {
            count[x]++;
        }
        
        long long result = 0;
        
        for (int a = 0; a <= 100; a++) {
            for (int b = a; b <= 100; b++) {
                int c = target - a - b;
                if (c < b || c > 100) continue;
                if (count[a] == 0 || count[b] == 0 || count[c] == 0) continue;
                
                if (a == b && b == c) {
                    // 三个值都相同: C(count[a], 3)
                    result += count[a] * (count[a] - 1) * (count[a] - 2) / 6;
                } else if (a == b) {
                    // a == b != c: C(count[a], 2) * count[c]
                    result += count[a] * (count[a] - 1) / 2 * count[c];
                } else if (b == c) {
                    // a != b == c: count[a] * C(count[b], 2)
                    result += count[a] * count[b] * (count[b] - 1) / 2;
                } else {
                    // 三个都不同
                    result += count[a] * count[b] * count[c];
                }
                
                result %= MOD;
            }
        }
        
        return (int)result;
    }
};
```

**关键点**：
- 枚举时 `b` 从 `a` 开始，`c = target - a - b`，确保 `a <= b <= c`，避免重复计数。
- `c < b` 或 `c > 100` 时跳过（不合法的三元组）。
- 用 `long long` 避免乘法溢出。

### 解法2: 排序 + 双指针 — O(n^2) / O(1)

**思考过程**：

排序后，固定 `arr[i]`，对 `[i+1, n-1]` 区间用双指针找两数之和等于 `target - arr[i]`。当找到匹配时，需要统计重复元素的组合数。

```cpp
class Solution {
public:
    int threeSumMulti(vector<int>& arr, int target) {
        const int MOD = 1e9 + 7;
        sort(arr.begin(), arr.end());
        long long result = 0;
        int n = arr.size();
        
        for (int i = 0; i < n - 2; i++) {
            int left = i + 1, right = n - 1;
            int remain = target - arr[i];
            
            while (left < right) {
                int sum = arr[left] + arr[right];
                if (sum < remain) {
                    left++;
                } else if (sum > remain) {
                    right--;
                } else {
                    // arr[left] + arr[right] == remain
                    if (arr[left] == arr[right]) {
                        // 区间 [left, right] 全是相同值
                        long long len = right - left + 1;
                        result = (result + len * (len - 1) / 2) % MOD;
                        break;
                    } else {
                        // 统计左边相同值的个数和右边相同值的个数
                        int leftCount = 1, rightCount = 1;
                        while (left + leftCount < right && arr[left + leftCount] == arr[left])
                            leftCount++;
                        while (right - rightCount > left && arr[right - rightCount] == arr[right])
                            rightCount++;
                        result = (result + (long long)leftCount * rightCount) % MOD;
                        left += leftCount;
                        right -= rightCount;
                    }
                }
            }
        }
        
        return (int)result;
    }
};
```

**关键点**：
- 排序后，重复值是连续的，方便统计。
- 当 `arr[left] == arr[right]` 时，区间内所有元素相同，选2个的方案数 = C(len, 2)。
- 否则分别统计左右连续相同值的数量，相乘即为组合数。

## 解法对比

| 维度 | 值域计数 | 排序 + 双指针 |
|------|---------|--------------|
| 时间 | O(V^2) ≈ 10^4 | O(n^2) ≈ 9*10^6 |
| 空间 | O(V) = O(101) | O(1) 排序外 |
| 适用条件 | 值域小 (arr[i] <= 100) | 通用 |
| 代码难度 | 组合公式分类讨论 | 重复值计数逻辑 |
| 面试推荐 | 值域小时首选 | 通用性更好 |

## 易错点

1. **组合数公式搞错**：
   - ✗ C(n,2) = n*n/2 → 多算了
   - ✓ C(n,2) = n*(n-1)/2

2. **忘记取模导致溢出**：
   - ✗ 用 int 做乘法，count[a]*count[b]*count[c] 可能溢出
   - ✓ 用 long long，每次乘法后取模

3. **值域计数时 c 的范围没检查**：
   - ✗ 不检查 `c > 100` → 数组越界
   - ✓ 必须检查 `c >= b && c <= 100`

4. **双指针法中重复值计数错误**：
   - ✗ 只加 1 而不是乘以重复数量 → 漏算大量组合
   - ✓ 统计 leftCount 和 rightCount，结果加 leftCount * rightCount

## 面试追问

**Q1: 如果 arr[i] 的范围不是 [0,100] 而是 [0, 10^9]，值域计数法还能用吗？**

> 不能直接用了，因为值域太大。此时排序+双指针是更好的选择，O(n^2)。或者用哈希表代替计数数组，只记录出现过的值，但最坏情况下哈希表大小等于 n，枚举三元组的时间与排序+双指针相当。

**Q2: 为什么枚举时要保证 a <= b <= c？如果不保证会怎样？**

> 如果 a, b, c 无序枚举，同一组值会被枚举多次（比如 (1,2,5) 和 (2,1,5) 和 (5,2,1) 算了 6 次）。强制 a <= b <= c 确保每组值只枚举一次，然后用组合数学精确计算下标排列数。这样避免了去重的复杂逻辑。

**Q3: 这道题和经典 3Sum (LeetCode 15) 的核心区别是什么？**

> 经典 3Sum 要求去重返回不同的值三元组，本题要求统计下标三元组的总数量（含重复值）。因此本题的核心难点是"如何正确计算重复元素的组合数"，而经典 3Sum 的难点是"如何跳过重复值避免输出重复三元组"。两者双指针框架类似，但处理重复的逻辑完全不同。

## 相关题型

- **LeetCode 15. 三数之和** — 经典 3Sum，返回去重后的值三元组。本题是其计数变体，侧重组合数学。
- **LeetCode 1. 两数之和** — 本题固定一个数后变成 2Sum 计数问题。理解 2Sum 的哈希/双指针思路是基础。
- **LeetCode 18. 四数之和** — 多一层循环，但核心的排序+双指针框架相同。值域计数法也可类推到四数之和。
