# LeetCode 958: Check Completeness of a Binary Tree

[View Solution](./solution.cpp)

## Problem Understanding

Given the root of a binary tree, determine whether it is a complete binary tree. A complete binary tree has every level fully filled except possibly the last level, and all nodes in the last level are packed as far left as possible. In practical terms: if you do a level-order traversal (BFS), once you encounter a null child, every subsequent node in the traversal must also be null. Any non-null node appearing after a null means there is a "gap" in the tree, which violates completeness. This is a classic BFS problem that tests your understanding of tree properties and level-order traversal.

## Key Insights

1. **BFS with a "null seen" flag is the simplest approach.** Perform a standard level-order traversal using a queue. Push both left and right children (including nulls). Once you dequeue a null, set a flag. If you ever dequeue a non-null node after the flag is set, the tree is NOT complete.

2. **The completeness property is equivalent to "no gaps in BFS order."** In a complete binary tree, when you list all nodes in BFS order (including nulls at the end), all non-null nodes appear as a contiguous prefix. The moment you see a null followed by a non-null, there is a gap.

3. **Node numbering provides an alternative approach.** In a complete binary tree with n nodes, if you assign index 1 to the root, 2i and 2i+1 to children of node i, then the maximum index should equal n. If the max index exceeds n, there are gaps. This approach uses DFS and avoids a queue.

4. **The first and last nodes on the last level determine completeness.** You can also check by doing BFS level by level. Every level except the last must be completely full. On the last level, all nodes must be contiguous from the left.

5. **Edge cases are minimal.** A single-node tree is always complete. A tree where the root has only a right child (no left) is NOT complete. These are naturally handled by the BFS flag approach.

## Approach Categories

| Approach | Strategy | Time | Space |
|----------|----------|------|-------|
| BFS with Null Flag | Level-order traversal, flag first null, check for non-null after | O(n) | O(n) |
| BFS with Index Numbering | Assign indices, check max index = node count | O(n) | O(n) |
| DFS with Index Numbering | Recursive DFS, compute max index and node count | O(n) | O(h) |
| Level-by-Level BFS | Check each level is full; last level is left-packed | O(n) | O(n) |

## Step-by-Step Approach (BFS with Null Flag)

1. **Handle edge case.** If root is null, return true (an empty tree is trivially complete).

2. **Initialize a queue** and push the root node.

3. **Initialize a boolean flag `seenNull = false`.**

4. **BFS loop:** While the queue is not empty:
   - Dequeue the front node.
   - If the node is null: set `seenNull = true` and continue to the next iteration.
   - If the node is NOT null AND `seenNull` is true: return `false` (gap detected).
   - Otherwise: push the node's left child and right child (even if they are null) into the queue.

5. **If the loop completes without returning false**, the tree is complete. Return `true`.

## Complexity Analysis

**Time Complexity: O(n)**
- Every node in the tree is visited exactly once during the BFS traversal.
- Each node is enqueued and dequeued once.
- The null sentinels add at most O(n) additional queue operations (each node contributes at most 2 null children).
- Total: O(n) where n is the number of nodes.

**Space Complexity: O(n)**
- The queue stores at most one level of the tree at a time.
- In a complete binary tree, the last level has at most ceil(n/2) nodes.
- Including null sentinels, the queue can hold up to O(n) entries.
- Total: O(n).

## Common Mistakes

1. **Not pushing null children into the queue.** The key to this approach is that you must push null children so that you can detect when a non-null child appears after a gap. If you skip nulls, you lose the ability to detect the gap.

2. **Checking only within each level instead of globally.** A common mistake is to check completeness level-by-level but forget that the null flag must persist across levels. For example, a tree where the last node on level 2 has no right child, and the first node on level 3 exists, is incomplete. The global flag catches this.

3. **Confusing "complete" with "full" or "perfect."** A full binary tree has every node with 0 or 2 children. A perfect binary tree has all leaves at the same depth. A complete binary tree is neither -- it allows the last level to be partially filled, just left-aligned. Do not apply wrong conditions.

4. **Using DFS without proper index tracking.** If you try a naive DFS approach without numbering, you might miss the completeness check because DFS does not naturally visit nodes in level order. DFS works only if you track indices and compare max index to node count.

5. **Returning early on seeing the first null without continuing the check.** Seeing a null does NOT immediately mean the tree is incomplete. You must continue the BFS and check if any NON-null node appears after the null. Only the combination of "null followed by non-null" proves incompleteness.

## Related Patterns

This problem belongs to the **BFS Level-Order Traversal** pattern for tree property verification. Related problems include:
- **LeetCode 102: Binary Tree Level Order Traversal** -- the foundational BFS template that this problem builds upon.
- **LeetCode 222: Count Complete Tree Nodes** -- counts nodes in a complete binary tree using the completeness property for O(log^2 n) efficiency.
- **LeetCode 199: Binary Tree Right Side View** -- another BFS-based tree property problem.
- **LeetCode 116: Populating Next Right Pointers in Each Node** -- level-order traversal for connecting siblings in a perfect binary tree.
- **LeetCode 662: Maximum Width of Binary Tree** -- uses index numbering (same concept as approach 2) to compute level widths.

## Code Template

```
class Solution {
public:
    bool isCompleteTree(TreeNode* root) {
        if (!root) return true;

        queue<TreeNode*> q;
        q.push(root);
        bool seenNull = false;

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            if (!node) {
                // Mark that we've encountered a gap
                seenNull = true;
            } else {
                // Non-null node after a null -> NOT complete
                if (seenNull) return false;

                // Push both children (including nulls)
                q.push(node->left);
                q.push(node->right);
            }
        }

        return true;
    }
};
```
