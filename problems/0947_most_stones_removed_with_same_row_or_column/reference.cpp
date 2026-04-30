/*
 * LeetCode 947 - Most Stones Removed with Same Row or Column
 *
 * Approach: Union-Find (Disjoint Set Union)
 *
 * Key Idea:
 *   Two stones are connected if they share a row or column. Within a connected
 *   component of k stones, we can remove k-1 stones. So the answer is:
 *       total_stones - number_of_connected_components
 *
 * Time Complexity:  O(n * alpha(n)) ~ O(n) amortized
 * Space Complexity: O(n)
 *
 * Reference: ./solution.cpp
 */

// ============================================================
// Solution 1: Union-Find with Row/Column Grouping
// ============================================================
//
// Group stones by their row index and column index using hash maps.
// Union all stones that share the same row. Union all stones that
// share the same column. Count connected components.
class Solution {
public:
    // Union-Find data structure with path compression and union by rank
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        // Union by rank
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) ++rank_[px];
    }

    int removeStones(vector<vector<int>>& stones) {
        int n = stones.size();
        parent.resize(n);
        rank_.resize(n, 0);

        // Initialize: each stone is its own parent
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }

        // Group stones by row and by column
        unordered_map<int, vector<int>> rowMap, colMap;
        for (int i = 0; i < n; ++i) {
            rowMap[stones[i][0]].push_back(i);
            colMap[stones[i][1]].push_back(i);
        }

        // Union all stones in the same row
        for (auto& [row, indices] : rowMap) {
            for (int i = 1; i < (int)indices.size(); ++i) {
                unite(indices[0], indices[i]);
            }
        }

        // Union all stones in the same column
        for (auto& [col, indices] : colMap) {
            for (int i = 1; i < (int)indices.size(); ++i) {
                unite(indices[0], indices[i]);
            }
        }

        // Count connected components
        unordered_set<int> components;
        for (int i = 0; i < n; ++i) {
            components.insert(find(i));
        }

        // Answer = total stones - number of components
        return n - (int)components.size();
    }
};

// ============================================================
// Solution 2: Union-Find on Row/Column Indices (Elegant)
// ============================================================
//
// Instead of unioning stone indices, union row and column values
// directly. Use an offset for columns to avoid collision with rows.
// For stone (r, c), union(r, c + OFFSET).
// Count unique components among all active row/column nodes.
class Solution2 {
public:
    unordered_map<int, int> parent, rankMap;

    int find(int x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rankMap[x] = 0;
        }
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rankMap[px] < rankMap[py]) swap(px, py);
        parent[py] = px;
        if (rankMap[px] == rankMap[py]) ++rankMap[px];
    }

    int removeStones(vector<vector<int>>& stones) {
        int n = stones.size();
        int OFFSET = 10001; // since coordinates are in [0, 10^4]

        for (auto& stone : stones) {
            // Union the row index with the (offset) column index
            unite(stone[0], stone[1] + OFFSET);
        }

        // Count unique components among all nodes that appeared
        unordered_set<int> components;
        for (auto& stone : stones) {
            components.insert(find(stone[0]));
            // Note: find(stone[1] + OFFSET) would give the same root
        }

        return n - (int)components.size();
    }
};

// ============================================================
// Solution 3: DFS-Based Approach
// ============================================================
//
// Build a graph where stones sharing a row or column are connected.
// Run DFS to count connected components.
// Simpler to understand but O(n^2) edge construction.
class Solution3 {
public:
    int removeStones(vector<vector<int>>& stones) {
        int n = stones.size();

        // Build adjacency list using row/column grouping
        unordered_map<int, vector<int>> rowMap, colMap;
        for (int i = 0; i < n; ++i) {
            rowMap[stones[i][0]].push_back(i);
            colMap[stones[i][1]].push_back(i);
        }

        // DFS to count connected components
        vector<bool> visited(n, false);
        int components = 0;

        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            ++components;
            // DFS from stone i
            vector<int> stack = {i};
            while (!stack.empty()) {
                int cur = stack.back();
                stack.pop_back();
                if (visited[cur]) continue;
                visited[cur] = true;

                // Visit all stones in the same row
                for (int neighbor : rowMap[stones[cur][0]]) {
                    if (!visited[neighbor]) {
                        stack.push_back(neighbor);
                    }
                }
                // Visit all stones in the same column
                for (int neighbor : colMap[stones[cur][1]]) {
                    if (!visited[neighbor]) {
                        stack.push_back(neighbor);
                    }
                }
            }
        }

        return n - components;
    }
};

// ============================================================
// Detailed Walkthrough with Example
// ============================================================
//
// stones = [[0,0],[0,1],[1,0],[1,2],[2,1],[2,2]]
//
// Row groups:
//   row 0: stones {0, 1}    -> union(0, 1)
//   row 1: stones {2, 3}    -> union(2, 3)
//   row 2: stones {4, 5}    -> union(4, 5)
//
// Column groups:
//   col 0: stones {0, 2}    -> union(0, 2)
//   col 1: stones {1, 4}    -> union(1, 4)
//   col 2: stones {3, 5}    -> union(3, 5)
//
// After all unions:
//   Stone 0 -- Stone 1 (row 0)
//   Stone 0 -- Stone 2 (col 0)
//   Stone 2 -- Stone 3 (row 1)
//   Stone 1 -- Stone 4 (col 1)
//   Stone 4 -- Stone 5 (row 2)
//   Stone 3 -- Stone 5 (col 2)
//
// All 6 stones form ONE connected component.
// Answer = 6 - 1 = 5
//
// ============================================================
// Why k-1 Removals Per Component?
// ============================================================
//
// Consider a connected component as a tree (take any spanning tree).
// A leaf node in this tree shares a row or column with its parent,
// and since the parent is still present, the leaf can be removed.
// After removing all leaves, the tree shrinks. Repeat until only
// the root remains. This yields exactly (k - 1) removals for k nodes.
//
// Formally, for a connected graph on k nodes, any spanning tree
// has k-1 edges. We can remove nodes in reverse BFS order (leaves
// first), and each removal is valid because the parent remains.
