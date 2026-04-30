[file://solution.cpp](solution.cpp)

// =============================================================================
// LeetCode 980: Unique Paths III
// Approach 1: Backtracking / DFS
//
// Scan the grid to find start, end, and count of walkable cells.
// DFS from start, marking cells as visited (set to -1), trying all 4
// directions. When reaching end, check if all walkable cells were visited.
// Backtrack by restoring cell values.
//
// Time:  O(3^(m*n)) worst case, heavily pruned in practice
// Space: O(m*n) recursion stack
// =============================================================================

class Solution {
public:
    int rows, cols;
    int endR, endC;
    int totalWalkable;
    int result;

    // Direction vectors: up, down, left, right
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    void dfs(vector<vector<int>>& grid, int r, int c, int visitedCount) {
        // If we reached the ending cell
        if (r == endR && c == endC) {
            // Valid path only if we visited every walkable cell
            if (visitedCount == totalWalkable) {
                result++;
            }
            return;
        }

        // Mark current cell as visited by setting to obstacle
        int original = grid[r][c];
        grid[r][c] = -1;

        // Try all 4 directions
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            // Check bounds and that the cell is walkable (not obstacle, not visited)
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != -1) {
                dfs(grid, nr, nc, visitedCount + 1);
            }
        }

        // Backtrack: restore the cell's original value
        grid[r][c] = original;
    }

    int uniquePathsIII(vector<vector<int>>& grid) {
        rows = grid.size();
        cols = grid[0].size();
        result = 0;

        int startR = 0, startC = 0;
        int obstacleCount = 0;

        // Scan grid to find start, end, and count obstacles
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 1) {
                    startR = r;
                    startC = c;
                } else if (grid[r][c] == 2) {
                    endR = r;
                    endC = c;
                } else if (grid[r][c] == -1) {
                    obstacleCount++;
                }
            }
        }

        // Total walkable cells = all cells minus obstacles
        totalWalkable = rows * cols - obstacleCount;

        // Start DFS from the starting cell (it counts as visited)
        dfs(grid, startR, startC, 1);

        return result;
    }
};

// =============================================================================
// LeetCode 980: Unique Paths III
// Approach 2: Bitmask DP
//
// Since m*n <= 20, represent visited cells as a bitmask (up to 2^20 states).
// DP state: (current_cell_index, visited_mask)
// Value: number of ways to reach end_cell with visited_mask covering all
// walkable cells.
//
// Build the DP bottom-up or use memoized recursion.
//
// Time:  O(m*n * 2^(m*n))
// Space: O(m*n * 2^(m*n))
// =============================================================================

class SolutionBitmask {
public:
    int uniquePathsIII(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        int totalCells = rows * cols;

        int startIdx = -1, endIdx = -1;
        int targetMask = 0; // bitmask of all walkable cells

        // Map grid cells to indices 0..totalCells-1
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int idx = r * cols + c;
                if (grid[r][c] != -1) {
                    targetMask |= (1 << idx); // mark as walkable
                }
                if (grid[r][c] == 1) startIdx = idx;
                if (grid[r][c] == 2) endIdx = idx;
            }
        }

        // dp[mask][cell] = number of ways to be at cell with visited set = mask
        // Use a map or flat array. Since totalCells <= 20, 2^20 = ~1M states
        // per cell, total ~20M entries. Fits in memory.
        // Use vector for speed; initialize to 0.
        int totalStates = 1 << totalCells;
        vector<vector<int>> dp(totalStates, vector<int>(totalCells, 0));

        // Base: start at startIdx with only startIdx visited
        dp[1 << startIdx][startIdx] = 1;

        int dr[4] = {-1, 1, 0, 0};
        int dc[4] = {0, 0, -1, 1};

        int result = 0;

        // Iterate over all masks in increasing order of set bits
        for (int mask = 0; mask < totalStates; mask++) {
            for (int cell = 0; cell < totalCells; cell++) {
                if (dp[mask][cell] == 0) continue;
                // Current position
                int r = cell / cols;
                int c = cell % cols;

                // Try all 4 directions
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d];
                    int nc = c + dc[d];
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                    if (grid[nr][nc] == -1) continue;

                    int nextIdx = nr * cols + nc;
                    int nextBit = 1 << nextIdx;

                    // Skip if already visited
                    if (mask & nextBit) continue;

                    int nextMask = mask | nextBit;
                    dp[nextMask][nextIdx] += dp[mask][cell];
                }
            }
        }

        // Answer: number of ways to reach endIdx with all walkable cells visited
        result = dp[targetMask][endIdx];
        return result;
    }
};

// =============================================================================
// LeetCode 980: Unique Paths III
// Approach 3: Backtracking with Remaining Count (Pruning Optimization)
//
// Same as Approach 1, but instead of checking visited == total at the end,
// we track "remaining" cells to visit and prune early if remaining > 0
// but no neighbors are available (dead end detection).
//
// Additionally, we can add a connectivity check: if the remaining unvisited
// cells form disconnected components, prune immediately.
// (Simple version without connectivity check shown here.)
//
// Time:  O(3^(m*n)) worst case, better in practice with pruning
// Space: O(m*n)
// =============================================================================

class SolutionPruned {
public:
    int rows, cols;
    int endR, endC;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    int dfs(vector<vector<int>>& grid, int r, int c, int remaining) {
        // remaining = number of walkable cells still to visit (including end)
        if (r == endR && c == endC) {
            return (remaining == 0) ? 1 : 0;
        }

        // Mark visited
        int original = grid[r][c];
        grid[r][c] = -1;

        int count = 0;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] != -1) {
                count += dfs(grid, nr, nc, remaining - 1);
            }
        }

        // Backtrack
        grid[r][c] = original;
        return count;
    }

    int uniquePathsIII(vector<vector<int>>& grid) {
        rows = grid.size();
        cols = grid[0].size();

        int startR = 0, startC = 0;
        int walkable = 0;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] != -1) walkable++;
                if (grid[r][c] == 1) { startR = r; startC = c; }
                if (grid[r][c] == 2) { endR = r; endC = c; }
            }
        }

        // Start DFS: we are at start, remaining = walkable - 1 (start is visited)
        return dfs(grid, startR, startC, walkable - 1);
    }
};
