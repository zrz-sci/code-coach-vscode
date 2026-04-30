// LeetCode 836: Rectangle Overlap
// 解法一：区间重叠法（正向判断）
// 时间复杂度: O(1), 空间复杂度: O(1)

class Solution {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        // 矩形重叠 = x方向区间重叠 AND y方向区间重叠
        // 一维区间 [a,b] 和 [c,d] 重叠条件: a < d && c < b
        //
        // rec1: [x1, y1, x2, y2] = [rec1[0], rec1[1], rec1[2], rec1[3]]
        // rec2: [x1, y1, x2, y2] = [rec2[0], rec2[1], rec2[2], rec2[3]]
        //
        // X方向: rec1的x范围[rec1[0], rec1[2]], rec2的x范围[rec2[0], rec2[2]]
        // Y方向: rec1的y范围[rec1[1], rec1[3]], rec2的y范围[rec2[1], rec2[3]]

        bool xOverlap = rec1[0] < rec2[2] && rec2[0] < rec1[2];
        bool yOverlap = rec1[1] < rec2[3] && rec2[1] < rec1[3];

        return xOverlap && yOverlap;
    }
};

// ================================================================
// 解法二：反向排除法
// 时间复杂度: O(1), 空间复杂度: O(1)

class Solution_Exclude {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        // 四种不重叠的情况（互斥）:
        // 1. rec1在rec2左边: rec1[2] <= rec2[0]  (rec1的右边界 <= rec2的左边界)
        // 2. rec1在rec2右边: rec1[0] >= rec2[2]  (rec1的左边界 >= rec2的右边界)
        // 3. rec1在rec2下边: rec1[3] <= rec2[1]  (rec1的上边界 <= rec2的下边界)
        // 4. rec1在rec2上边: rec1[1] >= rec2[3]  (rec1的下边界 >= rec2的上边界)

        bool noOverlap = rec1[2] <= rec2[0] ||  // rec1在rec2左边
                          rec1[0] >= rec2[2] ||  // rec1在rec2右边
                          rec1[3] <= rec2[1] ||  // rec1在rec2下边
                          rec1[1] >= rec2[3];    // rec1在rec2上边

        return !noOverlap;
    }
};

// ================================================================
// 解法三：计算交集面积法
// 时间复杂度: O(1), 空间复杂度: O(1)

class Solution_Area {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        // 计算重叠区域的宽度和高度
        // 重叠区域的左下角: (max(x1_1, x1_2), max(y1_1, y1_2))
        // 重叠区域的右上角: (min(x2_1, x2_2), min(y2_1, y2_2))

        int overlapWidth  = min(rec1[2], rec2[2]) - max(rec1[0], rec2[0]);
        int overlapHeight = min(rec1[3], rec2[3]) - max(rec1[1], rec2[1]);

        // 如果宽度和高度都 > 0，则有正面积的重叠区域
        return overlapWidth > 0 && overlapHeight > 0;
    }
};

// ================================================================
// 扩展：计算两个矩形的交集矩形坐标（如果重叠的话）
// 这在很多几何问题中是一个常用的子函数

class Solution_Intersection {
public:
    // 返回交集矩形，如果不重叠返回空vector
    vector<int> getIntersection(vector<int>& rec1, vector<int>& rec2) {
        int x1 = max(rec1[0], rec2[0]);
        int y1 = max(rec1[1], rec2[1]);
        int x2 = min(rec1[2], rec2[2]);
        int y2 = min(rec1[3], rec2[3]);

        if (x1 < x2 && y1 < y2) {
            return {x1, y1, x2, y2};
        }
        return {}; // 不重叠
    }

    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        return !getIntersection(rec1, rec2).empty();
    }
};

// ================================================================
// 扩展：N个矩形中找出所有重叠对
// 使用排序 + 扫描线思想

class Solution_NRectangles {
public:
    // 返回所有重叠的矩形对的索引
    vector<pair<int,int>> findOverlappingPairs(vector<vector<int>>& rects) {
        int n = rects.size();
        vector<pair<int,int>> result;

        // 按x1排序（扫描线的事件点）
        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);
        sort(indices.begin(), indices.end(), [&](int a, int b) {
            return rects[a][0] < rects[b][0];
        });

        // 简单的O(n^2)检查，但通过排序可以提前终止
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int a = indices[i], b = indices[j];
                // 如果rects[b]的左边界 >= rects[a]的右边界，
                // 后面的矩形更不可能与a重叠
                if (rects[b][0] >= rects[a][2]) break;

                // 检查y方向是否也有重叠
                if (rects[a][1] < rects[b][3] && rects[b][1] < rects[a][3]) {
                    result.push_back({a, b});
                }
            }
        }

        return result;
    }
};
