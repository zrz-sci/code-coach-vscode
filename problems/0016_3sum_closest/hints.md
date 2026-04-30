# 16. 最接近的三数之和

## 核心思路
本质上就是在数组中找三个数，使它们的和与 target 的**绝对差**最小。这是 LeetCode 15「三数之和」的变体——从"找等于0"变成"找最接近target"。

## 思维链

1. **读完题第一反应**：三个数的组合有 C(n,3) 种，暴力枚举所有组合，记录与 target 差值最小的那个和。三重循环，O(n³)。

2. **暴力解的瓶颈在哪？** 固定第一个数后，需要在剩余数组中找两个数使和最接近某个值——这本质是一个「两数之和最接近」的子问题。暴力用双重循环找，能不能更快？

3. **怎么突破瓶颈？** 如果数组是**有序的**，两个数的和可以用**双指针**在 O(n) 内逼近目标值：和太小就左指针右移（增大和），和太大就右指针左移（减小和）。

4. **组合起来**：先排序 O(n log n)，外层枚举第一个数 O(n)，内层双指针找剩余两个数 O(n)，总共 O(n²)。

5. **提前终止优化**：如果某次三数之和恰好等于 target，直接返回（差值为0不可能更小了）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 三重循环遍历所有三元组 | O(n³) | O(1) | 能说出即可 |
| 排序 + 双指针 | 固定一个数，双指针找另外两个 | O(n²) | O(1)* | ⭐ 必须写出 |

*排序的空间取决于实现，一般认为 O(log n) ~ O(n)，但额外辅助空间 O(1)。

## 关键提示

1. **和第15题「三数之和」对比思考**：15题找 sum == 0，本题找 sum 最接近 target。核心框架完全一样，只是判断条件不同。

2. **排序不改变答案**：题目要求返回的是**和**而不是下标，所以排序是安全的。

3. **双指针为什么不会漏解？** 排序后，`sum < target` 时右移左指针、`sum > target` 时左移右指针——每次移动都在朝"更接近target"的方向走，被跳过的组合一定不会更优。

4. **更新 closest 的条件**：比较的是 `abs(sum - target)` 和 `abs(closest - target)`，不是 sum 本身。

5. **提前返回**：如果 `sum == target`，直接返回 target，不可能找到更近的了。

## 解法详解

### 解法1: 暴力枚举 — O(n³) / O(1)

**思考过程**: 最直接的想法——把所有三元组列出来，计算和，记录与 target 差值最小的。

```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int n = nums.size();
        int closest = nums[0] + nums[1] + nums[2]; // 用第一个合法组合初始化
        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 1; j < n - 1; j++) {
                for (int k = j + 1; k < n; k++) {
                    int sum = nums[i] + nums[j] + nums[k];
                    // 用绝对差来衡量"接近程度"
                    if (abs(sum - target) < abs(closest - target)) {
                        closest = sum;
                    }
                }
            }
        }
        return closest;
    }
};
```

**关键点**: n ≤ 500 时 O(n³) 约 1.25 亿次运算，会超时或卡在边界。但它帮助我们理解问题结构：固定 i 后，本质是找两个数使和最接近 `target - nums[i]`。

---

### 解法2: 排序 + 双指针 — O(n²) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是内层双重循环 O(n²) 找两个数。排序后利用有序性，用双指针把内层降到 O(n)。

**双指针逼近过程示意**（以 `nums = [-4, -1, 1, 2]`, `target = 1` 为例）：

```
排序后: [-4, -1, 1, 2]

固定 i=0, nums[i]=-4, 需要另外两数之和接近 1-(-4)=5
  left=1, right=3: sum = -4 + (-1) + 2 = -3  |diff|=4  太小→left++
  left=2, right=3: sum = -4 + 1 + 2 = -1     |diff|=2  太小→left++
  left==right, 结束

固定 i=1, nums[i]=-1, 需要另外两数之和接近 1-(-1)=2
  left=2, right=3: sum = -1 + 1 + 2 = 2      |diff|=0  等于target! 直接返回
```

```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end()); // 排序是双指针的前提
        int n = nums.size();
        int closest = nums[0] + nums[1] + nums[2];
        
        for (int i = 0; i < n - 2; i++) {
            // 优化: 跳过重复的 i（不影响正确性，但能减少无意义计算）
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                // 更新最接近的和
                if (abs(sum - target) < abs(closest - target)) {
                    closest = sum;
                }
                
                if (sum == target) {
                    return target; // 差值为0，不可能更近了
                } else if (sum < target) {
                    left++;  // 和太小，需要更大的数
                } else {
                    right--; // 和太大，需要更小的数
                }
            }
        }
        return closest;
    }
};
```

