# LCR 083. 全排列

## 核心思路

**回溯法** -- 经典的排列问题。每一层递归选择一个还没有使用过的元素加入当前路径，当路径长度等于数组长度时，收集一个完整排列。回溯的本质是在决策树上做 DFS，每个节点代表一次选择，叶节点代表一个完整排列。同主站 LeetCode 46 题。

## 思维链

1. **读完题第一反应** -- 全排列就是所有元素的不同排列方式。n 个不同元素有 n! 种排列。暴力生成所有排列的方法就是回溯。

2. **回溯的核心三要素**:
   - **路径**: 已经做出的选择（当前正在构建的排列）
   - **选择列表**: 还能做的选择（还没有使用的元素）
   - **结束条件**: 路径长度等于 n（收集结果）

3. **如何标记"已使用"?**
   - 方法 A: `used[]` 布尔数组，`used[i]=true` 表示 `nums[i]` 已在路径中
   - 方法 B: swap 原地交换法，`nums[0..idx-1]` 是已选的，`nums[idx..n-1]` 是未选的

4. **时间复杂度分析** -- 共 n! 个叶节点，每个叶节点需要 O(n) 复制路径，总时间 O(n * n!)。空间 O(n) 用于递归栈和路径。

## 决策树可视化

```
nums = [1, 2, 3]

决策树 (每层选择一个未使用的元素):

                          []
                 /         |         \
               [1]        [2]        [3]          ← 第1层: 选第一个元素
              / \         / \         / \
           [1,2] [1,3] [2,1] [2,3] [3,1] [3,2]   ← 第2层: 选第二个元素
             |     |     |     |     |     |
          [1,2,3][1,3,2][2,1,3][2,3,1][3,1,2][3,2,1]  ← 第3层: 选第三个元素(叶节点)

6 个叶节点 = 3! = 6 种全排列

回溯过程（选择 → 递归 → 撤销）:
  []: 选1 → [1]
    [1]: 选2 → [1,2]
      [1,2]: 选3 → [1,2,3] ← 收集! 撤销3
    [1,2]: 撤销2
    [1]: 选3 → [1,3]
      [1,3]: 选2 → [1,3,2] ← 收集! 撤销2
    [1,3]: 撤销3
  [1]: 撤销1
  []: 选2 → [2]
    [2]: 选1 → [2,1]
      [2,1]: 选3 → [2,1,3] ← 收集! 撤销3
    [2,1]: 撤销1
    [2]: 选3 → [2,3]
      [2,3]: 选1 → [2,3,1] ← 收集! 撤销1
    [2,3]: 撤销3
  [2]: 撤销2
  []: 选3 → [3]
    ...同理...
```

```
nums = [0, 1]

        []
       /    \
     [0]    [1]
      |      |
    [0,1]  [1,0]

2 个叶节点 = 2! = 2 种全排列
输出: [[0,1], [1,0]]
```

## 回溯模板

```
回溯法通用框架:

void backtrack(选择列表, 路径, 结果集) {
    if (满足结束条件) {
        结果集.add(路径的拷贝);
        return;
    }

    for (每个选择 in 选择列表) {
        if (该选择不合法) continue;  // 剪枝

        做出选择;          // 选择: 加入路径 + 标记已用
        backtrack(选择列表, 路径, 结果集);  // 递归
        撤销选择;          // 回溯: 移出路径 + 取消标记
    }
}

全排列的具体化:
  路径: path 数组
  选择列表: nums 中所有元素
  合法性检查: used[i] == false (该元素还没用过)
  做出选择: path.push_back(nums[i]); used[i] = true;
  撤销选择: path.pop_back(); used[i] = false;
  结束条件: path.size() == nums.size()
```

## used 数组状态追踪

