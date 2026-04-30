# 85. 最大矩形

## 核心思路

本题本质是：**把二维矩阵的"最大全1矩形"问题，逐行转化为一维的"柱状图中最大矩形"问题（LeetCode 84）**。每一行作为底边，向上统计连续1的高度形成柱状图，然后对每行的柱状图求最大矩形面积。

## 思维链

1. **读完题第一反应**：枚举所有可能的矩形，检查是否全为1。一个矩形由左上角 `(r1,c1)` 和右下角 `(r2,c2)` 确定 → 四重循环枚举 + 检查内部 → O(m²n²·mn) 太慢。

2. **用前缀和优化检查**：可以用二维前缀和将"区域求和"降到 O(1)，但枚举矩形本身仍需 O(m²n²)，对 200×200 来说大约 1.6×10⁹，勉强超时。

3. **换个角度 — 柱状图转化**：如果我们固定矩形的底边在第 i 行，那么对每一列 j，可以预算从第 i 行往上连续 1 的个数（即"高度"）。这样第 i 行就变成了一个柱状图！

4. **关键洞察**：对每一行形成的柱状图，求"柱状图中最大矩形" — 这正是 **LeetCode 84** 的经典单调栈问题，O(n) 解决。

5. **总流程**：逐行更新高度数组 heights[]，对每行调用一次 84 题的单调栈算法，取所有行的最大值。总时间 O(m·n)。

6. **另一条路 — 纯 DP**：对每个位置维护三个信息 `height[j]`, `left[j]`, `right[j]`（连续高度、该高度能向左/右延伸的边界），直接算面积，同样 O(mn)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有矩形+前缀和检查 | O(m²n²) | O(mn) | 能说出即可 |
| 柱状图+单调栈 | 逐行转84题 | O(mn) | O(n) | ⭐ 必须写出 |
| DP（left/right/height） | 逐行维护三个数组 | O(mn) | O(n) | 加分项 |

## 关键提示

- **提示1**：如果你做过 84. 柱状图中最大的矩形，想想怎么把二维问题"降维"到一维。
- **提示2**：对于每一行，把它当作"地面"，每一列向上的连续1就是柱子的高度。遇到0高度归零。
- **提示3**：单调栈的核心——维护一个递增栈，当遇到比栈顶矮的柱子时，弹出栈顶并计算以它为高度的最大矩形宽度。
- **提示4（DP 解法）**：对于每个位置 (i,j)，如果知道"以 height[j] 的高度最多能向左延伸到哪、向右延伸到哪"，就能直接算面积。

**柱状图转化示意：**
```
原始矩阵:             逐行的柱状图高度:
1 0 1 0 0             row0: [1, 0, 1, 0, 0]
1 0 1 1 1             row1: [2, 0, 2, 1, 1]
1 1 1 1 1             row2: [3, 1, 3, 2, 2]
1 0 0 1 0             row3: [4, 0, 0, 3, 0]

以 row2 为例的柱状图:
  3     3
  █  1  █  2  2
  █  █  █  █  █
  █  █  █  █  █
  █  █  █  █  █
  ───────────────
  0  1  2  3  4

最大矩形面积 = 高度2 × 宽度3 = 6 (列2~4，高度2)
```

## 解法详解

### 解法1: 暴力枚举+前缀和 — O(m²n²) / O(mn)

**思考过程**：最朴素的想法——枚举所有可能的矩形，判断其内部是否全为1。用二维前缀和可以 O(1) 判断子矩阵的和是否等于面积。

```cpp
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        // prefix[i][j] = matrix[0..i-1][0..j-1] 区域中1的个数
        vector<vector<int>> prefix(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                prefix[i][j] = (matrix[i-1][j-1] - '0')
                    + prefix[i-1][j] + prefix[i][j-1] - prefix[i-1][j-1];
        
        int ans = 0;
        // 枚举左上角 (r1,c1) 和右下角 (r2,c2)
        for (int r1 = 1; r1 <= m; r1++)
            for (int c1 = 1; c1 <= n; c1++)
                for (int r2 = r1; r2 <= m; r2++)
                    for (int c2 = c1; c2 <= n; c2++) {
                        int area = (r2 - r1 + 1) * (c2 - c1 + 1);
                        int sum = prefix[r2][c2] - prefix[r1-1][c2]
                                - prefix[r2][c1-1] + prefix[r1-1][c1-1];
                        if (sum == area) // 全为1
                            ans = max(ans, area);
                    }
        return ans;
    }
};
```

**关键点**：这个解法 O(m²n²) 在 200×200 约 1.6×10⁹，会超时。但它帮助理解题意，并且暴露瓶颈——我们在做大量重复的枚举。

---

### 解法2: 柱状图 + 单调栈 — O(mn) / O(n) ⭐ 面试首选

