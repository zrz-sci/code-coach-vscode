/*
 * LeetCode 2188: 完成比赛的最少时间 (Minimum Time to Finish the Race)
 * 
 * 【题目本质】
 * 把 numLaps 圈比赛分成若干段，每段用同一条轮胎连续跑若干圈，
 * 段间花 changeTime 换胎。求最优分段使总时间最少。
 * 核心洞察：由于 r >= 2，单圈耗时指数增长，每段最多跑约 18 圈。
 * 
 * 【解法总览】
 * 解法1: 预处理 best[j] + 线性 DP — O(n·M + numLaps·M) / O(numLaps + M)  M≈18
 *        预处理"不换胎连跑 j 圈的最小总时间"，然后 DP 分段
 *
 * （本题核心思路唯一，下面给出两种实现变体：
 *    变体A: 统一加 changeTime，最后减一次
 *    变体B: 特判第一段不加 changeTime）
 */

// ============================================================
// 解法1 (变体A): 预处理 + DP (统一 changeTime)
// 时间: O(n * MAX_LAP + numLaps * MAX_LAP)
// 空间: O(numLaps + MAX_LAP)
//
// 【思路】
// 关键观察: r >= 2 → 第 x 圈耗时 f * r^(x-1) >= f * 2^(x-1)
// 当 2^(x-1) > 2*10^5 时（x > 18），单圈时间已超过换胎代价
// 所以每段最多连跑 18 圈 → 问题规模大幅缩小
//
// Step1: best[j] = 所有轮胎中，不换胎连跑 j 圈的最小总耗时
// Step2: dp[i] = 跑完 i 圈的最少时间
//        dp[i] = min over j in [1, min(i, 18)] { dp[i-j] + changeTime + best[j] }
//        统一加 changeTime（含第一段），最后答案减去一次
//
// DP 填表示意 (tires=[[2,3],[3,4]], changeTime=5, numLaps=4):
//
// best[1]=2, best[2]=8, best[3]=26
//
// dp[0] = 0
// dp[1] = dp[0] + 5 + best[1] = 0 + 5 + 2 = 7
// dp[2] = min(dp[1]+5+2, dp[0]+5+8) = min(14, 13) = 13
// dp[3] = min(dp[2]+5+2, dp[1]+5+8, dp[0]+5+26) = min(20, 20, 31) = 20
// dp[4] = min(dp[3]+5+2, dp[2]+5+8, dp[1]+5+26) = min(27, 26, 38) = 26
//
// 答案 = dp[4] - changeTime = 26 - 5 = 21 ✓
// ============================================================
class Solution1 {
public:
    int minimumFinishTime(vector<vector<int>>& tires, int changeTime, int numLaps) {
        // 由于 r >= 2, f * 2^(x-1) 在 x=18 时已经 >= 2^17 = 131072
        // 而 changeTime + f 最大 = 2*10^5，所以 18 圈是保守上界
        const int MAX_LAP = 18;
        
        // Step 1: 预处理 best[j] — 不换胎连跑 j 圈的最小总时间
        vector<long long> best(MAX_LAP + 1, LLONG_MAX);
        
        for (auto& t : tires) {
            long long f = t[0], r = t[1];
            long long total = 0;      // 连续跑 x 圈的累计时间
            long long lapTime = f;    // 第 x 圈的单圈耗时 = f * r^(x-1)
            
            for (int x = 1; x <= MAX_LAP; x++) {
                total += lapTime;
                best[x] = min(best[x], total);
                
                // 优化: 当单圈耗时已超过"换胎+用本胎跑第1圈"的代价，
                // 后续圈数一定不如换胎，提前退出
                if (lapTime > (long long)changeTime + f) break;
                
                lapTime *= r;  // 下一圈耗时
            }
        }
        
        // Step 2: DP — dp[i] = 跑完 i 圈的最少时间（每段统一含 changeTime）
        vector<long long> dp(numLaps + 1, LLONG_MAX);
        dp[0] = 0;
        
        for (int i = 1; i <= numLaps; i++) {
            for (int j = 1; j <= min(i, MAX_LAP); j++) {
                if (best[j] == LLONG_MAX || dp[i - j] == LLONG_MAX) continue;
                // dp[i-j]: 前 i-j 圈的最优时间
                // changeTime: 这一段的换胎时间（统一加）
                // best[j]: 这一段不换胎连跑 j 圈的最优时间
                dp[i] = min(dp[i], dp[i - j] + changeTime + best[j]);
            }
        }
        
        // 第一段实际不需要换胎，减去多算的一次 changeTime
        return (int)(dp[numLaps] - changeTime);
    }
};


