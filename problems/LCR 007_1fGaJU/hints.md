# LCR 007. 三数之和

## 核心思路

**排序 + 双指针**。先对数组排序，然后枚举第一个数 `nums[i]`，在 `i` 右侧使用双指针 `left` 和 `right` 从两端向中间夹逼，寻找满足 `nums[i] + nums[left] + nums[right] == 0` 的三元组。排序的双重好处：(1) 双指针利用有序性快速收缩搜索范围；(2) 去重只需跳过连续相同元素。

> **同主站 15. 3Sum**，面试高频中的高频，排序+双指针是经典模板。

## 思维链

1. **读完题第一反应** → 找出数组中所有**不重复**的三元组 `(a, b, c)` 使得 `a + b + c = 0`。最朴素做法？三重循环暴力枚举 O(n^3)，但太慢且去重困难。

2. **暴力做法的瓶颈** → 三重循环 O(n^3) 时间，去重需要用 `set<vector<int>>` 额外 O(n) 空间且常数大。能不能利用"和为 0"的结构来优化？

3. **关键洞察 — 降维到 Two Sum** →
   - 固定第一个数 `nums[i]`，问题变成"在剩余数组中找两个数使和为 `-nums[i]`"——这就是 **Two Sum** 的变体
   - Two Sum 如果数组**有序**，可以用双指针 O(n) 解决，且去重只需跳过连续相同元素
   - 排序代价 O(n log n)，不影响总体 O(n^2) 复杂度

4. **双指针的工作原理** →
```
排序后: [-4, -1, -1, 0, 1, 2]

固定 i=0, nums[i]=-4, target=4:
  left=1(-1), right=5(2): sum=1 < 4  → left++ (和太小，需要更大的数)
  left=2(-1), right=5(2): sum=1 < 4  → left++
  left=3(0),  right=5(2): sum=2 < 4  → left++
  left=4(1),  right=5(2): sum=3 < 4  → left++
  left=5 >= right=5 → 结束，没找到

固定 i=1, nums[i]=-1, target=1:
  left=2(-1), right=5(2): sum=1 == 1  ✓ → 收录 [-1,-1,2]
    left 跳重复→3, right 跳重复→4
  left=3(0), right=4(1): sum=1 == 1  ✓ → 收录 [-1,0,1]
    left++→4, right--→3
  left=4 >= right=3 → 结束

i=2, nums[2]=-1 == nums[1]=-1 → 跳过（外层去重）
i=3, nums[3]=0 > 0 → 三个非负数不可能和为0 → break

结果: [[-1,-1,2], [-1,0,1]]
```

5. **去重的三个关键位置** →
   - **外层去重**: `if (i > 0 && nums[i] == nums[i-1]) continue;` — 跳过重复的第一个数
   - **内层去重**（找到一组解后）: left 向右跳过相同值，right 向左跳过相同值
   - **提前退出**: `if (nums[i] > 0) break;` — 排序后第一个数都 > 0，后面不可能和为 0

6. **为什么外层去重用 `nums[i] == nums[i-1]` 而不是 `nums[i] == nums[i+1]`？** →
```
反例: nums = [-1, -1, 0, 1, 2]

如果用 nums[i] == nums[i+1] 去重:
  i=0, nums[0]=-1 == nums[1]=-1 → 跳过！
  → 错过了 [-1, -1, 2] 这个合法三元组

正确: 用 nums[i] == nums[i-1] 去重:
  i=0, i > 0? No → 不跳过 → 正常处理
  i=1, nums[1]=-1 == nums[0]=-1 → 跳过（i=0 已经搜索过所有以 -1 开头的三元组）
```
核心逻辑：向前看 `nums[i-1]` 意味着"以这个值开头的三元组已经在之前被搜索过了"。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力三重循环 | 枚举所有三元组 + set 去重 | O(n^3) | O(n) | 能说出即可 |
| 排序 + 双指针 | 固定一个数，双指针找另两个 | O(n^2) | O(1) | ⭐ 必须写出 |
| 排序 + 哈希表 | 固定一个数，哈希表找第三个 | O(n^2) | O(n) | 备选方案 |
| 通用 kSum 框架 | 递归降维到 2Sum | O(n^{k-1}) | O(k) | 扩展到 4Sum 时讲 |

## 关键提示

1. **双指针移动方向**：`sum < 0` → left 右移（需要更大的数使和变大）；`sum > 0` → right 左移（需要更小的数使和变小）。有序数组中，左移增大、右移减小。

2. **去重是本题最易错的地方**。外层 `i` 去重必须用 `nums[i] == nums[i-1]`（向前看），不是 `nums[i] == nums[i+1]`（向后看）。向后看会跳过合法解。

