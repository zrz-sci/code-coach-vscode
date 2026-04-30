/*
 * LeetCode 264: Ugly Number II
 *
 * 标签: Hash Table, Math, Dynamic Programming, Heap (Priority Queue)
 * 难度: Medium
 *
 * 丑数 = 质因数只包含 2, 3, 5 的正整数。1 是第一个丑数。
 * 求第 n 个丑数。
 *
 * 核心: 每个丑数 = 某个更小丑数 * 2/3/5
 *       三指针归并 = 最优解 O(n)
 *
 * 丑数序列的本质:
 *   三条有序的"流水线" (ugly*2, ugly*3, ugly*5)
 *   三路归并取最小 → 就是完整的丑数序列
 */

// ==================== 解法一: 三指针 DP (最优解) ====================
// 时间: O(n)  空间: O(n)
//
// 核心思想:
//   三条"流水线"分别生产 ugly*2, ugly*3, ugly*5
//   三个指针 p2, p3, p5 追踪各流水线进度
//   每步取三个候选值的最小值 → 下一个丑数
//
// 去重关键:
//   用三个独立的 if (不是 else if) 推进指针
//   因为 6 = 2*3 = 3*2, 多个流水线可能同时产生相同值
//
// 完整模拟 (n=12):
//   i  | dp[p2]*2  dp[p3]*3  dp[p5]*5 | dp[i] | p2 p3 p5
//   ---+-------------------------------+-------+---------
//   2  | 1*2=2     1*3=3     1*5=5     |  2    | 2  1  1
//   3  | 2*2=4     1*3=3     1*5=5     |  3    | 2  2  1
//   4  | 2*2=4     2*3=6     1*5=5     |  4    | 3  2  1
//   5  | 3*2=6     2*3=6     1*5=5     |  5    | 3  2  2
//   6  | 3*2=6     2*3=6     2*5=10    |  6    | 4  3  2  ← 两指针同时推进
//   7  | 4*2=8     3*3=9     2*5=10    |  8    | 5  3  2
//   8  | 5*2=10    3*3=9     2*5=10    |  9    | 5  4  2
//   9  | 5*2=10    4*3=12    2*5=10    | 10    | 6  4  3  ← 两指针同时推进
//  10  | 6*2=12    4*3=12    3*5=15    | 12    | 7  5  3  ← 两指针同时推进
//
class Solution {
public:
    int nthUglyNumber(int n) {
        // dp[i] = 第 i 个丑数 (1-indexed)
        vector<int> dp(n + 1);
        dp[1] = 1;

        // 三个指针: 分别追踪 *2, *3, *5 流水线的当前位置
        int p2 = 1, p3 = 1, p5 = 1;

        for (int i = 2; i <= n; i++) {
            // 三个候选值
            int val2 = dp[p2] * 2;
            int val3 = dp[p3] * 3;
            int val5 = dp[p5] * 5;

            // 取最小值作为下一个丑数
            dp[i] = min({val2, val3, val5});

            // 推进产生最小值的指针 (可能多个!)
            // 关键: 用三个独立的 if, 不是 if-else-if
            if (dp[i] == val2) p2++;
            if (dp[i] == val3) p3++;
            if (dp[i] == val5) p5++;
        }

        return dp[n];
    }
};

/*
 * 为什么三个 if 不能用 if-else-if?
 *
 * 当 i=6 时: dp[p2]*2=6, dp[p3]*3=6, dp[p5]*5=10
 *
 * 如果用 if-else-if:
 *   dp[6]=6, 只推进 p2
 *   下一轮 dp[p3]*3 还是 6 → dp[7]=6 → 重复!
 *
 * 用三个独立 if:
 *   dp[6]=6, p2 和 p3 都推进 → 不会重复
 */


// ==================== 解法二: 最小堆 + HashSet ====================
// 时间: O(n log n)  空间: O(n)
//
// 思路: BFS 式生成
//   1. 初始堆中放入 1
//   2. 每次弹出最小值 curr
//   3. 将 curr*2, curr*3, curr*5 入堆（如果没见过）
//   4. 第 n 次弹出的就是答案
//
// 注意:
//   - 用 long 防止乘法溢出
//   - HashSet 去重（否则同一个值入堆多次）
class Solution_Heap {
public:
    int nthUglyNumber(int n) {
        // 最小堆
        priority_queue<long, vector<long>, greater<long>> pq;
        // 去重集合
        unordered_set<long> seen;

        pq.push(1L);
        seen.insert(1L);

        long curr = 0;
        vector<long> factors = {2L, 3L, 5L};

        for (int i = 0; i < n; i++) {
            curr = pq.top();
            pq.pop();

            // 生成 curr * 2, curr * 3, curr * 5
            for (long f : factors) {
                long next = curr * f;
                if (seen.find(next) == seen.end()) {
                    seen.insert(next);
                    pq.push(next);
                }
            }
        }

        return static_cast<int>(curr);
    }
};


