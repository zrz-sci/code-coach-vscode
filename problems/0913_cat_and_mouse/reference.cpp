// LeetCode 913: Cat and Mouse
// Approach: BFS from Terminal States (Backward Induction / Retrograde Analysis)
// Time: O(n^3), Space: O(n^2)

// ============================================================
// Game State:
//   (mouse_pos, cat_pos, turn)
//   turn: 0 = mouse's turn, 1 = cat's turn
//
// Terminal States:
//   mouse_pos == 0           -> MOUSE_WIN (mouse reached the hole)
//   mouse_pos == cat_pos     -> CAT_WIN   (cat caught mouse)
//
// Backward Induction:
//   Start from terminal states, propagate outcomes backward.
//   If a player can move to a winning state -> they win.
//   If all moves lead to opponent winning -> they lose.
//   Unresolved states after propagation -> DRAW.
// ============================================================

const int DRAW = 0;
const int MOUSE_WIN = 1;
const int CAT_WIN = 2;
const int MOUSE_TURN = 0;
const int CAT_TURN = 1;

class Solution {
public:
    int catMouseGame(vector<vector<int>>& graph) {
        int n = graph.size();

        // result[mouse][cat][turn] = game outcome from this state
        // 0 = unknown/draw, 1 = mouse wins, 2 = cat wins
        vector<vector<vector<int>>> result(n, vector<vector<int>>(n, vector<int>(2, DRAW)));

        // degree[mouse][cat][turn] = number of unresolved successor states
        // When a successor is resolved as a loss for the current player,
        // we decrement degree. When it hits 0, all moves lose -> opponent wins.
        vector<vector<vector<int>>> degree(n, vector<vector<int>>(n, vector<int>(2, 0)));

        // Initialize degrees
        for (int m = 0; m < n; m++) {
            for (int c = 0; c < n; c++) {
                // Mouse turn: mouse can move to any neighbor
                degree[m][c][MOUSE_TURN] = graph[m].size();
                // Cat turn: cat can move to any neighbor EXCEPT node 0
                int catDeg = 0;
                for (int next : graph[c]) {
                    if (next != 0) catDeg++;
                }
                degree[m][c][CAT_TURN] = catDeg;
            }
        }

        // BFS queue: stores resolved states as (mouse, cat, turn, result)
        queue<tuple<int, int, int, int>> q;

        // Initialize terminal states
        for (int c = 1; c < n; c++) {  // cat != 0 (cat can never be at hole)
            for (int t = 0; t < 2; t++) {
                // Mouse at hole -> Mouse wins
                result[0][c][t] = MOUSE_WIN;
                q.push({0, c, t, MOUSE_WIN});

                // Mouse and Cat at same position -> Cat wins
                // (c != 0 is guaranteed by loop starting at 1)
                result[c][c][t] = CAT_WIN;
                q.push({c, c, t, CAT_WIN});
            }
        }

        // BFS: propagate backwards from terminal states
        while (!q.empty()) {
            auto [m, c, t, r] = q.front();
            q.pop();

            // Find all predecessor states that could transition INTO (m, c, t)
            // If current turn is MOUSE (mouse just moved here),
            //   then predecessor had CAT turn, and mouse moved from some m' to m.
            //   Predecessor state: (m', c, CAT_TURN) for m' in graph[m]
            // If current turn is CAT (cat just moved here),
            //   then predecessor had MOUSE turn, and cat moved from some c' to c.
            //   Predecessor state: (m, c', MOUSE_TURN) for c' in graph[c]

            vector<tuple<int, int, int>> predecessors;
            if (t == MOUSE_TURN) {
                // Mouse moved: predecessor had cat turn, mouse was at some m'
                for (int prevM : graph[m]) {
                    predecessors.push_back({prevM, c, CAT_TURN});
                }
            } else {
                // Cat moved: predecessor had mouse turn, cat was at some c'
                for (int prevC : graph[c]) {
                    if (prevC == 0) continue; // Cat cannot be at node 0
                    predecessors.push_back({m, prevC, MOUSE_TURN});
                }
            }

            for (auto [pm, pc, pt] : predecessors) {
                // Skip if this predecessor is already resolved
                if (result[pm][pc][pt] != DRAW) continue;

                // Determine if this predecessor can be resolved
                // pt tells us whose turn it is at the predecessor state

                if (pt == MOUSE_TURN && r == MOUSE_WIN) {
                    // Mouse's turn at predecessor, and moving to (m,c,t) gives mouse win
                    // Mouse will choose this move -> predecessor is MOUSE_WIN
                    result[pm][pc][pt] = MOUSE_WIN;
                    q.push({pm, pc, pt, MOUSE_WIN});
                }
                else if (pt == CAT_TURN && r == CAT_WIN) {
                    // Cat's turn at predecessor, and moving to (m,c,t) gives cat win
                    // Cat will choose this move -> predecessor is CAT_WIN
                    result[pm][pc][pt] = CAT_WIN;
                    q.push({pm, pc, pt, CAT_WIN});
                }
                else {
                    // The resolved child is not favorable for the current player
                    // Decrement degree (one less option for the current player)
                    degree[pm][pc][pt]--;
                    if (degree[pm][pc][pt] == 0) {
                        // All moves lead to opponent winning
                        // If it is mouse's turn and all moves lead to cat winning:
                        //   result = CAT_WIN
                        // If it is cat's turn and all moves lead to mouse winning:
                        //   result = MOUSE_WIN
                        int loserResult = (pt == MOUSE_TURN) ? CAT_WIN : MOUSE_WIN;
                        result[pm][pc][pt] = loserResult;
                        q.push({pm, pc, pt, loserResult});
                    }
                }
            }
        }

        // Initial state: mouse at 1, cat at 2, mouse's turn
        return result[1][2][MOUSE_TURN];
    }
};

