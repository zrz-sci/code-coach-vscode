/*
 * 【题目本质】
 * 二维矩阵单点更新 + 矩形区域求和 → 二维树状数组（BIT）
 * 是 307 题的二维推广，核心是两层嵌套的 lowbit 跳跃
 *
 * 【解法总览】
 * Solution1: 二维 BIT  ⭐⭐⭐⭐⭐  update O(log m * log n) / query O(log m * log n)
 * Solution2: 按行 BIT  ⭐⭐⭐      update O(log n) / query O(m * log n)
 */

// ===================== Solution1: 二维树状数组 BIT（推荐） =====================
class NumMatrix1 {
    vector<vector<int>> tree; // 二维 BIT，(1,1)-indexed
    vector<vector<int>> mat;  // 保存原始值（计算 update 差值）
    int m, n;

    int lowbit(int x) { return x & (-x); }

    // 二维单点加：嵌套两层 lowbit 跳跃
    void add(int r, int c, int delta) {
        for (int i = r; i <= m; i += lowbit(i))
            for (int j = c; j <= n; j += lowbit(j))
                tree[i][j] += delta;
    }

    // 二维前缀和查询：返回 [1,1] 到 [r,c] 矩形的和
    int query(int r, int c) {
        int sum = 0;
        for (int i = r; i > 0; i -= lowbit(i))
            for (int j = c; j > 0; j -= lowbit(j))
                sum += tree[i][j];
        return sum;
    }

public:
    NumMatrix1(vector<vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        mat = matrix;
        tree.assign(m + 1, vector<int>(n + 1, 0));
        // 初始化：逐个元素 add 到 BIT
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                add(i + 1, j + 1, matrix[i][j]);
    }

    void update(int row, int col, int val) {
        int delta = val - mat[row][col]; // 差值
        add(row + 1, col + 1, delta);    // 0-indexed → 1-indexed
        mat[row][col] = val;              // 同步更新原矩阵
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        // 容斥原理：大矩形 - 上方 - 左方 + 左上角
        return query(row2 + 1, col2 + 1)
             - query(row1,     col2 + 1)
             - query(row2 + 1, col1)
             + query(row1,     col1);
    }
};

// ===================== Solution2: 按行建 BIT =====================
// 每行维护一个一维 BIT，sumRegion 遍历行
// update O(log n)，sumRegion O(m * log n)
class NumMatrix2 {
    vector<vector<int>> trees; // trees[i] 是第 i 行的一维 BIT
    vector<vector<int>> mat;
    int m, n;

    int lowbit(int x) { return x & (-x); }

    void add(int row, int col, int delta) {
        for (int j = col; j <= n; j += lowbit(j))
            trees[row][j] += delta;
    }

    int query(int row, int col) {
        int sum = 0;
        for (int j = col; j > 0; j -= lowbit(j))
            sum += trees[row][j];
        return sum;
    }

public:
    NumMatrix2(vector<vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        mat = matrix;
        trees.assign(m, vector<int>(n + 1, 0));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                add(i, j + 1, matrix[i][j]);
    }

    void update(int row, int col, int val) {
        add(row, col + 1, val - mat[row][col]);
        mat[row][col] = val;
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        int sum = 0;
        for (int i = row1; i <= row2; i++)
            sum += query(i, col2 + 1) - query(i, col1);
        return sum;
    }
};

/*
 * 【解法对比】
 * ┌──────────────────┬────────────────────┬──────────┬──────────────────────┐
 * │ 解法             │ update / query     │ 空间     │ 特点                 │
 * ├──────────────────┼────────────────────┼──────────┼──────────────────────┤
 * │ Solution1 二维BIT│ O(logm*logn) 每次  │ O(mn)    │ 最优解，推荐首选     │
 * │ Solution2 按行BIT│ O(logn) / O(m*logn)│ O(mn)    │ 实现简单，query 慢   │
 * └──────────────────┴────────────────────┴──────────┴──────────────────────┘
 *
 * 【易错点】
 * 1. 二维 BIT 索引从 (1,1) 开始，数组大小 (m+1) x (n+1)
 * 2. update 传差值 val - mat[row][col]，不是直接传 val
 * 3. 容斥公式四项缺一不可：+ q(r2,c2) - q(r1-1,c2) - q(r2,c1-1) + q(r1-1,c1-1)
 * 4. 更新 BIT 后必须同步 mat[row][col] = val
 * 5. 0-indexed 到 1-indexed 转换：外部 row → BIT 内部 row+1
 *
 * 【面试追问】
 * Q1: 二维 BIT 的空间/时间？→ O(mn) / O(logm * logn)
 * Q2: 如何从一维推广到二维？→ 嵌套两层 lowbit 循环
 * Q3: 区间更新+区间查询？→ 二维差分 + 两个二维 BIT
 */
