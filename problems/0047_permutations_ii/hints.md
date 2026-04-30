# 47. 全排列 II

## 核心思路

本题本质上是 **带重复元素的全排列**：在回溯框架生成排列的基础上，需要保证 **同一层决策树中不选择重复的数字**，从而避免产生重复排列。

## 思维链

1. **读完题第一反应**：这不就是 46 题全排列的升级版吗？区别在于输入数组可能有重复元素，但输出不能有重复排列。
2. **最朴素做法**：先像 46 题一样生成所有排列，再用 set 去重。能 work 但浪费大量计算——重复排列依然被完整构造了。
3. **瓶颈在哪**：问题出在决策树的同一层中，如果有两个值相同的元素，它们各自展开的子树完全一样，产生重复。
4. **怎么突破**：先排序让相同元素相邻，然后在同一层选择时，如果当前元素和前一个元素相同，且前一个元素在这一层没被使用（即已经被跳过/撤销了），就跳过当前元素——这就是 **同层去重**。
5. **关键判断条件**：`i > 0 && nums[i] == nums[i-1] && !used[i-1]`，这个条件保证了：对于一组重复数字，我们只按"从左到右"的固定顺序使用它们，从而消除重复。
6. **为什么 `!used[i-1]`**：如果 `used[i-1] == true`，说明前一个相同元素在当前路径中（更深层选的），不是同层重复；如果 `used[i-1] == false`，说明前一个相同元素在同层已经尝试过并撤销了，此时再选当前元素会产生重复分支。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力去重 | 全排列 + set 去重 | O(n! × n) | O(n! × n) | 能说出即可 |
| 排序 + 回溯剪枝 | 排序后同层跳过重复 | O(n! × n) | O(n) | ⭐ 必须写出 |
| swap 回溯 + set 去重 | 不排序，用 set 控制同层 | O(n! × n) | O(n) | 加分项 |

> 注：最坏情况（无重复）时间都是 O(n! × n)，但有重复时解法2/3通过剪枝跳过大量分支，实际远快于解法1。

## 关键提示

1. **先想 46 题**：如果你能写出无重复的全排列（LeetCode 46），本题只需加一个去重条件。
2. **排序是去重的前置条件**：排序后相同元素相邻，才能用 `nums[i] == nums[i-1]` 判断。
3. **同层 vs 同路径**：去重发生在决策树的"同一层"（同一个 for 循环里），不是"同一条路径"上。
4. **画决策树**看重复在哪：

```
nums = [1, 1, 2] (已排序)

决策树（带去重）:
                        []
              /          |          \
           [1]          [1]✗        [2]
          /   \          ↑         /   \
       [1,1] [1,2]   跳过!同层  [2,1] [2,1]✗
         |     |      重复        |      ↑
      [1,1,2] [1,2,1]          [2,1,1] 跳过!

✗ 表示被剪枝: nums[1]==nums[0] 且 !used[0] → 同层重复，跳过
结果: [1,1,2], [1,2,1], [2,1,1]
```

5. **去重条件记忆口诀**：`i > 0 && 值相同 && 前一个没用` → 跳过。

## 解法详解

### 解法1: 暴力去重 — O(n! × n) / O(n! × n)

**思考过程**：最直觉的方法——先不管重复，像 46 题一样生成所有排列，最后用 set 去重。

```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        set<vector<int>> resultSet;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        backtrack(nums, used, path, resultSet);
        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }
    
    void backtrack(vector<int>& nums, vector<bool>& used, 
                   vector<int>& path, set<vector<int>>& resultSet) {
        if (path.size() == nums.size()) {
            resultSet.insert(path); // 用 set 自动去重
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            backtrack(nums, used, path, resultSet);
            path.pop_back();
            used[i] = false;
        }
    }
};
```

**关键点**：
- 这个解法正确但低效：对于 `[1,1,1]`，我们生成了 6 个 `[1,1,1]` 然后去重到 1 个。
- set 插入每个排列需要 O(n) 比较，且存储了所有中间结果，空间开销大。
- 面试中可以先说出这个思路，然后主动提出优化。

### 解法2: 排序 + 回溯剪枝 — O(n! × n) / O(n) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是"先生成后去重"，浪费了大量计算在构造重复排列上。我们能不能在 **构造过程中就避免重复**？

关键观察：排序后，重复元素相邻。在决策树的同一层中，如果两个位置要放相同的值，后面那个可以直接跳过。

```
// 决策树详解 nums = [1₁, 1₂, 2]
// 
// 第0层选择（根节点展开）:
//     选 1₁ → 进入子树
//     选 1₂ → nums[1]==nums[0] 且 !used[0] → 跳过！
//              (因为 1₁ 刚刚在同层用过又撤销了，used[0]=false)
//     选 2  → 进入子树
//
// 为什么 !used[i-1] 时跳过？
//   used[i-1]=false 意味着 nums[i-1] 在当前层选过又回溯了（同层重复）
//   used[i-1]=true  意味着 nums[i-1] 在更深层被选中（路径中的合法使用）
```

```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        
        sort(nums.begin(), nums.end()); // 排序是去重的前提！
        
        backtrack(nums, used, path, result);
        return result;
    }
    
    void backtrack(vector<int>& nums, vector<bool>& used, 
                   vector<int>& path, vector<vector<int>>& result) {
        // 终止条件：路径长度等于数组长度，收集一个完整排列
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }
        
        for (int i = 0; i < nums.size(); i++) {
            // 已经在当前路径中使用过，跳过
            if (used[i]) continue;
            
            // 【核心去重】同层中，如果当前值等于前一个值，
            // 且前一个值没被使用（说明是同层回溯后的状态），跳过
            if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) continue;
            
            // 做选择
            used[i] = true;
            path.push_back(nums[i]);
            
            // 递归
            backtrack(nums, used, path, result);
            
            // 撤销选择
            path.pop_back();
            used[i] = false;
        }
    }
};
```

