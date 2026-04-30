/*
 * LeetCode LCR 146: 螺旋遍历二维数组 (同主站 54. Spiral Matrix)
 *
 * 【题目本质】
 * 模拟题：按"右→下→左→上"的螺旋顺序遍历矩阵，关键是正确维护边界。
 *
 * 【解法总览】
 * 解法1: 边界收缩法 — O(m×n) / O(1) — ⭐ 面试首选
 * 解法2: 方向模拟 + visited — O(m×n) / O(m×n) — 最直觉的模拟
 */

// ============================================================
// 解法1: 边界收缩法 — 维护 top/bottom/left/right 四个边界
// 时间: O(m×n)  空间: O(1)（不算输出数组）
//
// 【思路】
// 螺旋遍历就是一圈一圈地走。用四个变量表示当前未遍历区域的边界：
//   top: 当前最上面一行
//   bottom: 当前最下面一行
//   left: 当前最左边一列
//   right: 当前最右边一列
//
// 每走完一条边，就收缩对应的边界。一圈 = 四条边：
//   1. 从左到右（遍历 top 行）→ top++
//   2. 从上到下（遍历 right 列）→ right--
//   3. 从右到左（遍历 bottom 行）→ bottom--
//   4. 从下到上（遍历 left 列）→ left++
//
// 关键：第3步和第4步前要检查边界是否仍有效，防止单行/单列重复遍历。
//
// 示例 3×4 矩阵的遍历过程：
//
//   初始: top=0, bottom=2, left=0, right=3
//
//    1 → 2 → 3 → 4     Step1: row=0, col: 0→3  → top=1
//                  ↓
//   12  13  14    5     Step2: col=3, row: 1→2  → right=2
//    ↑             ↓
//   11  16  15 ← 6     Step3: row=2, col: 2→0  → bottom=1 (需检查 top<=bottom)
//    ↑
//   10 ← 9 ← 8 ← 7     Step4: col=0, row: 1→1  → left=1 (需检查 left<=right)
//
//   现在: top=1, bottom=1, left=1, right=2
//
//   Step5: row=1, col: 1→2 → 取到 13, 14 → top=2
//   top=2 > bottom=1 → 结束
//
//   结果: [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
//
// ============================================================
class Solution1 {
public:
    vector<int> spiralArray(vector<vector<int>>& array) {
        vector<int> res;
        if (array.empty() || array[0].empty()) return res;
        
        int top = 0, bottom = (int)array.size() - 1;
        int left = 0, right = (int)array[0].size() - 1;
        
        while (top <= bottom && left <= right) {
            // 第1条边：从左到右遍历 top 行
            for (int col = left; col <= right; col++)
                res.push_back(array[top][col]);
            top++;  // 这行已经走完，收缩上边界
            
            // 第2条边：从上到下遍历 right 列
            for (int row = top; row <= bottom; row++)
                res.push_back(array[row][right]);
            right--;  // 这列已经走完，收缩右边界
            
            // 第3条边：从右到左遍历 bottom 行
            // 为什么要检查 top <= bottom？
            // 因为上面 top++ 后，可能 top > bottom 了（单行的情况）
            // 如果不检查，这一行会被重复遍历
            if (top <= bottom) {
                for (int col = right; col >= left; col--)
                    res.push_back(array[bottom][col]);
                bottom--;  // 收缩下边界
            }
            
            // 第4条边：从下到上遍历 left 列
            // 为什么要检查 left <= right？
            // 因为上面 right-- 后，可能 left > right 了（单列的情况）
            if (left <= right) {
                for (int row = bottom; row >= top; row--)
                    res.push_back(array[row][left]);
                left++;  // 收缩左边界
            }
        }
        
        return res;
    }
};

