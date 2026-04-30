[file://solution.cpp](./solution.cpp)

# 913. Cat and Mouse

## 1. Problem Understanding (What is being asked?)

A two-player game is played on an undirected graph. Mouse starts at node 1, Cat starts at node 2. There is a hole at node 0. Players alternate turns (Mouse first). On each turn, a player must move to an adjacent node. The Cat cannot enter node 0 (the hole).

The game ends when:
- **Mouse wins (return 1)**: Mouse reaches node 0 (the hole).
- **Cat wins (return 2)**: Cat occupies the same node as Mouse.
- **Draw (return 0)**: A game state repeats (same mouse position, same cat position, same player's turn).

Both players play optimally. Return 1 (mouse wins), 2 (cat wins), or 0 (draw).

Key observations:
- This is a combinatorial game theory problem. The state is (mouse_pos, cat_pos, whose_turn).
- The total number of states is bounded: O(n^2 * 2) where n is the number of nodes (up to 50), giving at most 5000 states.
- "Optimal play" means Mouse tries to reach node 0, Cat tries to catch Mouse, and both avoid losing.
- A draw occurs when neither player can force a win, resulting in an infinite cycle of moves.

## 2. Key Insights & Patterns

**Pattern: Minimax Game Theory with Backward Induction (Topological Sort from Terminal States)**

This problem is a classic example of solving a game by **backward induction** (also called "retrograde analysis"):

1. **Start from known terminal states**: States where the game outcome is determined:
   - Mouse at node 0 -> Mouse wins (result = 1).
   - Mouse and Cat at the same node -> Cat wins (result = 2).

2. **Propagate backwards**: For each determined state, look at which states can transition into it. If a state where it is Mouse's turn can lead to a Mouse-win state, Mouse will choose it (Mouse wins). If ALL successors from a Cat-turn state are Mouse-win states, then Mouse wins from that state too. Similarly for Cat.

3. **Undetermined states are draws**: Any state not resolved after full propagation is a draw.

This is essentially a **topological sort / BFS from terminal states** approach, similar to course scheduling but applied to game states.

## 3. Approach Discussion

### Approach A: BFS from Terminal States (Backward Induction) -- Optimal
- Define state as (mouse, cat, turn) where turn = 0 (mouse) or 1 (cat).
- Mark all terminal states (mouse=0 -> MOUSE_WIN; mouse=cat -> CAT_WIN).
- BFS backwards: for each resolved state, find predecessor states and update them.
- A state is resolved when:
  - It is the current player's turn and one of its successors gives a win for that player (the player will choose that move).
  - All successors give a win for the opponent (the player has no winning move).
- Any unresolved state after BFS completes is a DRAW.
- Time: O(n^3), Space: O(n^2).

### Approach B: DFS with Memoization + Cycle Detection
- Recursively explore states with memoization.
- Use a step counter or visited set to detect draws (cycles).
- The challenge is determining when a cycle means DRAW vs when further exploration might resolve it.
- Common trick: if the depth exceeds 2*n (or 4*n*n), declare a draw.
- Time: O(n^3) with memoization, but harder to implement correctly.

### Approach C: Dynamic Programming (Iterative)
- Similar to Approach A but iterate until no more states can be resolved.
- Process states in rounds until convergence.
- Time: O(n^4) in the worst case without careful ordering.

**Best approach: BFS from Terminal States (Approach A)** -- most principled, handles draws correctly, and has clean implementation.

## 4. Step-by-Step Solution Walk-through

1. **Define the state space**: `state[mouse][cat][turn]` where:
   - mouse in [0, n-1], cat in [0, n-1], turn in {MOUSE=0, CAT=1}.
   - Result: 0 = DRAW (unknown), 1 = MOUSE_WIN, 2 = CAT_WIN.

2. **Initialize terminal states**:
   - For all cat positions c (c != 0): `state[0][c][0] = state[0][c][1] = MOUSE_WIN` (mouse at hole).
   - For all positions p (p != 0): `state[p][p][0] = state[p][p][1] = CAT_WIN` (cat catches mouse).
   - Add all these to the BFS queue.

3. **Count successors for each state** (degree array):
   - For mouse-turn states (mouse=m, cat=c, turn=MOUSE): degree = number of neighbors of m.
   - For cat-turn states (mouse=m, cat=c, turn=CAT): degree = number of neighbors of c that are not node 0.

4. **BFS backward propagation**:
   - Pop a resolved state (m, c, t) with result r from the queue.
   - Find all predecessor states (states that can transition INTO this state):
     - If t = MOUSE (this is mouse's turn now, so the previous turn was cat's):
       Previous state = (m, c', CAT) for all c' adjacent to c.
     - If t = CAT: Previous state = (m', c, MOUSE) for all m' adjacent to m.
   - For each predecessor state (pm, pc, pt):
     - If already resolved, skip.
     - If the resolved child is a win for the player whose turn it is in the predecessor:
       - Resolve the predecessor as a win for that player. Add to queue.
     - Otherwise, decrement the degree of the predecessor.
       - If degree reaches 0 (all children are losses for this player), resolve as a win for the opponent. Add to queue.

5. **Return** `state[1][2][MOUSE]` -- the initial state.

## 5. Complexity Analysis

| Aspect | Complexity |
|--------|-----------|
| Time | O(n^3) -- There are O(n^2) states times 2 turns. For each state, we examine O(n) predecessors. Total: O(n^2 * n) = O(n^3) |
| Space | O(n^2) for the state array and degree array (each has 2*n^2 entries) |

With n up to 50: n^3 = 125,000 operations, very fast.

## 6. Common Pitfalls & Edge Cases

1. **Cat cannot enter node 0**: When computing successors for the cat, exclude node 0. Also exclude these edges when computing degree counts.
2. **Initial positions**: Mouse starts at node 1, Cat at node 2. The answer is `state[1][2][MOUSE_TURN]`.
3. **Self-loops in predecessors**: The graph can have complex structure. Make sure predecessor computation follows graph edges correctly.
4. **Draw detection**: States that remain unresolved after BFS are draws. Do NOT confuse this with a specific "draw" terminal state.
5. **Turn alternation**: When going backwards, if the current state has turn=MOUSE, the predecessor had turn=CAT (the cat just moved to create this state). This is a common source of confusion.
6. **Degree counting for Cat states**: Remember to subtract neighbors of cat that are node 0 when counting the degree for cat-turn states.
7. **Mouse at node 0 with cat at node 0**: This state is technically invalid (cat can't be at 0), but in the terminal state initialization, we only set mouse=0 for cat != 0.

## 7. Related Problems & Patterns

| Problem | Relationship |
|---------|-------------|
| 1728. Cat and Mouse II | Extended version with a grid and jump distances, same game theory approach |
| 464. Can I Win | Minimax game with bitmask DP |
| 486. Predict the Winner | Two-player game with optimal play |
| 294. Flip Game II | Simple two-player game theory |
| 843. Guess the Word | Interactive game with optimal strategy |
| 810. Chalkboard XOR Game | Game theory with XOR analysis |

## 8. Interview Tips & Communication Guide

**Opening statement**: "This is a two-player game theory problem that I will solve using backward induction from terminal states. The state is (mouse_position, cat_position, whose_turn), and I will use BFS to propagate known outcomes backward through the game graph."

**Key points to communicate**:
- Explain the state representation and why it fully captures the game.
- Clearly describe terminal states and the backward propagation logic.
- Emphasize that unresolved states are draws -- this is the elegant part of the algorithm.
- Discuss why this approach guarantees optimal play for both players.

**Follow-up questions to prepare for**:
- "Can you solve it with DFS + memoization instead?" -- Yes, but cycle detection makes it tricky. A common heuristic is to declare DRAW if recursion depth exceeds 2*n*n (since there are only n^2 * 2 states, exceeding this means we are in a cycle). The BFS approach is cleaner because it naturally handles draws as unresolved states.
- "What if the graph is very large (n = 10^5)?" -- The O(n^3) approach would be too slow. You would need problem-specific optimizations or approximate solutions.
- "What if there are multiple mice or cats?" -- The state space explodes exponentially with the number of players. You would need heuristic search or alpha-beta pruning.

**Time management**: This is a hard problem. Spend 5 minutes on problem understanding and approach, 15 minutes on implementation. The backward BFS with predecessor tracking is the most implementation-heavy part. Start by writing the state initialization, then the BFS loop.
