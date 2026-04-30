# 18. 四数之和 (4Sum)

## 核心思路

这道题本质上是 **3Sum 的扩展**：在数组中找出所有不重复的四元组，使得四个数之和等于 target。核心方法是**排序 + 外层两层循环固定两个数 + 内层双指针找剩余两个数**，将 O(n⁴) 暴力降到 O(n³)。

## 思维链

1. **读完题第一反应** → 四重循环枚举所有四元组，O(n⁴)，n=200 时约 1.6×10⁹，会超时。还得用 set 去重，很麻烦。

2. **回忆系列题的演进** → Two Sum 用哈希表 O(n)，3Sum 用排序+双指针 O(n²)。4Sum 能否沿用同样思路？

3. **关键突破：降维思想** → 3Sum 是"固定一个数 + 双指针找两个数"，4Sum 就是"固定两个数 + 双指针找两个数"。每多固定一层，就多一层循环，所以 kSum 的复杂度是 O(n^(k-1))。

4. **去重是难点** → 排序之后，相同元素相邻。在每一层循环中，如果当前数和前一个数相同，就跳过，避免重复四元组。

5. **剪枝加速** → 在每层循环中可以提前判断：
   - **最小值剪枝**：当前位置连续取最小的 k 个数之和已经 > target，后面不可能更小，直接 break
   - **最大值剪枝**：当前位置取最大的 k 个数之和已经 < target，当前数太小，跳过 continue

6. **溢出风险** → nums[i] 可达 10⁹，四个数相加可达 4×10⁹，超过 int 范围，必须用 long long。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 四重循环 + set 去重 | O(n⁴) | O(n) | 能说出即可 |
| 排序 + 双指针 | 两层循环固定前两数 + 双指针 | O(n³) | O(1)* | ⭐ 必须写出 |
| 通用 kSum 递归 | 递归拆解成 2Sum，适用于任意 k | O(n^(k-1)) | O(k) | 加分项 |

*不计输出空间

## 关键提示

1. **从 3Sum 出发**：如果你会 3Sum（排序 + 一层循环 + 双指针），4Sum 只是在外面再套一层循环，核心逻辑完全一样。

2. **去重三层都要做**：第一层 i 跳重复、第二层 j 跳重复、双指针层 left/right 找到解后也要跳重复。想想为什么每层都要，如果漏一层会怎样？

3. **剪枝很重要**：虽然 O(n³) 能过，但面试中展示剪枝意识是加分项。思考：什么情况下可以确定"从当前位置开始不可能有解"？

4. **溢出陷阱**：`nums[i] + nums[j] + nums[left] + nums[right]` 四个 int 相加可能溢出。用 `(long long)nums[i] + nums[j]` 还是先转 target？

5. **对比系列题的代码差异**：
   ```
   2Sum: 哈希表 O(n) 或 排序+双指针 O(n log n)
   3Sum: 1层循环 + 双指针 O(n²)
   4Sum: 2层循环 + 双指针 O(n³)
   kSum: k-2层循环 + 双指针 O(n^(k-1))
   ```

## 解法详解

### 解法1: 暴力枚举 — O(n⁴) / O(n)

**思考过程**: 最直接的想法——四重循环枚举所有可能的四元组，用 set 去重。

```cpp
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        int n = nums.size();
        sort(nums.begin(), nums.end()); // 排序方便去重
        set<vector<int>> resultSet;
        
        for (int a = 0; a < n; a++)
            for (int b = a + 1; b < n; b++)
                for (int c = b + 1; c < n; c++)
                    for (int d = c + 1; d < n; d++) {
                        // 用 long long 防溢出
                        long long sum = (long long)nums[a] + nums[b] + nums[c] + nums[d];
                        if (sum == target)
                            resultSet.insert({nums[a], nums[b], nums[c], nums[d]});
                    }
        
        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }
};
```

**关键点**: n=200 时 O(n⁴) ≈ 1.6×10⁹，会 TLE。但理解这个暴力是后续优化的基础——我们要消除最内两层循环。

---

### 解法2: 排序 + 双指针 — O(n³) / O(1) ⭐ 面试首选

**从解法1优化**: 暴力解最内两层循环其实在做"给定前两个数，在剩余数中找两个数使和等于 remainder"。这就是 Two Sum 问题！排序后用双指针可以 O(n) 解决，于是最内两层 O(n²) 降到 O(n)。

```
// 整体结构（对比 3Sum）:
// 
// 3Sum:  for i          → 双指针(left, right)    = O(n²)
//         ↑固定1个数      ↑找2个数
//
// 4Sum:  for i → for j  → 双指针(left, right)    = O(n³)
//         ↑固定2个数        ↑找2个数
//
// 去重逻辑完全一样：排序后跳过相邻重复元素
```

