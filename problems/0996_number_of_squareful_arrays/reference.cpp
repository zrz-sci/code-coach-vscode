// [file://solution.cpp](./solution.cpp)
//
// LeetCode 996: Number of Squareful Arrays
// 解法一: 回溯 + 排序去重 (推荐面试)
// 解法二: 状态压缩 DP
// 解法三: 图论建模 + 计数

// 辅助函数: 判断一个数是否是完全平方数
// 注意浮点精度, 使用 round 后再验证
bool isSquare(int x) {
    if (x < 0) return false;
    int s = round(sqrt(x));
    return (long long)s * s == x;
}

// ============================================================
// 解法一: 回溯 + 排序去重
// 时间 O(n!), 空间 O(n)
// 排序后跳过同层重复选择, 相邻不满足 squareful 时剪枝
// ============================================================
class Solution {
public:
    int numSquarefulPerms(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int result = 0;
        vector<bool> used(n, false);

        // path: 当前已构建的排列
        vector<int> path;

        // 回溯函数
        function<void()> backtrack = [&]() {
            if ((int)path.size() == n) {
                result++;
                return;
            }

            for (int i = 0; i < n; i++) {
                // 跳过已使用的元素
                if (used[i]) continue;

                // 同层去重: 相同值的元素, 只允许按索引顺序使用
                // 即前一个相同值未使用时, 跳过当前
                if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) {
                    continue;
                }

                // squareful 检查: 当前元素与排列末尾元素之和必须是完全平方数
                if (!path.empty() && !isSquare(path.back() + nums[i])) {
                    continue;
                }

                // 选择当前元素
                used[i] = true;
                path.push_back(nums[i]);

                backtrack();

                // 撤销选择
                path.pop_back();
                used[i] = false;
            }
        };

        backtrack();
        return result;
    }
};

// ============================================================
// 解法二: 状态压缩 DP
// 时间 O(n^2 * 2^n), 空间 O(n * 2^n)
// dp[mask][i] = 已选集合为 mask, 最后选的是索引 i 的排列数
// ============================================================
class SolutionBitmaskDP {
public:
    int numSquarefulPerms(vector<int>& nums) {
        int n = nums.size();
        int total = 1 << n;

        // 预处理: 任意两个索引之间是否 squareful
        vector<vector<bool>> sq(n, vector<bool>(n, false));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    sq[i][j] = isSquare(nums[i] + nums[j]);
                }
            }
        }

        // dp[mask][i]: mask 集合中元素已选, 最后一个是 i
        vector<vector<int>> dp(total, vector<int>(n, 0));

        // 初始化: 选择单个元素作为起点
        // 处理重复: 对于相同值的元素, 只让第一个出现的作为起点
        for (int i = 0; i < n; i++) {
            // 检查是否有更早的相同值元素
            bool duplicate = false;
            for (int j = 0; j < i; j++) {
                if (nums[j] == nums[i]) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                dp[1 << i][i] = 1;
            }
        }

        // 状态转移
        for (int mask = 1; mask < total; mask++) {
            for (int i = 0; i < n; i++) {
                if (dp[mask][i] == 0) continue;
                if (!(mask & (1 << i))) continue;

                for (int j = 0; j < n; j++) {
                    // j 未被选过
                    if (mask & (1 << j)) continue;

                    // squareful 条件
                    if (!sq[i][j]) continue;

                    // 去重: 对于相同值元素, 强制按索引递增顺序选择
                    bool skipDup = false;
                    for (int k = 0; k < j; k++) {
                        if (nums[k] == nums[j] && !(mask & (1 << k))) {
                            skipDup = true;
                            break;
                        }
                    }
                    if (skipDup) continue;

                    dp[mask | (1 << j)][j] += dp[mask][i];
                }
            }
        }

        // 汇总: 所有元素都选了, 以任意元素结尾
        int fullMask = total - 1;
        int result = 0;
        for (int i = 0; i < n; i++) {
            result += dp[fullMask][i];
        }
        return result;
    }
};

// ============================================================
// 解法三: 图论建模 + DFS 计数
// 用 unordered_map 做邻接表, count map 处理重复
// 时间 O(n!), 空间 O(n^2)
// ============================================================
class SolutionGraph {
public:
    int numSquarefulPerms(vector<int>& nums) {
        int n = nums.size();

        // 统计每个值出现的次数
        unordered_map<int, int> count;
        for (int x : nums) count[x]++;

        // 建立邻接表: 哪些值的对可以相邻
        unordered_map<int, vector<int>> graph;
        for (auto& [v1, c1] : count) {
            for (auto& [v2, c2] : count) {
                if (isSquare(v1 + v2)) {
                    graph[v1].push_back(v2);
                }
            }
        }

        int result = 0;

        // DFS: 当前选的值是 cur, 已选 length 个
        function<void(int, int)> dfs = [&](int cur, int length) {
            if (length == n) {
                result++;
                return;
            }
            for (int next : graph[cur]) {
                if (count[next] > 0) {
                    count[next]--;
                    dfs(next, length + 1);
                    count[next]++;
                }
            }
        };

        // 尝试每个不同的值作为起点
        for (auto& [val, cnt] : count) {
            count[val]--;
            dfs(val, 1);
            count[val]++;
        }

        return result;
    }
};
