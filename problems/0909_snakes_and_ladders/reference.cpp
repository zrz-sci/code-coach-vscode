// LeetCode 909: Snakes and Ladders
// Approach: BFS on implicit graph
// Time: O(N^2), Space: O(N^2) where N is board side length

// ============================================================
// Helper: Convert 1-indexed square number to (row, col) in the
// board matrix. The board uses Boustrophedon numbering starting
// from the bottom-left.
// ============================================================
pair<int,int> getCoord(int s, int n) {
    // r = row from the bottom (0-indexed)
    int r = (s - 1) / n;
    int c = (s - 1) % n;
    // Odd rows from bottom go right-to-left
    if (r % 2 == 1) {
        c = n - 1 - c;
    }
    // Convert to actual board row (top-indexed)
    int row = n - 1 - r;
    return {row, c};
}

// ============================================================
// Main Solution: BFS for minimum dice rolls
// ============================================================
//
// We model each square (1 to n*n) as a node in a graph. From
// each square, we can move to up to 6 successors (dice roll 1-6).
// If a successor has a snake or ladder, we are redirected to the
// destination of that snake/ladder (but do not chain further).
//
// BFS guarantees the shortest path since all edges have unit weight.
// ============================================================

class Solution {
public:
    int snakesAndLadders(vector<vector<int>>& board) {
        int n = board.size();
        int target = n * n;

        // visited[i] = true if square i has been enqueued
        vector<bool> visited(target + 1, false);
        queue<int> q;

        q.push(1);
        visited[1] = true;
        int moves = 0;

        while (!q.empty()) {
            int sz = q.size();
            // Process all nodes at the current BFS level
            for (int i = 0; i < sz; i++) {
                int curr = q.front();
                q.pop();

                // Try all dice outcomes: 1 through 6
                for (int dice = 1; dice <= 6; dice++) {
                    int next = curr + dice;
                    if (next > target) break;

                    // Convert square number to board coordinates
                    auto [row, col] = getCoord(next, n);

                    // Check for snake or ladder at this square
                    int dest = next;
                    if (board[row][col] != -1) {
                        dest = board[row][col];
                    }

                    // Check if we reached the target
                    if (dest == target) {
                        return moves + 1;
                    }

                    // Enqueue if not visited
                    if (!visited[dest]) {
                        visited[dest] = true;
                        q.push(dest);
                    }
                }
            }
            moves++;
        }

        // Target is unreachable
        return -1;
    }
};

// ============================================================
// Alternative: Flatten board into 1D array first, then BFS
// ============================================================
// Some people find it cleaner to preprocess the board into a
// 1D array flatBoard[1..n*n] where flatBoard[s] = board value
// at square s. This avoids repeated coordinate conversions.
// ============================================================

class SolutionFlatten {
public:
    int snakesAndLadders(vector<vector<int>>& board) {
        int n = board.size();
        int target = n * n;

        // Step 1: Flatten the board into a 1D array
        // flatBoard[s] = board value at square s (1-indexed)
        // -1 means no snake/ladder, otherwise it is the destination
        vector<int> flatBoard(target + 1, -1);
        int label = 1;
        bool leftToRight = true;

        for (int row = n - 1; row >= 0; row--) {
            if (leftToRight) {
                for (int col = 0; col < n; col++) {
                    flatBoard[label] = board[row][col];
                    label++;
                }
            } else {
                for (int col = n - 1; col >= 0; col--) {
                    flatBoard[label] = board[row][col];
                    label++;
                }
            }
            leftToRight = !leftToRight;
        }

        // Step 2: Standard BFS on the flattened representation
        vector<bool> visited(target + 1, false);
        queue<int> q;
        q.push(1);
        visited[1] = true;
        int moves = 0;

        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                int curr = q.front();
                q.pop();

                for (int dice = 1; dice <= 6; dice++) {
                    int next = curr + dice;
                    if (next > target) break;

                    // Follow snake/ladder if present
                    int dest = (flatBoard[next] != -1) ? flatBoard[next] : next;

                    if (dest == target) return moves + 1;
                    if (!visited[dest]) {
                        visited[dest] = true;
                        q.push(dest);
                    }
                }
            }
            moves++;
        }

        return -1;
    }
};

// ============================================================
// Walkthrough with Example 1:
// board = [[-1,-1,-1,-1,-1,-1],
//          [-1,-1,-1,-1,-1,-1],
//          [-1,-1,-1,-1,-1,-1],
//          [-1,35,-1,-1,13,-1],
//          [-1,-1,-1,-1,-1,-1],
//          [-1,15,-1,-1,-1,-1]]
//
// n = 6, target = 36
// Boustrophedon numbering (bottom to top):
// Row 5 (L->R): 1  2  3  4  5  6
// Row 4 (R->L): 12 11 10 9  8  7
// Row 3 (L->R): 13 14 15 16 17 18
// Row 2 (R->L): 24 23 22 21 20 19
// Row 1 (L->R): 25 26 27 28 29 30
// Row 0 (R->L): 36 35 34 33 32 31
//
// Snakes/Ladders:
// Square 2 -> 15 (ladder), Square 14 -> 35 (ladder)
// Square 17 -> 13 (snake)
//
// BFS:
// Level 0: {1}
// Level 1: dice 1->sq2->ladder to 15, dice 2->3, 3->4, 4->5, 5->6, 6->7
//          Queue: {15, 3, 4, 5, 6, 7}
// Level 2: From 15: 16,17->13(snake),18,19,20,21; From 3: 4(vis),5(vis)...
//          Important: 15+2=17->redirected to 13
// Level 3: From 13: 14->35(ladder)! etc.
// Level 4: From 35: 35+1=36 = target! Return 4.
// ============================================================
