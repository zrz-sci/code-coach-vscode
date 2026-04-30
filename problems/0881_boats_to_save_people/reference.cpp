// ============================================================
// LeetCode 881: Boats to Save People (救生艇)
//
// 【解法】排序 + 双指针 (贪心) — O(n log n) / O(1)
//
// 每艘船最多两人, 总重不超过 limit
// 排序后, 最重 + 最轻尝试配对
// 能配对 → 两人同船; 不能 → 最重的人独占一船
// ============================================================

/*
 * 贪心配对过程 (people = [3,2,2,1], limit = 3):
 *
 * 排序后: [1, 2, 2, 3]
 *          L           R
 *
 * Round 1: people[0]+people[3] = 1+3 = 4 > 3
 *          → 3 独占一船, R--
 *          boats=1
 *          [1, 2, 2, |3|]
 *           L     R
 *
 * Round 2: people[0]+people[2] = 1+2 = 3 <= 3
 *          → 1 和 2 配对同船, L++, R--
 *          boats=2
 *          [|1|, 2, |2|, |3|]
 *                LR
 *
 * Round 3: L == R, 2 独占一船
 *          boats=3
 *
 * 答案: 3 艘船
 */

class Solution {
public:
    int numRescueBoats(vector<int>& people, int limit) {
        // Step 1: 排序, 让最轻和最重的人对齐
        sort(people.begin(), people.end());

        // Step 2: 双指针从两端向内逼近
        int left = 0, right = people.size() - 1;
        int boats = 0;

        while (left <= right) {
            // 尝试配对最轻的人和最重的人
            if (people[left] + people[right] <= limit) {
                left++;   // 最轻的人上船了, 不再参与配对
            }
            // 无论是否配对成功, 最重的人都上船 (独占或配对)
            right--;
            boats++;
        }

        return boats;
    }
};


// ============================================================
// 【变体】计数排序优化 — O(n + W) / O(W)
//
// 当 limit (权重范围 W) 较小时, 用计数排序代替比较排序
// 可以将排序从 O(n log n) 优化到 O(n + W)
// ============================================================

/*
 * 适用场景: 1 <= people[i] <= limit <= 30000
 * 计数排序后用相同的双指针逻辑
 */

class Solution_CountSort {
public:
    int numRescueBoats(vector<int>& people, int limit) {
        // 计数排序
        vector<int> count(limit + 1, 0);
        for (int w : people) count[w]++;

        // 重建排序数组
        vector<int> sorted;
        sorted.reserve(people.size());
        for (int w = 1; w <= limit; w++) {
            for (int i = 0; i < count[w]; i++) {
                sorted.push_back(w);
            }
        }

        // 双指针 (同主解法)
        int left = 0, right = sorted.size() - 1;
        int boats = 0;

        while (left <= right) {
            if (sorted[left] + sorted[right] <= limit) {
                left++;
            }
            right--;
            boats++;
        }

        return boats;
    }
};


// ============================================================
// 【变体】不用排序数组, 直接在计数数组上双指针
//
// 更节省空间: 不需要重建排序数组
// 直接在 count[] 上用 left/right 指针
// ============================================================

class Solution_CountDirect {
public:
    int numRescueBoats(vector<int>& people, int limit) {
        vector<int> count(limit + 1, 0);
        for (int w : people) count[w]++;

        int left = 1, right = limit;
        int boats = 0;

        while (left <= right) {
            // 跳过计数为 0 的桶
            while (left <= right && count[left] == 0) left++;
            while (left <= right && count[right] == 0) right--;

            if (left > right) break;

            if (left == right) {
                // 同一个桶里的人: 看能否自己配对
                if (count[left] == 1) {
                    boats++;  // 只剩一个, 独占
                    count[left]--;
                } else if (left + left <= limit) {
                    // 两个相同重量的人可以配对
                    boats++;
                    count[left] -= 2;
                } else {
                    // 两个相同重量的人不能配对, 各自独占
                    boats++;
                    count[left]--;
                }
            } else {
                // 不同桶: 尝试配对
                if (left + right <= limit) {
                    boats++;
                    count[left]--;
                    count[right]--;
                } else {
                    boats++;
                    count[right]--;  // 重的人独占
                }
            }
        }

        return boats;
    }
};


// ============================================================
// 贪心正确性证明 (交换论证)
// ============================================================
//
// 假设最优解 OPT 中最轻的人 A 与某人 X 配对, 而非最重的人 B.
// 我们的贪心 G 让 A 与 B 配对 (如果 A+B <= limit).
//
// 因为 B >= X (B 是最重的), 且 A+B <= limit,
// 所以 A+X <= limit (X <= B).
//
// 交换: 在 OPT 中把 A-X 配对改成 A-B 配对:
// - B 原来是独占或与 Y 配对
// - 独占情况: X 变成独占或与 Y 配对, 船数不增加
// - B-Y 配对情况: X 与 Y 配对或各自独占, 船数不增加
//
// 因此 G 的船数 <= OPT 的船数.


// ============================================================
// 复杂度分析
// ============================================================
//
// 主解法 (排序 + 双指针):
//   时间: O(n log n) — 排序是瓶颈, 双指针 O(n)
//   空间: O(1) — 原地排序, 常数额外空间
//
// 计数排序变体:
//   时间: O(n + W) — W = limit, 计数排序 + 双指针
//   空间: O(W) — 计数数组


// ============================================================
// 总结
// ============================================================
//
// 【解法对比】
// ┌──────────────────┬────────────┬────────┬──────────────────────────┐
// │ 解法              │ 时间       │ 空间   │ 适用场景                 │
// ├──────────────────┼────────────┼────────┼──────────────────────────┤
// │ 1. 排序+双指针    │ O(nlogn)   │ O(1)   │ 面试首选, 代码简洁       │
// │ 2. 计数排序重建   │ O(n+W)     │ O(W)   │ limit 范围小时更快       │
// │ 3. 计数数组直接   │ O(n+W)     │ O(W)   │ 省去重建数组, 但代码复杂 │
// └──────────────────┴────────────┴────────┴──────────────────────────┘
//
// 【易错点】
// 1. 每艘船最多 2 人 → 不是背包问题, 不能塞 3+ 人
// 2. left == right 时表示只剩一人, 也需要一艘船 → while 条件是 <=
// 3. 无论是否配对, right-- 和 boats++ 都要执行 → 最重的人必须上船
// 4. 计数数组直接做双指针时, left==right 的桶内配对逻辑容易遗漏
// 5. 贪心正确性依赖"最多 2 人" → 若允许 k 人则需不同策略(FFD/BFD)
//
// 【面试追问】
// Q1: 如果每艘船可以坐 k 个人 (k >= 2), 怎么办?
//     → 变成 Bin Packing 问题 (NP-Hard), 用 First Fit Decreasing 近似.
//       排序后从重到轻, 每个人尝试放入已有最满的船.
//
// Q2: 如果要最小化总重量差 (尽量让每艘船装满), 怎么改?
//     → 排序后双指针仍然有效: 最轻+最重已是最优配对方式.
//       每艘船的"浪费" = limit - (people[L]+people[R]), 双指针使浪费最小化.
//
// Q3: 如果 people 数组已经有序, 能否做到 O(n)?
//     → 可以! 跳过排序, 直接双指针, O(n) 时间 O(1) 空间.
