/*
 * LeetCode 85: 最大矩形 (Maximal Rectangle)
 *
 * 【题目本质】
 * 在01矩阵中找面积最大的全1矩形。
 * 核心转化：逐行构建柱状图，将二维问题降为一维的"柱状图最大矩形"（LeetCode 84）。
 *
 * 【解法总览】
 * 解法1: 暴力枚举+前缀和    — O(m²n²) / O(mn)  — 帮助理解题意
 * 解法2: 柱状图+单调栈       — O(mn)   / O(n)   — 面试首选 ⭐
 * 解法3: DP (height/left/right) — O(mn) / O(n)   — 不用栈的纯DP方案
 */

// ============================================================
// 解法1: 暴力枚举 + 二维前缀和
// 时间: O(m²n²)  空间: O(mn)
//
// 【思路】
// 最朴素的想法：枚举所有可能的矩形（由左上角和右下角确定），
// 用二维前缀和 O(1) 判断矩形内是否全为1。
// 瓶颈：矩形数量为 O(m²n²)，200×200 时约 1.6×10⁹ 次，超时。
// 但这是理解题意的起点。
// ============================================================
class Solution1 {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        // prefix[i][j] = matrix[0..i-1][0..j-1] 中 '1' 的个数
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
                        if (sum == area)
                            ans = max(ans, area);
                    }
        return ans;
    }
};

// ============================================================
// 解法2: 柱状图 + 单调栈  ⭐ 面试首选
// 时间: O(mn)  空间: O(n)
//
// 【思路】
// 关键转化：逐行把矩阵看成柱状图。
// 对第 i 行，heights[j] = 从第 i 行往上连续 '1' 的个数。
// 遇到 '0' 则 heights[j] 归零（柱子断了）。
//
// 每行的 heights 就是一个一维柱状图，用 84 题的单调栈算法
// O(n) 求最大矩形面积。遍历 m 行，总时间 O(mn)。
//
// 柱状图转化过程（以示例为例）：
//
// 原始矩阵:               逐行 heights:
// 1 0 1 0 0               row0: [1, 0, 1, 0, 0]  → maxArea=1
// 1 0 1 1 1               row1: [2, 0, 2, 1, 1]  → maxArea=3
// 1 1 1 1 1               row2: [3, 1, 3, 2, 2]  → maxArea=6 ←
// 1 0 0 1 0               row3: [4, 0, 0, 3, 0]  → maxArea=4
//
// row2 的柱状图:
//   3     3
//   █  1  █  2  2
//   █  █  █  █  █
//   █  █  █  █  █
//   ───────────────
//   0  1  2  3  4
//
// 单调栈处理 [3,1,3,2,2]:
//
// i=0: push 0               栈: [0]
// i=1: h[1]=1 < h[0]=3
//      弹出0: w=1, 面积=3×1=3
//      push 1               栈: [1]
// i=2: push 2               栈: [1,2]
// i=3: h[3]=2 < h[2]=3
//      弹出2: w=3-1-1=1, 面积=3×1=3
//      push 3               栈: [1,3]
// i=4: push 4               栈: [1,3,4]
// 清栈(i=5, h=0):
//      弹出4: w=5-3-1=1, 面积=2×1=2
//      弹出3: w=5-1-1=3, 面积=2×3=6 ← 最大!
//      弹出1: w=5,        面积=1×5=5
// ============================================================
class Solution2 {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> heights(n, 0);
        int ans = 0;
        
        for (int i = 0; i < m; i++) {
            // 步骤1: 更新柱状图高度
            for (int j = 0; j < n; j++) {
                // 遇到'1'累加高度，遇到'0'归零（柱子从底部断开）
                heights[j] = (matrix[i][j] == '1') ? heights[j] + 1 : 0;
            }
            // 步骤2: 对当前行的柱状图，用单调栈求最大矩形（84题）
            ans = max(ans, largestRectangleInHistogram(heights));
        }
        return ans;
    }

private:
    int largestRectangleInHistogram(vector<int>& heights) {
        int n = heights.size();
        stack<int> stk; // 栈中存下标，维护高度从底到顶递增
        int maxArea = 0;
        
        for (int i = 0; i <= n; i++) {
            // i==n 时用高度0，作为"哨兵"强制弹出栈中所有剩余柱子
            int curHeight = (i == n) ? 0 : heights[i];
            
            // 当前柱子比栈顶矮 → 栈顶柱子的右边界确定了
            while (!stk.empty() && curHeight < heights[stk.top()]) {
                int h = heights[stk.top()];
                stk.pop();
                // 宽度计算：
                // 右边界 = i（不含）
                // 左边界 = 新栈顶的下一位（若栈空则为0）
                int w = stk.empty() ? i : (i - stk.top() - 1);
                maxArea = max(maxArea, h * w);
            }
            stk.push(i);
        }
        return maxArea;
    }
};

