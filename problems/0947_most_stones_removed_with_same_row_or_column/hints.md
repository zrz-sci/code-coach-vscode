# LeetCode 947 - Most Stones Removed with Same Row or Column

[View Solution](./solution.cpp)

## 1. Problem Understanding (Read & Rephrase)

You have `n` stones placed on a 2D plane at integer coordinates. A stone can be removed if it shares the same row OR the same column as at least one other stone that has not yet been removed. You want to maximize the number of stones removed.

The key insight is that this is fundamentally a graph connectivity problem. Two stones are "connected" if they share a row or column. Within a connected component of `k` stones, you can always remove `k - 1` stones (leaving exactly one stone as the "anchor"). Therefore, the answer is `n - (number of connected components)`.

**Restatement:** Build a graph where stones sharing a row or column are connected. The maximum removable stones equals `total stones - number of connected components`.

## 2. Key Observations & Insights

1. **Graph model:** Create an undirected graph where each stone is a node. Add an edge between two stones if they share the same row (`xi == xj`) or the same column (`yi == yj`). Stones in the same connected component can all be reduced to a single remaining stone.

2. **Why `k - 1` removals per component:** Within a connected component, you can always find an order of removal where each removed stone still has at least one neighbor remaining. Think of it like pruning leaves from a spanning tree -- you can remove all nodes except the root.

3. **Union-Find is ideal:** Since we only need to count connected components (not traverse them), Union-Find (Disjoint Set Union) is the most natural and efficient data structure.

4. **Row-column unification trick:** Instead of checking all O(n^2) pairs of stones, you can union stones that share a row and stones that share a column. Group stones by row index and by column index using hash maps, then union all stones in the same row group and all stones in the same column group.

5. **Alternative: Union rows and columns directly.** A clever approach is to treat each unique row index and each unique column index as a node in the Union-Find. When you see stone `(r, c)`, union the row node `r` with the column node `c` (using an offset to avoid collision, e.g., column nodes are `c + 10001`). The number of connected components among the nodes that actually appear gives the answer.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|---|---|---|---|
| Union-Find (stone nodes) | O(n * alpha(n)) | O(n) | Union stones sharing row/col via hash maps |
| Union-Find (row/col nodes) | O(n * alpha(n)) | O(n) | Union row and column indices directly |
| DFS/BFS on stone graph | O(n^2) | O(n^2) | Build adjacency list, run DFS to count components |
| DFS with row/col index maps | O(n) | O(n) | Group by row/col, DFS through shared indices |

**Recommended:** Union-Find with row/column grouping. Clean, efficient, and demonstrates strong DSU skills.

## 4. Step-by-Step Solution (Union-Find with Row/Column Grouping)

**Approach A: Union stones by shared rows and columns**

1. Initialize a Union-Find structure with `n` elements (one per stone).
2. Create two hash maps:
   - `rowMap[r]` = list of stone indices with row `r`
   - `colMap[c]` = list of stone indices with column `c`
3. For each row group: union all stones in that group together (chain them: union stone 0 with stone 1, stone 1 with stone 2, etc.).
4. For each column group: similarly union all stones together.
5. Count the number of distinct connected components using the Union-Find (count how many unique roots exist among stones 0 through n-1).
6. Answer = `n - number of components`.

**Approach B: Union row indices with column indices**

1. Initialize a Union-Find that can handle both row and column indices. Use an offset (e.g., add 10001 to column indices) to avoid collision.
2. For each stone `(r, c)`, call `union(r, c + 10001)`.
3. Count the number of distinct roots among all row and column indices that actually appear.
4. Answer = `n - number of components`.

## 5. Complexity Analysis

**Approach A (Union stones by grouping):**
- Time: O(n * alpha(n)) where alpha is the inverse Ackermann function (effectively constant).
- Space: O(n) for the Union-Find structure and hash maps.

**Approach B (Union rows and columns):**
- Time: O(n * alpha(n)).
- Space: O(n) for the Union-Find and the set of active nodes.

**DFS approach:**
- Time: O(n^2) due to pairwise edge construction.
- Space: O(n^2) for the adjacency list in the worst case.

The Union-Find approaches are superior for large `n`.

## 6. Common Mistakes & Edge Cases

- **Forgetting that sharing a row OR column is sufficient:** Both conditions create a connection. A stone at `(0, 0)` and a stone at `(0, 5)` are connected via shared row, even though their columns differ.
- **Off-by-one in the offset for approach B:** When unioning row and column indices directly, you must ensure no collision. Since `xi, yi <= 10^4`, using an offset of 10001 for columns is safe.
- **Counting components incorrectly:** Only count unique roots among the stones (or the active row/column nodes). Do not count nodes that have no stones.
- **Edge case -- single stone:** One stone, zero removals. There is one component, so answer = 1 - 1 = 0.
- **Edge case -- all stones in same row:** All `n` stones share the same row. One connected component. Answer = n - 1.
- **Edge case -- no shared rows or columns:** Every stone is in a unique row and unique column, and no two stones are connected. Each stone is its own component. Answer = 0.
- **Edge case -- stones forming a chain:** Stone A shares a row with B, B shares a column with C, C shares a row with D. All four are in one component despite no single row/column containing all of them.

## 7. Related LeetCode Problems

| Problem | Relationship |
|---|---|
| **200. Number of Islands** | Graph connectivity / connected components on a grid |
| **721. Accounts Merge** | Union-Find to merge connected groups (similar grouping pattern) |
| **684. Redundant Connection** | Union-Find to detect cycles (core DSU technique) |
| **1061. Lexicographically Smallest Equivalent String** | Union-Find for equivalence classes |
| **128. Longest Consecutive Sequence** | Hash-map-based connectivity / component reasoning |
| **305. Number of Islands II** | Dynamic connectivity with Union-Find |

## 8. Interview Tips & Follow-ups

- **State the key insight immediately:** "The answer is `n` minus the number of connected components, because within each component of size `k`, we can remove all but one stone." This shows deep understanding.
- **Justify the `k - 1` claim:** Briefly explain that in a connected component, you can remove stones one by one from the "outside in," like peeling leaves off a tree. Each removed stone still has at least one same-row or same-column neighbor.
- **Choose your Union-Find flavor:** Approach A (grouping by row/col) is more intuitive. Approach B (unioning row and column indices) is more elegant but requires explaining the offset trick. Choose based on your comfort.
- **Implement Union-Find with path compression + union by rank:** This is the standard efficient implementation. Mention that it gives nearly-O(1) amortized per operation.
- **Follow-up -- online version:** If stones are added one at a time, how does the answer change? This is a dynamic connectivity problem. Union-Find handles additions easily; deletions are much harder (link-cut trees or offline processing).
- **Follow-up -- return the removal order:** Instead of just the count, return one valid sequence of removals. This requires a BFS/DFS approach to identify a valid ordering (e.g., remove leaves of the spanning tree first).
- **Follow-up -- 3D coordinates:** If stones are in 3D and can be removed if they share any coordinate, the same approach works with three grouping dimensions instead of two.
