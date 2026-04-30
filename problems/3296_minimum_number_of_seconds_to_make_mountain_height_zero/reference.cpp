/*
 * LeetCode 3296: Minimum Number of Seconds to Make Mountain Height Zero
 *                (移山所需的最少秒数)
 *
 * 【题目本质】
 * n 个工人并行工作降低山的高度。工人 i 降低高度 x 需要
 * workerTimes[i] * x*(x+1)/2 秒（边际成本递增）。
 * 求使山高度降到 0 的最少总时间。
 *
 * 核心：二分答案——二分总时间 T，对每个工人反推在 T 秒内
 * 最多能降多少高度，检查总和是否 >= mountainHeight。
 *
 * 【解法总览】
 * 解法1: 二分答案         — O(n * log(maxT)) / O(1) — 面试首选
 * 解法2: 小根堆贪心分配   — O(H * log(n))   / O(n) — 替代方案
 * 解法3: 二分答案(纯整数) — O(n * log(maxT)) / O(1) — 避免浮点
 */

// ============================================================
// 解法1: 二分答案 — 面试首选 ⭐
// 时间: O(n * log(maxT))  空间: O(1)
//
// 【思路】
// 二分总时间 T。对于给定的 T 秒：
//   工人 i 降高度 x 需要 w[i] * x*(x+1)/2 秒
//   → x*(x+1) <= 2*T/w[i]
//   → x = floor((-1 + sqrt(1 + 8*T/w[i])) / 2)
//
// check(T): 对每个工人算出 x_i，如果 sum(x_i) >= H 则 T 可行
//
// 示例: mountainHeight=4, workerTimes=[2,1,1]
//   T=3:
//     w[0]=2: limit=2*3/2=3, x*(x+1)<=3 → x=1 (1*2=2<=3, 2*3=6>3)
//     w[1]=1: limit=2*3/1=6, x*(x+1)<=6 → x=2 (2*3=6<=6)
//     w[2]=1: limit=6 → x=2
//     total = 1+2+2 = 5 >= 4 ✓
//   T=2:
//     w[0]=2: limit=2, x=1 (1*2=2<=2)
//     w[1]=1: limit=4, x=1 (1*2=2<=4, 2*3=6>4)
//     w[2]=1: limit=4, x=1
//     total = 1+1+1 = 3 < 4 ✗
//   → 答案是 3
//
// 浮点修正：
//   sqrt 可能不精确，计算 x 后需要 while 微调：
//   - 如果 (x+1)*(x+2) <= limit，说明 x 可以更大
//   - 如果 x*(x+1) > limit，说明 x 应该更小
// ============================================================
class Solution1 {
public:
    long long minNumberOfSeconds(int mountainHeight, vector<int>& workerTimes) {
        // 二分范围
        long long lo = 0;
        // 上界: 最快的工人单独完成全部高度的时间
        long long minW = *min_element(workerTimes.begin(), workerTimes.end());
        long long H = mountainHeight;
        long long hi = minW * H * (H + 1) / 2;

        // check: 给定 T 秒，所有工人总共能降多少高度？
        auto check = [&](long long T) -> bool {
            long long total = 0;
            for (int w : workerTimes) {
                // x*(x+1) <= 2*T/w
                long long limit = 2 * T / w;

                // 用 sqrt 估算 x，然后整数修正
                long long x = (long long)((-1.0 + sqrt(1.0 + 4.0 * (double)limit)) / 2.0);

                // 浮点修正：确保 x 是最大满足条件的整数
                while ((x + 1) * (x + 2) <= limit) x++;
                while (x > 0 && x * (x + 1) > limit) x--;

                total += x;
                // 提前退出优化
                if (total >= mountainHeight) return true;
            }
            return false;
        };

        // 标准二分模板：找最小的可行 T
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (check(mid)) {
                hi = mid;  // T 可行，尝试更小
            } else {
                lo = mid + 1;  // T 不可行，需要更大
            }
        }
        return lo;
    }
};