// ============================================================
// 解法3: DP — height/left/right 数组
// 时间: O(mn)  空间: O(n)
//
// 【思路】
// 不用栈，纯 DP 思路。对每个位置 (i,j) 维护三个值：
// - height[j]: 从第 i 行往上连续 '1' 的个数
// - left[j]:   在 height[j] 高度下，能向左延伸的最左列（闭区间）
// - right[j]:  在 height[j] 高度下，能向右延伸的最右列+1（开区间）
//
// 面积 = height[j] × (right[j] - left[j])
//
// 关键转移（以 left 为例）：
//   如果 matrix[i][j] == '1':
//     left[j] = max(left[j], curLeft)
//     // left[j] 保留了上一行的值（上面若干行的约束）
//     // curLeft 是当前行的约束（当前行最近的'0'的右边）
//     // 取较大值 = 取更紧的约束
//   如果 matrix[i][j] == '0':
//     left[j] = 0    // 高度归零，重置为最宽
//     curLeft = j + 1 // 更新当前行约束
//
// 以 row2 为例 (heights = [3,1,3,2,2]):
//  j:       0    1    2    3    4
//  height:  3    1    3    2    2
//  left:    0    0    2    2    2
//  right:   1    5    3    5    5
//  面积:   3×1  1×5  3×1  2×3  2×3
//          =3   =5   =3   =6   =6  → max=6
// ============================================================
class Solution3 {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<int> height(n, 0);
        vector<int> left(n, 0);   // 初始化为0：最左可达列0
        vector<int> right(n, n);  // 初始化为n：最右可达列n-1的开区间
        int ans = 0;
        
        for (int i = 0; i < m; i++) {
            // 更新 height
            for (int j = 0; j < n; j++)
                height[j] = (matrix[i][j] == '1') ? height[j] + 1 : 0;
            
            // 更新 left: 从左往右扫
            int curLeft = 0; // 当前行中，当前连续'1'段的起始列
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == '1') {
                    // 取"上方约束"和"当前行约束"中更紧的
                    left[j] = max(left[j], curLeft);
                } else {
                    // 高度为0，重置 left 为最宽（0），让下一行重新约束
                    left[j] = 0;
                    curLeft = j + 1;
                }
            }
            
            // 更新 right: 从右往左扫
            int curRight = n;
            for (int j = n - 1; j >= 0; j--) {
                if (matrix[i][j] == '1') {
                    right[j] = min(right[j], curRight);
                } else {
                    right[j] = n;
                    curRight = j;
                }
            }
            
            // 计算每个位置的面积
            for (int j = 0; j < n; j++)
                ans = max(ans, height[j] * (right[j] - left[j]));
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间     | 空间  | 核心思想              | 面试推荐 |
// |--------------|----------|-------|-----------------------|---------|
// | 暴力+前缀和   | O(m²n²)  | O(mn) | 枚举所有矩形          | 说思路   |
// | 柱状图+单调栈  | O(mn)    | O(n)  | 转化为84题逐行求解     | ⭐ 首选 |
// | DP(l/r/h)    | O(mn)    | O(n)  | 逐行维护边界           | 加分项   |
//
// 解法2 vs 解法3:
// - 解法2 思路更直观（会84题就能做），代码模块化好（复用84题函数）
// - 解法3 不需要栈，纯数组操作，但 left/right 的更新逻辑较难理解
// - 实际面试优先写解法2，被追问时再讲解法3
//
// ============================================================
// 【易错点】
//
// 1. heights 遇到 '0' 没有归零:
//    ✗ heights[j] += (matrix[i][j] - '0')
//      → 上方有1下方有0时高度不会断开，算出错误面积
//    ✓ heights[j] = (matrix[i][j] == '1') ? heights[j] + 1 : 0
//
// 2. 单调栈宽度计算:
//    ✗ w = i - stk.top()
//      → 没有减1，且没处理栈空的情况
//    ✓ w = stk.empty() ? i : (i - stk.top() - 1)
//      → 左边界是新栈顶的下一位，右边界是 i-1，宽度 = 右-左+1 = i-stk.top()-1
//
// 3. 单调栈遍历只到 n-1:
//    ✗ for (int i = 0; i < n; i++) → 栈中最后剩余的柱子没被处理
//    ✓ for (int i = 0; i <= n; i++)，且 i==n 时 curHeight=0
//
// 4. matrix 元素是 char 不是 int:
//    ✗ matrix[i][j] == 1  → 永远为 false
//    ✓ matrix[i][j] == '1'
//
// 5. DP解法中 left/right 重置值错误:
//    ✗ 遇到'0'时 left[j] = j（用当前列）
//    ✓ 遇到'0'时 left[j] = 0, right[j] = n（重置为最宽，由下一行重新约束）
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力法怎么做？复杂度多少？
// → 枚举四个边界 O(m²n²)，前缀和 O(1) 检查。200×200 约 1.6e9 超时。
//
// Q2: 怎么把二维问题降为一维？
// → 逐行构建柱状图，heights[j]表示第j列从当前行往上连续1的个数。
//    每行变成84题"柱状图中最大矩形"，用单调栈O(n)求解，m行总计O(mn)。
//
// Q3: 单调栈是怎么保证不漏解的？
// → 栈中维护递增高度。对每个被弹出的柱子，它的右边界就是当前柱子，
//    左边界就是新栈顶。这保证了每个柱子作为最矮柱子时的最大宽度都被计算到。
//
// Q4: 不用栈能做吗？
// → 可以。用 height/left/right 三个数组的 DP 方案（解法3），
//    关键是 left[j] = max(left[j], curLeft) 同时考虑上方和当前行的约束。
//
// Q5: 如果改成求最大全1正方形呢？
// → LeetCode 221，DP: dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1
//    本质不同：矩形需要跟踪宽和高，正方形只需跟踪边长。
// ============================================================
