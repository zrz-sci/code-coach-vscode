/*
 * ============================================================
 * LeetCode 204: Count Primes (计数质数)
 * 难度: Medium | 标签: Array, Math, Enumeration, Number Theory
 *
 * 【题目本质】
 * 统计严格小于 n 的质数个数
 * 经典筛法问题，核心是「埃拉托斯特尼筛法」
 *
 * 【解法总览】
 * 解法1: 埃氏筛法（优化版） — 从 i*i 开始标记，面试首选
 * 解法2: 线性筛（欧拉筛）   — 每个合数只标记一次，严格 O(n)
 * ============================================================
 */

// ==================== 解法1: 埃氏筛法（推荐） ====================
// 思路：从 2 开始，将每个质数的倍数标记为合数
//   优化1：外层只循环到 sqrt(n)
//   优化2：内层从 i*i 开始（i*2 ~ i*(i-1) 已被更小质数标记）
// 时间 O(n log log n)，空间 O(n)
class Solution1 {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;

        // is_prime[i] = true 表示 i 是质数
        vector<bool> is_prime(n, true);
        is_prime[0] = is_prime[1] = false;

        // 只需筛到 sqrt(n)
        for (int i = 2; (long)i * i < n; i++) {
            if (is_prime[i]) {
                // 从 i*i 开始标记合数，步长为 i
                for (int j = i * i; j < n; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        // 统计质数个数
        int count = 0;
        for (int i = 2; i < n; i++) {
            if (is_prime[i]) count++;
        }
        return count;
    }
};

// ==================== 解法2: 线性筛（欧拉筛） ====================
// 思路：每个合数只被它的「最小质因子」标记一次
//   关键：当 i % primes[j] == 0 时 break
//         因为 i 的更大倍数会被 i 的最小质因子在后续标记
// 时间 O(n)，空间 O(n)
class Solution2 {
public:
    int countPrimes(int n) {
        if (n <= 2) return 0;

        vector<bool> is_prime(n, true);
        vector<int> primes;  // 收集已发现的质数
        is_prime[0] = is_prime[1] = false;

        for (int i = 2; i < n; i++) {
            if (is_prime[i]) {
                primes.push_back(i);
            }
            // 用已有质数标记 i 的倍数
            for (int j = 0; j < (int)primes.size() && (long)i * primes[j] < n; j++) {
                is_prime[i * primes[j]] = false;
                // i 是 primes[j] 的倍数时停止
                // 保证 i*primes[j+1] 会被更小质因子在未来标记
                if (i % primes[j] == 0) break;
            }
        }

        return primes.size();
    }
};

/*
 * ============================================================
 * 【解法对比】
 * | 解法       | 时间            | 空间 | 重复标记 | 推荐 |
 * |-----------|----------------|------|---------|------|
 * | 埃氏筛法   | O(n log log n) | O(n) | 有      | ★★★  |
 * | 线性筛     | O(n)           | O(n) | 无      | ★★★  |
 *
 * 实际表现：n < 10^7 时埃氏筛法足够快，常数更小
 *          n > 10^7 时线性筛优势明显
 *
 * 【易错点】
 * 1. i*i 溢出 int → 用 (long)i*i < n 防溢出
 * 2. 题目要求「严格小于 n」→ 数组大小 n，不含 n
 * 3. 忘记处理 n <= 2 的边界 → 直接返回 0
 * 4. 线性筛中忘记 break 条件 → 退化为 O(n log n)
 * 5. 内层循环从 2*i 开始而非 i*i → 正确但低效
 *
 * 【面试追问】
 * Q1: O(n log log n) 怎么推导？
 *     → 质数的倒数和 ~ log log n（Mertens 定理）
 * Q2: 线性筛 break 的数学原理？
 *     → 合数 = 最小质因子 * 剩余因子，break 避免重复分配
 * Q3: 空间优化方案？
 *     → bitset 压缩 8 倍 / 只标记奇数减半 / 分段筛法处理超大范围
 * Q4: 如果要输出所有质数而非计数？
 *     → 线性筛已经收集在 primes 数组中，直接返回
 * ============================================================
 */