3. **`nums[i] > 0` 时直接 break**。排序后 `i` 右边的数都 >= `nums[i]`，三个正数之和不可能为 0。这是一个关键的**剪枝优化**。

4. **找到一组解后 left 和 right 都要跳过重复值再移动**。否则 `[-2, 0, 0, 2, 2]` 中 `[-2, 0, 2]` 会被重复收录。

5. **空数组和长度 < 3 的数组**需要特判，直接返回空结果。

```
去重的三处位置总结:

for i in [0, n-3]:
    if i > 0 && nums[i] == nums[i-1]:  ← (1) 外层去重
        continue
    
    if nums[i] > 0: break              ← 剪枝: 三正数不可能和为0
    
    left = i+1, right = n-1
    while left < right:
        sum = nums[i] + nums[left] + nums[right]
        if sum == 0:
            记录结果
            while left<right && nums[left]==nums[left+1]: left++   ← (2) 内层去重
            while left<right && nums[right]==nums[right-1]: right-- ← (3) 内层去重
            left++; right--
        elif sum < 0: left++
        else: right--
```

## 解法详解

### 解法1: 排序 + 双指针 — O(n^2) / O(1) ⭐ 面试首选

**思考过程**: 把三数之和拆解为：枚举第一个数 `nums[i]`，在 `[i+1, n-1]` 范围内用双指针找两数之和为 `-nums[i]`。排序保证了双指针的单调性和去重的便利性。

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> result;
        
        // 特判: 少于3个元素不可能有三元组
        if (n < 3) return result;
        
        // Step 1: 排序——为双指针和去重做准备
        sort(nums.begin(), nums.end());
        
        // Step 2: 枚举第一个数 nums[i]
        for (int i = 0; i < n - 2; i++) {
            // 剪枝: 排序后 nums[i] > 0，后面都 >= nums[i]，三正数和 > 0
            if (nums[i] > 0) break;
            
            // 外层去重: 跳过与前一个相同的值（向前看！）
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            // Step 3: 双指针找两数之和 = -nums[i]
            int left = i + 1, right = n - 1;
            int target = -nums[i];
            
            while (left < right) {
                int sum = nums[left] + nums[right];
                
                if (sum == target) {
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // 内层去重: left 跳过连续相同值
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    // 内层去重: right 跳过连续相同值
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    
                    left++;
                    right--;
                } else if (sum < target) {
                    left++;   // 和太小，左指针右移
                } else {
                    right--;  // 和太大，右指针左移
                }
            }
        }
        
        return result;
    }
};
```

**去重正确性证明**:
- **外层**: `nums[i] == nums[i-1]` 时跳过。因为以 `nums[i-1]` 为首的所有三元组在上一轮已经被完整搜索过了。跳过不会漏解。
- **内层**: 找到 `(nums[i], nums[left], nums[right])` 后，如果 left 右边值相同或 right 左边值相同，组成的三元组也相同，必须跳过。

---

### 解法2: 排序 + 哈希表 — O(n^2) / O(n)

**思考过程**: 固定前两个数，用哈希表 O(1) 查找第三个数。但去重比双指针复杂。

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> result;
        if (n < 3) return result;
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 2; i++) {
            if (nums[i] > 0) break;
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            unordered_set<int> seen;
            for (int j = i + 1; j < n; j++) {
                int complement = -nums[i] - nums[j];
                if (seen.count(complement)) {
                    result.push_back({nums[i], complement, nums[j]});
                    // 跳过重复的 nums[j]
                    while (j + 1 < n && nums[j] == nums[j + 1]) j++;
                }
                seen.insert(nums[j]);
            }
        }
        return result;
    }
};
```

**与双指针对比**: 双指针不需要额外空间且去重更自然，面试中更受青睐。哈希表法作为思路拓展了解即可。

---

### 解法3: 通用 kSum 框架 — 面试加分

**思考过程**: 如果面试官追问 4Sum / kSum，可以展示递归降维的通用框架。

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        return kSum(nums, 0, 0, 3);
    }
    
