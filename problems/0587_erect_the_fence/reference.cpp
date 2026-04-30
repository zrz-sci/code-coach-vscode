// 587. Erect the Fence - Andrew's Monotone Chain 凸包算法
// 时间 O(n log n)  空间 O(n)

class Solution {
public:
    // 叉积: 判断 O->A->B 的转向
    // > 0 逆时针(左转), == 0 共线, < 0 顺时针(右转)
    int cross(const vector<int>& O, const vector<int>& A, const vector<int>& B) {
        return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0]);
    }

    vector<vector<int>> outerTrees(vector<vector<int>>& trees) {
        int n = trees.size();
        if (n <= 3) return trees;  // 3个及以下点全在凸包上

        // 按 x 升序，x 相同按 y 升序
        sort(trees.begin(), trees.end());

        vector<vector<int>> hull;

        // 构建下凸包 (从左到右)
        for (int i = 0; i < n; ++i) {
            // 用 < 0 而非 <= 0 弹栈，保留共线点
            while (hull.size() >= 2 &&
                   cross(hull[hull.size()-2], hull[hull.size()-1], trees[i]) < 0) {
                hull.pop_back();
            }
            hull.push_back(trees[i]);
        }

        // 构建上凸包 (从右到左)
        int lowerSize = hull.size();
        for (int i = n - 2; i >= 0; --i) {
            while ((int)hull.size() > lowerSize &&
                   cross(hull[hull.size()-2], hull[hull.size()-1], trees[i]) < 0) {
                hull.pop_back();
            }
            hull.push_back(trees[i]);
        }

        // 去重 (上下凸包端点重复)
        set<vector<int>> unique(hull.begin(), hull.end());
        return vector<vector<int>>(unique.begin(), unique.end());
    }
};

// ============================================================
// 解法二: Jarvis March (Gift Wrapping)
// 时间 O(nh)  空间 O(n)
// 适合凸包顶点少的场景, 且天然能收集共线点
// ============================================================
/*
class Solution {
public:
    int cross(const vector<int>& O, const vector<int>& A, const vector<int>& B) {
        return (A[0] - O[0]) * (B[1] - O[1]) - (A[1] - O[1]) * (B[0] - O[0]);
    }

    int dist(const vector<int>& A, const vector<int>& B) {
        return (A[0]-B[0])*(A[0]-B[0]) + (A[1]-B[1])*(A[1]-B[1]);
    }

    vector<vector<int>> outerTrees(vector<vector<int>>& trees) {
        int n = trees.size();
        if (n <= 3) return trees;

        // 找最左下角的点作为起始
        int start = 0;
        for (int i = 1; i < n; ++i) {
            if (trees[i][0] < trees[start][0] ||
                (trees[i][0] == trees[start][0] && trees[i][1] < trees[start][1]))
                start = i;
        }

        set<int> visited;
        int cur = start;
        do {
            visited.insert(cur);
            int next = 0;
            for (int i = 0; i < n; ++i) {
                if (i == cur) continue;
                int c = cross(trees[cur], trees[next], trees[i]);
                if (next == cur || c > 0 ||
                    (c == 0 && dist(trees[cur], trees[i]) > dist(trees[cur], trees[next]))) {
                    next = i;
                }
            }
            // 收集与 cur->next 共线的所有点
            for (int i = 0; i < n; ++i) {
                if (i != cur && i != next &&
                    cross(trees[cur], trees[next], trees[i]) == 0) {
                    visited.insert(i);
                }
            }
            cur = next;
        } while (cur != start);

        vector<vector<int>> result;
        for (int idx : visited) {
            result.push_back(trees[idx]);
        }
        return result;
    }
};
*/
