/*
 * LeetCode 818: Race Car (赛车)
 *
 * 【题目本质】
 * 车从 (pos=0, speed=+1) 出发，每步 A(加速) 或 R(反转)。
 * A: pos += speed, speed *= 2
 * R: speed = (speed > 0 ? -1 : 1), pos 不变
 * 求到达 target 的最少操作次数。
 *
 * 【解法总览】
 * 解法1: BFS 状态空间搜索    — O(t*logt) / O(t*logt)  — 直觉解法
 * 解法2: DP (bottom-up)      — O(t*logt) / O(t)       — 面试首选
 * 解法3: 记忆化搜索 (top-down) — O(t*logt) / O(t)     — DP 的递归版
 */

// ============================================================
// 解法1: BFS — 最短路径搜索
// 时间: O(t * log t)  空间: O(t * log t)
//
// 【思路】
// 状态 = (position, speed)。从 (0, 1) 出发做 BFS。
// 每个状态有两个转移: A 和 R。
// BFS 保证第一次到达 target 时步数最少。
//
// 关键剪枝:
// 1. position 限制在 [0, 2*target] 范围内
//    (越过太多再回来不可能比直接策略更优)
// 2. 用 visited 集合避免重复状态
//
// 示例: target = 3
//   (0,1) --A--> (1,2) --A--> (3,4) 到达! 步数=2
// ============================================================
class Solution1 {
public:
    int racecar(int target) {
        // BFS 队列: (position, speed, steps)
        queue<tuple<int, int, int>> q;
        q.push({0, 1, 0});

        // visited: 记录已访问的 (position, speed) 状态
        set<pair<int,int>> visited;
        visited.insert({0, 1});

        while (!q.empty()) {
            auto [pos, speed, steps] = q.front();
            q.pop();

            // 到达目标
            if (pos == target) return steps;

            // 选择1: Accelerate
            int npos = pos + speed;
            int nspeed = speed * 2;
            if (npos > 0 && npos < 2 * target
                && !visited.count({npos, nspeed})) {
                visited.insert({npos, nspeed});
                q.push({npos, nspeed, steps + 1});
            }

            // 选择2: Reverse
            nspeed = (speed > 0) ? -1 : 1;
            if (!visited.count({pos, nspeed})) {
                visited.insert({pos, nspeed});
                q.push({pos, nspeed, steps + 1});
            }
        }
        return -1; // 不会到达这里
    }
};

// ============================================================
// 解法2: DP (Bottom-Up) — 面试首选
// 时间: O(t * log t)  空间: O(t)
//
// 【思路】
// dp[i] = 从 (0, +1) 到达位置 i 的最少操作次数
//
// 核心数学性质:
//   连续 k 次 A 后: position = 2^k - 1, speed = 2^k
//   (速度 1 + 2 + 4 + ... + 2^(k-1) = 2^k - 1)
//
// 对于目标 t，找最小的 k 使得 2^k - 1 >= t:
//
// Case 1: 2^k - 1 == t
//   完美! dp[t] = k (连续 k 次 A 直达)
//
// Case 2: 2^k - 1 > t (越过了)
//   先走 k 步到 2^k - 1，反转(1步)，回头走 2^k - 1 - t
//   dp[t] = k + 1 + dp[2^k - 1 - t]
//   注: 2^k - 1 - t < t (否则 k-1 就够了)，子问题规模变小
//
// Case 3: 不越过，先走 k-1 步到 2^(k-1) - 1，反转，走 j 步(反方向)，再反转
//   位置 = 2^(k-1) - 1 - (2^j - 1) = 2^(k-1) - 2^j
//   剩余距离 = t - (2^(k-1) - 2^j)
//   dp[t] = (k-1) + 1(R) + j + 1(R) + dp[t - 2^(k-1) + 2^j]
//   j 范围: 0 到 k-2
//
// 示例: target = 6
//   k = 3 (2^3 - 1 = 7 >= 6)
//   Case 1: 7 != 6, 跳过
//   Case 2: 3 + 1 + dp[7-6] = 4 + dp[1] = 4 + 1 = 5
//   Case 3:
//     j=0: 2 + 1 + 0 + 1 + dp[6-3+1] = 4 + dp[4] = 4 + ?
//     j=1: 2 + 1 + 1 + 1 + dp[6-3+2] = 5 + dp[5] = 5 + ?
//   最终 dp[6] = 5 (序列 "AAARA")
// ============================================================
class Solution2 {
public:
    int racecar(int target) {
        vector<int> dp(target + 1, INT_MAX);
        dp[0] = 0;

        for (int t = 1; t <= target; t++) {
            // 找最小的 k 使得 2^k - 1 >= t
            int k = 1;
            while ((1 << k) - 1 < t) k++;

            int reach = (1 << k) - 1; // = 2^k - 1

            // Case 1: 恰好到达
            if (reach == t) {
                dp[t] = k;
                continue;
            }

            // Case 2: 越过后反转回来
            // 越过距离 = reach - t, 子问题 dp[reach - t]
            dp[t] = min(dp[t], k + 1 + dp[reach - t]);

            // Case 3: 不到(走 k-1 步), 反转走 j 步, 再反转继续
            int partial = (1 << (k - 1)) - 1; // = 2^(k-1) - 1
            for (int j = 0; j < k - 1; j++) {
                int back = (1 << j) - 1; // 反方向走的距离
                // 当前位置 = partial - back
                // 剩余距离 = t - partial + back
                int remaining = t - partial + back;
                dp[t] = min(dp[t], (k - 1) + 1 + j + 1 + dp[remaining]);
            }
        }

        return dp[target];
    }
};

