[file://solution.cpp](solution.cpp)

# LeetCode 935 - Knight Dialer

## 1. Problem Understanding (Restate & Clarify)

We are given a standard phone dial pad (digits 0-9 arranged in a 3x4 grid with * and # as non-numeric cells) and a chess knight. The knight can be placed on any numeric cell initially. From any cell, the knight makes standard L-shaped chess moves (two squares in one direction, one square perpendicular). We need to count how many distinct phone numbers of length `n` can be dialed, where each hop is a valid knight move. Since numbers can be astronomically large, we return the answer modulo 10^9 + 7.

Key clarifications:
- The knight can start on ANY digit 0-9 for the first position.
- Digits 0-9 are positioned on the phone pad: row 0 has [1,2,3], row 1 has [4,5,6], row 2 has [7,8,9], row 3 has [*,0,#]. The knight cannot land on * or #.
- n = 1 means the knight just stands on one cell with no jumps, giving 10 possible numbers.
- For n >= 2, the knight must make exactly n-1 valid jumps.

## 2. Approach Brainstorming (At Least 2 Approaches)

**Approach A: Brute Force DFS/BFS**
For each starting digit, recursively explore all valid knight moves for n-1 steps. This produces an exponential O(3^n) or similar branching factor. Clearly too slow for n up to 5000, but establishes the baseline.

**Approach B: Dynamic Programming (Bottom-Up)**
Define dp[step][digit] = number of distinct sequences of length `step` that end at `digit`. Since a knight on digit d can only have come from a fixed set of predecessor digits, we can precompute the adjacency map and iterate from step 1 to step n. This runs in O(10 * n) time with O(10) space if we optimize to two layers.

**Approach C: Matrix Exponentiation**
Model the transitions as a 10x10 matrix and compute the n-th power using fast matrix exponentiation in O(10^3 * log n). This is the most efficient for very large n but more complex to implement.

## 3. Optimal Approach Walkthrough (Step by Step)

We choose **Approach B: Bottom-Up DP** as the best balance of simplicity and efficiency.

Step 1 -- Build the adjacency map. For each digit 0-9, precompute which digits a knight can reach:
- 0 -> {4, 6}
- 1 -> {6, 8}
- 2 -> {7, 9}
- 3 -> {4, 8}
- 4 -> {0, 3, 9}
- 5 -> {} (knight on 5 cannot reach any valid cell)
- 6 -> {0, 1, 7}
- 7 -> {2, 6}
- 8 -> {1, 3}
- 9 -> {2, 4}

Step 2 -- Initialize dp array. For step 1 (a single digit), dp[d] = 1 for all d in [0..9]. This means there is exactly one number of length 1 ending at each digit.

Step 3 -- Transition. For each step from 2 to n, compute new_dp[d] = sum of dp[prev] for all prev in adjacency[d]. Apply modulo at each addition.

Step 4 -- Final answer is sum of dp[d] for all d in [0..9] after n steps.

Step 5 -- Space optimization: we only need the current and previous step, so use two arrays of size 10 and swap them.

## 4. Implementation Notes

- The adjacency map is static and can be hardcoded as a vector<vector<int>> of size 10.
- Use `long long` for intermediate sums before taking mod to avoid overflow.
- The modulo constant is 1e9 + 7 = 1000000007.
- Note that digit 5 is a dead end -- the knight on 5 has no valid moves. For n >= 2, digit 5 contributes nothing.
- Edge case: n = 1 returns 10 immediately.
- Memory: O(10) with the rolling array optimization, or O(10 * n) without it. Both are fine for n <= 5000.

## 5. Complexity Analysis

**Time Complexity:** O(n * 10) = O(n). For each of the n steps, we iterate over 10 digits, and for each digit we sum over at most 3 predecessors (the maximum out-degree in the adjacency map). So effectively O(n * 10 * 3) = O(30n) = O(n).

**Space Complexity:** O(10) = O(1) with the rolling array optimization. We only store the DP values for the current and previous steps.

If matrix exponentiation is used instead, time becomes O(10^3 * log n) = O(1000 * log n) which is better for very large n but overkill here since n <= 5000.

## 6. Edge Cases & Pitfalls

- **n = 1:** Every digit is valid. Answer is 10. Do not perform any transitions.
- **Digit 5 is isolated:** A knight on 5 cannot move anywhere valid on the phone pad. For n >= 2, it contributes 0. This is a common pitfall -- make sure the adjacency map for 5 is empty.
- **Modular arithmetic overflow:** When summing dp values, intermediate sums can exceed 32-bit integer range. Use `long long` or apply mod after each addition.
- **Off-by-one in step count:** We dial n digits, meaning n-1 jumps. Make sure the loop runs from step 2 to step n (inclusive), not n-1.
- **Large n (n = 5000):** The DP approach handles this easily, but a naive recursive approach would TLE.

## 7. Mnemonics / Memory Hooks

**"Phone-Knight Graph"** -- Visualize the phone pad as a small graph with 10 nodes. The knight's moves define edges. DP on this graph is just counting paths of length n in a 10-node graph.

**"5 is stuck"** -- On the phone pad, digit 5 is surrounded by *, #, and other cells that are exactly one step away (not an L-shape away from any numeric cell). Remembering "5 is a dead end" prevents adjacency mistakes.

**"Rolling two rows"** -- Classic DP space optimization pattern. Only the immediately previous row matters, so swap two arrays.

## 8. Related Problems & Patterns

- **LeetCode 62 - Unique Paths:** Another grid-based DP counting problem. Same pattern of counting paths in a grid.
- **LeetCode 70 - Climbing Stairs:** Simpler version of counting paths with a transition function. Foundation for understanding DP on sequences.
- **LeetCode 688 - Knight Probability in Chessboard:** Nearly identical structure but on an NxN board with probability instead of count. Same knight-move DP pattern.
- **LeetCode 576 - Out of Boundary Paths:** DP on a grid with boundary conditions, similar transition logic.
- **Pattern: "DP on a Graph"** -- Whenever you need to count paths of length n in a small graph, define dp[step][node] and transition using adjacency lists. This is a fundamental DP pattern that appears in many problems involving state machines, graph walks, and sequence counting.
