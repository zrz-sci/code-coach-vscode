# LeetCode 963 - Minimum Area Rectangle II

[View Solution](./solution.cpp)

## Problem Understanding

You are given an array of points in the X-Y plane. You need to find the minimum area of any rectangle that can be formed using four of these points as vertices. Unlike the standard "Minimum Area Rectangle" problem (LeetCode 939), the rectangle sides do NOT need to be parallel to the axes -- the rectangle can be at any angle. If no rectangle can be formed, return 0. Answers within 10^-5 of the actual answer are accepted, indicating floating-point computation is expected.

## Key Insights

1. **Diagonal characterization of rectangles**: A key property of rectangles is that their two diagonals bisect each other and have equal length. So if we group all pairs of points by their midpoint and the distance between them, any two pairs in the same group can potentially form a rectangle. This is the most efficient approach.

2. **Midpoint + diagonal length grouping**: For every pair of points `(P1, P2)`, compute the midpoint `((x1+x2)/2, (y1+y2)/2)` and the squared distance `(x1-x2)^2 + (y1-y2)^2`. Group pairs by `(midpoint, squared_distance)`. Within each group, any two pairs of points form a rectangle (since they share the same diagonal midpoint and diagonal length).

3. **Cross product for area**: Given four points forming a rectangle with vertices A, B, C, D (where A-C and B-D are the diagonals), the area can be computed as the cross product of two adjacent sides. If `(A, C)` and `(B, D)` are diagonal pairs, then vectors `AB` and `AD` are adjacent sides, and `area = |AB x AD|`.

4. **Brute force with perpendicularity check**: For each triple of points `(A, B, C)`, check if angle at B is 90 degrees using dot product: `(A-B) . (C-B) == 0`. If so, compute the fourth point `D = A + C - B` and check if D exists in the point set. If yes, compute the area as `|AB| * |CB|`.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Diagonal Grouping | Group pairs by midpoint+distance, check within groups | O(n^2) avg, O(n^2 log n) worst | O(n^2) |
| Brute Force + 4th Point | Check all triples for right angle, verify 4th point | O(n^3) | O(n) |
| Enumerate All Quads | Check all 4-point combos for rectangle | O(n^4) | O(1) |

## Step-by-Step Solution (Diagonal Grouping)

1. Create a hash map where the key is `(midpoint_x, midpoint_y, squared_distance)` and the value is a list of point pairs that share this diagonal characterization.
2. For each pair of points `(i, j)` where `i < j`:
   - Compute midpoint: `mx = points[i][0] + points[j][0]`, `my = points[i][1] + points[j][1]` (use sum to avoid floating point; divide by 2 later or use integer representation).
   - Compute squared diagonal length: `d2 = (points[i][0]-points[j][0])^2 + (points[i][1]-points[j][1])^2`.
   - Add the pair `(i, j)` to the map entry for key `(mx, my, d2)`.
3. For each group in the map with at least 2 pairs:
   - For each combination of two pairs `(A, C)` and `(B, D)`:
     - Compute the rectangle area using cross product: `area = |AB x AD|` where `AB = B - A` and `AD = D - A`.
     - Update the global minimum area.
4. Return the minimum area found, or 0 if no rectangle exists.

## Step-by-Step Solution (Brute Force + 4th Point Lookup)

1. Store all points in a hash set for O(1) lookup.
2. For each pair of points `(A, B)` -- these will be one side of the rectangle:
   - For each third point `C`:
     - Check if angle at A is 90 degrees: `dot(AB, AC) == 0` where `AB = B - A` and `AC = C - A`.
     - If yes, the fourth vertex is `D = B + C - A`.
     - Check if `D` exists in the point set.
     - If yes, compute area = `|AB| * |AC|` and update minimum.
3. Return the minimum area, or 0.

## Complexity Analysis

- **Diagonal Grouping**: Time O(n^2) to build the map. Checking within groups is O(n^2) total amortized across all groups (since the total number of pairs across all groups is O(n^2)). Space O(n^2) for the map.
- **Brute Force + 4th Point**: Time O(n^3) for iterating through all triples. Space O(n) for the hash set. Feasible for n <= 50.
- **Enumerate All Quads**: Time O(n^4), Space O(1). Only works for very small n.

## Common Mistakes

1. **Floating-point precision issues**: Using floating-point for midpoint coordinates can cause hash collisions or misses. Use integer sums `(x1+x2, y1+y2)` instead of `((x1+x2)/2.0, (y1+y2)/2.0)` as the key to avoid this entirely.
2. **Confusing with axis-aligned rectangles**: LeetCode 939 requires sides parallel to axes. This problem allows arbitrary rotations, so the approach must handle diagonal rectangles.
3. **Wrong area formula**: Using `width * height` with Euclidean distances but forgetting that for a general rectangle, the area is the magnitude of the cross product of two adjacent side vectors, not just the product of diagonal lengths.
4. **Missing degenerate cases**: Not checking that the four points actually form a valid rectangle (not a degenerate line). With the diagonal grouping approach, this is handled automatically since two different diagonals imply non-degeneracy.
5. **Hash key collision**: When using the midpoint + distance as a key, accidentally grouping collinear point pairs that share a midpoint and distance but do not form a rectangle. This cannot happen in practice because the pair itself is stored, and rectangle verification uses the actual coordinates.

## Related Problems

- **LeetCode 939 - Minimum Area Rectangle**: Axis-aligned version of this problem. Simpler because only horizontal/vertical sides need to be checked.
- **LeetCode 356 - Line Reflection**: Point-set geometry problem requiring midpoint analysis.
- **LeetCode 149 - Max Points on a Line**: Geometric grouping of points, similar hash-map-based approach.
- **LeetCode 587 - Erect the Fence**: Convex hull computation, another computational geometry problem with point sets.
