/*
 * LeetCode 885: Spiral Matrix III
 *
 * 解法：模拟螺旋行走
 *
 * 思路：
 * 从 (rStart, cStart) 出发，按 右->下->左->上 顺时针方向行走。
 * 步长模式：1,1,2,2,3,3,4,4,...（每两个方向共享步长，然后步长+1）
 * 行走时可能走出网格，继续走但不记录。收集够 rows*cols 个位置后停止。
 *
 * 时间复杂度：O(max(rows, cols)^2)
 * 空间复杂度：O(rows * cols) 存结果
 */

class Solution {
public:
    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
        // 四个方向：右、下、左、上（顺时针）
        // dirs[i] = {行偏移, 列偏移}
        int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        int total = rows * cols;
        vector<vector<int>> result;
        result.reserve(total);  // 预分配空间提升性能

        // 起始位置是第一个被访问的点
        int r = rStart, c = cStart;
        result.push_back({r, c});

        if (total == 1) return result;

        int steps = 1;  // 当前步长
        int dir = 0;    // 当前方向索引（0=右, 1=下, 2=左, 3=上）

        // 持续行走直到收集够所有位置
        while ((int)result.size() < total) {
            // 每个步长会被用于两个方向
            // 第一个方向用 steps 步，第二个方向也用 steps 步
            // 然后 steps++
            for (int round = 0; round < 2; round++) {
                // 沿当前方向走 steps 步
                for (int i = 0; i < steps; i++) {
                    r += dirs[dir][0];
                    c += dirs[dir][1];

                    // 如果在网格内，记录这个位置
                    if (r >= 0 && r < rows && c >= 0 && c < cols) {
                        result.push_back({r, c});
                        if ((int)result.size() == total) {
                            return result;
                        }
                    }
                }

                // 切换到下一个方向
                dir = (dir + 1) % 4;
            }

            // 两个方向完成后，步长+1
            steps++;
        }

        return result;
    }
};

/*
 * 详细示例分析：
 *
 * rows=5, cols=6, rStart=1, cStart=4
 *
 * 从 (1,4) 开始：result = [(1,4)]
 *
 * steps=1:
 *   方向0（右），走1步：(1,5) -> 在网格内，记录
 *   方向1（下），走1步：(2,5) -> 在网格内，记录
 *
 * steps=2:
 *   方向2（左），走2步：(2,4), (2,3) -> 都在网格内
 *   方向3（上），走2步：(1,3), (0,3) -> 都在网格内
 *
 * steps=3:
 *   方向0（右），走3步：(0,4), (0,5), (0,6) -> (0,6)超出列范围，不记录
 *   方向1（下），走3步：(1,6), (2,6), (3,6) -> 全部超出列范围，不记录
 *   （注意：虽然超出但坐标会更新）
 *   等等...
 *
 * 最终按螺旋顺序收集所有30个位置。
 */

/*
 * 步长变化的另一种理解方式：
 *
 * 在一个以起点为中心的螺旋中：
 * - 第1圈：右1, 下1, 左2, 上2  (步长: 1,1,2,2)
 * - 第2圈：右3, 下3, 左4, 上4  (步长: 3,3,4,4)
 * - 第3圈：右5, 下5, 左6, 上6  (步长: 5,5,6,6)
 *
 * 但更好的理解是两两一组：
 * - (右1, 下1) -> steps变为2
 * - (左2, 上2) -> steps变为3
 * - (右3, 下3) -> steps变为4
 * ...
 *
 * 这就是代码中 for (int round = 0; round < 2; round++) 的含义。
 */

/*
 * 解法二：带网格外跳步优化的模拟（面试中不推荐但可了解）
 *
 * 当沿某方向行走时，如果整段都在网格外，可以直接跳过而非逐步模拟。
 * 这样理论上时间复杂度可以优化为 O(rows * cols)。
 */
class Solution_Optimized {
public:
    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
        int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        int total = rows * cols;
        vector<vector<int>> result;
        result.reserve(total);

        int r = rStart, c = cStart;
        result.push_back({r, c});

        int steps = 1;
        int dir = 0;

        while ((int)result.size() < total) {
            for (int round = 0; round < 2; round++) {
                // 计算沿当前方向走 steps 步后的位置
                int endR = r + dirs[dir][0] * steps;
                int endC = c + dirs[dir][1] * steps;

                // 确定在网格内的有效步数范围
                for (int i = 0; i < steps; i++) {
                    r += dirs[dir][0];
                    c += dirs[dir][1];

                    if (r >= 0 && r < rows && c >= 0 && c < cols) {
                        result.push_back({r, c});
                        if ((int)result.size() == total) {
                            return result;
                        }
                    }
                }

                dir = (dir + 1) % 4;
            }
            steps++;
        }

        return result;
    }
};

/*
 * 螺旋矩阵系列题目对比：
 *
 * LC 54  (Spiral Matrix):
 *   - 从左上角开始，只在矩阵内螺旋
 *   - 边界不断收缩（top/bottom/left/right 四个边界）
 *   - 不会走到矩阵外
 *
 * LC 59  (Spiral Matrix II):
 *   - 从左上角开始，填充 n x n 矩阵
 *   - 同样是边界收缩法
 *
 * LC 885 (Spiral Matrix III - 本题):
 *   - 从任意位置开始
 *   - 会走到网格外（但不记录）
 *   - 使用步长递增法而非边界收缩法
 *   - 步长模式：1,1,2,2,3,3,4,4,...
 *
 * LC 2326 (Spiral Matrix IV):
 *   - 从左上角开始，用链表值填充矩阵
 *   - 类似 LC 59 但数据源是链表
 */
