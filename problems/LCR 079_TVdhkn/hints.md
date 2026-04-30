# LCR 079. 子集 (同主站 78. Subsets)

## 难度: Medium | 标签: Bit Manipulation, Array, Backtracking

---

## 题目理解

给定一个元素**互不相同**的整数数组 `nums`，返回该数组所有可能的子集(幂集)。解集不能包含重复的子集，可以按任意顺序返回。

例如 `nums = [1,2,3]`，共有 2^3 = 8 个子集:
```
[], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]
```

**本质**: 对于每个元素，做出"选"或"不选"的决策，所有决策的组合就是全部子集。

---

## 解法一: 回溯法 (最经典, 面试推荐)

### 核心思想

用 DFS 遍历一棵决策树。每到一个节点就**收集当前路径**(子集问题的特点 -- 不只收集叶子节点)。通过 `start` 参数保证只向后选择，避免产生重复子集。

### 决策树完整可视化 (nums = [1, 2, 3])

```
                        backtrack(start=0, path=[])
                        收集 []
                 ┌────────────┼────────────┐
             i=0 │        i=1 │        i=2 │
             选1 │        选2 │        选3 │
                 ▼            ▼            ▼
          bt(1, [1])    bt(2, [2])    bt(3, [3])
          收集 [1]      收集 [2]      收集 [3]
          ┌──────┐          │
      i=1 │  i=2 │      i=2 │
      选2 │  选3 │      选3 │
          ▼      ▼          ▼
   bt(2,[1,2]) bt(3,[1,3]) bt(3,[2,3])
   收集[1,2]   收集[1,3]   收集[2,3]
      │
  i=2 │选3
      ▼
 bt(3,[1,2,3])
 收集[1,2,3]
```

**收集顺序**: `[], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]`
共 **8** 个子集 = 2^3

### 详细回溯过程 (展示 push/pop)

```
backtrack(start=0, path=[])
│  收集 path=[] → result = [[]]
│
├── i=0: push(1), path=[1]
│   backtrack(start=1, path=[1])
│   │  收集 [1] → result = [[], [1]]
│   │
│   ├── i=1: push(2), path=[1,2]
│   │   backtrack(start=2, path=[1,2])
│   │   │  收集 [1,2] → result = [[], [1], [1,2]]
│   │   │
│   │   └── i=2: push(3), path=[1,2,3]
│   │       backtrack(start=3, path=[1,2,3])
│   │       │  收集 [1,2,3]
│   │       │  start=3 >= n=3, for循环不执行, 返回
│   │       pop(3), path=[1,2]    ← 回溯!
│   │
│   │   pop(2), path=[1]          ← 回溯!
│   │
│   └── i=2: push(3), path=[1,3]
│       backtrack(start=3, path=[1,3])
│       │  收集 [1,3], 返回
│       pop(3), path=[1]          ← 回溯!
│
│   pop(1), path=[]               ← 回溯!
│
├── i=1: push(2), path=[2]
│   backtrack(start=2, path=[2])
│   │  收集 [2]
│   │
│   └── i=2: push(3), path=[2,3]
│       backtrack(start=3, path=[2,3])
│       │  收集 [2,3], 返回
│       pop(3), path=[2]          ← 回溯!
│
│   pop(2), path=[]               ← 回溯!
│
└── i=2: push(3), path=[3]
    backtrack(start=3, path=[3])
    │  收集 [3], 返回
    pop(3), path=[]               ← 回溯!

最终 result: [[], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]]
```

### 算法模板

```
backtrack(start, path):
    result.add(path 的拷贝)       // 每个节点都收集!
    for i = start to n-1:
        path.push(nums[i])        // 做选择
        backtrack(i + 1, path)    // 递归 (i+1 避免重复)
        path.pop()                // 撤销选择 (回溯!)
```

### 三个关键设计决策

**Q1: 为什么 `start` 从 `i+1` 开始?**
因为子集是**无序的**，`[1,2]` 和 `[2,1]` 是同一个子集。通过 `start` 保证只向后选择，避免重复。

