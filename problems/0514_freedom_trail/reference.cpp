/*
 * 【题目本质】
 * 环形拨盘上拼字符串的最少步数。本质是"环形距离 + 多阶段 DP"。
 * 状态：(已拼字符数, ring 当前对齐位置)
 *
 * 【解法总览】
 * 解法一：DP + 位置预处理（推荐）   O(m*n^2) / O(m*n)
 * 解法二：记忆化搜索               O(m*n^2) / O(m*n)
 * 解法三：DP 空间优化（滚动数组）    O(m*n^2) / O(n)
 */

// ===================== 解法一：DP + 位置预处理（推荐） =====================
// 思路：dp[i][j] = 拼完 key[0..i] 且 ring[j] 对齐12点的最小步数
//       转移：从上一步所有可能位置 k 出发，计算环形最短距离
class Solution1 {
public:
    int findRotateSteps(string ring, string key) {
        int n = ring.size(), m = key.size();

        // 预处理每个字符在 ring 中的位置
        vector<vector<int>> pos(26);
        for (int i = 0; i < n; i++) {
            pos[ring[i] - 'a'].push_back(i);
        }

        // dp[i][j] = 拼完 key[0..i]，ring[j] 对齐12点的最小步数
        vector<vector<int>> dp(m, vector<int>(n, INT_MAX));

        // 初始化：从位置 0 拼 key[0]
        for (int j : pos[key[0] - 'a']) {
            int dist = min(j, n - j);   // 环形最短距离
            dp[0][j] = dist + 1;        // +1 按按钮
        }

        // 转移
        for (int i = 1; i < m; i++) {
            for (int j : pos[key[i] - 'a']) {           // 当前字符的候选位置
                for (int k : pos[key[i - 1] - 'a']) {   // 上一字符的可能位置
                    int dist = abs(j - k);
                    dist = min(dist, n - dist);          // 环形取较小方向
                    dp[i][j] = min(dp[i][j], dp[i - 1][k] + dist + 1);
                }
            }
        }

        // 答案：拼完最后一个字符的所有位置中的最小值
        int ans = INT_MAX;
        for (int j : pos[key[m - 1] - 'a']) {
            ans = min(ans, dp[m - 1][j]);
        }
        return ans;
    }
};

// ===================== 解法二：记忆化搜索 =====================
// 思路：dfs(i, j) = 从 ring 位置 j 开始拼 key[i..m-1] 的最小步数
class Solution2 {
public:
    int findRotateSteps(string ring, string key) {
        int n = ring.size(), m = key.size();
        vector<vector<int>> pos(26);
        for (int i = 0; i < n; i++) pos[ring[i] - 'a'].push_back(i);

        vector<vector<int>> memo(m, vector<int>(n, -1));

        function<int(int, int)> dfs = [&](int i, int j) -> int {
            if (i == m) return 0;
            if (memo[i][j] != -1) return memo[i][j];

            int res = INT_MAX;
            for (int k : pos[key[i] - 'a']) {
                int dist = abs(k - j);
                dist = min(dist, n - dist);
                res = min(res, dist + 1 + dfs(i + 1, k));
            }
            return memo[i][j] = res;
        };

        return dfs(0, 0);
    }
};

// ===================== 解法三：DP 空间优化 =====================
// 思路：dp[i] 只依赖 dp[i-1]，用滚动数组
class Solution3 {
public:
    int findRotateSteps(string ring, string key) {
        int n = ring.size(), m = key.size();
        vector<vector<int>> pos(26);
        for (int i = 0; i < n; i++) pos[ring[i] - 'a'].push_back(i);

        vector<int> prev(n, INT_MAX), cur(n, INT_MAX);

        // 初始化
        for (int j : pos[key[0] - 'a']) {
            prev[j] = min(j, n - j) + 1;
        }

        for (int i = 1; i < m; i++) {
            fill(cur.begin(), cur.end(), INT_MAX);
            for (int j : pos[key[i] - 'a']) {
                for (int k : pos[key[i - 1] - 'a']) {
                    int dist = abs(j - k);
                    dist = min(dist, n - dist);
                    cur[j] = min(cur[j], prev[k] + dist + 1);
                }
            }
            swap(prev, cur);
        }

        int ans = INT_MAX;
        for (int j : pos[key[m - 1] - 'a']) {
            ans = min(ans, prev[j]);
        }
        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优点 | 缺点 |
 * |------|------|------|------|------|
 * | DP+预处理 | O(m*n^2) | O(m*n) | 逻辑清晰 | 空间较大 |
 * | 记忆化搜索 | O(m*n^2) | O(m*n) | 自然递归思路 | 递归栈开销 |
 * | DP空间优化 | O(m*n^2) | O(n)   | 空间最优 | 代码稍复杂 |
 *
 * 【易错点】
 * 1. ring 是环形：距离 = min(abs(j-k), n-abs(j-k))
 * 2. 每拼一个字符需按按钮 +1 步
 * 3. 初始位置是 ring[0]，不是任意位置
 * 4. 预处理字符位置可显著减少无效枚举
 *
 * 【面试追问】
 * Q1: 时间复杂度能否优化？→ O(m*n) 如果用 BFS 分层但实践中不一定快
 * Q2: ring 不是环形怎么办？→ 去掉逆时针距离计算
 * Q3: 如果字符集很大（非26字母）？→ 用 unordered_map 存位置
 */
