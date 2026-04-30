# 491. 非递减子序列

## 核心思路

本题本质上是：**在不能排序的前提下，枚举所有长度≥2的非递减子序列，并去重。**

经典的子集/组合去重技巧（先排序再跳过相邻相同元素）在这里**不能用**，因为排序会破坏原数组的相对顺序，导致非递减子序列的结果改变。因此去重策略需要另辟蹊径：**同一层递归中，相同值的元素只选一次。**

## 思维链

1. **读完题第一反应**：要找所有非递减子序列——这是一个"选/不选"的子集枚举问题。暴力做法：枚举所有 2^n 个子集，筛选出非递减且长度≥2的，用 set 去重。

2. **暴力解的瓶颈**：枚举所有 2^n 子集后用 set<vector<int>> 去重，虽然 n≤15 能过，但 set 的比较开销大，且不够优雅。能否在枚举过程中就避免产生重复？

3. **回溯框架**：这道题天然适合回溯——从左到右依次决定每个位置"选不选"。关键问题是：不能排序，怎么去重？

4. **同层去重的核心洞察**：在回溯树的**同一层**（即同一个递归调用中的 for 循环），如果两个不同位置的元素值相同，选后面那个得到的子序列一定是选前面那个的子集。所以**同层中相同值只选第一个**。用一个局部 set/数组记录当前层已选过的值即可。

5. **非递减约束**：递归时维护"当前子序列的最后一个元素"，只有 nums[i] >= last 时才能选。

6. **收集时机**：只要当前 path 长度 ≥ 2，就收入结果。不需要等到递归末尾。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 同层 set 去重 | 每层用 unordered_set 记录已选值 | O(n·2^n) | O(n) | ⭐ 必须写出 |
| 回溯 + 数组哈希去重 | 利用值域 [-100,100] 用数组替代 set | O(n·2^n) | O(n) | ⭐ 最优实现 |
| 位掩码枚举 + 哈希去重 | 枚举所有 2^n 子集，过滤+去重 | O(n·2^n) | O(n·2^n) | 能说出即可 |

## 关键提示

1. **不能排序！** 这和 LeetCode 90（子集 II）的最大区别。排序后 `[4,4,3,2,1]` 变成 `[1,2,3,4,4]`，非递减子序列完全不同了。

2. **同层去重 vs 同枝去重**：去重发生在回溯树的**同一层**（同一个 for 循环内），不是同一条路径上。路径上可以有重复值（如 `[7,7]`），但同层不能从两个不同位置选出相同值。

3. **决策树长这样**：
```
// nums = [4, 6, 7, 7]
//
// 起点 start=0:
//              []
//        /     |      \       \
//      [4]    [6]    [7]    [7]← 同层重复，跳过!
//     / | \    | \     |
//  [4,6][4,7][4,7] [6,7][6,7] [7,7]
//       ↑同层重复跳过↑   ↑跳过↑
//   ...
```

4. **值域有限**：`-100 ≤ nums[i] ≤ 100`，共 201 个值，可以用 `bool used[201]` 替代 set，更快。

5. **收集结果的时机**：不是只在叶子节点收集，而是在 `path.size() >= 2` 时随时收集。

## 解法详解

### 解法1: 回溯 + unordered_set 同层去重 — O(n·2^n) / O(n) ⭐ 面试首选

**思考过程**：

枚举子序列 → 经典回溯框架。对于每个位置 `start`，从 `start` 到 `n-1` 遍历，决定下一个要加入 path 的元素。两个约束：
- **非递减**：`nums[i] >= path.back()`（或 path 为空）
- **去重**：同一层 for 循环中，相同值只选一次

为什么 unordered_set 是局部变量？因为去重只在"同一层"生效。每次进入一个新的递归函数，就是新的一层，用新的 set。

```cpp
class Solution {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }
    
    void backtrack(vector<int>& nums, int start, vector<int>& path, 
                   vector<vector<int>>& result) {
        // 只要 path 长度 >= 2，就收入结果（不是只在叶子节点）
        if (path.size() >= 2) {
            result.push_back(path);
            // 注意：这里不 return，因为还可以继续延长子序列
        }
        
        // 同层去重：记录当前层已经选过哪些值
        unordered_set<int> used;
        
        for (int i = start; i < nums.size(); i++) {
            // 非递减约束：当前值必须 >= path 的最后一个元素
            if (!path.empty() && nums[i] < path.back()) continue;
            
            // 同层去重：这个值在当前层已经选过了
            if (used.count(nums[i])) continue;
            
            used.insert(nums[i]);  // 标记当前层选过这个值
            
            path.push_back(nums[i]);
            backtrack(nums, i + 1, path, result);  // 从 i+1 开始（子序列不能复用同一位置）
            path.pop_back();  // 撤销选择
        }
    }
};
```

**关键点**：
- `used` 是**局部变量**，每层递归有自己的 `used`，退出时自动销毁。
- 收集结果后**不 return**，子序列可以继续延长。
- `start = i + 1` 保证子序列中元素的索引严格递增（不重复使用同一位置）。

### 解法2: 回溯 + 数组哈希去重 — O(n·2^n) / O(n) ⭐ 最优实现

**从解法1优化**：解法1 每层创建 unordered_set 有额外开销。由于 `nums[i] ∈ [-100, 100]`，共 201 个值，可以用一个定长数组 `bool used[201]` 替代，通过 `nums[i] + 100` 映射到 `[0, 200]`。

