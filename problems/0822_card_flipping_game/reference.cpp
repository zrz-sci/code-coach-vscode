/*
 * 【题目本质】
 * 给定 N 张双面卡片，可以任意翻转。找最小的"good"数字：
 * 该数字至少出现在某张卡片的背面，且不出现在任何卡片的正面。
 * 关键洞察：只有正反面相同的卡片上的数字绝对不可能成为 good。
 *
 * 【解法总览】
 * Solution1: HashSet 排除法（推荐）—— 排除正反相同的数字，剩余取最小
 * Solution2: 暴力枚举验证 —— 逐个候选值验证可行性
 */

// ===================== Solution1: HashSet 排除法（推荐） =====================
// 思路：
// 1. 正反面相同的卡片 → 该数字永远无法隐藏，加入 forbidden
// 2. 遍历所有 fronts[i] 和 backs[i]，跳过 forbidden 中的数字，取最小值
// 时间 O(N)，空间 O(N)
class Solution1 {
public:
    int flipgame(vector<int>& fronts, vector<int>& backs) {
        int n = fronts.size();
        unordered_set<int> forbidden;

        // 找出正反面数字相同的卡片 —— 这些数字不可能成为 good
        for (int i = 0; i < n; i++) {
            if (fronts[i] == backs[i]) {
                forbidden.insert(fronts[i]);
            }
        }

        // 在所有数字中找不在 forbidden 中的最小值
        int result = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!forbidden.count(fronts[i])) {
                result = min(result, fronts[i]);
            }
            if (!forbidden.count(backs[i])) {
                result = min(result, backs[i]);
            }
        }

        return result == INT_MAX ? 0 : result;
    }
};

// ===================== Solution2: 暴力枚举验证 =====================
// 思路：枚举所有出现的数字，对每个数字检查是否存在正反面都等于它的卡片
// 时间 O(N^2)，空间 O(1)
class Solution2 {
public:
    int flipgame(vector<int>& fronts, vector<int>& backs) {
        int n = fronts.size();
        int result = INT_MAX;

        auto canBeGood = [&](int val) -> bool {
            for (int i = 0; i < n; i++) {
                // 如果某张卡片正反面都是 val，则 val 无法成为 good
                if (fronts[i] == val && backs[i] == val) {
                    return false;
                }
            }
            return true;
        };

        for (int i = 0; i < n; i++) {
            if (canBeGood(fronts[i])) {
                result = min(result, fronts[i]);
            }
            if (canBeGood(backs[i])) {
                result = min(result, backs[i]);
            }
        }

        return result == INT_MAX ? 0 : result;
    }
};

/*
 * 【解法对比】
 * ┌────────────┬──────────────┬──────────────┬──────────────────────────┐
 * │ 解法       │ 时间复杂度   │ 空间复杂度   │ 适用场景                 │
 * ├────────────┼──────────────┼──────────────┼──────────────────────────┤
 * │ HashSet    │ O(N)         │ O(N)         │ 最优，一次遍历即可       │
 * │ 暴力枚举   │ O(N^2)       │ O(1)         │ 面试中快速实现验证思路   │
 * └────────────┴──────────────┴──────────────┴──────────────────────────┘
 *
 * 【易错点】
 * 1. 只有 fronts[i] == backs[i] 时才排除该数字。不能因为数字出现在某张卡片
 *    的正面就排除它——可以翻转那张卡片。
 * 2. 候选数字来源于 fronts 和 backs 的并集，不能只看一面。
 * 3. 无解时返回 0 而非 -1（题目明确要求）。
 * 4. INT_MAX 作为哨兵值，最终需要判断是否更新过。
 *
 * 【面试追问】
 * Q1: 如果要返回所有 good 数字而非最小值？
 *     → 收集所有不在 forbidden 中且出现在某张卡片上的数字，去重后排序返回
 *
 * Q2: 如果每张卡片有三面（正面、背面、侧面），如何扩展？
 *     → forbidden 条件变为三面都相同。候选来源扩展为三个数组。逻辑不变。
 *
 * Q3: 如果限制最多翻转 K 张卡片？
 *     → 需要贪心或 DP：对每个候选值，计算使其成为 good 的最少翻转次数，
 *       取 <= K 的候选中的最小值。
 *
 * Q4: 这道题的核心贪心思想是什么？
 *     → 翻转操作没有限制，所以唯一的限制来自不可翻转的情况（正反面相同）。
 *       排除这些"硬约束"后，答案就是剩余数字的最小值。
 */
