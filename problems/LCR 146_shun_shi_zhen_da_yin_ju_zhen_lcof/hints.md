# LCR 146. 螺旋遍历二维数组

## 核心思路

这道题本质上是**模拟**问题：按照"右→下→左→上"的顺序一圈一圈地遍历矩阵，关键在于如何正确地维护当前遍历的边界，避免重复访问或越界。

## 思维链

1. **读完题第一反应**：就是按照螺旋的方向依次读取元素，这不是算法优化问题，而是"怎么把这个过程写对"的模拟问题。
2. **怎么控制方向？** 螺旋遍历每一圈都是"右→下→左→上"四个方向。一圈走完后，边界收缩，进入内层继续。
3. **关键问题：怎么知道一圈走完了？** 维护四个边界变量 `top, bottom, left, right`，每走完一条边就收缩对应的边界。
4. **什么时候停止？** 当 `top > bottom` 或 `left > right` 时，说明所有元素已遍历完毕。
5. **易漏点：单行或单列**。走完"右"和"下"后，"左"和"上"需要检查边界是否仍然有效，否则会重复遍历。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 边界收缩法 | 维护 top/bottom/left/right 四个边界，逐圈遍历 | O(m×n) | O(1) | ⭐ 必须写出 |
| 方向模拟 + visited | 用方向数组 + visited 标记，碰壁/访问过就转向 | O(m×n) | O(m×n) | 能说出即可 |

## 关键提示

- **提示1**：想象你在矩阵上走路，走到尽头或者走到已经走过的地方就右转。这就是螺旋的本质。
- **提示2**：边界收缩法不需要额外空间标记"是否访问过"，只用四个整数就能精确控制范围。
- **提示3**：走完"从左到右"这一行后，`top++` 把这行"删掉"；走完"从上到下"这一列后，`right--` 把这列"删掉"。以此类推。
- **提示4**：向左和向上遍历前，必须再次检查 `top <= bottom` 和 `left <= right`，防止单行/单列被重复遍历。

```
// 示例矩阵 3×3 的螺旋遍历过程：
//
//  初始边界: top=0, bottom=2, left=0, right=2
//
//   1 → 2 → 3        第1步: 从左到右 (row=0, col: 0→2) → top++
//             ↓       第2步: 从上到下 (col=2, row: 1→2) → right--
//   8   9   4        
//   ↑       ↓       第3步: 从右到左 (row=2, col: 1→0) → bottom--
//   7 ← 6 ← 5        第4步: 从下到上 (col=0, row: 1→1) → left++
//
//  现在边界: top=1, bottom=1, left=1, right=1
//
//   第5步: 从左到右 (row=1, col: 1→1) → 取到 9 → top++
//   top=2 > bottom=1 → 结束
//
//  结果: [1,2,3,4,5,6,7,8,9]
```

## 解法详解

### 解法1: 边界收缩法 — O(m×n) / O(1) ⭐ 面试首选

**思考过程**：螺旋遍历就是一圈一圈地走。每走完一条边，那条边就不需要再走了。用四个变量 `top, bottom, left, right` 表示当前还没走过的矩形区域，每条边走完就收缩对应边界。

```cpp
class Solution {
public:
    vector<int> spiralArray(vector<vector<int>>& array) {
        vector<int> res;
        if (array.empty() || array[0].empty()) return res;
        
        int top = 0, bottom = array.size() - 1;
        int left = 0, right = array[0].size() - 1;
        
        while (top <= bottom && left <= right) {
            // 从左到右遍历上边界
            for (int col = left; col <= right; col++)
                res.push_back(array[top][col]);
            top++;  // 上边界用完了，收缩
            
            // 从上到下遍历右边界
            for (int row = top; row <= bottom; row++)
                res.push_back(array[row][right]);
            right--;  // 右边界用完了，收缩
            
            // 从右到左遍历下边界（需检查 top <= bottom，防止单行重复）
            if (top <= bottom) {
                for (int col = right; col >= left; col--)
                    res.push_back(array[bottom][col]);
                bottom--;  // 下边界用完了，收缩
            }
            
            // 从下到上遍历左边界（需检查 left <= right，防止单列重复）
            if (left <= right) {
                for (int row = bottom; row >= top; row--)
                    res.push_back(array[row][left]);
                left++;  // 左边界用完了，收缩
            }
        }
        
        return res;
    }
};
```

**关键点**：第3步（向左）和第4步（向上）之前必须再检查边界。否则对于单行矩阵 `[[1,2,3]]`，走完"右"之后 `top++` 使得 `top > bottom`，如果不检查就会多走一次反方向。

