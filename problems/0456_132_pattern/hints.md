# 456. 132 模式 (132 Pattern)

## 题目理解

给定整数数组 `nums`，判断是否存在下标 `i < j < k` 使得 `nums[i] < nums[k] < nums[j]`。

这个模式叫 "132"：
- `nums[i]` 是 "1"（最小值）
- `nums[j]` 是 "3"（最大值）  
- `nums[k]` 是 "2"（中间值）
- **位置顺序**：1 在最左，3 在中间，2 在最右
- **大小顺序**：1 < 2 < 3

```
示例 1: nums = [1,2,3,4] → false  (单调递增，不存在 132 模式)
示例 2: nums = [3,1,4,2] → true   (1,4,2 是 132 模式)
示例 3: nums = [-1,3,2,0] → true  (-1,3,2), (-1,3,0), (-1,2,0) 都是
```

---

## 核心思路

**从右往左遍历 + 单调递减栈**：

- 栈维护 "3" 的候选（`nums[j]`，较大值）
- 被弹出的最大值作为 "2" 的候选（`nums[k]`，中间值）
- 当前元素如果小于 "2"，就找到了 "1"（`nums[i]`，最小值）

---

## 思维链：从暴力到最优

### Level 0：暴力三重循环 O(n^3)

```python
for i in range(n):
    for j in range(i+1, n):
        for k in range(j+1, n):
            if nums[i] < nums[k] < nums[j]:
                return True
```

太慢，不可接受。

### Level 1：优化到 O(n^2) -- 固定 j

固定 "3"（最大值 `nums[j]`），向左找最小的 "1"，向右找满足条件的 "2"：

```python
for j in range(1, n-1):
    left_min = min(nums[0:j])          # "1" 的候选
    for k in range(j+1, n):
        if left_min < nums[k] < nums[j]:  # 找到了!
            return True
```

可以预计算前缀最小值优化 `left_min`，但仍然 O(n^2)。

### Level 2：单调栈 O(n) -- 关键突破

**核心问题**：怎么同时维护三个角色的关系？

**从右往左遍历的直觉**：
- 当我们从右往左扫描时，当前元素可能是 "1"（nums[i]）
- 我们需要在它的右边已经有一对 "3" > "2" 的组合
- 用单调递减栈维护右边的候选 "3"，被弹出的值就是 "2" 的候选

**为什么单调递减栈有效？**

```
从右到左遍历时，栈中元素从栈底到栈顶递减。
当遇到一个大数 nums[i]，它会弹出栈中比它小的元素。
弹出的元素就是 "2" (nums[k]) 的候选——它们比当前数小但在右边。
我们记录弹出的最大值 third 作为当前最优的 "2"。
如果未来某个更小的数 < third，就找到了 "1"!
```

---

## 单调栈状态详细推演

### 推演 1: nums = [3, 1, 4, 2]

从右往左遍历（i = 3, 2, 1, 0）：

```
初始: stack = [], third = -INF

i=3, nums[3]=2:
  nums[3]=2 > third=-INF? NO → 不是 "1"
  栈为空，无需弹出
  push 2
  stack = [2]  third = -INF
  栈状态: bottom [2] top

i=2, nums[2]=4:
  nums[2]=4 > third=-INF? NO → 不是 "1"  
  4 > stack.top()=2 → 弹出 2, third = max(-INF, 2) = 2
  push 4
  stack = [4]  third = 2
  栈状态: bottom [4] top
  ※ 此时 third=2 表示我们有一对 (4, 2) 即 "3"=4, "2"=2

i=1, nums[1]=1:
  nums[1]=1 < third=2? YES → 找到 "1"!
  返回 true
  ※ 132 模式: nums[1]=1 < nums[3]=2 < nums[2]=4 → 1 < 2 < 4 ✓
```

### 推演 2: nums = [1, 2, 3, 4]（单调递增，应返回 false）

```
初始: stack = [], third = -INF

i=3, nums[3]=4:
  4 > -INF? NO
  push 4
  stack = [4]  third = -INF

i=2, nums[2]=3:
  3 > -INF? NO
  3 < stack.top()=4 → 不弹出
  push 3
  stack = [4, 3]  third = -INF
  栈状态: bottom [4, 3] top  (递减 ✓)

i=1, nums[1]=2:
  2 > -INF? NO
  2 < stack.top()=3 → 不弹出
  push 2
  stack = [4, 3, 2]  third = -INF
  栈状态: bottom [4, 3, 2] top

i=0, nums[0]=1:
  1 > -INF? NO
  1 < stack.top()=2 → 不弹出
  push 1
  stack = [4, 3, 2, 1]  third = -INF

遍历结束，返回 false ✓
※ 单调递增序列，栈始终保持递减，没有元素被弹出，third 永远是 -INF
```

### 推演 3: nums = [-1, 3, 2, 0]（多个 132 模式）

```
初始: stack = [], third = -INF

i=3, nums[3]=0:
  0 > -INF? NO
  push 0
  stack = [0]  third = -INF

i=2, nums[2]=2:
  2 > -INF? NO
  2 > stack.top()=0 → 弹出 0, third = max(-INF, 0) = 0
  push 2
  stack = [2]  third = 0
  ※ 此时有一对: "3"=2, "2"=0

i=1, nums[1]=3:
  3 > third=0? NO (3 > 0 但条件是 < 不是 >)
  ※ 等等，判断条件是 nums[i] < third，3 < 0? NO
  3 > stack.top()=2 → 弹出 2, third = max(0, 2) = 2
  push 3
  stack = [3]  third = 2
  ※ 此时有一对: "3"=3, "2"=2

i=0, nums[0]=-1:
  -1 < third=2? YES → 找到 "1"!
  返回 true
  ※ 132 模式: nums[0]=-1, nums[1]=3, nums[2]=2 → -1 < 2 < 3 ✓
```

