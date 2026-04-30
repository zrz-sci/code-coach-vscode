/*
 * LeetCode 990: Satisfiability of Equality Equations
 *
 * Approach 1: Union-Find (Disjoint Set Union) with path compression
 * - Process equality equations first to build equivalence classes.
 * - Then check inequality equations for contradictions.
 * - If any inequality connects two variables in the same class, return false.
 *
 * Time:  O(N * alpha(26)) ~ O(N) where N = number of equations
 * Space: O(1) -- fixed 26-element arrays
 */

class Solution {
public:
    int parent[26];

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootX] = rootY;
        }
    }

    bool equationsPossible(vector<string>& equations) {
        // Initialize: each variable is its own parent
        for (int i = 0; i < 26; i++) {
            parent[i] = i;
        }

        // First pass: process all equality equations
        for (const string& eq : equations) {
            if (eq[1] == '=') { // "x==y"
                int x = eq[0] - 'a';
                int y = eq[3] - 'a';
                unite(x, y);
            }
        }

        // Second pass: check all inequality equations
        for (const string& eq : equations) {
            if (eq[1] == '!') { // "x!=y"
                int x = eq[0] - 'a';
                int y = eq[3] - 'a';
                if (find(x) == find(y)) {
                    return false; // contradiction found
                }
            }
        }

        return true; // no contradictions
    }
};


/*
 * Approach 2: Union-Find with union by rank
 * - Same two-pass strategy as above but with rank-based union
 *   to keep the tree balanced.
 * - In practice, for only 26 elements, the rank optimization
 *   has negligible impact, but it demonstrates the full DSU pattern.
 *
 * Time:  O(N * alpha(26)) ~ O(N)
 * Space: O(1)
 */

class Solution2 {
public:
    int parent[26];
    int rank_arr[26]; // using rank_arr to avoid conflict with std::rank

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return;

        // Union by rank: attach smaller tree under larger tree
        if (rank_arr[rootX] < rank_arr[rootY]) {
            parent[rootX] = rootY;
        } else if (rank_arr[rootX] > rank_arr[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank_arr[rootX]++;
        }
    }

    bool equationsPossible(vector<string>& equations) {
        for (int i = 0; i < 26; i++) {
            parent[i] = i;
            rank_arr[i] = 0;
        }

        // First pass: union all equal variables
        for (const string& eq : equations) {
            if (eq[1] == '=') {
                unite(eq[0] - 'a', eq[3] - 'a');
            }
        }

        // Second pass: verify inequalities
        for (const string& eq : equations) {
            if (eq[1] == '!') {
                if (find(eq[0] - 'a') == find(eq[3] - 'a')) {
                    return false;
                }
            }
        }

        return true;
    }
};


/*
 * Approach 3: BFS/DFS graph-based connected components
 * - Build an adjacency list from equality equations.
 * - Find connected components using BFS.
 * - Check inequality equations: if both variables are in the
 *   same connected component, return false.
 *
 * Time:  O(N + 26) ~ O(N) for building graph and finding components
 * Space: O(26 + E) where E is the number of equality edges
 */

class Solution3 {
public:
    bool equationsPossible(vector<string>& equations) {
        // Build adjacency list from equality equations
        vector<vector<int>> adj(26);

        for (const string& eq : equations) {
            if (eq[1] == '=') {
                int x = eq[0] - 'a';
                int y = eq[3] - 'a';
                adj[x].push_back(y);
                adj[y].push_back(x);
            }
        }

        // Find connected components using BFS
        vector<int> component(26, -1);
        int componentId = 0;

        for (int i = 0; i < 26; i++) {
            if (component[i] != -1) continue;

            // BFS from node i
            queue<int> q;
            q.push(i);
            component[i] = componentId;

            while (!q.empty()) {
                int node = q.front();
                q.pop();

                for (int neighbor : adj[node]) {
                    if (component[neighbor] == -1) {
                        component[neighbor] = componentId;
                        q.push(neighbor);
                    }
                }
            }

            componentId++;
        }

        // Check inequality equations
        for (const string& eq : equations) {
            if (eq[1] == '!') {
                int x = eq[0] - 'a';
                int y = eq[3] - 'a';
                if (component[x] == component[y]) {
                    return false; // contradiction
                }
            }
        }

        return true;
    }
};


/*
 * Key insights and common pitfalls:
 *
 * 1. Order matters: You MUST process all equality ('==') equations before
 *    checking any inequality ('!=') equations. If you interleave them,
 *    you might miss a transitive equality that was established later.
 *    Example: ["a!=b", "b==c", "a==c"]
 *    If you check a!=b first, a and b are in different sets -> looks OK.
 *    But after processing b==c and a==c, a and b are in the same set.
 *    The two-pass approach correctly catches this.
 *
 * 2. Self-inequality ("a!=a") is always unsatisfiable. After the first
 *    pass, find(a) == find(a) is always true, so the second pass
 *    correctly returns false.
 *
 * 3. The equation format is fixed: eq[0] is the left variable, eq[1]
 *    is '=' or '!', eq[2] is always '=', and eq[3] is the right variable.
 *    You check eq[1] to distinguish equality from inequality.
 *
 * 4. Why Union-Find over simple graph traversal? Both work for this problem.
 *    Union-Find is more concise and has near-optimal time complexity.
 *    The graph approach (BFS/DFS) requires building an adjacency list and
 *    explicitly finding connected components, which is more code but
 *    equally valid.
 *
 * 5. Path compression is important for correctness when checking:
 *    Without path compression, find(x) returns the root but traverses
 *    the full chain. With path compression, subsequent find() calls
 *    are O(1). For 26 elements this barely matters, but it is good practice.
 *
 * 6. This problem is a direct application of the mathematical concept
 *    of equivalence relations. Equality is an equivalence relation
 *    (reflexive, symmetric, transitive), and Union-Find models it perfectly.
 */
