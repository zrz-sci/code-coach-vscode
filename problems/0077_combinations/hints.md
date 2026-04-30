# 77. 组合

## 核心思路

从 `[1, n]` 中选 `k` 个数的所有组合，本质上是一个**枚举子集**的问题——对每个数做"选/不选"的决策，或者说在决策树上做深度优先搜索，用 `start` 参数保证组合不重复。

## 思维链

1. **读完题第一反应**：要列出所有 C(n,k) 种组合，必须穷举。最朴素的方式——用 k 层嵌套 for 循环，但 k 不固定，嵌套层数写不出来。
2. **怎么处理"不固定层数的嵌套循环"？**→ 递归/回溯天然能模拟任意层嵌套。每一层递归等价于一层 for 循环。
3. **怎么避免重复？** 组合 `[1,2]` 和 `[2,1]` 算同一个。→ 每次只从上一个选的数的下一个位置开始选（`start` 参数），保证组合中的数是递增的。
4. **有没有优化空间？** 如果剩余可选数字不够凑满 k 个，可以提前剪枝。比如还需要选 3 个数，但从 `start` 到 `n` 只剩 2 个，就不用继续了。
5. **还有其他思路吗？** 对每个数做"选"或"不选"的二叉决策——这就是子集枚举的经典框架，选够 k 个就收集结果。
6. **迭代方式？** 可以用位掩码枚举所有 k 个 1 的 n 位二进制数，不过对于面试来说回溯更通用。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯（for 循环选择） | 每层从 start 开始枚举选哪个数 | O(k·C(n,k)) | O(k) 递归栈 | ⭐ 必须写出 |
| 回溯（选/不选决策） | 对每个数做选或不选的二叉决策 | O(k·C(n,k)) | O(k) 递归栈 | 能说出即可 |
| 回溯 + 剪枝 | 在解法1基础上加剩余数量剪枝 | O(k·C(n,k)) 但常数更小 | O(k) | ⭐ 面试加分 |
| 迭代（位掩码/字典序） | 枚举所有 k 位子集 | O(k·C(n,k)) | O(k) | 加分项 |

## 关键提示

1. **组合 vs 排列**：组合不在乎顺序，所以每次递归的起点必须是 `start`（上一个选的数+1），而不是从 1 开始。
2. **剪枝关键条件**：还需要选 `k - path.size()` 个数，但从 `i` 到 `n` 只有 `n - i + 1` 个数可选。如果 `n - i + 1 < k - path.size()`，直接 break。
3. **决策树长这样（n=4, k=2）**：

```
                      []
            /       /     \      \
          [1]     [2]     [3]    [4]
         /|\      / \      |
      [1,2][1,3][1,4] [2,3][2,4] [3,4]
```

每个叶子节点（`path.size() == k`）就是一个合法组合。

4. **`start` 参数的作用**：保证选的数严格递增，从而避免 `[1,2]` 和 `[2,1]` 的重复。
5. **两种回溯视角**：
   - "枚举选哪个"：在 `[start, n]` 中选一个数加入 path → 组合型回溯经典模板
   - "选或不选"：对数字 `start` 做选/不选两个分支 → 子集型回溯的变体

## 解法详解

### 解法1: 回溯（for 循环枚举选择）— O(k·C(n,k)) / O(k) ⭐ 面试首选

**思考过程**：我们要从 `[1,n]` 中按升序选 k 个数。第一个位置可以选 `1~n`，第二个位置从上一个选的数+1 开始，直到选够 k 个。这就是一棵多叉树：

```
// 决策树（n=4, k=2）:
//                    []
//          /       /     \      \
//        [1]     [2]     [3]    [4]  ← 第1层: 选第1个数
//       /|\      / \      |
//   [1,2][1,3][1,4] [2,3][2,4] [3,4] ← 第2层: 选第2个数（k=2, 收集结果）
//
// 注意 [4] 没有子节点——从5开始没有数可选了，这就是剪枝的直觉来源
```