### 解法2: 方向模拟 + visited 标记 — O(m×n) / O(m×n)

**思考过程**：最直觉的模拟——像走迷宫一样，沿当前方向走，碰到边界或已访问格子就右转。不需要思考"哪一圈、哪条边"，逻辑更简单但需要额外的 visited 数组。

```cpp
class Solution {
public:
    vector<int> spiralArray(vector<vector<int>>& array) {
        vector<int> res;
        if (array.empty() || array[0].empty()) return res;
        
        int m = array.size(), n = array[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        
        // 方向数组：右→下→左→上
        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};
        
        int dir = 0;  // 当前方向，从"右"开始
        int x = 0, y = 0;
        
        for (int i = 0; i < m * n; i++) {
            res.push_back(array[x][y]);
            visited[x][y] = true;
            
            // 计算下一个位置
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            
            // 如果下一个位置越界或已访问，就右转
            if (nx < 0 || nx >= m || ny < 0 || ny >= n || visited[nx][ny]) {
                dir = (dir + 1) % 4;  // 右转：换下一个方向
                nx = x + dx[dir];
                ny = y + dy[dir];
            }
            
            x = nx;
            y = ny;
        }
        
        return res;
    }
};
```

**关键点**：方向数组的顺序必须是"右→下→左→上"，对应 `(0,1), (1,0), (0,-1), (-1,0)`。右转就是 `(dir+1) % 4`。

## 解法对比

| | 解法1: 边界收缩 | 解法2: 方向模拟 |
|---|---|---|
| 时间 | O(m×n) | O(m×n) |
| 空间 | O(1)（不算输出） | O(m×n)（visited 数组） |
| 代码复杂度 | 中等，需要处理4条边 | 简单，通用模板 |
| 面试推荐 | ⭐ 首选，空间最优 | 思路直觉，可作为第一版 |
| 适用场景 | 矩形矩阵 | 更通用，任意形状也能改造 |

**总结**：面试中优先写解法1（边界收缩），代码清晰且空间最优。如果紧张写不出来，解法2的"模拟+visited"是安全的退路。

## 易错点

1. **空矩阵没判断**
   - ✗ 直接 `array[0].size()` → 空矩阵时数组越界
   - ✓ 先检查 `array.empty() || array[0].empty()`

2. **向左/向上遍历前忘了检查边界**
   - ✗ 直接 `for (col = right; col >= left; col--)` → 单行矩阵重复遍历
   - ✓ 加 `if (top <= bottom)` 和 `if (left <= right)` 保护

3. **非方阵的处理**
   - 矩阵可能是 1×4、4×1、2×5 等非方阵形状，四条边收缩步骤中有些可能不执行
   - 保护条件确保了这种情况的正确性

4. **for 循环的起点弄错**
   - ✗ 从上到下时写 `for (row = top; ...)` → 左上角被重复访问
   - ✓ 因为 `top` 已经在第1步之后 `++` 了，所以第2步自然从新的 `top` 开始，不会重复

## 面试追问

**Q1: 这道题的时间复杂度能优化吗？**
→ 不能。每个元素必须访问恰好一次，O(m×n) 已经是下界。

**Q2: 如果矩阵非常大（百万级），解法2的 O(m×n) 额外空间有问题吗？**
→ 有，内存可能不够。解法1只用 4 个变量控制边界，空间 O(1)，更优。

**Q3: 如果反过来，要求"逆螺旋"（从外到内逆时针），代码怎么改？**
→ 把方向改成"下→右→上→左"（解法2改方向数组顺序），或者解法1把四条边的遍历顺序调整为"上到下→左到右→下到上→右到左"。

**Q4: 如果要生成一个螺旋填充的矩阵（LeetCode 59），怎么做？**
→ 逻辑完全相同，只是把"读取"改成"写入"。

## 相关题型

- **LeetCode 59. 螺旋矩阵 II** — 本题的逆操作，从1到n²按螺旋顺序填入矩阵。复用完全相同的边界收缩框架，把 `res.push_back(array[...])` 改成 `matrix[...] = num++`
- **LeetCode 885. 螺旋矩阵 III** — 从任意起点开始螺旋行走，边界不是矩阵边而是步长递增。复用方向模拟思路，但步长规律变为 1,1,2,2,3,3,...
- **LeetCode 498. 对角线遍历** — 另一种矩阵遍历顺序，同样是模拟+边界处理