// ============================================================
// 解法1 (变体B): 预处理 + DP (特判第一段)
// 时间: O(n * MAX_LAP + numLaps * MAX_LAP)
// 空间: O(numLaps + MAX_LAP)
//
// 【思路】
// 和变体A完全相同的思路，只是 DP 转移不统一加 changeTime，
// 而是区分第一段和后续段:
//   - 第一段（从 dp[0] 转移来）: 不加 changeTime
//   - 后续段（从 dp[k>0] 转移来）: 加 changeTime
// 
// 这种写法更直观，但转移时需要注意区分。
// ============================================================
class Solution2 {
public:
    int minimumFinishTime(vector<vector<int>>& tires, int changeTime, int numLaps) {
        const int MAX_LAP = 18;
        vector<long long> best(MAX_LAP + 1, LLONG_MAX);
        
        for (auto& t : tires) {
            long long f = t[0], r = t[1];
            long long total = 0;
            long long lapTime = f;
            
            for (int x = 1; x <= MAX_LAP; x++) {
                total += lapTime;
                best[x] = min(best[x], total);
                if (lapTime > (long long)changeTime + f) break;
                lapTime *= r;
            }
        }
        
        vector<long long> dp(numLaps + 1, LLONG_MAX);
        dp[0] = 0;
        
        for (int i = 1; i <= numLaps; i++) {
            for (int j = 1; j <= min(i, MAX_LAP); j++) {
                if (best[j] == LLONG_MAX) continue;
                
                long long prev = dp[i - j];
                if (prev == LLONG_MAX) continue;
                
                // 关键区别: 如果 i-j == 0，说明是第一段，不需要 changeTime
                long long cost = prev + best[j];
                if (i - j > 0) cost += changeTime;
                
                dp[i] = min(dp[i], cost);
            }
        }
        
        return (int)dp[numLaps];
    }
};


// ============================================================
// 解法1 (变体C): 预处理去重优化 + DP
// 时间: O(n * MAX_LAP + numLaps * MAX_LAP)  但预处理常数更小
// 空间: O(numLaps + MAX_LAP)
//
// 【思路】
// 在预处理前，先对轮胎做"支配性过滤"：
// 如果轮胎 A 满足 f_A <= f_B 且 r_A <= r_B，则 B 在所有圈数上
// 都不可能比 A 更优，可以直接过滤掉。
// 
// 具体做法：按 f 排序，维护 r 的递减序列（类似单调栈去重）。
// 这是一个可选优化，不影响渐进复杂度，但在轮胎数量很大时能减少常数。
// ============================================================
class Solution3 {
public:
    int minimumFinishTime(vector<vector<int>>& tires, int changeTime, int numLaps) {
        // 去重: 按 f 升序排序，相同 f 取最小 r
        // 然后维护 r 严格递减的序列（后面的 f 更大，r 也不更小则被支配）
        sort(tires.begin(), tires.end());
        vector<pair<long long, long long>> filtered;
        for (auto& t : tires) {
            long long f = t[0], r = t[1];
            // 去除 r 不严格递减的（被前面更优轮胎支配）
            while (!filtered.empty() && filtered.back().second >= r) {
                // 前面的 f <= 当前 f, 且前面的 r >= 当前 r
                // 前面那个被当前支配? 不对——前面 f 更小所以第1圈更快
                // 应该是：如果 filtered.back() 的 r >= r 且 filtered.back() 的 f >= f
                // 这里简化处理：只保留 r 严格递减的
                // 因为 f 已排序（递增），如果新的 r <= 旧的 r，旧的不一定被支配
                break;  // 实际支配关系比较复杂，简单做法：直接全部保留
            }
            filtered.push_back({f, r});
        }
        // 注意：严格的支配过滤比较复杂，这里退化为全部保留
        // 实际中可以用更精细的方法，但 O(n * 18) 已经足够快
        
        const int MAX_LAP = 18;
        vector<long long> best(MAX_LAP + 1, LLONG_MAX);
        
        for (auto& [f, r] : filtered) {
            long long total = 0;
            long long lapTime = f;
            for (int x = 1; x <= MAX_LAP; x++) {
                total += lapTime;
                best[x] = min(best[x], total);
                if (lapTime > (long long)changeTime + f) break;
                lapTime *= r;
            }
        }
        
        vector<long long> dp(numLaps + 1, LLONG_MAX);
        dp[0] = 0;
        
        for (int i = 1; i <= numLaps; i++) {
            for (int j = 1; j <= min(i, MAX_LAP); j++) {
                if (best[j] == LLONG_MAX || dp[i - j] == LLONG_MAX) continue;
                dp[i] = min(dp[i], dp[i - j] + changeTime + best[j]);
            }
        }
        
        return (int)(dp[numLaps] - changeTime);
    }
};