// ============================================================
// Alternative: DFS with Memoization + Depth Limit for Draws
// ============================================================
// This approach is simpler to write but requires a heuristic
// depth limit to detect draws. The idea: if we exceed 2*n*n
// recursive calls without reaching a terminal state, we must
// be in a cycle, which is a draw.
// ============================================================

class SolutionDFS {
public:
    int n;
    vector<vector<int>>* graphPtr;
    // memo[mouse][cat][turn] = result, -1 if not computed
    vector<vector<vector<int>>> memo;

    int catMouseGame(vector<vector<int>>& graph) {
        n = graph.size();
        graphPtr = &graph;
        memo.assign(n, vector<vector<int>>(n, vector<int>(2, -1)));
        return solve(1, 2, 0, 0);
    }

    // depth tracks recursion depth for draw detection
    int solve(int mouse, int cat, int turn, int depth) {
        // Terminal states
        if (mouse == 0) return MOUSE_WIN;
        if (mouse == cat) return CAT_WIN;
        // Draw detection: if depth exceeds total states, it is a cycle
        if (depth >= 2 * n * n) return DRAW;

        if (memo[mouse][cat][turn] != -1) {
            return memo[mouse][cat][turn];
        }

        // Temporarily mark as DRAW to handle cycles in recursion
        memo[mouse][cat][turn] = DRAW;

        if (turn == MOUSE_TURN) {
            // Mouse wants MOUSE_WIN, tries to avoid CAT_WIN
            bool canDraw = false;
            for (int next : (*graphPtr)[mouse]) {
                int res = solve(next, cat, CAT_TURN, depth + 1);
                if (res == MOUSE_WIN) {
                    memo[mouse][cat][turn] = MOUSE_WIN;
                    return MOUSE_WIN;
                }
                if (res == DRAW) canDraw = true;
            }
            // If no winning move, settle for draw if possible
            memo[mouse][cat][turn] = canDraw ? DRAW : CAT_WIN;
        } else {
            // Cat wants CAT_WIN, tries to avoid MOUSE_WIN
            bool canDraw = false;
            for (int next : (*graphPtr)[cat]) {
                if (next == 0) continue; // Cat cannot go to hole
                int res = solve(mouse, next, MOUSE_TURN, depth + 1);
                if (res == CAT_WIN) {
                    memo[mouse][cat][turn] = CAT_WIN;
                    return CAT_WIN;
                }
                if (res == DRAW) canDraw = true;
            }
            memo[mouse][cat][turn] = canDraw ? DRAW : MOUSE_WIN;
        }

        return memo[mouse][cat][turn];
    }
};

// ============================================================
// Walkthrough with Example 2: graph = [[1,3],[0],[3],[0,2]]
// Node 0: hole, connected to 1, 3
// Node 1: mouse start, connected to 0
// Node 2: cat start, connected to 3
// Node 3: connected to 0, 2
//
// Initial state: mouse=1, cat=2, mouse's turn
//
// Terminal states:
//   (0, c, t) = MOUSE_WIN for c in {1,2,3}, t in {0,1}
//   (p, p, t) = CAT_WIN for p in {1,2,3}, t in {0,1}
//
// BFS propagation:
// From (0,2,0)=MW: predecessor (cat turn, mouse was at m' in graph[0]={1,3})
//   -> (1,2,1): cat's turn. Result=MW. Cat doesn't want this.
//      degree[1][2][1]-- (cat has graph[2]={3}, minus node0 -> deg=1, now 0)
//      All moves lose for cat -> result[1][2][1] = MOUSE_WIN
//   -> (3,2,1): cat's turn. Result=MW.
//      degree[3][2][1]-- -> check if 0
//
// From (0,2,1)=MW: predecessor (mouse turn, cat was at c' in graph[2]={3})
//   -> (0,3,0): already MW, skip.
//
// Eventually: result[1][2][0] = MOUSE_WIN = 1
// Answer: 1 (mouse wins)
// ============================================================
