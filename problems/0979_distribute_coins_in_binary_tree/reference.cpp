[file://solution.cpp](solution.cpp)

// =============================================================================
// LeetCode 979: Distribute Coins in Binary Tree
// Approach 1: Post-order DFS (Flow Computation)
//
// For each node, compute the "excess" of its subtree:
//   excess = (total coins in subtree) - (number of nodes in subtree)
//
// The number of moves across the edge connecting a child subtree to its
// parent is |excess of child|. Sum all |excess| values across all edges.
//
// DFS returns the signed excess of the subtree. We accumulate |leftExcess|
// and |rightExcess| into a global answer at each node.
//
// Time:  O(n)
// Space: O(h), h = tree height (recursion stack)
// =============================================================================

class Solution {
public:
    int moves = 0;

    // Returns the excess coins in the subtree rooted at node.
    // Positive = subtree has extra coins to export
    // Negative = subtree needs coins imported
    int dfs(TreeNode* node) {
        if (!node) return 0;

        // Recursively compute excess for left and right subtrees
        int leftExcess = dfs(node->left);
        int rightExcess = dfs(node->right);

        // The number of moves on the edge to the left child = |leftExcess|
        // The number of moves on the edge to the right child = |rightExcess|
        moves += abs(leftExcess) + abs(rightExcess);

        // This node's excess: it has node->val coins, needs 1 for itself,
        // and must absorb/emit whatever its children need.
        return node->val - 1 + leftExcess + rightExcess;
    }

    int distributeCoins(TreeNode* root) {
        moves = 0;
        dfs(root);
        return moves;
    }
};

// =============================================================================
// LeetCode 979: Distribute Coins in Binary Tree
// Approach 2: Post-order DFS without Global Variable
//
// Same algorithm, but avoids the global/member variable by passing
// the result through a reference parameter or using a pair return value.
//
// Time:  O(n)
// Space: O(h)
// =============================================================================

class SolutionNoGlobal {
public:
    // Returns {excess, totalMoves} for the subtree rooted at node
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};

        auto [leftExcess, leftMoves] = dfs(node->left);
        auto [rightExcess, rightMoves] = dfs(node->right);

        // Moves at this node: flow through left edge + flow through right edge
        int movesHere = abs(leftExcess) + abs(rightExcess);

        // Total moves: contributions from left subtree + right subtree + this node
        int totalMoves = leftMoves + rightMoves + movesHere;

        // Excess of this subtree
        int excess = node->val - 1 + leftExcess + rightExcess;

        return {excess, totalMoves};
    }

    int distributeCoins(TreeNode* root) {
        auto [excess, totalMoves] = dfs(root);
        return totalMoves;
    }
};

// =============================================================================
// LeetCode 979: Distribute Coins in Binary Tree
// Approach 3: Iterative Post-order with Explicit Stack
//
// For environments where recursion depth is a concern (very deep trees),
// use an explicit stack to simulate post-order traversal.
//
// Each stack entry tracks: node pointer, left excess (once computed),
// right excess (once computed), and processing state.
//
// Time:  O(n)
// Space: O(n) -- explicit stack
// =============================================================================

class SolutionIterative {
public:
    int distributeCoins(TreeNode* root) {
        if (!root) return 0;

        int totalMoves = 0;

        // Stack stores pairs: (node, state)
        // state: 0 = not yet processed children
        //        1 = left child processed, need right
        //        2 = both children processed, ready to compute
        // We also need a way to store child excess values.
        // Use a separate stack for excess values (simulating return values).

        stack<pair<TreeNode*, int>> callStack;
        stack<int> excessStack; // stores computed excess for processed subtrees

        callStack.push({root, 0});

        while (!callStack.empty()) {
            auto& [node, state] = callStack.top();

            if (state == 0) {
                // Push left child if exists
                if (node->left) {
                    state = 1;
                    callStack.push({node->left, 0});
                } else {
                    excessStack.push(0); // null left child has 0 excess
                    state = 1;
                    // Fall through to state 1 check
                    if (node->right) {
                        state = 2;
                        callStack.push({node->right, 0});
                    } else {
                        excessStack.push(0); // null right child
                        state = 2;
                    }
                }
            } else if (state == 1) {
                // Left child done, push right child
                if (node->right) {
                    state = 2;
                    callStack.push({node->right, 0});
                } else {
                    excessStack.push(0);
                    state = 2;
                }
            } else {
                // state == 2: both children processed
                int rightExcess = excessStack.top(); excessStack.pop();
                int leftExcess = excessStack.top(); excessStack.pop();

                totalMoves += abs(leftExcess) + abs(rightExcess);

                int myExcess = node->val - 1 + leftExcess + rightExcess;
                excessStack.push(myExcess);

                callStack.pop();
            }
        }

        return totalMoves;
    }
};
