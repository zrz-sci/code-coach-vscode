[file://solution.cpp](solution.cpp)

# 980. Unique Paths III

## 1. Problem Summary

Given an `m x n` grid where cells contain: `1` (start), `2` (end), `0` (empty, walkable), or `-1` (obstacle), count the number of 4-directional paths from start to end that visit every non-obstacle cell exactly once. This is a Hamiltonian path counting problem on a small grid (m*n <= 20), which makes exhaustive backtracking feasible. The constraint "walk over every non-obstacle square exactly once" means that when you reach the end cell, you must have visited all `0` cells along the way.

## 2. Key Insights / "Aha!" Moments

- **The grid is tiny (m*n <= 20).** This rules out polynomial-time algorithms (Hamiltonian path is NP-hard in general) but makes exponential approaches perfectly feasible. Backtracking with at most 2^20 states is fast enough.

- **Count non-obstacle cells to know when a path is complete.** Before starting the search, count all cells with value 0, plus the start cell and end cell. A valid path must visit exactly this many cells. Equivalently, count the zeros and track how many you have visited; when you reach the end cell, check that all zeros have been visited.

- **Standard backtracking: mark visited, recurse, unmark.** Use the grid itself as the visited marker (set cell to -1 when entering, restore when leaving). This avoids a separate visited array.

- **Bitmask DP as an alternative.** Since m*n <= 20, represent the set of visited cells as a bitmask. DP state = (current cell, visited mask). This avoids redundant recomputation but uses more memory. In practice, backtracking is simpler and fast enough for this constraint.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|----------|------|-------|-------|
| Backtracking / DFS | O(4^(m*n)) worst case, but heavily pruned | O(m*n) recursion depth | Simple to implement. Mark cell as visited, try 4 directions, backtrack. The small grid makes this fast. |
| Bitmask DP | O(m*n * 2^(m*n)) | O(m*n * 2^(m*n)) | State = (cell, visited mask). Memoize to avoid recomputation. More memory but theoretically faster for dense graphs. Overkill for this problem. |

## 4. Step-by-Step Solution (Backtracking)

1. **Scan the grid** to find:
   - The starting cell coordinates `(startR, startC)` (cell value 1).
   - The ending cell coordinates `(endR, endC)` (cell value 2).
   - The count of walkable cells `totalWalkable` (count all cells that are not -1, including start and end).

2. **Define a DFS function** `dfs(r, c, visited_count)`:
   - **Base case:** If `(r, c)` is the ending cell, return 1 if `visited_count == totalWalkable`, else return 0. (We only count paths that have visited every non-obstacle cell.)
   - **Mark current cell as visited:** Set `grid[r][c] = -1` (obstacle) to prevent revisiting.
   - **Try all 4 directions:** For each neighbor `(nr, nc)`:
     - If in bounds and `grid[nr][nc] != -1` (not obstacle and not already visited), recurse: add `dfs(nr, nc, visited_count + 1)` to the result.
   - **Backtrack:** Restore `grid[r][c]` to its original value.
   - **Return** the accumulated count.

3. **Start the DFS** from `(startR, startC)` with `visited_count = 1` (the start cell itself counts as visited).

4. **Return** the result of the DFS call.

## 5. Complexity Analysis

- **Time Complexity:** O(3^(m*n)) in practice. At each cell, we have at most 3 useful directions (we came from one direction, so 4 - 1 = 3). The actual branching is much smaller due to obstacles and visited cells, so the runtime is well within limits for m*n <= 20. The bitmask DP approach has time O(m*n * 2^(m*n)), which for 20 cells is about 20 * 10^6 -- also fine.

- **Space Complexity:** O(m*n) for the recursion stack (at most m*n frames deep). The grid modification uses O(1) extra space since we modify in place.

## 6. Common Pitfalls

- **Not counting the start cell as visited.** When initiating the DFS, the start cell is already "visited." Start with `visited_count = 1`, not 0. Forgetting this causes off-by-one in the completion check.

- **Checking visited_count at every cell instead of only at the end.** You should only check whether all cells are visited when you reach the end cell, not at intermediate cells. An intermediate cell with `visited_count == totalWalkable` but not at the end is a dead-end, not a valid path.

- **Forgetting to backtrack.** After exploring all directions from a cell, you must restore the cell's value. Without this, subsequent DFS branches from sibling calls will see the cell as an obstacle and miss valid paths.

- **Including obstacles in totalWalkable.** Only count cells with values 0, 1, and 2 as walkable. Cells with -1 are obstacles and must not be counted. A simple way: `totalWalkable = m*n - obstacleCount`.

- **Modifying the end cell's value.** When you mark a cell as visited by setting it to -1, make sure you can still recognize the end cell. One approach: check if `(r, c) == (endR, endC)` before modifying. Another: mark as -1 only after checking.

- **Not handling the case where start == end.** If the grid is 1x1 with value 1, there are no zeros and no end cell... but the constraints guarantee exactly one start and one end, so this edge case does not arise. Still, be aware.

## 7. Pattern Recognition

- **Grid Backtracking / Hamiltonian Path:** Enumerate all paths that visit every cell exactly once. This pattern appears in puzzle-solving and combinatorial search on small grids. The "mark, recurse, unmark" template is fundamental.

- **State-Space Search with Completion Check:** The DFS explores all reachable states and checks a global condition (all cells visited) at terminal states. Similar to solving mazes, N-Queens, Sudoku, and other constraint-satisfaction problems.

- **Bitmask DP on Grid:** For grids with m*n <= 20, bitmask DP is a standard technique. Each bitmask represents a subset of visited cells. This transforms backtracking into a tabular DP problem, trading space for deterministic runtime.

## 8. Related Problems

- **LeetCode 62 - Unique Paths / LeetCode 63 - Unique Paths II:** Count paths from top-left to bottom-right in a grid (with optional obstacles). Simpler DP problems without the "visit every cell" constraint.

- **LeetCode 79 - Word Search:** Backtracking on a grid with visited marking. Same "mark, recurse, unmark" pattern but with a different search goal (matching a word).

- **LeetCode 37 - Sudoku Solver:** Another backtracking problem on a grid that exhaustively tries possibilities and backtracks on constraint violations.

- **LeetCode 51 - N-Queens:** Classic backtracking problem. The "try all options, check constraints, backtrack" framework is identical.

- **LeetCode 847 - Shortest Path Visiting All Nodes:** BFS + bitmask DP to find shortest path visiting all nodes in a graph. The bitmask representation of visited nodes is the same technique applied to a different graph structure.
