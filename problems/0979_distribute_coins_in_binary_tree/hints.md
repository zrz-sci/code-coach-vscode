[file://solution.cpp](solution.cpp)

# 979. Distribute Coins in Binary Tree

## 1. Problem Summary

Given a binary tree with `n` nodes where each node has some number of coins (total coins across the tree equals `n`), determine the minimum number of moves to redistribute coins so that every node has exactly one coin. A single move transfers one coin between two adjacent nodes (parent-child). The key insight is that each edge in the tree must carry a flow equal to the absolute excess or deficit of the subtree below it, and the total moves is the sum of these absolute flows.

## 2. Key Insights / "Aha!" Moments

- **Think in terms of flow through edges, not individual coin movements.** Each edge in the tree carries a net flow of coins. If a subtree has 5 nodes and 3 coins, 2 coins must flow in through the edge connecting the subtree to its parent. If it has 5 nodes and 8 coins, 3 coins must flow out. The number of moves across that edge is the absolute value of this net flow.

- **Post-order DFS computes subtree excess/deficit.** For each node, recursively compute how many excess coins its subtree has (positive means the subtree has extra coins to export, negative means it needs coins imported). The excess for a leaf with value `v` is `v - 1` (it has `v` coins but needs exactly 1).

- **The answer is the sum of |excess| across all edges.** Each edge's contribution to the total moves is `|excess of child subtree|`. The DFS naturally computes this: for each node, the excess flowing through the edge to its left child is `|leftExcess|` and to its right child is `|rightExcess|`. Accumulate both into a global answer.

- **The DFS return value propagates upward.** A node's excess = `node.val - 1 + leftExcess + rightExcess`. This is the net flow that must pass through the edge connecting this node to its parent.

## 3. Approach Categories

| Approach | Time | Space | Notes |
|----------|------|-------|-------|
| Post-order DFS (flow computation) | O(n) | O(h) | Single DFS pass. Return excess from each subtree, accumulate |excess| into answer. Optimal. |
| BFS / level-order with parent tracking | O(n) | O(n) | Process leaves first, propagate excess upward. Equivalent result but more complex implementation. |

## 4. Step-by-Step Solution (Post-order DFS)

1. **Define a helper function** `dfs(node)` that returns the "excess" of the subtree rooted at `node`. Excess = (total coins in subtree) - (number of nodes in subtree). Positive excess means coins need to flow out; negative means coins need to flow in.

2. **Base case:** If `node` is `nullptr`, return 0 (empty subtree has zero excess).

3. **Recursive case:**
   a. Compute `leftExcess = dfs(node->left)`.
   b. Compute `rightExcess = dfs(node->right)`.
   c. The number of moves across the edge to the left child is `|leftExcess|`. Add this to the global answer.
   d. The number of moves across the edge to the right child is `|rightExcess|`. Add this to the global answer.
   e. Return `node->val - 1 + leftExcess + rightExcess`. This is the excess of the current subtree: the node itself contributes `val - 1` excess (it has `val` coins but needs 1), plus whatever excess its children propagate up.

4. **Call `dfs(root)`** and return the accumulated global answer.

## 5. Complexity Analysis

- **Time Complexity:** O(n) -- Each node is visited exactly once in the DFS. The work per node is O(1) (constant-time arithmetic and a single addition to the result).

- **Space Complexity:** O(h) where h is the height of the tree -- This is the recursion stack depth. In the worst case (skewed tree), h = n, giving O(n). For a balanced tree, h = log(n).

This is optimal: any algorithm must visit every node at least once to know its coin count, so O(n) time is a lower bound.

## 6. Common Pitfalls

- **Returning |excess| instead of excess.** The DFS must return the signed excess (which can be negative), not the absolute value. The absolute value is only used when accumulating the answer (to count moves). The signed value must propagate upward so the parent knows the true net flow.

- **Forgetting `val - 1`.** Each node needs exactly 1 coin for itself. The excess contributed by the node is `val - 1`, not `val`. A node with `val = 1` has zero excess; `val = 0` has -1 (deficit); `val = 3` has +2 (surplus).

- **Accumulating at the wrong level.** The moves should be accumulated at the point where we compute left/right excess, not at the return statement. Each call to `dfs(child)` gives us the flow through the edge to that child, and `|flow|` is the number of moves on that edge.

- **Not handling single-node trees.** A tree with one node and `val = 1` requires 0 moves. The DFS handles this correctly (excess = 0, no children to process), but make sure the global counter starts at 0.

- **Confusing moves with coin value.** A move transfers exactly one coin one hop. If 5 coins must traverse an edge, that costs 5 moves, not 1. This is why we sum `|excess|` -- each unit of flow is one move.

## 7. Pattern Recognition

- **Post-order DFS with Accumulation:** Process children before the current node, combine child results, and accumulate a global answer. This pattern is the bread and butter of tree problems: diameter, height, path sums, subtree sums, etc.

- **Flow / Excess Computation on Trees:** Model the problem as flow through edges. The flow on each edge equals the surplus or deficit of the subtree below it. This reframing turns a seemingly complex redistribution problem into a single-pass DFS.

- **Absolute Value of Net Flow = Cost:** The total cost is the sum of absolute flows across all edges. This pattern appears in network flow problems and in tree problems involving redistribution or balancing.

## 8. Related Problems

- **LeetCode 543 - Diameter of Binary Tree:** Another post-order DFS where each node computes a value from its children (depth) and accumulates a global answer (diameter). The structural pattern is identical.

- **LeetCode 968 - Binary Tree Cameras:** A tree DP problem where you make decisions at each node based on children's states. Similar post-order structure with state propagation.

- **LeetCode 124 - Binary Tree Maximum Path Sum:** Post-order DFS where each node returns its best contribution upward while updating a global maximum. Same "return value vs. accumulation" duality.

- **LeetCode 1443 - Minimum Time to Collect All Apples in a Tree:** Compute the minimum number of edge traversals to collect all apples. Uses DFS with edge-cost accumulation, analogous to coin flow through edges.