// ============================================================
// 解法2: 方向模拟 + visited 标记 — 像走迷宫一样螺旋行走
// 时间: O(m×n)  空间: O(m×n)（visited 数组）
//
// 【思路】
// 最直觉的模拟：维护当前位置 (x, y) 和当前方向 dir。
// 沿当前方向走，碰到边界或已访问的格子就"右转"（换下一个方向）。
//
// 方向数组：
//   dir=0: 右 (0, +1)
//   dir=1: 下 (+1, 0)
//   dir=2: 左 (0, -1)
//   dir=3: 上 (-1, 0)
// 右转 = (dir + 1) % 4
//
// 这个方法的优势是逻辑统一，不需要分4条边讨论。
// 劣势是需要 O(m×n) 的 visited 数组。
//
// 模拟过程 (3×3):
//   方向: →  位置: (0,0)→(0,1)→(0,2) 碰壁
//   方向: ↓  位置: (1,2)→(2,2)       碰壁
//   方向: ←  位置: (2,1)→(2,0)       碰壁
//   方向: ↑  位置: (1,0)             碰壁(已访问)
//   方向: →  位置: (1,1)             总共9个，结束
//
// ============================================================
class Solution2 {
public:
    vector<int> spiralArray(vector<vector<int>>& array) {
        vector<int> res;
        if (array.empty() || array[0].empty()) return res;
        
        int m = array.size(), n = array[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        
        // 方向数组：右→下→左→上
        // dx 控制行变化，dy 控制列变化
        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};
        
        int dir = 0;    // 初始方向：向右
        int x = 0, y = 0;  // 初始位置：左上角
        
        for (int i = 0; i < m * n; i++) {
            res.push_back(array[x][y]);
            visited[x][y] = true;
            
            // 试探下一个位置
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            
            // 越界或已访问 → 右转
            if (nx < 0 || nx >= m || ny < 0 || ny >= n || visited[nx][ny]) {
                dir = (dir + 1) % 4;  // 右转
                nx = x + dx[dir];
                ny = y + dy[dir];
            }
            
            x = nx;
            y = ny;
        }
        
        return res;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1 边界收缩  | 解法2 方向模拟    |
// |-----------|----------------|-----------------|
// | 时间       | O(m×n)         | O(m×n)          |
// | 空间       | O(1)           | O(m×n)          |
// | 代码风格   | 4个for循环，结构清晰 | 1个循环+方向数组，更通用 |
// | 面试推荐   | ⭐ 首选         | 退路方案         |
// | 扩展性     | 矩形矩阵专用    | 不规则区域也能用   |
//
// 面试建议：先说解法2的思路（最直觉），然后写解法1（空间更优）。
//
// ============================================================
// 【易错点】
//
// 1. 空矩阵判断不全：
//    ✗ 只判断 array.empty()，没判断 array[0].empty()
//    ✓ if (array.empty() || array[0].empty()) return {};
//    原因：输入可能是 [[]]，此时 array.size()=1 但 array[0] 为空
//
// 2. 解法1中"向左"和"向上"忘了边界检查：
//    ✗ 直接写 for (col = right; col >= left; col--)
//    ✓ 加 if (top <= bottom) 保护
//    原因：单行矩阵 [[1,2,3]]，走完"右"后 top++ 使 top>bottom，
//          不检查就会反向再走一遍同一行
//
// 3. 解法2中方向数组顺序写错：
//    ✗ dx={1,0,-1,0}, dy={0,1,0,-1} (下→右→上→左)
//    ✓ dx={0,1,0,-1}, dy={1,0,-1,0} (右→下→左→上)
//    原因：螺旋遍历是"右下左上"，不是"下右上左"
//
// 4. 解法2中 for 循环次数是 m*n 而不是 while(true)：
//    用 m*n 可以精确控制，避免死循环。
//    如果用 while(true)，需要额外的退出条件。
//
// ============================================================
// 【面试追问】
//
// Q1: 时间复杂度能优化吗？
// → 不能。每个元素必须恰好访问一次，O(m×n) 是下界。
//
// Q2: 解法1的 O(1) 空间是怎么做到的？为什么不需要 visited？
// → 四个边界变量精确定义了"未访问区域"的矩形范围，
//   已访问的元素一定在这个矩形之外，所以不需要逐格标记。
//
// Q3: 如果要逆时针螺旋遍历（右→上→左→下变成下→右→上→左），怎么改？
// → 解法1: 第一条边改为"从上到下(left列)"，然后顺序调整。
//   解法2: 方向数组改为 {(1,0),(0,1),(-1,0),(0,-1)} 即可。
//
// Q4: LeetCode 59 螺旋矩阵II — 给定 n，生成 1~n² 的螺旋填充矩阵？
// → 用解法1的框架，把 push_back 改为 matrix[row][col] = num++。
//
// ============================================================
