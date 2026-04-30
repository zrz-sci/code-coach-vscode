// LeetCode 963: Minimum Area Rectangle II
// ./solution.cpp

// ============================================================
// Approach 1: Diagonal Grouping (Optimal for larger n)
// ============================================================
// Group pairs of points by (midpoint, squared diagonal length).
// Two pairs in the same group form a rectangle. Compute area
// for all such rectangle candidates using cross product.
//
// Time:  O(n^2) amortized for grouping + checking
// Space: O(n^2) for the hash map

class Solution {
public:
    double minAreaFreeRect(vector<vector<int>>& points) {
        int n = points.size();
        if (n < 4) return 0.0;

        // Map: (mx, my, dist2) -> list of {i, j} pairs
        // Use mx = x1+x2, my = y1+y2 to avoid floating point
        // dist2 = squared distance between the pair
        map<tuple<int,int,int>, vector<pair<int,int>>> groups;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int mx = points[i][0] + points[j][0];
                int my = points[i][1] + points[j][1];
                int dx = points[i][0] - points[j][0];
                int dy = points[i][1] - points[j][1];
                int dist2 = dx * dx + dy * dy;

                groups[{mx, my, dist2}].push_back({i, j});
            }
        }

        double minArea = DBL_MAX;
        bool found = false;

        for (auto& [key, pairs] : groups) {
            int sz = pairs.size();
            if (sz < 2) continue;

            // Check all combinations of two pairs in the same group
            for (int a = 0; a < sz; a++) {
                for (int b = a + 1; b < sz; b++) {
                    // Pair a: diagonal (P[i1], P[j1])
                    // Pair b: diagonal (P[i2], P[j2])
                    int i1 = pairs[a].first, j1 = pairs[a].second;
                    int i2 = pairs[b].first, j2 = pairs[b].second;

                    // The four vertices of the potential rectangle are:
                    // points[i1], points[j1], points[i2], points[j2]
                    // where (i1,j1) and (i2,j2) are the two diagonals.
                    //
                    // Take vertex A = points[i1]
                    // Adjacent vertices are points[i2] and points[j2]
                    // Area = |AB x AC| where B = points[i2], C = points[j2]

                    double ax = points[i2][0] - points[i1][0];
                    double ay = points[i2][1] - points[i1][1];
                    double bx = points[j2][0] - points[i1][0];
                    double by = points[j2][1] - points[i1][1];

                    double area = abs(ax * by - ay * bx);
                    if (area > 1e-9) { // skip degenerate (collinear) cases
                        found = true;
                        minArea = min(minArea, area);
                    }
                }
            }
        }

        return found ? minArea : 0.0;
    }
};


// ============================================================
// Approach 2: Brute Force with Right-Angle Check (O(n^3))
// ============================================================
// For each triple (A, B, C), check if angle at A is 90 degrees.
// If so, the fourth point D = B + C - A must exist in the set.
// Area = |AB| * |AC|.
//
// Time:  O(n^3)
// Space: O(n) for the point set

class Solution2 {
public:
    double minAreaFreeRect(vector<vector<int>>& points) {
        int n = points.size();
        if (n < 4) return 0.0;

        // Store all points in a set for O(1) lookup
        set<pair<int,int>> pointSet;
        for (auto& p : points) {
            pointSet.insert({p[0], p[1]});
        }

        double minArea = DBL_MAX;
        bool found = false;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                for (int k = j + 1; k < n; k++) {
                    if (k == i) continue;

                    // Check if angle at point i is 90 degrees
                    // Vector ij and ik should be perpendicular
                    int vx1 = points[j][0] - points[i][0];
                    int vy1 = points[j][1] - points[i][1];
                    int vx2 = points[k][0] - points[i][0];
                    int vy2 = points[k][1] - points[i][1];

                    // Dot product must be zero for perpendicularity
                    if (vx1 * vx2 + vy1 * vy2 != 0) continue;

                    // Compute 4th vertex: D = B + C - A
                    int dx = points[j][0] + points[k][0] - points[i][0];
                    int dy = points[j][1] + points[k][1] - points[i][1];

                    if (pointSet.count({dx, dy})) {
                        // Compute area = |ij| * |ik|
                        double lenIJ = sqrt((double)(vx1*vx1 + vy1*vy1));
                        double lenIK = sqrt((double)(vx2*vx2 + vy2*vy2));
                        double area = lenIJ * lenIK;

                        if (area > 1e-9) {
                            found = true;
                            minArea = min(minArea, area);
                        }
                    }
                }
            }
        }

        return found ? minArea : 0.0;
    }
};


// ============================================================
// Approach 3: Diagonal Grouping with unordered_map
// ============================================================
// Same logic as Approach 1 but uses a custom hash for
// unordered_map to achieve better average-case performance.
//
// Time:  O(n^2) amortized
// Space: O(n^2)

class Solution3 {
public:
    double minAreaFreeRect(vector<vector<int>>& points) {
        int n = points.size();
        if (n < 4) return 0.0;

        // Custom hash combining three integers
        // Use a nested map: outer key = (mx, my), inner key = dist2
        map<pair<int,int>, map<int, vector<pair<int,int>>>> groups;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int mx = points[i][0] + points[j][0];
                int my = points[i][1] + points[j][1];
                int dx = points[i][0] - points[j][0];
                int dy = points[i][1] - points[j][1];
                int dist2 = dx * dx + dy * dy;

                groups[{mx, my}][dist2].push_back({i, j});
            }
        }

        double minArea = DBL_MAX;
        bool found = false;

        for (auto& [midPt, distMap] : groups) {
            for (auto& [d2, pairs] : distMap) {
                int sz = pairs.size();
                if (sz < 2) continue;

                for (int a = 0; a < sz; a++) {
                    for (int b = a + 1; b < sz; b++) {
                        int i1 = pairs[a].first, j1 = pairs[a].second;
                        int i2 = pairs[b].first, j2 = pairs[b].second;

                        // Compute area via cross product of two adjacent sides
                        // from vertex points[i1] to points[i2] and points[j2]
                        double ax = points[i2][0] - points[i1][0];
                        double ay = points[i2][1] - points[i1][1];
                        double bx = points[j2][0] - points[i1][0];
                        double by = points[j2][1] - points[i1][1];

                        double area = abs(ax * by - ay * bx);
                        if (area > 1e-9) {
                            found = true;
                            minArea = min(minArea, area);
                        }
                    }
                }
            }
        }

        return found ? minArea : 0.0;
    }
};
