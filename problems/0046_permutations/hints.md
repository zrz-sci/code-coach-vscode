# 46. 全排列 (Permutations)

## 核心思路

这道题本质上是一个**穷举所有排列方式**的问题：从 n 个不重复数字中，按不同顺序选出全部 n 个，列出所有可能的排列。排列问题是回溯法的经典应用场景。

## 思维链

1. **读完题第一反应**：n 个数的全排列共有 n! 种，数据量最大为 6，即 6! = 720 种。必须全部列出来，没有"跳过"的可能。

2. **怎么系统地生成排列？** 想象你面前有 n 个空位，第一个位置有 n 种选择，第二个位置有 n-1 种选择（已选的不能再选），第三个有 n-2 种……这就是一棵**决策树**。

3. **怎么遍历这棵决策树？** 经典的 DFS + 回溯：选一个数 → 往下递归 → 递归结束后撤销选择 → 尝试下一个数。

4. **怎么知道哪些数已经选过？** 两种方式：
   - 用一个 `used[]` 布尔数组标记
   - 用 swap 把已选的数交换到前面，未选的在后面（原地操作，无需额外标记）

5. **什么时候结束？** 当路径长度等于数组长度时，说明所有数都选完了，收集结果。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + used 数组 | DFS，用布尔数组记录哪些数已选 | O(n × n!) | O(n) | ⭐ 必须写出 |
| 回溯 + swap 原地 | 用交换代替 used 数组 | O(n × n!) | O(n) 递归栈 | 加分项 |
| 下一个排列迭代法 | 调用 n! 次 next_permutation | O(n × n!) | O(1) 额外 | 能说出即可 |

> 时间复杂度说明：一共 n! 个排列，每个排列需要 O(n) 时间来复制存入结果，所以是 O(n × n!)。

## 关键提示

1. **排列 vs 组合 vs 子集的核心区别**：排列关心**顺序**（[1,2] 和 [2,1] 是不同排列），所以每次都从索引 0 开始遍历所有元素，用 used 数组跳过已选的；组合不关心顺序，用 start 参数控制只往后选。

2. **回溯的本质是"撤销"**：做选择（加入路径、标记 used）→ 递归 → 撤销选择（移出路径、取消 used）。忘记撤销就会出错。

3. **决策树长这样（以 [1,2,3] 为例）**：
```
//                          []
//                /          |          \
//              [1]         [2]         [3]
//             /   \       /   \       /   \
//          [1,2] [1,3] [2,1] [2,3] [3,1] [3,2]
//            |     |     |     |     |     |
//        [1,2,3][1,3,2][2,1,3][2,3,1][3,1,2][3,2,1]
```
每个叶子节点就是一个完整排列。

4. **swap 做法的直觉**：把数组分成"已排好的前半部分"和"待选的后半部分"，每次从后半部分选一个交换到当前位置。

5. **本题 nums 无重复**，如果有重复数字则需要去重（见 LeetCode 47），那是 follow-up。

## 解法详解

### 解法1: 回溯 + used 数组 — O(n × n!) / O(n) ⭐ 面试首选

**思考过程**：最直觉的方式——每次从所有数中选一个还没用过的，加入当前路径，选满 n 个就记录。怎么知道用没用过？一个 `used[i]` 布尔数组搞定。

```cpp
class Solution {
public:
    vector<vector<int>> result;
    vector<int> path;
    
    vector<vector<int>> permute(vector<int>& nums) {
        vector<bool> used(nums.size(), false);
        backtrack(nums, used);
        return result;
    }
    
    void backtrack(vector<int>& nums, vector<bool>& used) {
        // 终止条件：路径长度 == 数组长度，说明所有数都选了
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }
        
        // 每次都从 0 开始遍历（排列关心顺序，不用 start）
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;  // 跳过已选的数
            
            // 做选择
            path.push_back(nums[i]);
            used[i] = true;
            
            // 递归
            backtrack(nums, used);
            
            // 撤销选择
            path.pop_back();
            used[i] = false;
        }
    }
};
```

**关键点**：
- 排列问题的 for 循环从 `i = 0` 开始，而不是从某个 `start` 开始（那是组合/子集的做法）。
- `used` 数组保证同一个位置的数不会被重复选入同一条路径。

---

### 解法2: 回溯 + swap 原地交换 — O(n × n!) / O(n) 递归栈

**从解法1优化**：解法1需要额外的 `used[]` 数组和 `path` 数组。能不能直接在原数组上操作？可以！用 swap 把"已选区"和"未选区"分开。

**核心思想**：`nums[0..idx-1]` 是已经排好的，`nums[idx..n-1]` 是待选的。每次从待选区选一个交换到 `idx` 位置，然后递归处理 `idx+1`。

```
// swap 方式的决策过程（以 [1,2,3] 为例）:
//
// idx=0: 可选 [1,2,3]
//   swap(0,0) → [1,2,3], 递归 idx=1
//     idx=1: 可选 [2,3]
//       swap(1,1) → [1,2,3], 递归 idx=2
//         idx=2: 可选 [3]
//           swap(2,2) → [1,2,3] ✓ 收集
//       swap(1,2) → [1,3,2], 递归 idx=2
//         idx=2: 可选 [2]
//           swap(2,2) → [1,3,2] ✓ 收集
//       (swap 回来恢复)
//   swap(0,1) → [2,1,3], 递归 idx=1
//     ... (类似展开)
//   swap(0,2) → [3,2,1], 递归 idx=1
//     ... (类似展开)
```

