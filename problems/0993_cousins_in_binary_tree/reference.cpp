/*
 * LeetCode 993: Cousins in Binary Tree
 *
 * Approach 1: BFS (Level-Order Traversal)
 * - Process nodes level by level using a queue.
 * - At each level, check if both x and y are present.
 * - Also check that x and y are not siblings (same parent).
 * - If both found at the same level with different parents, return true.
 *
 * Time:  O(N) where N = number of nodes
 * Space: O(W) where W = maximum width of the tree
 */

class Solution {
public:
    bool isCousins(TreeNode* root, int x, int y) {
        if (!root) return false;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            bool foundX = false;
            bool foundY = false;

            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();

                // Check if node's value matches x or y
                if (node->val == x) foundX = true;
                if (node->val == y) foundY = true;

                // Sibling check: if both children are x and y,
                // they share the same parent -> not cousins
                if (node->left && node->right) {
                    if ((node->left->val == x && node->right->val == y) ||
                        (node->left->val == y && node->right->val == x)) {
                        return false; // siblings, not cousins
                    }
                }

                // Enqueue children for next level
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }

            // After processing this level:
            if (foundX && foundY) return true;  // same level, different parents
            if (foundX || foundY) return false;  // only one found -> different levels
        }

        return false; // should not reach here given constraints
    }
};


/*
 * Approach 2: DFS with depth and parent tracking
 * - Traverse the entire tree, recording depth and parent for x and y.
 * - After traversal, check: same depth AND different parents.
 *
 * Time:  O(N)
 * Space: O(H) where H = height of the tree (recursion stack)
 */

class Solution2 {
public:
    int xDepth = -1, yDepth = -1;
    int xParent = -1, yParent = -1;

    void dfs(TreeNode* node, int parent, int depth, int x, int y) {
        if (!node) return;

        if (node->val == x) {
            xDepth = depth;
            xParent = parent;
        }
        if (node->val == y) {
            yDepth = depth;
            yParent = parent;
        }

        dfs(node->left, node->val, depth + 1, x, y);
        dfs(node->right, node->val, depth + 1, x, y);
    }

    bool isCousins(TreeNode* root, int x, int y) {
        xDepth = yDepth = -1;
        xParent = yParent = -1;

        dfs(root, -1, 0, x, y);

        return (xDepth == yDepth) && (xParent != yParent);
    }
};


/*
 * Approach 3: DFS with early termination
 * - Same as Approach 2 but stops as soon as both x and y are found.
 * - Uses a boolean return value to signal early termination.
 *
 * Time:  O(N) worst case, but often terminates early
 * Space: O(H)
 */

class Solution3 {
public:
    int xDepth, yDepth;
    int xParent, yParent;
    bool xFound, yFound;

    void dfs(TreeNode* node, int parent, int depth, int x, int y) {
        if (!node || (xFound && yFound)) return;

        if (node->val == x) {
            xDepth = depth;
            xParent = parent;
            xFound = true;
        }
        if (node->val == y) {
            yDepth = depth;
            yParent = parent;
            yFound = true;
        }

        // Early termination: if both found, no need to continue
        if (xFound && yFound) return;

        dfs(node->left, node->val, depth + 1, x, y);
        dfs(node->right, node->val, depth + 1, x, y);
    }

    bool isCousins(TreeNode* root, int x, int y) {
        xDepth = yDepth = -1;
        xParent = yParent = -1;
        xFound = yFound = false;

        dfs(root, -1, 0, x, y);

        return (xDepth == yDepth) && (xParent != yParent);
    }
};


/*
 * Approach 4: BFS with parent tracking using pair
 * - Each queue entry stores the node and its parent value.
 * - When x or y is found, record its parent.
 * - After each level, check the cousin conditions.
 *
 * Time:  O(N)
 * Space: O(W)
 */

class Solution4 {
public:
    bool isCousins(TreeNode* root, int x, int y) {
        if (!root) return false;

        // Queue stores (node, parent_value) pairs
        queue<pair<TreeNode*, int>> q;
        q.push({root, -1});

        while (!q.empty()) {
            int levelSize = q.size();
            int xParent = -1, yParent = -1;
            bool foundX = false, foundY = false;

            for (int i = 0; i < levelSize; i++) {
                auto [node, parentVal] = q.front();
                q.pop();

                if (node->val == x) {
                    foundX = true;
                    xParent = parentVal;
                }
                if (node->val == y) {
                    foundY = true;
                    yParent = parentVal;
                }

                if (node->left) {
                    q.push({node->left, node->val});
                }
                if (node->right) {
                    q.push({node->right, node->val});
                }
            }

            // Both found at this level
            if (foundX && foundY) {
                return xParent != yParent; // cousins iff different parents
            }
            // Only one found: different depths -> not cousins
            if (foundX || foundY) {
                return false;
            }
        }

        return false;
    }
};


/*
 * Key insights and common pitfalls:
 *
 * 1. Cousins require TWO conditions: same depth AND different parents.
 *    A very common mistake is checking only the depth condition and
 *    forgetting that siblings (same parent) are NOT cousins.
 *
 * 2. BFS sibling detection: In BFS Approach 1, we check if a node's
 *    left and right children are x and y. This is sufficient because
 *    siblings must share the same parent node. We catch this before
 *    enqueueing children, so foundX and foundY would both be true
 *    at the same level, but we return false first.
 *
 * 3. BFS Approach 4 is arguably the cleanest: by storing parent values
 *    in the queue, we naturally track parents without a separate sibling
 *    check. After finding both x and y in the same level, we simply
 *    compare their recorded parents.
 *
 * 4. DFS requires storing state: Unlike BFS where level boundaries are
 *    explicit, DFS must explicitly track depth and parent for each
 *    target node. This is straightforward with instance variables or
 *    pass-by-reference parameters.
 *
 * 5. The root cannot have a cousin: The root is at depth 0. No other
 *    node can be at depth 0. So if x or y is the root value, the
 *    answer is always false. All approaches handle this correctly:
 *    BFS finds only one target at level 0, DFS records depth 0 for
 *    one and a different depth for the other.
 *
 * 6. Unique values guarantee: The problem states all values are unique.
 *    This means we will find exactly one node with value x and exactly
 *    one with value y. No need to handle duplicate values.
 *
 * 7. Small constraints (N <= 100) mean any O(N) solution is more than
 *    sufficient. Even an O(N^2) approach would work within limits.
 *    However, writing clean O(N) solutions is good practice.
 */