```cpp
class Solution {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }
    
    void backtrack(vector<int>& nums, int start, vector<int>& path,
                   vector<vector<int>>& result) {
        if (path.size() >= 2) {
            result.push_back(path);
        }
        
        // 用数组替代 set，利用值域有限的特性
        bool used[201] = {false};  // 值域 [-100, 100] → 索引 [0, 200]
        
        for (int i = start; i < nums.size(); i++) {
            if (!path.empty() && nums[i] < path.back()) continue;
            if (used[nums[i] + 100]) continue;  // +100 偏移到非负索引
            
            used[nums[i] + 100] = true;
            path.push_back(nums[i]);
            backtrack(nums, i + 1, path, result);
            path.pop_back();
        }
    }
};
```

### 解法3: 位掩码枚举 — O(n·2^n) / O(n·2^n)

**思路**：n ≤ 15，可以用 2^n 个二进制掩码枚举所有子集。对每个子集检查：长度≥2 且非递减。用 set<vector<int>> 去重。

```cpp
class Solution {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        int n = nums.size();
        set<vector<int>> resultSet;  // 自动去重
        
        for (int mask = 0; mask < (1 << n); mask++) {
            vector<int> sub;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    sub.push_back(nums[i]);
                }
            }
            // 检查长度 >= 2 且非递减
            if (sub.size() < 2) continue;
            bool valid = true;
            for (int i = 1; i < sub.size(); i++) {
                if (sub[i] < sub[i - 1]) {
                    valid = false;
                    break;
                }
            }
            if (valid) resultSet.insert(sub);
        }
        
        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }
};
```

**关键点**：这个解法概念最简单但效率最低，`set<vector<int>>` 的插入和比较都是 O(n)，总体约 O(n²·2^n)。面试中说出思路即可，实际编写推荐解法2。

## 解法对比

| | 回溯 + set | 回溯 + 数组 | 位掩码枚举 |
|---|---|---|---|
| 去重方式 | 每层 unordered_set | 每层 bool[201] | 全局 set<vector> |
| 时间常数 | 中等（哈希开销） | 最小 | 最大（set比较） |
| 空间 | O(n) 递归栈 | O(n) 递归栈 | O(n·2^n) 存所有子集 |
| 面试推荐 | ✓ 通用 | ✓ 值域有限时最优 | 作为暴力思路说明 |

**什么时候选哪个**：
- 值域有限（如本题 [-100,100]）→ 数组哈希最快
- 值域很大或不确定 → unordered_set
- 只需要快速写出能过的代码 → 位掩码（但要说清楚复杂度劣势）

## 易错点

1. **✗ 尝试排序后去重**：
   ```cpp
   sort(nums.begin(), nums.end());  // 错！破坏原顺序
   if (i > start && nums[i] == nums[i-1]) continue;  // 子集II的去重法
   ```
   → **✓ 本题不能排序**，必须用同层 set/数组去重。因为排序后 `[4,4,3,2,1]` 变成 `[1,2,3,4,4]`，非递减子序列完全不同。

2. **✗ 收集结果时加 return**：
   ```cpp
   if (path.size() >= 2) {
       result.push_back(path);
       return;  // 错！提前返回导致漏掉更长的子序列
   }
   ```
   → **✓ 不加 return**，`[4,6]` 收集后还要继续探索 `[4,6,7]` 和 `[4,6,7,7]`。

3. **✗ 把 used 定义在函数外或回溯后重置**：
   ```cpp
   // 错误：在类成员变量中定义 used，被所有层共享
   unordered_set<int> used;  // 类成员
   void backtrack(...) {
       // used 被所有递归层共享，导致不同层之间互相干扰
   }
   ```
   → **✓ `used` 必须是每层递归的局部变量**，每层有独立的去重记录。

4. **✗ 数组去重时忘记偏移**：
   ```cpp
   used[nums[i]] = true;  // 错！nums[i] 可能是负数，数组越界
   ```
   → **✓ `used[nums[i] + 100] = true`**，将 [-100, 100] 映射到 [0, 200]。

## 面试追问

**Q1（基础理解）**：这道题能不能先排序再用子集 II 的去重方法？
> 不能。排序会改变元素的相对顺序，而"非递减子序列"依赖原始顺序。比如 `[4,4,3,2,1]` 排序后变成 `[1,2,3,4,4]`，会多出大量原本不存在的递增子序列。

**Q2（去重深入）**：同层去重的 set 为什么是局部变量而不是全局？能否用"回溯后重置"的方式？
> 因为去重的语义是"在同一个选择分支的同一层（for 循环）中不选相同值"。不同层之间的选择互不影响。如果用全局 set + 回溯重置，那进入子递归后又清除了标记，会导致同层去重失效。局部变量天然在函数返回后销毁，正好是我们要的生命周期。

**Q3（变体）**：如果要求返回的是严格递增子序列（不允许相等），怎么改？
> 只需把 `nums[i] < path.back()` 改为 `nums[i] <= path.back()`——即相等时也跳过。代码只改一个比较运算符。

**Q4（复杂度分析）**：最坏情况下结果有多少个？
> 当数组本身就是非递减的（如 `[1,2,3,...,15]`），所有长度 ≥ 2 的子集都是答案，共 2^15 - 15 - 1 = 32752 个。

## 相关题型

- **78. 子集** — 本题的基础框架，区别：78 无去重、无非递减约束
- **90. 子集 II** — 去重方式对比：90 可以排序后 `nums[i]==nums[i-1]` 跳过，本题不能排序所以用同层 set
- **47. 全排列 II** — 同样涉及"含重复元素的去重"，但排列可以排序后用 `used[i-1]` 去重
- **300. 最长递增子序列 (LIS)** — 如果只求最长长度而非枚举所有，用 DP/二分 O(n log n) 解决，复用"非递减"的概念但方法完全不同