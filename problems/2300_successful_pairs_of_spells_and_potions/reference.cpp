/*
 * LeetCode 2300: 咒语和药水的成功对数
 * Successful Pairs of Spells and Potions
 *
 * 难度: Medium
 * 标签: Array, Two Pointers, Binary Search, Sorting
 *
 * =============================================
 * 解法一 (推荐): 排序 + 二分查找
 * =============================================
 *
 * 核心思路:
 *   对 potions 排序后, 对每个 spell 计算需要的最小 potion:
 *     minPotion = ceil(success / spell)
 *   然后在排序后的 potions 中二分找第一个 >= minPotion 的位置。
 *   从该位置到末尾都是成功对, 个数 = m - idx。
 *
 * 时间: O((m + n) * log m)
 * 空间: O(log m) (排序用栈)
 *
 * 二分收缩过程 (spell=5, potions=[1,2,3,4,5], success=7):
 *
 *   minPotion = ceil(7/5) = 2
 *
 *   Step 0: lo=0, hi=5
 *           [1, 2, 3, 4, 5]
 *            ^              ^
 *           lo              hi
 *
 *   Step 1: mid=2, potions[2]=3 >= 2 → hi=2
 *           [1, 2, 3]
 *            ^     ^
 *           lo     hi
 *
 *   Step 2: mid=1, potions[1]=2 >= 2 → hi=1
 *           [1, 2]
 *            ^  ^
 *           lo  hi
 *
 *   Step 3: mid=0, potions[0]=1 < 2 → lo=1
 *           [_, 2]
 *               ^
 *             lo=hi=1
 *
 *   结果: idx=1, 成功对 = 5-1 = 4 ✓
 */
class Solution {
public:
    vector<int> successfulPairs(vector<int>& spells,
                                vector<int>& potions,
                                long long success) {
        // Step 1: 排序 potions (建立单调性)
        sort(potions.begin(), potions.end());
        int m = potions.size();

        vector<int> result;
        result.reserve(spells.size());

        for (int spell : spells) {
            // Step 2: 计算需要的最小 potion 值
            // 需要 spell * potion >= success
            // 即 potion >= success / spell (向上取整)
            long long minPotion = (success + spell - 1) / spell;

            // Step 3: 二分找第一个 >= minPotion 的位置
            int idx = (int)(lower_bound(potions.begin(),
                                        potions.end(),
                                        minPotion)
                           - potions.begin());

            // Step 4: 从该位置到末尾都是成功对
            result.push_back(m - idx);
        }

        return result;
    }
};


/*
 * =============================================
 * 解法一变体: 手写二分 (面试常考)
 * =============================================
 *
 * 面试中可能要求不使用 STL 的 lower_bound,
 * 手写二分查找第一个 >= target 的位置。
 *
 * 二分模板: 左闭右开 [lo, hi)
 *   lo = 第一个 >= target 的位置
 *   hi = 搜索范围的右边界 (不包含)
 *
 * 时间: O((m + n) * log m)
 * 空间: O(log m)
 */
class Solution_ManualBS {
public:
    vector<int> successfulPairs(vector<int>& spells,
                                vector<int>& potions,
                                long long success) {
        sort(potions.begin(), potions.end());
        int m = potions.size();
        vector<int> result;

        for (int spell : spells) {
            long long minPotion = (success + spell - 1) / spell;

            // 手写二分: 找第一个 >= minPotion 的位置
            int lo = 0, hi = m;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (potions[mid] < minPotion) {
                    lo = mid + 1;  // 太小, 搜索右半
                } else {
                    hi = mid;      // 可能是答案, 搜索左半 (含 mid)
                }
            }
            // lo == hi == 第一个 >= minPotion 的位置

            result.push_back(m - lo);
        }

        return result;
    }
};