**Q2: 为什么每个节点都收集，而不是只在叶子节点收集?**
因为子集可以是**任意长度**的。空集、单元素、双元素...都是合法子集。如果只在叶子收集，就变成了"排列"问题。

**Q3: 收集的是 path 的拷贝还是引用?**
必须是**拷贝**! C++ 的 `push_back(path)` 自动拷贝。但 Java/Python 中添加的是引用，需要 `new ArrayList<>(path)` 或 `path[:]`。

---

## 解法二: 二进制枚举法

### 核心思想

n 个元素有 2^n 个子集。用 0 到 2^n-1 的二进制数来表示每个子集: 第 i 位为 1 表示选择 `nums[i]`，为 0 表示不选。

### 完整枚举表 (nums = [1, 2, 3])

```
掩码  二进制  bit0(1?) bit1(2?) bit2(3?)  子集
 0    000      0        0        0       []
 1    001      1        0        0       [1]
 2    010      0        1        0       [2]
 3    011      1        1        0       [1,2]
 4    100      0        0        1       [3]
 5    101      1        0        1       [1,3]
 6    110      0        1        1       [2,3]
 7    111      1        1        1       [1,2,3]
```

### 位运算判断

```
如何判断 mask=5(101) 中哪些位是 1?

(5 >> 0) & 1 = (101 >> 0) & 1 = 101 & 001 = 1  → 选 nums[0]=1  ✓
(5 >> 1) & 1 = (010)      & 1 = 010 & 001 = 0  → 不选 nums[1]=2
(5 >> 2) & 1 = (001)      & 1 = 001 & 001 = 1  → 选 nums[2]=3  ✓

子集: [1, 3]
```

### 代码

```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    int total = 1 << n;  // 2^n
    vector<vector<int>> result;
    for (int mask = 0; mask < total; mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subset.push_back(nums[i]);
            }
        }
        result.push_back(subset);
    }
    return result;
}
```

**优点**: 代码简洁，不需要递归
**缺点**: n 不能太大 (受限于整数位数)，本题 n<=10 完全没问题

---

## 解法三: 迭代法 (逐步构建)

### 核心思想

从空集开始，每遇到一个新元素，就把已有的所有子集各复制一份，并在拷贝中加入新元素。

### 构建过程可视化

```
初始:  result = [[]]                            (1个子集)

处理 num=1:
  已有: [[]]
  复制并加1: [[1]]
  合并 → result = [[], [1]]                     (2个子集)
                    ↑新增

处理 num=2:
  已有: [[], [1]]
  复制并加2: [[2], [1,2]]
  合并 → result = [[], [1], [2], [1,2]]         (4个子集)
                            ↑新增  ↑新增

处理 num=3:
  已有: [[], [1], [2], [1,2]]
  复制并加3: [[3], [1,3], [2,3], [1,2,3]]
  合并 → result = [[], [1], [2], [1,2],         (8个子集)
                   [3], [1,3], [2,3], [1,2,3]]
                   ↑新增  ↑新增  ↑新增   ↑新增

每步翻倍: 1 → 2 → 4 → 8 = 2^3 ✓
```

### 代码

```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> result = {{}};
    for (int num : nums) {
        int size = result.size();
        for (int i = 0; i < size; i++) {
            vector<int> newSubset = result[i];
            newSubset.push_back(num);
            result.push_back(newSubset);
        }
    }
    return result;
}
```

---

## 回溯法模式总结: 子集 vs 组合 vs 排列

| 问题 | 关键区别 | start 参数 | 收集时机 | 决策树形状 |
|------|---------|-----------|---------|-----------|
| **子集** (78/LCR079) | 无序, 任意长度 | 从 i+1 | **每个节点** | 每层"选/不选" |
| **组合** (77) | 无序, 固定长度 k | 从 i+1 | path.size()==k | 同上但只收叶子 |
| **排列** (46) | **有序**, 全排列 | 无 start, 用 visited | path.size()==n | 每层选未用过的 |
| **子集II** (90) | 有重复元素 | 排序+跳相同 | 每个节点 | 同层跳重复 |