```cpp
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(n, k, 1, path, result);
        return result;
    }
    
    void backtrack(int n, int k, int start, vector<int>& path, vector<vector<int>>& result) {
        // 终止条件：选够了 k 个数
        if (path.size() == k) {
            result.push_back(path);
            return;
        }
        // 从 start 开始枚举，保证组合中数字递增（避免重复）
        for (int i = start; i <= n; i++) {
            path.push_back(i);       // 做选择
            backtrack(n, k, i + 1, path, result);  // i+1 保证不重复选
            path.pop_back();          // 撤销选择
        }
    }
};
```

**关键点**：
- `start` 参数保证每次只往后选，避免 `[2,1]` 这样的重复。
- 递归深度 = k，每层 for 循环的范围逐层缩小。

---

### 解法2: 回溯 + 剪枝 — O(k·C(n,k)) / O(k) ⭐⭐ 面试加分

**从解法1优化**：解法1中 `[4]` 这个分支（n=4, k=2）进入后发现已经没有数可选了，白白多了一次递归调用。我们可以**提前判断剩余数字够不够**，不够就直接跳过。

```
// 剪枝的逻辑:
// 当前 path 已有 path.size() 个数，还需要 k - path.size() 个
// 从 i 到 n 共有 n - i + 1 个数可选
// 如果 n - i + 1 < k - path.size()，即 i > n - (k - path.size()) + 1，直接停止
//
// 以 n=4, k=3 为例:
// 无剪枝: [1],[2],[3],[4] 都会尝试作为第一个数
// 有剪枝: [3] 时还需2个数, 剩3,4够用 ✓
//          [4] 时还需2个数, 只剩4不够 ✗ → 剪掉!
```

```cpp
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(n, k, 1, path, result);
        return result;
    }
    
    void backtrack(int n, int k, int start, vector<int>& path, vector<vector<int>>& result) {
        if ((int)path.size() == k) {
            result.push_back(path);
            return;
        }
        // 剪枝: i 最多到 n - (k - path.size()) + 1
        // 因为从 i 开始还需要选 k - path.size() 个数
        // 需要 i + (k - path.size() - 1) <= n  →  i <= n - (k - path.size()) + 1
        int need = k - path.size();  // 还需要选几个
        for (int i = start; i <= n - need + 1; i++) {
            path.push_back(i);
            backtrack(n, k, i + 1, path, result);
            path.pop_back();
        }
    }
};
```

**关键点**：剪枝只改了 for 循环的上界，从 `i <= n` 变成 `i <= n - (k - path.size()) + 1`。虽然最坏情况复杂度不变（因为合法组合数本身就是 C(n,k)），但实际运行中会减少大量无效递归。

---

### 解法3: 回溯（选/不选决策）— O(k·C(n,k)) / O(n)

**换一个视角**：不用 for 循环枚举"选哪个"，而是对每个数字 `cur` 做"选"或"不选"的二叉决策。

```
// 二叉决策树（n=4, k=2）:
//                         (cur=1, [])
//                 选1 /              \ 不选1
//           (cur=2, [1])          (cur=2, [])
//          选2/      \不选2      选2/     \不选2
//     (cur=3,[1,2]) (cur=3,[1]) (cur=3,[2]) (cur=3,[])
//       收集✓     选3/ \不选3   选3/ \不选3  选3/ \不选3
//            [1,3]✓ (cur=4,[1]) [2,3]✓ ...  [3]  []
//                  选4/ \不选4
//                [1,4]✓  X(不够)
```

```cpp
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> path;
        dfs(n, k, 1, path, result);
        return result;
    }
    
    void dfs(int n, int k, int cur, vector<int>& path, vector<vector<int>>& result) {
        // 剪枝: 即使把 cur~n 全选也不够 k 个
        if ((int)path.size() + (n - cur + 1) < k) return;
        
        // 选够了 k 个
        if ((int)path.size() == k) {
            result.push_back(path);
            return;
        }
        
        // 选 cur
        path.push_back(cur);
        dfs(n, k, cur + 1, path, result);
        path.pop_back();
        
        // 不选 cur
        dfs(n, k, cur + 1, path, result);
    }
};
```

**关键点**：这种视角和子集问题（LeetCode 78）框架一样，区别仅在于收集结果的条件是 `path.size() == k` 而不是遍历完所有数字。

---

### 解法4: 迭代（字典序枚举）— O(k·C(n,k)) / O(k)

