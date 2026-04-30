# 740. 删除并获得点数

## 核心思路

这道题的关键洞察是**问题转化**：选了一个数 `nums[i]`，就必须删掉所有 `nums[i]-1` 和 `nums[i]+1`。这意味着如果你决定选某个值 v，你一定会选**所有的 v**（因为选了一个 v 后其他 v 不会被删除）。所以实质上是对每个"值"做决策：选还是不选？

把每个值 v 能获得的总点数 `earn[v] = v * count(v)` 算出来，问题就变成了：在值域 `[1, maxVal]` 上，选了 v 就不能选 v-1 和 v+1，求最大收益。这就是经典的 **House Robber（打家劫舍）** 问题！

## 思维链

1. **读完题第一反应** → 选 `nums[i]` 得分 `nums[i]`，但必须删掉所有值为 `nums[i]-1` 和 `nums[i]+1` 的元素。这看起来像一个选择/不选择的决策问题 → DP。
2. **关键观察：选了 v 就选所有 v** → 假设你选了一个值为 3 的元素，所有 2 和 4 都被删了。但其他 3 还在！所以如果你决定选 3，最优策略一定是选完所有的 3，获得 `3 * count(3)` 点。
3. **问题转化** → 定义 `earn[v] = v * count(v)`，表示选择值 v 的总收益。现在问题变成：在值域上做决策，选 v 得 `earn[v]`，但不能同时选相邻的值 v-1 或 v+1。
4. **这不就是 House Robber 吗？** → 把值域看成一排"房子"，`earn[v]` 是每栋房子的财宝。不能选相邻的两栋（相邻的值不能同时选）。完全一样！
5. **状态转移** → `dp[v] = max(dp[v-1], dp[v-2] + earn[v])`。不选 v 时继承 dp[v-1]；选 v 时跳过 v-1，从 dp[v-2] 转移。
6. **空间优化** → dp 数组只依赖前两个值，可以用两个变量滚动优化到 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DP 数组版 | earn[] + dp[] | O(n + maxVal) | O(maxVal) | 思路最清晰 |
| 滚动变量优化 | 两个变量代替 dp 数组 | O(n + maxVal) | O(maxVal) | ⭐ 面试首选 |
| HashMap + 排序 | 只处理出现过的值 | O(n log n) | O(n) | 值域很大时 |

## 关键提示

1. **问题转化是核心**：本题的难点不在 DP 本身（House Robber 是入门级 DP），而在于**识别出这就是 House Robber**。面试中先说出这个转化，其他就水到渠成了。
2. **earn[v] 的含义**：选择值 v 能获得的**总点数**（所有值为 v 的元素之和），不是单个元素的点数。
3. **为什么选了 v 就要选所有 v？** 因为选一个 v 只会删除 v-1 和 v+1，不会删除其他 v。留着其他 v 不选是浪费。
4. **值域范围决定效率**：如果 maxVal 很大但元素很少，用 HashMap 存 earn 更高效。题目限制 `nums[i] <= 10^4`，所以直接开数组完全可行。

## 解法详解

### 解法1: DP 数组版 — O(n + maxVal) / O(maxVal) 思路最清晰

**思考过程**: 分两步——第一步统计 earn[]，第二步在 earn[] 上做 House Robber DP。

```
示例: nums = [2,2,3,3,3,4]

Step 1: 统计 earn[]
  earn[2] = 2 * 2 = 4  (两个2)
  earn[3] = 3 * 3 = 9  (三个3)
  earn[4] = 4 * 1 = 4  (一个4)
  earn[] = [0, 0, 4, 9, 4]
            0  1  2  3  4

Step 2: House Robber DP

  DP 表:
  v   | earn[v] | dp[v] = max(dp[v-1], dp[v-2]+earn[v]) | 选择
  ----|---------|--------------------------------------|------
  0   |    0    |  0                                    | -
  1   |    0    |  max(0, 0+0) = 0                      | 不选
  2   |    4    |  max(0, 0+4) = 4                      | 选2 (得4)
  3   |    9    |  max(4, 0+9) = 9                      | 选3 (得9, 跳过2)
  4   |    4    |  max(9, 4+4) = 9                      | 不选4 (选3更好)

  答案: dp[4] = 9 ✓

  决策路径: 选了所有的3 (3*3=9), 2和4都被删掉了
```

