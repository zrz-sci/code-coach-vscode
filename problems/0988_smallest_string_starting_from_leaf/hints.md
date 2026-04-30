[file://solution.cpp](solution.cpp)

# 988. Smallest String Starting From Leaf

## 1. Problem Summary
Given a binary tree where each node has a value in the range [0, 25] representing letters 'a' to 'z', find the lexicographically smallest string that starts at a **leaf** node and ends at the **root**. Note that the string is built from leaf to root (bottom-up), and among all leaf-to-root paths, you must return the lexicographically smallest one. The comparison follows standard lexicographic rules: a shorter prefix is smaller than a longer string that starts with it.

## 2. Key Insights / "Aha!" Moments
- The string is constructed from **leaf to root**, not root to leaf. This means the leaf character is the first character of the string and the root character is the last. You must reverse the path or build the string carefully.
- A greedy approach of always picking the smaller child does NOT work. Consider a tree where the left subtree starts with 'a' but the full left path is "azz", while the right subtree produces "ab". The greedy choice at the root would pick left, but the right path is actually smaller.
- You cannot prune early based on the current node's character alone. Two paths might share a prefix and only diverge deeper in the tree. You must compare complete leaf-to-root strings.
- Building the string as you recurse (prepending the current character) and comparing at leaves is the cleanest approach. Alternatively, build top-down and reverse at leaves.
- Edge case: a node with only one child is NOT a leaf. You must only collect strings at actual leaf nodes (both children are null).

## 3. Approach Walkthrough (Step-by-Step)
1. **DFS with path tracking**: Start a DFS from the root, maintaining a current path string as you descend.
2. **Build the path**: At each node, prepend (or append and reverse later) the character corresponding to `node->val + 'a'` to the current path.
3. **Leaf check**: When you reach a leaf node (no left child AND no right child), the current path represents a complete leaf-to-root string. Compare it with the current best answer.
4. **Update answer**: If no answer exists yet, or the current path is lexicographically smaller, update the answer.
5. **Recurse into children**: If the node has a left child, recurse left with the extended path. Same for the right child.
6. **Backtrack**: After returning from recursion, remove the character added at this level (if using a mutable string approach).
7. **Return the result**: After DFS completes, the stored answer is the lexicographically smallest leaf-to-root string.

The key decision is how to build the string. Two common strategies:
- **Strategy A**: Build the string top-down (root first), then reverse at leaf nodes before comparison.
- **Strategy B**: Build the string bottom-up by prepending at each level. This avoids the reverse but string prepending is O(n) per operation.

Strategy A is typically more efficient since appending is O(1) amortized.

## 4. Complexity Analysis
- **Time Complexity**: O(N * L) where N is the number of nodes in the tree and L is the maximum depth (length of the longest path). At each leaf, we may need O(L) to compare or copy the string.
- **Space Complexity**: O(L) for the recursion stack and the current path string, plus O(L) for the answer string. In the worst case (skewed tree), L = N, so space is O(N).
- Note: If using Strategy B with string prepending, each prepend is O(current_length), leading to O(L^2) per path. Strategy A with append + reverse at leaf is O(L) per path.

## 5. Edge Cases to Watch
- **Single node tree**: The tree has only the root. The root is also a leaf. Return the single character corresponding to root's value.
- **Skewed tree (all left or all right)**: Only one leaf exists. Return that single leaf-to-root path.
- **Node with one child**: This node is NOT a leaf. Do not collect a string here. Only recurse into the existing child.
- **Multiple paths with same prefix**: Must compare full strings. A path "abc" is smaller than "abd" regardless of tree structure.
- **All nodes have the same value**: e.g., all zeros. Any leaf-to-root path gives the same string. Return any one of them.
- **Deep tree with many leaves**: Performance matters. Avoid unnecessary string copies; use reference parameters where possible.

## 6. Similar / Related Problems
- **LeetCode 257 - Binary Tree Paths**: Enumerate all root-to-leaf paths. This problem is similar but requires the reverse direction and finding the minimum.
- **LeetCode 112 - Path Sum**: Another DFS problem collecting information along root-to-leaf paths.
- **LeetCode 129 - Sum Root to Leaf Numbers**: Building numbers from root to leaf, conceptually similar path construction.
- **LeetCode 1022 - Sum of Root To Leaf Binary Numbers**: Similar path accumulation in a binary tree.
- **LeetCode 687 - Longest Univalue Path**: DFS on trees with path-based logic.

## 7. Mnemonics / Memory Hooks
- **"Leaf speaks first"**: Remember that the string starts at the leaf, not the root. The leaf character is the most significant character in lexicographic comparison.
- **"No shortcut in the forest"**: You cannot greedily prune paths based on intermediate nodes. You must reach every leaf and compare complete strings.
- **"One-child = not a leaf"**: A common bug is treating a node with one child as a leaf. Always check that BOTH children are null.

## 8. Code Template / Pseudocode
```
function smallestFromLeaf(root):
    answer = ""  // empty means "no answer yet"
    
    function dfs(node, currentPath):
        if node is null:
            return
        
        // Prepend current character (or append and reverse later)
        currentPath = char(node.val + 'a') + currentPath
        
        // Check if this is a leaf
        if node.left is null AND node.right is null:
            if answer is empty OR currentPath < answer:
                answer = currentPath
            return  // backtrack handled by value semantics
        
        // Recurse into children
        if node.left is not null:
            dfs(node.left, currentPath)
        if node.right is not null:
            dfs(node.right, currentPath)
    
    dfs(root, "")
    return answer
```

Alternative approach using append + reverse:
```
function dfs(node, path):
    if node is null:
        return
    
    path.append(char(node.val + 'a'))
    
    if node.left is null AND node.right is null:
        reversed_path = reverse(path)
        if answer is empty OR reversed_path < answer:
            answer = reversed_path
    else:
        dfs(node.left, path)
        dfs(node.right, path)
    
    path.pop_back()  // backtrack
```
