/*
 * LeetCode 54: 螺旋矩阵 (Spiral Matrix)
 *
 * 【题目本质】
 * 按顺时针螺旋顺序遍历 m×n 矩阵的所有元素。
 * 本质是一道模拟题，核心在于精确控制遍历边界。
 *
 * 【解法总览】
 * 解法1: 方向模拟 + visited       — O(m·n) / O(m·n)  — 最直觉
 * 解法2: 边界收缩（层剥离）        — O(m·n) / O(1)    — 面试首选 ⭐
 * 解法3: 方向数组 + 步数递减       — O(m·n) / O(1)    — 代码最短
 */

// ============================================================
// 解法1: 方向模拟 + visited — 模拟行走，碰壁就右转
// 时间: O(m·n)  空间: O(m·n)（visited 数组）
//
// 【思路】
// 想象一个人从(0,0)出发，初始方向"右"。
// 每走一步，检查前方是否越界或已访问，如果是就顺时针转90°。
// 这是最自然的模拟，但需要 O(m·n) 的 visited 数组。
//
// 螺旋路径示意（3×3）:
//  1 → 2 → 3
//            ↓
//  4 → 5    6
//  ↑        ↓
//  7 ← 8 ← 9
//
// 输出: [1,2,3,6,9,8,7,4,5]
// ============================================================
class Solution1 {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));

        // 方向数组：右(0,1) → 下(1,0) → 左(0,-1) → 上(-1,0)
        int dr[] = {0, 1, 0, -1};
        int dc[] = {1, 0, -1, 0};

        vector<int> result;
        int r = 0, c = 0, dir = 0;

        for (int i = 0; i < m * n; i++) {
            result.push_back(matrix[r][c]);
            visited[r][c] = true;

            // 试探下一步
            int nr = r + dr[dir], nc = c + dc[dir];

            // 越界或已访问 → 顺时针转向
            if (nr < 0 || nr >= m || nc < 0 || nc >= n || visited[nr][nc]) {
                dir = (dir + 1) % 4;
                nr = r + dr[dir];
                nc = c + dc[dir];
            }
            r = nr;
            c = nc;
        }
        return result;
    }
};

// ============================================================
// 解法2: 边界收缩（层剥离） — 面试首选 ⭐
// 时间: O(m·n)  空间: O(1)（不计输出数组）
//
// 【思路】
// 解法1的瓶颈：需要 O(m·n) 的 visited 数组来判断"是否走过"。
// 
// 观察螺旋规律：每一"圈"就是四条直线段（右→下→左→上），
// 走完一圈后矩阵缩小一圈。用 top/bottom/left/right 四个边界
// 变量精确控制，不需要 visited。
//
// 边界收缩过程（3×4矩阵）:
//
//  初始: top=0, bottom=2, left=0, right=3
//
//  第1圈:
//   →: matrix[0][0..3] = 1,2,3,4    → top=1
//   ↓: matrix[1..2][3] = 8,12       → right=2
//   ←: matrix[2][2..0] = 11,10,9    → bottom=1
//   ↑: matrix[1][0]    = 5          → left=1
//
//  第2圈: top=1,bottom=1,left=1,right=2
//   →: matrix[1][1..2] = 6,7        → top=2
//   此时 top > bottom，循环结束
//
// 最终: [1,2,3,4,8,12,11,10,9,5,6,7]
//
// 【关键】走"左"之前必须检查 top<=bottom（因为"右"之后top已++）
//         走"上"之前必须检查 left<=right（因为"下"之后right已--）
//         否则单行/单列矩阵会重复遍历！
// ============================================================
class Solution2 {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> result;
        int top = 0, bottom = matrix.size() - 1;
        int left = 0, right = matrix[0].size() - 1;

        while (top <= bottom && left <= right) {
            // → 右：遍历上边 (row=top, col从left到right)
            for (int c = left; c <= right; c++)
                result.push_back(matrix[top][c]);
            top++;  // 上边已经遍历完，向下收缩

            // ↓ 下：遍历右边 (col=right, row从top到bottom)
            for (int r = top; r <= bottom; r++)
                result.push_back(matrix[r][right]);
            right--;  // 右边已经遍历完，向左收缩

            // ← 左：遍历下边（必须检查还有没有下边！）
            if (top <= bottom) {
                for (int c = right; c >= left; c--)
                    result.push_back(matrix[bottom][c]);
                bottom--;  // 下边已经遍历完，向上收缩
            }

            // ↑ 上：遍历左边（必须检查还有没有左边！）
            if (left <= right) {
                for (int r = bottom; r >= top; r--)
                    result.push_back(matrix[r][left]);
                left++;  // 左边已经遍历完，向右收缩
            }
        }
        return result;
    }
};

