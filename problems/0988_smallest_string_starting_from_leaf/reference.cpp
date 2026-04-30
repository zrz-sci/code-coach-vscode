/*
 * LeetCode 988: Smallest String Starting From Leaf
 *
 * Approach 1: DFS with string prepending
 * - At each node, prepend the character to the current path string.
 * - At leaf nodes, compare the complete path with the current best answer.
 * - Prepending ensures the string is already in leaf-to-root order.
 *
 * Time:  O(N * L) where N = number of nodes, L = max depth
 * Space: O(L) for recursion stack and path string
 */

class Solution {
public:
    string answer;

    void dfs(TreeNode* node, string current) {
        if (!node) return;

        // Prepend the current node's character to the path
        current = string(1, 'a' + node->val) + current;

        // If this is a leaf node, compare with current best
        if (!node->left && !node->right) {
            if (answer.empty() || current < answer) {
                answer = current;
            }
            return;
        }

        // Recurse into children (only if they exist)
        if (node->left) {
            dfs(node->left, current);
        }
        if (node->right) {
            dfs(node->right, current);
        }
    }

    string smallestFromLeaf(TreeNode* root) {
        answer = "";
        dfs(root, "");
        return answer;
    }
};


/*
 * Approach 2: DFS with append + reverse at leaf
 * - More efficient than prepending since string append is O(1) amortized.
 * - At each leaf, reverse the accumulated path and compare.
 * - Uses backtracking to restore the path after each recursive call.
 *
 * Time:  O(N * L) where N = number of nodes, L = max depth
 * Space: O(L) for recursion stack and path string
 */

class Solution2 {
public:
    string answer;

    void dfs(TreeNode* node, string& path) {
        if (!node) return;

        // Append the current node's character to the path
        path.push_back('a' + node->val);

        // If this is a leaf node, reverse and compare
        if (!node->left && !node->right) {
            // Create the leaf-to-root string by reversing
            string leafToRoot(path.rbegin(), path.rend());
            if (answer.empty() || leafToRoot < answer) {
                answer = leafToRoot;
            }
        } else {
            // Recurse into existing children
            if (node->left) {
                dfs(node->left, path);
            }
            if (node->right) {
                dfs(node->right, path);
            }
        }

        // Backtrack: remove the character added at this level
        path.pop_back();
    }

    string smallestFromLeaf(TreeNode* root) {
        answer = "";
        string path;
        dfs(root, path);
        return answer;
    }
};


/*
 * Approach 3: Iterative DFS using a stack
 * - Uses an explicit stack to avoid recursion.
 * - Each stack entry stores the node and the accumulated path so far.
 * - At leaf nodes, constructs the leaf-to-root string and compares.
 *
 * Time:  O(N * L) where N = number of nodes, L = max depth
 * Space: O(N * L) in the worst case for storing paths on the stack
 *
 * This approach uses more memory than recursive DFS because each stack
 * entry carries its own copy of the path string. However, it avoids
 * stack overflow for extremely deep trees.
 */

class Solution3 {
public:
    string smallestFromLeaf(TreeNode* root) {
        if (!root) return "";

        string answer;
        // Stack stores pairs of (node, path_from_root_so_far)
        stack<pair<TreeNode*, string>> stk;
        stk.push({root, string(1, 'a' + root->val)});

        while (!stk.empty()) {
            auto [node, path] = stk.top();
            stk.pop();

            // Check if this is a leaf
            if (!node->left && !node->right) {
                // Reverse path to get leaf-to-root order
                string leafToRoot(path.rbegin(), path.rend());
                if (answer.empty() || leafToRoot < answer) {
                    answer = leafToRoot;
                }
                continue;
            }

            // Push children onto the stack with extended paths
            if (node->right) {
                stk.push({node->right, path + string(1, 'a' + node->right->val)});
            }
            if (node->left) {
                stk.push({node->left, path + string(1, 'a' + node->left->val)});
            }
        }

        return answer;
    }
};


/*
 * Key insights and common pitfalls:
 *
 * 1. String direction: The problem asks for leaf-to-root, not root-to-leaf.
 *    The leaf character is the FIRST character, and root is the LAST.
 *
 * 2. Greedy pruning does NOT work: You cannot stop exploring a subtree just
 *    because its root has a larger character. The deeper nodes might produce
 *    a shorter or lexicographically smaller overall string.
 *    Example: root='b', left child is leaf 'a' -> path "ab"
 *             root='b', right subtree produces "a" -> path "ab" too
 *    But consider: root='z', left='a' (leaf) -> "az"
 *                  root='z', right path -> "ab" which is smaller
 *
 * 3. One-child nodes are NOT leaves: A node with only a left child or only
 *    a right child must NOT be treated as a leaf. You must recurse into
 *    the existing child. A common bug is checking "if !left, collect string"
 *    which incorrectly treats one-child nodes as leaves.
 *
 * 4. String comparison: C++ string comparison using < operator performs
 *    lexicographic comparison by default, which is exactly what we need.
 *    "ab" < "b" is true. "ab" < "abc" is true. This matches the problem
 *    definition where shorter prefixes are considered smaller.
 *
 * 5. Empty tree: If root is null, return "". This edge case should be
 *    handled at the entry point of the function.
 */