// ============================================================
// 解法3: 记忆化搜索 (Top-Down)
// 时间: O(t * log t)  空间: O(t)
//
// 【思路】
// 和解法2 完全相同的转移方程，但用递归 + 记忆化实现。
// 好处：只计算需要的子问题，不用填整张 DP 表。
// ============================================================
class Solution3 {
public:
    unordered_map<int, int> memo;

    int racecar(int target) {
        if (target == 0) return 0;
        if (memo.count(target)) return memo[target];

        int k = 1;
        while ((1 << k) - 1 < target) k++;
        int reach = (1 << k) - 1;

        // Case 1: 恰好到达
        if (reach == target) {
            memo[target] = k;
            return k;
        }

        // Case 2: 越过后回来
        int best = k + 1 + racecar(reach - target);

        // Case 3: 不到，反转走一段再反转
        int partial = (1 << (k - 1)) - 1;
        for (int j = 0; j < k - 1; j++) {
            int back = (1 << j) - 1;
            best = min(best, (k - 1) + 1 + j + 1 + racecar(target - partial + back));
        }

        memo[target] = best;
        return best;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间       | 空间       | 优势                  |
// |------------|-----------|-----------|----------------------|
// | BFS        | O(t*logt) | O(t*logt) | 直觉，无需数学推导     |
// | DP         | O(t*logt) | O(t)      | 空间更优，面试首选     |
// | 记忆化搜索  | O(t*logt) | O(t)      | 只算需要的子问题       |
//
// ============================================================
// 【易错点】
//
// 1. 加速公式:
//    x 连续 k 次 A 到达 2^k
//    v 到达 2^k - 1 (等比数列求和: 1+2+4+...+2^(k-1) = 2^k - 1)
//
// 2. Case 3 中 j 的范围:
//    x j 从 0 到 k-1
//    v j 从 0 到 k-2 (j=k-1 时走回了起点附近，等于白走)
//
// 3. 反转操作的步数:
//    x R 不消耗步数
//    v R 消耗 1 步 (题目明确定义)
//
// 4. Case 2 子问题规模:
//    2^k - 1 - t < t 一定成立 (因为 2^(k-1) - 1 < t)
//    所以递推方向正确，不会无限循环
//
// ============================================================
// 【相关题型】
//
// - 1654. 到家的最少跳跃次数:
//   一维最短路径, BFS。跳跃规则不同但搜索框架相同。
//
// - 279. 完全平方数:
//   DP 求最少操作数到达目标值。
//   dp[n] = min(dp[n - k^2]) + 1, 子问题结构类似。
//
// - 322. 零钱兑换:
//   DP 求最少硬币数凑出目标金额, 同类型最优化问题。
// ============================================================