private:
    vector<vector<int>> kSum(vector<int>& nums, long long target, int start, int k) {
        vector<vector<int>> result;
        int n = nums.size();
        
        if (k == 2) {
            // 基本情况: Two Sum 用双指针
            int left = start, right = n - 1;
            while (left < right) {
                long long sum = (long long)nums[left] + nums[right];
                if (sum == target) {
                    result.push_back({nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++; right--;
                } else if (sum < target) left++;
                else right--;
            }
            return result;
        }
        
        // 递归情况: 固定一个数，降维到 (k-1)Sum
        for (int i = start; i < n - k + 1; i++) {
            if (i > start && nums[i] == nums[i - 1]) continue;
            // 剪枝
            if ((long long)nums[i] * k > target) break;
            if ((long long)nums[i] + (long long)nums[n-1] * (k-1) < target) continue;
            
            auto sub = kSum(nums, target - nums[i], i + 1, k - 1);
            for (auto& t : sub) {
                t.insert(t.begin(), nums[i]);
                result.push_back(t);
            }
        }
        return result;
    }
};
```

## 解法对比

| | 暴力 | 排序+双指针 | 排序+哈希表 | kSum框架 |
|---|---|---|---|---|
| 时间 | O(n^3) | **O(n^2)** | O(n^2) | O(n^{k-1}) |
| 空间 | O(n) set去重 | **O(1)** | O(n) | O(k) 递归 |
| 去重难度 | 极难 | **简单** | 中等 | 简单 |
| 面试推荐 | 说思路 | ⭐ 必写 | 备选 | 追问时展示 |

## 易错点

1. **外层去重方向写错**：
   - ✗ `if (nums[i] == nums[i+1]) continue;` → 跳过 `[-1,-1,2]` 合法解
   - ✓ `if (i > 0 && nums[i] == nums[i-1]) continue;` → 向前看，安全

2. **内层去重遗漏**：
   - ✗ 找到解后直接 `left++; right--;` 不跳重复 → `[-2,0,0,2,2]` 中 `[-2,0,2]` 重复收录
   - ✓ 找到解后先 while 跳重复值，再 `left++; right--`

3. **忘记 `nums[i] > 0` 剪枝**：
   - 不影响正确性但影响效率。排序后 `nums[i] > 0` 意味着后续所有三元组之和 > 0，不可能为 0。

4. **边界条件遗漏**：
   - ✗ 不检查 `n < 3` → 空数组或 `[0]` 时数组越界
   - ✓ 开头加 `if (n < 3) return {};`

5. **外层循环范围**：
   - ✗ `for (int i = 0; i < n; i++)` → 当 i = n-1 或 n-2 时 left/right 无有效范围
   - ✓ `for (int i = 0; i < n - 2; i++)` → 确保至少有 3 个元素

## 面试追问

**Q1: "暴力能过吗？瓶颈在哪？"**
> n <= 3000, O(n^3) = 2.7 * 10^10，肯定 TLE。排序+双指针 O(n^2) = 9 * 10^6，轻松通过。

**Q2: "4Sum / kSum 怎么办？"**
> 递归降维：kSum 固定一个数后变成 (k-1)Sum，直到 2Sum 用双指针。总时间 O(n^{k-1})。展示上面的 kSum 框架代码。

**Q3: "为什么去重用向前看？"**
> 向后看 `nums[i]==nums[i+1]` 会跳过像 `[-1,-1,2]` 这样的合法解（i=0 时两个 -1 相邻就被跳过了）。向前看 `nums[i]==nums[i-1]` 表示"以这个值开头的搜索已经做过了"，不会漏解。

**Q4: "能否不排序？"**
> 可以用哈希表，但去重极其复杂。需要对每个三元组排序后存入 `set<vector<int>>` 去重，空间 O(n) 常数巨大。不推荐。

**Q5: "双指针为什么不会漏解？"**
> 反证法：假设答案 (a, b)，a 在 left 位置时 right 已经越过 b。这说明之前某一步 left <= a 且 right = b 时和 > target，即 left 的值 + b > target。但 left <= a 意味着 left 的值 <= a，则 a + b >= left值 + b > target，矛盾。

## 复杂度分析

- **时间**: O(n^2) — 外层循环 O(n) * 内层双指针 O(n)，排序 O(n log n) 被吸收
- **空间**: O(1) — 不计排序使用的栈空间和输出空间；如果算输出则为 O(答案数量)

## 相关题目

| 题目 | 关联 |
|------|------|
| [1. 两数之和](https://leetcode.cn/problems/two-sum/) | 本题的基础——哈希表查找配对 |
| [15. 三数之和](https://leetcode.cn/problems/3sum/) | 本题的主站原题，完全相同 |
| [16. 最接近的三数之和](https://leetcode.cn/problems/3sum-closest/) | 不要求精确等于 0，找最接近 target 的和 |
| [18. 四数之和](https://leetcode.cn/problems/4sum/) | 四数之和，递归降维到本题 |
| [167. 两数之和 II](https://leetcode.cn/problems/two-sum-ii-input-array-is-sorted/) | 有序数组双指针基础，本题的内层操作 |
| [LCR 006. 两数之和 II](https://leetcode.cn/problems/kLl5u1/) | 排序数组版 Two Sum |