// ============================================================
// 解法2: 小根堆贪心 — 替代方案
// 时间: O(H * log(n))  空间: O(n)
//
// 【思路】
// 维护一个小根堆，存每个工人"完成下一步"的时间。
// 每次从堆顶取出完成时间最小的工人，让他多降 1 高度。
// 重复 mountainHeight 次，最后一次取出的时间就是答案。
//
// 堆中每个元素: (next_finish_time, worker_time, current_height)
//   next_finish_time = w * (h+1) * (h+2) / 2
//   即工人再多降 1 高度后的总时间
//
// 示例: mountainHeight=4, workerTimes=[2,1,1]
//   初始堆 (按 next_finish_time):
//     (1, 1, 0) ← 工人1: 降1需 1*1*2/2=1s
//     (1, 1, 0) ← 工人2: 降1需 1*1*2/2=1s
//     (2, 2, 0) ← 工人0: 降1需 2*1*2/2=2s
//
//   Round 1: pop (1,1,0) → 工人完成降1, 时间=1
//     push (3, 1, 1): 该工人下次完成时间=1*2*3/2=3
//   Round 2: pop (1,1,0) → 另一个工人完成降1, 时间=1
//     push (3, 1, 1)
//   Round 3: pop (2,2,0) → 工人0完成降1, 时间=2
//     push (6, 2, 1)
//   Round 4: pop (3,1,1) → 工人完成降2, 时间=3
//
//   答案 = 3 ✓
// ============================================================
class Solution2 {
public:
    long long minNumberOfSeconds(int mountainHeight, vector<int>& workerTimes) {
        // 小根堆: {next_finish_time, worker_time, current_assigned_height}
        using T = tuple<long long, int, int>;
        priority_queue<T, vector<T>, greater<T>> pq;

        // 初始化：每个工人"降第1个高度"需要的时间
        for (int w : workerTimes) {
            // 降 1 高度的时间 = w * 1 * 2 / 2 = w
            pq.push({(long long)w, w, 1});
        }

        long long ans = 0;
        for (int i = 0; i < mountainHeight; i++) {
            auto [finishTime, w, h] = pq.top();
            pq.pop();
            ans = finishTime; // 当前这步完成的时间

            // 该工人下一步: 从降 h 变成降 h+1
            // 总时间 = w * (h+1) * (h+2) / 2
            long long nextTime = (long long)w * (h + 1) * (h + 2) / 2;
            pq.push({nextTime, w, h + 1});
        }
        return ans;
    }
};