// ==================== 解法三: 三指针 DP (0-indexed 变体) ====================
// 逻辑完全相同，只是数组下标从 0 开始
class Solution_0indexed {
public:
    int nthUglyNumber(int n) {
        vector<int> ugly(n);
        ugly[0] = 1;
        int p2 = 0, p3 = 0, p5 = 0;

        for (int i = 1; i < n; i++) {
            int next2 = ugly[p2] * 2;
            int next3 = ugly[p3] * 3;
            int next5 = ugly[p5] * 5;

            ugly[i] = min({next2, next3, next5});

            if (ugly[i] == next2) p2++;
            if (ugly[i] == next3) p3++;
            if (ugly[i] == next5) p5++;
        }

        return ugly[n - 1];
    }
};


// ==================== 推广: Super Ugly Number (LC 313) ====================
// 质因数从 {2,3,5} 推广到任意质数集合 primes
// 三指针变成 k 指针, 时间 O(n*k)
class Solution_SuperUgly {
public:
    int nthSuperUglyNumber(int n, vector<int>& primes) {
        int k = primes.size();
        vector<int> dp(n, 0);
        dp[0] = 1;
        vector<int> ptrs(k, 0); // k 个指针

        for (int i = 1; i < n; i++) {
            dp[i] = INT_MAX;
            for (int j = 0; j < k; j++) {
                dp[i] = min(dp[i], dp[ptrs[j]] * primes[j]);
            }
            for (int j = 0; j < k; j++) {
                if (dp[i] == dp[ptrs[j]] * primes[j]) {
                    ptrs[j]++;
                }
            }
        }

        return dp[n - 1];
    }
};


/*
 * ┌──────────────────────┬──────────────┬────────┬──────────────────────────┐
 * │ 解法                 │ 时间         │ 空间   │ 说明                     │
 * ├──────────────────────┼──────────────┼────────┼──────────────────────────┤
 * │ 三指针 DP            │ O(n)         │ O(n)   │ 最优, 面试首选           │
 * │ 最小堆 + HashSet     │ O(n log n)   │ O(n)   │ 思路直观, 代码稍长       │
 * │ Super Ugly 推广      │ O(n * k)     │ O(n)   │ k个质因数的通用版本      │
 * └──────────────────────┴──────────────┴────────┴──────────────────────────┘
 *
 * 面试策略:
 * 1. 先讲暴力不可行 (第1690个丑数是21亿+)
 * 2. 引出"每个丑数 = 更小丑数 * 2/3/5"的递推性
 * 3. 讲三路归并的思想，用小例子 (n=6) 模拟
 * 4. 强调去重关键: 三个独立 if
 * 5. 被追问时提 Super Ugly Number 的 k 指针推广
 *
 * 三指针 DP 的本质:
 *   等价于"合并三个有序序列"
 *   流水线 *2: 2, 4, 6, 8, 10, 12 ...
 *   流水线 *3: 3, 6, 9, 12, 15, 18 ...
 *   流水线 *5: 5, 10, 15, 20, 25, 30 ...
 *   三路归并取最小 → 丑数序列
 *
 * ═══════════════════════════════════════════════════════
 * 易错点
 * ═══════════════════════════════════════════════════════
 *
 * ✗ 用 else if 推进指针 → 序列出现重复元素 (如 6=2*3=3*2 只推进一个指针)
 * ✗ 堆方案不去重 → 同一丑数多次入堆, 结果错误
 * ✗ 堆方案用 int → 中间乘积溢出, 必须用 long
 * ✗ dp 数组大小 off-by-one → 1-indexed 需要 dp[n+1], 0-indexed 需要 dp[n]
 * ✗ 三指针初始值搞错 → p2=p3=p5 都应指向 dp[1]=1 (1-indexed) 或 dp[0]=1 (0-indexed)
 *
 * ═══════════════════════════════════════════════════════
 * 面试追问
 * ═══════════════════════════════════════════════════════
 *
 * Q1: 推广到 Super Ugly Number (LC 313) 怎么做?
 * A1: 三指针变 k 指针, 时间 O(n*k), 空间 O(n+k). 每步取 k 个候选最小值.
 *
 * Q2: 为什么三个独立 if 而不是 else if?
 * A2: 同一个丑数可能被多条流水线同时产生 (如 6=2*3=3*2).
 *     else if 只推进一个指针, 下轮另一条线仍产生相同值, 导致重复.
 *
 * Q3: 堆解法和 DP 解法如何选?
 * A3: DP O(n) 最优, 面试首选. 堆 O(n log n) 思路更直观,
 *     可作为初始方案再优化. 讲堆时别忘去重(HashSet).
 *
 * Q4: 这道题和"合并 k 个有序链表"(LC 23)有什么关系?
 * A4: 本质相同 — 多路归并. 三条流水线 = 三个有序序列, 归并取最小.
 */
