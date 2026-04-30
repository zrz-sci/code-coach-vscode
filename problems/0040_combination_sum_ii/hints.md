# 40. 组合总和 II

## 核心思路

本题本质上是一个 **含重复元素的组合枚举问题**：从一个可能有重复数字的数组中，选若干个数（每个数最多用一次），使得它们的和等于 target，且结果集合不能有重复组合。

## 思维链

1. **读完题第一反应**：这是一个"选/不选"问题 → 每个元素要么选、要么不选 → 暴力枚举所有子集，检查和是否等于 target。但 2^100 的子集数爆炸了。

2. **回溯框架**：不需要枚举所有子集再判断，可以用回溯在搜索过程中累加 sum，当 sum > target 时直接剪枝。这就是 **DFS + 剪枝**。

3. **新的瓶颈 — 去重**：candidates 里有重复数字（如 `[1,1,2]`），不去重的话，选第一个 1 和选第二个 1 会产生相同的组合。怎么避免？

4. **去重核心思想**：先排序，让相同的数字相邻。在同一层递归中（即同一个位置的选择），如果当前数字和前一个相同，就跳过。这保证了"对于相同的数字，只有第一个会被选中开启新分支"。

5. **与 39 题（Combination Sum）的关键区别**：39 题每个数字可以无限使用，所以递归时 `start` 不变；40 题每个数字只能用一次，递归时 `start = i + 1`。另外 40 题有重复元素需要去重，39 题没有。

6. **优化剪枝**：排序后，如果 `candidates[i] > remain`，后面的数只会更大，直接 break 整个循环。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 排序去重 | 排序后回溯，同层跳过重复元素 | O(2^n) | O(n) 递归栈 | ⭐ 必须写出 |
| 回溯 + 计数去重 | 用哈希表统计频次，按"选几个"来递归 | O(2^n) | O(n) | 加分项 |

> 注：时间复杂度最坏情况下是 O(2^n)，但排序+剪枝在实际中大幅减少搜索空间。

## 关键提示

1. **为什么要排序？** 排序不只是为了剪枝（提前终止），更关键的是让相同元素相邻，这是去重的前提条件。

2. **去重条件 `i > start && candidates[i] == candidates[i-1]`** — 注意是 `i > start` 不是 `i > 0`。想一想：为什么用 `i > start`？`start` 代表什么含义？

3. **剪枝的位置**：`if (candidates[i] > remain) break;` 为什么用 `break` 而不是 `continue`？因为数组已排序，后面的数只会更大。

4. **与 39 题对比**：递归调用时传 `i + 1`（不是 `i`），因为每个元素只能用一次。

5. **决策树示意** — 以 `candidates = [1,1,2], target = 3` 为例：
```
//                        []  remain=3
//                  /      |       \
//               [1]      [1]✗    [2]
//             /    \      ↑跳过    |
//          [1,1]  [1,2]         (remain=1, 无可选)
//            |      ↑找到!
//         [1,1,2]✗超了
//           ↑remain<0
//
//  标 ✗ 的 [1] 分支被去重跳过（i>start 且 candidates[1]==candidates[0]）
//  结果: [[1,2]]
```

## 解法详解

### 解法1: 排序 + 回溯去重 — O(2^n) / O(n) ⭐ 面试首选

**思考过程**：

这道题和 39 题的框架几乎一样，核心区别是：
- 每个数只能用一次 → 递归时 `start = i + 1`
- 有重复数字 → 先排序，同一层跳过重复

"同一层跳过重复"的含义：在某一次递归调用中，我们要从 `start` 到 `n-1` 中选一个数。如果 `candidates[i] == candidates[i-1]` 且 `i > start`，说明前一个相同的数已经尝试过了，再选当前数会产生重复组合。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        sort(candidates.begin(), candidates.end()); // 排序：去重+剪枝的前提
        backtrack(candidates, target, 0, path, result);
        return result;
    }
    