```
// 搜索过程示意 (nums = [-2,-1,0,0,1,2], target = 0):
//
// i=0 (nums[i]=-2), j=1 (nums[j]=-1):
//   remainder = 0-(-2)-(-1) = 3
//   left=2, right=5: nums[2]+nums[5] = 0+2 = 2 < 3  → left++
//   left=3, right=5: nums[3]+nums[5] = 0+2 = 2 < 3  → left++
//   left=4, right=5: nums[4]+nums[5] = 1+2 = 3 == 3 → 找到! [-2,-1,1,2]
//
// i=0 (nums[i]=-2), j=2 (nums[j]=0):
//   remainder = 0-(-2)-0 = 2
//   left=3, right=5: nums[3]+nums[5] = 0+2 = 2 == 2 → 找到! [-2,0,0,2]
//   left++ → left=4, right-- → right=4, left>=right 结束
//
// i=1 (nums[i]=-1), j=2 (nums[j]=0):
//   remainder = 0-(-1)-0 = 1
//   left=3, right=5: nums[3]+nums[5] = 0+2 = 2 > 1  → right--
//   left=3, right=4: nums[3]+nums[4] = 0+1 = 1 == 1 → 找到! [-1,0,0,1]
```

```cpp
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> result;
        int n = nums.size();
        if (n < 4) return result;
        
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 3; i++) {
            // 第一层去重：i > 0 且和前一个相同则跳过
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            // 剪枝1: 当前最小四数之和 > target，后面不可能更小
            if ((long long)nums[i] + nums[i+1] + nums[i+2] + nums[i+3] > target) break;
            // 剪枝2: 当前数 + 最大三个数 < target，当前数太小，跳过
            if ((long long)nums[i] + nums[n-3] + nums[n-2] + nums[n-1] < target) continue;
            
            for (int j = i + 1; j < n - 2; j++) {
                // 第二层去重：j > i+1 且和前一个相同则跳过
                if (j > i + 1 && nums[j] == nums[j - 1]) continue;
                
                // 剪枝3: 当前 i,j 固定后，最小两数之和 > target
                if ((long long)nums[i] + nums[j] + nums[j+1] + nums[j+2] > target) break;
                // 剪枝4: 当前 i,j 固定后，最大两数之和 < target
                if ((long long)nums[i] + nums[j] + nums[n-2] + nums[n-1] < target) continue;
                
                // 双指针找剩余两个数
                int left = j + 1, right = n - 1;
                while (left < right) {
                    long long sum = (long long)nums[i] + nums[j] + nums[left] + nums[right];
                    
                    if (sum < target) {
                        left++;
                    } else if (sum > target) {
                        right--;
                    } else {
                        // 找到一组解
                        result.push_back({nums[i], nums[j], nums[left], nums[right]});
                        
                        // 双指针层去重
                        while (left < right && nums[left] == nums[left + 1]) left++;
                        while (left < right && nums[right] == nums[right - 1]) right--;
                        
                        left++;
                        right--;
                    }
                }
            }
        }
        
        return result;
    }
};
```

**关键点**:
- 去重条件写对：`i > 0`、`j > i + 1`，不是 `j > 0`（否则会跳过合法的同值不同位组合）
- 剪枝用 `break` 还是 `continue` 要分清：最小和超标用 break（后面更大），最大和不够用 continue（当前太小，但后面的数可能行）

---

### 解法3: 通用 kSum 递归 — O(n^(k-1)) / O(k)

**思考过程**: 面试追问"如果变成 5Sum、6Sum 怎么办？" 我们把 kSum 写成递归：当 k>2 时固定一个数递归成 (k-1)Sum，当 k==2 时用双指针。

