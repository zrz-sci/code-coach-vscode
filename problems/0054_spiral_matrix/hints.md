# 54. 螺旋矩阵

## 核心思路

本质上就是**模拟**：按照"右→下→左→上"的顺序不断缩小边界，依次遍历矩阵的外圈、次外圈……直到所有元素都被访问。没有什么高深算法，关键是把边界管理做对。

## 思维链

1. **读完题第一反应**：这就是一个模拟题——按螺旋方向一个一个读元素。最朴素的做法是用一个 visited 数组标记已访问，遇到边界或已访问就转向。
2. **暴力解的"浪费"在哪？**：额外的 visited 数组占 O(m×n) 空间，而且每次都要检查"是否越界或已访问"才决定转向，逻辑稍显繁琐。
3. **能不能不用 visited？**：观察螺旋的规律——每一圈就是"右→下→左→上"四条边，走完一圈后四个边界各缩一格。用四个变量 `top, bottom, left, right` 就能精确控制当前要遍历的范围。
4. **核心逻辑**：外层 `while(top <= bottom && left <= right)`，内层依次遍历四条边，每条边走完立即缩边界。注意"左"和"上"两条边可能在单行或单列时不需要走（否则会重复）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 方向模拟 + visited | 模拟行走，碰壁或已访问就右转 | O(m·n) | O(m·n) | 能说出即可 |
| 边界收缩（层剥离） | 四边界变量，逐圈遍历 | O(m·n) | O(1)* | ⭐ 必须写出 |
| 方向数组（不用 visited） | 用方向数组+边界缩减，统一循环 | O(m·n) | O(1)* | 加分项 |

> *O(1) 是除了输出数组外的额外空间

## 关键提示

1. **画出螺旋路径看规律**：每一"圈"恰好是四条直线段（右→下→左→上），走完一圈后矩阵缩小了一圈。

```
示例 3×4 矩阵螺旋路径：
 1 → 2 → 3 → 4
                ↓
 5    6    7    8
 ↑              ↓
 9 ← 10 ← 11  12

外圈: 1,2,3,4,8,12,11,10,9,5
内圈: 6,7
```

2. **边界收缩的节奏**：走完"右"就 `top++`，走完"下"就 `right--`，走完"左"就 `bottom--`，走完"上"就 `left++`。

3. **单行/单列的陷阱**：走完"右"和"下"后，如果 `top > bottom` 或 `left > right`，说明已经没有"左"和"上"要走了。这是最容易写 bug 的地方。

4. **总元素个数**：螺旋遍历结束时，结果恰好有 m×n 个元素，可以用这个做断言验证。

## 解法详解

### 解法1: 方向模拟 + visited — O(m·n) / O(m·n)

**思考过程**：最直觉的模拟——像一个人在矩阵里走路，碰到墙壁或已走过的格子就顺时针转90°。

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        // 方向：右、下、左、上
        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};
        
        vector<int> result;
        int r = 0, c = 0, dir = 0; // 从(0,0)开始，初始方向"右"
        
        for (int i = 0; i < m * n; i++) {
            result.push_back(matrix[r][c]);
            visited[r][c] = true;
            
            // 计算下一步位置
            int nr = r + dx[dir], nc = c + dy[dir];
            // 如果越界或已访问，就顺时针转向
            if (nr < 0 || nr >= m || nc < 0 || nc >= n || visited[nr][nc]) {
                dir = (dir + 1) % 4; // 右转
                nr = r + dx[dir];
                nc = c + dy[dir];
            }
            r = nr;
            c = nc;
        }
        return result;
    }
};
```

**关键点**：需要额外的 visited 数组，空间 O(m·n)。逻辑简单但不够优雅。

---

### 解法2: 边界收缩（层剥离） — O(m·n) / O(1) ⭐ 面试首选

**从解法1优化**：解法1需要 visited 数组来判断"是否走过"。但螺旋的规律很明显——每一圈就是四条边。用四个边界变量就能精确控制，不需要 visited。

```
// 边界收缩过程（3×4矩阵）：
//
// 初始: top=0, bottom=2, left=0, right=3
//
//  [ 1   2   3   4 ] ← 右: row=top, col: left→right, 然后 top++
//  [ 5   6   7   8 ]    下: col=right, row: top→bottom, 然后 right--
//  [ 9  10  11  12 ]    左: row=bottom, col: right→left, 然后 bottom--
//                        上: col=left, row: bottom→top, 然后 left++
//
// 第1圈后: top=1, bottom=1, left=1, right=2
//  输出: 1,2,3,4,8,12,11,10,9,5
//
// 第2圈: 只剩 [6, 7]
//  右: 6,7  → top++ → top=2 > bottom=1 → 结束
//  输出: 6,7
```

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> result;
        int top = 0, bottom = matrix.size() - 1;
        int left = 0, right = matrix[0].size() - 1;
        
        while (top <= bottom && left <= right) {
            // → 右：遍历上边，从left到right
            for (int c = left; c <= right; c++)
                result.push_back(matrix[top][c]);
            top++;  // 上边已遍历，收缩
            
            // ↓ 下：遍历右边，从top到bottom
            for (int r = top; r <= bottom; r++)
                result.push_back(matrix[r][right]);
            right--;  // 右边已遍历，收缩
            
            // ← 左：遍历下边（需要检查是否还有下边）
            if (top <= bottom) {
                for (int c = right; c >= left; c--)
                    result.push_back(matrix[bottom][c]);
                bottom--;  // 下边已遍历，收缩
            }
            
            // ↑ 上：遍历左边（需要检查是否还有左边）
            if (left <= right) {
                for (int r = bottom; r >= top; r--)
                    result.push_back(matrix[r][left]);
                left++;  // 左边已遍历，收缩
            }
        }
        return result;
    }
};
```