private:
    void backtrack(vector<int>& candidates, int remain, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        if (remain == 0) {
            result.push_back(path);
            return;
        }
        
        for (int i = start; i < candidates.size(); i++) {
            // 剪枝：排序后，当前数已大于剩余目标，后面的更大，直接终止
            if (candidates[i] > remain) break;
            
            // 去重：同一层中，跳过和前一个相同的数字
            // i > start 表示不是本层第一个选择（第一个不跳）
            if (i > start && candidates[i] == candidates[i - 1]) continue;
            
            path.push_back(candidates[i]);
            // 关键：传 i+1 而不是 i，因为每个元素只能用一次
            backtrack(candidates, remain - candidates[i], i + 1, path, result);
            path.pop_back(); // 撤销选择
        }
    }
};
```

**关键点**：
- `i > start`（不是 `i > 0`）：`start` 是本层选择的起点，`i > start` 意味着"不是本层第一个候选"
- `break` vs `continue`：因为已排序，`break` 直接终止整个循环；`continue` 只跳过当前重复项

### 解法2: 计数法回溯 — O(2^n) / O(n)

**从另一个角度去重**：不依赖排序后跳过，而是先统计每个数字出现的次数，然后对每个不重复的数字决定"选几个"（0个、1个、...、count个）。

这个思路的好处是去重逻辑更直观——根本不会产生重复，因为我们是按"数字种类"而非"数字下标"来选择的。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        // 统计每个数字出现次数
        map<int, int> counter; // 用 map 保证有序
        for (int c : candidates) counter[c]++;
        
        // 转为 (数字, 次数) 的列表
        vector<pair<int, int>> nums(counter.begin(), counter.end());
        
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, target, 0, path, result);
        return result;
    }
    
private:
    void backtrack(vector<pair<int, int>>& nums, int remain, int idx,
                   vector<int>& path, vector<vector<int>>& result) {
        if (remain == 0) {
            result.push_back(path);
            return;
        }
        if (idx == nums.size()) return;
        
        int num = nums[idx].first;
        int maxCount = nums[idx].second;
        
        // 对当前数字，选 0 个、1 个、...、min(maxCount, remain/num) 个
        // 先递归"选 0 个"的情况（直接跳到下一种数字）
        backtrack(nums, remain, idx + 1, path, result);
        
        // 再递归"选 1~k 个"的情况
        for (int count = 1; count <= maxCount && count * num <= remain; count++) {
            path.push_back(num);
            backtrack(nums, remain - count * num, idx + 1, path, result);
        }
        
        // 撤销：把加入的所有 num 都弹出
        for (int count = min(maxCount, remain / num); count > 0; count--) {
            path.pop_back();
        }
    }
};
```

**注意**：上面的撤销写法不够优雅，更清晰的写法如下：

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        map<int, int> counter;
        for (int c : candidates) counter[c]++;
        vector<pair<int, int>> nums(counter.begin(), counter.end());
        
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, target, 0, path, result);
        return result;
    }
    
