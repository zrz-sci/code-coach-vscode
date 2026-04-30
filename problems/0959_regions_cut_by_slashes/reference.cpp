// LeetCode 959: Regions Cut By Slashes
// ./solution.cpp

// ============================================================
// Approach 1: Union-Find on 4 Triangles per Cell
// ============================================================
// Each 1x1 cell is divided into 4 triangles:
//   0 = top, 1 = right, 2 = bottom, 3 = left
// Depending on the character, we union internal triangles,
// and always union adjacent-cell boundary triangles.
//
// Time:  O(n^2 * alpha(n^2)) ~ O(n^2)
// Space: O(n^2)

class UnionFind {
    vector<int> parent, rank_;
public:
    int components;

    UnionFind(int n) : parent(n), rank_(n, 0), components(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path compression (halving)
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
        components--;
    }
};

class Solution {
public:
    int regionsBySlashes(vector<string>& grid) {
        int n = grid.size();
        // 4 triangles per cell: cell (r,c) -> indices [4*(r*n+c) + t] for t in {0,1,2,3}
        UnionFind uf(4 * n * n);

        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                int base = 4 * (r * n + c);
                char ch = grid[r][c];

                // Internal unions based on character
                if (ch == ' ') {
                    // All 4 triangles form a single region
                    uf.unite(base + 0, base + 1);
                    uf.unite(base + 1, base + 2);
                    uf.unite(base + 2, base + 3);
                } else if (ch == '/') {
                    // '/' divides into top-left and bottom-right
                    uf.unite(base + 0, base + 3); // top + left
                    uf.unite(base + 1, base + 2); // right + bottom
                } else { // '\\'
                    // '\\' divides into top-right and bottom-left
                    uf.unite(base + 0, base + 1); // top + right
                    uf.unite(base + 2, base + 3); // bottom + left
                }

                // External unions: connect to right neighbor
                if (c + 1 < n) {
                    int rightBase = 4 * (r * n + (c + 1));
                    uf.unite(base + 1, rightBase + 3); // my right <-> neighbor's left
                }
                // External unions: connect to bottom neighbor
                if (r + 1 < n) {
                    int bottomBase = 4 * ((r + 1) * n + c);
                    uf.unite(base + 2, bottomBase + 0); // my bottom <-> neighbor's top
                }
            }
        }

        return uf.components;
    }
};


// ============================================================
// Approach 2: Upscale to 3x3 Grid + BFS
// ============================================================
// Expand each cell into a 3x3 block. Draw slashes as walls (1s).
// Count connected components of 0s using BFS.
//
// Time:  O(n^2)
// Space: O(n^2)

class Solution2 {
public:
    int regionsBySlashes(vector<string>& grid) {
        int n = grid.size();
        int size = 3 * n;
        vector<vector<int>> expanded(size, vector<int>(size, 0));

        // Build upscaled grid
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                int br = 3 * r, bc = 3 * c;
                if (grid[r][c] == '/') {
                    expanded[br][bc + 2]     = 1;
                    expanded[br + 1][bc + 1] = 1;
                    expanded[br + 2][bc]     = 1;
                } else if (grid[r][c] == '\\') {
                    expanded[br][bc]         = 1;
                    expanded[br + 1][bc + 1] = 1;
                    expanded[br + 2][bc + 2] = 1;
                }
                // blank space: all remain 0
            }
        }

        // BFS to count connected components of 0s
        int regions = 0;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (expanded[i][j] == 0) {
                    regions++;
                    // BFS flood fill
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    expanded[i][j] = 1; // mark visited

                    while (!q.empty()) {
                        auto [cr, cc] = q.front();
                        q.pop();
                        for (auto& d : dirs) {
                            int nr = cr + d[0], nc = cc + d[1];
                            if (nr >= 0 && nr < size && nc >= 0 && nc < size
                                && expanded[nr][nc] == 0) {
                                expanded[nr][nc] = 1;
                                q.push({nr, nc});
                            }
                        }
                    }
                }
            }
        }

        return regions;
    }
};


// ============================================================
// Approach 3: Upscale to 3x3 Grid + DFS (Recursive)
// ============================================================
// Same grid expansion as Approach 2, but uses DFS instead of BFS.
//
// Time:  O(n^2)
// Space: O(n^2) stack depth in worst case

class Solution3 {
public:
    int regionsBySlashes(vector<string>& grid) {
        int n = grid.size();
        int size = 3 * n;
        vector<vector<int>> expanded(size, vector<int>(size, 0));

        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                int br = 3 * r, bc = 3 * c;
                if (grid[r][c] == '/') {
                    expanded[br][bc + 2] = 1;
                    expanded[br + 1][bc + 1] = 1;
                    expanded[br + 2][bc] = 1;
                } else if (grid[r][c] == '\\') {
                    expanded[br][bc] = 1;
                    expanded[br + 1][bc + 1] = 1;
                    expanded[br + 2][bc + 2] = 1;
                }
            }
        }

        int regions = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (expanded[i][j] == 0) {
                    regions++;
                    dfs(expanded, i, j, size);
                }
            }
        }
        return regions;
    }

private:
    void dfs(vector<vector<int>>& grid, int r, int c, int size) {
        if (r < 0 || r >= size || c < 0 || c >= size || grid[r][c] != 0) return;
        grid[r][c] = 1;
        dfs(grid, r + 1, c, size);
        dfs(grid, r - 1, c, size);
        dfs(grid, r, c + 1, size);
        dfs(grid, r, c - 1, size);
    }
};
