[file://solution.cpp](./solution.cpp)

# 909. Snakes and Ladders

## 1. Problem Understanding (What is being asked?)

You are given an `n x n` board labeled from 1 to n^2 in **Boustrophedon** (alternating left-right, right-left) order starting from the bottom-left corner. You start at square 1. Each move simulates a dice roll: you choose a destination in `[curr+1, min(curr+6, n^2)]`. If the destination has a snake or ladder (`board[r][c] != -1`), you are teleported to `board[r][c]`. You only follow one snake/ladder per move (no chaining). Return the minimum number of dice rolls to reach square n^2, or -1 if impossible.

Key observations:
- The board uses Boustrophedon numbering: row n-1 goes left-to-right, row n-2 goes right-to-left, etc.
- Snakes move you backwards (higher to lower), ladders move you forward (lower to higher), but the code treats them identically.
- You must convert between the 1-indexed square number and the 2D (row, col) coordinates.
- Each move has at most 6 choices, and you want the minimum moves -- a classic BFS setup.

## 2. Key Insights & Patterns

**Pattern: BFS on Implicit Graph (Shortest Path in Unweighted Graph)**

The problem is equivalent to finding the shortest path from node 1 to node n^2 in a graph where each node (square) has edges to at most 6 neighbors (the dice outcomes), and some edges are redirected by snakes/ladders. Since every edge has weight 1 (one dice roll), BFS gives the shortest path.

Critical insights:
1. **Coordinate conversion**: You need a helper to convert square number s (1-indexed) to (row, col). The row from the bottom is `(s-1) / n`, and the column depends on whether that row is even (left-to-right) or odd (right-to-left).
2. **Visited tracking**: Use a visited array indexed by square number (1 to n^2) to avoid revisiting.
3. **Snake/ladder handling**: When you land on a square with a snake/ladder, you jump to its destination but do NOT follow further snakes/ladders from the destination.
4. **No self-loops**: You always move forward by at least 1 square per dice roll, so the BFS is guaranteed to terminate.

## 3. Approach Discussion

### Approach A: BFS (Optimal)
- Model each square as a node. For each node, generate up to 6 successors.
- For each successor, check if there is a snake/ladder; if yes, redirect to the destination.
- Use a queue for BFS with level-based counting for the number of moves.
- Time: O(n^2), Space: O(n^2) where n is the board dimension.

### Approach B: Dijkstra (Overkill)
- Since all edges have weight 1, Dijkstra reduces to BFS but with extra overhead from the priority queue.
- Time: O(n^2 log n), unnecessary for this problem.

### Approach C: DP / DFS with Memoization
- You could try DP from square 1 to n^2, but the snake/ladder redirections can create backward edges, making a simple forward DP incorrect. You would need to handle cycles carefully, which BFS already handles naturally via visited tracking.

**Best approach: BFS (Approach A)** -- simple, optimal, and naturally handles the shortest-path requirement.

## 4. Step-by-Step Solution Walk-through

1. **Flatten the board**: Write a helper `getCoord(s, n)` that converts square number s to (row, col). The key formula:
   - `r = (s - 1) / n` gives the row from the bottom (0-indexed).
   - If `r` is even, column goes left-to-right: `c = (s - 1) % n`.
   - If `r` is odd, column goes right-to-left: `c = (n - 1) - (s - 1) % n`.
   - Actual board row (from top) is `n - 1 - r`.

2. **Initialize BFS**: Start with square 1 in the queue, mark it visited. Set moves = 0.

3. **Process level by level**: For each square `curr` in the current level:
   - For each `next` in `[curr+1, min(curr+6, n*n)]`:
     - Convert `next` to (row, col) and check `board[row][col]`.
     - If there is a snake/ladder, set `dest = board[row][col]`; otherwise `dest = next`.
     - If `dest == n*n`, return moves + 1.
     - If not visited, mark visited and add to queue.
   - Increment moves after processing the entire level.

4. **Return -1** if the queue empties without reaching n^2.

## 5. Complexity Analysis

| Aspect | Complexity |
|--------|-----------|
| Time | O(N^2) where N is the board side length; total squares = N^2, each visited at most once with up to 6 neighbors |
| Space | O(N^2) for the visited array and BFS queue |

The BFS visits each of the N^2 squares at most once and does O(1) work per square (checking at most 6 neighbors), giving O(N^2) total time.

## 6. Common Pitfalls & Edge Cases

1. **Wrong coordinate conversion**: The Boustrophedon ordering is the trickiest part. Off-by-one errors in row/column calculation are extremely common. Test with a small 2x2 board to verify.
2. **Chaining snakes/ladders**: The problem explicitly says you do NOT chain. If you land on a snake/ladder destination that itself is a snake/ladder start, you stay there.
3. **Visiting the redirected square, not the intermediate**: When square `next` has a snake/ladder to `dest`, you should mark `dest` as visited, not `next`. However, you should also be careful: you might want to mark `next` as visited too, or you could end up re-processing it. A common clean approach is to always use `dest` (which equals `next` if no snake/ladder exists).
4. **Board indexing**: The board is given with `board[0]` being the TOP row, but square 1 starts at the BOTTOM-LEFT. This is a major source of bugs.
5. **Single-element board**: n=2 minimum, so square n^2 = 4 at minimum. But test with n=2 boards.

## 7. Related Problems & Patterns

| Problem | Relationship |
|---------|-------------|
| 127. Word Ladder | BFS for shortest transformation sequence -- same pattern of level-based BFS |
| 752. Open the Lock | BFS on implicit graph with state transitions |
| 1091. Shortest Path in Binary Matrix | BFS on a grid for shortest path |
| 847. Shortest Path Visiting All Nodes | BFS with bitmask state |
| 286. Walls and Gates | Multi-source BFS on a grid |

## 8. Interview Tips & Communication Guide

**Opening statement**: "This is a shortest-path problem on an implicit graph where each square has at most 6 neighbors from a dice roll, with teleportation via snakes and ladders. Since all edges have weight 1, BFS gives the optimal solution."

**Key points to communicate**:
- Explain the Boustrophedon coordinate mapping first -- interviewers want to see you handle this detail correctly.
- Clarify the no-chaining rule for snakes/ladders.
- Mention that BFS guarantees shortest path for unweighted graphs.

**Follow-up questions to prepare for**:
- "What if you could choose to NOT take a snake/ladder?" -- This changes the graph: each square with a snake/ladder now has two outgoing options. BFS still works, but you add both the redirected and non-redirected destinations.
- "What if the board is very large (n = 10^4)?" -- The BFS is O(n^2) which would be 10^8. You might need to optimize with bidirectional BFS or A* heuristic.
- "What if dice has more than 6 faces?" -- Simply change the range from 6 to k. Complexity becomes O(n^2 * k).

**Time management**: Spend 3-4 minutes on the coordinate conversion helper, then 5-6 minutes on the BFS. The coordinate conversion is the hardest part to get right.
