# LeetCode 959 - Regions Cut By Slashes

[View Solution](./solution.cpp)

## Problem Understanding

You are given an `n x n` grid of characters where each cell is one of `'/'`, `'\\'`, or `' '` (blank space). Each slash or backslash divides its 1x1 square diagonally, creating sub-regions. The task is to count the total number of contiguous regions formed by these divisions across the entire grid. A blank cell contributes no division, while `'/'` and `'\\'` each split the cell into two triangular halves. Regions can span across multiple cells as long as they are connected through open boundaries.

## Key Insights

1. **Upscaling trick**: Each 1x1 cell can be expanded into a 3x3 sub-grid. A `'/'` becomes a diagonal line of 1s from top-right to bottom-left in the 3x3 block, and `'\\'` becomes a diagonal from top-left to bottom-right. A blank cell remains all 0s. This transforms the slash-counting problem into a standard connected-components problem on a binary grid.

2. **Union-Find on triangles**: Alternatively, divide each 1x1 cell into 4 triangles (top, right, bottom, left). For a blank cell, all 4 triangles are unioned. For `'/'`, the top and left triangles merge, and the bottom and right triangles merge. For `'\\'`, the top and right merge, and the bottom and left merge. Adjacent cells share boundaries that must also be unioned (e.g., the right triangle of cell `(r,c)` connects with the left triangle of cell `(r,c+1)`).

3. **BFS/DFS on upscaled grid**: After building the 3x3 upscaled grid (size `3n x 3n`), simply count connected components of 0-valued cells using BFS or DFS, treating 1-valued cells as walls.

4. **Boundary merging in Union-Find**: The inter-cell connections are crucial. Every pair of vertically or horizontally adjacent cells shares a boundary that connects their respective triangle regions, ensuring regions that cross cell boundaries are counted as one.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| Upscale 3x3 + BFS/DFS | Expand each cell to 3x3, count components | O(n^2) | O(n^2) |
| Union-Find on 4 triangles | 4 regions per cell, merge by slash type | O(n^2 * alpha(n)) | O(n^2) |
| Upscale 3x3 + Union-Find | Expand grid, use UF for components | O(n^2 * alpha(n)) | O(n^2) |

## Step-by-Step Solution (Union-Find on 4 Triangles)

1. Create a Union-Find structure with `4 * n * n` elements (4 triangles per cell). Label them as: triangle 0 = top, 1 = right, 2 = bottom, 3 = left within each cell.
2. For each cell `(r, c)`, examine the character:
   - If `' '`: union all 4 triangles within the cell (0-1, 1-2, 2-3).
   - If `'/'`: union triangles 0 and 3 (top-left region), and union triangles 1 and 2 (bottom-right region).
   - If `'\\'`: union triangles 0 and 1 (top-right region), and union triangles 2 and 3 (bottom-left region).
3. Merge boundaries between adjacent cells:
   - For each cell `(r, c)`, if `c + 1 < n`, union triangle 1 (right) of `(r, c)` with triangle 3 (left) of `(r, c+1)`.
   - If `r + 1 < n`, union triangle 2 (bottom) of `(r, c)` with triangle 0 (top) of `(r+1, c)`.
4. Count the number of distinct roots across all `4 * n * n` elements. That is the answer.

## Step-by-Step Solution (Upscale 3x3 + BFS)

1. Create a `(3n) x (3n)` grid initialized to 0.
2. For each cell `(r, c)` in the original grid:
   - If `'/'`: set `upscaled[3r][3c+2] = upscaled[3r+1][3c+1] = upscaled[3r+2][3c] = 1`.
   - If `'\\'`: set `upscaled[3r][3c] = upscaled[3r+1][3c+1] = upscaled[3r+2][3c+2] = 1`.
3. Run BFS/DFS from every unvisited cell with value 0, counting each traversal as one region.
4. Return the total count.

## Complexity Analysis

- **Time Complexity**: O(n^2) for both approaches. The upscale approach processes a `3n x 3n` grid in O(9n^2) = O(n^2). The Union-Find approach processes `4n^2` elements with near-constant-time union operations.
- **Space Complexity**: O(n^2). The upscale approach uses O(9n^2) for the expanded grid. The Union-Find approach uses O(4n^2) for the parent/rank arrays.

## Common Mistakes

1. **Forgetting inter-cell merges**: Only handling intra-cell triangle unions but neglecting the boundary connections between adjacent cells, which leads to overcounting regions.
2. **Wrong upscale factor**: Using a 2x2 upscale instead of 3x3. A 2x2 grid cannot properly represent diagonal lines without creating ambiguous connectivity at corners.
3. **Confusing slash directions**: Mixing up which triangles `'/'` vs `'\\'` connect. Remember: `'/'` goes from bottom-left to top-right (connects top+left, bottom+right), while `'\\'` goes from top-left to bottom-right (connects top+right, bottom+left).
4. **Off-by-one in upscaled coordinates**: Incorrectly mapping original cell `(r,c)` to upscaled cell coordinates. The mapping should be `(3r + dr, 3c + dc)`.

## Related Problems

- **LeetCode 200 - Number of Islands**: Classic connected-component counting on a grid, which is the underlying technique after upscaling.
- **LeetCode 130 - Surrounded Regions**: Grid-based region identification with boundary conditions, related BFS/DFS pattern.
- **LeetCode 547 - Number of Provinces**: Union-Find for connected components, same core data structure used in the triangle approach.
- **LeetCode 990 - Satisfiability of Equality Equations**: Union-Find application with similar merge-and-count logic.