**进一步剪枝优化（可选加分项）**：

```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int closest = nums[0] + nums[1] + nums[2];
        
        for (int i = 0; i < n - 2; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            // 剪枝1: 当前i能取到的最小和（i + 紧邻的两个最小值）
            int minSum = nums[i] + nums[i + 1] + nums[i + 2];
            if (minSum > target) {
                // 后面的i更大，minSum只会更大，但当前minSum可能比之前的closest更近
                if (abs(minSum - target) < abs(closest - target)) closest = minSum;
                break; // 后续i不可能更优
            }
            
            // 剪枝2: 当前i能取到的最大和（i + 最大的两个值）
            int maxSum = nums[i] + nums[n - 2] + nums[n - 1];
            if (maxSum < target) {
                // 当前i的所有组合都小于target，最大和是最接近的
                if (abs(maxSum - target) < abs(closest - target)) closest = maxSum;
                continue; // 跳到下一个更大的i
            }
            
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (abs(sum - target) < abs(closest - target)) closest = sum;
                
                if (sum == target) return target;
                else if (sum < target) left++;
                else right--;
            }
        }
        return closest;
    }
};
```

## 解法对比

| | 暴力枚举 | 排序+双指针 | 排序+双指针+剪枝 |
|---|---|---|---|
| 时间 | O(n³) | O(n²) | O(n²) 最坏，实际更快 |
| 空间 | O(1) | O(1) | O(1) |
| 核心优化 | — | 排序+有序性消除一层循环 | 额外跳过不可能的分支 |
| 面试推荐 | 开场说思路 | **主力解法** | 追问时展示 |

## 易错点

1. **closest 初始化错误**
   - ✗ `int closest = INT_MAX;` → 后续 `abs(closest - target)` 可能溢出！
   - ✓ `int closest = nums[0] + nums[1] + nums[2];` → 用一个合法的三数之和初始化

2. **比较条件写反**
   - ✗ `if (abs(sum - target) > abs(closest - target))` → 这是在找最远的和
   - ✓ `if (abs(sum - target) < abs(closest - target))` → 严格小于才更新

3. **忘记处理 sum == target 的提前返回**
   - 不是 bug，但是重要优化。面试中不写这个，面试官可能会问"能不能更快终止？"

4. **跳过重复元素时 i 的判断条件**
   - ✗ `if (nums[i] == nums[i - 1]) continue;` → 当 i==0 时越界
   - ✓ `if (i > 0 && nums[i] == nums[i - 1]) continue;`

5. **剪枝逻辑中 break vs continue 搞混**
   - `minSum > target` 时应该 `break`（后面的 i 更大，不可能更优）
   - `maxSum < target` 时应该 `continue`（当前 i 不行，但后面更大的 i 可能行）

## 面试追问

**Q1（基础理解）**: 暴力解的时间复杂度是多少？能通过吗？
> O(n³)，n=500 时约 1.25 亿次，可能超时。面试中先说暴力思路，然后主动优化。

**Q2（核心优化）**: 为什么排序后双指针不会遗漏最优解？
> 排序后数组有序。当 sum < target 时，右移 left 使 sum 增大（更接近 target）；当 sum > target 时，左移 right 使 sum 减小。对于固定的 i，left 和 right 的每次移动都是在朝着"更接近"的方向走。被跳过的 (left, right) 组合，其 sum 与 target 的差值一定不会比当前更小。

**Q3（变体）**: 如果要返回三个数的下标而不是和呢？
> 排序会打乱下标。方案1：记录 (值, 原始下标) 的 pair 数组，排序后双指针同时维护下标。方案2：用哈希表记录每个值到原始下标的映射（注意重复值处理）。

**Q4（拓展）**: 如果改成"K数之和最接近target"呢？
> 推广为 K-Sum 问题。外层 K-2 层循环固定前 K-2 个数，最内层用双指针。时间 O(n^(K-1))。这就是为什么 3Sum 用 O(n²)、4Sum 用 O(n³)。

## 相关题型

- **15. 三数之和 (3Sum)** — 框架完全一样（排序+固定一个+双指针），区别：15题找 `sum == 0` 且要去重收集所有结果，本题找 `abs(sum - target)` 最小只要一个值。掌握本题后做15题只需改判断条件和结果收集方式。
- **1. 两数之和 (Two Sum)** — 本题的子问题。固定第一个数后，内层就是"两数之和最接近"，复用双指针技巧。
- **18. 四数之和 (4Sum)** — 在本题基础上再套一层循环。代码改动：外层从1层变2层，其余不变。
- **259. 较小的三数之和 (3Sum Smaller)** — 同样的排序+双指针框架，区别在于统计 `sum < target` 的组数而不是找最接近的。