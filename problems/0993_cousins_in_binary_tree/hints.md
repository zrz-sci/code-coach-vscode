[file://solution.cpp](solution.cpp)

# 993. Cousins in Binary Tree

## 1. Problem Summary
Given the root of a binary tree with unique values, and two distinct values `x` and `y` that exist in the tree, determine if the nodes with values `x` and `y` are **cousins**. Two nodes are cousins if they are at the same depth but have different parents. The root is at depth 0, and children of a depth-k node are at depth k+1. For example, in tree `[1,2,3,null,4,null,5]`, nodes 4 and 5 are cousins (both at depth 2, parents are 2 and 3 respectively). But in `[1,2,3,4]`, nodes 4 and 3 are not cousins (depth 2 vs depth 1).

## 2. Key Insights / "Aha!" Moments
- **Two conditions must both be true**: (1) x and y must be at the same depth, AND (2) x and y must have different parents. Missing either condition is a common bug.
- **BFS (level-order traversal) is natural**: Since we need to check if nodes are at the same depth, BFS processes nodes level by level, making depth comparison straightforward. During BFS, we also track parents.
- **DFS works equally well**: During DFS, record the depth and parent of each target node. After traversal, compare the recorded depths and parents.
- **Sibling check shortcut**: While doing BFS, if both x and y are children of the same node, they are siblings (same parent), not cousins. This can be checked as we process each node's children.
- **Early termination**: Once both x and y are found, there is no need to continue the traversal. This is an optimization, not a correctness requirement.
- **The tree has unique values**: This guarantees that x and y each appear exactly once, simplifying the search.

## 3. Approach Walkthrough (Step-by-Step)

**Approach A: BFS (Level-Order Traversal)**

1. **Initialize**: Create a queue and push the root. Initialize variables for tracking: `xFound = false`, `yFound = false`.
2. **Process level by level**: While the queue is not empty:
   - Record the current level size: `levelSize = queue.size()`.
   - Reset `xFound` and `yFound` for this level.
   - For each node in the current level:
     - **Sibling check**: Before pushing children, check if both children exist and one is x and the other is y. If so, they share the same parent -> return `false` (siblings, not cousins).
     - Push left and right children to the queue.
     - If the current node's value is x, set `xFound = true`.
     - If the current node's value is y, set `yFound = true`.
   - After processing the level: if both `xFound` and `yFound` are true, return `true` (same depth, and we already ruled out same-parent via sibling check). If only one is found, return `false` (different depths).
3. **If traversal completes**: Return `false` (should not happen given constraints).

**Approach B: DFS with depth and parent tracking**

1. **Initialize**: `xDepth = -1, yDepth = -1, xParent = null, yParent = null`.
2. **DFS from root**: At each node, check if its value matches x or y. If so, record the current depth and the parent node.
3. **After DFS**: Return `xDepth == yDepth && xParent != yParent`.

## 4. Complexity Analysis
- **BFS Approach**:
  - Time: O(N) where N is the number of nodes. Each node is visited exactly once.
  - Space: O(W) where W is the maximum width of the tree. In the worst case (complete binary tree), W = N/2 for the last level.
- **DFS Approach**:
  - Time: O(N) -- each node visited once.
  - Space: O(H) where H is the height of the tree (recursion stack). In the worst case (skewed tree), H = N.
- Both approaches are optimal in time. BFS uses more space for wide trees; DFS uses more space for deep trees.

## 5. Edge Cases to Watch
- **x and y are siblings (children of the same node)**: They have the same depth but the same parent -> NOT cousins. Return `false`.
- **x is the root**: The root has depth 0 and no parent. No other node can be its cousin (any node at depth 0 would be the root itself). Since x != y, return `false`.
- **x or y is a direct child of root**: Depth 1. The other must also be at depth 1 with a different parent. But the root has at most 2 children, and if both are x and y, they are siblings -> `false`.
- **Skewed tree (linked-list shape)**: All nodes are at different depths. x and y cannot be at the same depth unless the tree branches at some point.
- **Two-node tree**: root and one child. They have different depths -> `false`.
- **x and y at the deepest level but far apart**: They can still be cousins if they have different parents. BFS handles this naturally by checking the entire level.
- **Large tree (100 nodes)**: Both BFS and DFS handle this easily. No performance concerns.

## 6. Similar / Related Problems
- **LeetCode 102 - Binary Tree Level Order Traversal**: Core BFS pattern used in the BFS approach for this problem.
- **LeetCode 111 - Minimum Depth of Binary Tree**: BFS for depth-related tree queries.
- **LeetCode 1161 - Maximum Level Sum of a Binary Tree**: Another level-by-level BFS problem.
- **LeetCode 236 - Lowest Common Ancestor of a Binary Tree**: Related tree relationship problem where you need to understand parent-child relationships.
- **LeetCode 662 - Maximum Width of Binary Tree**: Level-order traversal with positional awareness.
- **LeetCode 2641 - Cousins in Binary Tree II**: A harder follow-up problem where you replace each node's value with the sum of its cousins' values.

## 7. Mnemonics / Memory Hooks
- **"Same floor, different family"**: Cousins must be at the same depth (same floor of the tree) but have different parents (different family branches).
- **"BFS = level detector"**: BFS naturally groups nodes by level, making it perfect for depth-based problems.
- **"Siblings are NOT cousins"**: The most common bug. Always check that parents are different, not just that depths match.
- **"Find both or fail"**: If you find x at one level and y is not at that level, immediately return false. No need to continue.

## 8. Code Template / Pseudocode
```
// BFS Approach
function isCousins(root, x, y):
    queue = [root]
    
    while queue is not empty:
        levelSize = queue.size()
        foundX = false
        foundY = false
        
        for i in range(levelSize):
            node = queue.dequeue()
            
            // Check if this node's children are x and y (siblings check)
            if node.left and node.right:
                if (node.left.val == x and node.right.val == y) OR
                   (node.left.val == y and node.right.val == x):
                    return false  // siblings, not cousins
            
            if node.left:
                queue.enqueue(node.left)
            if node.right:
                queue.enqueue(node.right)
            
            if node.val == x: foundX = true
            if node.val == y: foundY = true
        
        if foundX and foundY: return true   // same level, different parents
        if foundX or foundY: return false   // different levels
    
    return false
```

```
// DFS Approach
xDepth = -1, yDepth = -1
xParent = -1, yParent = -1

function dfs(node, parent, depth):
    if node is null: return
    
    if node.val == x:
        xDepth = depth
        xParent = parent
    if node.val == y:
        yDepth = depth
        yParent = parent
    
    dfs(node.left, node.val, depth + 1)
    dfs(node.right, node.val, depth + 1)

function isCousins(root, x, y):
    dfs(root, -1, 0)
    return xDepth == yDepth AND xParent != yParent
```