### 推演 4: nums = [3, 5, 0, 3, 4]

```
初始: stack = [], third = -INF

i=4, nums[4]=4:
  push 4 → stack = [4], third = -INF

i=3, nums[3]=3:
  3 < stack.top()=4 → 不弹出
  push 3 → stack = [4, 3], third = -INF
  栈: bottom [4, 3] top

i=2, nums[2]=0:
  0 < 3 → 不弹出
  push 0 → stack = [4, 3, 0], third = -INF
  栈: bottom [4, 3, 0] top

i=1, nums[1]=5:
  5 > stack.top()=0 → 弹出 0, third = 0
  5 > stack.top()=3 → 弹出 3, third = 3
  5 > stack.top()=4 → 弹出 4, third = 4
  push 5 → stack = [5], third = 4
  栈: bottom [5] top
  ※ 此时: "3"=5, "2"=4 (最大被弹出值)

i=0, nums[0]=3:
  3 < third=4? YES → 找到 "1"!
  返回 true
  ※ 132 模式: 3 < 4 < 5 ✓ (位置 0, 1, 4)
```

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐度 |
|------|------|------|------|--------|
| 单调递减栈（从右到左） | 栈存"3"候选，弹出最大值为"2" | O(n) | O(n) | 强烈推荐 |
| 前缀最小值 + 遍历 | 预处理left_min，枚举j找k | O(n^2) | O(n) | 理解用 |
| TreeSet / 有序集合 | 维护右侧元素，二分查找 | O(nlogn) | O(n) | 了解即可 |

---

## 关键提示

### 提示 1：从右往左遍历 + 单调递减栈

```
当前元素 → 候选 "1" (nums[i])
栈中元素 → 候选 "3" (nums[j])，从栈底到栈顶递减
被弹出的最大值 → 候选 "2" (nums[k])，记为 third
```

### 提示 2：third 变量记录弹出的最大值

`third` 代表当前最优的 "2"（nums[k]）。它是被某个更大的 "3" 弹出的，所以一定满足 "2" < "3"。当有元素 < third 时，"1" < "2" < "3" 成立。

### 提示 3：为什么不从左到右？

从左到右时：
- "1" (最小值) 好维护 → 前缀最小值
- 但同时维护 "3" 和 "2" 的 "3" > "2" 关系很困难
- 从右到左时，栈天然维护了 "3" > "2" 的关系

### 提示 4：每个元素最多入栈出栈各一次

所以虽然有 while 循环，总体时间仍是 O(n)。

---

## 易错点

### 1. third 初始化为 INT_MIN 而非 0

```cpp
// 错误: nums[i] 可能是负数
int third = 0;

// 正确: 用 INT_MIN
int third = INT_MIN;
```

### 2. 判断条件的方向

```cpp
// 错误: 把 < 和 > 搞反
if (nums[i] > third) return true;  // 这是找 "3" 不是 "1"

// 正确: 当前元素 < third 才是找到了 "1"
if (nums[i] < third) return true;
```

### 3. 弹出时更新 third 用 max

```cpp
// 错误: 直接赋值，可能把 third 变小
third = stk.top();

// 正确: 取最大值
third = max(third, stk.top());
```

### 4. 遍历方向

```cpp
// 错误: 从左到右
for (int i = 0; i < n; i++) ...

// 正确: 从右到左
for (int i = n - 1; i >= 0; i--) ...
```

---

## 为什么 third 只记录最大值就够了？

**直觉**：third 越大，越容易找到比它小的 "1"。

**严格证明**：
- 假设弹出了元素 a 和 b（a < b），对应的 "3" 都在栈中
- third = max(a, b) = b
- 如果后续某个元素 x < a < b，那 x < third 也成立
- 如果 a < x < b，那 x < third 也成立
- 只有 x > b 时 x 不满足 < b，但此时 x 也不满足 < a

所以记录最大值不会漏掉任何解。

---

## 拓展思考

### 面试追问

1. **如何输出具体的 (i, j, k) 三元组？**
   → 在栈中存 pair<value, index>，third 也记录 index

2. **如果改成 "123" 模式（i < j < k, nums[i] < nums[j] < nums[k]）？**
   → 用前缀最小值 + 后缀最大值，一次扫描即可

3. **如果改成 "321" 模式（i < j < k, nums[i] > nums[j] > nums[k]）？**
   → 用从左到右的单调递增栈

4. **数组中有多少个 132 模式？（计数版）**
   → 更复杂，需要 BIT/归并排序，O(nlogn)

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 496 | 下一个更大元素 I | 单调栈入门 |
| 503 | 下一个更大元素 II | 环形单调栈 |
| 739 | 每日温度 | 单调递减栈经典应用 |
| 84 | 柱状图中最大的矩形 | 单调栈高级应用 |
| 42 | 接雨水 | 单调栈/双指针 |
| 334 | 递增的三元子序列 | "123" 模式变体 |
