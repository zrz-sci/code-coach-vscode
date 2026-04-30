/*
 * LeetCode 883: Projection Area of 3D Shapes
 *
 * 解法一：一次遍历法
 *
 * 思路：
 * - 俯视图（xy）：grid[i][j] > 0 的格子数
 * - 正视图（yz）：每行最大值之和
 * - 侧视图（zx）：每列最大值之和
 *
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(n)
 */

class Solution {
public:
    int projectionArea(vector<vector<int>>& grid) {
        int n = grid.size();

        int topView = 0;    // 俯视图面积（xy平面）
        int frontView = 0;  // 正视图面积（yz平面）
        int sideView = 0;   // 侧视图面积（zx平面）

        // colMax[j] 记录第 j 列的最大值，用于计算侧视图
        vector<int> colMax(n, 0);

        for (int i = 0; i < n; i++) {
            int rowMax = 0;  // 当前行的最大值

            for (int j = 0; j < n; j++) {
                // 俯视图：只要有方块（值 > 0），投影面积 +1
                if (grid[i][j] > 0) {
                    topView++;
                }

                // 更新当前行最大值（用于正视图）
                rowMax = max(rowMax, grid[i][j]);

                // 更新当前列最大值（用于侧视图）
                colMax[j] = max(colMax[j], grid[i][j]);
            }

            // 正视图：该行贡献 = 该行最高的方块高度
            frontView += rowMax;
        }

        // 侧视图：每列贡献 = 该列最高的方块高度
        for (int j = 0; j < n; j++) {
            sideView += colMax[j];
        }

        return topView + frontView + sideView;
    }
};

/*
 * 详细示例分析：
 *
 * grid = [[1,2],[3,4]]
 *
 * 俯视图（从上往下看）：
 *   [1,2]  -> 两个非零格子
 *   [3,4]  -> 两个非零格子
 *   topView = 4
 *
 * 正视图（从前往后看，看到每行最高的）：
 *   第0行：max(1,2) = 2
 *   第1行：max(3,4) = 4
 *   frontView = 2 + 4 = 6
 *
 * 侧视图（从侧面看，看到每列最高的）：
 *   第0列：max(1,3) = 3
 *   第1列：max(2,4) = 4
 *   sideView = 3 + 4 = 7
 *
 * 总面积 = 4 + 6 + 7 = 17 (correct!)
 */

/*
 * 解法二：O(1) 额外空间的三次遍历法
 *
 * 分三步分别计算三个投影面积，无需额外数组。
 */
class Solution_ThreePass {
public:
    int projectionArea(vector<vector<int>>& grid) {
        int n = grid.size();
        int total = 0;

        // 第一步：计算俯视图面积
        // 所有 grid[i][j] > 0 的格子数
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] > 0) {
                    total++;
                }
            }
        }

        // 第二步：计算正视图面积（每行最大值之和）
        // 从前面看，同一行中最高的方块决定了该行的投影高度
        for (int i = 0; i < n; i++) {
            int rowMax = 0;
            for (int j = 0; j < n; j++) {
                rowMax = max(rowMax, grid[i][j]);
            }
            total += rowMax;
        }

        // 第三步：计算侧视图面积（每列最大值之和）
        // 从侧面看，同一列中最高的方块决定了该列的投影高度
        for (int j = 0; j < n; j++) {
            int colMax = 0;
            for (int i = 0; i < n; i++) {
                colMax = max(colMax, grid[i][j]);
            }
            total += colMax;
        }

        return total;
    }
};

/*
 * 投影面积的几何直觉：
 *
 * 想象你把积木搭在一个棋盘上。
 *
 * 1. 俯视图（从天花板往下看）：
 *    你看到的是"哪些格子上有积木"。有积木的格子看起来是实心的，
 *    没有积木的是空的。不管积木堆多高，从上面看都是一个格子的面积。
 *
 * 2. 正视图（从棋盘前面往后看）：
 *    你看到的是每一行中最高的积木。矮的积木被高的遮住了。
 *    所以每一行的投影高度 = 该行中最高积木的高度。
 *
 * 3. 侧视图（从棋盘侧面看）：
 *    你看到的是每一列中最高的积木。同理，投影高度 = 列最大值。
 *
 * 特殊情况：grid = [[1,0],[0,2]]
 *   俯视：2 个非零格子 -> 2
 *   正视：max(1,0)=1, max(0,2)=2 -> 3
 *   侧视：max(1,0)=1, max(0,2)=2 -> 3
 *   总计：2 + 3 + 3 = 8
 */