**关键点**：
- "左"和"上"两条边之前必须检查 `top <= bottom` 和 `left <= right`，因为在"右"和"下"之后边界已经收缩了，可能已经没有剩余行/列。
- 这就是单行或单列矩阵容易出 bug 的地方。

---

### 解法3: 方向数组 + 边界收缩 — O(m·n) / O(1)

**思路**：把解法2的四段循环统一成一个方向数组驱动的循环，代码更短。记录每个方向还要走多少步，走完一个方向就切换。

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        // 方向：右(0,1), 下(1,0), 左(0,-1), 上(-1,0)
        int dr[] = {0, 1, 0, -1};
        int dc[] = {1, 0, -1, 0};
        // 每个方向要走的步数：初始为 n, m-1, n-1, m-2, n-2, ...
        // 规律：先走 n 步，再走 m-1 步，之后每转两次各减1
        
        vector<int> result;
        int r = 0, c = -1; // 起点偏移，因为第一步 c 会 +1
        int dir = 0;
        int steps[] = {n, m - 1}; // steps[0]=水平步数, steps[1]=垂直步数
        
        while (steps[dir % 2] > 0) {
            for (int i = 0; i < steps[dir % 2]; i++) {
                r += dr[dir];
                c += dc[dir];
                result.push_back(matrix[r][c]);
            }
            steps[dir % 2]--;  // 下次同方向少走一步
            dir = (dir + 1) % 4;  // 换方向
        }
        return result;
    }
};
```

**关键点**：利用步数递减的规律——每转两次（即一个完整的水平+垂直），同方向步数各减1。代码简洁但理解门槛稍高。

## 解法对比

| | 解法1: visited模拟 | 解法2: 边界收缩 | 解法3: 方向+步数 |
|---|---|---|---|
| 时间 | O(m·n) | O(m·n) | O(m·n) |
| 额外空间 | O(m·n) | O(1) | O(1) |
| 代码量 | 中等 | 中等 | 最短 |
| 可读性 | 最直觉 | 清晰 | 需要理解步数规律 |
| 面试推荐 | 作为起点 | **首选** | 加分项 |

解法2是面试中的黄金选择：思路清晰、代码可控、没有额外空间。

## 易错点

1. **"左"和"上"缺少边界检查导致重复遍历**
   - ✗ 在解法2中，走完"右"和"下"后直接走"左"和"上"
   - ✓ 必须在"左"之前检查 `if (top <= bottom)`，在"上"之前检查 `if (left <= right)`
   - 原因：对于单行矩阵 `[[1,2,3]]`，走完"右"后 `top++` 使得 `top > bottom`，如果不检查就会反向重复遍历

2. **边界收缩的时机搞错**
   - ✗ 先收缩再遍历：`top++; for(c=left;c<=right;c++) result.push(matrix[top][c]);`
   - ✓ 先遍历再收缩：遍历完这条边后，才把这条边"剥掉"

3. **解法3中起始位置写成 (0,0)**
   - ✗ `r=0, c=0`，第一步移动后变成 (0,1)，漏掉了 (0,0)
   - ✓ `r=0, c=-1`，第一步 c+=1 后到 (0,0)，正确

## 面试追问

**Q1: 这道题的时间复杂度能比 O(m·n) 更优吗？**
不能。必须输出所有 m×n 个元素，所以 O(m·n) 是下界。

**Q2: 如果要反过来——给你一个一维数组，按螺旋顺序填入 m×n 矩阵呢？（LeetCode 59）**
思路完全一样，只是从"读"变成"写"。边界收缩的逻辑不变，把 `result.push_back(matrix[r][c])` 改成 `matrix[r][c] = num++`。

**Q3: 如果矩阵非常大（如 10000×10000），但只需要输出第 k 个螺旋元素，不需要全部？**
可以根据每圈的元素个数快速定位第 k 个元素在第几圈、第几条边、第几个位置。每圈的元素数 = `2*(cols + rows - 2)`，然后 cols 和 rows 各减2进入下一圈。这样可以 O(min(m,n)) 定位，不需要遍历前 k-1 个。

**Q4: 如果要求逆时针螺旋呢？**
把方向顺序改成"下→右→上→左"，或者等价地：先顺时针螺旋，然后以左上角为起点改为先向下走。边界收缩逻辑同理调整。

## 相关题型

- **59. 螺旋矩阵 II** — 复用**完全相同的边界收缩框架**，区别：从"读矩阵→写数组"变成"读计数器→写矩阵"，核心四段循环一模一样
- **885. 螺旋矩阵 III** — 复用**解法3的步数递增规律**（而非递减），区别：从矩阵中心向外扩展，步数 1,1,2,2,3,3...
- **498. 对角线遍历** — 同类"矩阵模拟遍历"题，复用方向切换 + 边界处理的思维模式