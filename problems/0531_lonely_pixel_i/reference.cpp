/*
 * 【题目本质】
 * 统计矩阵中"孤独"的黑色像素：所在行和列都恰好只有1个'B'
 * 核心思路：行列计数 + 条件过滤
 *
 * 【解法总览】
 * 解法一：两遍遍历 + 行列计数 ⭐推荐  — O(m*n) / O(m+n)
 * 解法二：优化遍历（记录B的位置）      — O(m*n) / O(m+n+k), k为B的个数
 */

// ===================== 解法一：两遍遍历 + 行列计数（推荐） =====================
// 思路：第一遍统计每行每列B数量，第二遍检查每个B是否满足条件
class Solution {
public:
    int findLonelyPixel(vector<vector<char>>& picture) {
        int m = picture.size(), n = picture[0].size();
        vector<int> rowCount(m, 0), colCount(n, 0);

        // 第一遍：统计每行每列的'B'数量
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B') {
                    rowCount[i]++;
                    colCount[j]++;
                }
            }
        }

        // 第二遍：检查每个'B'是否为孤独像素
        int result = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // 当前位置是'B'，且所在行列都只有1个B
                if (picture[i][j] == 'B' && rowCount[i] == 1 && colCount[j] == 1) {
                    result++;
                }
            }
        }
        return result;
    }
};

// ===================== 解法二：稀疏优化（记录B的位置） =====================
// 思路：先收集所有B的位置，再筛选满足条件的
// 适合B很少（矩阵稀疏）的场景
class Solution2 {
public:
    int findLonelyPixel(vector<vector<char>>& picture) {
        int m = picture.size(), n = picture[0].size();
        vector<int> rowCount(m, 0), colCount(n, 0);
        vector<pair<int,int>> blacks; // 记录所有B的位置

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B') {
                    rowCount[i]++;
                    colCount[j]++;
                    blacks.push_back({i, j});
                }
            }
        }

        int result = 0;
        for (auto& [r, c] : blacks) {
            if (rowCount[r] == 1 && colCount[c] == 1) {
                result++;
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 两遍遍历 | O(m*n) | O(m+n) | 代码简洁直观 |
 * | 稀疏优化 | O(m*n) | O(m+n+k) | B很少时第二遍更快 |
 *
 * 【易错点】
 * 1. 必须同时满足行和列都只有1个B
 * 2. 第二遍必须先检查 picture[i][j]=='B'
 * 3. 统计是"恰好1个"而非"至少1个"
 *
 * 【面试追问】
 * Q1: 能否一遍完成？
 *     → 不太方便，因为处理某个B时还不知道列中最终有多少B
 * Q2: 稀疏矩阵如何优化？
 *     → 记录B的位置列表，第二遍只遍历这些位置
 * Q3: 与 Lonely Pixel II (533) 的区别？
 *     → II 增加了 target 参数和"行相同"约束
 */
