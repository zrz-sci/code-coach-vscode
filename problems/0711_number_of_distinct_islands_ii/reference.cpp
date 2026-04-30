// 711. Number of Distinct Islands II
// 难度: Hard
// 标签: DFS, BFS, Union Find, Array, Hash Table, Matrix, Sorting
//
// 思路: DFS提取岛屿坐标 -> 对每个岛屿生成8种对称变换 -> 归一化排序 ->
//       取字典序最小作为canonical form -> 用set去重
//
// 时间复杂度: O(m*n * k*log(k)), k为单个岛屿最大格子数
// 空间复杂度: O(m*n)

// ============================================================
// 解法一: DFS + 8种对称变换标准化（推荐面试解法）
// ============================================================
class Solution {
public:
    int numDistinctIslands2(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        set<vector<pair<int,int>>> distinctIslands;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    // DFS 收集岛屿坐标
                    vector<pair<int,int>> island;
                    dfs(grid, i, j, m, n, island);

                    // 计算 canonical form 并加入 set
                    auto canonical = getCanonical(island);
                    distinctIslands.insert(canonical);
                }
            }
        }

        return distinctIslands.size();
    }

private:
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    void dfs(vector<vector<int>>& grid, int r, int c, int m, int n,
             vector<pair<int,int>>& island) {
        grid[r][c] = 0;  // 标记已访问
        island.push_back({r, c});

        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                dfs(grid, nr, nc, m, n, island);
            }
        }
    }

    // 生成8种对称变换下的canonical form（字典序最小的归一化坐标）
    vector<pair<int,int>> getCanonical(vector<pair<int,int>>& island) {
        vector<vector<pair<int,int>>> allTransforms(8);

        for (auto& [r, c] : island) {
            // 8种对称变换 (二面体群 D4):
            // 4种旋转 + 4种旋转的镜像
            allTransforms[0].push_back({r, c});
            allTransforms[1].push_back({r, -c});
            allTransforms[2].push_back({-r, c});
            allTransforms[3].push_back({-r, -c});
            allTransforms[4].push_back({c, r});
            allTransforms[5].push_back({c, -r});
            allTransforms[6].push_back({-c, r});
            allTransforms[7].push_back({-c, -r});
        }

        // 对每种变换进行归一化：平移到原点 + 排序
        vector<vector<pair<int,int>>> normalized;
        for (auto& transform : allTransforms) {
            normalized.push_back(normalize(transform));
        }

        // 取字典序最小的作为 canonical form
        return *min_element(normalized.begin(), normalized.end());
    }

    // 归一化: 平移到(0,0)起点并排序
    vector<pair<int,int>> normalize(vector<pair<int,int>>& coords) {
        int minR = INT_MAX, minC = INT_MAX;
        for (auto& [r, c] : coords) {
            minR = min(minR, r);
            minC = min(minC, c);
        }

        vector<pair<int,int>> result;
        for (auto& [r, c] : coords) {
            result.push_back({r - minR, c - minC});
        }
        sort(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 解法二: BFS + 字符串哈希去重
// ============================================================
// 使用BFS代替DFS，用字符串序列化作为哈希key
class Solution_V2 {
public:
    int numDistinctIslands2(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        unordered_set<string> distinctIslands;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    vector<pair<int,int>> island;
                    bfs(grid, i, j, m, n, island);

                    string canonical = getCanonicalStr(island);
                    distinctIslands.insert(canonical);
                }
            }
        }

        return distinctIslands.size();
    }

private:
    void bfs(vector<vector<int>>& grid, int r, int c, int m, int n,
             vector<pair<int,int>>& island) {
        queue<pair<int,int>> q;
        q.push({r, c});
        grid[r][c] = 0;

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        while (!q.empty()) {
            auto [cr, cc] = q.front();
            q.pop();
            island.push_back({cr, cc});

            for (auto& d : dirs) {
                int nr = cr + d[0], nc = cc + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                    grid[nr][nc] = 0;
                    q.push({nr, nc});
                }
            }
        }
    }

    string getCanonicalStr(vector<pair<int,int>>& island) {
        vector<vector<pair<int,int>>> allTransforms(8);

        for (auto& [r, c] : island) {
            allTransforms[0].push_back({r, c});
            allTransforms[1].push_back({r, -c});
            allTransforms[2].push_back({-r, c});
            allTransforms[3].push_back({-r, -c});
            allTransforms[4].push_back({c, r});
            allTransforms[5].push_back({c, -r});
            allTransforms[6].push_back({-c, r});
            allTransforms[7].push_back({-c, -r});
        }

        string minStr;
        for (auto& transform : allTransforms) {
            string s = serializeNormalized(transform);
            if (minStr.empty() || s < minStr) {
                minStr = s;
            }
        }
        return minStr;
    }

    string serializeNormalized(vector<pair<int,int>>& coords) {
        int minR = INT_MAX, minC = INT_MAX;
        for (auto& [r, c] : coords) {
            minR = min(minR, r);
            minC = min(minC, c);
        }

        vector<pair<int,int>> normalized;
        for (auto& [r, c] : coords) {
            normalized.push_back({r - minR, c - minC});
        }
        sort(normalized.begin(), normalized.end());

        string result;
        for (auto& [r, c] : normalized) {
            result += to_string(r) + "," + to_string(c) + ";";
        }
        return result;
    }
};

// ============================================================
// 关键点总结:
// 1. 8种对称变换覆盖了旋转(4种)+翻转(镜像后再4种旋转)
// 2. 归一化: 平移到(0,0)原点 + 排序，保证同一形状有唯一表示
// 3. canonical form = 8种归一化中字典序最小的
// 4. 解法一用 set<vector<pair>> 去重，简洁直观
// 5. 解法二用字符串序列化 + unordered_set，哈希可能更快
// 6. 注意DFS/BFS中要标记已访问（grid[r][c]=0），避免重复
// ============================================================