**完全不用递归**：维护一个长度为 k 的数组，模拟字典序递增生成下一个组合。

```cpp
class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> comb(k);
        // 初始化为 [1, 2, ..., k]
        for (int i = 0; i < k; i++) comb[i] = i + 1;
        
        while (true) {
            result.push_back(comb);
            // 找到最右边能+1的位置
            // comb[i] 的上界: n - (k - 1 - i)
            int i = k - 1;
            while (i >= 0 && comb[i] == n - (k - 1 - i)) i--;
            if (i < 0) break;  // 所有位置都到上界了
            comb[i]++;         // 第 i 位 +1
            // i 之后的位置依次递增
            for (int j = i + 1; j < k; j++) {
                comb[j] = comb[j - 1] + 1;
            }
        }
        return result;
    }
};
```

**关键点**：这本质是在模拟"进位"——最右边到上界后往左找还能加的位置，加完后右边重新从最小值开始。

## 解法对比

| | 解法1: 基础回溯 | 解法2: 回溯+剪枝 | 解法3: 选/不选 | 解法4: 迭代 |
|---|---|---|---|---|
| **模板** | 组合型回溯标准模板 | 在1基础上改上界 | 子集型回溯变体 | 无递归 |
| **直觉** | 最自然 | 最自然+优化 | 需要换视角 | 需要理解进位 |
| **面试推荐** | ⭐ 首选 | ⭐⭐ 追问优化 | 拓展理解 | 加分项 |
| **适用范围** | 组合/排列/子集 | 组合/排列/子集 | 子集/组合 | 仅组合 |

## 易错点

1. **for 循环起点写成 0 或 1**：
   - ✗ `for (int i = 1; i <= n; i++)` — 每层都从 1 开始，会产生 `[2,1]` 这样的重复
   - ✓ `for (int i = start; i <= n; i++)` — 从 `start` 开始保证递增

2. **剪枝条件算错**：
   - ✗ `i <= n - k + 1` — 忘了减去已经选的数量
   - ✓ `i <= n - (k - path.size()) + 1` — 用"还需要选几个"来算

3. **path.size() 与 int 比较**：
   - `path.size()` 返回 `size_t`（无符号），和 `int` 比较可能有意外行为
   - 安全做法：`(int)path.size() == k`

4. **忘记撤销选择**：
   - ✗ 只写了 `path.push_back(i)` 没有 `path.pop_back()`
   - ✓ push 和 pop 必须成对出现

## 面试追问

**Q1: 基础理解** — 为什么组合问题用 `start` 参数而排列问题不用？
> 组合不在乎顺序，`[1,2]` 和 `[2,1]` 是同一个，用 `start` 保证只按升序选取；排列在乎顺序，每个位置都要考虑所有未使用的数。

**Q2: 优化** — 你的回溯有什么可以优化的地方？
> 剪枝：如果从当前位置到 n 的剩余数字不够填满 path，直接返回。具体来说，for 循环上界从 `n` 改成 `n - (k - path.size()) + 1`。

**Q3: 变体** — 如果不是从 `[1,n]` 选，而是给你一个有重复元素的数组，求所有大小为 k 的组合（不能有重复组合），怎么办？
> 先排序，然后在回溯的 for 循环中加去重：`if (i > start && nums[i] == nums[i-1]) continue;`。这就是 LeetCode 40 的做法。

**Q4: 复杂度分析** — 时间复杂度到底是多少？
> 合法组合有 C(n,k) 个，每个组合需要 O(k) 时间拷贝到 result。所以是 O(k · C(n,k))。加了剪枝不改变最坏情况，但减少了访问无效节点的常数。

## 相关题型

- **LeetCode 78 子集** — 复用解法3"选/不选"框架，区别：78 不限制选几个，遍历完所有数字就收集结果；77 限制选 k 个。
- **LeetCode 46 全排列** — 复用回溯框架，区别：排列不用 `start`，改用 `visited[]` 数组标记已用元素。
- **LeetCode 40 组合总和 II** — 复用本题的回溯模板 + 排序去重，区别：40 的终止条件是 `target==0` 而不是 `path.size()==k`。
- **LeetCode 39 组合总和** — 和本题类似但每个数可以重复选，区别：递归时传 `i` 而不是 `i+1`。