```cpp
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        int maxVal = *max_element(nums.begin(), nums.end());
        
        // Step 1: 统计每个值的总收益
        vector<int> earn(maxVal + 1, 0);
        for (int x : nums) {
            earn[x] += x;
        }
        
        // Step 2: House Robber DP
        vector<int> dp(maxVal + 1, 0);
        dp[1] = earn[1];
        for (int v = 2; v <= maxVal; v++) {
            dp[v] = max(dp[v - 1],         // 不选 v
                        dp[v - 2] + earn[v]); // 选 v
        }
        
        return dp[maxVal];
    }
};
```

**DP 表推演（另一个例子）**:
```
nums = [3,4,2]

earn[] = [0, 0, 2, 3, 4]

v   | earn[v] | dp[v-2] | dp[v-1] | dp[v] = max(dp[v-1], dp[v-2]+earn[v])
----|---------|---------|---------|------
1   |    0    |    0    |    -    | 0
2   |    2    |    0    |    0    | max(0, 0+2) = 2
3   |    3    |    0    |    2    | max(2, 0+3) = 3
4   |    4    |    2    |    3    | max(3, 2+4) = 6  ← 选4 (4分) + 选2 (2分) = 6

答案: 6 ✓ (选4删3, 再选2得6)
```

**关键点**:
- DP 在**值域**上进行，不是在原数组上。
- `dp[v-1]` 代表"不选 v"（继承前面的最优解）。
- `dp[v-2] + earn[v]` 代表"选 v"（必须跳过 v-1）。

---

### 解法2: 滚动变量优化 — O(n + maxVal) / O(maxVal) ⭐ 面试首选

**从解法1优化空间**: `dp[v]` 只依赖 `dp[v-1]` 和 `dp[v-2]`，所以只需要两个变量。

```cpp
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        int maxVal = *max_element(nums.begin(), nums.end());
        
        // Step 1: 统计每个值的总收益
        vector<int> earn(maxVal + 1, 0);
        for (int x : nums) {
            earn[x] += x;
        }
        
        // Step 2: House Robber with rolling variables
        int prev2 = 0, prev1 = 0; // prev2 = dp[v-2], prev1 = dp[v-1]
        for (int v = 1; v <= maxVal; v++) {
            int curr = max(prev1, prev2 + earn[v]);
            prev2 = prev1;
            prev1 = curr;
        }
        
        return prev1;
    }
};
```

**滚动变量推演**:
```
nums = [2,2,3,3,3,4], earn = [0, 0, 4, 9, 4]

v=1: curr = max(0, 0+0) = 0, prev2=0, prev1=0
v=2: curr = max(0, 0+4) = 4, prev2=0, prev1=4
v=3: curr = max(4, 0+9) = 9, prev2=4, prev1=9
v=4: curr = max(9, 4+4) = 9, prev2=9, prev1=9

答案: prev1 = 9 ✓
```

**空间优化对比**:
```
DP数组版: dp = [0, 0, 4, 9, 9]     → O(maxVal) 额外空间
滚动变量: prev2, prev1, curr        → O(1) 额外空间（不含 earn 数组）

总空间: O(maxVal)（earn 数组不可避免，但 dp 数组被优化掉了）
```

**关键点**:
- `prev2` 对应 `dp[v-2]`，`prev1` 对应 `dp[v-1]`。
- 每步更新：`curr = max(prev1, prev2 + earn[v])`，然后 `prev2 = prev1, prev1 = curr`。
- earn 数组的 O(maxVal) 空间无法避免（需要知道每个值的总收益）。

---

### 解法3: HashMap + 排序（值域稀疏时更优）— O(n log n) / O(n)

**不同视角**: 如果值域很大但元素很少（比如 `nums = [1, 100000]`），用大小为 maxVal 的数组太浪费。可以用 HashMap 存 earn，然后只对出现过的值排序后做 DP。

```cpp
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        // Step 1: 统计每个值的总收益
        map<int, int> earn; // 有序 map
        for (int x : nums) {
            earn[x] += x;
        }
        
        // Step 2: 在出现过的值上做 DP
        // 把 key 提取出来（已排序）
        vector<int> keys;
        for (auto& [k, v] : earn) {
            keys.push_back(k);
        }
        
        int prev2 = 0, prev1 = 0;
        int lastKey = -2; // 上一个处理的 key
        
        for (int key : keys) {
            int curr;
            if (key == lastKey + 1) {
                // key 和上一个相邻，不能同时选
                curr = max(prev1, prev2 + earn[key]);
            } else {
                // key 和上一个不相邻，直接选
                curr = prev1 + earn[key];
            }
            prev2 = prev1;
            prev1 = curr;
            lastKey = key;
        }
        
        return prev1;
    }
};
```

