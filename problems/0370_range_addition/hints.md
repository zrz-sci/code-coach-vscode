# 370. 区间加法

## 题目理解

给定长度为 `n` 的全零数组和 `k` 个更新操作 `[startIdx, endIdx, inc]`，每个操作将 `arr[startIdx..endIdx]` 区间内所有元素增加 `inc`。返回所有操作后的数组。

**本质**：这是"差分数组"的经典应用场景。朴素方法每次操作 O(n)，k 次操作 O(kn)。差分数组将每次操作降为 O(1)，最后一次前缀和还原，总共 O(n + k)。

```
示例: length = 5, updates = [[1,3,2],[2,4,3],[0,2,-2]]

初始:     [0, 0, 0, 0, 0]
操作1:    [0, 2, 2, 2, 0]
操作2:    [0, 2, 5, 5, 3]
操作3:    [-2, 0, 3, 5, 3]
```

---

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| ⭐ 差分数组 | 起点 +inc，终点+1 -inc，前缀和还原 | O(n + k) | O(n) | 面试首选 |
| 暴力模拟 | 每个操作遍历区间 | O(n * k) | O(n) | 不推荐 |

---

## 渐进提示

### 提示 1 -- 暴力思路与瓶颈

<details>
<summary>展开</summary>

朴素方法：对每个操作 `[start, end, inc]`，遍历 `arr[start..end]` 逐个加 `inc`。

```
for each update [start, end, inc]:
    for i = start to end:
        arr[i] += inc
```

时间 O(n * k)，当 n = 10^5, k = 10^4 时最坏 10^9，超时。

**观察**：每个操作影响的是一个连续区间。有没有办法用 O(1) 记录一个区间操作？

</details>

### 提示 2 -- 差分数组的核心思想

<details>
<summary>展开</summary>

**差分数组** `diff[i]` 表示 `arr[i] - arr[i-1]`（arr[-1] = 0）。

如果要对区间 `[start, end]` 加 `inc`：
- `diff[start] += inc` —— 从 start 开始增加
- `diff[end+1] -= inc` —— 从 end+1 开始取消增加

最后对 `diff` 做前缀和就得到最终的 `arr`。

```
为什么有效？

arr: [0, 0, 0, 0, 0]    操作 [1, 3, 2]
diff:[0, 0, 0, 0, 0]

diff[1] += 2  =>  [0, 2, 0, 0, 0]
diff[4] -= 2  =>  [0, 2, 0, 0, -2]

前缀和: [0, 2, 2, 2, 0]  ✓ (恰好是 arr[1..3] 加了 2)
```

**直觉**：diff[start] 标记"从这里开始加 inc"，diff[end+1] 标记"从这里开始不再加 inc"。前缀和操作将这些标记"展开"成实际值。

</details>

### 提示 3 -- 完整算法

<details>
<summary>展开</summary>

```
1. 创建 diff[0..n-1] = {0}
2. 对每个操作 [start, end, inc]:
     diff[start] += inc
     if end + 1 < n:
         diff[end + 1] -= inc
3. 前缀和还原:
     for i = 1 to n-1:
         diff[i] += diff[i-1]
4. 返回 diff (此时 diff 即为最终数组)
```

**注意边界**：当 `end + 1 == n` 时，不需要 `diff[end+1] -= inc`（数组越界，且不影响结果）。

</details>

### 提示 4 -- 多操作叠加原理

<details>
<summary>展开</summary>

差分数组支持叠加：多个操作各自在 diff 上做标记，前缀和一次性还原。

```
length = 5
操作1: [1,3,2]  -> diff: [0, +2, 0, 0, -2]
操作2: [2,4,3]  -> diff: [0, +2, +3, 0, -2]  (end+1=5 越界,省略)
操作3: [0,2,-2] -> diff: [-2, +2, +3, +2, -2]

前缀和:
  diff[0] = -2
  diff[1] = -2+2 = 0
  diff[2] = 0+3 = 3
  diff[3] = 3+2 = 5
  diff[4] = 5+(-2) = 3

结果: [-2, 0, 3, 5, 3] ✓
```

</details>

---

## 关键思维模式

### 1. "差分数组"模式