**从暴力到优化**：暴力的瓶颈是枚举矩形。换个思路：固定底边逐行扫描，将问题转化为 LeetCode 84（柱状图中最大矩形），用单调栈 O(n) 求解。

**核心步骤：**
1. 维护 `heights[j]`：第 j 列从当前行往上连续1的个数
2. 对每行的 `heights` 调用 84 题的单调栈算法
3. 取所有行的最大面积

```
// 单调栈处理 row2 的 heights = [3,1,3,2,2] 的过程:
//
// 栈中存下标，维护递增（栈底到栈顶高度递增）
//
// i=0: push 0          栈: [0]          heights: 3
// i=1: h[1]=1 < h[0]=3 → 弹出0, 宽度=1, 面积=3×1=3
//      栈空, push 1    栈: [1]
// i=2: h[2]=3 > h[1]=1 → push 2  栈: [1,2]
// i=3: h[3]=2 < h[2]=3 → 弹出2, 宽度=3-1-1=1, 面积=3×1=3
//      h[3]=2 > h[1]=1 → push 3  栈: [1,3]
// i=4: h[4]=2 >= h[3]=2 → push 4  栈: [1,3,4]
// 
// 清栈:
// 弹出4: 宽度=5-3-1=1, 面积=2×1=2
// 弹出3: 宽度=5-1-1=3, 面积=2×3=6  ← 最大!
// 弹出1: 宽度=5,       面积=1×5=5
//
// 该行最大面积 = 6
```

```cpp
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> heights(n, 0);
        int ans = 0;
        
        for (int i = 0; i < m; i++) {
            // 步骤1: 更新柱状图高度
            for (int j = 0; j < n; j++) {
                // 遇到1就累加，遇到0就归零（柱子断了）
                heights[j] = (matrix[i][j] == '1') ? heights[j] + 1 : 0;
            }
            // 步骤2: 对当前行的柱状图求最大矩形（84题）
            ans = max(ans, largestRectangleInHistogram(heights));
        }
        return ans;
    }
    
private:
    int largestRectangleInHistogram(vector<int>& heights) {
        int n = heights.size();
        stack<int> stk; // 存下标，维护高度递增
        int maxArea = 0;
        
        for (int i = 0; i <= n; i++) {
            // i==n 时用高度0，强制清空栈中所有元素
            int curHeight = (i == n) ? 0 : heights[i];
            
            // 当前柱子比栈顶矮 → 栈顶柱子找到了右边界
            while (!stk.empty() && curHeight < heights[stk.top()]) {
                int h = heights[stk.top()];
                stk.pop();
                // 宽度: 左边界是新栈顶的右边，右边界是 i 的左边
                int w = stk.empty() ? i : (i - stk.top() - 1);
                maxArea = max(maxArea, h * w);
            }
            stk.push(i);
        }
        return maxArea;
    }
};
```

**关键点**：
- `heights[j]` 遇到 `'0'` 必须归零，不是跳过——柱子从底边断开了
- 单调栈遍历到 `i == n` 时用虚拟高度 0，确保栈中所有柱子都被处理
- 宽度计算 `stk.empty() ? i : (i - stk.top() - 1)` 是最易错的地方

---

### 解法3: DP（height/left/right 数组） — O(mn) / O(n)

**思考过程**：不用单调栈，换一种 DP 思路。对每个位置 (i,j)，维护三个信息：
- `height[j]`：从第 i 行往上连续1的高度
- `left[j]`：在 height[j] 这个高度下，最多能向左延伸到哪一列（闭区间左端点）
- `right[j]`：最多能向右延伸到哪一列（开区间右端点）

面积 = `height[j] × (right[j] - left[j])`

```
// 以示例 row2 为例 (heights = [3,1,3,2,2]):
//
//  j:        0    1    2    3    4
//  height:   3    1    3    2    2
//  left:     0    0    2    2    2    (能向左延伸的最左列)
//  right:    1    5    3    5    5    (能向右延伸的最右列+1，开区间)
//
//  面积:  3×1=3 1×5=5 3×1=3 2×3=6 2×3=6
//                                ↑ 最大=6
```

