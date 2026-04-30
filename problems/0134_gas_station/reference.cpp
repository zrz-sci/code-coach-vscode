/*
 * LeetCode 134: 加油站 (Gas Station)
 *
 * 【题目本质】
 * 在环形数组上找一个起点，使得从该点出发，沿途累计净油量(gas[i]-cost[i])
 * 的前缀和始终 ≥ 0。
 *
 * 【解法总览】
 * 解法1: 暴力模拟     — O(n²) / O(1) — 枚举每个起点，模拟走一圈
 * 解法2: 贪心跳跃     — O(n)  / O(1) — 累计为负则跳过，一次遍历找起点 ⭐
 * 解法3: 最低点法     — O(n)  / O(1) — 找累计净油量最低点的下一个位置
 */

// ============================================================
// 解法1: 暴力模拟 — 枚举起点 + 模拟绕一圈
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的做法：对每个站点 start，模拟从 start 出发绕一圈。
// 每到一站：油箱 += gas[i] - cost[i]，如果变负则此 start 不行。
// 所有站点都不行则返回 -1。
// 
// 瓶颈：n 个起点 × 每个起点走 n 步 = O(n²)。
// 当 n = 10⁵ 时会超时。
// ============================================================
class Solution1 {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int n = gas.size();
        for (int start = 0; start < n; start++) {
            int tank = 0;
            bool ok = true;
            for (int step = 0; step < n; step++) {
                int i = (start + step) % n;  // 环形索引
                tank += gas[i] - cost[i];
                if (tank < 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) return start;
        }
        return -1;
    }
};

// ============================================================
// 解法2: 贪心（一次遍历 + 跳跃重置）⭐ 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 暴力解的瓶颈：从 start 出发在位置 j 失败后，只移到 start+1。
// 但实际上 start+1 ~ j 之间的所有点都不行（证明见下）。
//
// 核心贪心逻辑：
//   维护两个累加量：
//   - totalSum: 全程的净油量总和，用于判断有没有解
//   - curSum:   从当前候选 start 出发的累计净油量
//   如果 curSum < 0，说明 start ~ i 之间都不行，跳到 i+1 重新开始。
//
// 【为什么跳跃是对的？】
//   从 start 出发，在位置 j 首次 curSum < 0。
//   对于 start < k ≤ j，cumulative(start→k-1) ≥ 0（否则 j 不是首次变负）。
//   cumulative(k→j) = cumulative(start→j) - cumulative(start→k-1) < 0
//   所以从 k 出发也到不了 j。
//
// 【为什么只遍历 [0, n-1] 就够了？】
//   totalSum ≥ 0 保证了全程油量充足。
//   从 start 到 n-1 的正余量足够覆盖 0 到 start-1 的亏损。
//
// 【模拟示例】
// gas  = [1, 2, 3, 4, 5]
// cost = [3, 4, 5, 1, 2]
// diff = [-2,-2,-2, 3, 3]
//
// i=0: curSum=-2 < 0 → start=1, curSum=0
// i=1: curSum=-2 < 0 → start=2, curSum=0
// i=2: curSum=-2 < 0 → start=3, curSum=0
// i=3: curSum= 3 ≥ 0 → 继续
// i=4: curSum= 6 ≥ 0 → 继续
// totalSum=0 ≥ 0 → 返回 start=3 ✓
// ============================================================
class Solution2 {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int n = gas.size();
        int totalSum = 0;   // 全程净油量，判断可行性
        int curSum = 0;     // 当前候选起点出发的累计
        int start = 0;      // 候选起点

        for (int i = 0; i < n; i++) {
            int diff = gas[i] - cost[i];
            totalSum += diff;
            curSum += diff;

            if (curSum < 0) {
                // 从 start 到 i 走不通
                // start ~ i 之间的所有点都不行（贪心跳跃）
                start = i + 1;  // 注意是 i+1 不是 i
                curSum = 0;     // 从新起点重新累计
            }
        }

        // totalSum < 0: 总油量不够，无论从哪出发都不行
        return totalSum >= 0 ? start : -1;
    }
};