区间加减操作的利器：O(1) 修改，O(n) 还原。

```
核心公式:
  diff[start] += inc     // 标记起点
  diff[end+1] -= inc     // 标记终点后一位(取消)
  arr = prefix_sum(diff) // 前缀和还原
```

适用场景：多次区间修改 + 最终查询。

### 2. "标记 + 展开"模式

不直接修改数组，而是在端点做标记，最后一次性展开。

类似思想还有：
- 扫描线算法（标记 +1/-1）
- 事件驱动的区间合并

### 3. 差分 vs 前缀和（互逆操作）

```
原数组 arr:     [a0, a1, a2, a3]
差分 diff:      [a0, a1-a0, a2-a1, a3-a2]
prefix_sum(diff) = [a0, a1, a2, a3] = arr

差分是前缀和的逆操作!
```

---

## 完整代码 (C++)

```cpp
class Solution {
public:
    vector<int> getModifiedArray(int length, vector<vector<int>>& updates) {
        vector<int> diff(length, 0);

        // 差分标记
        for (auto& update : updates) {
            int start = update[0], end = update[1], inc = update[2];
            diff[start] += inc;
            if (end + 1 < length) {
                diff[end + 1] -= inc;
            }
        }

        // 前缀和还原
        for (int i = 1; i < length; i++) {
            diff[i] += diff[i - 1];
        }

        return diff;
    }
};
```

---

## 算法图解

```
length = 5, updates = [[1,3,2],[2,4,3],[0,2,-2]]

Step 1: 差分标记 (每个操作 O(1))

操作 [1,3,2]:
  diff[1] += 2, diff[4] -= 2
  diff = [0, +2, 0, 0, -2]

操作 [2,4,3]:
  diff[2] += 3, diff[5] 越界省略
  diff = [0, +2, +3, 0, -2]

操作 [0,2,-2]:
  diff[0] += -2, diff[3] -= -2 (即 +2)
  diff = [-2, +2, +3, +2, -2]

Step 2: 前缀和还原
  i=0: diff[0] = -2
  i=1: diff[1] = -2 + 2 = 0
  i=2: diff[2] = 0 + 3 = 3
  i=3: diff[3] = 3 + 2 = 5
  i=4: diff[4] = 5 + (-2) = 3

最终: [-2, 0, 3, 5, 3] ✓

对比暴力:
  暴力: 3个操作 × 平均3个元素 = 9次修改
  差分: 3个操作 × 2次标记 + 1次前缀和 = 6+4 = 10次操作
  当 n,k 很大时差分优势明显: O(n+k) vs O(nk)
```

---

## 常见错误

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 忘记边界检查 `end+1 < length` | ✓ 只在 end+1 < length 时减 | 数组越界 |
| ✗ `diff[end] -= inc` | ✓ `diff[end+1] -= inc` | 是 end 的下一个位置，不是 end |
| ✗ 前缀和从 i=0 开始 | ✓ 从 i=1 开始 | diff[0] 本身就是正确值 |
| ✗ 差分标记后直接返回 diff | ✓ 必须做前缀和还原 | diff 只是标记，不是最终值 |

---

## 面试追问

**Q1: 如果不只是区间加法，还有区间赋值呢？**
> 区间赋值不能用简单差分数组。需要更高级的数据结构如线段树（Segment Tree）配合懒传播（Lazy Propagation）。

**Q2: 如果操作后还需要多次区间查询呢？**
> 差分数组适合"多次修改 + 一次全量查询"。如果需要多次区间查询，考虑树状数组（BIT）或线段树，支持 O(log n) 的单次修改和查询。

**Q3: 差分数组能处理二维区间修改吗？**
> 可以。二维差分数组对矩形区域 `(r1,c1)-(r2,c2)` 加 `inc`：在四个角做标记 (`+inc, -inc, -inc, +inc`)，然后做二维前缀和还原。类似 LC 304 的二维前缀和的逆操作。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 303 | Range Sum Query - Immutable | 前缀和（差分的逆操作） |
| 304 | Range Sum Query 2D | 二维前缀和 |
| 1094 | Car Pooling | 差分数组应用 |
| 1109 | Corporate Flight Bookings | 差分数组应用 |
