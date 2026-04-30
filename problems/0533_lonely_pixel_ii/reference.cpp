/*
 * 【题目本质】
 * Lonely Pixel I 的进阶版：增加 target 参数和"行相同"约束
 * 核心思路：行编码做分组 + 行列计数三重验证
 *
 * 【解法总览】
 * 解法一：行编码 + 哈希表 ⭐推荐  — O(m*n) / O(m*n)
 * 解法二：暴力逐列验证            — O(m^2 * n) / O(m+n)
 */

// ===================== 解法一：行编码 + 哈希表（推荐） =====================
// 思路：将每行转为字符串做key，三个条件联合判断
//   条件1: rowCount[r] == target
//   条件2: colCount[c] == target
//   条件3: 行模式出现次数 == target（保证列中所有B所在行都相同）
class Solution {
public:
    int findBlackPixel(vector<vector<char>>& picture, int target) {
        int m = picture.size(), n = picture[0].size();
        vector<int> colCount(n, 0);
        unordered_map<string, int> rowPatternCount; // 行模式 -> 出现次数
        vector<string> rowStr(m);

        // 统计每列B数量 + 构建行字符串编码
        for (int i = 0; i < m; i++) {
            string s(picture[i].begin(), picture[i].end());
            rowStr[i] = s;
            rowPatternCount[s]++;
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B') colCount[j]++;
            }
        }

        int result = 0;
        for (int i = 0; i < m; i++) {
            // 条件1: 行中B的数量等于target
            int rowB = count(picture[i].begin(), picture[i].end(), 'B');
            if (rowB != target) continue;
            // 条件3: 相同行模式出现次数等于target
            if (rowPatternCount[rowStr[i]] != target) continue;

            // 条件2: 列中B的数量等于target
            for (int j = 0; j < n; j++) {
                if (picture[i][j] == 'B' && colCount[j] == target) {
                    result++;
                }
            }
        }
        return result;
    }
};

// ===================== 解法二：暴力逐列验证 =====================
// 思路：对每个候选B位置，逐一验证列中所有B所在行是否与当前行相同
class Solution2 {
public:
    int findBlackPixel(vector<vector<char>>& picture, int target) {
        int m = picture.size(), n = picture[0].size();
        vector<int> rowCount(m, 0), colCount(n, 0);

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (picture[i][j] == 'B') {
                    rowCount[i]++;
                    colCount[j]++;
                }

        int result = 0;
        for (int i = 0; i < m; i++) {
            if (rowCount[i] != target) continue; // 行B数 != target，跳过
            for (int j = 0; j < n; j++) {
                if (picture[i][j] != 'B' || colCount[j] != target) continue;
                // 暴力验证：列j中所有有B的行是否和行i完全相同
                bool valid = true;
                for (int r = 0; r < m && valid; r++) {
                    if (picture[r][j] == 'B' && r != i) {
                        // 逐字符比较行r和行i
                        for (int c = 0; c < n; c++) {
                            if (picture[r][c] != picture[i][c]) {
                                valid = false;
                                break;
                            }
                        }
                    }
                }
                if (valid) result++;
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 行编码+哈希 | O(m*n) | O(m*n) | 高效，避免重复比较 |
 * | 暴力验证 | O(m^2*n) | O(m+n) | 空间少但慢 |
 *
 * 【易错点】
 * 1. 三个条件缺一不可：rowCount、colCount、行模式次数都要等于target
 * 2. "行相同"是整行所有字符相同，不仅仅是B的位置相同
 * 3. 行模式次数 == target（不是 >= target）
 *
 * 【面试追问】
 * Q1: 为什么行模式次数==target就够了？
 *     → colCount[j]==target 保证列有target个B，行模式相同的有target个，
 *       这些行在列j的字符相同（B或W），恰好能匹配
 * Q2: 字符串编码的空间能否优化？
 *     → 可用行哈希代替，但要处理冲突
 * Q3: 与 Lonely Pixel I 的关系？
 *     → I 是 target=1 且无行相同约束的特例
 */