// ============================================================
// 解法3: 二分答案（纯整数运算，避免浮点误差）
// 时间: O(n * log(maxT))  空间: O(1)
//
// 【思路】
// 和解法1相同，但不用 sqrt 估算 x。
// 改用内层二分来求 x：对于给定的 limit = 2*T/w，
// 二分找最大的 x 使得 x*(x+1) <= limit。
//
// 内层二分范围: [0, sqrt(limit)] ≈ [0, mountainHeight]
// 总时间: O(n * log(maxT) * log(H))
//
// 优点: 完全避免浮点误差
// 缺点: 多一层 log（但常数小，实际影响不大）
// ============================================================
class Solution3 {
public:
    long long minNumberOfSeconds(int mountainHeight, vector<int>& workerTimes) {
        long long lo = 0;
        long long minW = *min_element(workerTimes.begin(), workerTimes.end());
        long long H = mountainHeight;
        long long hi = minW * H * (H + 1) / 2;

        auto maxHeight = [](long long limit) -> long long {
            // 二分找最大的 x 使得 x*(x+1) <= limit
            long long lo = 0, hi = (long long)sqrt((double)limit) + 2;
            while (lo < hi) {
                long long mid = lo + (hi - lo + 1) / 2;
                if (mid * (mid + 1) <= limit) {
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }
            return lo;
        };

        auto check = [&](long long T) -> bool {
            long long total = 0;
            for (int w : workerTimes) {
                long long limit = 2 * T / w;
                total += maxHeight(limit);
                if (total >= mountainHeight) return true;
            }
            return false;
        };

        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (check(mid)) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
};

/*
 * 易错点总结:
 * 1. 整数溢出:
 *    - hi 可达 10^6 * 10^5 * (10^5+1) / 2 ≈ 5 * 10^15, 必须用 long long
 *    - limit = 2 * T / w，中间结果也可能溢出
 * 2. 浮点 sqrt 精度:
 *    - sqrt(1 + 4*limit) 对于大 limit 可能丢失精度
 *    - 必须用 while 循环做整数修正
 *    - 或者用纯整数二分（解法3）
 * 3. 二分边界:
 *    - 使用 while(lo < hi), mid = lo+(hi-lo)/2, check(mid) ? hi=mid : lo=mid+1
 *    - 返回 lo（第一个可行的 T）
 * 4. check 函数的提前退出:
 *    - total >= mountainHeight 后立即返回 true
 *    - 避免不必要的 sqrt 计算
 * 5. 堆解法中完成时间的计算:
 *    - 工人降 h 高度的总时间是 w*h*(h+1)/2，不是 w*h
 *    - 堆中存的是"完成当前高度后的总时间"
 *
 * 面试追问:
 * Q: 为什么二分可行？
 *    → T 越大，每个工人能降越多，总能力单调递增，满足二分条件
 * Q: 二分的上界怎么取？
 *    → 最快工人单独完成全部: w_min * H*(H+1)/2
 * Q: 堆 vs 二分哪个更快？
 *    → 二分 O(n*log(maxT)) vs 堆 O(H*logn)
 *    → H 大时二分更优，n 大时堆更优
 * Q: 能否用贪心直接分配？
 *    → 可以用堆贪心（解法2），但直接数学分配比较困难
 */

// ============================================================
// 总结区块
// ============================================================
/*
 * 【解法对比】
 * | 解法               | 时间               | 空间   | 特点                        |
 * |-------------------|--------------------|--------|----------------------------|
 * | 二分+sqrt(解法1)   | O(n*log(maxT))     | O(1)   | 面试首选，最优空间           |
 * | 小根堆贪心(解法2)  | O(H*log(n))        | O(n)   | 直观，H 小时更优             |
 * | 二分+纯整数(解法3)  | O(n*log(maxT)*logH)| O(1)  | 避免浮点，最安全             |
 *
 * 面试首选: 解法1(二分+sqrt)。空间 O(1)，时间与 H 无关。
 * 当 H 很大(10^5)时，堆解法 O(H*logn) 可能 TLE，二分更稳。
 *
 * 【易错点清单】
 * 1. 整数溢出: hi 可达 ~5*10^15，中间乘法更大
 *    → 所有涉及 T、limit 的计算必须用 long long
 * 2. sqrt 浮点精度: 大数下 sqrt 丢失精度导致 x 偏差
 *    → 必须用 while 循环做 +1/-1 修正
 * 3. 二分模板: 求"最小可行值"用 lo < hi + hi = mid 模板
 *    → 不要用 lo <= hi 模板，容易死循环
 * 4. 堆中时间公式: 工人降 h 高度的总时间是 w*h*(h+1)/2
 *    → 不是 w*h！边际成本递增
 * 5. check 函数提前退出: total >= H 即可 return true
 *    → 避免对剩余工人做无意义的 sqrt 计算
 *
 * 【面试追问链】
 * Q1: 多个工人并行降山，求最少时间？（本题）
 *     → 二分答案 T，反推每个工人在 T 秒内能降多少
 * Q2: 如果每个工人有工作时间上限（最多工作 L 秒）？
 *     → check 函数中对每个工人取 min(T, L) 作为可用时间，
 *       其余逻辑不变
 * Q3: 如果工人之间有协作加成（两人一组效率翻倍）？
 *     → 变成分组优化问题，二分答案 + 贪心配对，
 *       类似 LC 2141 的"机器分配"思路
 */