```
nums = [1, 2, 3], 追踪 used 数组的变化:

backtrack([], used=[F,F,F]):
  i=0: used[0]=T, path=[1]
    backtrack([1], used=[T,F,F]):
      i=0: used[0]=T, skip
      i=1: used[1]=T, path=[1,2]
        backtrack([1,2], used=[T,T,F]):
          i=0: skip, i=1: skip
          i=2: used[2]=T, path=[1,2,3]
            → path.size()==3, 收集 [1,2,3]!
          used[2]=F, path=[1,2]  ← 回溯
        used[1]=F, path=[1]  ← 回溯
      i=2: used[2]=T, path=[1,3]
        backtrack([1,3], used=[T,F,T]):
          i=1: used[1]=T, path=[1,3,2]
            → 收集 [1,3,2]!
          used[1]=F, path=[1,3]  ← 回溯
        used[2]=F, path=[1]  ← 回溯
    used[0]=F, path=[]  ← 回溯
  i=1: used[1]=T, path=[2]
    ... (类似过程)
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + used 数组 | 标记已用元素 | O(n*n!) | O(n) | 必须写出 |
| 回溯 + swap | 原地交换避免 used 数组 | O(n*n!) | O(n) | 进阶 |

## 关键提示

1. **结果必须拷贝**: 收集结果时必须 `res.push_back(path)` 做一份拷贝（C++ 中 vector 赋值是深拷贝）。如果用 Java/Python 等语言，需要显式 `new ArrayList<>(path)` 或 `path[:]`，否则后续回溯会修改已收集的结果。

2. **used 数组 vs swap**: used 数组更直观易懂，面试首选。swap 方法省去了 used 数组的空间，但代码可读性稍差，且会改变 nums 的顺序。

3. **与 "子集" 和 "组合" 的区别**:
   - 排列: 关注元素顺序，`[1,2]` 和 `[2,1]` 是不同排列 → 每层从 0 开始遍历
   - 组合: 不关注顺序，`[1,2]` 和 `[2,1]` 是同一个组合 → 每层从 start 开始遍历
   - 子集: 组合的推广，每个中间路径都是一个子集

4. **为什么不需要 start 参数**: 排列问题中，[1,2] 和 [2,1] 是不同结果，所以每层遍历都从 i=0 开始，用 used 数组避免重复选同一个元素。而组合/子集问题中需要 start 参数来避免重复组合。

5. **时间复杂度 O(n * n!) 的理解**: n! 个叶节点，每个叶节点需要 O(n) 时间复制路径到结果。实际上从根到所有叶的路径总长度也是 O(n * n!)。

## 解法详解

### 解法1: 回溯 + used 数组 -- O(n*n!) / O(n)

**思路**: 用 `used[i]` 标记 `nums[i]` 是否已在当前路径中。每层遍历所有元素，跳过已使用的，选择未使用的加入路径。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        backtrack(nums, used, path, res);
        return res;
    }

    void backtrack(vector<int>& nums, vector<bool>& used,
                   vector<int>& path, vector<vector<int>>& res) {
        // 结束条件: 路径长度 == 数组长度，找到一个完整排列
        if (path.size() == nums.size()) {
            res.push_back(path);  // C++ vector 赋值是深拷贝
            return;
        }

        // 遍历所有元素（排列问题从 0 开始，不是从 start 开始）
        for (int i = 0; i < (int)nums.size(); i++) {
            if (used[i]) continue;  // 跳过已使用的元素

            // 做出选择
            used[i] = true;
            path.push_back(nums[i]);

            // 递归
            backtrack(nums, used, path, res);

            // 撤销选择（回溯）
            path.pop_back();
            used[i] = false;
        }
    }
};
```

### 解法2: 回溯 + swap -- O(n*n!) / O(n)

