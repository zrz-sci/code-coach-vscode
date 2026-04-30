# 39. 组合总和

## 核心思路

本质上是一个**完全背包问题的枚举版**：从一组无重复的数中（每个数可以无限次使用），找出所有和为 target 的组合。因为要求列举所有方案而不是计数，所以用**回溯（DFS + 剪枝）**来穷举。

## 思维链

1. **读完题第一反应**：要找出"所有"满足条件的组合 → 这是一个穷举问题 → 想到回溯/DFS。
2. **怎么穷举？** 对于每个候选数，要么选它（还可以继续选它），要么不选它（跳到下一个数）。这就形成了一棵决策树。
3. **如何避免重复组合？** 比如 `[2,2,3]` 和 `[3,2,2]` 是同一组合。关键：**规定只能从当前位置往后选**，不能回头选之前的数。
4. **如何剪枝？** 如果剩余的 target 已经小于当前候选数了，后面更大的数也不可能满足 → **先排序，再剪枝**，可以提前终止循环。
5. **递归终止**：当 target == 0 时，找到一个合法组合；当 target < 0 或遍历完所有候选数时，回溯。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯（无排序） | DFS 枚举所有组合 | O(n^(T/M)) | O(T/M) | 能说出即可 |
| 回溯 + 排序剪枝 | 排序后提前终止无效分支 | O(n^(T/M))，常数更小 | O(T/M) | ⭐ 必须写出 |
| 完全背包 DP | 先计数再回溯构造 | 特殊场景 | O(T) | 加分项（知道关联即可） |

> 其中 T = target，M = min(candidates)，n = candidates.length

## 关键提示

1. **避免重复的关键**：每层递归都从 `start` 位置开始遍历，不从 0 开始。这样 `[2,3]` 会被枚举但 `[3,2]` 不会。
2. **同一个数可以重复选取**：递归时传入的 `start` 不是 `i+1` 而是 `i`（对比 LeetCode 40 组合总和 II，那道题用 `i+1`）。
3. **排序 + 剪枝是加速利器**：排序后，一旦 `candidates[i] > remaining`，后面所有数都更大，直接 `break`。
4. **决策树（以 candidates=[2,3,6,7], target=7 为例）**：

```
                        target=7
                  /        |        \         \
              选2(t=5)   选3(t=4)  选6(t=1)  选7(t=0)✓
             /   |   \      |   \      ✗
          选2   选3  选6  选3  选6
         (t=3) (t=2)(t<0)(t=1)(t<0)
         / \     |        ✗
       选2 选3  选2
      (t=1)(t=0)✓(t=0)✓
       |
      选2
     (t<0)✗

结果: [2,2,3], [7]
```

5. **与组合总和 II（LeetCode 40）的区别**：本题元素可重复使用，40 题每个元素只能用一次。代码上只差一行：递归时传 `i` 还是 `i+1`。

## 解法详解

### 解法1: 回溯（基础版，无排序）— O(n^(T/M)) / O(T/M)

**思考过程**：要列举所有组合，自然想到回溯。核心问题是如何避免重复：用 `start` 参数控制"只能往后选"。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(candidates, target, 0, path, result);
        return result;
    }
    
    void backtrack(vector<int>& candidates, int remaining, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        // 终止条件：凑够了
        if (remaining == 0) {
            result.push_back(path);
            return;
        }
        
        for (int i = start; i < candidates.size(); i++) {
            if (candidates[i] > remaining) continue; // 跳过太大的数
            
            path.push_back(candidates[i]);           // 做选择
            // 注意传 i 而不是 i+1：同一个数可以重复使用
            backtrack(candidates, remaining - candidates[i], i, path, result);
            path.pop_back();                          // 撤销选择
        }
    }
};
```

**关键点**：`continue` 只是跳过当前这个数，不能提前终止循环，因为数组未排序，后面可能有更小的数。

---

### 解法2: 回溯 + 排序剪枝 — O(n^(T/M)) / O(T/M) ⭐ 面试首选

**从解法1优化**：解法1 的问题是即使某个数太大，还得继续遍历后面的数。如果**先排序**，一旦当前数 > remaining，后面所有数都更大，直接 `break` 终止循环。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        sort(candidates.begin(), candidates.end()); // 排序，为剪枝做准备
        backtrack(candidates, target, 0, path, result);
        return result;
    }
    
    void backtrack(vector<int>& candidates, int remaining, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        if (remaining == 0) {
            result.push_back(path);
            return;
        }
        
        for (int i = start; i < candidates.size(); i++) {
            // 剪枝：排序后，当前数已经超了，后面更大的数更不可能
            if (candidates[i] > remaining) break;  // 注意是 break 不是 continue
            
            path.push_back(candidates[i]);
            backtrack(candidates, remaining - candidates[i], i, path, result);
            path.pop_back();
        }
    }
};
```

