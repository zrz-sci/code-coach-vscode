/*
 * LeetCode 939 - Minimum Area Rectangle
 * Approach: Enumerate diagonal pairs + hash set lookup
 *
 * For an axis-aligned rectangle, two opposite corners (x1,y1) and (x2,y2)
 * uniquely define it.  The other two corners are (x1,y2) and (x2,y1).
 * We store all points in a hash set, then for every pair of points with
 * different x AND different y, check if the complementary corners exist.
 *
 * Time:  O(n^2)  -- enumerate all pairs, O(1) hash lookups per pair
 * Space: O(n)    -- hash set storing all points
 */

class Solution {
public:
    int minAreaRect(vector<vector<int>>& points) {
        int n = points.size();

        // ---- Step 1: Build a hash set for O(1) point existence checks ----
        // We use an unordered_map from x -> set of y values.
        // This gives us O(1) amortized lookup for "does point (a, b) exist?"
        // and also naturally groups points by x-coordinate.
        unordered_map<int, unordered_set<int>> pointSet;
        for (const auto& p : points) {
            pointSet[p[0]].insert(p[1]);
        }

        int minArea = INT_MAX;

        // ---- Step 2: Check every pair of points as potential diagonals ----
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int x1 = points[i][0], y1 = points[i][1];
                int x2 = points[j][0], y2 = points[j][1];

                // Skip pairs that share x or y coordinate --
                // they cannot be opposite corners of a rectangle
                if (x1 == x2 || y1 == y2) continue;

                // Check if the other two corners exist:
                //   corner A = (x1, y2)  and  corner B = (x2, y1)
                if (pointSet[x1].count(y2) && pointSet[x2].count(y1)) {
                    int area = abs(x2 - x1) * abs(y2 - y1);
                    minArea = min(minArea, area);
                }
            }
        }

        // If no rectangle was found, return 0
        return (minArea == INT_MAX) ? 0 : minArea;
    }
};

/*
 * Walkthrough with points = [[1,1],[1,3],[3,1],[3,3],[2,2]]:
 *
 * pointSet:
 *   1 -> {1, 3}
 *   3 -> {1, 3}
 *   2 -> {2}
 *
 * Checking pairs (showing only interesting ones):
 *
 *   (1,1) and (3,3): x1=1, y1=1, x2=3, y2=3
 *     Check (1,3): pointSet[1].count(3) = YES
 *     Check (3,1): pointSet[3].count(1) = YES
 *     area = |3-1| * |3-1| = 2 * 2 = 4
 *     minArea = 4
 *
 *   (1,1) and (2,2): x1=1, y1=1, x2=2, y2=2
 *     Check (1,2): pointSet[1].count(2) = NO  -> skip
 *
 *   (1,3) and (3,1): x1=1, y1=3, x2=3, y2=1
 *     Check (1,1): pointSet[1].count(1) = YES
 *     Check (3,3): pointSet[3].count(3) = YES
 *     area = |3-1| * |1-3| = 2 * 2 = 4
 *     minArea remains 4 (same rectangle found via other diagonal)
 *
 *   (other pairs with point (2,2) fail the corner check)
 *
 * Final answer: 4  -- CORRECT
 *
 * ---------------------------------------------------------------
 * Walkthrough with points = [[1,1],[1,3],[3,1],[3,3],[4,1],[4,3]]:
 *
 * pointSet:
 *   1 -> {1, 3}
 *   3 -> {1, 3}
 *   4 -> {1, 3}
 *
 * Key pair: (3,1) and (4,3):
 *   Check (3,3): YES.  Check (4,1): YES.
 *   area = |4-3| * |3-1| = 1 * 2 = 2
 *   minArea = 2
 *
 * Final answer: 2  -- CORRECT (rectangle formed by (3,1),(3,3),(4,1),(4,3))
 *
 * ---------------------------------------------------------------
 * Alternative approach: Group by X-coordinate
 *
 * For each pair of x-values (x1, x2) that share >= 2 common y-values,
 * sort the common y-values and check adjacent pairs for minimal height:
 *
 *   for each pair (x1, x2) with x1 < x2:
 *     commonYs = intersection of pointSet[x1] and pointSet[x2]
 *     sort commonYs
 *     for adjacent y_i, y_{i+1} in commonYs:
 *       area = (x2 - x1) * (y_{i+1} - y_i)
 *       update minArea
 *
 * This can be more efficient when there are many y-values per x
 * because we avoid re-checking non-existent corners. However, the
 * worst case remains O(n^2) and the code is more complex.
 * ---------------------------------------------------------------
 */