```cpp
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> height(n, 0);
        vector<int> left(n, 0);      // left[j]: 以height[j]高度能延伸的最左列
        vector<int> right(n, n);     // right[j]: 以height[j]高度能延伸的最右列+1(开区间)
        int ans = 0;
        
        for (int i = 0; i < m; i++) {
            // 更新 height
            for (int j = 0; j < n; j++)
                height[j] = (matrix[i][j] == '1') ? height[j] + 1 : 0;
            
            // 更新 left (从左到右扫描)
            // curLeft: 当前行中从左边起最近的'1'块的起始列
            int curLeft = 0;
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == '1') {
                    // 取上一行的left[j]和当前行约束curLeft的较大值
                    // 因为矩形必须同时满足上一行和当前行的约束
                    left[j] = max(left[j], curLeft);
                } else {
                    left[j] = 0;       // 高度为0，重置为最宽（下一行会重新约束）
                    curLeft = j + 1;    // 下一个可能的左边界
                }
            }
            
            // 更新 right (从右到左扫描)
            int curRight = n;
            for (int j = n - 1; j >= 0; j--) {
                if (matrix[i][j] == '1') {
                    right[j] = min(right[j], curRight);
                } else {
                    right[j] = n;       // 高度为0，重置为最宽
                    curRight = j;        // 下一个可能的右边界
                }
            }
            
            // 计算面积
            for (int j = 0; j < n; j++)
                ans = max(ans, height[j] * (right[j] - left[j]));
        }
        return ans;
    }
};
```

**关键点**：
- `left[j]` 在遇到 `'0'` 时重置为 `0` 而不是 `-1`，因为下一行如果该位置是 `'1'`，高度重新从1开始，左边界自然要重新算
- `right[j]` 同理重置为 `n`
- 这个解法的巧妙之处在于 `left[j] = max(left[j], curLeft)`：利用了上一行的信息（`left[j]` 保留了上一行的值），同时受当前行的约束（`curLeft`），取较紧的那个

## 解法对比

| | 暴力+前缀和 | 柱状图+单调栈 | DP (left/right/height) |
|---|---|---|---|
| 时间 | O(m²n²) | O(mn) | O(mn) |
| 空间 | O(mn) | O(n) | O(n) |
| 核心思想 | 枚举所有矩形 | 转化为84题 | 逐行维护边界 |
| 代码难度 | 简单但慢 | 中等（需要会84题） | 中等偏难（理解left/right转移） |
| 面试推荐 | ✗ | ⭐ 首选 | 加分项 |

**选择建议**：面试优先写解法2（柱状图+单调栈），因为思路清晰且能展示对84题的掌握。如果面试官追问"不用栈能做吗"，再给解法3。

## 易错点

1. **heights 遇到 '0' 没有归零**
   - ✗ `heights[j] += (matrix[i][j] == '1')` → 遇到0时高度还是上一行的值
   - ✓ `heights[j] = (matrix[i][j] == '1') ? heights[j] + 1 : 0`

2. **单调栈的宽度计算**
   - ✗ `w = i - stk.top()` → 少算了左边可以延伸的部分
   - ✓ `w = stk.empty() ? i : (i - stk.top() - 1)` → 左边界是新栈顶的下一个位置

3. **单调栈没有清空尾部**
   - ✗ 遍历完 `0..n-1` 就结束 → 栈中剩余的柱子没有计算面积
   - ✓ 遍历到 `i == n` 且 `curHeight = 0`，强制弹出所有剩余柱子

4. **DP 解法中 left/right 重置值搞反**
   - ✗ 遇到 `'0'` 时 `left[j] = j`（用当前位置）
   - ✓ 遇到 `'0'` 时 `left[j] = 0`, `right[j] = n`（重置为最宽，让下一行重新约束）

5. **matrix 元素是 char 不是 int**
   - ✗ `matrix[i][j] == 1` → 永远为 false
   - ✓ `matrix[i][j] == '1'`

## 面试追问

**Q1（基础理解）**：暴力法怎么做？复杂度是多少？瓶颈在哪？
> 枚举所有矩形的四个边界 O(m²n²)，用前缀和 O(1) 判断全1。瓶颈在枚举矩形数量太多。

**Q2（核心优化）**：怎么把这道二维问题转化为一维问题？
> 逐行构建柱状图：每列的高度是从当前行往上连续1的个数。这样每一行变成一个"柱状图中最大矩形"问题（LeetCode 84），用单调栈 O(n) 解决。总共 m 行，总时间 O(mn)。

**Q3（深入追问）**：如果不用栈，能否用纯 DP 解决？
> 可以。对每列维护 height、left（当前高度最多向左到哪）、right（向右到哪），逐行更新。关键是 `left[j] = max(left[j], curLeft)`，取上一行约束和当前行约束中较紧的那个。

**Q4（变体）**：如果要求的不是矩形，而是最大正方形呢？
> 那就是 LeetCode 221（最大正方形），用不同的 DP：`dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1`。

## 相关题型

- **84. 柱状图中最大的矩形** — 本题解法2的核心子问题，必须先掌握。单调栈模板完全复用。
- **221. 最大正方形** — 同样是二维矩阵找最大全1区域，但限制为正方形。DP 状态转移不同：`dp[i][j] = min(三个邻居) + 1`。
- **304. 二维区域和检索** — 解法1 中用到的二维前缀和技巧的专项练习。
- **42. 接雨水** — 单调栈的另一经典应用，和84题是"姊妹题"。