### 回溯法通用模板

```
void backtrack(选择列表, 路径, 结果):
    if 满足收集条件:
        结果.add(路径的拷贝)
        // 如果是叶子节点问题, 这里 return
    for 选择 in 选择列表:
        if 该选择不合法: continue   // 剪枝
        做选择 (路径.add(选择))
        backtrack(下一层选择列表, 路径, 结果)
        撤销选择 (路径.remove(选择))
```

---

## 复杂度分析

| 方面 | 复杂度 | 说明 |
|------|--------|------|
| 子集总数 | 2^n | n 最大为 10，最多 1024 个 |
| 时间 | O(n * 2^n) | 2^n 个子集, 每个平均长度 n/2, 复制需 O(n) |
| 空间 | O(n) | 递归栈深度 (不计输出空间) |

三种解法的时间复杂度相同，都是 O(n * 2^n)。

---

## 完整代码 (C++ 回溯法)

```cpp
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        result.push_back(path);  // 每个节点都收集

        for (int i = start; i < (int)nums.size(); i++) {
            path.push_back(nums[i]);               // 做选择
            backtrack(nums, i + 1, path, result);  // 递归
            path.pop_back();                        // 撤销选择 (回溯)
        }
    }
};
```

### Python

```python
class Solution:
    def subsets(self, nums: List[int]) -> List[List[int]]:
        result = []

        def backtrack(start, path):
            result.append(path[:])  # 注意: path[:] 是拷贝!
            for i in range(start, len(nums)):
                path.append(nums[i])
                backtrack(i + 1, path)
                path.pop()

        backtrack(0, [])
        return result
```

---

## 常见错误与陷阱

| 错误 | 说明 |
|------|------|
| 只在叶子收集 | 子集要在**每个节点**收集，不是只在叶子 |
| push 了忘 pop | 必须在递归返回后撤销选择 (回溯的核心!) |
| 收集引用而非拷贝 | Java: `new ArrayList<>(path)`, Python: `path[:]` |
| 子集/子集II 搞混 | 有重复元素(LC 90)需排序 + `if(i>start && nums[i]==nums[i-1]) continue` |
| start 写成 0 | 每次都从 0 开始会产生重复子集如 [1,2] 和 [2,1] |

---

## 面试沟通建议

1. **先确认**: "元素互不相同对吧?" 如果有重复就是 LC 90
2. **画决策树**: 在白板上画 `[1,2,3]` 的树，展示回溯过程
3. **提三种方法**: 回溯、位运算、迭代，说各自优缺点
4. **分析复杂度**: 2^n 个子集是无法避免的，O(n * 2^n) 已是最优

---

## 面试追问

1. **"数组有重复元素怎么办?"** → LC 90 Subsets II: 先排序，在回溯中 `if (i > start && nums[i] == nums[i-1]) continue` 跳过同层重复。
2. **"子集长度恰好为 k?"** → LC 77 Combinations: 收集条件改为 `path.size() == k`，并在 `path.size() > k` 时剪枝。
3. **"子集元素和恰好为 target?"** → LC 39/40: 在回溯中累加 sum，当 `sum > target` 时剪枝。
4. **"为什么每个节点都收集而不是只在叶子收集?"** → 子集可以是任意长度。只在叶子收集就变成了排列/组合问题。

## Follow-up 拓展

**Q: 数组有重复元素? (LC 90 Subsets II)**
- 先排序，在回溯中跳过同层相同元素:
```cpp
if (i > start && nums[i] == nums[i-1]) continue;
```

**Q: 子集长度恰好为 k? (LC 77 Combinations)**
- 收集条件改为 `path.size() == k`，并在 `path.size() > k` 时提前返回(剪枝)

**Q: 子集元素和恰好为 target? (LC 39/40 Combination Sum)**
- 在回溯中累加 sum，当 `sum > target` 时剪枝

**Q: 如何不用递归实现回溯?**
- 用显式栈模拟递归调用栈，但代码可读性较差