```cpp
class Solution {
public:
    vector<vector<int>> result;
    
    vector<vector<int>> permute(vector<int>& nums) {
        backtrack(nums, 0);
        return result;
    }
    
    void backtrack(vector<int>& nums, int idx) {
        // 终止条件：所有位置都已确定
        if (idx == nums.size()) {
            result.push_back(nums);  // 直接用 nums 本身作为一个排列
            return;
        }
        
        // 从 idx 到末尾，每个数都有机会放到 idx 这个位置
        for (int i = idx; i < nums.size(); i++) {
            swap(nums[idx], nums[i]);   // 做选择：把 nums[i] 放到 idx 位置
            backtrack(nums, idx + 1);   // 递归：处理下一个位置
            swap(nums[idx], nums[i]);   // 撤销选择：恢复原状
        }
    }
};
```

**关键点**：
- 不需要 `used` 数组，也不需要 `path`，空间更省。
- 注意 `i` 从 `idx` 开始（不是 0），因为 `0..idx-1` 已经确定了。
- 递归结束后要 swap 回来（撤销），否则后续的排列会错乱。

---

### 解法3: 迭代法 — 基于 next_permutation — O(n × n!) / O(1) 额外

**另一种思路**：先把数组排序，然后不断调用"下一个排列"算法（LeetCode 31），直到回到最小排列为止。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        sort(nums.begin(), nums.end());  // 从最小排列开始
        vector<vector<int>> result;
        do {
            result.push_back(nums);
        } while (next_permutation(nums.begin(), nums.end()));
        return result;
    }
};
```

**关键点**：
- 这种写法极简，但面试中面试官大概率会要求你自己实现 `next_permutation`。
- 本质是字典序遍历所有排列，和回溯的顺序不同。

## 解法对比

|  | 解法1: used 数组 | 解法2: swap 原地 | 解法3: next_permutation |
|---|---|---|---|
| 核心机制 | 布尔数组标记 | 交换已选/未选区 | 字典序迭代 |
| 额外空间 | O(n) used + path | O(1)（递归栈 O(n)） | O(1) |
| 代码可读性 | 最直观 | 稍难理解 | 最简洁 |
| 面试推荐 | ⭐ 首选 | 追问时展示 | 知道即可 |
| 适用于有重复？ | 加排序+剪枝（→47题） | 需要额外处理 | 天然支持 |

**什么时候选哪个？**
- 面试第一轮：写解法1，清晰展示回溯三要素。
- 面试追问"能否不用额外空间"：给解法2。
- 面试追问"还有什么方法"：提解法3，展示知识面。

## 易错点

1. **排列写成了组合**
   - ✗ `for (int i = start; i < n; i++)` — 这是组合/子集的写法，会漏掉排列
   - ✓ `for (int i = 0; i < n; i++)` + `if (used[i]) continue;` — 排列每次都从 0 开始

2. **忘记撤销选择**
   - ✗ 只写了 `path.push_back()` 和 `used[i] = true`，没有对应的 pop 和恢复
   - ✓ 递归后必须 `path.pop_back()` 和 `used[i] = false`

3. **swap 解法中忘记 swap 回来**
   - ✗ 只 swap 了一次，递归回来没有恢复
   - ✓ 递归前 swap，递归后再 swap 回来（成对出现）

4. **终止条件里忘记复制**
   - ✗ `result.push_back(path)` 写在了递归调用里而不是终止条件里
   - ✓ 在 `path.size() == nums.size()` 时才 push_back

## 面试追问

**Q1（基础理解）**：排列和组合的回溯有什么区别？
> 排列关心顺序，每层 for 循环从 0 开始，用 used 数组跳过已选元素；组合不关心顺序，用 start 参数保证只往后选，避免重复。

**Q2（优化）**：能否不使用额外的 used 数组和 path？
> 可以，用 swap 方法。把数组分成已确定部分（前面）和待选部分（后面），每次从后面选一个 swap 到当前位置。这样直接在原数组上操作，不需要额外空间。

**Q3（变体）**：如果 nums 中有重复数字怎么办？（→ LeetCode 47）
> 在解法1的基础上：先对 nums 排序，然后在同一层的 for 循环中，如果 `nums[i] == nums[i-1]` 且 `used[i-1] == false`（说明前一个相同数字在这一层被撤销了），就跳过，避免同一层选相同的数产生重复排列。

**Q4（拓展）**：能否按字典序输出所有排列？
> 用解法3（next_permutation 迭代法），从排序后的最小排列开始，每次生成下一个字典序排列，天然有序。或者在解法1中先排序 nums，回溯过程中每层按顺序遍历，输出也是字典序。

## 相关题型

- **LeetCode 47 全排列 II** — 本题的 follow-up，nums 有重复。复用解法1的回溯框架，加一行 `if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;` 即可去重。
- **LeetCode 31 下一个排列** — 解法3 的底层算法。学会后可以不用库函数实现 next_permutation。
- **LeetCode 77 组合** — 复用回溯框架，区别：用 start 而不是 used，for 循环从 start 开始。
- **LeetCode 78 子集** — 复用回溯框架，区别：每个节点（不只是叶子）都收集结果。
- **LeetCode 784 字母大小写全排列** — 回溯框架的变体，每个字符有"变/不变"两种选择。