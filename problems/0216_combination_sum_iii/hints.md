# 216. 组合总和 III

## 核心思路

本题本质上是：**从固定候选集 {1,2,...,9} 中，选出恰好 k 个不重复的数，使它们的和为 n**。这是一个经典的"组合 + 约束"问题，用回溯法在决策树上搜索所有满足条件的路径。

## 思维链

1. **读完题第一反应**：从 1~9 中选 k 个数，和为 n。候选集很小（只有9个数），最多选9个，暴力枚举所有子集也才 2^9 = 512 种，完全可以穷举。
2. **怎么穷举？** 每个数字有"选"或"不选"两个选择，这就是经典的子集/组合问题。可以用回溯法：从小到大依次考虑每个数字，选或不选。
3. **如何保证"恰好 k 个且和为 n"？** 在回溯过程中维护两个变量：当前已选个数和当前累加和。当已选 k 个时检查和是否为 n。
4. **能不能剪枝提前终止？** 当然可以！如果当前和已经 ≥ n 但还没选够 k 个，继续选只会更大，直接剪掉。如果剩余数字不够凑 k 个，也剪掉。
5. **还有别的方法吗？** 由于候选集只有9个元素，可以用**位掩码（bitmask）**枚举所有子集，检查大小为 k 且和为 n 的子集。代码更简洁，但不如回溯通用。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯（基础） | 从 1~9 中依次选数，递归+回溯 | O(C(9,k)) | O(k) | ⭐ 必须写出 |
| 回溯（剪枝优化） | 在基础回溯上加和剪枝+个数剪枝 | O(C(9,k)) 但常数更小 | O(k) | ⭐ 必须写出 |
| 位掩码枚举 | 枚举 2^9 个子集，筛选满足条件的 | O(2^9 × 9) | O(k) | 加分项 |

## 关键提示

1. **组合问题的核心去重技巧**：用 `start` 参数控制每次只能从上一个选的数字的下一个开始选，避免 [1,2,4] 和 [2,1,4] 重复。

2. **剪枝条件有两个维度**：
   - **个数剪枝**：如果剩余可选数字不够凑齐 k 个，提前返回
   - **和剪枝**：如果当前和已经 ≥ n，不需要继续往大的数字选

3. **决策树长什么样？** 以 k=3, n=7 为例：
```
//                          []  (sum=0, count=0)
//                /     |     \    ...
//              [1]    [2]    [3]  ...
//            / | \    / \     |
//        [1,2][1,3].. [2,3][2,4] [3,4]
//        / \    |      |     ✗
//    [1,2,3][1,2,4] [1,3,5] [2,3,4]
//     sum=6  sum=7✓  sum=9✗  sum=9✗
//      ✗      ✓
```

4. **和系列题的关系**：
   - LeetCode 77（组合）：从 1~n 中选 k 个，不限制和 → 本题多了"和为 n"的约束
   - LeetCode 39（组合总和）：数字可重复使用 → 本题每个数字最多用一次
   - LeetCode 40（组合总和 II）：候选集有重复元素 → 本题候选集固定为 1~9 无重复

5. **位掩码思路**：9位二进制数，第 i 位为 1 表示选了数字 i+1。遍历 0~511，检查 popcount == k 且对应数字之和 == n。

## 解法详解

### 解法1: 回溯（基础版） — O(C(9,k)) / O(k)

**思考过程**: 这是一个"从固定集合中选子集"的问题。回溯的思路是：从数字 1 开始，对每个数字决定"选"或"跳过"，选够 k 个时检查和。用 `start` 参数保证只向后选（不回头），天然去重。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(result, path, k, n, 1, 0);
        return result;
    }
    
    void backtrack(vector<vector<int>>& result, vector<int>& path,
                   int k, int n, int start, int currentSum) {
        // 已选够 k 个数
        if (path.size() == k) {
            if (currentSum == n) {
                result.push_back(path);
            }
            return; // 不管和对不对，都不再继续选
        }
        
        // 从 start 到 9，依次尝试选每个数字
        for (int i = start; i <= 9; i++) {
            path.push_back(i);             // 做选择
            backtrack(result, path, k, n, 
                      i + 1,               // 下一个从 i+1 开始（不重复使用）
                      currentSum + i);     // 累加和
            path.pop_back();               // 撤销选择
        }
    }
};
```

**关键点**: `start` 参数确保每次只往后选，[1,2,4] 不会产生 [2,1,4] 这样的重复。这是组合问题（而非排列问题）的标准套路。

---

### 解法2: 回溯（剪枝优化） — O(C(9,k)) / O(k) ⭐ 面试首选

**从解法1优化**: 解法1在选够 k 个之前不做任何判断，白白探索了很多不可能的分支。我们可以加两个剪枝：
- **和剪枝**: 如果 currentSum + i > n，后面的数字更大，不可能凑出 n，直接 break
- **个数剪枝**: 如果从当前 start 到 9 的数字不够凑齐剩余需要的个数，直接返回

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(result, path, k, n, 1, 0);
        return result;
    }
    
    void backtrack(vector<vector<int>>& result, vector<int>& path,
                   int k, int n, int start, int currentSum) {
        if (path.size() == k) {
            if (currentSum == n) {
                result.push_back(path);
            }
            return;
        }
        
        // 个数剪枝: 还需要 (k - path.size()) 个数，
        // 所以 i 最大只能到 9 - (k - path.size()) + 1
        int need = k - path.size();
        
        for (int i = start; i <= 9 - need + 1; i++) {
            // 和剪枝: 当前数已经让总和超过 n，后面更大的数更不行
            if (currentSum + i > n) break;
            
            path.push_back(i);
            backtrack(result, path, k, n, i + 1, currentSum + i);
            path.pop_back();
        }
    }
};
```