**关键点**:
- `map<int, int>` 自动排序，省去手动排序步骤。
- 关键判断：如果当前 key 和上一个 key 相邻（差 1），则存在冲突，做 House Robber 决策。否则直接选（没有冲突）。
- 当值域稀疏时（如 `[1, 50000, 100000]`），这个方法比解法1/2 高效得多（不需要开 100001 大小的数组）。

## 解法对比

|  | DP数组版 | 滚动变量优化 | HashMap+排序 |
|--|---------|------------|-------------|
| 时间 | O(n + maxVal) | O(n + maxVal) | O(n log n) |
| 空间 | O(maxVal) dp+earn | O(maxVal) earn | O(n) |
| 适用场景 | 通用 | **通用首选** | 值域稀疏时 |
| 代码量 | 中等 | ⭐ 最简 | 较长 |

**面试选择**: 解法2（滚动变量）代码最短、思路最清晰。如果面试官追问"值域很大怎么办"，再提解法3。

## 易错点

1. **earn[v] 计算错误**：
   - ✗ `earn[x] = x` → 只记了值，没有乘以出现次数
   - ✗ `earn[x]++` → 只计了次数，不是总点数
   - ✓ `earn[x] += x` → 每出现一次就加 x，等效于 `x * count(x)`

2. **DP 的遍历范围错误**：
   - ✗ 从 0 到 n-1（原数组长度）→ DP 应该在值域上
   - ✓ 从 1 到 maxVal → DP 在值域 [1, maxVal] 上进行

3. **忘记初始化 dp[1]**：
   - ✗ `dp[1] = 0` → 丢失了 earn[1] 的收益
   - ✓ `dp[1] = earn[1]`
   - 滚动变量版不需要显式初始化（循环从 v=1 开始自然处理）

4. **空数组未处理**：
   - ✗ `*max_element(nums.begin(), nums.end())` 在空数组上未定义行为
   - 题目保证 `nums.length >= 1`，但面试时提一句是好习惯

5. **HashMap 版忘记处理不相邻 key**：
   - ✗ 所有 key 都用 `max(prev1, prev2 + earn[key])` → 不相邻的值没有冲突，应该直接加上
   - ✓ 相邻 key 做 House Robber 决策，不相邻 key 直接累加

## 面试追问

**Q1: 为什么这个问题等价于 House Robber？**
> 选了值 v 就不能选 v-1 和 v+1，等价于"不能选相邻的"。把值域看作一排房子，每个值的总收益 `earn[v]` 就是房子里的财宝。选了第 v 栋就不能选第 v-1 和 v+1 栋，完全是 House Robber。

**Q2: 时间复杂度中 maxVal 这一项能消除吗？**
> 可以用 HashMap+排序方案（解法3），时间变成 O(n log n)，不依赖 maxVal。但在 maxVal <= 10^4 的约束下，O(maxVal) 完全可以接受。

**Q3: 如果允许选 v 后只删除 v-1（不删 v+1）呢？**
> 限制变弱了。如果只有单向限制，可以排序后贪心——从最大值开始选。或者用 DP 但转移方程变了：`dp[v] = max(dp[v-1], dp[v-2] + earn[v])` 只约束一侧。

**Q4: 如果数组中的值可以是负数呢？**
> `earn[v]` 可能为负。负收益的值永远不应该选。DP 中需要处理 `earn[v] < 0` 的情况——要么跳过负值，要么 DP 转移中取 max 时自然排除。

**Q5: House Robber 的环形变体（首尾相连）呢？**
> 即 LeetCode 213。分两次 DP：一次考虑第 1 到 n-1 号（不选最后），一次考虑第 2 到 n 号（不选第一），取两者最大值。

## 相关题型

- **198. 打家劫舍 (House Robber)** — 本题的直接前置。理解了 198，本题就是加一层"问题转化"。`dp[i] = max(dp[i-1], dp[i-2] + nums[i])`，完全相同的转移方程。
- **213. 打家劫舍 II (House Robber II)** — House Robber 的环形变体。如果本题的值域是环形的（最大值和最小值相邻），就需要这个技巧。
- **309. 最佳买卖股票时机含冷冻期** — 也是"选了当前就不能选相邻"的 DP 模式（买了之后有一天冷冻期）。状态转移的"互斥选择"思路一致。
- **1388. 3n 块比萨** — 更复杂的"不能选相邻"DP。环形 + 只能选 n 个，但核心思路仍然是 House Robber 的扩展。