**思路**: 不用 used 数组。通过 swap 操作，让 `nums[0..idx-1]` 存放已选元素，`nums[idx..n-1]` 是待选元素。每层将 `nums[i]`（i >= idx）和 `nums[idx]` 交换，递归 idx+1，再交换回来恢复。

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        backtrack(nums, 0, res);
        return res;
    }

    void backtrack(vector<int>& nums, int idx, vector<vector<int>>& res) {
        if (idx == (int)nums.size()) {
            res.push_back(nums);  // nums 本身就是当前排列
            return;
        }

        for (int i = idx; i < (int)nums.size(); i++) {
            swap(nums[i], nums[idx]);       // 选择: 把 nums[i] 放到位置 idx
            backtrack(nums, idx + 1, res);  // 递归处理下一个位置
            swap(nums[i], nums[idx]);       // 撤销: 恢复原状
        }
    }
};
```

**注意**: swap 方法生成的排列顺序和 used 方法不同（不是字典序），但结果集合是一样的。

## 解法对比

| | used 数组 | swap |
|---|---|---|
| 时间 | O(n*n!) | O(n*n!) |
| 额外空间 | O(n) used 数组 | 无额外数组 |
| 代码清晰度 | 直观 | 稍复杂 |
| 输出顺序 | 字典序 | 非字典序 |
| 面试推荐 | 首选 | 进阶 |

## 易错点

1. **忘记撤销选择（回溯）**: 递归返回后必须 `path.pop_back()` 和 `used[i] = false`。否则后续分支会受到前一个分支选择的影响，产生错误结果。

2. **收集结果时忘记拷贝**（Java/Python 易错）:
   - Java: `res.add(new ArrayList<>(path))` 而不是 `res.add(path)`
   - Python: `res.append(path[:])` 而不是 `res.append(path)`
   - C++ 中 `res.push_back(path)` 自动做深拷贝，不会有此问题

3. **排列问题误用 start 参数**: 排列问题中 `[1,2]` 和 `[2,1]` 不同，每层必须从 `i=0` 开始遍历。如果像组合那样用 `start` 参数，会漏掉结果。

4. **swap 方法中 swap 了两次但忘记在 i==idx 时也做**: `i==idx` 时 swap 是恒等操作不影响正确性，但如果加了 `if(i==idx) continue` 的"优化"就会漏掉结果。

## 面试追问

**Q1: 如果数组有重复元素怎么办？(LeetCode 47)**
-> 先排序，然后在回溯中加一个剪枝条件：如果 `nums[i] == nums[i-1]` 且 `used[i-1] == false`，则跳过。这确保相同元素只有一种选取顺序。

**Q2: 全排列的输出能否按字典序？**
-> 用 used 数组的方法天然按字典序输出（如果 nums 先排序的话），因为每层按 i 从小到大遍历。swap 方法不保证字典序。

**Q3: 如果只要求第 k 个排列？(LeetCode 60)**
-> 不需要生成所有排列。利用数学：第 `i` 位的元素可以通过 `k / (n-1)!` 确定，然后 `k %= (n-1)!`，逐位确定。时间 O(n^2)。

**Q4: 回溯的时间复杂度为什么是 O(n*n!) 而不是 O(n!)?**
-> 有 n! 个叶节点（完整排列），每个叶节点需要 O(n) 的时间来复制路径到结果集。另外，从根到叶的路径总工作量也是 O(n * n!)。所以总时间是 O(n * n!)。

**Q5: 这道题和 "子集" (LC78)、"组合" (LC77) 的回溯框架有什么区别？**
-> 三者都用回溯模板。区别在于：
  - 排列: 每层从 0 开始，用 used 避免重复选同一个元素
  - 组合: 每层从 start 开始，避免选已经选过的前面的元素
  - 子集: 和组合一样用 start，但每个中间路径都是一个合法子集

## 相关题型

- **LeetCode 46 全排列** -- 和本题完全相同（LCR 083 就是 LC 46）
- **LeetCode 47 全排列 II** -- 有重复元素，需要排序 + 剪枝
- **LeetCode 77 组合** -- 回溯框架用 start 参数
- **LeetCode 78 子集** -- 回溯框架收集所有中间路径
- **LeetCode 60 排列序列** -- 数学法求第 k 个排列
- **LeetCode 31 下一个排列** -- 不用回溯，纯数学/贪心