// ============================================================
// 解法3: 最低点法 — 找累计净油量的全局最低点
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 换一种理解方式：把 diff[i] = gas[i] - cost[i] 的前缀和画成折线图。
//
// gas  = [1, 2, 3, 4, 5]
// cost = [3, 4, 5, 1, 2]
// diff = [-2,-2,-2, 3, 3]
//
// 从 index 0 开始的累计前缀和:
//
//  0 ─┐
//     │ -2 ─┐
//            │ -4 ─┐
//                  │ -6 ─┐         ← 最低点 minIdx=2
//                        │ -3 ─┐
//                              │ 0
//
// 从最低点的下一个位置出发，等于把折线图"旋转"，
// 使得起点处的值变成 0，全程折线都被抬高到 ≥ 0。
//
// 为什么？最低点是全程"最困难"的位置，从它后面出发，
// 前半段积累正油量，正好能扛过这个最低谷。
// ============================================================
class Solution3 {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int n = gas.size();
        int totalSum = 0;
        int minSum = INT_MAX;
        int minIdx = 0;

        for (int i = 0; i < n; i++) {
            totalSum += gas[i] - cost[i];
            if (totalSum < minSum) {
                minSum = totalSum;
                minIdx = i;         // 记录最低点
            }
        }

        if (totalSum < 0) return -1;   // 总油量不够

        // 最低点的下一个位置就是起点
        // 取模处理 minIdx = n-1 的边界情况
        return (minIdx + 1) % n;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间  | 空间 | 核心思想              | 面试推荐 |
// |----------|-------|------|-----------------------|----------|
// | 暴力模拟 | O(n²) | O(1) | 枚举+验证             | 口述     |
// | 贪心跳跃 | O(n)  | O(1) | 失败则跳到下一个候选  | ⭐首选   |
// | 最低点法 | O(n)  | O(1) | 找全局最低点          | 加分项   |
//
// 贪心跳跃和最低点法本质是同一件事的两种表达：
//   - 贪心跳跃：自动跳过不合法的区间，最终剩下的 start 就是最低点后面
//   - 最低点法：显式找到最低点，取 minIdx+1
//
// ============================================================
// 【易错点】
//
// 1. 忘记判断 totalSum < 0:
//    ✗ return start;  // 无解时也返回了一个值
//    ✓ return totalSum >= 0 ? start : -1;
//
// 2. 跳跃时 start 设成 i 而不是 i+1:
//    ✗ start = i;     // i 本身的 diff 已经被算进 curSum 了，它不能作为新起点
//    ✓ start = i + 1; // 从 i 的下一个站重新开始
//
// 3. 最低点法忘记取模:
//    ✗ return minIdx + 1;       // minIdx = n-1 时返回 n，越界
//    ✓ return (minIdx + 1) % n; // 正确处理环形边界
//
// 4. curSum 重置时忘了赋 0:
//    ✗ start = i + 1; // 漏掉 curSum = 0
//    ✓ start = i + 1; curSum = 0;
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力解的瓶颈在哪？怎么优化？
//   → 每次失败只移一步，重复验证了中间的站点。
//   → 利用"失败区间内所有点都不行"的性质，跳跃前进，O(n²) → O(n)。
//
// Q2: 请严格证明"start~j 之间的点都不能作为起点"。
//   → 设从 start 到 k-1 的累计 ≥ 0（否则 j 不是首次变负点）。
//   → 从 k 到 j 的累计 = (start到j的累计) - (start到k-1的累计) < 0 - 0 < 0。
//   → 所以从 k 出发也在 j 之前就会失败。
//
// Q3: 只遍历了一半（start 到 n-1），怎么保证 0 到 start-1 也能走通？
//   → totalSum ≥ 0 意味着：(start→n-1 的正余量) + (0→start-1 的净油量) ≥ 0。
//   → 从 start 出发时累积了足够的正余量来覆盖前半段可能的亏损。
//
// Q4: 这道题和最大子数组和（LC 53）有什么关系？
//   → 两者都用"累计为负则重置"的思想。LC53 的 Kadane 算法：
//     curMax = max(nums[i], curMax + nums[i])，负数时重置。
//   → 本题：curSum < 0 时重置起点。核心都是"负前缀不值得保留"。
//
// ============================================================