**关键点**: 
- `i <= 9 - need + 1` 是个数剪枝。比如还需要3个数，i 最多只能到7（选7,8,9）。
- `currentSum + i > n` 时直接 `break` 而不是 `continue`，因为 i 递增，后面只会更大。

---

### 解法3: 位掩码枚举 — O(2^9 × 9) / O(k)

**换一个角度**: 候选集只有 {1,...,9}，共 9 个元素，所有子集只有 2^9 = 512 个。用一个 9 位二进制数表示选哪些数字，逐一检查是否满足"恰好 k 个且和为 n"。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> result;
        
        // 枚举 0 到 (1<<9)-1 = 511 的所有子集
        for (int mask = 0; mask < (1 << 9); mask++) {
            // 检查这个子集是否恰好有 k 个元素
            if (__builtin_popcount(mask) != k) continue;
            
            // 计算子集的和，同时收集元素
            int sum = 0;
            vector<int> combo;
            for (int bit = 0; bit < 9; bit++) {
                if (mask & (1 << bit)) {
                    sum += bit + 1;       // 第 bit 位对应数字 bit+1
                    combo.push_back(bit + 1);
                }
            }
            
            if (sum == n) {
                result.push_back(combo);
            }
        }
        
        return result;
    }
};
```

**关键点**: 这种方法思路简单、不需要递归，但不具通用性——只有候选集很小时才能用。面试中写完回溯后可以提一嘴"因为搜索空间只有 512，也可以位掩码暴力"作为加分。

---

## 解法对比

| | 回溯（基础） | 回溯（剪枝） | 位掩码 |
|---|---|---|---|
| **时间** | O(C(9,k)) | O(C(9,k)) 常数更小 | O(512 × 9) |
| **空间** | O(k) 递归栈 | O(k) 递归栈 | O(k) |
| **通用性** | 高，可推广到任意候选集 | 高，同上 | 低，只适用于小集合 |
| **面试推荐** | ✓ 作为起步 | ⭐ 首选 | 提一嘴即可 |
| **代码难度** | 简单 | 简单 | 最简单 |

回溯法是面试考察的核心——面试官想看你对"选择→递归→撤销"模式的掌握程度，以及能否主动提出剪枝。

## 易错点

1. **✗ `for` 循环上界写成 `i <= 9` 不做个数剪枝**
   → ✓ `i <= 9 - (k - path.size()) + 1`
   不加也能 AC（候选集小），但面试中面试官会追问"能不能优化循环范围"。

2. **✗ 和剪枝用 `continue` 而不是 `break`**
   ```cpp
   // 错误：跳过当前但继续检查更大的数
   if (currentSum + i > n) continue;  // ✗
   // 正确：i 递增，后面不可能更小，直接终止
   if (currentSum + i > n) break;     // ✓
   ```

3. **✗ 递归时传 `start` 而不是 `i + 1`**
   ```cpp
   backtrack(..., start + 1, ...);  // ✗ 会漏选某些组合
   backtrack(..., i + 1, ...);      // ✓ 下一个从当前选的数的下一个开始
   ```
   `start + 1` 意味着每层递归只尝试下一个起始值，但 `i` 在循环中是变化的，应该基于 `i` 推进。

4. **✗ 忘记在 `path.size() == k` 时 return**
   如果不 return，会继续往 path 里加数字，导致组合长度超过 k。

## 面试追问

**Q1: 基础理解 — 为什么用 `start` 参数而不是 `visited` 数组？**
> 因为我们要求的是**组合**（无序），不是**排列**（有序）。`start` 保证只向后选，天然避免重复。排列题（如 LeetCode 46）才需要 `visited` 数组。

**Q2: 优化 — 你能说出几种剪枝？各自的原理是什么？**
> 两种：(1) **个数剪枝**：剩余可选数字不够凑齐 k 个时提前终止循环；(2) **和剪枝**：当前累加和已超过目标 n 时 break（因为后面的数字更大）。两种剪枝都是利用了 1~9 递增的性质。

**Q3: 变体 — 如果候选数字不是 1~9，而是任意给定的无重复数组，怎么改？**
> 先对候选数组排序，然后把 `for (int i = start; i <= 9; ...)` 改为 `for (int i = start; i < nums.size(); ...)`，`currentSum + i` 改为 `currentSum + nums[i]`。排序是为了让和剪枝（break）生效。这就变成了 LeetCode 40 的简化版。

**Q4: 变体 — 如果每个数字可以重复使用呢？**
> 递归时传 `i` 而不是 `i + 1`，表示同一个数字可以再次选择。这就是 LeetCode 39（组合总和 I）的做法。

## 相关题型

- **LeetCode 77 组合** — 复用完全相同的回溯框架，区别：77 只限制个数为 k，不限制和。本题在 77 的基础上多了 `currentSum == n` 的检查。
- **LeetCode 39 组合总和** — 区别：39 允许重复使用元素，所以递归传 `i` 而不是 `i + 1`；候选集任意而非 1~9。
- **LeetCode 40 组合总和 II** — 区别：40 的候选集有重复元素，需要排序后用 `if (i > start && nums[i] == nums[i-1]) continue` 去重。本题候选集天然无重复，不需要此步骤。
- **LeetCode 78 子集** — 复用同样的回溯框架，区别：78 收集所有节点（不只是叶子），没有个数和总和的限制。