**关键点**：`break` vs `continue` — 排序后用 `break` 可以砍掉整棵子树，这是剪枝的核心。

---

### 解法3: 选/不选 视角的回溯 — O(n^(T/M)) / O(T/M)

**不同的思考角度**：解法2是"对于当前位置，枚举选哪个数"。另一种思路是"对于每个候选数，决定选几次"。这种"选或不选"的框架在背包问题中更常见。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> path;
        sort(candidates.begin(), candidates.end());
        dfs(candidates, target, 0, path, result);
        return result;
    }
    
    // 对于 candidates[idx]，选择"再选一次"或"跳过它"
    void dfs(vector<int>& candidates, int remaining, int idx,
             vector<int>& path, vector<vector<int>>& result) {
        if (remaining == 0) {
            result.push_back(path);
            return;
        }
        if (idx >= candidates.size() || candidates[idx] > remaining) return;
        
        // 分支1：选当前数（还可以继续选它，所以 idx 不变）
        path.push_back(candidates[idx]);
        dfs(candidates, remaining - candidates[idx], idx, path, result);
        path.pop_back();
        
        // 分支2：不选当前数，跳到下一个候选
        dfs(candidates, remaining, idx + 1, path, result);
    }
};
```

**关键点**：这种写法与完全背包 DP 的状态转移 `dp[i][j] = dp[i][j-w] + dp[i+1][j]` 完全对应，有助于理解背包问题。

## 解法对比

| | 解法1（无排序） | 解法2（排序+剪枝）⭐ | 解法3（选/不选） |
|---|---|---|---|
| 遍历策略 | 枚举选哪个 | 枚举选哪个 | 二叉决策（选/不选） |
| 排序 | 不需要 | 需要 | 需要 |
| 剪枝效果 | 差（只能 continue） | 好（可以 break） | 好（提前 return） |
| 代码风格 | 经典回溯 | 经典回溯+剪枝 | 类似背包递归 |
| 面试推荐 | 初始版本 | **首选** | 追问时展示 |

## 易错点

1. **递归传 `i` 还是 `i+1`**：
   - ✗ `backtrack(..., i+1, ...)` — 每个数只能用一次，变成了 LeetCode 40 的逻辑
   - ✓ `backtrack(..., i, ...)` — 同一个数可以重复使用

2. **剪枝用 `break` 还是 `continue`**：
   - ✗ 排序后用 `continue` — 跳过当前但还检查后面更大的数，浪费时间
   - ✓ 排序后用 `break` — 后面全部更大，直接终止

3. **忘记排序就用 `break`**：
   - ✗ 没排序就 `break` — 可能跳过后面更小的有效数字，漏解
   - ✓ 排序是 `break` 剪枝的前提

4. **path 忘记 pop_back**：
   - ✗ 只有 `push_back` 没有 `pop_back` — 回溯的核心就是撤销选择
   - ✓ `push_back` 和 `pop_back` 成对出现

## 面试追问

**Q1: 这道题的时间复杂度怎么分析？**
> 最坏情况下，决策树的深度为 T/M（T=target, M=最小候选数），每层最多 n 个分支，所以时间复杂度为 O(n^(T/M))。但实际因为剪枝，远达不到这个上界。

**Q2: 如果每个数字只能使用一次呢？（→ LeetCode 40）**
> 把递归调用中的 `i` 改成 `i+1`。另外因为 40 题的 candidates 有重复元素，还需要在同一层跳过相同元素：`if (i > start && candidates[i] == candidates[i-1]) continue;`

**Q3: 如果不要求列举所有组合，只要求方案数呢？**
> 用完全背包 DP：`dp[j] += dp[j - candidates[i]]`，时间 O(n*T)，空间 O(T)。这比回溯高效得多，因为不需要构造每个方案。

**Q4: 如果 candidates 非常大（上千个），但 target 很小，怎么优化？**
> 排序后只保留 ≤ target 的候选数，其余全部剪掉。同时排序后的 break 剪枝效果会很好。

## 相关题型

- **LeetCode 40 组合总和 II** — 每个元素只能用一次，且有重复元素。复用本题框架，改两处：递归传 `i+1`，同层跳过重复元素。
- **LeetCode 216 组合总和 III** — 从 1-9 中选 k 个数，和为 n。复用本题框架，加上个数限制。
- **LeetCode 377 组合总和 IV** — 只求方案数，顺序不同算不同方案。用 DP 而非回溯，注意是排列而非组合。
- **LeetCode 518 零钱兑换 II** — 只求方案数，顺序不同算同一方案。经典完全背包 DP，与本题的 DP 视角完全对应。
- **LeetCode 77 组合** — 基础组合问题，复用"从 start 开始遍历"的去重框架。