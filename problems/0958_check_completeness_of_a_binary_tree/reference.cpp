// LeetCode 958: Check Completeness of a Binary Tree
// Difficulty: Medium
// Tags: Tree, Breadth-First Search, Binary Tree
// Link: https://leetcode.com/problems/check-completeness-of-a-binary-tree/
//
// Approach: BFS level-order traversal with null gap detection.
// In a complete tree, BFS encounters all non-null nodes before
// any null. If a non-null appears after a null, the tree is incomplete.
//
// Time:  O(n) -- visit each node once
// Space: O(n) -- queue holds at most one level (~n/2 nodes)

// ============================================================
// Solution 1: BFS with Null Flag (Cleanest Approach)
// ============================================================
// Core idea: Push both children of every node into the queue,
// even if they are null. Once we dequeue a null, set a flag.
// If we dequeue a non-null node after the flag is set, there is
// a gap -> tree is NOT complete.

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

            if (node == nullptr) {
                // We've reached a null position in the BFS order
                // From now on, every subsequent node must also be null
                seenNull = true;
            } else {
                if (seenNull) {
                    // A real node appeared after a null gap
                    // This violates the completeness property
                    return false;
                }

                // Enqueue both children, even if null
                // This is critical: pushing nulls lets us detect gaps
                q.push(node->left);
                q.push(node->right);
            }
        }

        // No gaps detected -> tree is complete
        return true;
    }
};


// ============================================================
// Solution 2: BFS with Index Numbering
// ============================================================
// Assign index 1 to root. For node at index i, left child = 2i,
// right child = 2i+1. In a complete tree with n nodes, the
// maximum index should equal n. If max_index > n, there are gaps.
//
// This approach is also used in LC 662 (Maximum Width of Binary Tree).

class SolutionIndexing {
public:
    bool isCompleteTree(TreeNode* root) {
        if (!root) return true;

        // BFS with (node, index) pairs
        queue<pair<TreeNode*, int>> q;
        q.push({root, 1});

        int nodeCount = 0;
        int maxIndex = 0;

        while (!q.empty()) {
            auto [node, idx] = q.front();
            q.pop();

            nodeCount++;
            maxIndex = max(maxIndex, idx);

            if (node->left) {
                q.push({node->left, 2 * idx});
            }
            if (node->right) {
                q.push({node->right, 2 * idx + 1});
            }
        }

        // In a complete tree, max index equals total node count
        // because nodes fill positions 1, 2, 3, ..., n contiguously
        return maxIndex == nodeCount;
    }
};


// ============================================================
// Solution 3: DFS with Index Numbering
// ============================================================
// Same index logic as Solution 2, but using recursive DFS.
// Count total nodes and find max index. Compare at the end.
// Uses O(h) stack space instead of O(n) queue space.

class SolutionDFS {
public:
    bool isCompleteTree(TreeNode* root) {
        int nodeCount = countNodes(root);
        // Check if max index equals node count
        return checkComplete(root, 1, nodeCount);
    }

private:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    // Returns true if all nodes have index <= nodeCount
    // A node at index > nodeCount means there's a gap
    bool checkComplete(TreeNode* node, int index, int nodeCount) {
        if (!node) return true;

        // If this node's index exceeds the total count,
        // the tree has a gap (some earlier position is missing)
        if (index > nodeCount) return false;

        // Recursively check both subtrees
        return checkComplete(node->left, 2 * index, nodeCount) &&
               checkComplete(node->right, 2 * index + 1, nodeCount);
    }
};


// ============================================================
// Solution 4: Level-by-Level BFS
// ============================================================
// Explicit level-by-level traversal. Check that:
// 1. Every level except the last is completely full
// 2. On the last level, all nodes are contiguous from the left
// More verbose but shows the completeness definition directly.

class SolutionLevelByLevel {
public:
    bool isCompleteTree(TreeNode* root) {
        if (!root) return true;

        queue<TreeNode*> q;
        q.push(root);
        bool lastLevelReached = false;

        while (!q.empty()) {
            int levelSize = q.size();
            bool nullFoundThisLevel = false;

            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();

                // Check left child
                if (node->left) {
                    // Non-null child after a null was seen -> not complete
                    if (nullFoundThisLevel || lastLevelReached) {
                        return false;
                    }
                    q.push(node->left);
                } else {
                    nullFoundThisLevel = true;
                }

                // Check right child
                if (node->right) {
                    if (nullFoundThisLevel || lastLevelReached) {
                        return false;
                    }
                    q.push(node->right);
                } else {
                    nullFoundThisLevel = true;
                }
            }

            // If any null was found on this level, the next level
            // should not have any nodes (this was the last level)
            if (nullFoundThisLevel) {
                lastLevelReached = true;
            }
        }

        return true;
    }
};


// ============================================================
// Walkthrough with Example 1: root = [1, 2, 3, 4, 5, 6]
// ============================================================
//
// Tree structure:
//         1
//        / \
//       2   3
//      / \ /
//     4  5 6
//
// BFS (Solution 1):
//   Queue: [1]
//   Dequeue 1 (non-null, seenNull=false) -> push 2, 3
//   Queue: [2, 3]
//
//   Dequeue 2 (non-null, seenNull=false) -> push 4, 5
//   Queue: [3, 4, 5]
//
//   Dequeue 3 (non-null, seenNull=false) -> push 6, null
//   Queue: [4, 5, 6, null]
//
//   Dequeue 4 (non-null, seenNull=false) -> push null, null
//   Queue: [5, 6, null, null, null]
//
//   Dequeue 5 (non-null, seenNull=false) -> push null, null
//   Queue: [6, null, null, null, null, null]
//
//   Dequeue 6 (non-null, seenNull=false) -> push null, null
//   Queue: [null, null, null, null, null, null, null]
//
//   Dequeue null -> seenNull = true
//   Dequeue null -> seenNull already true, still null, continue
//   ... all remaining are null
//
//   Result: true. Tree IS complete.
//
//
// Walkthrough with Example 2: root = [1, 2, 3, 4, 5, null, 7]
// ============================================================
//
// Tree structure:
//         1
//        / \
//       2   3
//      / \   \
//     4  5    7
//
// BFS (Solution 1):
//   Dequeue 1 -> push 2, 3
//   Dequeue 2 -> push 4, 5
//   Dequeue 3 -> push null, 7    <-- null left child of 3!
//   Dequeue 4 -> push null, null
//   Dequeue 5 -> push null, null
//   Dequeue null -> seenNull = true
//   Dequeue 7 -> seenNull is true, but node is non-null -> RETURN FALSE
//
//   Result: false. Tree is NOT complete (node 7 has a gap before it).
//
//
// Index Approach Verification (Solution 2):
// Example 2: nodes at indices 1, 2, 3, 4, 5, 7
//   nodeCount = 6, maxIndex = 7
//   7 != 6 -> NOT complete. Correct!