/*
 * =============================================
 * 解法二: 排序两个数组 + 双指针
 * =============================================
 *
 * 思路:
 *   1. 将 (spell, 原始下标) 按 spell 降序排序
 *   2. potions 升序排序
 *   3. spell 从大到小扫描, minPotion 从小到大递增
 *      → 指针 j 只会从左向右移动 (单调指针)
 *
 *   spell 越大 → minPotion = ceil(success/spell) 越小
 *   → 更多 potions 满足
 *
 *   spell 越小 → minPotion 越大 → 更少 potions 满足
 *   → j 只需要向右移动跳过不满足的 potion
 *
 * 推演 (spells=[5,1,3], potions=[1,2,3,4,5], success=7):
 *   排序后 spells: [(5,idx0), (3,idx2), (1,idx1)]
 *   排序后 potions: [1, 2, 3, 4, 5]
 *
 *   spell=5: minPotion=2, j从0开始, potions[0]=1<2 → j=1
 *            potions[1]=2>=2 → 停. 答案[0]=5-1=4
 *   spell=3: minPotion=3, j从1继续, potions[1]=2<3 → j=2
 *            potions[2]=3>=3 → 停. 答案[2]=5-2=3
 *   spell=1: minPotion=7, j从2继续, 3<7→j=3, 4<7→j=4, 5<7→j=5
 *            j=5=m → 停. 答案[1]=5-5=0
 *
 * 时间: O(n log n + m log m) — 排序是瓶颈
 * 空间: O(n)
 */
class Solution2 {
public:
    vector<int> successfulPairs(vector<int>& spells,
                                vector<int>& potions,
                                long long success) {
        int n = spells.size(), m = potions.size();

        // (spell值, 原始下标) 按 spell 降序排序
        vector<pair<int, int>> sorted_spells(n);
        for (int i = 0; i < n; i++) {
            sorted_spells[i] = {spells[i], i};
        }
        sort(sorted_spells.begin(), sorted_spells.end(),
             [](auto& a, auto& b) { return a.first > b.first; });

        // potions 升序排序
        sort(potions.begin(), potions.end());

        vector<int> result(n);
        int j = 0; // 单调指针

        for (auto& [spell, idx] : sorted_spells) {
            long long minPotion = (success + spell - 1) / spell;

            // j 只会向右移动 (因为 spell 递减 → minPotion 递增)
            while (j < m && potions[j] < minPotion) {
                j++;
            }
            result[idx] = m - j;
        }

        return result;
    }
};


/*
 * =============================================
 * 向上取整的数学证明
 * =============================================
 *
 * 需要: potion >= success / spell  (精确除法)
 * 等价: potion >= ceil(success / spell)  (因为 potion 是整数)
 *
 * 整数向上取整公式:
 *   ceil(a / b) = (a + b - 1) / b   (a, b > 0)
 *
 * 证明:
 *   设 a = q*b + r, 其中 0 <= r < b
 *   floor(a/b) = q
 *   ceil(a/b) = q + (r > 0 ? 1 : 0)
 *   (a + b - 1) / b = (q*b + r + b - 1) / b
 *                    = q + (r + b - 1) / b
 *   当 r = 0: (0 + b - 1) / b = (b-1)/b = 0 → q + 0 = q ✓
 *   当 r > 0: (r + b - 1) / b, 分子 >= b → 商 >= 1
 *             但 r < b → 分子 < 2b → 商 = 1 → q + 1 ✓
 *
 * =============================================
 * 测试验证
 * =============================================
 *
 * 示例 1: spells=[5,1,3], potions=[1,2,3,4,5], success=7
 *   排序后 potions = [1,2,3,4,5]
 *   spell=5: minPotion=(7+4)/5=2, lower_bound→idx=1, 5-1=4 ✓
 *   spell=1: minPotion=(7+0)/1=7, lower_bound→idx=5, 5-5=0 ✓
 *   spell=3: minPotion=(7+2)/3=3, lower_bound→idx=2, 5-2=3 ✓
 *   输出: [4, 0, 3] ✓
 *
 * 示例 2: spells=[3,1,2], potions=[8,5,8], success=16
 *   排序后 potions = [5,8,8]
 *   spell=3: minPotion=(16+2)/3=6, lower_bound(6)→idx=1, 3-1=2 ✓
 *   spell=1: minPotion=(16+0)/1=16, lower_bound(16)→idx=3, 3-3=0 ✓
 *   spell=2: minPotion=(16+1)/2=8, lower_bound(8)→idx=1, 3-1=2 ✓
 *   输出: [2, 0, 2] ✓
 *
 * 边界:
 *   spell=100000, potion=100000, success=10^10:
 *     100000 * 100000 = 10^10 >= 10^10 ✓ (刚好满足)
 *   spell=1, success=10^10:
 *     minPotion = 10^10, 所有 potion <= 10^5 < 10^10 → 0对 ✓
 */


