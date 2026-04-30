# 373. 查找和最小的K对数字

## 核心思路

本题是经典的 **多路合并 + 最小堆** 问题。两个有序数组 `nums1` 和 `nums2` 形成一个"虚拟矩阵"，其中 `matrix[i][j] = nums1[i] + nums2[j]`。每行和每列都是递增的。问题转化为：从这个有序矩阵中找最小的 k 个元素。这和"合并 K 个有序链表"是同一个模型。

## 思维链

1. **读完题第一反应**：枚举所有 `n * m` 个配对，排序后取前 k 个。但 `n, m` 可达 10^5，总配对数 10^10，完全不可行。

2. **关键观察 — 有序矩阵结构**：由于 `nums1` 和 `nums2` 都是非递减的，配对和形成一个矩阵，其中每行递增、每列递增：
   ```
           nums2[0]  nums2[1]  nums2[2]
   nums1[0]   2         4         6      ← 最小行
   nums1[1]   8        10        12
   nums1[2]  12        14        16      ← 最大行
   ```
   每行是一个有序序列。问题变成：从 n 个有序序列中，找总体最小的 k 个元素。

3. **类比"合并 K 个有序链表"**：每行相当于一个有序链表。用**最小堆**维护每个"链表"的当前头部。每次弹出全局最小值，然后将该链表的下一个元素加入堆。

4. **初始化**：将每行的第一个元素 `(nums1[i] + nums2[0], i, 0)` 入堆。因为 k <= 10^4，只需入堆 `min(n, k)` 个（后面的行不可能贡献前 k 小）。

5. **为什么弹出 (i,j) 后只推进 j 而不推进 i？** 因为 (i+1, j) 在初始化时已经入堆了（或者通过 (i+1, 0) → (i+1, 1) → ... → (i+1, j) 的路径被推进到堆中）。而 (i, j+1) 还没入堆，需要我们手动推入。