private:
    void backtrack(vector<pair<int, int>>& nums, int remain, int idx,
                   vector<int>& path, vector<vector<int>>& result) {
        if (remain == 0) {
            result.push_back(path);
            return;
        }
        if (idx == nums.size()) return;
        
        int num = nums[idx].first;
        int maxCount = min(nums[idx].second, remain / num);
        
        // 选 0 个当前数字 → 跳到下一种
        backtrack(nums, remain, idx + 1, path, result);
        
        // 选 1, 2, ..., maxCount 个当前数字
        for (int count = 1; count <= maxCount; count++) {
            path.push_back(num);
            backtrack(nums, remain - count * num, idx + 1, path, result);
        }
        
        // 撤销所有加入的 num
        for (int i = 0; i < maxCount; i++) {
            path.pop_back();
        }
    }
};
```

## 解法对比

| | 解法1: 排序+跳过去重 | 解法2: 计数法 |
|---|---|---|
| 去重方式 | 排序后同层跳过相同元素 | 按数字种类枚举选几个 |
| 代码复杂度 | 简洁，面试首选 | 稍长，但去重逻辑更直观 |
| 适用场景 | 面试中最常用 | 当重复元素特别多时更高效 |
| 核心区别 | 以"下标"为维度做选择 | 以"数字种类"为维度做选择 |

## 易错点

1. **去重条件写成 `i > 0` 而不是 `i > start`**
   - ✗ `if (i > 0 && candidates[i] == candidates[i-1]) continue;`
   - ✓ `if (i > start && candidates[i] == candidates[i-1]) continue;`
   - 为什么：`i > 0` 会导致 `[1,1,6]` 这种合法组合被跳过。第二个 1 作为"下一层的第一个选择"（`i == start`）时不应该被跳过，只有作为"同层的非第一个选择"（`i > start`）时才应该跳过。

2. **忘记排序**
   - 没有排序就用 `candidates[i] == candidates[i-1]` 去重，相同的数字不一定相邻，去重失败。

3. **递归传 `i` 而不是 `i + 1`**
   - ✗ `backtrack(candidates, remain - candidates[i], i, path, result);` ← 这是 39 题的写法！
   - ✓ `backtrack(candidates, remain - candidates[i], i + 1, path, result);`
   - 传 `i` 意味着同一个元素可以重复选，这是 39 题（无限次使用）的逻辑。

4. **剪枝用 `continue` 而不是 `break`**
   - ✗ `if (candidates[i] > remain) continue;` — 只跳过当前，后面可能更大，白白多循环
   - ✓ `if (candidates[i] > remain) break;` — 数组有序，后面都不可能，直接终止

## 面试追问

**Q1: 这道题和 39 题（Combination Sum）有什么区别？代码具体改了哪里？**
> 两处改动：① 递归调用传 `i + 1` 而不是 `i`（每个数只用一次）；② 增加了 `i > start && candidates[i] == candidates[i-1]` 的去重逻辑（因为有重复元素）。排序在两题中都有，但 39 题的排序只为剪枝，40 题还额外为去重。

**Q2: 去重条件为什么是 `i > start` 而不是 `i > 0`？能举具体例子吗？**
> 以 `[1,1,6], target=8` 为例。第一层选了 `1`（index=0），进入第二层时 `start=1`。第二层选 `candidates[1]=1` 时，`i == start == 1`，不满足 `i > start`，所以不跳过 → 可以产生 `[1,1,6]`。如果用 `i > 0`，第二层就会跳过这个 1，丢失合法解。

**Q3: 如果不排序，还有其他去重方式吗？**
> 可以用 HashSet 存已找到的组合（先排序组合再转字符串做 key），但开销大。更好的替代是 **计数法**（解法2），统计频次后按数字种类枚举，天然不重复。

**Q4: 这道题的时间复杂度怎么分析？**
> 最坏情况下，所有元素都不同且都很小，结果集大小可能是指数级 O(2^n)。但排序+剪枝在实际中大幅减少搜索，平均性能远好于最坏情况。

## 相关题型

- **39. 组合总和 (Combination Sum)** — 同一框架，区别：39 题每个数可无限使用（递归传 `i`），无重复元素（不需要去重）。掌握 40 题后做 39 题只需删掉去重逻辑并改 `i+1` 为 `i`。
- **46. 全排列 / 47. 全排列 II** — 47 题和本题的去重思想完全一致：排序 + 同层跳过重复。区别是排列不用 `start`，而是用 `visited` 数组。
- **78. 子集 / 90. 子集 II** — 90 题是子集版的"含重复元素去重"，去重手法和本题一模一样。
- **216. 组合总和 III** — 更简单的变体：候选数字是 1-9 且无重复，每个只用一次，不需要去重。