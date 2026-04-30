[file://solution.cpp](solution.cpp)

# LeetCode 939 - Minimum Area Rectangle

## 1. Problem Understanding (Restate & Clarify)

We are given a set of points in the X-Y plane. We need to find the minimum area of a rectangle formed from exactly four of these points, with the constraint that the rectangle's sides must be parallel to the X and Y axes. If no such rectangle exists, return 0.

Key clarifications:
- "Sides parallel to axes" means the rectangle's edges are horizontal and vertical -- no rotated rectangles.
- A valid rectangle requires four points: (x1, y1), (x1, y2), (x2, y1), (x2, y2) where x1 != x2 and y1 != y2.
- The area is |x2 - x1| * |y2 - y1|.
- All given points are unique.
- Up to 500 points, with coordinates in [0, 40000].

## 2. Approach Brainstorming (At Least 2 Approaches)

**Approach A: Brute Force (Check All Quadruples)**
Try all combinations of 4 points and check if they form an axis-aligned rectangle. This is O(n^4) which is too slow even for n = 500.

**Approach B: Check Diagonal Pairs (Hash Set)**
For every pair of points, treat them as opposite corners of a potential rectangle. Check if the other two corners exist in a hash set. This is O(n^2) with O(n) space for the set.

**Approach C: Group by X-Coordinate**
Group points by their x-coordinate. For each pair of x-values that share at least two common y-values, compute the smallest rectangle. This can be very efficient when there are few distinct x-values but may degrade in the worst case.

## 3. Optimal Approach Walkthrough (Step by Step)

We choose **Approach B: Diagonal Pairs with Hash Set** for simplicity and reliable O(n^2) performance.

Step 1 -- Store all points in a hash set for O(1) lookup. We can encode each point (x, y) as a single value or use a set of pairs.

Step 2 -- Iterate over all pairs of points (i, j). For a pair of points p1 = (x1, y1) and p2 = (x2, y2) to be diagonal corners of an axis-aligned rectangle, we need x1 != x2 AND y1 != y2.

Step 3 -- For each valid diagonal pair, check if the other two corners (x1, y2) and (x2, y1) both exist in the hash set.

Step 4 -- If they do, compute the area = |x2 - x1| * |y2 - y1| and update the minimum area found so far.

Step 5 -- After checking all pairs, return the minimum area (or 0 if no rectangle was found).

Optimization: To avoid counting each rectangle twice (once for each diagonal), we can either keep track or simply let it happen since we are taking the minimum anyway.

## 4. Implementation Notes

- **Hash set encoding:** Use `set<pair<int,int>>` or use an `unordered_set` with a custom hash. A common trick is encoding (x, y) as `x * 40001 + y` (since y <= 40000) to get a unique integer for `unordered_set<long long>`.
- **Alternatively, use `unordered_map<int, unordered_set<int>>`** mapping x -> set of y-values. Then to check if point (a, b) exists, check if `map[a].count(b)`.
- **Area tracking:** Initialize `minArea` to INT_MAX. After all pairs, if minArea is still INT_MAX, return 0.
- **Nested loop optimization:** The outer loop runs i from 0 to n-1, inner loop j from i+1 to n-1. This examines each pair once. Each rectangle is found exactly twice (once per diagonal), but since we track the minimum, this is fine.
- **No need to worry about degenerate rectangles:** The condition x1 != x2 AND y1 != y2 ensures nonzero width and height.

## 5. Complexity Analysis

**Time Complexity:** O(n^2) where n is the number of points. We iterate over all pairs of points, and for each pair, we do two O(1) hash set lookups. With n = 500, this is 500 * 499 / 2 = ~125,000 operations, very fast.

**Space Complexity:** O(n) for the hash set storing all points.

**Approach C (Group by X) complexity:** O(n^2) in the worst case as well, but can be faster when points are clustered on few x-values. However, if all points have distinct x-values, it degrades. The diagonal pair approach is more uniformly O(n^2).

## 6. Edge Cases & Pitfalls

- **No rectangle exists:** Fewer than 4 points, or no four points form an axis-aligned rectangle. Return 0.
- **All points on one line:** Whether horizontal, vertical, or neither -- no rectangle possible. Return 0.
- **Minimum area is 1:** Points like (0,0), (0,1), (1,0), (1,1) form a 1x1 rectangle.
- **Large coordinate values:** Coordinates up to 40000. Area can be up to 40000 * 40000 = 1.6 * 10^9, which fits in a 32-bit integer (max ~2.1 * 10^9). Using `long long` is safer to avoid any overflow risk.
- **Duplicate pair counting:** Each rectangle is found multiple times (once per diagonal pair). This is harmless since we track the minimum, but be aware if you need to count rectangles.
- **Hash collision:** If using the encoding x * 40001 + y, ensure the multiplier is large enough (40001 works since y <= 40000).
- **Same x or same y in a pair:** Skip pairs where x1 == x2 or y1 == y2 since they cannot be opposite corners of a rectangle.

## 7. Mnemonics / Memory Hooks

**"Diagonals define rectangles"** -- Any two opposite corners uniquely determine an axis-aligned rectangle. If you know (x1,y1) and (x2,y2), the other corners are fixed at (x1,y2) and (x2,y1).

**"Hash and check"** -- The core pattern: store all data points in a hash set, then for each candidate structure (here, a pair of diagonal points), check if the required complementary elements exist in O(1).

**"40001 encoding"** -- A useful trick for 2D point hashing when coordinates are bounded: encode (x, y) as x * (maxY + 1) + y.

## 8. Related Problems & Patterns

- **LeetCode 963 - Minimum Area Rectangle II:** The harder version where the rectangle need not be axis-aligned. Requires checking all triples and verifying perpendicularity.
- **LeetCode 356 - Line Reflection:** Another geometry problem using a hash set of points to verify symmetric counterparts.
- **LeetCode 149 - Max Points on a Line:** Geometry + hash map pattern for analyzing point relationships.
- **LeetCode 447 - Number of Boomerangs:** Enumerate pairs, use hash maps to find matching structures among points.
- **Pattern: "Hash Set Complement Check"** -- Store all elements in a set, then for each candidate pair/tuple, check if the complementary elements needed to complete a structure exist. This transforms an O(n^k) brute force into O(n^(k-1)) or better.
- **Pattern: "Encode 2D as 1D for Hashing"** -- When coordinates are bounded integers, combine (x, y) into a single integer for efficient hashing. This avoids the overhead of pair-based hash functions.