```
多路合并示意 (nums1=[1,7,11], nums2=[2,4,6], k=3):

虚拟矩阵:
          2    4    6
   1  →  [3]   5    7    ← 链表0: 3→5→7
   7  →  [9]  11   13    ← 链表1: 9→11→13
  11  →  [13]  15   17    ← 链表2: 13→15→17
         ↑
         初始堆中的头部

初始堆: [(3,0,0), (9,1,0), (13,2,0)]

Step1: 弹出 (3, 0, 0) → 结果加入 (1,2)
       推入 (1+4=5, 0, 1) → 堆: [(5,0,1), (9,1,0), (13,2,0)]

Step2: 弹出 (5, 0, 1) → 结果加入 (1,4)
       推入 (1+6=7, 0, 2) → 堆: [(7,0,2), (9,1,0), (13,2,0)]

Step3: 弹出 (7, 0, 2) → 结果加入 (1,6)
       j+1=3 >= nums2.size()=3, 不推入

结果: [[1,2], [1,4], [1,6]] ✓
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力排序 | 枚举所有配对排序 | O(nm * log(nm)) | O(nm) | 不可行 |
| 最小堆（多路合并） | 初始化 min(n,k) 个头部，每次弹出后推进 | O(k * log(min(n,k))) | O(min(n,k)) | ⭐ 必须写出 |

## 关键提示

1. **"合并 K 个有序链表"的抽象**：把 `nums1` 的每个元素看作一条有序链表的起点：`nums1[i] + nums2[0], nums1[i] + nums2[1], ...`。共 n 条链表，每条长度 m。用最小堆做多路合并。

2. **初始化只入堆 min(n, k) 个**：`nums1[i] + nums2[0]` 对于 `i >= k` 的行，即使整行最小值也不可能进入前 k 个（因为前 k 行的最小值都更小）。所以只需初始化 `min(n, k)` 个元素。

3. **弹出 (i,j) 后只推进 j**：
   - `(i, j+1)` 还没在堆中 → 需要推入
   - `(i+1, j)` 已经在初始化时（通过 `(i+1, 0)`）或后续推进中被加入堆了 → 不需要推入
   - 这样保证**没有重复元素入堆**，无需 visited 集合

4. **堆的大小**：堆中最多有 `min(n, k)` 个元素，每次 push/pop 是 O(log(min(n,k)))。总共 k 次操作，时间 O(k * log(min(n,k)))。

5. **k 可能大于 n*m**：虽然题目约束 `k <= n*m`，但代码中仍需检查 `!pq.empty()`，否则堆空了还 pop 会报错。

## 解法详解

### 解法1: 暴力（不可行，仅说明）

**思考过程**：枚举所有 `n*m` 个配对，排序取前 k 个。n, m 可达 10^5 时有 10^10 个配对，完全不可行。即使 k 很小，也无法提前终止排序。

### 解法2: 最小堆 — 多路合并（最优解）— O(k log min(n,k)) / O(min(n,k)) ⭐

**思考过程**：
- 将问题转化为"从 n 个有序序列中找全局前 k 小"。
- 最小堆维护各序列的当前最小值。初始化时放入每个序列的头部。
- 每次弹出全局最小值，将其在原序列中的下一个元素推入堆。
- 重复 k 次。

```cpp
class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<vector<int>> res;
        if (nums1.empty() || nums2.empty()) return res;
        
        int n = nums1.size(), m = nums2.size();
        
        // 最小堆: {sum, i, j}
        auto cmp = [](const tuple<int,int,int>& a, const tuple<int,int,int>& b) {
            return get<0>(a) > get<0>(b); // 小顶堆
        };
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, decltype(cmp)> pq(cmp);
        
        // 初始化: 将每行的第一个元素入堆，最多 k 个
        for (int i = 0; i < min(n, k); i++) {
            pq.push({nums1[i] + nums2[0], i, 0});
        }
        
        // 弹出 k 个最小值
        while (k-- > 0 && !pq.empty()) {
            auto [sum, i, j] = pq.top();
            pq.pop();
            res.push_back({nums1[i], nums2[j]});
            
            // 推进 j: 将同一行的下一个元素入堆
            if (j + 1 < m) {
                pq.push({nums1[i] + nums2[j + 1], i, j + 1});
            }
        }
        return res;
    }
};
```

**为什么不会漏掉配对？**

考虑矩阵中任意一个格子 (i, j)，它被访问到的路径是：
```
(i, 0) → (i, 1) → ... → (i, j)
```
`(i, 0)` 在初始化时入堆，之后每次弹出 `(i, j)` 时推入 `(i, j+1)`。所以沿着每一行，元素是按顺序被考虑的。

而行间的顺序由堆自动维护。堆中始终保持了"每行尚未被弹出的最小元素"，每次弹出全局最小的那个，所以不会漏掉。

### 解法3: 最小堆 — 另一种初始化（仅推入第一个配对）

```cpp
class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<vector<int>> res;
        if (nums1.empty() || nums2.empty()) return res;
        
        int n = nums1.size(), m = nums2.size();
        
        // 用 set 去重（因为 (i+1,j) 和 (i,j+1) 可能被同时推入）
        set<pair<int,int>> visited;
        
        auto cmp = [&](const pair<int,int>& a, const pair<int,int>& b) {
            return nums1[a.first] + nums2[a.second] > nums1[b.first] + nums2[b.second];
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
        
        pq.push({0, 0});
        visited.insert({0, 0});
        
        while (k-- > 0 && !pq.empty()) {
            auto [i, j] = pq.top();
            pq.pop();
            res.push_back({nums1[i], nums2[j]});
            
            // 推入 (i+1, j) 和 (i, j+1)
            if (i + 1 < n && !visited.count({i+1, j})) {
                pq.push({i+1, j});
                visited.insert({i+1, j});
            }
            if (j + 1 < m && !visited.count({i, j+1})) {
                pq.push({i, j+1});
                visited.insert({i, j+1});
            }
        }
        return res;
    }
};
```

**注意**：这种方式需要 `visited` 集合防止重复入堆。相比解法2（只推进 j，无需 visited）更复杂。面试推荐解法2。

## 解法对比

| 维度 | 解法2（推进 j） | 解法3（推进 i 和 j） |
|------|----------------|---------------------|
| 时间复杂度 | O(k log min(n,k)) | O(k log k) |
| 是否需要 visited | 不需要 | 需要 set 去重 |
| 堆大小 | min(n, k) | 最大 2k |
| 代码复杂度 | 简洁 | 较复杂 |
| 面试推荐 | 首选 | 作为补充说明 |

## 易错点

1. **初始化入堆个数不对**：应该是 `min(n, k)` 而非 `n`。如果 `n = 10^5` 且 `k = 3`，入堆 10^5 个元素完全浪费。

2. **弹出后推进 i 而非 j**：
   ```cpp
   // 错误: 推进 i → (i+1, j) 可能和初始化的 (i+1, 0) 重复
   pq.push({nums1[i+1] + nums2[j], i+1, j});
   
   // 正确: 推进 j → (i, j+1) 不会和任何已有元素重复
   pq.push({nums1[i] + nums2[j+1], i, j+1});
   ```

3. **j+1 越界**：推入 `(i, j+1)` 前必须检查 `j + 1 < m`。否则 `nums2[j+1]` 越界。

4. **k 大于实际配对数**：当所有配对都弹出后堆可能为空。while 条件必须包含 `!pq.empty()`。

5. **堆的比较函数方向搞反**：
   ```cpp
   // 错误: 大顶堆 (最大值先弹出)
   return get<0>(a) < get<0>(b);
   
   // 正确: 小顶堆 (最小值先弹出)
   return get<0>(a) > get<0>(b);
   ```
   C++ 的 `priority_queue` 默认是大顶堆，自定义比较器中 `a > b` 才是小顶堆。

6. **sum 溢出**：`nums1[i] + nums2[j]` 可能溢出 int（各自可达 10^9）。使用 `long long` 更安全，虽然题目约束下不会溢出。

## 面试追问

1. **"这和'合并 K 个有序链表'有什么关系？"**
   → 完全同构。`nums1` 的每个元素 `nums1[i]` 生成一条有序链表：`nums1[i]+nums2[0] ≤ nums1[i]+nums2[1] ≤ ...`。共 n 条链表，用堆做多路合并，取前 k 个。LeetCode 23 (Merge k Sorted Lists) 的同类题。

2. **"如果 nums1 和 nums2 不是有序的呢？"**
   → 先排序再用堆。排序 O(n log n + m log m)，堆操作 O(k log min(n,k))。总共 O(n log n + m log m + k log min(n,k))。

3. **"能否用二分搜索？"**
   → 可以。二分搜索目标值 `mid`，计算有多少配对和 <= mid（利用有序性可以 O(n+m) 算出）。找到使得恰好有 k 个配对和 <= mid 的 `mid` 值。但构造具体的 k 个配对需要额外工作。时间 O((n+m) * log(value_range))，在某些场景下可能更优。

4. **"堆中元素最多有多少个？"**
   → 初始化 min(n, k) 个，之后每次弹出 1 个推入最多 1 个，所以堆大小始终 <= min(n, k)。

5. **"和 LeetCode 378 (Kth Smallest Element in a Sorted Matrix) 有什么区别？"**
   → 378 是在一个 n*n 有序矩阵中找第 k 小，本题是在两个数组构成的虚拟有序矩阵中找前 k 小。本质相同，都可用堆或二分解。区别是 373 需要返回具体的配对而非仅返回值。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 23 | Merge k Sorted Lists | 多路合并的经典题，完全同构 |
| 378 | Kth Smallest Element in a Sorted Matrix | 有序矩阵找第 k 小，同一模型 |
| 719 | Find K-th Smallest Pair Distance | 第 k 小的配对距离，二分 + 双指针 |
| 786 | K-th Smallest Prime Fraction | 有序矩阵 + 堆/二分 |
| 264 | Ugly Number II | 多路合并的另一应用 |
| 313 | Super Ugly Number | 多路合并 + 堆 |
| 668 | Kth Smallest Number in Multiplication Table | 乘法表 = 有序矩阵，二分搜索 |