```cpp
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        return kSum(nums, target, 0, 4);
    }
    
private:
    vector<vector<int>> kSum(vector<int>& nums, long long target, int start, int k) {
        vector<vector<int>> result;
        int n = nums.size();
        
        // 边界：剩余元素不够
        if (start >= n || k < 2) return result;
        
        // base case: 2Sum 用双指针
        if (k == 2) {
            int left = start, right = n - 1;
            while (left < right) {
                long long sum = (long long)nums[left] + nums[right];
                if (sum < target) {
                    left++;
                } else if (sum > target) {
                    right--;
                } else {
                    result.push_back({nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                }
            }
            return result;
        }
        
        // 一般情况: 固定一个数，递归成 (k-1)Sum
        for (int i = start; i < n - k + 1; i++) {
            // 去重
            if (i > start && nums[i] == nums[i - 1]) continue;
            
            // 剪枝: 最小 k 个数之和 > target
            long long minSum = 0;
            for (int m = 0; m < k; m++) minSum += nums[i + m];
            if (minSum > target) break;
            
            // 剪枝: 当前数 + 最大 k-1 个数之和 < target
            long long maxSum = nums[i];
            for (int m = 1; m < k; m++) maxSum += nums[n - m];
            if (maxSum < target) continue;
            
            // 递归
            auto subResult = kSum(nums, target - nums[i], i + 1, k - 1);
            for (auto& sub : subResult) {
                sub.insert(sub.begin(), nums[i]); // 把当前数加到子结果前面
                result.push_back(sub);
            }
        }
        
        return result;
    }
};
```

**关键点**: 这个递归框架可以解决任意 kSum 问题。面试中如果写出这个，说明你理解了整个系列的本质——kSum 最终都归结为 2Sum。

---

## 解法对比

| | 暴力枚举 | 排序+双指针 | 通用kSum递归 |
|---|---|---|---|
| 时间 | O(n⁴) | O(n³) | O(n³) (k=4时) |
| 空间 | O(n) set去重 | O(1) | O(k) 递归栈 |
| 去重方式 | set自动去重 | 排序+跳过相邻重复 | 排序+跳过相邻重复 |
| 代码复杂度 | 简单 | 中等 | 较高但通用 |
| 面试推荐度 | 仅说思路 | ⭐ 首选 | 追问kSum时展示 |

## 易错点

1. **去重条件写错**
   - ✗ `if (j > 0 && nums[j] == nums[j-1]) continue;` — j 从 i+1 开始，这会跳过 j==i+1 且值相同的合法情况
   - ✓ `if (j > i + 1 && nums[j] == nums[j-1]) continue;` — 只跳过同一层内的重复

2. **整数溢出**
   - ✗ `int sum = nums[i] + nums[j] + nums[left] + nums[right];` — 四个 10⁹ 相加爆 int
   - ✓ `long long sum = (long long)nums[i] + nums[j] + nums[left] + nums[right];` — 第一个操作数转 long long，后续自动提升

3. **剪枝用 break 和 continue 搞混**
   - 最小和 > target → `break`（数组有序，后面只会更大，不可能回到 target）
   - 最大和 < target → `continue`（当前数太小，但下一个数更大可能够）
   - 搞反的话要么漏解要么失去剪枝效果

4. **双指针找到解后忘记同时移动两个指针**
   - ✗ 只 `left++` 不 `right--`，导致死循环或漏解
   - ✓ 跳重复后 `left++; right--;` 同时移动

## 面试追问

**Q1: 这道题和 3Sum 的关系？代码改了哪里？**
> 3Sum 是 1 层循环 + 双指针，4Sum 在外面再加 1 层循环。核心区别就是多了一层 `for j` 和对应的去重/剪枝。双指针的逻辑完全不变。

**Q2: 如果是 kSum 怎么办？能否写一个通用解？**
> 用递归：k > 2 时固定一个数递归成 (k-1)Sum，k == 2 时用双指针做 base case。时间复杂度 O(n^(k-1))。见解法 3。

**Q3: 为什么不用哈希表来做？3Sum 可以用哈希表，4Sum 呢？**
> 可以。固定两个数后用哈希表做 Two Sum，时间也是 O(n³)。但排序+双指针方案**不需要额外空间**，且去重更容易（排序后相邻跳过 vs 哈希表需要额外逻辑）。面试中双指针更受青睐。

**Q4: 剪枝对性能提升有多大？**
> 最坏情况不改变 O(n³)，但平均情况下大幅减少内层执行次数。尤其当数组大且 target 较小/较大时，外层循环很快 break。面试中展示剪枝意识是加分项。

## 相关题型

- **[1] Two Sum** — kSum 的 base case。哈希表 O(n) 或排序+双指针 O(n log n)。4Sum 最终归结为 2Sum。
- **[15] 3Sum** — 4Sum 的直接前置题。区别：少一层外循环。复用完全相同的"排序+去重+双指针"模板。代码差异：4Sum 比 3Sum 多一层 `for j` 循环和对应的去重/剪枝。
- **[16] 3Sum Closest** — 不找精确值而找最接近的和。复用双指针框架，但判断条件从 `== target` 改为维护 `minDiff`。
- **[454] 4Sum II** — 四个独立数组各取一个数求和为 0。不需要去重，用哈希表把 O(n⁴) 降到 O(n²)。和本题思路完全不同，对比学习。