// ============================================================
// 解法3: 方向数组 + 步数递减 — 代码最短
// 时间: O(m·n)  空间: O(1)
//
// 【思路】
// 观察螺旋每个方向要走的步数：
//   3×4矩阵: →4步, ↓2步, ←3步, ↑1步, →2步, ↓0步(停)
//   即: n, m-1, n-1, m-2, n-2, ...
//   规律: 水平和垂直交替递减
//         水平: n, n-1, n-2, ...
//         垂直: m-1, m-2, m-3, ...
//
// 用 steps[0]=水平步数(初始n), steps[1]=垂直步数(初始m-1)
// 每走完一个方向，同方向步数减1，然后换方向
//
// 注意起点设为 (0,-1)，因为第一步 c+=1 后到达 (0,0)
// ============================================================
class Solution3 {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        int dr[] = {0, 1, 0, -1};  // 右、下、左、上
        int dc[] = {1, 0, -1, 0};

        vector<int> result;
        int r = 0, c = -1;  // 起点偏移，第一步移动后才到(0,0)
        int dir = 0;
        int steps[] = {n, m - 1};  // [0]=水平步数, [1]=垂直步数

        while (steps[dir % 2] > 0) {
            for (int i = 0; i < steps[dir % 2]; i++) {
                r += dr[dir];
                c += dc[dir];
                result.push_back(matrix[r][c]);
            }
            // 走完一个方向后，同方向的步数减1（下次轮到时少走一步）
            steps[dir % 2]--;
            dir = (dir + 1) % 4;  // 换到下一个方向
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间    | 额外空间 | 优点           | 缺点             |
// |----------------|---------|----------|----------------|------------------|
// | 1. visited模拟 | O(m·n)  | O(m·n)  | 最直觉         | 额外空间大       |
// | 2. 边界收缩    | O(m·n)  | O(1)    | 清晰+无额外空间 | 四段循环稍长     |
// | 3. 步数递减    | O(m·n)  | O(1)    | 代码最短        | 步数规律需推导   |
//
// 面试推荐：解法2。思路清晰、面试官容易跟着理解、没有额外空间。
//
// ============================================================
// 【易错点】
//
// 1. 解法2中"左"和"上"缺少边界检查：
//    ✗ 走完"右"和"下"后直接走"左"
//    ✓ 必须 if(top <= bottom) 再走"左"
//    原因：矩阵 [[1,2,3]] 只有1行，走完"右"后 top++=1 > bottom=0，
//    如果不检查，"左"会从right到left反向重复遍历第0行。
//
// 2. 解法2边界收缩时机：
//    ✗ top++; for(c=left..right) matrix[top][c]  (先缩再遍历→漏掉一行)
//    ✓ for(c=left..right) matrix[top][c]; top++;  (先遍历再缩)
//
// 3. 解法3起始位置：
//    ✗ r=0, c=0 → 第一步到(0,1)，漏掉(0,0)
//    ✓ r=0, c=-1 → 第一步 c+=1 到(0,0)
//
// 4. while 条件写成 top < bottom（缺等号）：
//    ✗ while(top < bottom && left < right)
//    ✓ while(top <= bottom && left <= right)
//    原因：当 top==bottom 时还剩一行需要遍历，缺等号会漏掉。
//
// ============================================================
// 【面试追问】
//
// Q1: 时间复杂度能否优于 O(m·n)？
// A1: 不能。必须输出所有元素，O(m·n) 是下界。
//
// Q2: 如果反过来，给一维数组按螺旋填入矩阵呢？(LeetCode 59)
// A2: 框架完全一样，把 result.push_back(matrix[r][c]) 
//     改成 matrix[r][c] = num++。边界收缩逻辑不变。
//
// Q3: 如果只要第 k 个螺旋元素，不要全部？
// A3: 每圈元素个数 = 2*(width+height-2)，可以 O(min(m,n)) 
//     定位第k个在哪圈哪条边的哪个位置，不需要遍历。
//
// Q4: 如果要逆时针螺旋呢？
// A4: 把方向顺序改为 下→右→上→左，或等价地改为先向下再向右。
//     边界收缩的逻辑同理调整收缩顺序。
// ============================================================
