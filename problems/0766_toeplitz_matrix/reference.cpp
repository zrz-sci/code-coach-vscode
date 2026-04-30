// 解法一：逐元素比较左上角邻居（最优解）
// 时间 O(m*n)，空间 O(1)
// 核心思路：Toeplitz 等价于每个元素等于其左上角邻居 matrix[r-1][c-1]
class Solution {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();

        // 从 (1,1) 开始遍历，检查每个元素是否等于其左上角
        for (int r = 1; r < m; ++r) {
            for (int c = 1; c < n; ++c) {
                if (matrix[r][c] != matrix[r - 1][c - 1]) {
                    return false;
                }
            }
        }
        return true;
    }
};

// 解法二：对角线分组法
// 时间 O(m*n)，空间 O(m+n)
// 核心思路：同一对角线上 r-c 相同，用 map 存每条对角线的标准值
class Solution2 {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();

        // key = r - c (对角线编号)，value = 该对角线上第一个元素的值
        unordered_map<int, int> diagValue;

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                int diag = r - c;
                if (diagValue.find(diag) == diagValue.end()) {
                    // 第一次遇到这条对角线，记录标准值
                    diagValue[diag] = matrix[r][c];
                } else {
                    // 后续元素必须与标准值相同
                    if (diagValue[diag] != matrix[r][c]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};

// 解法三：流式处理（Follow-up: 一次只能读一行）
// 时间 O(m*n)，空间 O(n)
// 核心思路：保留上一行数据，逐行比较 curRow[c] vs prevRow[c-1]
class Solution3 {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();

        // 模拟流式读取：prevRow 保存上一行
        vector<int> prevRow = matrix[0];

        for (int r = 1; r < m; ++r) {
            // curRow 是"刚从磁盘读入的当前行"
            vector<int>& curRow = matrix[r];

            // 比较当前行与上一行（偏移 1 位）
            // curRow[c] 应该等于 prevRow[c-1]（同一对角线）
            for (int c = 1; c < n; ++c) {
                if (curRow[c] != prevRow[c - 1]) {
                    return false;
                }
            }

            // 更新 prevRow 为当前行
            prevRow = curRow;
        }
        return true;
    }
};

// 解法四：首行首列定义法
// 时间 O(m*n)，空间 O(1)
// 核心思路：验证每个元素等于其所在对角线的起始元素
// 对角线起始元素在第一行或第一列
class Solution4 {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                // 找到这条对角线的起始位置
                // 沿对角线往左上走，直到碰到第一行或第一列
                int steps = min(r, c);
                int startR = r - steps;
                int startC = c - steps;

                // 当前元素必须等于对角线起始元素
                if (matrix[r][c] != matrix[startR][startC]) {
                    return false;
                }
            }
        }
        return true;
    }
};

// 解法五：逐条对角线检查
// 时间 O(m*n)，空间 O(1)
// 核心思路：枚举所有对角线，检查每条对角线上元素是否一致
class Solution5 {
public:
    bool isToeplitzMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();

        // 从第一列出发的对角线 (startR, 0)
        for (int startR = 0; startR < m; ++startR) {
            int val = matrix[startR][0];
            int r = startR + 1, c = 1;
            while (r < m && c < n) {
                if (matrix[r][c] != val) return false;
                ++r; ++c;
            }
        }

        // 从第一行出发的对角线 (0, startC)，跳过 (0,0) 避免重复
        for (int startC = 1; startC < n; ++startC) {
            int val = matrix[0][startC];
            int r = 1, c = startC + 1;
            while (r < m && c < n) {
                if (matrix[r][c] != val) return false;
                ++r; ++c;
            }
        }

        return true;
    }
};