**关键点**：
- `sort` 不能忘！没有排序的话 `nums[i] == nums[i-1]` 判断毫无意义。
- 去重条件 `!used[i-1]` 和 `used[i-1]` 都能实现去重，但 `!used[i-1]` 剪枝更早、效率更高（优先跳过"前一个没选"的情况，让重复元素只按固定顺序出现在路径中）。

### 解法3: swap 回溯 + 局部 set 去重 — O(n! × n) / O(n)

**不同角度**：不排序，用 swap 的方式生成排列（类似 46 题的 swap 解法），在每一层用一个局部 set 记录已经选过的值来去重。

```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> result;
        backtrack(nums, 0, result);
        return result;
    }
    
    void backtrack(vector<int>& nums, int start, vector<vector<int>>& result) {
        if (start == nums.size()) {
            result.push_back(nums);
            return;
        }
        
        unordered_set<int> seen; // 记录当前层已经用过的值
        
        for (int i = start; i < nums.size(); i++) {
            // 如果这个值在当前层已经选过，跳过
            if (seen.count(nums[i])) continue;
            seen.insert(nums[i]);
            
            swap(nums[start], nums[i]); // 把 nums[i] 放到 start 位置
            backtrack(nums, start + 1, result);
            swap(nums[start], nums[i]); // 撤销
        }
    }
};
```

**关键点**：
- 不需要排序，不需要 `used` 数组。
- 每层创建一个 set，空间 O(n)，每层结束自动销毁。
- swap 方式的好处是不需要额外的 `path` 和 `used` 数组；缺点是不如解法2直观，且 set 有常数开销。

## 解法对比

| | 解法1: 暴力去重 | 解法2: 排序+剪枝 | 解法3: swap+set |
|---|---|---|---|
| 去重时机 | 事后去重（set 存结果） | 事前剪枝（跳过分支） | 事前剪枝（set 判断值） |
| 是否需要排序 | 不需要 | 需要 | 不需要 |
| 额外空间 | set 存所有排列 O(n!×n) | used 数组 O(n) | 每层 set O(n) |
| 面试推荐度 | ✗ 不推荐 | ⭐ 最推荐 | ✓ 可以作为备选 |
| 与46题的差异 | +set 存储 | +sort + 一行去重判断 | +每层 set |

**什么时候选哪个？**
- **面试首选解法2**：思路清晰，从 46 题自然过渡，去重逻辑最经典。
- 如果面试官问"能不能不排序？"，给出解法3。

## 易错点

1. **忘记排序**
   - ✗ 直接用 `nums[i] == nums[i-1]` 但没有 `sort`
   - ✓ 在回溯前先 `sort(nums.begin(), nums.end())`
   - 原因：不排序时相同元素可能不相邻，`nums[i] == nums[i-1]` 判断不到所有重复

2. **去重条件写反**
   - ✗ `if (i > 0 && nums[i] == nums[i-1] && used[i-1])` — 这也能去重但效率低
   - ✓ `if (i > 0 && nums[i] == nums[i-1] && !used[i-1])` — 剪枝更早
   - 原因：`used[i-1]=true` 版本让重复元素从右到左使用，`used[i-1]=false` 版本让重复元素从左到右使用，后者在浅层就跳过更多分支

3. **漏掉 `used[i]` 判断**
   - ✗ 只写了去重判断，忘了 `if (used[i]) continue;`
   - ✓ 两个 continue 条件缺一不可：`used[i]` 防止路径重复使用同一位置，去重条件防止同层重复使用同一值

4. **swap 解法中修改了排序后的数组**
   - ✗ 对已排序数组用 swap 解法，swap 会打乱排序顺序，导致 `nums[i] == nums[i-1]` 失效
   - ✓ swap 解法应配合 set 去重（解法3），不要混用两种去重方式

## 面试追问

**Q1: 与 46 题全排列相比，代码具体改了哪里？**
→ 只改了两处：① 开头加 `sort`；② for 循环中加一行 `if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;`。其余代码完全一样。

**Q2: 去重条件用 `used[i-1]` 和 `!used[i-1]` 都能 AC，区别是什么？**
→ 两种都正确。`!used[i-1]`（从左到右固定顺序）在树的浅层就跳过了，剪枝量更大；`used[i-1]`（从右到左固定顺序）在深层才跳过，做了更多无效递归。面试推荐前者。

**Q3: 如果不允许排序，怎么去重？**
→ 用解法3：swap + 每层维护一个 set 记录已选值。或者用哈希表记录频次，每次从频次表中选一个还有剩余次数的数。

**Q4: 能否用 next_permutation 实现？**
→ 可以。先排序得到最小排列，然后反复调用 `next_permutation` 直到变回最小排列（或返回 false）。每次得到的都是下一个字典序排列，天然不重复。但面试中通常期望你手写回溯。

## 相关题型

- **46. 全排列 (Permutations)** — 本题的前置题，无重复版本。本题只需在 46 的基础上加排序 + 一行去重。
- **40. 组合总和 II (Combination Sum II)** — 复用完全相同的去重模板：排序 + `nums[i] == nums[i-1] && !used[i-1]`（组合中用 `i > start` 代替 `!used[i-1]`）。
- **90. 子集 II (Subsets II)** — 同样是"含重复元素的回溯去重"，去重逻辑一模一样。
- **31. 下一个排列 (Next Permutation)** — 理解排列的字典序生成，是 Q4 追问的基础。
- **491. 非递减子序列** — 不能排序的回溯去重，需要用每层 set（类似解法3的思路）。