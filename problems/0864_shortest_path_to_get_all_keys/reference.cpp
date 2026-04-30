/*
 * LeetCode 864: Shortest Path to Get All Keys
 * 解法: BFS + Bitmask 状态压缩
 *
 * 思路: 将 (row, col, keys_bitmask) 作为 BFS 状态，
 *       用 bitmask 编码已收集的钥匙集合，
 *       BFS 保证第一次到达"所有钥匙都收集齐"的状态即为最短路径。
 *
 * 时间复杂度: O(m * n * 2^k)  其中 k 为钥匙数量（最多6）
 * 空间复杂度: O(m * n * 2^k)
 */

// ===================== 解法一：标准 BFS + Bitmask =====================

class Solution {
public:
    int shortestPathAllKeys(vector<string>& grid) {
        int m = grid.size(), n = grid[0].size();
        int startR = 0, startC = 0;
        int totalKeys = 0;

        // 第一步：遍历网格，找到起点和钥匙总数
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '@') {
                    startR = i;
                    startC = j;
                } else if (grid[i][j] >= 'a' && grid[i][j] <= 'f') {
                    totalKeys = max(totalKeys, grid[i][j] - 'a' + 1);
                }
            }
        }

        // 目标：所有钥匙都收集齐的 bitmask
        int targetKeys = (1 << totalKeys) - 1;

        // visited[row][col][keys] 三维去重
        // keys 取值范围 [0, 2^totalKeys - 1]
        vector<vector<vector<bool>>> visited(
            m, vector<vector<bool>>(n, vector<bool>(1 << totalKeys, false))
        );

        // BFS 队列: {row, col, keys_bitmask}
        queue<tuple<int, int, int>> q;
        q.push({startR, startC, 0});
        visited[startR][startC][0] = true;

        int steps = 0;
        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        while (!q.empty()) {
            int sz = q.size();
            // 按层遍历，每层代表走了一步
            for (int i = 0; i < sz; i++) {
                auto [r, c, keys] = q.front();
                q.pop();

                // 检查是否已收集完所有钥匙
                if (keys == targetKeys) {
                    return steps;
                }

                // 向四个方向扩展
                for (auto& d : dirs) {
                    int nr = r + d[0];
                    int nc = c + d[1];

                    // 边界检查
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;

                    char cell = grid[nr][nc];

                    // 墙壁：不可通过
                    if (cell == '#') continue;

                    // 锁：需要对应的钥匙
                    if (cell >= 'A' && cell <= 'F') {
                        if (!(keys & (1 << (cell - 'A')))) continue;
                    }

                    // 计算新的钥匙集合
                    int newKeys = keys;
                    if (cell >= 'a' && cell <= 'f') {
                        newKeys = keys | (1 << (cell - 'a'));
                    }

                    // 去重检查
                    if (!visited[nr][nc][newKeys]) {
                        visited[nr][nc][newKeys] = true;
                        q.push({nr, nc, newKeys});
                    }
                }
            }
            steps++;
        }

        // 无法收集所有钥匙
        return -1;
    }
};

// ===================== 解法二：用编码整数压缩状态 =====================
// 将 (row, col, keys) 编码为单个整数，减少内存开销

class SolutionEncoded {
public:
    int shortestPathAllKeys(vector<string>& grid) {
        int m = grid.size(), n = grid[0].size();
        int startR = 0, startC = 0, totalKeys = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '@') { startR = i; startC = j; }
                else if (grid[i][j] >= 'a' && grid[i][j] <= 'f') {
                    totalKeys = max(totalKeys, grid[i][j] - 'a' + 1);
                }
            }
        }

        int targetKeys = (1 << totalKeys) - 1;
        int keyBits = totalKeys; // bits needed for key mask
        int colBits = 5;        // n <= 30, 5 bits enough
        int rowBits = 5;        // m <= 30, 5 bits enough

        // 编码函数: row(5 bits) | col(5 bits) | keys(6 bits)
        auto encode = [&](int r, int c, int k) -> int {
            return (r << (colBits + keyBits)) | (c << keyBits) | k;
        };

        unordered_set<int> visited;
        queue<tuple<int, int, int>> q;
        int initState = encode(startR, startC, 0);
        visited.insert(initState);
        q.push({startR, startC, 0});

        int steps = 0;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                auto [r, c, keys] = q.front();
                q.pop();

                if (keys == targetKeys) return steps;

                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;

                    char cell = grid[nr][nc];
                    if (cell == '#') continue;
                    if (cell >= 'A' && cell <= 'F' && !(keys & (1 << (cell - 'A')))) continue;

                    int newKeys = keys;
                    if (cell >= 'a' && cell <= 'f') {
                        newKeys |= (1 << (cell - 'a'));
                    }

                    int state = encode(nr, nc, newKeys);
                    if (visited.find(state) == visited.end()) {
                        visited.insert(state);
                        q.push({nr, nc, newKeys});
                    }
                }
            }
            steps++;
        }
        return -1;
    }
};