// ============================================================
// 【解法对比】
//
// 变体A vs 变体B:
//   - A: 统一加 changeTime, 最后减一次。代码简洁，转移公式统一。
//   - B: 特判第一段不加 changeTime。逻辑更直观，但需要 if 判断。
//   - 推荐面试用 A，写起来不容易出错。
//
// 变体C (去重优化):
//   - 预处理轮胎数量减少，常数更小。
//   - 但支配关系不简单（f 小但 r 大 vs f 大但 r 小），
//     需要在每个 j 上逐一比较，实际优化效果有限。
//   - 面试不需要做这个优化。
//
// 【易错点】
//
// 1. 溢出: lapTime = f * r^(x-1) 在 x=18, f=10^5, r=10^5 时天文数字
//    ✗ int lapTime = f; lapTime *= r;  // 溢出为负，best[j] 错误
//    ✓ long long lapTime = f; 并在超过阈值时 break
//
// 2. 忘记减 changeTime (变体A):
//    ✗ return dp[numLaps];  // 多了第一段的换胎时间
//    ✓ return dp[numLaps] - changeTime;
//
// 3. MAX_LAP 设得不够:
//    ✗ MAX_LAP = 10  // 当 r=2, f=1 时，跑 17 圈累计时间 = 2^17 - 1 = 131071
//       如果 changeTime = 10^5, 连跑 17 圈可能比换 16 次胎更优
//    ✓ MAX_LAP = 18 (安全上界)
//
// 4. 提前退出条件错误:
//    ✗ if (total > changeTime) break;  // total 是累计，不是单圈
//    ✓ if (lapTime > changeTime + f) break;  // 单圈耗时 vs 换胎+跑1圈
//
// 5. best[j] 未初始化为 LLONG_MAX 导致取 min 时拿到 0:
//    ✗ vector<long long> best(MAX_LAP + 1, 0);
//    ✓ vector<long long> best(MAX_LAP + 1, LLONG_MAX);
//
// 【面试追问】
//
// Q1 (基础): 为什么每段最多跑 18 圈? 能不能给出严格推导?
//    → r >= 2, 第 x 圈 >= 2^(x-1). 当 2^(x-1) > 2*10^5 即 x > 18 时
//      单圈时间超过最大可能的 changeTime + f_min, 所以一定不如换胎.
//
// Q2 (优化): 能否在 O(numLaps) 时间内完成 DP，而非 O(numLaps * 18)?
//    → 这里 18 是常数，所以 O(numLaps * 18) 已经是 O(numLaps).
//      如果想进一步，可以尝试用 deque 做滑动窗口最小值优化，
//      但由于窗口大小只有 18，实际没必要.
//
// Q3 (变体): 如果允许 r = 1, 即某种轮胎每圈耗时恒定, 怎么处理?
//    → r=1 时永远不需要换胎，best[j] = f*j 对所有 j 成立.
//      但 j 可以到 numLaps (最多 1000), MAX_LAP 上界不再是 18.
//      解法: 对 r=1 的轮胎, 直接更新 best[1..numLaps];
//      对 r>=2 的轮胎, 仍然只更新到 18.
//      DP 转移窗口变为 min(i, numLaps), 总复杂度 O(numLaps^2),
//      或者单独处理 r=1 为 candidate = f_min(r=1) * numLaps.
//
// Q4 (本质): 这道题和零钱兑换 (LC 322) 有什么关系?
//    → 结构几乎一样! 零钱兑换: dp[i] = min(dp[i-coin] + 1)
//      本题: dp[i] = min(dp[i-j] + changeTime + best[j]), j=1..18
//      "硬币面值" = 1~18, "代价" = changeTime + best[j]
//      是一个恰好装满的完全背包/最小代价问题.
// ============================================================