// =====================================================
// 总结
// =====================================================
/*
 * ── 解法对比 ──
 *
 * | 解法                    | 时间             | 空间      | 推荐度 |
 * |-------------------------|------------------|-----------|--------|
 * | 排序+lower_bound (推荐) | O((n+m) log m)   | O(log m)  | ★★★ 面试首选 |
 * | 排序+手写二分 (变体)    | O((n+m) log m)   | O(log m)  | ★★★ 展示基本功 |
 * | 双数组排序+双指针       | O(n log n+m log m)| O(n)     | ★★  巧妙但复杂 |
 *
 * 排序+二分: 最直观, potions 排序后每个 spell 独立二分, 互不干扰。
 * 双指针:    利用 spell 降序 → minPotion 递增的单调性, 指针不回退;
 *            渐近复杂度更优 (无 n*log m 项), 但需要额外空间存原始下标。
 *
 * ── 易错点 (具体bug) ──
 *
 * 1. 向上取整公式写错:
 *    BUG: minPotion = success / spell; (整数除法向下取整)
 *    → spell=3, success=7 时 minPotion=2 (实际应为 3)
 *    → 会多计入不满足条件的 potion, 导致答案偏大。
 *    正确: minPotion = (success + spell - 1) / spell;
 *
 * 2. 溢出: spell * potion 可达 10^5 * 10^5 = 10^10, 超过 int 范围。
 *    BUG: 在二分判断中写 if ((int)(spell * potions[mid]) >= success)
 *    → 溢出后结果不可预测。
 *    正确: 用 long long, 或改为比较 potion >= minPotion 避免乘法。
 *
 * 3. lower_bound 搜索的是 minPotion 而非 success:
 *    BUG: lower_bound(potions.begin(), potions.end(), success)
 *    → 直接搜 success 忽略了 spell 的影响, 完全错误。
 *
 * 4. 双指针法忘记按原始下标写回结果:
 *    BUG: result.push_back(m - j) 而非 result[idx] = m - j
 *    → spells 已排序, 顺序与原始不同, 直接 push_back 会错位。
 *
 * ── 面试追问链 ──
 *
 * Q1: 如果 potions 数组非常大无法排序 (流式数据), 怎么办?
 * A1: 可以用大小为 k 的 min-heap 维护最大的 k 个 potions,
 *     但精确查询仍需完整数据。另一种思路: 对值域二分,
 *     用 count 统计 >= threshold 的 potion 个数。
 *
 * Q2: 如果要求 spell * potion 恰好等于 success (而非 >=), 怎么做?
 * A2: 对每个 spell, 检查 success 是否能被 spell 整除;
 *     若能, 在 potions 中二分查找 success/spell 是否存在。
 *     用 equal_range 或 lower_bound + 值比较。O((n+m) log m)。
 *
 * Q3: 如果 spells 和 potions 都允许动态增删, 如何高效查询?
 * A3: 用平衡BST (如 std::multiset) 存储 potions,
 *     查询时 lower_bound 找 minPotion 的位置, 用 distance 或
 *     order-statistics tree 计算后缀长度。
 *     插入/删除 O(log m), 查询 O(log m)。
 */
